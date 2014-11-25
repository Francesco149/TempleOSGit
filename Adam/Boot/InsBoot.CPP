#help_index "Install"
//See [C:/Doc/Install.TXT.Z,1] Install Documentation or [C:/Doc/Upgrade.TXT.Z,1] Upgrading Documentation.

//Kernel module: Kernel.BIN.Z
#define KERNEL_BIN_C	"Kernel.BIN.C"

#define BOOT_DIR	"/0000Boot"
#define BOOT_DIR_KERNEL_BIN_C BOOT_DIR "/0000" KERNEL_BIN_C

//Stage 2 of master boot loader
#define BOOTMHD2_BIN_C	BOOT_DIR "/BootMHD2.BIN.C"

//Old master boot record
#define OLDMBR_BIN_C	BOOT_DIR "/OldMBR.BIN.C"

if (IsDrvWritable)
  MkDir("/Temp"); //Good to have a Temp

#include "BootCD"
#include "BootHD"
#include "BootMHD"
#include "BootMHD2"
#include "DskCDDVD"

#help_index "Call"
asm {
USE64
_HI_CALL::
	PUSH	RBP
	MOV	RBP,RSP
	MOV	RAX,U64 16[RBP]
	TEST	RAX,RAX
	JZ	@@05
	CALL	RAX
@@05:	POP	RBP
	RET1	8
//************************************
_HI_MEMCPY::
	PUSH	RBP
	MOV	RBP,RSP
	PUSH	RSI
	PUSH	RDI
	CLD
	MOV	RDI,U64 SF_ARG1[RBP]
	MOV	RSI,U64 SF_ARG2[RBP]
	MOV	RCX,U64 SF_ARG3[RBP]
	REP_MOVSB
	MOV	RAX,RDI
	POP	RDI
	POP	RSI
	POP	RBP
	RET1	24
}
_extern _HI_CALL I64 HiCall(U8 *s);
_extern _HI_MEMCPY U8 *HiMemCpy(U8 *dst,U8 *src,I64 cnt);

#help_index "Boot"
public U0 RAMReboot(U8 *filename)
{//Softboot Kernel Bin file. No hardware reset.
  I64 size;
  CKernel *hi_image,*lo_image=sys_boot_base-sizeof(CBinFile),reg *sys_ram_reboot;

  do if (!(hi_image=FileRead(filename,&size))) return;
  while (hi_image<0x100000);

  do sys_ram_reboot=MAlloc(SYS_RAM_REBOOT_END-SYS_RAM_REBOOT,Fs->code_heap);
  while (sys_ram_reboot<0x100000);

  hi_image->boot_src=BOOT_SRC_RAM;
  hi_image->boot_patch_table_base=lo_image(U8 *)+hi_image->h.patch_table_offset;
  hi_image->sys_run_level =lo_image->sys_run_level&(RLF_VGA|RLF_16BIT);
  MemCpy(&hi_image->start,&lo_image->start,sizeof(CKernel)-offset(CKernel.start));

  CLI
  if (mp_cnt>1)
    MPHalt;

  HiMemCpy(sys_ram_reboot,SYS_RAM_REBOOT,SYS_RAM_REBOOT_END-SYS_RAM_REBOOT);
  HiMemCpy(lo_image,hi_image,size);
  HiCall(sys_ram_reboot);
}

#help_index "Install"
U8 ChkWritable(U8 drv_let=0)
{
  Bool cont=TRUE;
  I64 writable_drv=drv_let;
  U8 buf_s[136],buf_d[136];
  CDrv *dv;
  CBlkDev *bd;
  if (!IsDrvWritable(drv_let)) {
    '' drv_let;
    ": Drive is read-only.\n";
    do {
      "Enter RAM drive (usually 'B') for temporary files:";
      writable_drv=ToUpper(GetChar);
      '\n';
      dv=Let2Drv(writable_drv);
      bd=dv->bd;
      if (bd->type!=BDT_RAM ||
	  !IsDrvWritable(writable_drv))
	"Not A RAM Drive\n";
      else
	cont=FALSE;
    } while (cont);
    if (drv_let!=writable_drv) {
      Fmt(writable_drv,FALSE,FALSE);
      StrPrint(buf_s,"%c:/",drv_let);
      StrPrint(buf_d,"%c:/",writable_drv);
      CopyTree(buf_s,buf_d);
    }
  }
  return writable_drv;
}

