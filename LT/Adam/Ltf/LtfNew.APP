U64 ltf_tag_types[(LTFT_NUM_TYPES+63)/64],
   ltf_nontag_invisible_types[(LTFT_NUM_TYPES+63)/64],
   ltf_form_types[(LTFT_NUM_TYPES+63)/64],
   ltf_data_types[(LTFT_NUM_TYPES+63)/64];
U64 ltf_clean_scancodes[4]={0,0,0,0};
//Don't forget [::/LT/Compiler/Lex.CPZ,ltf_stop_btypes] ltf_stop_btypes
U0 LtfTablesInit()
{
  ltf_tag_types[0]=
	1<<LTFT_TEXT|
	1<<LTFT_DATA|
	1<<LTFT_MENU_VAL|
	1<<LTFT_HIDE_START |
	1<<LTFT_TREE|
	1<<LTFT_HIDE_END|
	1<<LTFT_MACRO|
	1<<LTFT_BUTTON|
	1<<LTFT_CHECK_BOX|
	1<<LTFT_LINK|
	1<<LTFT_ANCHOR|
	1<<LTFT_PICTURE|
	1<<LTFT_INS_BINARY|
	1<<LTFT_INS_BINARY_TYPE|
	1<<LTFT_INS_BINARY_SIZE|
	1<<LTFT_SONG|
	1<<LTFT_HTML_IMG|
	1<<LTFT_HTML_VIDEO|
	1<<LTFT_HTML_CODE;

  //Don't forget [::/LT/Compiler/Lex.CPZ,ltf_stop_btypes] ltf_stop_btypes

  ltf_nontag_invisible_types[0]=
	1<<LTFT_PAGE_LEN|
	1<<LTFT_LEFT_MARGIN|
	1<<LTFT_RIGHT_MARGIN|
	1<<LTFT_HEADER|
	1<<LTFT_FOOTER|
	1<<LTFT_INDENT|
	1<<LTFT_FOREGROUND|
	1<<LTFT_BACKGROUND|
	1<<LTFT_DFT_FOREGROUND|
	1<<LTFT_DFT_BACKGROUND|
	1<<LTFT_ALT_FOREGROUND|
	1<<LTFT_LINK_FOREGROUND|
	1<<LTFT_MACRO_FOREGROUND|
	1<<LTFT_ANCHOR_FOREGROUND|
	1<<LTFT_HIDDEN_FOREGROUND|
	1<<LTFT_TREE_FOREGROUND|
	1<<LTFT_USER_FOREGROUND|
	1<<LTFT_BINARY_FOREGROUND|
	1<<LTFT_KEYWORD_FOREGROUND|
	1<<LTFT_KEYWORD2_FOREGROUND|
	1<<LTFT_COMMENT_FOREGROUND|
	1<<LTFT_QUOTE_FOREGROUND|
	1<<LTFT_WORD_WRAP|
	1<<LTFT_UNDERLINE|
	1<<LTFT_INVERT|
	1<<LTFT_BLINK|
	1<<LTFT_SHIFTED_X|
	1<<LTFT_SHIFTED_Y|
	1<<LTFT_PICTURE|
	1<<LTFT_BPT|
	1<<LTFT_HIGHLIGHT|
	1<<LTFT_LOCK_TO_PAGE|
	1<<LTFT_LOCK_TO_CENTER;

  ltf_form_types[0]=ltf_tag_types[0]&~(1<<LTFT_TEXT)|1<<LTFT_HEX_ED;

  ltf_data_types[0]=
	1<<LTFT_DATA|
	1<<LTFT_HEX_ED|
	1<<LTFT_CHECK_BOX;

  Bts(ltf_clean_scancodes,SC_CURSOR_RIGHT);
  Bts(ltf_clean_scancodes,SC_CURSOR_LEFT);
  Bts(ltf_clean_scancodes,SC_CURSOR_UP);
  Bts(ltf_clean_scancodes,SC_CURSOR_DOWN);
  Bts(ltf_clean_scancodes,SC_PAGE_UP);
  Bts(ltf_clean_scancodes,SC_PAGE_DOWN);
  Bts(ltf_clean_scancodes,SC_ALT);
  Bts(ltf_clean_scancodes,SC_CTRL);
  Bts(ltf_clean_scancodes,SC_SHIFT);
  Bts(ltf_clean_scancodes,SC_CAPS);
  Bts(ltf_clean_scancodes,SC_NUM);
  Bts(ltf_clean_scancodes,SC_SCROLL);
  Bts(ltf_clean_scancodes,SC_HOME);
  Bts(ltf_clean_scancodes,SC_END);
  Bts(ltf_clean_scancodes,SC_GUI);
  Bts(ltf_clean_scancodes,SC_BACKSPACE); //Handled specially
}

