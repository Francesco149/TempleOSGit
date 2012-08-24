TaskStruct *SingleSong(U8 *msg,U8 *name)
{
  BoolI8 old_preempt=Preempt(OFF);
  TaskStruct *task=Spawn(&ServantUserCmdLine,NULL,name,Fs);
  LBts(&task->task_flags,TASKf_LOCK_TITLE);
  StrCpy(task->task_title,name);
  TaskRqstQue(task,Fs,";",1<<TASKCf_WAKE_MASTER);
  task->win_top=2;
task->win_bottom=task->win_top+8;
  task->win_right=task->win_left+50;
  TaskDerivedValsUpdate(task);
  Preempt(old_preempt);
  TaskRqstQue(task,NULL,msg,1<<TASKCf_EXIT_ON_COMPLETE);
  LtfCur(task)->max_entries=100;
  return task;
}

#define JB_RUN_LEVEL_NULL	0
#define JB_RUN_LEVEL_ONE	1
#define JB_RUN_LEVEL_TWO	2

BoolI64 JBPutKey(Ltf *l,U0 *,U64 ch,U64 sc)
{  //ch=ASCII; sc=scancode
  LtfEntry *cl=l->cur_entry,*ll;
  LTDirEntry *tempm;
  Ltf *s;
  if (!(sc&(SCF_ALT|SCF_CTRL|SCF_SHIFT))
    && cl->type_u8==LTFT_MENU_VAL && cl->left_exp>=0) {
    tempm=cl->left_exp;
    if (sc.u8[0]==SC_DELETE ) {
      Beep;
      Silent(ON);
      Del(tempm->full_name);
      Silent(OFF);
    } else if ('0'<=ch<='9') {
      if (StrLen(cl->tag)>1)
	cl->tag[0]=ch;
      s=LtfRead(tempm->full_name);
      ll=s->root.next;
      while (ll!=s && ll->type_u8!=LTFT_TEXT)
	ll=ll->next;
      if (ll!=s && ll->type_u8==LTFT_TEXT && StrLen(ll->tag)>=3) {
	ll->tag[2]=ch;
	LtfWrite(s);
      }
      LtfDel(s);
      return TRUE;
    } else if (ch==CH_SPACE||ch==CH_ESC)
      tempm->user_data++; //JB_RUN_LEVEL++
    //<SPACE BAR> is followed by <ESC> --> JB_RUN_LEVEL_TWO
    //Actual <ESC> just exits          --> JB_RUN_LEVEL_ONE
  }
  return FALSE;
}

public U0 JukeBox(U8 *dirname=NULL,U8 **_filename=NULL)
{
//**_filename is for using this as a song-chooser program.
  I64 i=0,rating;
  U8 *st,*st2;
  LTDirEntry *tempm,*tempm1;
  Ltf *l=LtfNew,*s;
  LtfEntry *ll;
  StrCpy(l->desc,"JukeBox");
  TaskStruct *task=NULL;
  if (_filename)
    *_filename=NULL;
  if (!dirname)
    dirname=StrNew("HOME/MusicOrgan");
  else
    dirname=StrNew(dirname);
  st=MSPrintF("%s/*.CPZ",dirname);
  tempm=FilesFind(st);
  Free(st);
  Free(dirname);
  l->user_put_key=&JBPutKey;
  tempm1=tempm;
  LtfPutS(l,
  "Key: $FG,GREEN$Graphics $FG,BLUE$Words $FG,RED$No Nothing "
  "$FG,BLACK$Incomplete $FG,CYAN$Special$FG$\n\n"
  "$FG,GREEN$<DELETE>$FG$ to delete a song.\n"
      "Press a number to rate a song.\n");
  while (tempm1) {
    if (!(i++%5))
      LtfPutS(l,"\n");
    if (FileOcc("Play(",tempm1->full_name,"")) {
      st=StrNew(tempm1->name);
      FileExtRem(st);
      s=LtfRead(tempm1->full_name);
      ll=s->root.next;
      while (ll!=s && ll->type_u8!=LTFT_TEXT)
	ll=ll->next;
      rating='0';
      if (ll!=s && ll->type_u8==LTFT_TEXT) {
	if ('0'<=ll->tag[2]<='9')
	  rating=ll->tag[2];
	if (StrStr(ll->tag,"incomplete"))
	  LtfPutS(l,"$FM,BLACK$");
	else if (StrStr(ll->tag,"has graphics"))
	  LtfPutS(l,"$FM,GREEN$");
	else if (StrStr(ll->tag,"has words"))
	  LtfPutS(l,"$FM,BLUE$");
	else if (StrStr(ll->tag,"special"))
	  LtfPutS(l,"$FM,CYAN$");
	else if (StrStr(ll->tag,"no nothing")) {
	  LtfPutS(l,"$FM,RED$");
	  if (FileOcc("\\0",tempm1->full_name,"")) {
	    s->cur_entry=ll->next;
	    s->cur_data_col=0;
	    LtfEntryDel(s,ll);
	    LtfPutS(s,"//0 has words\n");
	    LtfWrite(s);
	  }
	}
	LtfPrintF(l,"$MU-UL,\"%c%-8ts\",%d$ ",rating,st,tempm1);
	tempm1->user_data=JB_RUN_LEVEL_NULL;
      }
      LtfDel(s);
      Free(st);
    }
    tempm1=tempm1->next;
  }
  LtfPrintF(l,"\n$FM,CYAN$$MU-UL,\"DONE\",%d$\n",LTFM_CANCEL);
  while (TRUE) {
    if (_filename)
      tempm1=PopUpMenu(l,DOF_INTERCEPT_TASK_END);
    else
      tempm1=PopUpMenu(l);
    if (task)
      Kill(task);
    if (tempm1<0) break;
    st2=StrNew(tempm1->name);
    if (_filename) {
      Free(*_filename);
      *_filename=StrNew(tempm1->full_name);
    }
    if (tempm1->user_data==JB_RUN_LEVEL_ONE) break; //<ESC>
    tempm1->user_data=JB_RUN_LEVEL_NULL; //Rst from <SPACE BAR>
    FileExtRem(st2);
    st=MSPrintF("ExeFile(\"%s\");",tempm1->full_name);
    MusicSettingsRst;
    task=SingleSong(st,st2);
    Free(st2);
    Free(st);
  }
  LtfDel(l);
  LTDirListDel(tempm);
}
