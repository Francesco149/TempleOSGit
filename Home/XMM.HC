//this is just a demo file I used to test the assembler modifications as I was
//adding SSE instructions. nothing useful in here

U64 test_var[2]={0,0};

U0 T() {
  asm {
    MOVAPS	XMM0,XMM1
    MOVAPS	XMM0,XMM2
    MOVAPS	XMM1,XMM0
    MOVAPS	XMM2,XMM0
    MOVAPS	XMM0,[test_var]
    MOVAPS	XMM1,[test_var]
    MOVAPS	XMM0,[RAX]
    MOVAPS	XMM1,[RBX]
    MOVAPS	XMM1,[RDX+16]
    MOVAPS	[test_var],XMM0
    MOVAPS	[test_var],XMM1
    MOVAPS	[RAX],XMM0
    MOVAPS	[RBX],XMM1

    //note: the UAsm cannot distinguish between MOVAPS and MOVAPD because
    //	    it's just MOVAPS with the 0x66 64-bit prefix. so if you see a
    //      MOVAPS that starts with 0x66, it's a MOVAPD, similar story for
    //	    other instrs
    MOVAPD	XMM0,XMM1
    MOVAPD	XMM0,XMM2
    MOVAPD	XMM1,XMM0
    MOVAPD	XMM2,XMM0
    MOVAPD	XMM0,[test_var]
    MOVAPD	XMM1,[test_var]
    MOVAPD	XMM0,[RAX]
    MOVAPD	XMM1,[RBX]
    MOVAPD	XMM1,[RDX+16]
    MOVAPD	[test_var],XMM0
    MOVAPD	[test_var],XMM1
    MOVAPD	[RAX],XMM0
    MOVAPD	[RBX],XMM1

    MOVUPS	XMM0,XMM1
    MOVUPS	XMM0,XMM2
    MOVUPS	XMM1,XMM0
    MOVUPS	XMM2,XMM0
    MOVUPS	XMM0,[test_var]
    MOVUPS	XMM1,[test_var]
    MOVUPS	XMM0,[RAX]
    MOVUPS	XMM1,[RBX]
    MOVUPS	XMM1,[RDX+16]
    MOVUPS	[test_var],XMM0
    MOVUPS	[test_var],XMM1
    MOVUPS	[RAX],XMM0
    MOVUPS	[RBX],XMM1


    MOVUPD	XMM0,XMM1
    MOVUPD	XMM0,XMM2
    MOVUPD	XMM1,XMM0
    MOVUPD	XMM2,XMM0
    MOVUPD	XMM0,[test_var]
    MOVUPD	XMM1,[test_var]
    MOVUPD	XMM0,[RAX]
    MOVUPD	XMM1,[RBX]
    MOVUPD	XMM1,[RDX+16]
    MOVUPD	[test_var],XMM0
    MOVUPD	[test_var],XMM1
    MOVUPD	[RAX],XMM0
    MOVUPD	[RBX],XMM1

    //NOTE: the UAsm will incorrectly say U128 for 32-bit and 64-bit movs
    MOVSS	XMM0,XMM1
    MOVSS	XMM0,XMM2
    MOVSS	XMM1,XMM0
    MOVSS	XMM2,XMM0
    MOVSS	XMM0,[test_var]
    MOVSS	XMM1,[test_var]
    MOVSS	XMM0,[RAX]
    MOVSS	XMM1,[RBX]
    MOVSS	XMM1,[RDX+16]
    MOVSS	[test_var],XMM0
    MOVSS	[test_var],XMM1
    MOVSS	[RAX],XMM0
    MOVSS	[RBX],XMM1

    //MOVSD name clashes with the legacy instruction,
    //so I aliased to SSE_MOVSD
    SSE_MOVSD	XMM0,XMM1
    SSE_MOVSD	XMM0,XMM2
    SSE_MOVSD	XMM1,XMM0
    SSE_MOVSD	XMM2,XMM0
    SSE_MOVSD	XMM0,[test_var]
    SSE_MOVSD	XMM1,[test_var]
    SSE_MOVSD	XMM0,[RAX]
    SSE_MOVSD	XMM1,[RBX]
    SSE_MOVSD	XMM1,[RDX+16]
    SSE_MOVSD	[test_var],XMM0
    SSE_MOVSD	[test_var],XMM1
    SSE_MOVSD	[RAX],XMM0
    SSE_MOVSD	[RBX],XMM1

    ADDPS	XMM0,XMM1
    ADDPS	XMM0,XMM2
    ADDPS	XMM0,[RAX]
    ADDPS	XMM1,[RBX]
    ADDPS	XMM1,[RDX+16]
    ADDPS	XMM1,[test_var]

    ADDPD	XMM0,XMM1
    ADDPD	XMM0,XMM2
    ADDPD	XMM0,[RAX]
    ADDPD	XMM1,[RBX]
    ADDPD	XMM1,[RDX+16]
    ADDPD	XMM1,[test_var]

    SUBPS	XMM0,XMM1
    SUBPS	XMM0,XMM2
    SUBPS	XMM0,[RAX]
    SUBPS	XMM1,[RBX]
    SUBPS	XMM1,[RDX+16]
    SUBPS	XMM1,[test_var]

    SUBPD	XMM0,XMM1
    SUBPD	XMM0,XMM2
    SUBPD	XMM0,[RAX]
    SUBPD	XMM1,[RBX]
    SUBPD	XMM1,[RDX+16]
    SUBPD	XMM1,[test_var]

    MULPS	XMM0,XMM1
    MULPS	XMM0,XMM2
    MULPS	XMM0,[RAX]
    MULPS	XMM1,[RBX]
    MULPS	XMM1,[RDX+16]
    MULPS	XMM1,[test_var]

    MULPD	XMM0,XMM1
    MULPD	XMM0,XMM2
    MULPD	XMM0,[RAX]
    MULPD	XMM1,[RBX]
    MULPD	XMM1,[RDX+16]
    MULPD	XMM1,[test_var]

    DIVPS	XMM0,XMM1
    DIVPS	XMM0,XMM2
    DIVPS	XMM0,[RAX]
    DIVPS	XMM1,[RBX]
    DIVPS	XMM1,[RDX+16]
    DIVPS	XMM1,[test_var]

    DIVPD	XMM0,XMM1
    DIVPD	XMM0,XMM2
    DIVPD	XMM0,[RAX]
    DIVPD	XMM1,[RBX]
    DIVPD	XMM1,[RDX+16]
    DIVPD	XMM1,[test_var]

    ADDSS	XMM0,XMM1
    ADDSS	XMM0,XMM2
    ADDSS	XMM0,[RAX]
    ADDSS	XMM1,[RBX]
    ADDSS	XMM1,[RDX+16]
    ADDSS	XMM1,[test_var]

    ADDSD	XMM0,XMM1
    ADDSD	XMM0,XMM2
    ADDSD	XMM0,[RAX]
    ADDSD	XMM1,[RBX]
    ADDSD	XMM1,[RDX+16]
    ADDSD	XMM1,[test_var]

    SUBSS	XMM0,XMM1
    SUBSS	XMM0,XMM2
    SUBSS	XMM0,[RAX]
    SUBSS	XMM1,[RBX]
    SUBSS	XMM1,[RDX+16]
    SUBSS	XMM1,[test_var]

    SUBSD	XMM0,XMM1
    SUBSD	XMM0,XMM2
    SUBSD	XMM0,[RAX]
    SUBSD	XMM1,[RBX]
    SUBSD	XMM1,[RDX+16]
    SUBSD	XMM1,[test_var]

    MULSS	XMM0,XMM1
    MULSS	XMM0,XMM2
    MULSS	XMM0,[RAX]
    MULSS	XMM1,[RBX]
    MULSS	XMM1,[RDX+16]
    MULSS	XMM1,[test_var]

    MULSD	XMM0,XMM1
    MULSD	XMM0,XMM2
    MULSD	XMM0,[RAX]
    MULSD	XMM1,[RBX]
    MULSD	XMM1,[RDX+16]
    MULSD	XMM1,[test_var]

    DIVSS	XMM0,XMM1
    DIVSS	XMM0,XMM2
    DIVSS	XMM0,[RAX]
    DIVSS	XMM1,[RBX]
    DIVSS	XMM1,[RDX+16]
    DIVSS	XMM1,[test_var]

    DIVSD	XMM0,XMM1
    DIVSD	XMM0,XMM2
    DIVSD	XMM0,[RAX]
    DIVSD	XMM1,[RBX]
    DIVSD	XMM1,[RDX+16]
    DIVSD	XMM1,[test_var]

    //the Asm doesn't seem to support 3-operator instrs so I just hardcode
    //these to use XMM0-1
    SHUFPS_XMM0_XMM1	0xFF
    SHUFPS_XMM0_XMM0	0xFF
    SHUFPD_XMM0_XMM1	0xFF
    SHUFPD_XMM0_XMM0	0xFF

    //the Asm gets confused with >4 byte opcodes so I pretend these insts are
    //16-bit so the Asm automatically adds the 0x66 prefix.
    //this breaks them in 16-bit but that's not important
    DPPS_XMM0_XMM0	0xFF
    DPPS_XMM0_XMM1	0xFF
    DPPD_XMM0_XMM0	0xFF
    DPPD_XMM0_XMM1	0xFF

    //the 2nd op for MOV{HL,LH}PS is defined to be XMM128 as a hack to force
    //a certain ModrM order, but it actually only accepts XMM XMM
  }
}

Uf("T");