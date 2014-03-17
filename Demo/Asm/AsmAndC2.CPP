/*
This is a JIT file, not AOT.

Asm labels can only be defined once
in a task.  Hit F5 to spawn a new task
each time and run it.
*/

//This is to demo glbl var access.
//Glbs defined elsewhere can accessed too, like sys_jiffies.
I64 glbl_hz=400,glbl_hz_step=100,glbl_hz_base=100;

asm {
//Opcodes are slightly different to make writing my x86_64 assembler easier.
//See [C:/Compiler/OpCodes.TXT.Z,1].
USE64	//Defaults to 64, but this is a good habit.

JIFFIES_MSG:	DU8 "Jiffies:",0;

//See [C:/Kernel/Str1a.CPP.Z,1] ::/Kernel/Str1a.CPP and [C:/Kernel/Utils1a.CPP.Z,1].

_BEEPS2::
//You can clobber RAX,RBX,RCX,RDX,R8,R9
	PUSH	RBP
	MOV	RBP,RSP
	MOV	RCX,U64 SF_ARG1[RBP] //[C:/Kernel/Adam1a.HPP.Z,1] SF_ARG1

	PUSH	U64 [&sys_jiffies]

@@05:	PUSH	RCX
//U0 Beep(I64 freq=500,Bool busy=FALSE)
	PUSH	FALSE	//Do not busy (spin) wait
	PUSH	U64 [&glbl_hz]	//evaluated at run time
	CALL	&Beep
	POP	RCX
	LOOP	@@05

	PUSH	RSI	//See [C:/Kernel/Adam1a.HPP.Z,3553] REG_VARS_MASK & [C:/Kernel/Adam1a.HPP.Z,3554] REG_NON_PTR_MASK
	MOV	RSI,JIFFIES_MSG
	CALL	PUT_STR
	POP	RSI

	POP	RAX
	SUB	RAX,U64 [&sys_jiffies]
	NEG	RAX
	CALL	PUT_HEX_U64
	MOV	RAX,'\n'
	CALL	PUT_CHARS

	POP	RBP
	RET1	8
}

//My convention is to put an underscore
//on C callable routines.
_extern _BEEPS2 U0 Beeps2(I64 cnt);

U0 AsmAndC2()
{
  I64 reg R15 i;

  i=PmtI64("$PURPLE$\n\nNum of beeps 1-5 (%d):$FG$",3,1,5);
  Beeps2(i);

  asm {
LIST
//You can clobber RAX,RBX,RCX,RDX, but preserve the rest.
USE64	//Defaults to 64, but this is a good habit.
	MOV	RCX,R15 //You can clobber RAX,RBX,RCX,RDX. Preserve the rest.

@@05:	PUSH	RCX
//U0 Snd(F64 freq);

	MOV	RAX,RCX	//freq=loop*100.0+100.0
	IMUL2	RAX,glbl_hz_step	//evaluated at compile ttime
	ADD	RAX,U64 [&glbl_hz_base] //evaluated at run time
	PUSH	RAX
	FILD	U64 [RSP]	//Convert from int to floating
	FSTP	U64 [RSP]

	CALL	&Snd	//We can skip IMPORT with & if JIT compiling.

	MOV	RCX,time_stamp_freq>>3	//JIT Const.  Simple delay loop.
@@10:	LOOP	@@10

	POP	RCX
	LOOP	@@05
  }
  Snd(0);
}

AsmAndC2;
