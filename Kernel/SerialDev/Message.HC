U0 InputFilterTask()
{
  CJob *tmpc,*tmpc1;
  Bool old_filter;
  I64 old_flags=GetRFlags;
  Fs->win_inhibit=WIG_USER_TASK_DFT;
  LBts(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  old_filter=LBts(&Fs->last_input_filter_task->task_flags,TASKf_FILTER_INPUT);
  LBEqu(&Fs->task_flags,TASKf_FILTER_INPUT,old_filter);
  while (TRUE) {
    CLI
    JobsHndlr(old_flags);
    tmpc1=&Fs->srv_ctrl.next_waiting;
    tmpc=tmpc1->next;
    if (tmpc==tmpc1)
      break;
    else {
      if (tmpc->job_code==JOBT_TEXT_INPUT) {
	QueRem(tmpc);
	SetRFlags(old_flags);
	try
		ExePrint("%s",tmpc->aux_str);
	catch
	  Fs->catch_except=TRUE;
	JobDel(tmpc);
      } else
	break;
    }
  }
  Fs->next_input_filter_task->last_input_filter_task=Fs->last_input_filter_task;
  Fs->last_input_filter_task->next_input_filter_task=Fs->next_input_filter_task;
  if (!old_filter)
    LBtr(&Fs->last_input_filter_task->task_flags,TASKf_FILTER_INPUT);
  SetRFlags(old_flags);
}

I64 ScanMsg(I64 *_arg1=NULL,I64 *_arg2=NULL,I64 mask=~1,CTask *task=NULL)
{/*Check for a message of type specified by a one in the mask.
Throw-out messages not in mask.
If no message fit mask, return NULL immediately.
Remove desired message, return [C:/Kernel/KernelA.HH.Z,3217] msg_code.
Note: This delivers messages from parent down to pop-up.
*/
  I64 res,old_flags;
  CJob *tmpc,*tmpc1;
  if (!task) task=Fs;
  old_flags=GetRFlags;
  tmpc1=&task->srv_ctrl.next_waiting;
  while (TRUE) {
    CLI
    if (task==Fs)
      JobsHndlr(old_flags);
    tmpc=tmpc1->next;
    if (tmpc==tmpc1)
      break;
    else {
      if (tmpc->job_code==JOBT_MSG) {
	QueRem(tmpc);
	SetRFlags(old_flags);
	res=tmpc->msg_code;
	if (_arg1)
	  *_arg1=tmpc->aux1;
	if (_arg2)
	  *_arg2=tmpc->aux2;
	JobDel(tmpc);
	if ((res!=MSG_KEY_DOWN || !(tmpc->aux2&SCF_KEY_DESC) ||
	      !Bt(&task->win_inhibit,WIf_SELF_KEY_DESC)) && Bt(&mask,res))
	  goto sm_done;
      }
    }
    SetRFlags(old_flags);
  }
  res=MSG_NULL;
  if (_arg1)
    *_arg1=0;
  if (_arg2)
    *_arg2=0;
  if (task->parent_task&&task->parent_task->popup_task==task) {
    SetRFlags(old_flags);
    return ScanMsg(_arg1,_arg2,mask,task->parent_task);
  }
sm_done:
  SetRFlags(old_flags);
  return res;
}

I64 FlushMsgs(CTask *task=NULL)
{//Throw away all messages.  Return count.
  I64 res=0,arg1,arg2;
  while (ScanMsg(&arg1,&arg2,~1,task))
    res++;
  return res;
}

I64 GetMsg(I64 *_arg1=NULL,I64 *_arg2=NULL,I64 mask=~1,CTask *task=NULL)
{//Wait for a message of type specified by a one in the mask.
//Throw-out all messages not in mask.
  //Returns [C:/Kernel/KernelA.HH.Z,3217] msg_code. See [C:/Demo/MsgLoop.HC,1].
  I64 res;
  if (!task) task=Fs;
  LBtr(&task->task_flags,TASKf_IDLE);
  while (!(res=ScanMsg(_arg1,_arg2,mask,task))) {
    LBts(&task->task_flags,TASKf_IDLE);
    Yield;
  }
  LBtr(&task->task_flags,TASKf_IDLE);
  return res;
}

I64 ScanChar()
{//Checks for [C:/Kernel/KernelA.HH.Z,3218] MSG_KEY_DOWN and returns 0 immediately if no key.
//Waits for [C:/Kernel/KernelA.HH.Z,3219] MSG_KEY_UP of non-zero [C:/Kernel/KernelA.HH.Z,3468] ASCII key and returns [C:/Kernel/KernelA.HH.Z,3468] ASCII if key.
  //[C:/Kernel/SerialDev/Message.HC.Z,37] ScanMsg() throws away other message types.
  I64 arg1a,arg2a,arg1b,arg2b;
  if (!ScanMsg(&arg1a,&arg2a,1<<MSG_KEY_DOWN)||!arg1a)
    return 0;
  else
    do GetMsg(&arg1b,&arg2b,1<<MSG_KEY_UP);
    while (!arg1b);  //Be careful of [C:/Kernel/KernelA.HH.Z,3543] SC_SHIFT and [C:/Kernel/KernelA.HH.Z,3544] SC_CTRL, etc.
  return arg1a;
}

Bool ScanKey(I64 *_ch=NULL,I64 *_scan_code=NULL,Bool echo=FALSE)
{//Checks for [C:/Kernel/KernelA.HH.Z,3218] MSG_KEY_DOWN and returns FALSE immediately if no key.
//Sets [C:/Kernel/KernelA.HH.Z,3468] ASCII and [C:/Doc/CharOverview.DD.Z,1] scan_code.
  //Removes key message and returns TRUE.
  //[C:/Kernel/SerialDev/Message.HC.Z,37] ScanMsg() throws away other message types.
  I64 ch=0,sc=0;
  if (ScanMsg(&ch,&sc,1<<MSG_KEY_DOWN)) {
    if (_ch) *_ch=ch;
    if (_scan_code) *_scan_code=sc;
    if (echo)
      PutKey(ch,sc);
    return TRUE;
  } else {
    if (_ch) *_ch=0;
    if (_scan_code) *_scan_code=0;
    return FALSE;
  }
}

I64 GetKey(I64 *_scan_code=NULL,Bool echo=FALSE,Bool raw_cursor=FALSE)
{//Waits for [C:/Kernel/KernelA.HH.Z,3218] MSG_KEY_DOWN message and returns [C:/Kernel/KernelA.HH.Z,3468] ASCII.
//Sets [C:/Doc/CharOverview.DD.Z,1] scan_code.
  //[C:/Kernel/SerialDev/Message.HC.Z,123] ScanKey() throws away other message types.
  I64 ch,sc;
  Bool cursor_on=FALSE;
  while (!ScanKey(&ch,&sc,FALSE)) {
    if (IsRaw && raw_cursor) {
      if (!cursor_on && ToI64(GetTSC*5/cnts.time_stamp_freq)&1) {
	'_';
	cursor_on=TRUE;
      } else if (cursor_on && !(ToI64(GetTSC*5/cnts.time_stamp_freq)&1)) {
	'' CH_BACKSPACE;
	cursor_on=FALSE;
      }
    }
    LBts(&Fs->task_flags,TASKf_IDLE);
    if (IsDbgMode) {
//We don't want interrupt-driven keyboard when in debugger
      //because that could have side-effects or crash, so we poll
      //keyboard when in debugger with interrupts off.
      PUSHFD
      CLI
      KbdMsHndlr(TRUE,FALSE);
      KbdMsgsQue;
      POPFD
    } else {
      LBts(&Fs->task_flags,TASKf_AWAITING_MSG);
      Yield;
    }
    LBtr(&Fs->task_flags,TASKf_IDLE);
  }
  if (IsRaw && raw_cursor && cursor_on)
    '' CH_BACKSPACE;
  if (echo)
    PutKey(ch,sc);
  if (_scan_code) *_scan_code=sc;
  return ch;
}

I64 GetChar(I64 *_scan_code=NULL,Bool echo=TRUE,Bool raw_cursor=FALSE)
{//Waits for non-zero [C:/Kernel/KernelA.HH.Z,3468] ASCII key.
//Sets [C:/Doc/CharOverview.DD.Z,1] scan_code.
  I64 ch1;
  do ch1=GetKey(_scan_code,FALSE,raw_cursor);
  while (!ch1);
  if (echo)
    "$PT$%c$FG$",ch1;
  return ch1;
}

U8 *GetStr(U8 *msg=NULL,U8 *dft=NULL,I64 flags=0)
{//Returns a [C:/Kernel/Mem/MAllocFree.HC.Z,387] MAlloc()ed prompted string.  See [C:/Kernel/KernelA.HH.Z,3765] Flags.
  U8 *st;
  if (msg)
    "" msg,dft;
  st=(*fp_getstr2)(flags);
  if (!*st) {
    Free(st);
    if (dft)
      return StrNew(dft);
    else
      return StrNew("");
  }
  return st;
}

I64 GetS(U8 *buf,I64 size,Bool allow_ext=TRUE)
{//Prompt into fixed length string. Size must include terminator.
  U8 *st;
  I64 ch,i=0;
  if (!size || !buf) return 0;
  if (allow_ext) {
    st=GetStr;
    if (StrLen(st)>size-1) {
      MemCpy(buf,st,size-1);
      buf[size-1]=0;
    } else
      StrCpy(buf,st);
    i=StrLen(buf);
    Free(st);
  } else {
    while (TRUE) {
      ch=GetChar(,FALSE,IsDbgMode);
      if (ch=='\n') {
	'' ch;
	break;
      } else if (ch==CH_BACKSPACE) {
	if (i>0) {
	  i--;
	  '' ch;
	}
      } else {
	if (i<size-1) {
	  buf[i++]=ch;
	  '' ch;
	}
      }
    }
    buf[i]=0;
  }
  return i;
}
