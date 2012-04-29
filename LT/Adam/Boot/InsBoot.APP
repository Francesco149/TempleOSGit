#help_index "Install"
//See [::/LT/Doc/Install.TXZ] Install Documentation or [::/LT/Doc/Upgrade.TXZ] Upgrading Documentation.

//OSMain module: OSMain.BIZ
#define OSMAIN_BIC	"OSMain.BIC"
//First dir on CD.  First file must be OSMain.BIC (on CD-ROMs).
#define BOOT_DIR	"/0000Boot"
//Stage 2 of master boot loader
#define BOOTMHD2_BIC	"/0000Boot/BootMHD2.BIC"
//Old master boot record
#define OLDMBR_BIC	"/0000Boot/OldMBR.BIC"
//CD-ROM patched Boot Loader
#define BOOTCD_BIZ	"/0000Boot/BootCD.BIZ"

if (IsDrvWritable)
  MkDir("/Temp"); //Good to have a Temp

#include "BootCD"
#include "BootHD"
#include "BootMHD"
#include "BootMHD2"

#help_index "Install;File/Install Related;File/CD DVD"
public BoolI64 CDBootFileCreate()
{
  U8 *br;
  U64 size=FileSize(BOOT_DIR "/" OSMAIN_BIC);

  br=CAlloc(CD_BOOT_LOADER_SIZE);
  *BCD_BLK_CNT(U16 *)=(size+CD_BLK_SIZE-1)/CD_BLK_SIZE;

  //After preboot + BootCD.BIZ comes OSMain.BIC
  //[::/LT/Adam/Dsk/DskCDDVD.APZ,CDBOOT] CDBOOT

  //No emulation
  //This trick assumes OSMain is first file in first Dir.
  *BCD_BLK_LO(U32 *)=1+(CD_FILE_OFFSET+CD_BOOT_LOADER_SIZE/BLK_SIZE)/(CD_BLK_SIZE>>BLK_SIZE_BITS);

  MemCpy(br,BCD_START,BCD_END-BCD_START);
  FileWrite(BOOTCD_BIZ,br,CD_BOOT_LOADER_SIZE);
  Free(br);
  return TRUE;
}

#help_index "Call"
asm {
USE64
_HI_CALL::
	ENTER	0
	MOV	RAX,U64 16[RBP]
	OR	RAX,RAX
	JZ	@@05
	CALL	RAX
@@05:	LEAVE
	RET1	8
}
LTextern _HI_CALL U64 HiCall(U0 *s);

#help_index "Boot"
public U0 Boot(U8 *filename)
//Soft Reboot (no hardware rst)
//Specify the OSMain binary file you
//wish to boot.
{
  U64 size;
  OSMainStruct	*hi_image,*lo_image=sys_boot_base-sizeof(BinFileHeaderStruct),
		reg *shutdown,*hi_image2;

  if (!(hi_image2=FileRead(filename,&size))) return;
  switch (snd_dev) {
    case SD_PC_SPEAKER:
      break;
    case SD_HD_AUDIO:
      CallExtStr("HDAudioEnd",TRUE);
      break;
  }

  shutdown=MAlloc(SYS_RESTART_OSMAIN_END-SYS_RESTART_OSMAIN,Fs->code_heap);
  hi_image=MAlloc(size,Fs->code_heap);
  MemCpy(hi_image,hi_image2,size);
  Free(hi_image2);

  hi_image->boot_base=lo_image(U8 *)+sizeof(BinFileHeaderStruct);
  hi_image->boot_patch_table_base=lo_image(U8 *)+hi_image->h.loader_patch_table_offset;
  MemCpy(&hi_image->sys_mem_E801,&lo_image->sys_mem_E801,2*2);
  MemCpy(&hi_image->sys_mem_E820,&lo_image->sys_mem_E820,NUM_MEM_E820_ENTRIES*MEM_E820_ENTRY_SIZE);
  hi_image->sys_gr_mode=lo_image->sys_gr_mode;
  hi_image->sys_pci_busses=lo_image->sys_pci_busses;
  hi_image->boot_code=BOOT_CODE_RAM;

  Cli;
  if (mp_cnt>1)
    MPHalt;

  MemCpy(shutdown,SYS_RESTART_OSMAIN,SYS_RESTART_OSMAIN_END-SYS_RESTART_OSMAIN);
  MemCpy(&hi_image->sys_gdttab,&lo_image->sys_gdttab,GDT_SIZE);
  MemCpy(lo_image,hi_image,size);
  HiCall(shutdown);
}

