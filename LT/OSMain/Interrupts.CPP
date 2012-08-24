asm {
USE64
SYS_MP_CRASH_HANDLER_ADDR:: //forward reference to work around compiler
	DU32	&MPCrash;

SYS_WAKE_HANDLER::
	PUSH	RAX
	XOR	RAX,RAX
	MOV	EAX,U32 LAPIC_EOI
	MOV	U32 [RAX],0
	POP	RAX
	IRET

//I_TIMER
SYS_TIMER_HANDLER::
	CALL	SYS_SAVE_CONTEXT		//PREEMPTIVE
	CLD
	MOV	RBP,RSP
	MOV	RBX,U64 [RBP]
	MOV	U64 TaskStruct.rip[RSI],RBX
	MOV	RAX,U64 16[RBP]
	MOV	U64 TaskStruct.rflags[RSI],RAX
	MOV	RAX,U64 24[RBP]
	MOV	U64 TaskStruct.rsp[RSI],RAX

	XOR	RAX,RAX
	MOV	RDI,U64 GS:CPUStruct.addr[RAX]

	LOCK
	INC	U64 CPUStruct.total_jiffies[RDI]

	BT	U64 TaskStruct.task_flags[RSI],TASKf_IDLE
	JC	@@05
	CMP	RBX,U32 SYS_IDLE_START
	JB	@@10
	CMP	RBX,U32 SYS_IDLE_END
	JA	@@10
@@05:	LOCK
	INC	U64 CPUStruct.idle_pt_hits[RDI]

@@10:	MOV	RAX,U64 CPUStruct.timer_irq_chain[RDI]
	OR	RAX,RAX
	JZ	@@15
	PUSH	RSI
	CALL	RAX
	JMP	@@20
@@15:	ADD	RSP,8
@@20:	CLI
	MOV	RAX,U64 CPUStruct.num[RDI]
	OR	RAX,RAX
	JZ	@@25

	XOR	RAX,RAX
	MOV	EAX,U32 LAPIC_EOI
	MOV	U32 [RAX],0
	JMP	@@30

@@25:	MOV	AL,0x20 		 //ACKNOWLEDGE INTERRUPT
	OUT	0x20,AL

@@30:	MOV	RAX,SYS_SEMAS
	BT	U32 SYS_SEMA_SINGLE_USER*SEMA_STRUCT_SIZE[RAX],0
	JC	@@35
	BT	U64 TaskStruct.task_flags[RSI],TASKf_PREEMPT
	JC	I32 RESTORE_NEXT_RSI_TASK
@@35:	JMP	I32 RESTORE_RSI_NO_PREEMPT_TASK

// ************************************
SYS_IRQ_FAULT::
	PUSH	RBX
	MOV	RBX,U64 8[RSP]
	PUSH	RAX
	XOR	RAX,RAX
	MOV	FS:U32 TaskStruct.fault_num[RAX],EBX
	POP	RAX
	POP	RBX
	ADD	RSP,8

	CALL	SYS_SAVE_CONTEXT

	XOR	RDX,RDX
	MOV	U64 TaskStruct.fault_err_code[RSI],RDX
	MOV	EDX,U32 TaskStruct.fault_num[RSI]
	BT	U64 [SYS_FAULT_ERR_CODE_BITMAP],RDX
	JNC	@@1
	POP	U64 TaskStruct.fault_err_code[RSI]

@@1:	MOV	RBP,RSP
	MOV	RAX,U64 [RBP]
	MOV	U64 TaskStruct.rip[RSI],RAX
	MOV	RAX,U64 16[RBP]
	MOV	U64 TaskStruct.rflags[RSI],RAX
	MOV	RAX,U64 24[RBP]
	MOV	U64 TaskStruct.rsp[RSI],RAX
	MOV	RSP,RAX
	MOV	RBP,TaskStruct.rbp[RSI]
	MOV	RSI,TaskStruct.rsi[RSI]
	CALL	&Fault2 //See [::/LT/OSMain/Dbg1a.CPZ,Fault2] Fault2
	JMP	I32 RESTORE_FS_TASK_WITH_KEY_CHECK

SYS_FAULT_ERR_CODE_BITMAP::
	DU32	0x00027D00,0,0,0,0,0,0,0;
}