LtfTablesInit;

public BoolI64 LtfLock(Ltf *l)
{
  if (!Bt(&l->locked_flags,LTFlf_LOCKED) || l->owning_task!=Fs) {
    while (LBts(&l->locked_flags,LTFlf_LOCKED))
      Yield;
    if (l->owning_task!=Fs)
      LBEqu(&l->flags,LTFf_BREAK_UNLOCKED,BreakLock(Fs));
    l->owning_task=Fs;
    return TRUE;
  } else
    return FALSE;
}

public BoolI64 LtfUnlock(Ltf *l)
{
  BoolI8 unlock_break;
  if (Bt(&l->locked_flags,LTFlf_LOCKED) && l->owning_task==Fs) {
    l->owning_task=0;
    unlock_break=Bt(&l->flags,LTFf_BREAK_UNLOCKED);
    LBtr(&l->locked_flags,LTFlf_LOCKED);
    if (unlock_break)
      BreakUnlock(Fs);
    return TRUE;
  } else
    return FALSE;
}


LtfBinEntry *LtfBinFind(Ltf *l,U64 num)
{
  LtfBinEntry *b=l->bin_root.next;
  while (b!=&l->bin_root) {
    if (b->num==num)
      return b;
    b=b->next;
  }
  return NULL;
}

U0 LtfBinsValidate(Ltf *l)
{
  LtfBinEntry *b=l->bin_root.next,*b1;
  LtfEntry *cl=l->root.next,*cl2;
  I64 renum_num=0;
  while (b!=&l->bin_root) {
    b->use_cnt=0;
    b->temp_use_cnt=0;
    b->renum_num=-1;
    b=b->next;
  }
  while (cl!=l) {
    cl2=cl->next;
    if (cl->flags & LTFLF_HAS_BIN) {
      if (cl->bin_data=LtfBinFind(l,cl->bin_num)) {
	if (!cl->bin_data->use_cnt++)
	  cl->bin_data->renum_num=++renum_num;
	cl->bin_num=cl->bin_data->renum_num;
      } else {
	DbgStr("Bin Not Found",3000);
	QueRem(cl); //don't attempt to delete
      }
    }
    cl=cl2;
  }

  b=l->bin_root.next;
  l->cur_bin_num=1;
  while (b!=&l->bin_root) {
    b1=b->next;
    if (!b->use_cnt) {
      QueRem(b);
      Free(b->data);
      Free(b);
    } else {
      b->num=b->renum_num;
      if (b->num>=l->cur_bin_num)
	l->cur_bin_num=b->num+1;
    }
    b=b1;
  }
}


U0 LtfBinDel(Ltf *l,LtfBinEntry *b)
{
  if (l && b && b->use_cnt) {
    b->use_cnt--;
    if (!b->use_cnt) {
      QueRem(b);
      Free(b->data);
      Free(b);
    }
  } else
    DbgStr("LtfBinDel",3000);
}