#help_index "Install"
U0 InstallLTDir(U8 src,U8 dst)
{
  U8 buf_s[136],buf_d[136];
  if (src==dst) return;
  SPrintF(buf_s,"%c:/LT",src);
  SPrintF(buf_d,"%c:/LT",dst);
  DelTree(buf_d);
  CopyTree(buf_s,buf_d);
}

U8 ChkWritable(U8 drv_let=0)
{
  BoolI8 cont=TRUE;
  U8 writable_drv=drv_let;
  LTPrt *p;
  LTBlkDev *bdev;
  if (!IsDrvWritable(drv_let)) {
    '' drv_let;
    ": Drive is read-only.\n";
    do {
      "Enter RAM drive (usually 'B') for temporary files:";
      writable_drv=ToUpper(GetChar);
      NewLine;
      p=Drv2Prt(writable_drv);
      bdev=p->bdev;
      if (bdev->type!=LTBDT_RAM ||
	  !IsDrvWritable(writable_drv))
	"Not A RAM Drive\n";
      else
	cont=FALSE;
    } while (cont);
    Fmt(writable_drv,FALSE,FALSE);
    InstallLTDir(drv_let,writable_drv);
  }
  return writable_drv;
}


#help_index "Install"
public U0 MakeAll()
//Call from [C:/LT/Adam/Boot/InsBoot.APZ,157] InstallBoot()  because loader must be patched with block address.

//These 2 modules (and bootloaders at top) are the only statically
//compiled code in LoseThos.  Everything else is JIT.
//You are doing it wrong if you make statically compiled code.

{
  Cd("/LT/Compiler");
  if (Cmp("Compiler","Compiler",,':'))
    throw;
  Cd("/LT/OSMain");
  if (Cmp("OSMain","OSMain",,':'))
    throw;
}

public BoolI64 InstallBoot(U8 src_drv,U8 dst_drv=0)
{
//Normally, you just supply a src_drv.	It will compile
//the code on that drive and update the boot sect.

//If you are making a bootable CD-ROM you supply a dst_drv.
//See [::/LT/Doc/FileMgr.TXZ,Bootable] Bootable CD-ROMs

//See [::/LT/Doc/Install.TXZ] Install Documentation

//Study my account examples my technique [::/LT/Demo/AcctExample/TSAdam.APZ,1] Cfg Strs [::/LT/Demo/AcctExample/TSDistribute.APZ,1] Update Funs

  BoolI8 result=FALSE;
  LTBlkDev *bdev;
  LTPrt *p;
  FAT32BootStruct br;
  LTDirEntry de;
  U64 *qq,i;
  U8 *_b;
  U16 *_w;

  try {
    if (!src_drv) src_drv=Fs->cur_prt->drv_let;
    if (!dst_drv) dst_drv=src_drv;

    //This passes a cfg arg for
    //compilation of OSMain which is accessed
    //within a #exe {} statement.
    //See [::/LT/OSMain/OSCfg.CPZ,InsBootDstDrv] InsBootDstDrv
    //(There is no way to set compiler options.)
    NamedFramePtrAdd("InsBootDstDrv",dst_drv);
    src_drv=ChkWritable(src_drv);
    if (!Drv(src_drv))
      throw;
    MakeAll;
    Cd("/");

    bdev=Drv2BlkDev(dst_drv);
    p	=Drv2Prt   (dst_drv);
    switch (bdev->type) {
      case LTBDT_ATAPI:
	Move("/LT/OSMain/OSMain.BIZ",BOOT_DIR "/" OSMAIN_BIC);
	CDBootFileCreate;
	result=TRUE;
	break;
      case LTBDT_RAM:
      case LTBDT_ATA:
	Move("/LT/OSMain/OSMain.BIZ",OSMAIN_BIC);
	if (!FileFind(OSMAIN_BIC,&de,FALSE,TRUE))
	  "No OSMain.BIC\n";
	else {
	  "Modifying partition boot record.\n";
	  RBlks(p,&br,p->offset,1);
	  br.jump_and_nop[0]=0xEB;
	  br.jump_and_nop[1]=0x58;
	  br.jump_and_nop[2]=0x90;
	  _w=BHD_BLK_CNT;
	  *_w=(de.size+BLK_SIZE-1)>>BLK_SIZE_BITS;
	  qq=BHD_DAP_BLK;
	  *qq=p->data+de.cluster*p->spc;
	  _b=BHD_CODE;
	  for (i=0;i<BHD_END-BHD_CODE;i++)
	    br.code[i]=_b[i];
	  for (;i<420;i++)
	    br.code[i]=0;
	  WBlks(p,&br,p->offset,1);
	  result=TRUE;
	}
	break;
    }
  } catch
    CatchAll;
  NamedFramePtrDel("InsBootDstDrv");
  return result;
}


