//Change anything you want in this
//acct file or others.

U0 UserStartUp()
{ //Run each time a user a spawned
  UseTermLtf("PersonalMenu.TXZ");
  LtfCurAttr(WHITE<<4+BLUE);
  Type("::/LT/Doc/Start.TXZ");
  LBts(&Fs->display_flags,DISPLAYf_SHOW);
  WinZBufUpdate;
  if (!IsSysDeterministic)
    Preempt(ON);
}

U0 ServantStartUp()
{ //Run each time a servant task is spawned.
  UseTermLtf(NULL);
  LBts(&Fs->display_flags,DISPLAYf_SHOW);
  WinZBufUpdate;
  if (!IsSysDeterministic)
    Preempt(ON);
}

U0 StartUpTasks(U64)
{
  U8 buf[136];
  TaskStruct *task1,*task2;

  TaskLocalInit; //needed for disk access
  if (!IsSysDeterministic)
    Preempt(ON);	//off by default
  Silent(ON);	//no output to screen

  try {
    task1=SpawnUser;
    task2=SpawnUser;
    WinToTop(task1);
    WinTileVert;

    if (!IsSysDeterministic)
      PopUp("#include \"::/LT/Adam/Start.CPZ\";");

    SPrintF(buf,"Boot Time:%h17Ts\n",BootTime);
    AdamLog(buf);

//    WsInit("/LT/" TEXT_FILE_MASK ";~*Bible.TXZ");

//This will prefetch.  You might want
//to [C:/LT/OSMain/Dsk/DskAddDev.CPZ,159] DrvEnable('',OFF) for other
//FAT32 partitions.
//  if (!IsSysDeterministic)
//	CacheLocalBlkDevDirs;

  } catch
    Fs->catch_except=TRUE;
}

//This must be spawned because start-up files
// may invoke the Adam() cmd.
Spawn(&StartUpTasks,NULL,"Spawn Users");
"\nOS Compile Time:%D %T\n",sys_compile_time,sys_compile_time;
