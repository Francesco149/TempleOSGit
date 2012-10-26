//The CFG defines are [::/SparrowOS/Demo/AcctExample/TSAdam.CPP.Z,1] Here.

U0 TSHDMount()
{
  Auto(CFG_HARD_DRVS "\n");
  Mount;
}

U0 ShowBootFileDate(U8 drv_let)
{
  CDirEntry de;
  U8 *name=MSPrintF("%c:/OSMain.BIN.C",drv_let);
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
  ShowBootFiles;
  "Drive:";
  result=ToUpper(GetChar);
  NewLine;
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
  Cd("/SparrowOS/OSMain");
  Auto(TS_MENU_CFG);
  if (!Cmp("OSMain","OSMain",NULL,':'))
    Boot("OSMain.BIN.Z");
}

U0 UpdateAndCompile(U8 src,U8 dst)
{
  U8 buf_s[128],buf_d[128];
  if (src==dst) return;

  SPrintF(buf_s,"%c:/SparrowOS",src);
  SPrintF(buf_d,"%c:/SparrowOS",dst);
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
  SPrintF(buf_s,"%c:/SparrowOS/*",src);
  SPrintF(buf_d,"%c:/SparrowOS/*",dst);
  MergeChk(buf_d,buf_s,"+r+d");
  Drv(dst);
}

U0 MyCopyDrv(U8 src,U8 dst)
{
  U8 buf_s[128],buf_d[128];
  src=ToUpper(src);
  dst=ToUpper(dst);

  Fmt(dst,TRUE,FALSE,PT_FAT32);

  SPrintF(buf_s,"%c:/SparrowOS",src);
  SPrintF(buf_d,"%c:/SparrowOS",dst);
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
    SPrintF(buf_d,"%c:/Logs/SPARROWOSLogs",dst);
    MkDir(buf_d);
  }
  DocClear;
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
  CDoc *doc;
  doc=DocRead(filename,DOCF_PLAIN_TEXT|DOCF_NO_CURSOR);
  DocWrite(doc);
  DocDel(doc);
}

public U0 FileCursorTop(U8 *filename)
{
  CDoc *doc;
  doc=DocRead(filename,DOCF_PLAIN_TEXT);
  doc->cur_entry=doc->root.next;
  DocWrite(doc);
  DocDel(doc);
}

public U0 FilesCursorRem(U8 *mask="*")
{
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+t+f+F");
  CDirEntry *tempm=FilesFind(mask,fuf_flags),*tempm1;
  while (tempm) {
    tempm1=tempm->next;
    FileCursorRem(tempm->full_name);
    Touch(tempm->full_name,"","",tempm->datetime);
    CDirDel(tempm);
    tempm=tempm1;
  }
}

public U0 FilesCursorTop(U8 *mask="*")
{
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+t+f+F");
  CDirEntry *tempm=FilesFind(mask,fuf_flags),*tempm1;
  while (tempm) {
    tempm1=tempm->next;
    FileCursorTop(tempm->full_name);
    Touch(tempm->full_name,"","",tempm->datetime);
    CDirDel(tempm);
    tempm=tempm1;
  }
}

U0 UncollapseFile(U8 *filename,U64 *)
{
  CDoc *doc=DocRead(filename,DOCF_NO_CURSOR);
  DocCollapse(FALSE,doc);
  DocWrite(doc);
  DocDel(doc);
}