public U0 OldMasterBootRead(U8 src_drv,U8 dst_drv)
{
  LTBlkDev *bdev=Drv2BlkDevFwded(src_drv);
  LTPrt *p;
  MasterBootStruct mbr;
  Drv(dst_drv);
  p=Fs->cur_prt;
  if (p->type!=PT_LT && p->type!=PT_FAT32)
    PutDefineErr("ST_NOT_LT");
  else {
    //Bypass partition bounds-checking
    BlkDevLock(bdev);
    ATAReadBlks(bdev,&mbr,0,1);
    BlkDevUnlock(bdev);

    Drv(dst_drv);
    MkDir(BOOT_DIR);
    FileWrite(OLDMBR_BIC,&mbr,BLK_SIZE);
  }
}

public U0 OldMasterBootWrite(U8 src_drv,U8 dst_drv)
{
  LTBlkDev *bdev=Drv2BlkDevFwded(dst_drv);
  MasterBootStruct *mbr;
  Drv(src_drv);
  if (mbr=FileRead(OLDMBR_BIC)) {
    //Bypass partition bounds-checking
    BlkDevLock(bdev);
    ATAWriteBlks(bdev,mbr,0,1);
    BlkDevUnlock(bdev);
  }
  Free(mbr);
}

public U0 MasterBootZero(U8 dst_drv)
{
//This is dangerous!!
//The LoseThos partitioner doesn't play well
//with other operating systems at this time and you need
//to do this on a drive partitioned by LoseThos
//if you wish to partition with another operating system.
  LTBlkDev *bdev=Drv2BlkDevFwded(dst_drv);
  MasterBootStruct mbr;
  MemSet(&mbr,0,BLK_SIZE);
  //Bypass partition bounds-checking
  BlkDevLock(bdev);
  ATAWriteBlks(bdev,&mbr,0,1);
  BlkDevUnlock(bdev);
}

