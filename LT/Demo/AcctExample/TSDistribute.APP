//The CFG defines are [::/LT/Demo/AcctExample/TSAdam.APZ] Here.

U0 TSHDMount()
{
  Auto(CFG_HARD_DRVS "\n");
  Mount;
}

U0 ShowBootFileDate(U8 drv_let)
{
  LTDirEntry de;
  U8 *name=MSPrintF("%c:/OSMain.BIC",drv_let);
  if (FileFind(name,&de))
    "%s %D %T\n",name,de.datetime,de.datetime;
  else
    "%s\n",name;
  Free(name);
}

U0 ShowBootFiles()
{
  U8 *ptr=TS_HDS;
  while (*ptr)
    ShowBootFileDate(*ptr++);
}

U8 DrvMySelect()
{
  U64 result=0;
  ShowBootFiles;
  "Drive:";
  result=ToUpper(GetChar);
  NewLine;
  return result;
}

U0 CmpDrv(U8 drv_let)
{
  drv_let=ToUpper(drv_let);
  Auto(TS_CFG);
  InstallBoot(drv_let);
  if (StrOcc(TS_MASTER_BOOT_DRVS,drv_let))
    InstallMasterBoot(drv_let);
}

U0 QMenuBoot(U8 drv_let=0)
{
  if (!drv_let)
    drv_let=Prt2Drv(Fs->cur_prt);
  Drv(drv_let);
  Cd("/LT/OSMain");
  Auto(TS_MENU_CFG);
  if (!Cmp("OSMain","OSMain",NULL,':'))
    Boot("OSMain.BIZ");
}

U0 UpdateAndCompile(U8 src,U8 dst)
{
  U8 buf_s[128],buf_d[128];
  if (src==dst) return;

  SPrintF(buf_s,"%c:/LT",src);
  SPrintF(buf_d,"%c:/LT",dst);
  CopyTree(buf_s,buf_d);
  CmpDrv(dst);
}

U0 PromptUpdateDrv()
{
  U8 src,dst;
  "\nSrc Drive:\n";
  src=DrvMySelect;
  "\nDst Drive:\n";
  dst=DrvMySelect;
  UpdateAndCompile(src,dst);
  ShowBootFiles;
  Drv(src);
}

U0 MyMerge()
{
  U8 src,dst,buf_s[128],buf_d[128];
  "\nMerge Src Drive:\n";
  src=DrvMySelect;
  "\nMerge Dst Drive:\n";
  dst=DrvMySelect;
  SPrintF(buf_s,"%c:/LT/*",src);
  SPrintF(buf_d,"%c:/LT/*",dst);
  MergeChk(buf_d,buf_s,"+r+d");
  Drv(dst);
}

U0 MyCopyDrv(U8 src,U8 dst)
{
  U8 buf_s[128],buf_d[128];
  src=ToUpper(src);
  dst=ToUpper(dst);

  Fmt(dst,TRUE,FALSE,PT_FAT32);

  SPrintF(buf_s,"%c:/LT",src);
  SPrintF(buf_d,"%c:/LT",dst);
  CopyTree(buf_s,buf_d);

  SPrintF(buf_s,"%c:/Text",src);
  SPrintF(buf_d,"%c:/Text",dst);
  CopyTree(buf_s,buf_d);

  SPrintF(buf_s,"%c:/Windows",src);
  SPrintF(buf_d,"%c:/Windows",dst);
  CopyTree(buf_s,buf_d);

  SPrintF(buf_s,"%c:/Linux",src);
  SPrintF(buf_d,"%c:/Linux",dst);
  CopyTree(buf_s,buf_d);

  SPrintF(buf_s,"%c:/0000Boot",src);
  SPrintF(buf_d,"%c:/0000Boot",dst);
  CopyTree(buf_s,buf_d);

  SPrintF(buf_d,"%c:/Temp",dst);
  MkDir(buf_d);

  SPrintF(buf_d,"%c:/Temp/ScreenShots",dst);
  MkDir(buf_d);

  if (ts_has_tra_drv && dst==TS_TRA_DRV_LET) {
    SPrintF(buf_d,"%c:/Logs",dst);
    MkDir(buf_d);
    SPrintF(buf_d,"%c:/Logs/LTLogs",dst);
    MkDir(buf_d);
  }

  Drv(dst);
  CmpDrv(dst);
}