public U0 LtfEntryDel(Ltf *l,LtfEntry *cl)
{
  if (!l || l==cl)
    DbgStr("LtfEntryDel",3000);
  else {
    if (l->cur_entry==cl)
      l->cur_entry=cl->next;
    QueRem(cl);
    if (Bt(ltf_tag_types,cl->type_u8))
      Free(cl->tag);
    if (cl->flags & LTFLF_LEFT_MACRO)
      Free(cl->left_macro);
    if (cl->flags & LTFLF_RIGHT_MACRO)
      Free(cl->right_macro);
    if (cl->flags & LTFLF_AUX_STR)
      Free(cl->aux_str);
    if (cl->flags & LTFLF_DEFINE)
      Free(cl->define_str);
    if (cl->flags & LTFLF_HTML_LINK)
      Free(cl->html_link);
    if (cl->flags & LTFLF_HAS_BIN)
      LtfBinDel(l,cl->bin_data);
    Free(cl);
  }
}

public U64 LtfEntrySize(Ltf *,LtfEntry *cl)
{
  U64 result=0;
  if (!cl) return 0;
  result+=MSize2(cl);
  if (Bt(ltf_tag_types,cl->type_u8))
    result+=MSize2(cl->tag);
  if (cl->flags & LTFLF_AUX_STR)
    result+=MSize2(cl->aux_str);
  if (cl->flags & LTFLF_DEFINE)
    result+=MSize2(cl->define_str);
  if (cl->flags & LTFLF_HTML_LINK)
    result+=MSize2(cl->html_link);
  if (cl->flags & LTFLF_LEFT_MACRO)
    result+=MSize2(cl->left_macro);
  if (cl->flags & LTFLF_RIGHT_MACRO)
    result+=MSize2(cl->right_macro);
  return result;
}

U0 LtfUndoDel(Ltf *,LtfUndo *u)
{
  Free(u->body);
  Free(u);
}

U0 LtfUndoCntSet(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  LtfUndo *u=l->undo_root.next;
  l->undo_cnt=0;
  while (u!=&l->undo_root) {
    l->undo_cnt++;
    u=u->next;
  }
  if (unlock)
    LtfUnlock(l);
}

public LtfEntry *LtfEntryCopy(Ltf *l,LtfEntry *cl)
{
  LtfEntry *nl;
  LtfBinEntry *tempb;
  TaskStruct *task=l->mem_task;
  nl=MAllocIdentical(cl,task);
  nl->next=nl;
  nl->last=nl;
  nl->settings=NULL;
  if (Bt(ltf_tag_types,cl->type_u8))
    nl->tag=MAllocIdentical(cl->tag,task);
  if (cl->flags & LTFLF_AUX_STR)
    nl->aux_str=MAllocIdentical(cl->aux_str,task);
  if (cl->flags & LTFLF_DEFINE)
    nl->define_str=MAllocIdentical(cl->define_str,task);
  if (cl->flags & LTFLF_HTML_LINK)
    nl->html_link=MAllocIdentical(cl->html_link,task);
  if (cl->flags & LTFLF_LEFT_MACRO)
    nl->left_macro=MAllocIdentical(cl->left_macro,task);
  if (cl->flags & LTFLF_RIGHT_MACRO)
    nl->right_macro=MAllocIdentical(cl->right_macro,task);
  if (cl->flags & LTFLF_HAS_BIN) {
    tempb=MAllocIdentical(cl->bin_data,task);
    tempb->data=MAllocIdentical(cl->bin_data->data,task);
    nl->bin_num=l->cur_bin_num;
    tempb->num=l->cur_bin_num++;
    nl->bin_data=tempb;
    QueIns(tempb,l->bin_root.last);
  }
  return nl;
}

U0 LtfFormFwd(Ltf *l,BoolI8 giveup=FALSE)
{
  LtfEntry *cl=l->cur_entry,*cl2=cl;
  if (l->flags & LTFF_FORM) {
    if (cl==l) goto recover;
    while (!Bt(ltf_form_types,cl->type_u8) ||
       cl->flags&LTFLF_SKIP_IN_FORM) {
      cl=cl->next;
      if (cl==l) {
recover:
	l->cur_data_col=0;
	if (!giveup) {
	  l->cur_entry=cl->last;
	  LtfFormBackward(l,TRUE);
	} else
	  l->cur_entry=l;
	return;
      }
    }
  }
  while (cl->type_u8==LTFT_INDENT)
    cl=cl->next;
  if (cl!=cl2) {
    l->cur_data_col=cl->min_col;
    l->cur_entry=cl;
  }
}

