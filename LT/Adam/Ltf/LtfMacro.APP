U8 captured_macro_name[136];
StrCpy(captured_macro_name,"Test");

I64 sys_macro_repeat_n=1;

U0 SysMacroDel(TaskCmdStruct *macro_root)
{
  TaskCmdStruct *tempc,*tempc1;
  tempc=macro_root->next;
  while (tempc!=macro_root) {
    tempc1=tempc->next;
    RemQue(tempc);
    Free(tempc);
    tempc=tempc1;
  }
}

U0 SysMacroStripKey(TaskCmdStruct *macro_root,I64 p1,I64 p2)
{
  TaskCmdStruct *tempc,*tempc1;
  tempc=macro_root->next;
  while (tempc!=macro_root) {
    tempc1=tempc->next;
    if (tempc->cmd_code==TASKCT_MSG &&
	(tempc->msg_code==MSG_KEY_DOWN ||
	 tempc->msg_code==MSG_KEY_UP ||
	 tempc->msg_code==MSG_KEY_DOWN_UP) &&
	 p1 && tempc->p1==p1 ||
	!p1 && tempc->p2==p2) {
      RemQue(tempc);
      Free(tempc);
    }
    tempc=tempc1;
  }
}

#define MT_NULL	0
#define MT_MSG	1
#define MT_CHAR	2

class MacroTemp
{
  MacroTemp *next,*last;
  I64 type;
  U8 buf[136];
};

MacroTemp *Cmd2MT(TaskCmdStruct *tempc)
{
  U8 buf[8];
  MacroTemp *tempmt=CAlloc(sizeof(MacroTemp));
  if (Bt(getkey_chars_bitmap,tempc->p1) && tempc->msg_code==MSG_KEY_DOWN) {
    tempmt->type=MT_CHAR;
    buf[0]=tempc->p1;
    buf[1]=0;
    SPrintF(tempmt->buf,"%Q",buf);
  } else {
    tempmt->type=MT_MSG;
    SPrintF(tempmt->buf,"Msg(0x%X,0x%X,0x%X);",
      tempc->msg_code,tempc->p1,tempc->p2);
  }
  return tempmt;
}


U8 *SysMacro2Str(TaskCmdStruct *macro_root)
{
  TaskCmdStruct *tempc;
  I64 cnt=1; //terminating zero
  U8 *ptr,*m;
  MacroTemp *tempmt,*tempmt1,root;
  BoolI8 old_preempt=Preempt(OFF);
  LBtr(&sys_semas[SYS_SEMA_RECORD_MACRO],0);

  root.next=root.last=&root;
  root.type=MT_NULL;
  tempc=macro_root->next;
  while (tempc!=macro_root) {
    tempmt=Cmd2MT(tempc);
    InsQue(tempmt,root.last);
    cnt+=StrLen(tempmt->buf);
    if (tempmt->type==MT_CHAR) {
      if (tempmt->last->type!=MT_CHAR)
	cnt+=StrLen("\"");
      if (tempmt->next->type!=MT_CHAR)
	cnt+=StrLen("\";");
    }
    tempc=tempc->next;
  }

  m=MAlloc(cnt);
  ptr=m;

  tempmt=root.next;
  while (tempmt!=&root) {
    tempmt1=tempmt->next;
    if (tempmt->type==MT_MSG) {
      StrCpy(ptr, tempmt->buf);
      ptr+=StrLen(tempmt->buf);
    } else {
      if (tempmt->last->type!=MT_CHAR) {
	StrCpy(ptr, "\"");
	ptr+=StrLen("\"");
      }
      StrCpy(ptr,tempmt->buf);
      ptr+=StrLen(tempmt->buf);
      if (tempmt->next->type!=MT_CHAR) {
	StrCpy(ptr, "\";");
	ptr+=StrLen("\";");
      }
    }
    Free(tempmt);
    tempmt=tempmt1;
  }
  *ptr=0;
  Preempt(old_preempt);
  return m;
}

