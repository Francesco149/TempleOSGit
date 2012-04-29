#help_index "Ltf/Cmd Line (Typically);Cmd Line (Typically)"

Ltf *EdJoin(U8 *filename,BoolI8 bail)
{ //bail is for [C:/LT/Adam/LinkChk.APZ,71] LinkChk()
  U8 *buf=StrNew(filename),
     *temp_filename=StrNew(filename),
     *tfn_ptr=temp_filename,
     *ptr;
  I64 i,num=1;
  SrcSymHashEntry *temph;
  BoolI8 cont;
  Ltf *l=NULL;

  i=LK_FILE;
  if (StrLen(filename)>3 && filename[2]==':') {
    buf[2]=0;
    i=DefineMatch(buf,"ST_LINK_TYPES",LEF_IGNORE_CASE);
    tfn_ptr=temp_filename+3;
    switch (i) {
      case LK_MAN_PAGE:
	if (temph=HashFind(tfn_ptr,Fs->hash_table,HTT_SRC_SYM)) {
	  if (temph->type&HTT_DEFINE_STR && temph(DefineStrHashEntry *)->caller) {
	    Free(buf);
	    if (buf=SrcEdLink(temph(DefineStrHashEntry *)->caller,DFT_ADDR_LINK_BIN_SIZE))
	      l=EdJoin(buf,bail);
	  } else
	    l=EdJoin(temph->src_link,bail);
	}
	goto et_done;
      case LK_ADDR:
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  i=A2I(ptr+1);
	} else
	  i=DFT_ADDR_LINK_BIN_SIZE;
	Free(buf);
	if (buf=SrcEdLink(ExePrintF("%s;",tfn_ptr),i))
	  l=EdJoin(buf,bail);
	goto et_done;
      case LK_DEF:
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  i=A2I(ptr+1);
	} else
	  i=-1;
	l=LtfNew;
	StrCpy(l->desc,"DictDef");
	WsdDisplayDefs(l,tfn_ptr,i);
	goto ej_ltf;
      case LK_HELP_INDEX:
	l=LtfNew;
	StrCpy(l->desc,"HelpIdx");
	LtfHelpIndex(l,tfn_ptr);
ej_ltf:
	Free(temp_filename);
	Free(buf);
	if (bail)
	  return l;
	else {
	  LtfDo(l);
	  LtfDel(l);
	  return NULL;
	}
      case LK_BIBLE_FIND:
	*buf=0;
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  StrCpy(buf,ptr+1);
	}
	num=DefineMatch(tfn_ptr,"ST_BIBLE_BOOKS",LEF_IGNORE_CASE);
	if (num<0) num=0;
	num=A2I(DefineSub(num,"ST_BIBLE_BOOK_LINES"));
	tfn_ptr=Define("ST_BIBLE_FILE");
	break;
      case LK_FILE_LINE:
      case LK_PLAIN_LINE:
	*buf=0;
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  num=A2I(ptr+1);
	}
	break;
      case LK_FILE_ANCHOR:
	*buf=0;
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  StrCpy(buf,ptr+1);
	}
	break;
      case LK_FILE_FIND:
      case LK_PLAIN_FIND:
	*buf=0;
	if (ptr=StrLastOcc(tfn_ptr,",")) {
	  *ptr=0;
	  StrCpy(buf,ptr+1);
	  if (ptr=StrLastOcc(buf,":")) {
	    *ptr=0;
	    num=A2I(ptr+1);
	  }
	}
	break;
    }
  }
  tfn_ptr=StrNew(tfn_ptr);

  cont=TRUE;
  if (!IsTextFile(tfn_ptr) &&
      !PopUpCancelOk(ST_WARN_ST "Not Text File\n\n"))
      cont=FALSE;
  if (cont)
    l=LtfFileEd(i,tfn_ptr,buf,num,bail);
  else
    l=NULL;
  WinTextClear(Fs);
  Free(tfn_ptr);
et_done:
  Free(temp_filename);
  Free(buf);
  return l;
}