U0 LtfFormBackward(Ltf *l,BoolI8 giveup=FALSE)
{
  LtfEntry *cl=l->cur_entry,*cl2=cl;
  if (l->flags & LTFF_FORM) {
    while (!Bt(ltf_form_types,cl->type_u8) ||
       cl->flags&LTFLF_SKIP_IN_FORM) {
      cl=cl->last;
      if (cl==l) {
	l->cur_data_col=0;
	if (!giveup) {
	  l->cur_entry=cl->next;
	  LtfFormFwd(l,TRUE);
	} else
	  l->cur_entry=l;
	return;
      }
    }
  }
  while (cl->type_u8==LTFT_INDENT)
    cl=cl->next;
  if (cl!=cl2) {
    l->cur_data_col=cl->min_col;
    l->cur_entry=cl;
  }
}

public U0 LtfHome(Ltf *l=NULL)
{
  BoolI8 unlock;
  if (!l) {
    l=LtfCur;
    if (!l) return;
  }
  unlock=LtfLock(l);
  l->cur_entry=l->root.next;
  l->cur_data_col=l->cur_entry->min_col;
  l->x=0;
  l->y=0;
  l->line_start_col=0;
  l->cur_top_line_num=0;

  LtfFormFwd(l);
  if (unlock)
    LtfUnlock(l);
}

U0 LtfSettlingsRemainingDel(Ltf *l,LtfEntry *cl,LtfSettingsStruct *s)
{
  LtfSettingsStruct *s1;
  if (cl) {
    while (cl!=l) {
      cl->settings=NULL;
      cl=cl->next;
    }
  }
  if (!s) return;
  while (s!=&l->settings_root) {
    s1=s->next;
    QueRem(s);
    Free(s);
    s=s1;
  }
}

LtfSettingsStruct *LtfSettingsIdentical(Ltf *l,LtfSettingsStruct *s1)
{
  LtfSettingsStruct *s=MAlloc(sizeof(LtfSettingsStruct),l->mem_task);
  MemCpy(&s->start,&s1->start,sizeof(LtfSettingsStruct)-offset(LtfSettingsStruct.start));
  return s;
}

LtfSettingsStruct *LtfSettingsLast(LtfEntry *cl)
{
  while (!cl->settings)
    cl=cl->last;
  return cl->settings;
}

LtfSettingsStruct *LtfSettingsNext(LtfEntry *cl)
{
  while (!cl->settings)
    cl=cl->next;
  return cl->settings;
}