U0 PlaySysMacro(I64 n=1)
{
  TaskStruct *task=sys_focus_task;
  BoolI8 old_preempt;
  U8 *m;
  if (TaskValidate(task)) {
    LBtr(&sys_semas[SYS_SEMA_RECORD_MACRO],0);
    old_preempt=Preempt(OFF);
    m=SysMacro2Str(&sys_macro_root);
    while (n-- && TaskValidate(task)) {
      if (task==Fs)
	AutoStr("%s",m);
      else
	XTalkStrWithWait(task,"%s",m);
    }
    Free(m);
    Preempt(old_preempt);
  }
}

U0 EdInsertCapturedMacro()
{
  BoolI8 old_preempt=Preempt(OFF);
  U8 *st=SysMacro2Str(&sys_macro_root);
  if (sys_focus_task) {
    XTalk(sys_focus_task,"$MA+A+LM+LA,\"%s\",\"%Q\"$",
	captured_macro_name,st);
    Free(st);
  }
  Preempt(old_preempt);
}

#define SM_CANCEL	-1
#define SM_RECORD	0
#define SM_INSERT	1
#define SM_PLAY		2
#define SM_REPEAT_N	3
#define SM_STOP		4

I64 PopUpExitRecordPlayStopInsert()
{
  I64 result=0;
  U8 buf[136];
  TaskCmdStruct *tempc;
  Ltf *l=LtfNew;
  LtfEntry *ll;
  ll=LtfPutS(l,"$DA-P,135,\"Name:%s\"$");
  ll->data=captured_macro_name;
  LtfDataFmt(ll);
  ll=LtfPutS(l,"\n$DA,32,\"Repeat N:%d\"$");
  ll->data=&sys_macro_repeat_n;
  LtfDataFmt(ll);
  LtfPutS(l,"\n"
	"$CM+LX,1,3$$BT,\"RECORD\",SM_RECORD$"
	"$CM+LX,17,0$$BT,\"INSERT\",SM_INSERT$"
	"$CM+LX,1,3$$BT,\"PLAY\",SM_PLAY$"
	"$CM+LX,17,0$$BT,\"REPEAT N\",SM_REPEAT_N$"
	"$CM+LX,1,3$$BT,\"STOP\",SM_STOP$"
	"$CM+LX,17,0$$BT,\"CANCEL\",SM_CANCEL$"
	"\n\n\n$FG,GREEN$SHIFT-F2$FG$ will play macro.\n");
  l->flags|=LTFF_MIN_SIZE | LTFF_FORM;
  SPrintF(buf,"LtfMenuDo(0x%X);",l);
  sys_macro_util_task=Spawn(&ServantUserCmdLine,NULL,"Macro Popup",Fs);
  Fs->popup_task=sys_macro_util_task;
  LBts(&sys_macro_util_task->display_flags,DISPLAYf_WIN_ON_TOP);
  tempc=TaskRequestQueue(sys_macro_util_task,Fs,buf,
      1<<TASKCf_WAKE_MASTER | 1<<TASKCf_FOCUS_MASTER);
  GetRequestResult(tempc,&result);
  Fs->popup_task=NULL;
  Kill(sys_macro_util_task);
  sys_macro_util_task=NULL;
  LtfDataGet(ll);
  LtfDel(l);
  return result;
}

U0 MacroTask(U64 dummy=0)
{
  nounusedwarn dummy;
  I64 i;
  StrCpy(captured_macro_name,"Click Here");
  sys_macro_repeat_n=1;
  do {
    i=PopUpExitRecordPlayStopInsert;
    WinRefocus;
    switch (i) {
      case SM_RECORD:
	LBtr(&sys_semas[SYS_SEMA_RECORD_MACRO],0);
	SysMacroDel(&sys_macro_root);
	LBts(&sys_semas[SYS_SEMA_RECORD_MACRO],0);
	break;
      case SM_PLAY:
	PlaySysMacro;
	break;
      case SM_REPEAT_N:
	PlaySysMacro(sys_macro_repeat_n);
	break;
      case SM_STOP:
	LBtr(&sys_semas[SYS_SEMA_RECORD_MACRO],0);
	break;
      case SM_INSERT:
	LBtr(&sys_semas[SYS_SEMA_RECORD_MACRO],0);
	EdInsertCapturedMacro;
	break;
    }
  } while (i>=0);
}

U0 EdMacroUtil()
{
  if (!sys_macro_util_task)
    Spawn(&MacroTask,NULL,"Macro");
}

