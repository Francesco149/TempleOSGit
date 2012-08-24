//This is the software interrupt num
//we will use.  See [C:/LT/OSMain/Adam1a.HPZ,326] Software Int's for
//picking a num.  Nums are subject to change.

#define I_F_UNARY_MINUS	I_USER

asm {

//This changes the sign of the floating
//point val in RAX
F_UNARY_MINUS_INT::
	PUSH	RAX
	FLD	U64 [RSP]
	FCHS
	FSTP	U64 [RSP]
	FWAIT
	POP	RAX
	IRET

//This does the same thing, but not as
//an interrupt.
F_UNARY_MINUS_CALL::
	PUSH	RAX
	FLD	U64 [RSP]
	FCHS
	FSTP	U64 [RSP]
	FWAIT
	POP	RAX
	RET

//This invokes the interrupt version
//with a C callable function.
_F_UM_INT::
	ENTER	0
	MOV	RAX,SF_ARG1[RBP]
	INT	I_F_UNARY_MINUS
	LEAVE
	RET1	8

//This invokes the call version
//with a C callable function.
_F_UM_CALL::
	ENTER	0
	MOV	RAX,SF_ARG1[RBP]
	CALL	F_UNARY_MINUS_CALL
	LEAVE
	RET1	8
}

LTextern _F_UM_INT F64 UnaryMinusInt(F64 d);
LTextern _F_UM_CALL F64 UnaryMinusCall(F64 d);


#define SAMPLE_SIZE	1000000
U0 TimeIns()
{
  U64 start,end;
  I64 i,old_irq;
  BoolI8 old_preempt=Preempt(OFF);

  CPURep;
  old_irq=SetIDTEntry(I_F_UNARY_MINUS,F_UNARY_MINUS_INT,0,IDTET_TRAP);

//Measure interrupt time
  start=GetTimeStamp;
  for (i=0;i<SAMPLE_SIZE;i++)
	  UnaryMinusInt(pi);
  end=GetTimeStamp;
  "Interrupt Cycles: %10.5f\n",ToF64(end-start)/SAMPLE_SIZE;

//Measure call time
  start=GetTimeStamp;
  for (i=0;i<SAMPLE_SIZE;i++)
	  UnaryMinusCall(pi);
  end=GetTimeStamp;
  "Call      Cycles: %10.5f\n",ToF64(end-start)/SAMPLE_SIZE;

  Preempt(old_preempt);
  SetIDTEntry(I_F_UNARY_MINUS,old_irq,0,IDTET_IRQ);
}

TimeIns;

/*  Program Output

8 Cores 2.660GHz
Interrupt Cycles:  492.36652
Call	  Cycles:   22.40882

*/