U0 PmtMyCopyDrv()
{
  U8 src,dst;
  "$FG,RED$\nCopy Src Drive:\n$FG$";
  src=DrvMySelect;
  "$FG,RED$\nCopy Dst Drive:\n$FG$";
  dst=DrvMySelect;
  MyCopyDrv(src,dst);
}

public U0 FileCursorRem(U8 *filename)
{
  Ltf *l;
  l=LtfRead(filename,LTFF_PLAIN_TEXT|LTFF_NO_CURSOR);
  LtfWrite(l);
  LtfDel(l);
}

public U0 FileCursorTop(U8 *filename)
{
  Ltf *l;
  l=LtfRead(filename,LTFF_PLAIN_TEXT);
  l->cur_entry=l->root.next;
  LtfWrite(l);
  LtfDel(l);
}

public U0 FilesCursorRem(U8 *mask="*")
{
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+t+f+F");
  LTDirEntry *tempm=FilesFind(mask,fuf_flags),*tempm1;
  while (tempm) {
    tempm1=tempm->next;
    FileCursorRem(tempm->full_name);
    Touch(tempm->full_name,"","",tempm->datetime);
    LTDirEntryDel(tempm);
    tempm=tempm1;
  }
}

public U0 FilesCursorTop(U8 *mask="*")
{
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+t+f+F");
  LTDirEntry *tempm=FilesFind(mask,fuf_flags),*tempm1;
  while (tempm) {
    tempm1=tempm->next;
    FileCursorTop(tempm->full_name);
    Touch(tempm->full_name,"","",tempm->datetime);
    LTDirEntryDel(tempm);
    tempm=tempm1;
  }
}

U0 UncollapseFile(U8 *filename,U64 *flags)
{
  nounusedwarn flags;
  Ltf *l=LtfRead(filename,LTFF_NO_CURSOR);
  LtfCollapse(FALSE,l);
  LtfWrite(l);
  LtfDel(l);
}

U0 UncollapseList(LTDirEntry *tempm1,U64 *flags)
{
  LTDirEntry *tempm2;
  while (tempm1 && !Bt(flags,FUf_CANCEL)) {
    tempm2=tempm1->next;
    if (tempm1->attr & LT_ATTR_DIR) {
      if (tempm1->sub) {
	"%S %s\n","ST_SCANNING_DIR",tempm1->full_name;
	UncollapseList(tempm1->sub,flags);
      }
    } else
      UncollapseFile(tempm1->full_name,flags);
    LTDirEntryDel(tempm1);
    tempm1=tempm2;
  }
}