U0 *GetIDTEntry(U64 irq,I64 cpu_num=-1)
{
  U64 result,*src;
  if (cpu_num<0) cpu_num=Gs->num;
  if (cpu_num>=mp_cnt&&cpu_num>Gs->num)
    throw(EXCEPT_MULTICORE,3);
  src=cpu_structs[cpu_num].idt(U8 *)+irq*16;
  result.u16[0]=*src(U16 *);
  src(U8 *)+=6;
  result.u16[1]=*src(U16 *)++;
  result.u32[1]=*src(U32 *);
  return result;
}

U0 *SetIDTEntry(U64 irq,U0 (*fp_new_handler)(),I64 cpu_num=-1,
    U64 type=IDTET_IRQ,U64 dpl=0)
{
//See [C:/LT/OSMain/Adam1a.HPZ,313] IDTET_IRQ
//See [::/LT/Demo/Lectures/InterruptDemo.CPZ] ::/LT/Demo/Lectures/InterruptDemo.CPZ
//See [::/LT/Demo/MultiCore/Interrupts.CPZ] ::/LT/Demo/MultiCore/Interrupts.CPZ
  U64 result,fp=fp_new_handler;
  U8 *dst;

  if (cpu_num<0) cpu_num=Gs->num;

  result=GetIDTEntry(irq,cpu_num);
  dst=cpu_structs[cpu_num].idt(U8 *)+irq*16;

  PushFD;
  Cli;
  *dst(U16 *)++=fp.u16[0];
  *dst(U16 *)++=GDT_CS64_SEL;
  *dst(U16 *)++=0x8000+type<<8+dpl<<13;
  *dst(U16 *)++=fp.u16[1];
  *dst(U32 *)++=fp.u32[1];
  *dst(U32 *)++=0;
  PopFD;
  return result;
}

U0 IntsInit()
{
//Init 8259
  OutU8(0x20,0x11); //IW1
  OutU8(0xA0,0x11); //IW1
  OutU8(0x21,0x20); //IW2
  OutU8(0xA1,0x28); //IW2
  OutU8(0x21,0x04); //IW3
  OutU8(0xA1,0x02); //IW3
  OutU8(0x21,0x0D); //IW4
  OutU8(0xA1,0x09); //IW4
  OutU8(0x21,0xFA); //Mask all but IRQ0 (timer) and IRQ2 Cascade
  OutU8(0xA1,0xFF);
}

U0 IntEOI()
{
  U32 *_d=LAPIC_EOI;
  *_d=0;
}

interrupt U0 IntNop1()
{
  IntEOI;
}

interrupt U0 IntNop2()
{
  OutU8(0xA0,0x20);
  OutU8(0x20,0x20);
}

U8 *IntFaultHandlersNew()
{
  I64 i;
  U8 *result=MAlloc(256*10,Fs->code_heap),*dst=result;
  for (i=0;i<256;i++) {
    *dst++=0x68; //PUSH I32 xxxxxxxx
    *dst(I32 *)++=i;
    *dst++=0xE9; //JMP	I32 xxxxxxxx
    *dst(I32 *)=SYS_IRQ_FAULT-dst-4;
    dst+=4;
  }
  return result;
}

U0 IDTInit1()
{ //Interrupt descriptor table part1
  U64 i,temp_ptr[2];
  U32 *ptr;
  Gs->idt=CAlloc(16*256);
  for (i=0;i<256;i++)
    if (!Gs->num && 0x20<=i<0x30)
      SetIDTEntry(i,&IntNop2);
    else
      SetIDTEntry(i,&IntNop1);

  ptr=temp_ptr;
  *ptr(U16 *)++=256*16-1;
  *ptr(U32 *)++=Gs->idt(U64).u32[0];
  *ptr(U32 *)++=Gs->idt(U64).u32[1];
  *ptr(U16 *)++=0;
  SetRAX(temp_ptr);
  asm {	LIDT U64 [RAX] }
}

U0 IDTInit2()
{ //Interrupt descriptor table part2
  U64 i;
  PushFD;
  Cli;
  for (i=0;i<256;i++)
    SetIDTEntry(i,&sys_fault_handlers_code[10*i]);
  SetIDTEntry(I_NMI,_SYS_HLT);
  SetIDTEntry(I_TIMER,SYS_TIMER_HANDLER);
  SetIDTEntry(I_MP_CRASH,*SYS_MP_CRASH_HANDLER_ADDR(U32 *));
  SetIDTEntry(I_WAKE,SYS_WAKE_HANDLER);
  PopFD;
}