#help_index "Install"
public U0 MakeAll()
{//Use [C:/Adam/Boot/InsBoot.CPP.Z,130] MakeOSDrvBootInstall. Loader must be patched.
//These 2 modules (and bootloaders at top) are the only AOT
//compiled code in TempleOS.  Everything else is JIT.
//You are doing it wrong if you make AOT compiled code.
  Cd("/Compiler");
  if (Cmp("Compiler","Compiler",,':'))
    throw;
  Cd("/Kernel");
  if (Cmp("Kernel","Kernel",,':'))
    throw;
}

public Bool MakeOSDrvBootInstall(U8 src_drv=0,U8 dst_drv=0)
{// Does [C:/Adam/Boot/InsBoot.CPP.Z,117] MakeAll and installs new boot-loader.

//Normally, you just supply a src_drv.	It will compile
//the code on that drive and update the boot sect.

//If you are making a bootable CD-ROM, you supply a dst_drv.
//See [C:/Misc/MakeDistro.CPP.Z,1]

//See [C:/Doc/Install.TXT.Z,1] Install Documentation

//Study my account examples my technique: [C:/Demo/AcctExample/TSAdam.CPP,1] Cfg Strs [C:/Demo/AcctExample/TSDistro.CPP,1] Update Funs

  CBlkDev *bd;
  CDrv *dv;
  CFAT32Boot br;
  CDirEntry de;
  Bool result=FALSE;
  I64 i;

  try {
    if (!src_drv) src_drv=Fs->cur_dv->drv_let;
    if (!dst_drv) dst_drv=src_drv;

    src_drv=ChkWritable(src_drv);
    if (!Drv(src_drv))
      throw;
    MakeAll;
    Cd("/");

    bd=Let2BlkDev(dst_drv);
    dv=Let2Drv	 (dst_drv);
    switch (bd->type) {
      case BDT_ATAPI:
	Move("/Kernel/Kernel.BIN.Z",BOOT_DIR_KERNEL_BIN_C);
	result=TRUE;
	break;
      case BDT_RAM:
      case BDT_ATA:
	Move("/Kernel/Kernel.BIN.Z",KERNEL_BIN_C);
	if (!FileFind(KERNEL_BIN_C,&de,FUF_JUST_FILES))
	  "No Kernel.BIN.C\n";
	else {
	  Free(de.full_name);
	  "Modifying partition boot record.\n";
	  RBlks(dv,&br,dv->base_offset,1);
	  br.jump_and_nop[0]=OC_JMP_REL8;
	  br.jump_and_nop[1]=offset(CFAT32Boot.code)-2;
#assert offset(CFAT32Boot.code)>=offset(CRedSeaBoot.code)
	  br.jump_and_nop[2]=OC_NOP;
	  *BHD_BLK_CNT(U16 *)=(de.size+BLK_SIZE-1)>>BLK_SIZE_BITS;
	  *BHD_DAP_BLK(I64 *)=Cluster2Blk(dv,de.cluster);
	  for (i=0;i<BHD_END-BHD_CODE;i++)
	    br.code[i]=BHD_CODE(U8 *)[i];
#assert sizeof(CFAT32Boot.code)>=BHD_END-BHD_CODE
	  for (;i<sizeof(CFAT32Boot.code);i++)
	    br.code[i]=0;
	  WBlks(dv,&br,dv->base_offset,1);
	  result=TRUE;
	}
	break;
    }
  } catch {
    PutExcept;
    Beep;
  }
  return result;
}

public U0 MasterBootOldRead(U8 src_drv,U8 dst_drv)
{//Reads MBR from disk drive containing src partition.
 //Writes a single blk file to dst /0000Boot directory.
  CBlkDev *bd=Let2BlkDev(src_drv);
  CDrv *dv;
  CMasterBoot mbr;
  Drv(dst_drv);
  dv=Fs->cur_dv;
  if (dv->type!=DVT_REDSEA && dv->type!=DVT_FAT32)
    PrintErr("Not RedSea Drv\n");
  else {
    //Bypass partition bounds-checking
    BlkDevLock(bd);
    ATAReadBlks(bd,&mbr,0,1);
    BlkDevUnlock(bd);

    Drv(dst_drv);
    MkDir(BOOT_DIR);
    FileWrite(OLDMBR_BIN_C,&mbr,BLK_SIZE);
  }
}

public U0 MasterBootOldWrite(U8 src_drv,U8 dst_drv)
{//Reads OldMBR from src drive /0000Boot directory
  //writes it to the MBR of the drive with dst partition.
  CBlkDev *bd=Let2BlkDev(dst_drv);
  CMasterBoot *mbr;
  Drv(src_drv);
  if (mbr=FileRead(OLDMBR_BIN_C)) {
    //Bypass partition bounds-checking
    BlkDevLock(bd);
    ATAWriteBlks(bd,mbr,0,1);
    BlkDevUnlock(bd);
  }
  Free(mbr);
}