public BoolI64 InstallMasterBoot(U8 drv_let,U8 *drv_list=NULL)
{
  BoolI8 result=FALSE;
  LTBlkDev *bdev,*bd1;
  LTPrt *p,*p1;
  MasterBootStruct mbr;
  LTDirEntry de;
  U64 i,j,size,*qq;
  U8 *_b;
  U16 *_w;
  U8 *menu_ptr,*ptr,ch,buf[128],buf2[128];

  try {
    if (drv_list)
      StrCpy(buf2,drv_list);
    else {
      j=0;
      for (i='A';i<='Z';i++)
	buf2[j++]=i;
      buf2[j++]=0;
    }

    Drv(drv_let);
    p=Fs->cur_prt;
    if (p->type!=PT_LT && p->type!=PT_FAT32)
      PutDefineErr("ST_NOT_LT");
    else {
      bdev=Drv2BlkDevFwded(drv_let);
      if (!FileFind(OLDMBR_BIC,&de,FALSE,TRUE))
	OldMasterBootRead(drv_let,drv_let);

      qq=BMHD2_BLK_ARRAY;
      MemSet(qq,0,sizeof(U64)*8);
      menu_ptr=BMHD2_BOOT_MSG;
      SPrintF(menu_ptr,"\n\r\n\rLoseThos Boot Loader\n\r\n\r");
      j=0;
      if (FileFind(OLDMBR_BIC,&de,FALSE,TRUE)) {
	*qq++=p->data+de.cluster*p->spc;
	StrCat(menu_ptr,"0. Old Boot Record\n\r");
	j++;
      }

      ptr=buf2;
      while (ch=*ptr++) {
	try {
	  p1 =Drv2Prt(ch);
	  bd1=Drv2BlkDevFwded(ch);
	} catch {
	  Fs->catch_except=TRUE;
	  p1 =NULL;
	  bd1=NULL;
	}
	if (p1 && bd1==bdev) {
	  *qq=p1->offset;
	  "Drive %C:%16X\n",Prt2Drv(p1),*qq;
	  SPrintF(buf,"%d. Drive %C\n\r",j++,Prt2Drv(p1));
	  StrCat(menu_ptr,buf);
	  qq++;
	}
      }
      StrCat(menu_ptr,"\n\rSelection:");

      size=BMHD2_END-BMHD2_START;
      FileWrite(BOOTMHD2_BIC,BMHD2_START,size);

      if (!FileFind(BOOTMHD2_BIC,&de,FALSE,TRUE))
	"No Boot Loader Image\n";
      else {
	_w=BMHD_BLK_CNT;
	*_w=(size+BLK_SIZE-1)>>BLK_SIZE_BITS;
	qq=BMHD_DAP_BLK;
	*qq=p->data+de.cluster*p->spc;
	_b=BMHD_CODE;
	//Bypass partition bounds-checking
	BlkDevLock(bdev);
	ATAReadBlks(bdev,&mbr,0,1);
	for (i=0;i<BMHD_END-BMHD_CODE;i++)
	  mbr.boot_code[i]=_b[i];
	for (;i<440;i++)
	  mbr.boot_code[i]=0;
	if (!mbr.media_id)
	  mbr.media_id=RandU32;
	mbr.zero=0;
	mbr.signature=0xAA55;

	ATAWriteBlks(bdev,&mbr,0,1);
	BlkDevUnlock(bdev);
	result=TRUE;
      }
    }
  } catch
    CatchAll;
  return result;
}

#help_index "Boot"

/*
These routines are intended for use when
booted to the production CD.  They
allow compiling and booting the RAM Drive 'B'.

These are good when you are trying to get
a new system working and you don't want to
burn a CD-ROM for each attempt.

If you alter the kernel, the RAM drive **might**
shift to a new location and data will not be
recognized.
*/

public U0 CmpProdCDROM(U8 src_drv,U8 dst_drv=0)
{
  Auto(PROD_CDROM_CFG);
  InstallBoot(src_drv,dst_drv);
}

public U0 Install2RAMDrv(U8 src_drv='T',U8 dst_drv='B')
{
  U8 buf_s[136],buf_d[136];
  Fmt(dst_drv,TRUE,FALSE);
  SPrintF(buf_s,"%C:/",src_drv);
  SPrintF(buf_d,"%C:/",dst_drv);
  CopyTree(buf_s,buf_d);
  CmpProdCDROM(dst_drv);
}

public U0 Boot2RAMDrv(U8 drv_let='B')
{
  U8 buf[136];
  CmpProdCDROM(drv_let);
  SPrintF(buf,"%C:/OSMain.BIC",drv_let);
  Boot(buf);
}

#help_index ""