U0 Uncollapse(U8 *files_find_mask=TEXT_FILE_MASK,U8 *fu_flags=NULL)
{
  U64 f=0;
  GetFFlags(&f,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&f,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  UncollapseList(FilesFind(files_find_mask,f),&f);
}

U0 PrepForDistribution(BoolI8 cont=FALSE)
{
  Touch("/LT/Doc/FileMgr.TXZ","+T");
  Touch("/LT/Doc/CtrlL.TXZ","+T");
  Touch("/LT/Accts/PersonalMenu.TXZ","+T");
  Del("/LT/Demo/WebLogExample/WebLog.TXZ");
  Del("/LT/Demo/GameStarters/EagleDive.BIZ");

//User
  Del("/LT/Accts/User/Registry.CPZ");

//Install
  Del("/LT/Accts/Install/Registry.CPZ");

//SafeMode
  Del("/LT/Accts/SafeMode/Registry.CPZ");

//TAD
  Touch("/LT/Accts/TAD/PersonalMenu.TXZ","+T");

//TS
  Touch("/LT/Accts/TS/PersonalMenu.TXZ","+T");

  Touch("/LT/Doc/Ed.TXZ","+T");

  DelTree("/LT/Demo/AcctExample");
  MkDir("/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/Adam3*.APZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/TSDistribute.APZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/TSAdam.APZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/TSIDComputer.APZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/TSIDDft.APZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TS/PersonalMenu.TXZ","/LT/Demo/AcctExample");
  Copy("/LT/Accts/TAD/Notes.TXZ","/LT/Demo/AcctExample");

  Cd("/LT");
  FilesCursorTop;

  DelTree("/LT/Demo/*.BI?");

  if (!cont) {
    Cd("/LT");
    S2T(SRC_FILE_MASK,"+r");
  }
  Move(WSD_DEF_FILENAME,WSD_DEF_FILENAME_Z);
}

U0 PrepForInHouse(BoolI8 cont=FALSE)
{
  Drv(TS_MAIN_DRV_LET);
  PrepForDistribution(cont);
  Auto(TS_CDROM_CFG);
  InstallBoot(TS_MAIN_DRV_LET,TS_CDROM_DRV_LET);
  Fmt(TS_IMG_DRV_LET,TRUE,FALSE,PT_LT);
  Drv(TS_IMG_DRV_LET);
  CopyTree(TS_MAIN_DRV ":/LT","/LT");
  CopyTree(TS_MAIN_DRV ":/Text","/Text");
  CopyTree(TS_MAIN_DRV ":/Windows","/Windows");
  CopyTree(TS_MAIN_DRV ":/Linux","/Linux");
  MkDir("/Temp");
  MkDir("/Temp/ScreenShots");
  CopyTree(TS_MAIN_DRV ":/0000Boot","/0000Boot");
}

U0 PrepForProduction(BoolI8 cont=FALSE)
{
  Drv(TS_MAIN_DRV_LET);
  if (!cont)
    PrepForDistribution;
  Auto(PROD_CDROM_CFG);
  InstallBoot(TS_MAIN_DRV_LET,TS_CDROM_DRV_LET);
  if (cont) {
    Drv(TS_IMG_DRV_LET);
    DelTree("/Text");
    DelTree("/Temp/ScreenShots");
  } else {
    Fmt(TS_IMG_DRV_LET,TRUE,FALSE,PT_LT);
    Drv(TS_IMG_DRV_LET);
    CopyTree(TS_MAIN_DRV ":/LT","/LT");
    CopyTree(TS_MAIN_DRV ":/Windows","/Windows");
    CopyTree(TS_MAIN_DRV ":/Linux","/Linux");
    MkDir("/Temp");
  }
  DelTree("/LT/Accts/TAD");
  DelTree("/LT/Accts/TS");
  CopyTree(TS_MAIN_DRV ":/0000Boot","/0000Boot");
  Del("/0000Boot/OldMBR.BIC");
}


U0 MakeInHouseCDImage(BoolI8 cont=FALSE)
{
  PrepForInHouse(cont);
  CDFileCreate(TS_ISO_NAME,TS_IMG_DRV ":/*",NULL,BOOTCD_BIZ);
  Drv(TS_MAIN_DRV_LET);
}

U0 MakeProductionCDImage(BoolI8 cont=FALSE)
{
  PrepForProduction(cont);
  CDFileCreate(TS_ISO_NAME,TS_IMG_DRV ":/*",NULL,BOOTCD_BIZ);
  Drv(TS_MAIN_DRV_LET);
}

U0 MakeBack()
{
  ChkAll;
  MyCopyDrv(TS_MAIN_DRV_LET,TS_BAK_DRV_LET);
  if (ts_has_tra_drv) {
    MyCopyDrv(TS_MAIN_DRV_LET,TS_TRA_DRV_LET);
    MakeInHouseCDImage;
    Copy(TS_ISO_NAME,TS_TRA_DRV ":LTTS.ISO");
    MakeProductionCDImage(TRUE);
    Copy(TS_ISO_NAME,TS_TRA_DRV ":LTCD.ISO");
  }
  ChkAll;
}

U0 TSVMNew()
{ //From CD-ROM to TSVM
  TSHDMount;
  DelTree(TS_MAIN_DRV ":/LT");
  CopyTree(TS_CDROM_DRV ":/LT",TS_MAIN_DRV ":/LT");
  CmpDrv(TS_MAIN_DRV_LET);
  DelTree(TS_BAK_DRV ":/LT");
  CopyTree(TS_CDROM_DRV ":/LT",TS_BAK_DRV ":/LT");
  CmpDrv(TS_BAK_DRV_LET);
}
