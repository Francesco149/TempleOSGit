U0 InputFilterTask()
{
  U8 *st;
  CTaskCmd *tempc,*tempc1;
  BoolI8 old_filter;
  U64 old_flags=GetFlags;
  LBts(&Fs->task_flags,TASKf_USER);
  TaskInitExt;
  LBts(&Fs->task_flags,TASKf_INPUT_FILTER_TASK);
  old_filter=LBts(&Fs->last_input_filter_task->task_flags,TASKf_FILTER_INPUT);
  LBEqu(&Fs->task_flags,TASKf_FILTER_INPUT,old_filter);
  while (TRUE) {
    ServantCmdsHandler;
    Cli;
    tempc1=&Fs->next_servant_cmd;
    tempc=tempc1->next;
    if (tempc==tempc1)
      break;
    else {
      if (tempc->cmd_code==TASKCT_TEXT_INPUT) {
	QueRem(tempc);
	SetFlags(old_flags);
	st=tempc->data;
	Free(tempc);
	if (!IsSysDeterministic)
	  Preempt(ON);
	try
	  ExePutS(st);
	catch
	  Fs->catch_except=TRUE;
	Preempt(OFF);
	Free(st);
      } else
	break;
    }
  }
  Fs->next_input_filter_task->last_input_filter_task=Fs->last_input_filter_task;
  Fs->last_input_filter_task->next_input_filter_task=Fs->next_input_filter_task;
  if (!old_filter)
    LBtr(&Fs->last_input_filter_task->task_flags,TASKf_FILTER_INPUT);
  SetFlags(old_flags);
}

I64 ScanMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,CTask *task=NULL)
{
  U64 old_flags;
  CTaskCmd *tempc,*tempc1;
  if (!task) task=Fs;
  old_flags=GetFlags;
  tempc1=&task->next_servant_cmd;
  while (TRUE) {
    if (task==Fs)
      ServantCmdsHandler;
    Cli;
    tempc=tempc1->next;
    if (tempc==tempc1)
      break;
    else {
      if (tempc->cmd_code==TASKCT_MSG) {
	QueRem(tempc);
	SetFlags(old_flags);
	result=tempc->msg_code;
	if (_p1)
	  *_p1=tempc->p1;
	if (_p2)
	  *_p2=tempc->p2;
	Free(tempc);
	if ((result!=MSG_KEY_DOWN || !(tempc->p2&SCF_KEY_DESC) ||
	  Bt(&mask,MSGF_ALLOW_KEY_DESC)) && Bt(&mask,result))
	    goto sm_done;
      }
    }
    SetFlags(old_flags);
  }
  result=MSG_NULL;
  if (_p1)
    *_p1=0;
  if (_p2)
    *_p2=0;
  if (task->parent_task&&task->parent_task->popup_task==task) {
    SetFlags(old_flags);
    return ScanMsg(_p1,_p2,mask,task->parent_task);
  }
sm_done:
  SetFlags(old_flags);
}

U64 FlushMsgs(CTask *task=NULL)
{
  U64 result=0,p1,p2;
  while (ScanMsg(&p1,&p2,~1,task))
    result++;
}

I64 GetMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,CTask *task=NULL)
{
  BoolI8 old_preempt=Preempt(OFF);
  if (!task) task=Fs;
  LBtr(&task->task_flags,TASKf_IDLE);
  while (!(result=ScanMsg(_p1,_p2,mask,task))) {
    LBts(&task->task_flags,TASKf_IDLE);
    Yield;
  }
  LBtr(&task->task_flags,TASKf_IDLE);
  Preempt(old_preempt);
}

U64 ScanChar()
{
  I64 p1a,p2a,p1b,p2b;
  if (!ScanMsg(&p1a,&p2a,1<<MSG_KEY_DOWN)||!p1a)
    return 0;
  else
    do GetMsg(&p1b,&p2b,1<<MSG_KEY_UP);
    while (!p1b);
  return p1a;
}

BoolI64 ScanKey(U64 *_ch=NULL,U64 *_scancode=NULL,BoolI8 echo=FALSE)
{
  U64 ch=0,sc=0;
  if (ScanMsg(&ch,&sc,1<<MSG_KEY_DOWN)) {
    if (_ch) *_ch=ch;
    if (_scancode) *_scancode=sc;
    if (echo)
      PutKey(ch,sc);
    return TRUE;
  } else {
    if (_ch) *_ch=0;
    if (_scancode) *_scancode=0;
    return FALSE;
  }
}

U64 GetKey(U64 *_scancode=NULL,BoolI8 echo=FALSE,BoolI8 raw_cursor=FALSE)
{
  U64 ch,sc;
  BoolI8 old_preempt=Preempt(OFF),cursor_on=FALSE;

  while (!ScanKey(&ch,&sc,FALSE)) {
    if (IsRaw && raw_cursor) {
      if (!cursor_on && Blink(,tNP)) {
	'' CH_SOLID;
	cursor_on=TRUE;
      } else if (cursor_on && !Blink(,tNP)) {
	'' CH_BACKSPACE;
	cursor_on=FALSE;
      }
    }
    LBts(&Fs->task_flags,TASKf_IDLE);
    if (IsInDbg) {
      //We don't want interrupt-driven keyboard when in debugger
      //because that could have side-effects or crash, so we poll
      //keyboard when in debugger with interrupts off.
      PushFD;
      Cli;
      KbdMouseHandler(TRUE,FALSE);
      KbdQueMsgs;
      PopFD;
      if (old_preempt && GetFlags&1<<RFLAGS_INTERRUPTS)
	Yield;
    } else {
      LBts(&Fs->task_flags,TASKf_AWAITING_MSG);
      Yield;
    }
    LBtr(&Fs->task_flags,TASKf_IDLE);
  }
  if (IsRaw && raw_cursor && cursor_on)
    '' CH_BACKSPACE;
  Preempt(old_preempt);
  if (echo)
    PutKey(ch,sc);
  if (_scancode) *_scancode=sc;
  return ch;
}

U64 GetChar(U64 *_scancode=NULL,BoolI8 echo=TRUE,BoolI8 raw_cursor=FALSE)
{
  U64 ch1;
  do ch1=GetKey(_scancode,FALSE,raw_cursor);
  while (!ch1);
  if (echo)
    "$FU$%c$FG$",ch1;
  return ch1;
}

U8 *MGetS(U64 flags=0)
//[C:/SparrowOS/OSMain/Adam1a.HPP.Z,1462] Flags
{
//Returns a [C:/SparrowOS/OSMain/Memory.CPP.Z,952] MAlloc()ed prompted string.
  return (*fp_mgets)(flags);
}

U64 GetS(U8 *buf,U64 size,BoolI8 allow_ext=TRUE)
{
  U8 *st;
  U64 ch,i=0;
  if (!size || !buf) return 0;
  if (allow_ext) {
    st=MGetS;
    if (StrLen(st)>size-1) {
      MemCpy(buf,st,size-1);
      buf[size-1]=0;
    } else
      StrCpy(buf,st);
    i=StrLen(buf);
    Free(st);
  } else {
    while (TRUE) {
      ch=GetChar(NULL,FALSE,IsInDbg);
      if (ch==CH_NEW_LINE) {
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
