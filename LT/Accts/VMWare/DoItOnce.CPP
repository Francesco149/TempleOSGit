U0 InstallWizard()
{
  TaskStruct *task=Fs->next_task;
  U8 *acct_name;
  U8 *st,*new_home_str;

  while (task->user_num!=2 && task!=Fs)
    task=task->next_task;
  XTalk(task,"Ed(\"::/LT/Doc/Install.TXZ\");\n");

  "Do you wish to install?  This better be a virtual machine because "
  "you will lose all data.\n";

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
    XTalkWithWait(task,"PrtDsk;\nYC2P0x1F0\n0x3F4\n0YYY");
    XTalkWithWait(task,"0x%X\nY\nY\n",prt_dsk_dsk_size/2);
    NewLine;
    Fmt('C',TRUE,FALSE,PT_FAT32);
    Fmt('D',TRUE,FALSE,PT_FAT32);
    st=MSPrintF("%C:/LT",dft_drv);
    CopyTree(st,"C:/LT");
    Free(st);
    MkDir("C:/0000Boot");
    MkDir("C:/Temp");

    new_home_str=MSPrintF("C:/LT/Accts/%s",acct_name);
    MkDir(new_home_str);
    XTalkWithWait(task,
    "InstallBoot('C');\n"
    "B10x20000\n"
    "C"
    "2p"
    "0x1F0\n"
    "0x3F4\n"
    "0"
    "\n"   //Exit Drives
    "%s\n" //Acct
    "\n"   //Dsk Cache
	"\n",	//Options
	acct_name);
    NewLine;
    Auto("0x1F0\n0x3F4\n0\n");
    InstallMasterBoot('C');

    CopyTree("C:/","D:/");
    new_home_str=MSPrintF("D:/LT/Accts/%s",acct_name);
    MkDir(new_home_str);
    XTalkWithWait(task,
    "InstallBoot('D');\n"
    "B10x20000\n"
    "C"
    "2p"
    "0x1F0\n"
    "0x3F4\n"
    "0"
    "\n"   //Exit Drives
    "%s\n" //Acct
    "\n"   //Dsk Cache
	"\n",	//Options
	acct_name);
    Reboot;
  }
}

InstallWizard;