public U0 LtfRst(Ltf *l,BoolI8 is_old)
{
  BoolI8 unlock;
  LtfEntry *cl,*cl1;
  LtfUndo *u,*u8;
  LtfSettingsStruct *s;
  LtfBinEntry *b,*b1;
  if (!l) {
    l=LtfCur;
    if (!l) return;
  }
  unlock=LtfLock(l);
  if (is_old) {
    cl=l->root.next;
    while (cl!=l) {
      cl1=cl->next;
      LtfEntryDel(l,cl);
      cl=cl1;
    }
    LtfSettlingsRemainingDel(l,NULL,l->settings_root.next);
    u=l->undo_root.next;
    while (u!=&l->undo_root) {
      u8=u->next;
      LtfUndoDel(l,u);
      u=u8;
    }
    b=l->bin_root.next;
    while (b!=&l->bin_root) {
      b1=b->next;
      QueRem(b);
      Free(b->data);
      Free(b);
      b=b1;
    }
  }
  //Check [C:/LT/Adam/Ltf/LtfFile.APZ,196] LtfInsLtf
  l->flags&=LTFF_BREAK_UNLOCKED;
  l->root.next=l->root.last=l;
  QueInit(&l->bin_root);
  QueInit(&l->undo_root);
  l->undo_root.time_stamp=0;
  l->undo_cnt=0;
  l->cur_bin_num=1;
  l->recalc_start=l;
  l->menu_ltf=NULL;
  l->dollar_buf_ptr=0;
  l->ext_ascii_cnt=l->ext_ascii_val=0;
  l->cmd_U8=CH_SPACE;
  l->page_line_num=0;
  l->best_d=MAX_I64;

  s=&l->settings_root;
  s->next=s->last=s;
  s->left_margin=LTF_DFT;
  s->right_margin=LTF_DFT;
  s->indent=0;
  s->page_len=66;
  s->header=LTF_DFT;
  s->footer=LTF_DFT;
  s->state=LTFSS_NORMAL;
  s->comment_depth=0;
  s->paren_depth=0;
  s->brace_depth=0;
  s->shifted_x=0;
  s->shifted_y=0;

  s->cur_text_attr=s->dft_text_attr=l->start_text_attr=WHITE<<4+BLACK;
  s->alt_text_attr	=LTGRAY;
  s->link_attr		=RED;
  s->macro_attr		=LTBLUE;
  s->anchor_attr	=DKGRAY;
  s->hidden_attr	=DKGRAY;
  s->tree_attr		=PURPLE;
  s->user_attr		=GREEN;
  s->bin_attr		=CYAN;
  s->keyword_attr	=BLUE;
  s->keyword2_attr	=LTRED;
  s->comment_attr	=GREEN;
  s->quote_attr		=BROWN;

  cl=&l->root;
  cl->type=LTFT_ERROR;
  cl->flags=0;
  cl->x=0;
  cl->y=0;
  cl->min_col=0;
  cl->max_col=0;
  cl->page_line_num=l->page_line_num;
  cl->settings=s;

  LtfHome(l);
  if (unlock)
    LtfUnlock(l);
}


public U0 LtfDel(Ltf *l)
{
  Ltf *m;
  if (!l || l->ltf_signature!=LTF_SIGNATURE_VAL) return;
  LtfLock(l);
  l->ltf_signature=0;
  l->ltf_in_que_signature=0;
  m=l->menu_ltf;
  LtfRst(l,TRUE);
  Free(l->find_replace);
  Free(l->dollar_buf);
  LtfDel(m);
  LtfUnlock(l);
  Free(l);
}

public U64 LtfSize(Ltf *l)
{
 U64 result=0;
  BoolI8 unlock;
  LtfEntry *cl;
  LtfUndo *u;
  LtfSettingsStruct *s;
  LtfBinEntry *b;
  Ltf *m;

  if (!l || l->ltf_signature!=LTF_SIGNATURE_VAL) return 0;
  unlock=LtfLock(l);

  m=l->menu_ltf;

  cl=l->root.next;
  while (cl!=l) {
    result+=LtfEntrySize(l,cl);
    cl=cl->next;
  }

  s=l->settings_root.next;
  while (s!=&l->settings_root) {
    result+=MSize2(s);
    s=s->next;
  }

  u=l->undo_root.next;
  while (u!=&l->undo_root) {
    result+=MSize2(u->body);
    result+=MSize2(u);
    u=u->next;
  }

  b=l->bin_root.next;
  while (b!=&l->bin_root) {
    result+=MSize2(b->data);
    result+=MSize2(b);
    b=b->next;
  }

  result+=MSize2(l->find_replace);
  result+=MSize2(l->dollar_buf);
  if (m)
    result+=LtfSize(m);
  result+=MSize2(l);
  if (unlock)
    LtfUnlock(l);
  return result;
}


