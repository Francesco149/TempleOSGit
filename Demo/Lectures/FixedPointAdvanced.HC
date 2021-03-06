/*"Fixed point" means you use ints
that are scaled by a value.  A common
example would be using number of pennies
instead of dollars with a float.

Fixed-point used to be much faster,
but modern processors do well with
floats.  It also depends on the compiler
and my compiler is poor with floats.

I often use 64-bit ints with upper 32-bits
as int and lower 32-bits as fraction.

See [C:/Demo/SubIntAccess.HC.Z,1] for how
to access upper or lower 32-bits.
*/

#define SAMPLE_SIZE	10000000

I32 coordinates[65536];

asm {
_ASM_FIXED_POINT::
	PUSH	RBP
	MOV	RBP,RSP
	PUSH	RSI
	PUSH	RDI
	MOV	RSI,coordinates
	MOV	RDI,ToI64(Sin(1.0)*0x100000000)
	XOR	RBX,RBX	//SUM
	MOV	RCX,SAMPLE_SIZE-1
@@05:	XOR	RDX,RDX
	MOV	DX,CX
	MOVSXD	RAX,U32 [RSI+RDX*4]
	IMUL	RDI
	SAR	RAX,32
	ADD	RBX,RAX
	DEC	RCX
	JGE	@@05
	MOV	RAX,RBX
	POP	RDI
	POP	RSI
	POP	RBP
	RET

SINE_VAL:	DU64	Sin(1.0);
RET_VAL:	DU64	0;

_ASM_FLOAT::
	PUSH	RBP
	MOV	RBP,RSP
	PUSH	RSI
	MOV	RSI,coordinates
	FLD	U64 [SINE_VAL]
	FLDZ
	MOV	RCX,SAMPLE_SIZE-1
@@05:	XOR	RDX,RDX
	MOV	DX,CX
	FILD	U32 [RSI+RDX*4]
	FMUL	ST0,ST2
	FADDP	ST1,ST0
	DEC	RCX
	JGE	@@05
	FISTP	U64 [RET_VAL]
	MOV	RAX,U64 [RET_VAL]
	FFREE	ST0
	FINCSTP
	POP	RSI
	POP	RBP
	RET
}

_extern _ASM_FIXED_POINT I64 AsmFixedPt();
_extern _ASM_FLOAT I64 AsmFloat();

U0 Main()
{
  I64 start,end,overhead_time,test_time;
  F64 d1,fsum;
  I64 reg i,tmp,reg d2,reg sum;

  CPURep;

  //Set-up some sample coordinates
  for (i=0;i<65536;i++)
    coordinates[i]=RandU32;

    //Measure Loop Overhead
  start=GetTSC;
  for (i=SAMPLE_SIZE-1;i>=0;i--) {
  }
  end=GetTSC;
  overhead_time=end-start;
  "$RED$Overhead Cycles       :%10.5f$FG$\n",
	ToF64(overhead_time)/SAMPLE_SIZE;

  //Measure F64 arithmetic
  // (Some of this is due to crappy
  // compiler code.)
  d1=Sin(1.0);
  fsum=0;
  start=GetTSC;
  for (i=SAMPLE_SIZE-1;i>=0;i--)
    fsum+=d1*coordinates[i&65535];
  end=GetTSC;
  test_time=end-start;
  "Float Sum             :%X\n",ToI64(fsum);
  "$RED$Float Cycles          :%10.5f$FG$\n",
	ToF64(test_time)/SAMPLE_SIZE;

  //Measure fixed point arithmetic
  d2=Sin(1.0)*0x100000000;
  sum=0;
  start=GetTSC;
  for (i=SAMPLE_SIZE-1;i>=0;i--) {
    tmp=d2*coordinates[i&65535];
    sum+=tmp.i32[1];
  }
  end=GetTSC;
  test_time=end-start;
  "Fixed-Point Sum       :%X\n",sum;
  "$RED$Fixed-Point Cycles    :%10.5f$FG$\n",
	ToF64(test_time)/SAMPLE_SIZE;

  //Measure fixed point arithmetic
  start=GetTSC;
  sum=AsmFixedPt;
  end=GetTSC;
  test_time=end-start;
  "Asm Fixed-Point Sum   :%X\n",sum;
  "$RED$Asm Fixed-Point Cycles:%10.5f$FG$\n",
	ToF64(test_time)/SAMPLE_SIZE;

  //Measure float arithmetic
  start=GetTSC;
  sum=AsmFloat;
  end=GetTSC;
  test_time=end-start;
  "Asm Float Sum         :%X\n",sum;
  "$RED$Asm Float Cycles      :%10.5f$FG$\n",
	ToF64(test_time)/SAMPLE_SIZE;

}

Main;

/*  Program Output

Machine 1:
8 Cores 2.660GHz
Overhead Cycles       :   2.00814
Float Sum             :FFFFE1D361BEED68
Float Cycles          :  10.16076
Fixed-Point Sum       :FFFFE1D361729914
Fixed-Point Cycles    :   5.29392
Asm Fixed-Point Sum   :FFFFE1D361729914
Asm Fixed-Point Cycles:   4.20464
Asm Float Sum         :FFFFE1D361BEED56
Asm Float Cycles      :   3.04635

Machine 2:
8 Cores 3.395GHz
Overhead Cycles       :   4.87040
Float Sum             :D20A01DB177
Float Cycles          :  10.11558
Fixed-Point Sum       :D209FD18CC7
Fixed-Point Cycles    :   4.50618
Asm Fixed-Point Sum   :D209FD18CC7
Asm Fixed-Point Cycles:   3.02426
Asm Float Sum         :D20A01DB17B
Asm Float Cycles      :   3.21070

*/