public BoolI64 Ed(U8 *filename)
{
  if (IsRaw)
    return EdLite(filename);
  else
    return ToBool(EdJoin(filename,FALSE));
}

Ltf *LtfLinkChk(U8 *filename)
{
  return EdJoin(filename,TRUE);
}

U8 *EdLinkFile(U8 *filename,I64 *_line_num=NULL)
{
  U8 *result=NULL;
  Ltf *l=EdJoin(filename,TRUE);
  I64 line_num=0;
  if (l) {
    result=StrNew(l->filename.name);
    line_num=l->cur_entry->y+1;
    LtfDel(l);
  }
  if (_line_num) *_line_num=line_num;
  return result;
}

public U0 Plain(U8 *filename)
//Open a document in plain text mode,
//so that dollar signs are not special.
{
  U8 *buf=MSPrintF("PI:%s",filename);
  Ed(buf);
  Free(buf);
}

#help_index "Ltf/Task"
public Ltf *SysAuxLtfInit(BoolI8 has_menu)
{
  LtfEntry *ll;
  Ltf *aux,*l=LtfCur;

  aux=LtfNew;
  aux->flags|=LTFF_AUX_LTF;

  if (l) {
    LtfPutS(aux,"$CM+H+BY+RX+NC,-7,1$");
    ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
    ll->user_data=l;
    ll->tag_cb=&EdFilterCB;
    ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
    ll->user_data=l;
    ll->tag_cb=&EdOverStrikeCB;
    ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
    ll->user_data=l;
    ll->tag_cb=&EdDollarCB;
  }

  LtfPutS(aux,"$CM+H+BY+RX+NC,-4,1$");
  ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
  ll->tag_cb=&EdQuotesCB;
  ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
  ll->tag_cb=&EdBracesCB;
  ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
  ll->tag_cb=&EdSemicolonCB;

  if (l) {
    LtfPutS(aux,"$CM+H+BY+RX+NC,-18,1$");
    ll=LtfPutS(aux,"$TX+BD+TC,\"     \"$");
    ll->user_data=l;
    ll->tag_cb=&EdMoreCB;
    ll=LtfPutS(aux,"$TX+H+BD+TC,\" \"$");
    ll->user_data=l;
    ll->tag_cb=&EdDollarTypeCB;
  }

  if (Fs->user_num)
    LtfPrintF(aux,"$CM+H+TY+LX+NC,5,-1$$TX+H+BD,\"#%d\"$",Fs->user_num);
  LtfPutS(aux,"$CM+H+TY+NC,0,-1$");
  ll=LtfPutS(aux,"$DA+H-T-P+BD+RD+CX+SCX+IV,140,\"%s...\",16$");
  ll->data=&Fs->task_title;
  LtfDataFmt(ll);
  LtfPrintF(aux,"$CM+H+NC,1,0$$TX+H+BD+IV,\"%X\"$",Fs);
  aux->settings_root.dft_text_attr=Fs->border_attr;
  if (Bt(&Fs->display_flags,DISPLAYf_HAS_CLOSE_WIN))
    LtfPutS(aux,"$TX+H+RX+BD,\"[X]\"$");
  if (has_menu) {
    LtfPutS(aux,"$BK,1$$TX+H+LX+BD,\"MENU\"$$BK,0$");
    LBts(&Fs->display_flags,DISPLAYf_HAS_MENU);
  }
  return aux;
}

public U0 UseTermLtf(U8 *ltf_menu_file=NULL,U8 *pulldown_menu_file=NULL)
{
  BoolI8 old_preempt=Preempt(OFF);
  Ltf *l=LtfNew,*m;
  l->start_text_attr=WHITE<<4+BLUE;
  LtfInsQue(l,Fs->last_ltf);
  l->right_click_link=&TermRightClickLink;
  l->max_entries=4096;
  l->flags|=LTFF_ATTR_BY_PRT;

  StrCpy(l->desc,"Term");
  if (ltf_menu_file) {
    m=LtfRead(ltf_menu_file);
    m->start_text_attr=WHITE<<4+BLUE;
    m->flags|=LTFF_ATTR_BY_PRT|LTFF_ALLOW_UNDO;
    l->menu_ltf=m;
    LtfInsQueRev(SysAuxLtfInit(TRUE),Fs->next_ltf);
  } else
    LtfInsQueRev(SysAuxLtfInit(FALSE),Fs->next_ltf);

  if (pulldown_menu_file)
    Fs->cur_menu=MenuFile(pulldown_menu_file);
  else
    Fs->cur_menu=MenuFile("::/LT/Doc/EdPullDown.TXZ");

  Fs->update_win=&LtfUpdateTaskLtfs;
  WinScrollsInit(Fs);
  Preempt(old_preempt);
  Raw(OFF);
}