U0 LtfDataFmt(LtfEntry *cl)
{
  I64 i;
  U64 d;
  U8 *ptr;
  if (cl->type_u8==LTFT_DATA || cl->type_u8==LTFT_CHECK_BOX) {
    if (cl->flags & LTFLF_DATA_IS_PTR) {
      if (!(ptr=cl->data)) return;
    } else
      ptr=&cl->data;
    if (cl->flags&LTFLF_U8)
      d=*ptr;
    else if (cl->flags&LTFLF_U16)
      d=*ptr(U16 *);
    else if (cl->flags&LTFLF_U32)
      d=*ptr(U32 *);
    else
      d=*ptr(U64 *);
    if (cl->type_u8==LTFT_DATA) {
      SPrintF(cl->tag,cl->aux_str,d,cl->my_fmt_data);
      if (cl->flags & LTFLF_HAS_TERMINATOR) {
	i=StrLen(cl->tag);
	cl->tag[i++]='_';
	cl->tag[i]=0;
      }
      cl->max_col=StrLen(cl->tag);
    } else {
      if (d)
	cl->flags|=LTFLF_CHECKED_COLLAPSED;
      else
	cl->flags&=~LTFLF_CHECKED_COLLAPSED;
    }
  }
}

U0 LtfDataGet(LtfEntry *cl)
{
  I64 i;
  U64 d;
  U8 *ptr;
  if (cl->type_u8==LTFT_DATA || cl->type_u8==LTFT_CHECK_BOX) {
    if (cl->flags & LTFLF_DATA_IS_PTR) {
      if (!(ptr=cl->data)) return;
    } else
      ptr=&cl->data;
    if (cl->type_u8==LTFT_DATA) {
      i=StrLen(cl->tag);
      if (cl->flags & LTFLF_HAS_TERMINATOR)
	cl->tag[--i]=0;
      if (i>cl->len+cl->min_col)
	cl->tag[cl->len+cl->min_col]=0;
      if (cl->flags&(LTFLF_U8|LTFLF_U16|LTFLF_U32)) {
	GetF(cl->tag,cl->aux_str,&d,cl->my_fmt_data);
	if (cl->flags & LTFLF_HAS_TERMINATOR)
	  cl->tag[i]='_';
      } else {
	GetF(cl->tag,cl->aux_str,ptr,cl->my_fmt_data);
	if (cl->flags & LTFLF_HAS_TERMINATOR)
	  cl->tag[i]='_';
	return;
      }
    } else {
      if (cl->flags & LTFLF_CHECKED_COLLAPSED)
	d=TRUE;
      else
	d=FALSE;
    }
    if (cl->flags&LTFLF_U8)
      *ptr=d;
    else if (cl->flags&LTFLF_U16)
      *ptr(U16 *)=d;
    else if (cl->flags&LTFLF_U32)
      *ptr(U32 *)=d;
    else
      *ptr(U64 *)=d;
  }
}

U0 LtfRemSoftNewLines(Ltf *l=NULL,LtfEntry *cl=NULL)
{
  LtfEntry *cl2,*saved_cl=cl;
  BoolI8 unlock;
  if (!l) {
    l=LtfCur;
    if (!l) return;
  }
  unlock=LtfLock(l);
  if (!cl) cl=l->root.next;
  while (cl!=l) {
    cl2=cl->next;
    if (cl->type_u8==LTFT_SOFT_NEW_LINE) {
      if (l->cur_entry==cl) {
	l->cur_entry=cl2;
	l->cur_data_col=l->cur_entry->min_col;
      }
      LtfEntryDel(l,cl);
    } else if (saved_cl && cl->type_u8==LTFT_NEW_LINE)
      break;
    cl=cl2;
  }
  if (unlock)
    LtfUnlock(l);
}

#help_index "Ltf/Cmd Line (Typically);Cmd Line (Typically)"

public I64 LtfMax(I64 i=MAX_I32)
//Adjusts the size of the Ltf buf.
//Normally, the cmd line deletes
//entries after a while.
{
  Ltf *l=LtfCur;
  I64 result;
  if (l) {
    result=l->max_entries;
    l->max_entries=i;
    return result;
  } else
    return 0;
}

