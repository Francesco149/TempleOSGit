/*
See [::/LT/Doc/MultiTasking.TXZ] LoseThos MultiTasking.

Tasks are ordered in a que based
on window front-to-back order with the
window mgr as the head, which
paints the wallpaper.  The scheduler
goes in a circle, checking if a task is
ready and swapping-it-in if so.
Swapping just involves storing and
restoring registers -- no disk I/O for
virtual memory and no addr map
changes.  Tasks can be switched in half
a microsecond.

The scheduler checks if a task is
waiting for a certain time or waiting
on a message and skips it if it's not
ready.  A task runs until it
voluntarily yields ctrl with a call
to [C:/LT/OSMain/Scheduler.CPZ,371] Yield or gets preempted by the
timer interrupt.  See [C:/LT/OSMain/Interrupts.CPZ,16] SYS_TIMER_HANDLER.
The timer freqency is [C:/LT/OSMain/Adam1a.HPZ,489] JIFFY_FREQ.  Tasks
can ask not to be preempted with
[C:/LT/OSMain/Adam1b.HPZ,94] Preempt(ON/OFF).  Tasks waiting on I/O
often loop, checking for a stat and
swapping-out.  This does not really
degrade performance, though it pegs the
CPU Load.

The scheduler checks for a few critical
key-strokes such as <CTRL-ALT-X>, which
kills a task; <CTRL-ALT-DEL>, which reboots;
<CTRL-ALT-T>, which changes focus and <CTRL-ALT-C>,
which breaks execution of a program.

Each core has it's own circular task
que.  For AP processors, they have a
"Seth" task which stays in a loop
waiting for jobs or requests to spawn
tasks.  See [C:/LT/OSMain/MultiProc.CPZ,280] MPSethServerTask().

All cores have an idle task.  If the
scheduler determined an IRQ opportunity
hasn't happened, the scheduler swaps in
the idle task(enabling IRQs).

*/

