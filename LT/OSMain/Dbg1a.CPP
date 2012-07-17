BoolI64 IsDbgTask()
{
  TaskStruct *task=Fs->parent_task;
  if (TaskValidate(task) && task->dbg_task==Fs)
    return TRUE;
  else
    return FALSE;
}

BoolI64 ChkPtr(U0 *ptr)
{
  if (sys_heap_base<=ptr<=MAPPED_MEM_SPACE) {
    if (*MemPointAtPageTableEntry(ptr)&1)
      return TRUE;
    else
      return FALSE;
  } else if (ptr<sys_boot_base)
    return FALSE;
  else if (ptr<VGA_GRAPHIC_MEM_BASE)
    return TRUE;
  else
    return FALSE;
}

BoolI64 ChkOnStk(U0 *ptr,TaskStruct *task=NULL)
{
  BoolI8 result=FALSE;
  PushFD;
  Cli;
  if (task) {
    if (&task->stk->stk_base<=ptr<=(&task->stk->stk_base)(U8 *)+task->stk->stk_size)
      result=TRUE;
  } else if (sys_heap_base<=ptr<=sys_heap_limit)
    result=TRUE;
  PopFD;
  return result;
}

I64 UnusedStk(TaskStruct *task=NULL)
{
  I64 result;
  if (!task) task=Fs;
  PushFD;
  Cli;
  if (task==Fs)
    result=GetStkPtr()(U8 *)-(&task->stk->stk_base)(U8 *);
  else
    result=task->rsp(U8 *)-(&task->stk->stk_base)(U8 *);
  PopFD;
  return result;
}

U0 *Caller(U64 num=1)
//Returns the addr of the fun
//which called this one, or the caller
//of the caller, etc.
{
  U0 **rbp=GetBasePtr,**ptr;
  while (num--) {
    if (rbp>=*rbp)
      return NULL;
    rbp=*rbp;
    if (!ChkOnStk(rbp,Fs))
      return NULL;
  }
  ptr=rbp+1;
  return *ptr;
}

U0 *TaskCaller(TaskStruct *task=NULL,I64 num=0,BoolI8 saved_context=FALSE)
{
  U0 **ptr,**rbp,**rsp;
  if (!task) task=Fs;
  if (!saved_context && task==Fs)
    return Caller(num+1);
  if (!TaskValidate(task))
    return NULL;
  rbp=task->rbp;
  rsp=task->rsp;
  if (num) {
    while (ChkOnStk(rbp,task)) {
      ptr=rbp+1;
      if (! --num)
	return *ptr;
      if (rbp>=*rbp)
	break;
      rbp=*rbp;
    }
    return NULL;
  } else {
    if (task->rip==SYS_IDLE_POINT)
      return *rsp;
    else
      return task->rip;
  }
}
#define STK_REP_LEN	32

U0 StkRep(TaskStruct *task=NULL)
//Reports whats on the stk
{
  I64 i,j,add;
  U64 **rbp,**rsp;
  U64 *my_rsp[STK_REP_LEN];
  SysHashTable *old_hash=Fs->hash_table;
  if (!task) task=Fs;
  if (!TaskValidate(task))
    return;
  PushFD;
  Cli;
  if (task==Fs) {
    rbp=GetBasePtr;
    rsp=rbp+3;
    rbp=*rbp;
  } else {
    rsp=task->rsp;
    rbp=task->rbp;
  }
  if (task->rip==SYS_IDLE_POINT)
    add=*rsp;
  else
    add=task->rip;
  MemCpy(my_rsp,rsp,STK_REP_LEN*sizeof(U0 *));
  PopFD;
  Fs->hash_table=task->hash_table;
  for (i=0;i<STK_REP_LEN;i++) {
    "%08X [RSP+%04X]: %016X ",rsp+i,
      i*sizeof(U0 *),my_rsp[i];
    while (TRUE) {
      if (!(&task->stk->stk_base<=rbp<(&task->stk->stk_base)(U8 *)+task->stk->stk_size))
	break;
      j=rbp-rsp;
      if (j>=i)
	break;
      add=my_rsp[j+1];
      if (rbp>=my_rsp[j])
	break;
      rbp=my_rsp[j];
    }
    if (my_rsp[i]==add)
      "$FG,RED$";
    "%P$FG$\n",my_rsp[i];
  }
  NewLine;
  Fs->hash_table=old_hash;
}

