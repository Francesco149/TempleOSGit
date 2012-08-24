#define EF_DONT_RECOMPILE	0
#define EF_RECOMPILE		1
#define EF_RENUM_ASM		2
#define EF_SLIDER_CTRL		3
#define EF_CH_SC		4

I64 PopUpEdFmt()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$FG,PURPLE$Reformat Function$FG$\n"
	"If you enter $FG,GREEN$DONT RECOMPILE$FG$ the stored\n"
	"source line indentation information will\n"
	"be used.  $BK,1$Be sure it is up-to-date!$BK,0$\n"
	"For $FG,GREEN$#exe$FG$ config'ed projects, enter $FG,GREEN$DONT RECOMPILE$FG$.\n"
	"$MU,\"Recompile       and autoindent CPP Fun\",EF_RECOMPILE$\n"
	"$MU,\"Don't Recompile but autoindent CPP Fun\",EF_DONT_RECOMPILE$\n\n"
	"$MU,\"Renum Asm Local @@ Labels for Fun\",EF_RENUM_ASM$\n\n"
	"$FG,PURPLE$Insert Managed Code$FG$\n"
	"$MU,\"Slider Ctrl\",EF_SLIDER_CTRL$\n\n"
	"$FG,PURPLE$Misc$FG$\n"
	"$MU,\"Insert ASCII/Scan Code Hex Codes for key pressed\",EF_CH_SC$\n\n"
	"$MU,\"CANCEL\",LTFM_CANCEL$\n"
	"$FG,GREEN$<ALT-BACKSPACE>$FG$ to undo if not happy\n"
	"with the results.\n");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}


#define C_INDENT_SPACES		2
#define ASM_RENUM_SPACING	5

U0 EdRemFunLeadingSpace(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  U8 ch,*ptr;
  I64 levels=1;
  LtfEntry *ll,*ll2;
  BoolI8 start_of_line=TRUE;

  EdGoToFun(l,FALSE,FALSE);
  ll=l->cur_entry->next;
  do {
    ll2=ll->next;
    if (ll!=l && ll!=l->cur_entry && !(ll->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK)))
      switch (ll->type_u8) {
	case LTFT_TEXT:
	  ptr=ll->tag;
	  if (start_of_line) {
	    while (*ptr==CH_SPACE)
	      ptr++;
	    if (*ptr)
	      start_of_line=FALSE;
	    ptr=StrNew(ptr,l->mem_task);
	    Free(ll->tag);
	    ll->tag=ptr;
	  }
	  if (!*ptr)
	    LtfEntryDel(l,ll);
	  else {
	    while (ch=*ptr++)
	      if (ch=='{')
		levels++;
	      else if (ch=='}') {
		if (!--levels)
		  break;
	      }
	    if (!levels) goto ls_done;
	  }
	  break;
	case LTFT_TAB:
	  if (start_of_line)
	    LtfEntryDel(l,ll);
	  break;
	case LTFT_NEW_LINE:
	  start_of_line=TRUE;
	  break;
	default:
	  start_of_line=FALSE;
      }
    ll=ll2;
  } while (ll!=l->cur_entry);
ls_done:
  LtfRecalc(l);
  LtfCenter(l);
  if (unlock)
    LtfUnlock(l);
}


class RenumStruct
{
  RenumStruct *next,*last;
  U8 label[sizeof(EdFindTextStruct.find_text)];
};

I64 EdRAGetU8(Ltf *l)
{
  I64 result=-1;
  while (l->cur_entry!=l && l->cur_entry->type&LTFLT_SELECTED && result<0) {
    result=EdCurU8(l);
    EdCursorRight(l);
  }
  return result;
}

U0 EdRACollect(Ltf *l,RenumStruct *root)
{
  I64 ch,i;
  RenumStruct *tempr;
  U8 buf[sizeof(EdFindTextStruct.find_text)];
  ch=EdRAGetU8(l);
  while (ch>=0) {
    if (ch!='@')
      ch=EdRAGetU8(l);
    else {
      ch=EdRAGetU8(l);
      if (ch=='@') {
	ch=EdRAGetU8(l);
	StrCpy(buf,"@@");
	i=2;
	while (ch>=0 && i<sizeof(EdFindTextStruct.find_text)) {
	  if (Bt(alpha_numeric_bitmap,ch))
	    buf[i++]=ch;
	  else
	    break;
	  ch=EdRAGetU8(l);
	}
	if (i<sizeof(EdFindTextStruct.find_text)) {
	  buf[i++]=0;
	  while (ch>=0 && Bt(white_space_bitmap,ch))
	    ch=EdRAGetU8(l);
	  if (ch==':') {
	    ch=EdRAGetU8(l);
	    tempr=MAlloc(sizeof(RenumStruct));
	    StrCpy(tempr->label,buf);
	    QueIns(tempr,root->last);
	  }
	}
      }
    }
  }
  //This is needed because we moved the
  //cursor and it didn't recalc.
  LtfRecalc(l);
}