asm {
USE64
// ************************************
SYS_IDLE_START::
SYS_SAVE_CONTEXT::
//OUT:	RSI=FS
	PUSH	RSI
	PUSHFD
	XOR	RSI,RSI
	MOV	RSI,FS:TaskStruct.addr[RSI]
	POP	U64 TaskStruct.rflags[RSI]
	POP	U64 TaskStruct.rsi[RSI]
	MOV	U64 TaskStruct.rax[RSI],RAX

/*
We divert the stk to the TaskStruct memory
and push onto it and divert it back.
It's a little faster.
*/
	MOV	RAX,RSP
	LEA	RSP,U64 TaskStruct.r15+8[RSI]
	PUSH	R15
	PUSH	R14
	PUSH	R13
	PUSH	R12
	PUSH	R11
	PUSH	R10
	PUSH	R9
	PUSH	R8
	PUSH	RDI
	PUSH	RBP
	PUSH	RBX
	PUSH	RDX
	PUSH	RCX
	MOV	RSP,RAX

	RDTSC
	SHL	RDX,32
	ADD	RAX,RDX
	SUB	RAX,U64 TaskStruct.time_slice_start[RSI]
	ADD	U64 TaskStruct.total_time[RSI],RAX

	MOV	RAX,U64 TaskStruct.fpu_mmx[RSI]
	FXSAVE	U64 [RAX]

	MOV	RDX,U64 TaskStruct.bpt_list[RSI]
@@05:	OR	RDX,RDX
	JZ	@@10
	MOV	RDI,U64 SysBpt.addr[RDX]
	MOV	AL,U8 SysBpt.val[RDX]
	MOV	U8 [RDI],AL
	MOV	RDX,U64 SysBpt.next[RDX]
	JMP	@@05
@@10:	LOCK
	BTR	U64 TaskStruct.task_flags[RSI],TASKf_CURRENTLY_RUNNING
	RET
// ************************************
SYS_RESTORE_CONTEXT::
	XOR	RAX,RAX
	INC	U64 GS:CPUStruct.swap_cnter[RAX]
	MOV	RSI,FS:TaskStruct.addr[RAX]
	BT	U32 TaskStruct.rflags[RSI],RFLAGS_INTERRUPTS
	JNC	@@05
	LOCK	//We will be enabling IRQ's
	BTS	U32 GS:CPUStruct.cpu_flags[RAX],CPUf_HAD_IRQ_OPPORTUNITY
@@05:	LOCK
	BTS	U64 TaskStruct.task_flags[RSI],TASKf_CURRENTLY_RUNNING
	BT	U64 TaskStruct.task_flags[RSI],TASKf_DISABLE_BPTS
	JC	@@15
	MOV	RDX,U64 TaskStruct.bpt_list[RSI]
@@10:	OR	RDX,RDX
	JZ	@@15
	MOV	RDI,U64 SysBpt.addr[RDX]
	MOV	U8 [RDI],OC_BPT
	MOV	RDX,U64 SysBpt.next[RDX]
	JMP	@@10

@@15:	INC	U64 TaskStruct.swap_cnter[RSI]

	MOV	RAX,U64 TaskStruct.fpu_mmx[RSI]
	FXRSTOR	U64 [RAX]

	RDTSC
	SHL	RDX,32
	ADD	RAX,RDX
	MOV	U64 TaskStruct.time_slice_start[RSI],RAX

	MOV	RAX,RSP
	LEA	RSP,U64 TaskStruct.rcx[RSI]
	POP	RCX
	POP	RDX
	POP	RBX
	POP	RBP
	POP	RDI
	POP	R8
	POP	R9
	POP	R10
	POP	R11
	POP	R12
	POP	R13
	POP	R14
	POP	R15
	MOV	RSP,RAX

	MOV	RAX,U64 TaskStruct.rax[RSI]
	PUSH	GDT_ZERO_DS_SEL
	PUSH	U64 TaskStruct.rsp[RSI]
	PUSH	U64 TaskStruct.rflags[RSI]
	PUSH	GDT_CS64_SEL
	PUSH	U64 TaskStruct.rip[RSI]
	MOV	RSI,U64 TaskStruct.rsi[RSI]
	IRET
// ************************************
END_RSI_TASK:
	MOV	RAX,RSI
	CALL	SET_FS_BASE
SYS_TASK_END_NOW::
	CALL	&TaskEnd
	MOV	RSI,RAX
	CALL	SET_FS_BASE
	JMP	I32 RESTORE_RSI_TASK_WITH_KEY_CHECK

RESTORE_RSI_NO_PREEMPT_TASK:
	BT	U32 [SYS_CTRL_ALT_FLAGS],SYSf_CTRL_ALT_DEL
	JC	I32 HANDLE_SWAP_KEY_EVENT
	XOR	RAX,RAX
	CMP	U64 GS:CPUStruct.idle_task[RAX],RSI	//IDLE TASK HAS PREEMPT-OFF
	JNE	RESTORE_RSI_TASK_NO_KEY_CHECK
	JMP	I32 SWAP_IN_SETH_TASK

SYS_SAVE_CONTEXT_YIELD::
	PUSHFD
	PUSH	RAX
	MOV	RAX,SYS_SEMAS
	BT	U32 SYS_SEMA_SINGLE_USER*SEMA_STRUCT_SIZE[RAX],0
	JC	@@05
	POP	RAX

	CLI
	PUSH	RSI
	XOR	RSI,RSI
	MOV	RSI,FS:TaskStruct.addr[RSI]
	CMP	U64 TaskStruct.next_task[RSI],RSI
	JNE	@@15

	PUSH	RAX
	MOV	RAX,U64 [SYS_JIFFIES]
	CMP	U64 TaskStruct.wake_jiffy[RSI],RAX
	JA	@@10

	POP	RAX
	POP	RSI
	POPFD
	RET

@@05:	POP	RAX
	POPFD
	RET

@@10:	POP	RAX
@@15:	POP	RSI
	CALL	SYS_SAVE_CONTEXT

	MOV	EBX,U32 SYS_IDLE_PT
	MOV	U64 TaskStruct.rip[RSI],RBX
	POP	U64 TaskStruct.rflags[RSI]
	MOV	U64 TaskStruct.rsp[RSI],RSP

RESTORE_NEXT_RSI_TASK:
	MOV	RSI,U64 TaskStruct.next_task[RSI]

RESTORE_RSI_TASK_WITH_KEY_CHECK:
	TEST	U64 [SYS_CTRL_ALT_FLAGS],1<<SYSf_CTRL_ALT_DEL|
1<<SYSf_CTRL_ALT_TAB|1<<SYSf_CTRL_ALT_X|1<<SYSf_CTRL_ALT_C
	JNZ	HANDLE_SWAP_KEY_EVENT

RESTORE_RSI_TASK_NO_KEY_CHECK:
@@20:	BT	U64 TaskStruct.task_flags[RSI],TASKf_KILL_TASK
	JC	I32 END_RSI_TASK
	TEST	U64 TaskStruct.task_flags[RSI],1<<TASKf_AWAITING_MSG|1<<TASKf_SUSPENDED|1<<TASKf_PAUSED
	JNZ	@@25

	MOV	RAX,U64 [SYS_JIFFIES]
	CMP	U64 TaskStruct.wake_jiffy[RSI],RAX
	JA	@@25

	CMP	U32 TaskStruct.task_in_que_signature[RSI],TASK_IN_QUE_SIGNATURE_VAL
	JNE	SWAP_IN_SETH_TASK
	MOV	RAX,RSI
	CALL	SET_FS_BASE
	JMP	I32 SYS_RESTORE_CONTEXT

@@25:	MOV	RSI,U64 TaskStruct.next_task[RSI]
	XOR	RAX,RAX
	CMP	U64 GS:CPUStruct.seth_task[RAX],RSI
	JNE	@@20
	LOCK
	BTR	U32 GS:CPUStruct.cpu_flags[RAX],CPUf_HAD_IRQ_OPPORTUNITY
	JC	@@20
	MOV	RAX,U64 GS:CPUStruct.idle_task[RAX]
	MOV	RSP,U64 TaskStruct.stk[RAX]
	ADD	RSP,DFT_STK+TaskStk.stk_base	//Rst to top
	CALL	SET_FS_BASE
	STI
	HLT
	CLI

SWAP_IN_SETH_TASK:
	XOR	RAX,RAX
	MOV	RSI,GS:CPUStruct.seth_task[RAX]
	JMP	I32 RESTORE_RSI_TASK_WITH_KEY_CHECK

SYS_IDLE_PT::
	RET

SYS_IDLE_END::

HANDLE_SWAP_KEY_EVENT:
	MOV	RAX,RSI
	CALL	SET_FS_BASE
	XOR	RAX,RAX
	MOV	RAX,GS:CPUStruct.num[RAX]
	OR	RAX,RAX
	JNZ	I32 RESTORE_RSI_TASK_NO_KEY_CHECK

	MOV	EAX,U32 SYS_CTRL_ALT_FLAGS
	LOCK
	BTR	U32 [RAX],SYSf_CTRL_ALT_DEL
	JC	I32 &Reboot

	LOCK
	BTR	U32 [RAX],SYSf_CTRL_ALT_TAB
	JNC	@@05
	PUSH	RSI
	MOV	RSI,U64 [SYS_FOCUS_TASK]
	MOV	U64 [SYS_FOCUS_TASK],0
	CALL	FOCUS_RSI_NEXT_USER
	POP	RSI
	JMP	I32 RESTORE_FS_TASK_WITH_KEY_CHECK

@@05:	LOCK
	BTR	U32 [RAX],SYSf_CTRL_ALT_X
	JC	END_FOCUS_USER
	LOCK
	BTR	U32 [RAX],SYSf_CTRL_ALT_C
	JNC	I32 RESTORE_RSI_TASK_WITH_KEY_CHECK

BREAK_FOCUS_USER:
	MOV	RSI,U64 [SYS_FOCUS_TASK]
	OR	RSI,RSI
	JZ	SWAP_IN_SETH_TASK
	BT	U64 TaskStruct.task_flags[RSI],TASKf_LOCAL_USER
	JNC	I32 RESTORE_RSI_TASK_NO_KEY_CHECK
	LOCK
	BTR	U64 TaskStruct.task_flags[RSI],TASKf_BREAK_LOCKED
	JNC	@@10
	LOCK
	BTS	U64 TaskStruct.task_flags[RSI],TASKf_PENDING_BREAK
	JMP	I32 RESTORE_RSI_TASK_NO_KEY_CHECK

@@10:	MOV	RAX,&SysThrowBreak
	MOV	U64 TaskStruct.rip[RSI],RAX
	BT	U64 TaskStruct.task_flags[RSI],TASKf_BREAK_TO_SHIFT_ESC
	JC	@@15

//We do these two now, in case interrupt happens
	MOV	U64 TaskStruct.wake_jiffy[RSI],0
	PUSH	RSI
	CALL	&TaskRstAwaitingMsg

@@15:	JMP	I32 RESTORE_RSI_TASK_NO_KEY_CHECK

END_FOCUS_USER:
	MOV	RSI,U64 [SYS_FOCUS_TASK]
	MOV	U64 [SYS_FOCUS_TASK],0
	PUSH	RSI
	CALL	FOCUS_RSI_NEXT_USER
	POP	RSI
	OR	RSI,RSI
	JZ	I32 SWAP_IN_SETH_TASK
	MOV	RAX,RSI
	CALL	SET_FS_BASE
	BT	U64 TaskStruct.task_flags[RSI],TASKf_LOCAL_USER
	JNC	I32 RESTORE_RSI_TASK_WITH_KEY_CHECK
	LOCK
	BTS	U64 TaskStruct.task_flags[RSI],TASKf_KILL_TASK
	JMP	I32 END_RSI_TASK

RESTORE_FS_TASK_WITH_KEY_CHECK:
	XOR	RSI,RSI
	MOV	RSI,FS:TaskStruct.addr[RSI]
	JMP	I32 RESTORE_RSI_TASK_WITH_KEY_CHECK

FOCUS_RSI_NEXT_USER:
	OR	RSI,RSI
	JNZ	@@20
	MOV	ESI,U32 SYS_ADAM_TASK
@@20:	MOV	RDI,RSI
	MOV	RSI,U64 TaskStruct.next_task[RSI]
@@25:	BT	U64 TaskStruct.task_flags[ESI],TASKf_LOCAL_USER
	JNC	@@30
	CMP	U64 [SYS_FOCUS_TASK],RSI
	JE	@@30
	MOV	U64 [SYS_FOCUS_TASK],RSI
	MOV	RAX,U64 [SYS_EXTERN_TABLE]
	MOV	RAX,U64 EXT_WIN_TO_TOP*8[RAX]
	OR	RAX,RAX
	JZ	@@35
	PUSH	TRUE
	PUSH	RSI
	CALL	RAX
	JMP	@@35
@@30:	MOV	RSI,U64 TaskStruct.next_task[RSI]
	CMP	RSI,RDI
	JNE	@@25
	MOV	U64 [SYS_FOCUS_TASK],0
@@35:	RET

}