public U0 MasterBootZero(U8 dst_drv)
{//Set MBR of disk with dst partition to zero.

//This is dangerous!!
//The TempleOS partitioner doesn't play well
//with other operating systems at this time and you need
//to do this on a drive partitioned by TempleOS
//if you wish to partition with another operating system.
  CBlkDev *bd=Let2BlkDev(dst_drv);
  CMasterBoot mbr;
  MemSet(&mbr,0,BLK_SIZE);
  //Bypass partition bounds-checking
  BlkDevLock(bd);
  ATAWriteBlks(bd,&mbr,0,1);
  BlkDevUnlock(bd);
}

public Bool MasterBootIntall(U8 drv_let,U8 *drv_lst=NULL)
{//Create new MBR on the disk that has drv_let as a partition.
  //Puts stage 2 in /0000Boot dir of drv_let.
  CBlkDev *bd,*bd1;
  CDrv *dv,*p1;
  CMasterBoot mbr;
  CDirEntry de;
  I64 i,j,size,*_q;
  U8 *menu_ptr,*ptr,ch,buf[128];
  Bool result=FALSE;

  try {
    if (drv_lst)
      StrCpy(buf,drv_lst);
    else {
      j=0;
      for (i='A';i<='Z';i++)
	buf[j++]=i;
      buf[j++]=0;
    }

    Drv(drv_let);
    dv=Fs->cur_dv;
    if (dv->type!=DVT_REDSEA && dv->type!=DVT_FAT32)
      PrintErr("Not RedSea Drv\n");
    else {
      bd=Let2BlkDev(drv_let);
      if (!FileFind(OLDMBR_BIN_C,,FUF_JUST_FILES))
	MasterBootOldRead(drv_let,drv_let);

      _q=BMHD2_BLK_ARRAY;
      MemSet(_q,0,sizeof(I64)*8);
      menu_ptr=BMHD2_BOOT_MSG;
      StrPrint(menu_ptr,"\n\r\n\rTempleOS Boot Loader\n\r\n\r");
      j=0;
      if (FileFind(OLDMBR_BIN_C,&de,FUF_JUST_FILES)) {
	Free(de.full_name);
	*_q++=Cluster2Blk(dv,de.cluster);
	CatPrint(menu_ptr,"0. Old Boot Record\n\r");
	j++;
      }

      ptr=buf;
      while (ch=*ptr++) {
	try {
	  p1 =Let2Drv(ch);
	  bd1=Let2BlkDev(ch);
	} catch {
	  Fs->catch_except=TRUE;
	  p1 =NULL;
	  bd1=NULL;
	}
	if (p1 && bd1==bd) {
	  *_q=p1->base_offset;
	  "Drive %C:%16X\n",Drv2Let(p1),*_q;
	  CatPrint(menu_ptr,"%d. Drive %C\n\r",j++,Drv2Let(p1));
	  _q++;
	}
      }
      CatPrint(menu_ptr,"\n\rSelection:");

      size=BMHD2_END-BMHD2_START;
      FileWrite(BOOTMHD2_BIN_C,BMHD2_START,size);

      if (!FileFind(BOOTMHD2_BIN_C,&de,FUF_JUST_FILES))
	"No Boot Loader Image\n";
      else {
	Free(de.full_name);
	*BMHD_BLK_CNT(U16 *)=(size+BLK_SIZE-1)>>BLK_SIZE_BITS;
	*BMHD_DAP_BLK(I64 *)=Cluster2Blk(dv,de.cluster);
	//Bypass partition bounds-checking
	BlkDevLock(bd);
	ATAReadBlks(bd,&mbr,0,1);
	for (i=0;i<BMHD_END-BMHD_CODE;i++)
	  mbr.code[i]=BMHD_CODE(U8 *)[i];
#assert sizeof(CMasterBoot.code)>=BMHD_END-BMHD_CODE
	for (;i<sizeof(CMasterBoot.code);i++)
	  mbr.code[i]=0;
	if (!mbr.media_id)
	  mbr.media_id=RandU32;
	mbr.zero=0;
	mbr.signature=0xAA55;

	ATAWriteBlks(bd,&mbr,0,1);
	BlkDevUnlock(bd);
	result=TRUE;
      }
    }
  } catch
    PutExcept;
  return result;
}