#help_index "Ltf/Input"
public U8 *LtfGetLine(Ltf *l,LtfEntry *cl,I64 *cur_col=NULL,LtfEntry **_cl_end=NULL)
{
  LtfEntry *cl2=cl;
  BoolI8 unlock=LtfLock(l);
  U8 *result,*dst,*src,*start;
  U64 i=0;
  if (cur_col)
    *cur_col=-1;
  while (cl2!=l && cl2->type_u8!=LTFT_NEW_LINE) {
    if (Bt(ltf_tag_types,cl2->type_u8)) {
      src=cl2->tag;
      i+=StrLen(src);
    } else if (cl2->type_u8==LTFT_TAB)
      i++;
    cl2=cl2->next;
  }
  result=MAlloc(i+1);
  dst=result;
  while (cl!=l && cl->type_u8!=LTFT_NEW_LINE) {
    start=dst;
    if (Bt(ltf_tag_types,cl->type_u8)) {
      src=cl->tag;
      while (*src)
	*dst++=*src++;
    } else if (cl->type_u8==LTFT_TAB)
      *dst++=CH_TAB;
    if (cl==l->cur_entry && cur_col)
      *cur_col=start-result+l->cur_data_col;
    cl=cl->next;
  }
  if (_cl_end) *_cl_end=cl;
  *dst=0;
  if (cl==l->cur_entry && cur_col && !l->cur_data_col)
    *cur_col=dst-result;
  if (unlock)
    LtfUnlock(l);
  return result;
}

#help_index "Ltf/Output"
public U0 LtfDump(Ltf *l,U64 uS_delay=0)
{
  U8 *st;
  LtfEntry *cl,*cl2;
  BoolI8 unlock=LtfLock(l);
  cl=l->root.next;
  while (cl!=l) {
    st=LtfGetLine(l,cl,NULL,&cl2);
    "" st;
    Free(st);
    cl=cl2;
    if (cl->type_u8==LTFT_NEW_LINE) {
      NewLine;
      BusyWait(uS_delay);
      cl=cl->next;
    }
  }
  if (unlock)
    LtfUnlock(l);
}

public LtfEntry *LtfPutLine(LtfEntry *cl)
{
  U8 ch,*ptr,*ptr2;
  while (cl->type_u8!=LTFT_ERROR && cl->type_u8!=LTFT_NEW_LINE) {
    if (Bt(ltf_tag_types,cl->type_u8)) {
      ptr=cl->tag;
      do {
	ptr2=ptr;
	while (ch=*ptr)
	  if (ch=='$')
	    break;
	  else
	    ptr++;
	*ptr=0;
	"" ptr2;
	*ptr=ch;
	if (ch=='$') {
	  "$$";
	  ptr++;
	}
      } while (ch);
    } else if (cl->type_u8==LTFT_TAB)
      '' CH_TAB;
    cl=cl->next;
  }
  NewLine;
  return cl->type_u8==LTFT_ERROR ? cl:cl->next;
}