#help_index "Ltf"
public Ltf *LtfNew(TaskStruct *task=NULL)
{
  Ltf *l;
  if (!task) task=Fs;
  l=CAlloc(sizeof(Ltf),task);
  StrCpy(l->filename.name,sys_temp_filename);
  l->find_replace=CAlloc(sizeof(EdFindTextStruct),task);
  l->find_replace->scan_fwd=TRUE;
  l->find_replace->match_case=TRUE;
  l->find_replace->pmt=TRUE;
  l->left_click_link=&EdLeftClickLink;
  l->dollar_buf_size=84;
  l->dollar_buf=MAlloc(l->dollar_buf_size,task);
  l->max_entries=MAX_I32;
  l->ww_lines_back=32;
  l->win_task=task;
  l->mem_task=task;
  LtfRst(l,FALSE);
  l->ltf_signature=LTF_SIGNATURE_VAL;
  return l;
}

sys_clipboard_ltf=LtfNew;

#help_index "Ltf/Task"
public U0 LtfQueIns(Ltf *l,Ltf *pred)
{
  Ltf *succ;
  BoolI8 *old_preempt;
  if (!l) return;
  old_preempt=Preempt(OFF);
  succ=pred->next_ltf;
  l->last_ltf=pred;
  l->next_ltf=succ;
  pred->next_ltf=l;
  succ->last_ltf=l;
  l->ltf_in_que_signature=LTF_IN_QUE_SIGNATURE_VAL;
  Preempt(old_preempt);
}

public U0 LtfQueInsRev(Ltf *l,Ltf *succ)
{
  Ltf *pred;
  BoolI8 *old_preempt;
  if (!l) return;
  old_preempt=Preempt(OFF);
  pred=succ->last_ltf;
  l->last_ltf=pred;
  l->next_ltf=succ;
  pred->next_ltf=l;
  succ->last_ltf=l;
  l->ltf_in_que_signature=LTF_IN_QUE_SIGNATURE_VAL;
  Preempt(old_preempt);
}

public U0 LtfQueRem(Ltf *l)
{
  Ltf *succ,*pred;
  BoolI8 *old_preempt;
  if (!l) return;
  old_preempt=Preempt(OFF);
  l->ltf_in_que_signature=0;
  succ=l->next_ltf;
  pred=l->last_ltf;
  pred->next_ltf=succ;
  succ->last_ltf=pred;
  Preempt(old_preempt);
}

public Ltf *LtfCur(TaskStruct *task=NULL)
{
  Ltf *result;
  if (!task) task=Fs;
  result=task->last_ltf;
  if (result==(&task->next_ltf)(U8 *)-offset(Ltf.next_ltf) ||
      result->flags&LTFF_AUX_LTF ||
      result->ltf_in_que_signature!=LTF_IN_QUE_SIGNATURE_VAL)
    result=NULL;
  return result;
}

public Ltf *LtfAux(TaskStruct *task=NULL)
{
  Ltf *result;
  if (!task) task=Fs;
  result=task->next_ltf;
  if (result==(&task->next_ltf)(U8 *)-offset(Ltf.next_ltf) ||
      !(result->flags&LTFF_AUX_LTF) ||
      result->ltf_in_que_signature!=LTF_IN_QUE_SIGNATURE_VAL)
    result=NULL;
  return result;
}


