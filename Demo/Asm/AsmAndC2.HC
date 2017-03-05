/*Asm labels can only be defined once
in a task.  <F5> will spawn a new task
each time, so you don't get redefine
error, like when repeatedly #including
it from the cmd line.
*/

//This is to demo glbl var access.
//Glbs defined elsewhere can accessed too, like cnts.jiffies.
I64 glbl_ona=Freq2Ona(400),glbl_ona_step=10,glbl_ona_base=Freq2Ona(100);

asm {
//Opcodes are slightly different to make writing my x86_64 assembler easier.
//See [C:/Compiler/OpCodes.DD.Z,1].

JIFFIES_MSG:	DU8 "Jiffies:",0;

//See [C:/Kernel/StrA.HC.Z,1] ::/Kernel/StrA.HC and [C:/Kernel/KUtils.HC.Z,1].

_BEEPS2::
//You can clobber RAX,RBX,RCX,RDX,R8,R9.  The compiler expects that.
//See [C:/Kernel/KernelA.HH.Z,1799] REGG_CLOBBERED and [C:/Kernel/KernelA.HH.Z,1801] REGG_STK_TMP.
	PUSH	RBP
	MOV	RBP,RSP
	MOV	RCX,U64 SF_ARG1[RBP] //[C:/Kernel/KernelA.HH.Z,1] SF_ARG1

	PUSH	U64 [&cnts.jiffies]

@@05:	PUSH	RCX
//U0 [C:/Kernel/KMisc.HC.Z,218] Beep(I8 ona=62,Bool busy=FALSE)
	PUSH	FALSE	//Do not busy (spin) wait
	PUSH	U64 [&glbl_ona]	//evaluated at run time
	CALL	&Beep
	POP	RCX
	LOOP	@@05

	PUSH	RSI	//See [C:/Kernel/KernelA.HH.Z,1802] REGG_LOCAL_VARS & [C:/Kernel/KernelA.HH.Z,1803] REGG_LOCAL_NON_PTR_VARS
	MOV	RSI,JIFFIES_MSG
	CALL	PUT_STR
	POP	RSI

	POP	RAX
	SUB	RAX,U64 [&cnts.jiffies]
	NEG	RAX
	CALL	PUT_HEX_U64
	MOV	RAX,'\n'
	CALL	PUT_CHARS

	POP	RBP
	RET1	8
}

//My convention is to put an underscore
//on C callable asm routines.
_extern _BEEPS2 U0 Beeps2(I64 cnt);

U0 AsmAndC2()
{
  I64 reg R15 i;

  i=GetI64("$PURPLE$\n\nNum of beeps 1-5 (%d):$FG$",3,1,5);
  Beeps2(i);

  asm {
    LIST
//You can clobber RAX,RBX,RCX,RDX, but preserve the rest.
    MOV	RCX,R15 //You can clobber RAX,RBX,RCX,RDX. Preserve the rest.
    @@05:	PUSH	RCX

//U0 [C:/Kernel/KMisc.HC.Z,179] Snd(I8 ona);
    MOV	RAX,RCX	//ona=loop*10+100.0Hz
    IMUL2	RAX,glbl_ona_step //Intentionally evaluated at compile time
    ADD	RAX,U64 [&glbl_ona_base]  //Intentionally evaluated at run time
    PUSH	RAX
    CALL	&Snd	//We can skip IMPORT with & if JIT compiling.
    MOV	RCX,cnts.time_stamp_freq>>3 //JIT Const.  Simple delay loop.
@@10:	LOOP	@@10

    POP	RCX
    LOOP	@@05
  }
  Snd;
}

AsmAndC2;
