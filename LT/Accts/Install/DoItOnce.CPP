U0 InstallWizard()
{
  TaskStruct *task=Fs->next_task;
  I64 unit;
  U8 ch,*base0,*base1,*ms,*dst,*acct_name;
  U8 *st1,*st2,*new_home_str;
  BoolI8 mbr;
  ATARepStruct *root=NULL,*tempha;
  I64 num_hints;

  while (task->user_num!=2 && task!=Fs)
    task=task->next_task;
  XTalk(task,"Ed(\"::/LT/Doc/Install.TXZ\");\n");

  "$CM,0,10$This wizard works if you have "
  "a partition ready.  For a new install, answer '$FG,GREEN$Yes$FG$' "
  "to '$FG,GREEN$format?$FG$'.  For an upgrade, answer '$FG,GREEN$No$FG$' to '$FG,GREEN$format?$FG$' "
  " and '$FG,GREEN$No$FG$' to '$FG,GREEN$Delete /LT directory?$FG$'.\n\n"
  "You can partition the drive or compile the kernel "
  "with more options if you do it by hand, not using this wizard.  The LoseThos "
  "disk partitioner, [C:/LT/Adam/Dsk/Dsk2b.APZ,86] PrtDsk(), doesn't play well with others.\n\n"
  "Continue ";

  if (YorN) {
    while (TRUE) {
      "Make-up a Log-In Name (account).\n";
      acct_name=PmtStr("User Acct Name (%s): ","User");
      if (FileNameChk(acct_name))
	break;
      Free(acct_name);
    }
    task=SpawnUser;
    task->win_top=Fs->win_top;
    task->win_bottom=(Fs->win_top+Fs->win_bottom)>>2-1;
    Fs-> win_top=task->win_bottom+3;
    task->win_right=Fs->win_right;
    task->win_left=Fs->win_left;
    TaskDerivedValsUpdate(task);
    TaskDerivedValsUpdate;
    WinToTop(Fs);
    XTalk(task,"Mount;\n");
    XTalk(task,"C");
    XTalk(task,"2 ");

    num_hints=ATARep(FALSE,&root);
    "\nInclude '$FG,PURPLE$0x$FG$' for hexidecimal numbers.\n\n";
    while (TRUE) {
      base0=PmtStr("Hard Drive I/O Port Base0  : ");
      if (0<A2I(base0)<=0xFFFF)
	break;
      Free(base0);
    }
    if (1<=A2I(base0)<=num_hints) {
      tempha=ATARepFind(&root,A2I(base0));
      Free(base0);
      base0=MSPrintF("0x%X",tempha->base0);
      base1=MSPrintF("0x%X",tempha->base1);
      ms   =MSPrintF("0x%X",tempha->unit);
      unit=A2I(ms);
      Free(ms);
    } else {
      while (TRUE) {
	base1=PmtStr("Hard Drive I/O Port Base1  : ");
	if (0<A2I(base1)<=0xFFFF)
	  break;
	Free(base1);
      }
      while (TRUE) {
	ms =PmtStr("Unit--$FG,PURPLE$0$FG$=Master or $FG,PURPLE$1$FG$=Slave: ","0");
	unit=A2I(ms);
	Free(ms);
	if (0<=unit<=1)
	  break;
      }
    }
    LinkedListDel(root);
    XTalk(task,"%s\n",base0);
    XTalk(task,"%s\n",base1);
    XTalk(task,"%C",'0'+unit);
    XTalkWithWait(task,"\n");
    PrtRep;
    while (TRUE) {
      dst=PmtStr("\nDestination Partition Letter: ");
      if ('A'<=ToUpper(*dst)<='Z')
	break;
      Free(dst);
    }

    NewLine;
    st1=MSPrintF("%C:/LT",dft_drv);
    st2=MSPrintF("%C:/LT",*dst);

    "$FG,RED$Format %C Partition?$FG$\n",*dst;
    if (YorN) {
      NewLine;
      while (TRUE) {
	"$FG,PURPLE$1$FG$) Use FAT32\n"
	"$FG,PURPLE$2$FG$) Use LoseThos Native\n"
	"\nFile System Type: ";
	ch=GetChar;
	NewLine;
	if ('1'<=ch<='2')
	  break;
      }
      if (ch=='1')
	Fmt(*dst,TRUE,FALSE,PT_FAT32);
      else
	Fmt(*dst,TRUE,FALSE,PT_LT);
    } else {
      "\n$BK,1$WARNING$BK,0$ if you answer yes to the following "
      "you will lose all LoseThos files in your /LT directory tree.\n"
      "Delete %s Directory ",st2;
      if (YorN)
	DelTree(st2);
      NewLine;
    }
    CopyTree(st1,st2);
    st2=MSPrintF("%C:/0000Boot",*dst);
    MkDir(st2);
    Free(st2);
    st2=MSPrintF("%C:/Temp",*dst);
    MkDir(st2);
    Free(st2);

    new_home_str=MSPrintF("%C:/LT/Accts/%s",*dst,acct_name);
    MkDir(new_home_str);
    XTalkWithWait(task,
    "InstallBoot('%C');\n"
    "B10x20000\n"
    "C"
    "2p"
    "%s\n"
    "%s\n"
    "%C"
    "\n"   //Exit Drives
    "%s\n" //Acct
    "\n"   //Dsk Cache
	"\n",	//Options
	*dst,base0,base1,'0'+unit,acct_name);
    "$FG,RED$Install Master Boot loader?$FG$";
    mbr=YorN;

    if (mbr) {
      NewLine;
      Auto("%s\n%s\n%C\n",base0,base1,'0'+unit);
      InstallMasterBoot(*dst);
    }

    Free(new_home_str);
    Free(acct_name);
    Free(base0);
    Free(base1);
    Free(ms);
    Free(dst);

    "Reboot Now ";
    if (YorN)
      Reboot;

    Fs->win_top=task->win_top;
    Kill(task);
  }
}

InstallWizard;