public Ltf *LtfDblBufStart(TaskStruct *task=NULL)
{ //See [::/LT/Demo/Spy.CPZ] ::/LT/Demo/Spy.CPZ
  Ltf *l,*l2;
  BoolI8 unlock,old_preempt=Preempt(OFF);
  if (!task) task=Fs;
  l2=task->last_ltf;
  unlock=LtfLock(l2); //dont want change during winupdate, so lock LtfCur
  l=LtfNew(task);
  l->win_task	=l2->win_task;
  l->start_text_attr=l2->start_text_attr;
  l->max_entries=l2->max_entries;
  MemCpy(l->find_replace,l2->find_replace,sizeof(EdFindTextStruct));
  l->menu_ltf	=l2->menu_ltf;
  l->start_x	=l2->start_x;
  l->start_y	=l2->start_y;
  MemCpy(&l->filename,&l2->filename,sizeof(EdFileNameStruct));
  l->left_click_link =l2->left_click_link;
  l->right_click_link=l2->right_click_link;
  l->user_put_data=l2->user_put_data;
  l->user_put_key =l2->user_put_key;
  l->user_put_s   =l2->user_put_s;
  l->parent_ltf	=l2->parent_ltf;
  l->desc[0](U64)=l2->desc[0](U64);
  l->user_data	=l2->user_data;
  l->flags|=l2->flags&LTF_DBL_BUF_FLAGS | LTFF_DONT_SHOW;
  LtfQueIns(l,task->last_ltf);
  if (unlock)
    LtfUnlock(l2);
  Preempt(old_preempt);
  return l;
}

public BoolI8 LtfDblBufEnd(TaskStruct *task=NULL)
{ //See [::/LT/Demo/Spy.CPZ] ::/LT/Demo/Spy.CPZ
  BoolI8 okay=FALSE,old_preempt=Preempt(OFF);
  Ltf *l1,*l2;
  if (!task) task=Fs;
  if (l1=LtfCur(task)) {
    l2=l1->last_ltf;
    if (l2!=(&task->next_ltf)(U8 *)-offset(Ltf.next_ltf) &&
	!(l2->flags&LTFF_AUX_LTF) &&
	l2->ltf_in_que_signature==LTF_IN_QUE_SIGNATURE_VAL) {
      l2->flags|=LTFF_DONT_SHOW;
      l1->flags&=~LTFF_DONT_SHOW;
      LtfLock(l2);
      LtfQueRem(l2);
      LtfUnlock(l2);
      l2->menu_ltf=NULL;
      LtfDel(l2);
      okay=TRUE;
    }
  }
  Preempt(old_preempt);
  return okay;
}

public BoolI8 LtfDblBufSwap(TaskStruct *task=NULL)
{ //See [::/LT/Demo/Spy.CPZ] ::/LT/Demo/Spy.CPZ
  BoolI8 okay=FALSE,old_preempt=Preempt(OFF);
  Ltf *l1,*l2;
  if (!task) task=Fs;
  if (l1=LtfCur(task)) {
    l2=l1->last_ltf;
    if (l2!=(&task->next_ltf)(U8 *)-offset(Ltf.next_ltf) &&
	!(l2->flags&LTFF_AUX_LTF) &&
	l2->ltf_in_que_signature==LTF_IN_QUE_SIGNATURE_VAL) {
      l2->flags|=LTFF_DONT_SHOW;
      l1->flags&=~LTFF_DONT_SHOW;
      LtfLock(l2);
      LtfQueRem(l2);
      LtfUnlock(l2);
      LtfRst(l2,TRUE);
      MemCpy(l2->find_replace,l1->find_replace,sizeof(EdFindTextStruct));
      MemCpy(&l2->filename,&l1->filename,sizeof(EdFileNameStruct));
      l2->start_text_attr=l1->start_text_attr;
      l2->menu_ltf     =l1->menu_ltf;
      l2->max_entries  =l1->max_entries;
      l2->flags        =l1->flags&LTF_DBL_BUF_FLAGS | l2->flags&~LTF_DBL_BUF_FLAGS;
      l2->left_click_link =l1->left_click_link;
      l2->right_click_link=l1->right_click_link;
      l2->user_put_data=l1->user_put_data;
      l2->user_put_key =l1->user_put_key;
      l2->user_put_s   =l1->user_put_s;
      l2->desc[0](U64)=l1->desc[0](U64);
      l2->user_data    =l1->user_data;
      LtfQueIns(l2,task->last_ltf);
      okay=TRUE;
    }
  }
  Preempt(old_preempt);
  return okay;
}
#help_index "Ltf"