U0 CallerRep(U0 **rbp=NULL,TaskStruct *task=NULL)
//Prints a report of calling routines
{
  U64 **ptr;
  if (!task) task=Fs;
  if (!rbp) rbp=GetBasePtr;
  while (ChkOnStk(rbp,task)) {
    ptr=rbp+1;
    "%08X:%08tX:%P\n",ptr,*ptr,*ptr;
    if (rbp>=*rbp)
      break;
    rbp=*rbp;
  }
}

U0 DbgStr(U8 *st,U64 mS=100)
//Shows a string using
//[C:/LT/OSMain/Misc1a.CPZ,325] Raw screen output for a fixed length
//of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  "DbgStr(";
  "" st;
  ')';
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 DbgNum(U64 num,U64 mS=100)
//Shows a hex num using
//[C:/LT/OSMain/Misc1a.CPZ,325] Raw screen output for a fixed length
//of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  "DbgNum(%016X)",num;
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 DbgF(F64 d,U64 mS=100)
//Shows a floating-point val using
//[C:/LT/OSMain/Misc1a.CPZ,325] Raw screen output for a fixed length
//of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  "DbgF(%15e)",d;
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 DbgAdd(U0 *add,U64 mS=100)
//Shows an addr using [C:/LT/OSMain/Misc1a.CPZ,325] Raw screen
//output for a length of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  "DbgAdd(%P)",add;
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 DbgD(U0 *add,U64 cnt=0x80,U64 mS=100)
//Dumps a block of mem using [C:/LT/OSMain/Misc1a.CPZ,325] Raw
//screen output for a fixed length
//of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  D(add,cnt);
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 DbgDm(U0 *add,U64 cnt=0x80,U64 mS=100)
//Dumps a block of mem using [C:/LT/OSMain/Misc1a.CPZ,325] Raw
//screen output for a fixed length
//of time.
{
  BoolI8 old_raw,old_input_filter;
  PushFD;
  Cli;
  old_raw=Raw(ON);
  old_input_filter=LBtr(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  Dm(add,cnt);
  BusyWait(mS<<10);
  PopFD;
  LBEqu(&Fs->task_flags,TASKf_INPUT_FILTER_TASK,old_input_filter);
  Raw(old_raw);
}

U0 D(U8 *buf,U64 cnt=0x80,BoolI8 show_offset=TRUE)
//Dump mem, showing offsets.
//See [C:/LT/Adam/Ltf/LtfMain.APZ,699] LtfD() for a live dump.
{
  U64 i,j,ch;
  U8 *b=buf;
  while (cnt) {
    if (show_offset)
      "%08X ",b-buf;
    else
      "%016X ",b;
    j= cnt>16 ? 16:cnt;
    for (i=0;i<j;i++)
      "%02X ",b[i];
    for (;i<16;i++)
      "   ";
    for (i=0;i<j;i++) {
      ch=b[i];
      if (ch<CH_SHIFT_SPACE || ch==CH_BACKSPACE)
	ch='.';
      '' ch;
      if (ch=='$')
	'' ch;
    }
    NewLine;
    cnt-=j;
    b+=j;
  }
}

U0 Dm(U8 *buf,U64 cnt=0x80)
//Show mem addr, not offsets
{
  D(buf,cnt,FALSE);
}

U0 Da(U0 **b,U64 cnt=0x10)
//Dump mem, showing symbolic addresses
{
  while (cnt-->0) {
    "%08X:%08X,%P\n",b,*b,*b;
    b++;
  }
}

U64 *TaskRegAdd(TaskStruct *task,I64 reg_num)
{
  switch (reg_num) {
    case CREG_RAX: return &task->rax;
    case CREG_RCX: return &task->rcx;
    case CREG_RDX: return &task->rdx;
    case CREG_RBX: return &task->rbx;
    case CREG_RSP: return &task->rsp;
    case CREG_RBP: return &task->rbp;
    case CREG_RSI: return &task->rsi;
    case CREG_RDI: return &task->rdi;
    case 8 : return &task->r8;
    case 9 : return &task->r9;
    case 10: return &task->r10;
    case 11: return &task->r11;
    case 12: return &task->r12;
    case 13: return &task->r13;
    case 14: return &task->r14;
    case 15: return &task->r15;
  }
  return NULL;
}

U0 Dr(TaskStruct *task=NULL)
{//Dump registers
  U64 i;
  if (!task) task=Fs;
  for (i=0;i<16;i++)
    "%3Z:%016X\n",i,"ST_U64_REGS",*TaskRegAdd(task,i);
  "RIP:%016X\n",task->rip;
}

U0 OverRideSingleUserSwap()
{
  BoolI8 old_single=SingleUser(OFF);
  Yield;
  SingleUser(old_single);
}

U0 DefineRegs()
{
  I64 i;
  U8 buf1[8],buf2[8];
  DefineLoad("RAX","Fs->rax");
  DefineLoad("RBX","Fs->rbx");
  DefineLoad("RCX","Fs->rcx");
  DefineLoad("RDX","Fs->rdx");
  DefineLoad("RSI","Fs->rsi");
  DefineLoad("RDI","Fs->rdi");
  DefineLoad("RBP","Fs->rbp");
  DefineLoad("RSP","Fs->rsp");
  DefineLoad("RIP","Fs->rip");
  for (i=8;i<16;i++) {
    SPrintF(buf1,"R%d",i);
    SPrintF(buf2,"Fs->r%d",i);
    DefineLoad(buf1,buf2);
  }
}

U8 *SysMGetS(U64 flags)
{
  nounusedwarn flags;
  U8 buf[512];
  GetS(buf,512,FALSE);
  return StrNew(buf);
}

U0 SysDbgExit()
{
  if (IsInDbg) {
    "Exit Dbg\n";
    CmdLineExit;
  }
}

U0 DbgExit()
{
  if (IsInDbg && ext[EXT_EXIT_DEBUGGER])
    CallExtNum(EXT_EXIT_DEBUGGER);
}

U0 SysDbgEnter(BoolI8 was_in_debugger)
{
  BoolI8 old_user_task,old_waiting_msg;
  U64 i,old_dbg_mgets;
  U8 buf[200];
  SysHashTable *tempht;

  if (IsRaw) {
    i=Fs->rip;
    Ui(buf,&i,64);
    PutS(buf);
  } else
    U(Fs->rip,1);

  if (!was_in_debugger) {
    old_waiting_msg=LBtr(&Fs->task_flags,TASKf_AWAITING_MSG);
    old_user_task=Bt(&Fs->task_flags,TASKf_LOCAL_USER);
    UserTaskInit;
    sys_focus_task=Fs;
    sys_cur_scan_code=0;
    old_dbg_mgets=fp_mgets;
    fp_mgets=&SysMGetS;
    while (!mouse_install_attempts)
      OverRideSingleUserSwap;

    tempht=HashTableNew(STD_HASH_SIZE);
    tempht->next=Fs->hash_table;
    Fs->hash_table=tempht;
    DefineRegs;
    UserTaskCont;
    Fs->hash_table=tempht->next;
    HashTableDel(tempht);

    fp_mgets=old_dbg_mgets;
    LBEqu(&Fs->task_flags,TASKf_LOCAL_USER,old_user_task);
    LBEqu(&Fs->task_flags,TASKf_AWAITING_MSG,old_waiting_msg);
  }
}

U0 Dbg(U8 *msg=NULL,U64 num=0,BoolI8 fault=FALSE)
//Enters the debugger.
//This can be used as a kernel panic.
{
  BoolI8 was_in_dbg,was_single_user,was_raw;
  U64 old_dc_flags=sys_raw_dc_flags;
  PushFD;
  Cli;
  if (Gs->num && mp_crash) {
    mp_cnt=1;
    mp_crash->cpu_num=Gs->num;
    mp_crash->task=Fs;
    mp_crash->msg=msg;
    mp_crash->msg_num=num;
    MPInt(I_MP_CRASH,0);
    SysHlt;
  }
 
  was_single_user=SingleUser(ON);
  was_raw=Raw(ON);
  was_in_dbg=InDbg(ON);
  sys_raw_dc_flags|=RDCF_SHOW_DOLLAR|RDCF_SCROLL;

	"\n\tLoseThos Debugger\n"
	"Use EdLite(\"name\"); for editing.\n"
	"\"DbgExit;\" or \"Exit;\" might work.\n\n";
  Beep(500,TRUE);
  if (msg) {
    "\n!!! %s",msg;
    if (num)
      "%016X",num;
    " !!!\n\n";
  }
  CallerRep;
  if (ext[EXT_DEBUGGER])
    CallExtNum(EXT_DEBUGGER,was_in_dbg);
  else {
    "No Debugger\n";
    while (TRUE);
  }
  InDbg(was_in_dbg);
  SingleUser(was_single_user);
  Raw(was_raw);
  sys_raw_dc_flags=old_dc_flags;
  PopFD;
  if (!fault && LBtr(&Fs->task_flags,TASKf_KILL_AFTER_DBG))
    Exit;
}

U0 Fault2()
{
//**Be careful not to swap-out and ruin the saved context**
  BoolI8 was_raw,was_single_user;
  U64  i,old_fault_num=sys_dbg_fault_num;
  U8 *old_fault_rip=sys_dbg_fault_rip;
  U64 old_dc_flags=sys_raw_dc_flags;
  if (Fs->fault_num==I_DIVIDE_BY_ZERO)
    throw(EXCEPT_DIVIDE_BY_ZERO,Fs->rip);
  was_single_user=SingleUser(ON);
  sys_focus_task=NULL;
  sys_dbg_fault_num=Fs->fault_num;
  if (Fs->fault_num==I_BPT)
    Fs->rip--;
  sys_dbg_fault_rip=Fs->rip;
  if (Fs->dbg_task)
    CallExtNum(EXT_RESUME_DBG);  //See [::/LT/Adam/Ltf/LtfIDE.APZ,IDEResumeDbg] IDEResumeDbg
  else {
    was_raw=Raw(ON);
    sys_raw_dc_flags|=RDCF_SHOW_DOLLAR|RDCF_SCROLL;
    "Task \"";
    "" Fs->task_title;
    "\"\n";
    "Fault:0x%02X %Z\t\tErr Code:%08X\n",Fs->fault_num,Fs->fault_num,"ST_INT_NAMES",Fs->fault_err_code;
    "RIP:%08X",Fs->rip; //Sometimes crashes on %P, so do this first
    ":%P  RSP:%08X\n",Fs->rip,Fs->rsp;
    if (Fs->fault_num==I_PAGE_FAULT) {
      asm {
	MOV_RAX_CR2
      }
      i=GetRAX;
      "Fault Addr:%08X:%P\n",i,i;
    }
    Dbg(NULL,0,TRUE);
    Raw(was_raw);
    sys_raw_dc_flags=old_dc_flags;
  }
  sys_dbg_fault_num=old_fault_num;
  sys_dbg_fault_rip=old_fault_rip;
  SingleUser(was_single_user);
  if (LBtr(&Fs->task_flags,TASKf_KILL_AFTER_DBG))
    Exit;
}