U0 EdRenumAsm(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  I64 num=0;
  RenumStruct root,*tempr,*tempr1;
  U8 buf[sizeof(EdFindTextStruct.find_text)],
  buf2[sizeof(EdFindTextStruct.find_text)];

  QueInit(&root);
  EdSelectFun(l,TRUE);
  EdRACollect(l,&root);

  tempr=root.next;
  while (tempr!=&root) {
    tempr1=tempr->next;
    num+=ASM_RENUM_SPACING;
    SPrintF(buf,"@#%02d",num);
    EdReplace(l,tempr->label,buf,TRUE,TRUE,TRUE);
    Free(tempr);
    tempr=tempr1;
  }

  while (num) {
    SPrintF(buf, "@#%02d",num);
    SPrintF(buf2,"@@%02d",num);
    EdReplace(l,buf,buf2,TRUE,TRUE,TRUE);
    num-=ASM_RENUM_SPACING;
  }
  EdSelectAll(l,FALSE);
  LtfRecalc(l);
  LtfCenter(l);
  if (unlock)
    LtfUnlock(l);
}

#define AC_ABORT	-1
#define AC_REINDENT	0
#define AC_RENUM_ASM	1

U0 EdRefmtFun(Ltf *l,I64 fmt_type)
{
  BoolI8 unlock=LtfLock(l),start_of_line=TRUE;
  LtfEntry *ll,*nl;
  I64 i,x,result,action=AC_ABORT,old_line_num;
  U8 *b,*st,*st2,*prj_file;
  SrcSymHashEntry *temph;
  IndentInfo *indent;
  TaskStruct *task=NULL;
  TaskCmdStruct *tempc;

  LtfRecalc(l);
  old_line_num=l->cur_entry->y+1;

  LtfCaptureUndo(l,TRUE);
  switch (fmt_type) {
    case EF_RECOMPILE:
      if (EdGoToFun(l,FALSE,FALSE)) {
	if (l->flags&LTFF_PLAIN_TEXT)
	  LtfFlagsToggle(l,LTFF_PLAIN_TEXT);
	LtfWrite(l);
	task=Spawn(&ServantUserCmdLine,NULL,"Servant",Fs);
	st2=CurDir;
	st=MSPrintF("Cd(\"%s\");",st2);
	tempc=TaskRqstQue(task,Fs,st,1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	Free(st2);
	Free(st);

	task->win_top=Fs->win_top;
	task->win_bottom=Fs->win_bottom;
	task->win_left=Fs->win_left;
	task->win_right=Fs->win_right;
	TaskDerivedValsUpdate(task);

	if (GetRqstResult(tempc,&result)) {
	  st=DirFile(l->filename.name,,"SPZ"),
	  prj_file=ZorNotZ(st,'P');
	  Free(st);
	  if (FileFind(prj_file)) {
	    st2=DirFile(prj_file),
	    st=MSPrintF("Cd(\"%s\");",st2);
	    Free(st2);
	    tempc=TaskRqstQue(task,Fs,st,1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	    Free(st);
	    st=MSPrintF("\"$WW,1$\";Cmp(\"%s\",\"SysTemp\",\"SysTemp\");",prj_file);
	    tempc=TaskRqstQue(task,Fs,st,1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	    Free(st);
	    if (GetRqstResult(tempc,&result))
	      if (!result) {
		tempc=TaskRqstQue(task,Fs,
		"Load(\"SysTemp\",LDF_JUST_LOAD);"
		    "MapFileLoad(\"SysTemp\");",1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
		if (GetRqstResult(tempc,&result))
		  action=AC_REINDENT;
	      }
	    tempc=TaskRqstQue(task,Fs,"Del(\"SysTemp.*\");",1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	    GetRqstResult(tempc,&result);
	  } else {
	    Free(prj_file);
	    st=DirFile(l->filename.name,"Load","CPZ");
	    prj_file=ZorNotZ(st,'P');
	    Free(st);
	    if (FileFind(prj_file))
	      st=MSPrintF("\"$WW,1$\";ExeFile(\"%s\",TRUE);",prj_file);
	    else
	      st=MSPrintF("\"$WW,1$\";ExeFile(\"%s\",TRUE);",l->filename.name);
	    tempc=TaskRqstQue(task,Fs,st,1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	    Free(st);
	    if (GetRqstResult(tempc,&result))
	      if (result)
		action=AC_REINDENT;
	  }
	  Free(prj_file);
	}
	if (action==AC_REINDENT)	{
	  LtfUnlock(l);
	  st=MSPrintF("DbgFunFind(0x%X,0);",l);
	  tempc=TaskRqstQue(task,Fs,st,1<<TASKCf_WAKE_MASTER|1<<TASKCf_FOCUS_MASTER);
	  Free(st);
	  if (GetRqstResult(tempc,&result))
	    temph=result;
	  else
	    action=AC_ABORT;
	  LtfLock(l);
	} else
	  PopUpOk("Has Errors");
      }
      break;
    case EF_DONT_RECOMPILE:
      if (EdGoToFun(l,FALSE,FALSE)) {
	if (temph=DbgFunFind(l,0))
	  action=AC_REINDENT;
      }
      break;
    case EF_RENUM_ASM:
      if (EdGoToFun(l,FALSE,TRUE)) {
	if (EdCurU8(l)=='{') {
	  EdCursorRight(l);
	  LtfRecalc(l);
	} else if (EdCurU8(l)==':') {
	  EdCursorRight(l);
	  if (EdCurU8(l)==':')
	    EdCursorRight(l);
	  LtfRecalc(l);
	}
	action=AC_RENUM_ASM;
      }
      break;
  }

  switch (action) {
    case AC_RENUM_ASM:
      LtfUnlock(l);
      Snd(2000); Sleep(150); Snd(0);
      Sleep(100);
      Snd(2000); Sleep(150); Snd(0);
      LtfLock(l);
      EdRenumAsm(l);
      break;
    case AC_REINDENT:
      if (temph->type&(HTT_FUN|HTT_EXPORT_SYS_SYM))
	indent=temph->indent;
      else
	indent=NULL;
      if (indent && indent->min_line<=indent->max_line) {
	LtfUnlock(l);
	Snd(2000); Sleep(150); Snd(0);
	Sleep(100);
	Snd(2000); Sleep(150); Snd(0);
	LtfLock(l);
	EdRemFunLeadingSpace(l);
	LtfLineNumGoTo(l,indent->min_line);
	ll=l->cur_entry;
	while (indent->min_line<=ll->y+1<=indent->max_line) {
	  if (ll!=l && ll!=l->cur_entry && !(ll->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK))) {
	    if (ll->type_u8==LTFT_NEW_LINE||ll->type_u8==LTFT_SOFT_NEW_LINE)
	      start_of_line=TRUE;
	    else {
	      if (start_of_line) {
		i=indent->body[ll->y+1-indent->min_line]*C_INDENT_SPACES;
		x=ll->x+1;
		while (i>8) {
		  nl=CAlloc(sizeof(LtfEntryBase),l->mem_task);
		  nl->type=LTFT_TAB | l->settings_root.dft_text_attr << 8;
		  nl->x=x;
		  nl->y=ll->y;
		  nl->page_line_num=ll->page_line_num;
		  QueIns(nl,ll->last);
		  i-=8;
		  x+=8;
		}
		if (i>0) {
		  b=MAlloc(i+1,l->mem_task);
		  MemSet(b,CH_SPACE,i);
		  b[i]=0;
		  nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
		  nl->type=LTFT_TEXT | l->settings_root.dft_text_attr << 8;
		  nl->tag=b;
		  nl->max_col=1;
		  nl->x=x;
		  nl->y=ll->y;
		  nl->page_line_num=ll->page_line_num;
		  QueIns(nl,ll->last);
		}
	      }
	      start_of_line=FALSE;
	    }
	  }
	  ll=ll->next;
	}
	break;
      }
  }
  LtfRecalc(l);
  LtfLineNumGoTo(l,old_line_num);

  LtfUnlock(l);
  if (!unlock)
    LtfLock(l);
  if (task)
    Kill(task,FALSE);
}

U0 EdPopUpChSC(U64 *_ch,U64 *_sc)
{
  U64 sc;
  "Press A Key\n";
  LtfCur->flags|=LTFF_MIN_SIZE;
  do GetMsg(_ch,&sc,1<<MSG_KEY_DOWN);
  while (sc.u8[0]==SC_SHIFT || sc.u8[0]==SC_CTRL || sc.u8[0]==SC_ALT);
  *_sc=sc;
}

U0 EdChSC(Ltf *l)
{
  U64 ch,sc;
  U8 buf[136];
  SPrintF(buf,"EdPopUpChSC(%d,%d);",&ch,&sc);
  PopUp(buf,Fs);
  LtfPrintF(l,"0x%X,0x%X",ch,sc);
}

U0 EdCodeTools(Ltf *l)
{
  I64 fmt_type=PopUpEdFmt;
  switch (fmt_type) {
    case EF_DONT_RECOMPILE:
    case EF_RECOMPILE:
    case EF_RENUM_ASM:
      EdRefmtFun(l,fmt_type);
      break;
    case EF_SLIDER_CTRL:
      MgdSliderCtrl(l);
      break;
    case EF_CH_SC:
      EdChSC(l);
      break;
  }
}