#help_index "Ltf/Input"
public U8 *LtfMGetS(U64 flags=0)
//[C:/LT/OSMain/Adam1a.HPZ,1417] Flags
{
  Ltf *l;
  LtfEntry *cl,*cl_end;
  BoolI8 old_preempt=IsPreempt;
  U8 *result,*st;
  U64 ch,sc;
  "$FU$";
  do {
    ch=LtfGetKey(&sc);
    if (ch==CH_ESC) {
      l=Fs->next_ltf;
      while (l!=(&Fs->next_ltf)(U8 *)-offset(Ltf.next_ltf) &&
	  l->ltf_in_queue_signature==LTF_IN_QUEUE_SIGNATURE_VAL) {
	LtfLock(l);
	if (!(l->flags&LTFF_DONT_SHOW) &&
	    (!l->best_d || !(l->flags&LTFF_AUX_LTF)) &&
	    l->ltf_in_queue_signature==LTF_IN_QUEUE_SIGNATURE_VAL && l->cur_entry!=l) {
	  cl=LtfEntryCopy(l,l->cur_entry);
	  if (l->flags&LTFF_AUX_LTF && LtfCur)
	    LtfBottom;
	  else
	    LtfBottom(l);
	  LtfEntryRun(l,cl,TRUE);
	  LtfEntryDel(l,cl);
	  LtfUnlock(l);
	  break;
	}
	LtfUnlock(l);
	l=l->next_ltf;
      }
      if (flags&GETSF_WITH_NEW_LINE)
	break;
    } if (ch==CH_SHIFT_ESC) {
      if (flags&GETSF_SHIFT_ESC_EXIT)
	Exit;
      else {
	result=StrNew("");
	goto mgets_done;
      }
    }
    //<CTRL-SHIFT-CR> is a blank line without entry
  } while (flags&GETSF_WITH_NEW_LINE || ch!=CH_NEW_LINE || sc & SCF_CTRL && sc & SCF_SHIFT);

  Preempt(OFF);
  l=LtfCur;
  LtfLock(l);
  if (flags&GETSF_WITH_NEW_LINE) {
    cl=l->cur_entry;
    do cl=cl->last;
    while (cl!=l && cl->type_u8!=LTFT_USER_FOREGROUND);
    cl=cl->next;
    if (result=LtfGetLine(l,cl,NULL,&cl_end)) {
      while (cl_end!=l && cl_end->type_u8==LTFT_NEW_LINE) {
	result=StrAdd(result,"\n");
	if (st=LtfGetLine(l,cl_end->next,NULL,&cl_end)) {
	  result=StrAdd(result,st);
	  Free(st);
	}
      }
    }
  } else {
    cl=l->cur_entry;
    do cl=cl->last;
    while (cl!=l && cl->type_u8!=LTFT_NEW_LINE &&
	cl->type_u8!=LTFT_USER_FOREGROUND);
    if (cl!=l && cl->type_u8!=LTFT_USER_FOREGROUND) {
      do cl=cl->last;
      while (cl!=l && cl->type_u8!=LTFT_NEW_LINE &&
	  cl->type_u8!=LTFT_USER_FOREGROUND);
    }
    cl=cl->next;
    result=LtfGetLine(l,cl,NULL);
  }
  LtfUnlock(l);
mgets_done:
  "$FU$$FG$$BG$";
  Preempt(old_preempt);
  return result;
}

#help_index "Char/Input"

public I64 PmtI64(U8 *msg,I64 dft,I64 lo=MIN_I64,I64 hi=MAX_I64)
{
  I64 result;
  BoolI8 okay;
  U8 *st;
  while (TRUE) {
    "" msg,dft;
    st=MGetS;
    if (!*st) {
      Free(st);
      return dft;
    }
    try {
      okay=FALSE;
      result=ExePrintF3("ToI64(%s);",st);
      okay=TRUE;
    } catch
      Fs->catch_except=TRUE;
    Free(st);
    if (okay && lo<=result<=hi)
      return result;
  }
}

public F64 PmtF64(U8 *msg,F64 dft,F64 lo=MIN_F64,F64 hi=MAX_F64)
{
  F64 result;
  BoolI8 okay;
  U8 *st;
  while (TRUE) {
    "" msg,dft;
    st=MGetS;
    if (!*st) {
      Free(st);
      return dft;
    }
    try {
      okay=FALSE;
      result=ExePrintF3("ToF64(%s);",st)(F64);
      okay=TRUE;
    } catch
      Fs->catch_except=TRUE;
    Free(st);
    if (okay && lo<=result<=hi)
      return result;
  }
}

