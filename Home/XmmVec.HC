// proof of concept vector operations using SSE instructions

// perf improvements TODO:
// * implement U128 into the HC compiler and do proper XMM reg alloc
// * minimize swapping to memory, which requires the above
// * find a way to ensure stk is 16-byte aligned and use aligned instrs

asm {
_F2V::
//kinda expensive but you only do it once to init from F64's
MOV RAX,[RSP+8]
MOVUPD XMM2,[RAX]
MOVUPD XMM3,16[RAX]
CVTPD2PS XMM0,XMM2
CVTPD2PS XMM1,XMM3
MOVLHPS XMM0,XMM1 // XMM0=xyzw
RET1 8

_V::
LEA RAX,[RSP+8]
PUSH RAX
CALL _F2V
RET1 32

_V2F::
//expensive but you shouldn't need to do this often. do whole vec ops instead
MOV RAX,[RSP+8]
CVTPS2PD XMM1,XMM0
MOVHLPS XMM3,XMM0
CVTPS2PD XMM2,XMM3
MOVUPD [RAX],XMM1
MOVUPD [RAX+16],XMM2
RET1 8

_VST::
MOV RAX,[RSP+8]
MOVUPS [RAX],XMM0
RET1 8

_VLD::
MOV RAX,[RSP+8]
MOVUPS XMM0,[RAX]
RET1 8

_VADD::
MOV RAX,[RSP+8]
MOVUPS XMM1,[RAX]
ADDPS XMM0,XMM1
RET1 8

_VMUL::
MOV RAX,[RSP+8]
MOVUPS XMM1,[RAX]
MULPS XMM0,XMM1
RET1 8
}

_extern _F2V	U0 F2V(F64 *m);
_extern _V	U0 V(F64 x,F64 y,F64 z,F64 w);
_extern _V2F	U0 V2F(F64 *f);
_extern _VST	U0 VSt(U32 *m);
_extern _VLD	U0 VLd(U32 *m);
_extern _VADD	U0 VAdd(U32 *m);
_extern _VMUL	U0 VMul(U32 *m);

U0 VStF(U32 *m,F64 x,F64 y,F64 z,F64 w) { V(x,y,z,w); VSt(m); }
U0 VAddSt(U32 *m) { VAdd(m); VSt(m); }
U0 VMulSt(U32 *m) { VMul(m); VSt(m); }

U0 Main() {
  F64 f[4];
  U32 m[4];

  VStF(m,1,2,3,4);
  V(     4,3,2,1);
  VAddSt(m);

  V(2,2,4,4);
  VMul(m);

  V2F(f);
  "%f %f %f %f\n",f[0],f[1],f[2],f[3];
}


Main;
