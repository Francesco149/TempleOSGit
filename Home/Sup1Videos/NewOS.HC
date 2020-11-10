asm {
USE16
S1_START::
	CLI
	XOR	SP,SP
	MOV	AX,0x7000
	MOV	SS,AX
	STI

	MOV	AX,0xB800
	MOV	ES,AX
	MOV	U32 ES:[0],0x20002000+'R'<<16+'B'

	XOR	EAX,EAX
	XOR	EBX,EBX
	CALL	@@1
@@1:	POP	BX
	PUSH	CS
	POP	AX
	SHL	EAX,4
	ADD	EAX,EBX
	SUB	EAX,U32 (@@1-S1_START)+CFAT32Boot.code

	SHR	EAX,4
	MOV	DS,AX
	MOV	SI,(S1_DAP-S1_START)+CFAT32Boot.code
	MOV	AH,0x42
	INT	0x13

	DU8	0xEA;	//JMP 0x1000:0000
	DU16	0,0x1000;

S1_DAP::
	DU8	0x10,0;
S1_DAP_BLK_CNT::
	DU16	1;	//BLK CNT
S1_DAP_BUF:
	DU16	0,0x1000;
S1_DAP_BLK::
	DU64	0;
S1_END::

S2_START::
	CLI
	XOR	SP,SP
	MOV	AX,0x7000
	MOV	SS,AX
	STI

	MOV	AX,0xB800
	MOV	ES,AX
	MOV	U32 ES:[8],0x20002000+'S'<<16+'O'
@@1:	JMP	@@1
S2_END::

}

U0 WriteKernel(U8 *filename)
{
  CDrv *dv;
  CDirEntry de;
  FileWrite(filename,S2_START,S2_END-S2_START);
  FileFind(filename,&de);
  dv=Let2Drv(*de.full_name);
  *S1_DAP_BLK(U64 *)=Clus2Blk(dv,de.clus);
  "BLK:%X\n",Clus2Blk(dv,de.clus);
  *S1_DAP_BLK_CNT(U16 *)=(de.size+BLK_SIZE-1)/BLK_SIZE;
  "CNT:%X\n",(de.size+BLK_SIZE-1)/BLK_SIZE;
  Free(de.full_name);
}

U0 InstallBoot(U8 drv_let)
{
  CDrv *dv=Let2Drv(drv_let);
  CFAT32Boot br;
  BlkRead(dv,&br,dv->drv_offset,1);
  br.jump_and_nop[0]=0xEB;
  br.jump_and_nop[1]=0x58;

  MemCpy(br.code,S1_START,S1_END-S1_START);
  BlkWrite(dv,&br,dv->drv_offset,1);

  ClassRep(&br);
  Un(br.code,S1_END-S1_START,16);
}

//Fmt('D',TRUE,FALSE,FSt_FAT32);
//WriteKernel("D:/Kernel.BIN.C");
//InstallBoot('D');
//Reboot;