public U8 *PmtStr(U8 *msg=NULL,U8 *dft=NULL)
{
  U8 *st;
  if (msg) {
    if (dft)
      "" msg,dft;
    else
      "" msg;
  }
  st=MGetS;
  if (!*st) {
    Free(st);
    if (dft)
      return StrNew(dft);
    else
      return StrNew("");
  }
  return st;
}

public LTDate PmtLTDate(U8 *msg,LTDate dft,LTDate lo=MIN_I64,LTDate hi=MAX_I64)
{
  LTDate result;
  U8 *st;
  while (TRUE) {
    "" msg,dft,dft;
    st=MGetS;
    if (!*st) {
      Free(st);
      return dft;
    }
    result=A2LTDate(st);
    Free(st);
    if (result>=lo && result<=hi)
      return result;
  }
}

#help_index "Ltf"

#define RIGHT_INCLUDE	0
#define RIGHT_DEBUG	1
#define RIGHT_COMPILE	2
#define RIGHT_LOAD	3
#define RIGHT_AINCLUDE	4
#define RIGHT_AUTOFILE	5
#define RIGHT_COPY	6
#define RIGHT_MOVE	7
#define RIGHT_DELETE	8

I64 PopUpCancelIncludeDelAuto(U8 *header=NULL,U8 *footer=NULL)
{
  I64 i;
  Ltf *l=LtfNew;
  if (header) LtfPutS(l,header);
  LtfPutS(l,"$CM+LX,1,3 $$BT,\"INCLUDE       (.CPZ)\",RIGHT_INCLUDE$"
	"$CM+LX,25,0$$BT,\"DEBUG               \",RIGHT_DEBUG$"
	"$CM+LX,1,3 $$BT,\"STATIC COMPILE(.SPZ)\",RIGHT_COMPILE$"
	"$CM+LX,25,0$$BT,\"LOAD          (.BIZ)\",RIGHT_LOAD$"
	"$CM+LX,1,3 $$BT,\"ADAM_INCLUDE  (.APZ)\",RIGHT_AINCLUDE$"
	"$CM+LX,25,0$$BT,\"AUTOFILE      (.AUZ)\",RIGHT_AUTOFILE$"
	"$CM+LX,1,3 $$BT,\"COPY                \",RIGHT_COPY$"
	"$CM+LX,25,0$$BT,\"MOVE                \",RIGHT_MOVE$"
	"$CM+LX,1,3 $$BT,\"DELETE              \",RIGHT_DELETE$"
	"$CM+LX,25,0$$BT,\"CANCEL              \",LTFM_CANCEL$\n");
  if (footer) LtfPutS(l,footer);
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

I64 EdLeftClickLink(Ltf *l,LtfEntry *cl)
{ //called with l locked, exit unlocked
  BoolI8 result;
  U8 *st;
  if (cl->flags & LTFLF_AUX_STR)
    st=StrNew(cl->aux_str);
  else
    st=StrNew(cl->tag);
  LtfUnlock(l);
  result=Ed(st);
  Free(st);
  return result;
}

I64 TermRightClickLink(Ltf *l,LtfEntry *cl)
{ //called with l locked, exit unlocked
  BoolI8 send_new_line=FALSE,result=FALSE;
  U8 *st;
  I64 i;
  EdFileNameStruct fn;
  if (cl->flags & LTFLF_AUX_STR)
    st=cl->aux_str;
  else
    st=cl->tag;
  st= st[2]==':' ? st+3:st;
  LtfUnlock(l);
  if ((i=PopUpCancelIncludeDelAuto(st))>=0) {
    LtfBottom(l);
    switch (i) {
      case RIGHT_INCLUDE:
	if (FileExtDot(st) && !WildCardMatch("*.CP?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .CP? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"#include \"%s\";\n$FU$$FG$$BG$",st;
	WinZBufUpdate;
	ExeFile(st);
	result=TRUE;
	break;
      case RIGHT_DELETE:
	Del(st);
	result=TRUE;
	break;
      case RIGHT_AUTOFILE:
	if (FileExtDot(st) && !WildCardMatch("*.AU?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .AU? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"AutoFile(\"%s\");\n$FU$$FG$$BG$",st;
	WinZBufUpdate;
	AutoStr("%F\n",st);
	result=TRUE;
	break;
      case RIGHT_AINCLUDE:
	if (FileExtDot(st) && !WildCardMatch("*.AP?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .AP? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"Adam(\"#include \\\"%s\\\"\" );\n$FU$$FG$$BG$",st;
	WinZBufUpdate;
	AdamFile(st,FALSE);
	result=TRUE;
	break;
      case RIGHT_COMPILE:
	if (FileExtDot(st) && !WildCardMatch("*.SP?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .SP? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"Cmp(\"%s\");\n$FU$$FG$$BG$",st;
	Cmp(st);
	result=TRUE;
	break;
      case RIGHT_LOAD:
	if (FileExtDot(st) && !WildCardMatch("*.BI?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .BI? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"Load(\"%s\");\n$FU$$FG$$BG$",st;
	WinZBufUpdate;
	Load(st);
	result=TRUE;
	break;
      case RIGHT_COPY:
	StrCpy(fn.name,st);
	if (LtfFormDo(fn.name,"EdFileNameStruct")) {
	  Copy(st,fn.name);
	  result=TRUE;
	}
	break;
      case RIGHT_MOVE:
	StrCpy(fn.name,st);
	if (LtfFormDo(fn.name,"EdFileNameStruct")) {
	  Move(st,fn.name);
	  result=TRUE;
	}
	break;
      case RIGHT_DEBUG:
	if (FileExtDot(st) && !WildCardMatch("*.CP?",st)) {
	  if (!PopUpCancelOk(ST_WARN_ST "Not .CP? File\n\n")) {
	    send_new_line=TRUE;
	    break;
	  }
	}
	"IDE(\"%s\");\n$FU$$FG$$BG$",st;
	WinZBufUpdate;
	IDE(st);
	result=TRUE;
	break;
    }
  } else
    send_new_line=TRUE;
  LtfBottom(l);
  "$FU$$FG$$BG$";
  if (send_new_line)
    '' CH_NEW_LINE;
  return result;
}

#help_index "Ltf/Cmd Line (Typically);Cmd Line (Typically)"

public U0 Man(U8 *st)
//Enter the name of a sym and this
//will jump to the src code.
{
  U8 *name=MAlloc(3+StrLen(st)+1);
  StrCpy(name,"MN:");
  StrCat(name,st);
  Ed(name);
  Free(name);
}

#help_index "Debugging;Ltf/Cmd Line (Typically);Ltf/Output;Cmd Line (Typically)"

public U0 LtfDm(U8 *buf,U64 cnt=0x80)
//Dumps a chunk of mem, showing
//addresses.  It is live and can be
//edited.
{
  LtfEntry *ll;
  Ltf *l=LtfCur;
  BoolI8 unlock=LtfLock(l);
  ll=LtfPrintF(l,"$HX-Z,%d,16$",cnt);
  ll->data=buf;
  l->cur_entry=ll->next;
  LtfRecalc(l);
  if (unlock)
    LtfUnlock(l);
}

public U0 LtfD(U8 *buf,U64 cnt=0x80)
//Dumps a chunk of mem, showing
//offsets.  It is live and can be
//edited.
{
  LtfEntry *ll;
  Ltf *l=LtfCur;
  BoolI8 unlock=LtfLock(l);
  ll=LtfPrintF(l,"$HX,%d,16$",cnt);
  ll->data=buf;
  l->cur_entry=ll->next;
  LtfRecalc(l);
  if (unlock)
    LtfUnlock(l);
}

#help_index "Ltf/Output"
BoolI64 AKDLtfPutS(U8 *st)
{
  Ltf *l;
  if (l=LtfCur)
    LtfPutSPartial(l,st);
  return FALSE;
}

KeyDevAdd(&AKDLtfPutKey,&AKDLtfPutS,0x80000000,TRUE);
fp_mgets=&LtfMGetS;
fp_ltf_cur=&LtfCur;

#help_index "Ltf"