U0 UncollapseList(CDirEntry *tempm1,U64 *flags)
{
  CDirEntry *tempm2;
  while (tempm1 && !Bt(flags,FUf_CANCEL)) {
    tempm2=tempm1->next;
    if (tempm1->attr & _ATTR_DIR) {
      if (tempm1->sub) {
	"%S %s\n","ST_SCANNING_DIR",tempm1->full_name;
	UncollapseList(tempm1->sub,flags);
      }
    } else
      UncollapseFile(tempm1->full_name,flags);
    CDirDel(tempm1);
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

U0 CopySongDir(U8 *files_find_mask,U8 threshold)
{
  CDoc *s;
  CDocEntry *doc_e;
  CDirEntry *tempm,*tempm1;
  tempm1=tempm=FilesFind(files_find_mask);
  while (tempm1) {
    s=DocRead(tempm1->full_name);
    doc_e=s->root.next;
    if (doc_e->type_u8==DOCT_TEXT && doc_e->tag[2]>=threshold)
      Copy(tempm1->full_name,"/SparrowOS/Apps/MusicOrgan/Examples");
    tempm1=tempm1->next;
  }
  CDirListDel(tempm);
}

public U0 CopySongs(U8 threshold='7')
{
  DelTree("/SparrowOS/Apps/MusicOrgan/Examples");
  MkDir("/SparrowOS/Apps/MusicOrgan/Examples");
  CopySongDir("/SparrowOS/Accts/TS/TAD/MusicOrgan/*.CPP.Z",threshold);
}

U0 PrepForDistribution()
{
  Del("/SparrowOS/Demo/WebLogExample/WebLog.TXT.Z");
  Del("/SparrowOS/Demo/GameStarters/EagleDive.BIN.Z");
  DelTree("/SparrowOS/Accts/*Temp.TXT.Z");
  CopySongs;

//User
  Del("/SparrowOS/Accts/User/Registry.CPP.Z");

//Install
  Del("/SparrowOS/Accts/Install/Registry.CPP.Z");

//SafeMode
  Del("/SparrowOS/Accts/SafeMode/Registry.CPP.Z");

  Touch("/SparrowOS/Accts/PersonalMenu.TXT.Z","+T");
//TS
  Touch("/SparrowOS/Accts/TS/PersonalMenu.TXT.Z","+T");

  DelTree("/SparrowOS/Demo/AcctExample");
  CopyTree("/SparrowOS/Accts/TS","/SparrowOS/Demo/AcctExample");
  DelTree("/SparrowOS/Demo/AcctExample/TAD/MusicOrgan");
  DelTree("/SparrowOS/Demo/AcctExample/Wb2");
  Del("/SparrowOS/Demo/AcctExample/TAD/Test*");

  Cd("/SparrowOS");
  FilesCursorTop;

  DelTree("/SparrowOS/Demo/*.BI?");

  Cd("/SparrowOS");
  S2T(SRC_FILE_MASK,"+r");
  Move(WSD_DEF_FILENAME,WSD_DEF_FILENAME_Z);
}

U0 PrepForInHouse()
{
  Drv(TS_MAIN_DRV_LET);
  PrepForDistribution;
  Auto(TS_CDROM_CFG);
  InstallBoot(TS_MAIN_DRV_LET,TS_CDROM_DRV_LET);
  Fmt(TS_IMG_DRV_LET,TRUE,FALSE,PT_NATIVE);
  Drv(TS_IMG_DRV_LET);
  CopyTree(TS_MAIN_DRV ":/SparrowOS","/SparrowOS");
  CopyTree(TS_MAIN_DRV ":/Windows","/Windows");
  CopyTree(TS_MAIN_DRV ":/Linux","/Linux");
  MkDir("/Temp");
  MkDir("/Temp/ScreenShots");
  CopyTree(TS_MAIN_DRV ":/0000Boot","/0000Boot");
  Del("/0000Boot/BootMHD2.BIN.C");
}

U0 MakeSymbolFiles()
{
  CDoc *doc;
  U8 *st;
  if (ts_has_tra_drv) {
    DocClear;
    DocMax;
    HashRep(Fs->hash_table,HTG_ALL,NULL,"-p",0,NULL);
    if (doc=DocCur) {
      st=StrNew(doc->filename.name);
      StrCpy(doc->filename.name,TS_TRA_DRV ":/Wb/Accts/TS/Wb2/SymbolsAlpha.TXT.Z");
      DocWrite(doc);
      StrCpy(doc->filename.name,st);
      Free(st);
    }
    DocClear;
    DocMax;
    HashRep(Fs->hash_table,HTG_ALL,NULL,"-p+m",0,NULL);
    if (doc=DocCur) {
      st=StrNew(doc->filename.name);
      StrCpy(doc->filename.name,TS_TRA_DRV ":/Wb/Accts/TS/Wb2/SymbolsAddress.TXT.Z");
      DocWrite(doc);
      StrCpy(doc->filename.name,st);
      Free(st);
    }
    DocClear;
  }
}

U64 MakeLineCntFile()
{
  U8 *st;
  CDoc *doc;
  CDocEntry *tree_branch,*start_indent,*end_indent;

  DocClear;
  Drv(TS_MAIN_DRV_LET);
  PrepForDistribution;

  DocMax;
  DocClear;
  Cd(TS_MAIN_DRV ":/SparrowOS");
  result=LineCntRep;
  if (doc=DocCur) {
    st=StrNew(doc->filename.name);
    StrCpy(doc->filename.name,TS_MAIN_DRV ":/SparrowOS/Accts/TS/Wb2/LineCnt.TXT.Z");
    DocWrite(doc);
    StrCpy(doc->filename.name,st);
    Free(st);
  }
  DocClear;

  doc=DocRead(TS_MAIN_DRV ":/SparrowOS/Adam/DocDefines.CPP.Z");
  DocTreeFind(doc,"LineCnt",&tree_branch,&start_indent,&end_indent);
  DocCut(doc,start_indent->next,end_indent->last);
  doc->cur_entry=start_indent->next;
  doc->cur_data_col=0;
  DocPrintF(doc,"DefinePrintF(\"DD_SPARROWOS_LOC\",\"%,d\");\n",result);

  st=MSPrintF("%,d",result);
  DefineLoad("DD_SPARROWOS_LOC",st);
  Free(st);

  DocWrite(doc);
  DocDel(doc);

  "Total LOC:%12,d\n\n",result;
}

U0 PrepForProduction(U8 *auto_cfg)
{
  Drv(TS_MAIN_DRV_LET);
  PrepForDistribution;
  Auto(auto_cfg);
  InstallBoot(TS_MAIN_DRV_LET,TS_CDROM_DRV_LET);
  Fmt(TS_IMG_DRV_LET,TRUE,FALSE,PT_NATIVE);
  Drv(TS_IMG_DRV_LET);
  CopyTree(TS_MAIN_DRV ":/SparrowOS","/SparrowOS");
  CopyTree(TS_MAIN_DRV ":/Windows","/Windows");
  CopyTree(TS_MAIN_DRV ":/Linux","/Linux");
  MkDir("/Temp");
  DelTree("/SparrowOS/Accts/TS");
  CopyTree(TS_MAIN_DRV ":/0000Boot","/0000Boot");
  Del("/0000Boot/OldMBR.BIN.C");
  Del("/0000Boot/BootMHD2.BIN.C");
}

U0 MakeInHouseCDImage()
{
  PrepForInHouse;
  CDFileCreate(TS_ISO_NAME,TS_IMG_DRV ":/*",NULL,BOOTCD_BIN_Z);
  Drv(TS_MAIN_DRV_LET);
}

U0 MakeProductionCDImage()
{
  PrepForProduction(PROD_CDROM_CFG);
  CDFileCreate(TS_ISO_NAME,TS_IMG_DRV ":/*",NULL,BOOTCD_BIN_Z);
  Drv(TS_MAIN_DRV_LET);
}

U0 TSWebFile(U8 *name)
{
  U8 *name2;
  CDirEntry de;
  if (FileFind(name,&de)) {
    "File:%s\n",name;
    UncollapseFile(name,NULL);
    Touch(name,"",NULL,de.datetime);
    name2=ChgExt(name,"html");
    ToHtml(name,name2,
	"<title>The Sparrow Operating System</title>\n"
	"<meta name=\"keywords\" content=\"Operating System,64-Bit,64 Bit,Sparrow,OS,SparrowOS,Free,Open Source,Public Domain,x86_64\">\n"
	);
    Touch(name2,"",NULL,de.datetime);
    Free(name2);
  }
}

U0 MakeWebSite()
{
  CDirEntry *tempm,*tempm2;
  if (ts_has_tra_drv) {
    DocClear;
    Cd(TS_TRA_DRV ":/Wb");
    tempm=tempm2=FilesFind(TEXT_FILE_MASK,FUF_RECURSE|FUF_FLATTEN_TREE|FUF_JUST_FILES);
    while (tempm) {
      TSWebFile(tempm->full_name);
      tempm=tempm->next;
    }
    CDirListDel(tempm2);
    Drv(TS_MAIN_DRV_LET);
  }
}

U0 ToWeb(U8 *name)
{
  U8 *st,*st2;
  if (ts_has_tra_drv) {
    st=FileNameAbs(name);
    if (!StrNCmp(st+1,":/SparrowOS/",12)) {
      st2=MSPrintF(TS_TRA_DRV":/Wb/%s",st+13);
      "%s\n",st2;
      Copy(name,st2);
      TSWebFile(st2);
      Free(st2);
    }
    Free(st);
  }
}

U0 MakeBack()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WordStat;
  WinMax;
  WinBorder;
  ChkAll;
  MyCopyDrv(TS_MAIN_DRV_LET,TS_BAK_DRV_LET);
  if (ts_has_tra_drv) {
    MakeLineCntFile;
    MyCopyDrv(TS_MAIN_DRV_LET,TS_TRA_DRV_LET);
    DelTree (TS_TRA_DRV  ":/Wb");
    CopyTree(TS_MAIN_DRV ":/SparrowOS",TS_TRA_DRV ":/Wb");
    DocClear;
    MakeInHouseCDImage;
    DocClear;
    Copy(TS_ISO_NAME,TS_TRA_DRV ":/Wb/SparrowOSTS.ISO");
    MakeProductionCDImage;
    DocClear;
    Copy(TS_ISO_NAME,TS_TRA_DRV ":/Wb/SparrowOSCD.ISO");
    Cd(TS_MAIN_DRV ":/SparrowOS");
    if (LinkChk)
      throw;
    MakeSymbolFiles;
    MakeWebSite;
  }
  ChkAll;
  SettingsPop;
}

U0 TSVMNew()
{ //From CD-ROM to TSVM
  TSHDMount;
  DelTree(TS_MAIN_DRV ":/SparrowOS");
  CopyTree(TS_CDROM_DRV ":/SparrowOS",TS_MAIN_DRV ":/SparrowOS");
  CmpDrv(TS_MAIN_DRV_LET);
  DelTree(TS_BAK_DRV ":/SparrowOS");
  CopyTree(TS_CDROM_DRV ":/SparrowOS",TS_BAK_DRV ":/SparrowOS");
  CmpDrv(TS_BAK_DRV_LET);
  Reboot;
}