LTextern SYS_RESTORE_CONTEXT U0 RestoreContext();
LTextern SYS_SAVE_CONTEXT_YIELD U0 Yield();
LTextern SYS_IDLE_START U8 sys_idle_start;
LTextern SYS_IDLE_END	U8 sys_idle_end;
LTextern SYS_TASK_END_NOW U0 TaskEndNow();

U0 TaskQueIns(TaskStruct *task,TaskStruct *pred=NULL)
{
  TaskStruct *last;
  PushFD;
  Cli;
  if (!pred) pred=Fs;
  last=pred->last_task;
  last->next_task=pred->last_task=task;
  task->last_task=last;
  task->next_task=pred;
  task->task_in_que_signature=TASK_IN_QUE_SIGNATURE_VAL;
  PopFD;
}

U0 TaskQueRem(TaskStruct *task)
{
  TaskStruct *next,*last;
  PushFD;
  Cli;
  task->task_in_que_signature=0;
  next=task->next_task;
  last=task->last_task;
  last->next_task=next;
  next->last_task=last;
  PopFD;
}

U0 TaskQueInsChild(TaskStruct *task)
{
  TaskStruct *last,*pred;
  PushFD;
  Cli;
  pred=task->parent_task->last_child_task;
  last=pred->last_sibling_task;
  last->next_sibling_task=pred->last_sibling_task=task;
  task->last_sibling_task=last;
  task->next_sibling_task=pred;
  PopFD;
}
