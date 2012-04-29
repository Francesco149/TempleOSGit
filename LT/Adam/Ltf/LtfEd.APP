#help_index "Ltf/Edit"
#define ED_MENU_NAME "::/LT/Doc/Ed.TXZ"

public LtfEntry *LtfAnchorFind(Ltf *l,U8 *pattern)
{
  BoolI8 unlock=LtfLock(l);
  LtfEntry *ll=l->root.next;
  while (ll!=l) {
    if (ll->type_u8==LTFT_ANCHOR) {
      if (!StrCmp(pattern,ll->aux_str)) {
	l->cur_entry=ll;
	l->cur_data_col=ll->min_col;
	if (unlock)
	  LtfUnlock(l);
	return ll;
      }
    }
    ll=ll->next;
  }
  if (unlock)
    LtfUnlock(l);
  return NULL;
}

public I64 EdCurU8(Ltf *l)
{
//See [C:/LT/Adam/Ltf/LtfCodeTools.APZ,150] EdRenumAsm for an example.
  I64 result=-1;
  BoolI8 unlock=LtfLock(l);
  LtfEntry *cl=l->cur_entry;
  if (cl->type_u8==LTFT_TEXT && cl->min_col<=l->cur_data_col<cl->max_col)
    result=cl->tag[l->cur_data_col];
  else if (cl->type_u8==LTFT_TAB)
    result=CH_TAB;
  else if (cl->type_u8==LTFT_NEW_LINE || cl->type_u8==LTFT_SOFT_NEW_LINE)
    result=CH_NEW_LINE;
  if (unlock)
    LtfUnlock(l);
  return result;
}

public U0 EdCursorLeft(Ltf *l,I64 sc=MIN_I64)
{//You might need a call to [C:/LT/Adam/Ltf/LtfRecalc.APZ,110] LtfRecalc().
//See [C:/LT/Adam/Ltf/LtfCodeTools.APZ,150] EdRenumAsm for an example.
  U8 *dst;
  BoolI8 unlock=LtfLock(l);
  LtfEntry *cl=l->cur_entry,*original_cl=cl,*nl;
  I64 cc=l->cur_data_col,y=cl->y;
  if (sc!=MIN_I64) sc=sc.u32[0];
  if (sc>=0 && sc&SCF_CTRL) {
    while (cl->last!=l && (cl->last->y==y ||
	cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP)))
      cl=cl->last;  //TODO: select? recurse?
    cc=cl->min_col;
  } else {
    if (cc>cl->min_col) {
      if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) && cc<cl->max_col) {
	nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	nl->type=cl->type;
	nl->flags=cl->flags;
	dst=cl->tag+cc;
	nl->tag=StrNew(dst,l->mem_task);
	nl->max_col=StrLen(dst);
	nl->x=cl->x;
	nl->y=cl->y;
	nl->page_line_num=cl->page_line_num;
	*dst=0;
	cl->max_col=cc;
	InsQue(nl,cl);
      }
      cc--;
      if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) && cc>cl->min_col) {
	nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	nl->type=cl->type;
	nl->flags=cl->flags;
	dst=cl->tag+cc;
	nl->tag=StrNew(dst,l->mem_task);
	nl->max_col=StrLen(dst);
	nl->x=cl->x;
	nl->y=cl->y;
	nl->page_line_num=cl->page_line_num;
	*dst=0;
	cl->max_col=cc;
	InsQue(nl,cl);
	cl=nl;
	cc=cl->min_col;
      }
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  cl->type|=LTFLT_SELECTED;
	else
	  cl->type&=~LTFLT_SELECTED;
      }
    } else {
      cc=cl->min_col;
      while (cl->last!=l &&
	(cl->last->type_u8==LTFT_SOFT_NEW_LINE || cl->last->type_u8==LTFT_INDENT ||
	cl->last->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
	cl=cl->last;
	if (sc>=0) {
	  if (sc&SCF_SHIFT)
	    cl->type|=LTFLT_SELECTED;
	  else
	    cl->type&=~LTFLT_SELECTED;
	}
      }
      if (cl->last!=l) {
	cl=cl->last;
	if (cl->max_col>cl->min_col) {
	  cc=cl->max_col-1;
	  if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))&& cc>cl->min_col) {
	    nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	    nl->type=cl->type;
	    nl->flags=cl->flags;
	    dst=cl->tag+cc;
	    nl->tag=StrNew(dst,l->mem_task);
	    nl->max_col=StrLen(dst);
	    nl->x=cl->x;
	    nl->y=cl->y;
	    nl->page_line_num=cl->page_line_num;
	    *dst=0;
	    cl->max_col=cc;
	    InsQue(nl,cl);
	    cl=nl;
	    cc=cl->min_col;
	  }
	} else
	  cc=cl->max_col;
	if (sc>=0) {
	  if (sc&SCF_SHIFT)
	    cl->type|=LTFLT_SELECTED;
	  else
	    cl->type&=~LTFLT_SELECTED;
	}
      }
    }
  }
  l->cur_data_col=cc;
  l->cur_entry=cl;
  if (cl!=original_cl)
    LtfFormBackward(l);
  l->recalc_start=l->cur_entry->last;
  if (unlock)
    LtfUnlock(l);
}

public U0 EdCursorRight(Ltf *l,I64 sc=MIN_I64)
{//You might need a call to [C:/LT/Adam/Ltf/LtfRecalc.APZ,110] LtfRecalc().
//See [C:/LT/Adam/Ltf/LtfCodeTools.APZ,150] EdRenumAsm for an example.
  BoolI8 unlock=LtfLock(l);
  U8 *dst;
  LtfEntry *cl=l->cur_entry,*original_cl=cl,*nl;
  I64 cc=l->cur_data_col,y=cl->y,old_flags,old_color;
  l->recalc_start=cl->last;
  if (sc!=MIN_I64) sc=sc.u32[0];
  if (sc>=0 && sc&SCF_CTRL) {
    while (cl!=l && cl->next->y==y &&
	   cl->next->type_u8!=LTFT_SOFT_NEW_LINE && cl->next!=l && (cl->next->type_u8!=LTFT_NEW_LINE || !(l->flags & LTFF_FORM)) ||
	   cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))
      cl=cl->next;
    if (cl->max_col>cl->min_col)
      cc=cl->max_col-1;
    else
      cc=cl->min_col;
  } else {
    if (cc<cl->max_col) {
      if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) && cc>cl->min_col) {
	nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	nl->type=cl->type;
	nl->flags=cl->flags;
	nl->x=cl->x;
	nl->y=cl->y;
	nl->page_line_num=cl->page_line_num;
	dst=cl->tag+cc;
	nl->tag=StrNew(dst,l->mem_task);
	nl->max_col=StrLen(dst);
	*dst=0;
	cl->max_col=cc;
	InsQue(nl,cl);
	cl=nl;
	cc=cl->min_col;
      }
      cc++;
      old_flags=cl->flags;
      old_color=cl->type;
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  cl->type|=LTFLT_SELECTED;
	else
	  cl->type&=~LTFLT_SELECTED;
      }
      if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) && cc<cl->max_col) {
	nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	nl->type=LTFT_TEXT | old_color & -0x100;
	nl->flags=old_flags;
	dst=cl->tag+cc;
	nl->tag=StrNew(dst,l->mem_task);
	nl->max_col=StrLen(dst);
	cl->max_col=cc;
	*dst=0;
	nl->x=cl->x;
	nl->y=cl->y;
	nl->page_line_num=cl->page_line_num;
	InsQue(nl,cl);
	cl=nl;
	cc=cl->min_col;
      } else if (cc>=cl->max_col) {
	cl=cl->next;
	cc=cl->min_col;
      }
    } else {
      if (cl!=l) {
	if (cc<=cl->min_col && sc>=0) {
	  if (sc&SCF_SHIFT)
	    cl->type|=LTFLT_SELECTED;
	  else
	    cl->type&=~LTFLT_SELECTED;
	}
	cl=cl->next;
	while (cl!=l && cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP)) {
	  if (sc>=0) {
	    if (sc&SCF_SHIFT)
	      cl->type|=LTFLT_SELECTED;
	    else
	      cl->type&=~LTFLT_SELECTED;
	  }
	  cl=cl->next;
	}
	cc=cl->min_col;
	if (cl->type_u8==LTFT_SOFT_NEW_LINE) {
	  if (sc>=0) {
	    if (sc&SCF_SHIFT)
	      cl->type|=LTFLT_SELECTED;
	    else
	      cl->type&=~LTFLT_SELECTED;
	  }
	  cl=cl->next;
	  cc=cl->min_col;
	}
      }
    }
  }
  l->cur_data_col=cc;
  l->cur_entry=cl;
  if (cl!=original_cl)
    LtfFormFwd(l);
  if (unlock)
    LtfUnlock(l);
}

public U0 EdLineUp(Ltf *l,I64 sc=MIN_I64)
{//You might need a call to [C:/LT/Adam/Ltf/LtfRecalc.APZ,110] LtfRecalc().
//See [C:/LT/Adam/Ltf/LtfCodeTools.APZ,150] EdRenumAsm for an example.
  BoolI8 unlock=LtfLock(l);
  U8 *dst;
  I64 y,x;
  LtfEntry *cl=l->cur_entry,*nl;

  if (sc!=MIN_I64) sc=sc.u32[0];
  if (cl->type_u8==LTFT_HEX_ED) {
    l->cur_data_col=l->cur_data_col-cl->hex_ed_width*3;
    if (l->cur_data_col>=0) {
      if (unlock)
	LtfUnlock(l);
      return;
    } else
      l->cur_data_col=0;
  }
  x=l->x; y=l->y;
  if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
    if (cl->min_col<l->cur_data_col<cl->max_col-1) {
      nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
      nl->type=cl->type;
      nl->flags=cl->flags;
      dst=cl->tag+l->cur_data_col;
      nl->tag=StrNew(dst,l->mem_task);
      nl->max_col=StrLen(dst);
      nl->y=cl->y;
      nl->x=cl->x+l->cur_data_col;
      nl->page_line_num=cl->page_line_num;
      *dst=0;
      cl->max_col=l->cur_data_col;
      InsQue(nl,cl);
    } else
      if (l->cur_data_col==cl->min_col && cl->last!=l)
	cl=cl->last;
  } else
    if (cl->last!=l)
      cl=cl->last;
  if (sc>=0) {
    if (sc&SCF_SHIFT)
      cl->type|=LTFLT_SELECTED;
    else
      cl->type&=~LTFLT_SELECTED;
  }
  l->cur_entry=cl;
  LtfFormBackward(l);
  cl=l->cur_entry;
  while (cl->last!=l && (cl->y>=y || cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
    cl=cl->last;
    if (sc>=0) {
      if (sc&SCF_SHIFT)
	cl->type|=LTFLT_SELECTED;
      else
	cl->type&=~LTFLT_SELECTED;
    }
  }
  y=cl->y;
  l->y=y;
  while (cl!=l && (cl->y>=y && cl->x>=x || cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
    if (sc>=0) {
      if (sc&SCF_SHIFT)
	cl->type|=LTFLT_SELECTED;
      else
	cl->type&=~LTFLT_SELECTED;
    }
    cl=cl->last;
  }

  if (cl==l || cl->y<y)
    cl=cl->next;
  else {
    if (cl->type_u8!=LTFT_TEXT|| cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) {
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  cl->type|=LTFLT_SELECTED;
	else
	  cl->type&=~LTFLT_SELECTED;
      }
    } else {
      if (cl->next->x==x) {
	cl=cl->next;
	if (l->flags & LTFF_FORM) {
	  while (cl->next->x==x &&
	    (!Bt(ltf_form_types,cl->type_u8) ||
	     cl->flags&LTFLF_SKIP_IN_FORM))
	    cl=cl->next;
	}
      }
    }
  }
  if (Bt(ltf_tag_types,cl->type_u8)) {
    l->cur_data_col=x-cl->x;
    if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
      if (l->cur_data_col>cl->max_col)
	l->cur_data_col=cl->max_col;
    } else {
      if (l->cur_data_col>=cl->max_col)
	l->cur_data_col=cl->max_col-1;
    }
    if (l->cur_data_col<cl->min_col)
      l->cur_data_col=cl->min_col;
  } else {
    if (cl->type_u8==LTFT_HEX_ED) {
      l->cur_data_col=RoundI64((cl->len-1)*3,cl->hex_ed_width*3);
      if (l->cur_data_col<0)
	l->cur_data_col=0;
    } else
      l->cur_data_col=cl->min_col;
  }
  if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) && cl->x<x) {
    if (l->cur_data_col<cl->max_col-1) {
      nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
      nl->type=cl->type;
      nl->flags=cl->flags;
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  nl->type=cl->type | LTFLT_SELECTED;
	else
	  nl->type=cl->type & ~LTFLT_SELECTED;
      }
      dst=cl->tag+l->cur_data_col;
      nl->tag=StrNew(dst,l->mem_task);
      nl->max_col=StrLen(dst);
      nl->page_line_num=cl->page_line_num;
      nl->y=cl->y;
      nl->x=cl->x+l->cur_data_col;
      *dst=0;
      cl->max_col=l->cur_data_col;
      InsQue(nl,cl);
      cl=nl;
      l->cur_data_col=cl->min_col;
    }
  }
  l->cur_entry=cl;
  LtfFormFwd(l);
  l->x=l->cur_entry->x+l->cur_data_col;
  l->recalc_start=l->cur_entry->last;
  if (unlock)
    LtfUnlock(l);
}

public U0 EdLineDown(Ltf *l,I64 sc=MIN_I64)
{//You might need a call to [C:/LT/Adam/Ltf/LtfRecalc.APZ,110] LtfRecalc().
//See [C:/LT/Adam/Ltf/LtfCodeTools.APZ,150] EdRenumAsm for an example.
  BoolI8 unlock=LtfLock(l);
  U8 *dst;
  I64 y,x,old_flags=0,old_color;
  LtfEntry *cl=l->cur_entry,*nl,*cl2;
  if (sc!=MIN_I64) sc=sc.u32[0];
  if (cl->type_u8==LTFT_HEX_ED) {
    l->cur_data_col=l->cur_data_col+cl->hex_ed_width*3;
    if (l->cur_data_col>=cl->len*3) {
      l->cur_entry=cl=cl->next;
      l->cur_data_col=cl->min_col;
      l->x=cl->x+l->cur_data_col;
      l->y=cl->y;
    }
    if (unlock)
      LtfUnlock(l);
    return;
  }
  l->recalc_start=cl->last;
  x=l->x; y=l->y;
  if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
    if (l->cur_data_col>cl->min_col && l->cur_data_col<cl->max_col-1) {
      nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
      nl->type=cl->type;
      nl->flags=cl->flags;
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  nl->type=cl->type | LTFLT_SELECTED;
	else
	  nl->type=cl->type & ~LTFLT_SELECTED;
      }
      dst=cl->tag+l->cur_data_col;
      nl->tag=StrNew(dst,l->mem_task);
      nl->max_col=StrLen(dst);
      nl->page_line_num=cl->page_line_num;
      nl->y=cl->y;
      nl->x=cl->x+l->cur_data_col;
      *dst=0;
      cl->max_col=l->cur_data_col;
      InsQue(nl,cl);
      cl=nl;
      l->cur_data_col=cl->min_col;
    }
  }
  cl2=cl;
  while (cl!=l && (cl->y<=y || cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP)))
    cl=cl->next;
  y=cl->y;
  l->y=y;
  while (cl!=l && (cl->y<=y && cl->x<=x || cl->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
    old_flags=cl->flags;
    old_color=cl->type;
    cl=cl->next;
  }
  if (cl->last!=l && (cl->x>x || cl->y>y)) {
    cl=cl->last;
    l->cur_entry=cl;
    if (!((cl->type_u8==LTFT_NEW_LINE || cl->type_u8==LTFT_SOFT_NEW_LINE || cl->type_u8==LTFT_INDENT) &&
	  (cl->last->type_u8==LTFT_NEW_LINE || cl->last->type_u8==LTFT_SOFT_NEW_LINE || cl->last->type_u8==LTFT_INDENT)))
      LtfFormBackward(l);
    cl=l->cur_entry;
  }
  while (cl2!=l && (cl2!=cl || cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)))) {
    if (cl2->y<y || cl2->x<x || cl2->flags & (LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP) ||
       cl2->x==x && !cl2->max_col &&
       Bt(ltf_nontag_invisible_types,cl2->type_u8)) {
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  cl2->type|=LTFLT_SELECTED;
	else
	  cl2->type&=~LTFLT_SELECTED;
      }
    }
    if (cl2==cl) break;
    cl2=cl2->next;
  }
  if (Bt(ltf_tag_types,cl->type_u8)) {
    l->cur_data_col=x-cl->x;
    if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
      if (l->cur_data_col>cl->max_col)
	l->cur_data_col=cl->max_col;
    } else {
      if (l->cur_data_col>=cl->max_col)
	l->cur_data_col=cl->max_col-1;
    }
    if (l->cur_data_col<cl->min_col)
      l->cur_data_col=cl->min_col;
  } else
    l->cur_data_col=cl->min_col;
  if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
    if (cl->min_col<l->cur_data_col<cl->max_col-1) {
      nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
      nl->type=LTFT_TEXT | old_color & -0x100;
      nl->flags=old_flags;
      dst=cl->tag+l->cur_data_col;
      nl->tag=StrNew(dst,l->mem_task);
      nl->max_col=StrLen(dst);
      *dst=0;
      cl->max_col=l->cur_data_col;
      nl->y=cl->y;
      nl->page_line_num=cl->page_line_num;
      nl->x=cl->x+l->cur_data_col;
      InsQue(nl,cl);
      cl=nl;
      l->cur_data_col=cl->min_col;
    }
  }
  l->cur_entry=cl;
  LtfFormFwd(l);
  if (!(l->flags & LTFF_FORM))
    while (cl!=l && cl!=l->cur_entry) {
      if (sc>=0) {
	if (sc&SCF_SHIFT)
	  cl->type|=LTFLT_SELECTED;
	else
	  cl->type&=~LTFLT_SELECTED;
      }
      cl=cl->next;
    }
  l->x=l->cur_entry->x+l->cur_data_col;
  if (unlock)
    LtfUnlock(l);
}

U0 EdDelChar(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  LtfEntry *cl=l->cur_entry;

  if (cl==l) {
    if (unlock)
      LtfUnlock(l);
    return;
  } else
    l->recalc_start=cl->last;
  if (cl->max_col!=0 && (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X)) || cl->type_u8==LTFT_DATA)) {
    if (cl->type_u8==LTFT_DATA) {
      if (cl->flags & LTFLF_HAS_TERMINATOR && l->cur_data_col==cl->max_col-1) {
	if (unlock)
	  LtfUnlock(l);
	return;
      }
    }
    if (l->cur_data_col<cl->max_col)
      StrCpy(cl->tag+l->cur_data_col,cl->tag+l->cur_data_col+1);
    if (l->cur_data_col>=cl->max_col-1) {
      l->cur_entry=cl->next;
      l->cur_data_col=l->cur_entry->min_col;
    }
    LtfRemSoftNewLines(l,l->cur_entry);
    if (unlock)
      LtfUnlock(l);
    return;
  }
  l->cur_entry=cl->next;
  l->cur_data_col=l->cur_entry->min_col;
  if (!(cl->flags&LTFLF_FILTER_SKIP))
    LtfEntryDel(l,cl);
  LtfRemSoftNewLines(l,l->cur_entry);
  if (unlock)
    LtfUnlock(l);
}

U0 ChkDollarBufSize(Ltf *l)
{
  U8 *b;
  if (l->dollar_buf_ptr>=l->dollar_buf_size-2) {
    l->dollar_buf_size<<=1;
    b=MAlloc(l->dollar_buf_size,l->mem_task);
    MemCpy(b,l->dollar_buf,l->dollar_buf_ptr);
    Free(l->dollar_buf);
    l->dollar_buf=b;
  }
}


U0 EdInsertChar(U64 ch,U64 sc,Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  U8 *b,*src,*dst;
  LtfEntry *cl=l->cur_entry,*nl;
  I64 i,j,m,y=cl->y;

  l->recalc_start=cl->last;
  while (l->recalc_start!=l && l->recalc_start->flags&(LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))
    l->recalc_start=l->recalc_start->last;

  if (l->flags & LTFF_IN_DOLLAR) {
    if (!Bt(getkey_chars_bitmap,ch))
      goto ic_done;
    ChkDollarBufSize(l);
    l->dollar_buf[l->dollar_buf_ptr++]=ch;
    if (ch=='$') {
      if (l->dollar_buf_ptr==2) {
	l->flags&=~LTFF_IN_DOLLAR;
	l->dollar_buf_ptr=0;
	goto ic_cont;
      } else {
	l->dollar_buf[l->dollar_buf_ptr]=0;
	LtfPutS(l,l->dollar_buf);
	l->flags&=~LTFF_IN_DOLLAR;
	l->dollar_buf_ptr=0;
	goto ic_done;
      }
    } else
      goto ic_done;
  }
  if (ch=='$') {
    if (!(l->flags & (LTFF_PLAIN_TEXT | LTFF_PLAIN_TEXT_WITH_TABS))) {
      l->flags|=LTFF_IN_DOLLAR;
      l->dollar_buf_ptr=0;
      l->dollar_buf[l->dollar_buf_ptr++]=ch;
      goto ic_done;
    }
  }
  if (ch==CH_CARRIAGE_RETURN) goto ic_done;

ic_cont:
  if ((ch==CH_SPACE || ch==CH_NEW_LINE) &&
      !(sc & (SCF_CTRL|SCF_SHIFT)) &&
       cl->flags & (LTFLF_LINK|LTFLF_TREE|LTFLF_CHECK_COLLAPSABLE|
       LTFLF_LEFT_MACRO|LTFLF_LEFT_EXP|LTFLF_LEFT_CB|LTFLF_LEFT_AUTO |
       LTFLF_RIGHT_MACRO|LTFLF_RIGHT_EXP|LTFLF_RIGHT_CB|LTFLF_RIGHT_AUTO)) {
    l->cmd_U8=ch;
    LtfEntryRun(l,cl,FALSE);
    LtfLock(l);
    goto ic_done;
  }
  if (cl->type_u8==LTFT_HEX_ED) {
    if (cl->flags & LTFLF_DATA_IS_PTR)
      b=cl->data;
    else
      b=&cl->data;
    i=l->cur_data_col;
    j=i%(cl->hex_ed_width*3);
    m=i/(cl->hex_ed_width*3)*cl->hex_ed_width;
    if (j>=cl->hex_ed_width<<1)
      b[j-cl->hex_ed_width<<1+m]=ch;
    else {
      ch=ToUpper(ch)-'0';
      if (ch>9) {
	ch+='0'-'A'+10;
	if (!(10<=ch<=15))
	  goto ic_done;
      }
      m=j>>1+m;
      if (j & 1)
	b[m]=b[m] & 0xF0| ch;
      else
	b[m]=b[m] & 0xF | ch<<4;
    }
    l->cur_data_col++;
    goto ic_done;
  }
  if (l->flags & LTFF_OVERSTRIKE) {
    if (Bt(displayable_chars_bitmap,ch)) {
ic_overstrike_char:
      if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
	if (l->cur_data_col<cl->max_col) {
	  if (cl->tag[l->cur_data_col]) {
	    cl->tag[l->cur_data_col++]=ch;
	    goto ic_done;
	  }
	} else {
	  cl=cl->next;
	  l->cur_entry=cl;
	  l->cur_data_col=cl->min_col;
	  goto ic_overstrike_char;
	}
      } else if (cl->type_u8==LTFT_DATA) {
	if (cl->flags & LTFLF_HAS_TERMINATOR) {
	  if (cl->tag[l->cur_data_col] &&
	     l->cur_data_col<cl->min_col+cl->len) {
	    cl->tag[l->cur_data_col++]=ch;
	    if ( ! cl->tag[l->cur_data_col]) {
	      cl->tag[l->cur_data_col]='_';
	      cl->tag[l->cur_data_col+1]=0;
	    }
	  }
	} else {
	  if (cl->tag[l->cur_data_col])
	     cl->tag[l->cur_data_col++]=ch;
	}
	goto ic_done;
      }
      b=MAlloc(2,l->mem_task);
      *b=ch;
      b[1]=0;
      nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
      nl->type=LTFT_TEXT | l->settings_root.dft_text_attr << 8;
      nl->tag=b;
      nl->max_col=1;
      nl->x=cl->x;
      nl->y=cl->y;
      nl->page_line_num=cl->page_line_num;
      InsQue(nl,cl->last);
    } else if (ch==CH_NEW_LINE) {
      while (l->cur_entry->next!=l && l->cur_entry->y==y)
	l->cur_entry=l->cur_entry->next;
      l->cur_data_col=l->cur_entry->min_col;
    } else if (ch==CH_TAB) {
      if (l->flags&LTFF_FORM)
	goto ic_form_tab;
    }
    goto ic_done;
  }
  if (ch==CH_NEW_LINE) {
    nl=CAlloc(sizeof(LtfEntryBase),l->mem_task);
    if (sc&SCF_CTRL && !(sc&SCF_SHIFT))
      nl->type=LTFT_PAGE_BREAK | l->settings_root.dft_text_attr << 8;
    else
      nl->type=LTFT_NEW_LINE | l->settings_root.dft_text_attr << 8;
    LtfInsert(l,nl);
  } else if (ch==CH_TAB) {
    if (l->flags&LTFF_FORM &&
       Bt(ltf_form_types,l->cur_entry->type_u8) &&
       !(l->cur_entry->flags&LTFLF_SKIP_IN_FORM)) {
ic_form_tab:
      l->cur_entry=l->cur_entry->next;
      l->cur_data_col=l->cur_entry->min_col;
      LtfFormFwd(l);
      goto ic_done;
    } else {
      nl=CAlloc(sizeof(LtfEntryBase),l->mem_task);
      nl->type=LTFT_TAB | l->settings_root.dft_text_attr << 8;
      LtfInsert(l,nl);
    }
  } else {
    if (Bt(displayable_chars_bitmap,ch)) {
      if (cl->type_u8==LTFT_DATA) {
	i=cl->len+cl->min_col;
	if (cl->flags & LTFLF_HAS_TERMINATOR)
	  i++;
	if (i>cl->max_col) {
	  b=cl->tag;
	  cl->max_col++;
	  for (i=cl->max_col;i>l->cur_data_col;i--)
	    b[i]=b[i-1];
	  b[l->cur_data_col++]=ch;
	}
      } else if (cl->type_u8==LTFT_TEXT&& !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
	dst=b=MAlloc(cl->max_col+2,l->mem_task);
	src=cl->tag;
	i=l->cur_data_col;
	while (i-->0)
	  *dst++=*src++;
	*dst++=ch;
	while (*dst++=*src++);
	Free(cl->tag);
	cl->tag=b;
	cl->max_col++;
	l->cur_data_col++;
      } else {
	b=MAlloc(2,l->mem_task);
	*b=ch;
	b[1]=0;
	nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	nl->type=LTFT_TEXT | l->settings_root.dft_text_attr << 8;
	nl->tag=b;
	nl->max_col=1;
	nl->x=cl->x+1;
	nl->y=cl->y;
	nl->page_line_num=cl->page_line_num;
	InsQue(nl,cl->last);
      }
    }
  }
ic_done:
  LtfRemSoftNewLines(l,l->cur_entry);
  if (l->cur_entry->flags & LTFLF_UPDATE_DATA) {
    if (l->cur_entry->type_u8==LTFT_DATA || l->cur_entry->type_u8==LTFT_CHECK_BOX)
    LtfDataGet(l->cur_entry);
  }
  if (unlock)
    LtfUnlock(l);
}

U0 EdDelLine(Ltf *l)
{
  LtfEntry *cl=l->cur_entry,*cl2;
  I64 y;
  y=l->y;
  while (cl!=l && cl->y==y)
    cl=cl->next;
  l->cur_entry=cl;
  l->cur_data_col=cl->min_col;
  cl=cl->last;
  while (cl!=l && cl->y==y) {
    cl2=cl->last;
    if (!(cl->flags&LTFLF_FILTER_SKIP))
      LtfEntryDel(l,cl);
    cl=cl2;
  }
  if (cl==l)
    l->recalc_start=cl;
  else
    l->recalc_start=cl->last;
}

public U0 EdFindNext(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  U8 *ptr,*ptr2,*ptr3;
  LtfEntry *cl=l->cur_entry,*ll=cl;
  U64 ss_flags;
  if (l->find_replace->match_case)
    ss_flags=0;
  else
    ss_flags=SRF_IGNORE_CASE;
  if (l->find_replace->whole_labels)
    ss_flags|=SRF_WHOLE_LABELS;
  do {
    if (ll!=l) {
      if (Bt(ltf_tag_types,ll->type_u8)&&!(ll->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_DEFINE|LTFLF_SCROLLING_X|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
	if (ll->type & LTFLT_SELECTED || !l->find_replace->scan_selected_text) {
	  if (l->find_replace->scan_fwd) {
	    if (ll==cl) {
	      ptr=cl->tag+l->cur_data_col+1;
	      if (ptr-cl->tag>=cl->max_col)
		goto fn_skip;
	      if (ptr-cl->tag<cl->min_col)
		ptr=cl->tag+cl->min_col;
	    } else
	      ptr=ll->tag;
	    if (ptr=SearchStr(ptr,l->find_replace->find_text,ss_flags)) {
	      l->cur_entry=ll;
	      l->cur_data_col=ptr-ll->tag;
	      if (l->cur_data_col>=ll->max_col)
		l->cur_data_col=ll->max_col-1;
	      if (l->cur_data_col<ll->min_col)
		l->cur_data_col=ll->min_col;
	      LtfCenter(l);
	      if (unlock)
		LtfUnlock(l);
	      return;
	    }
	  } else {
	    ptr2=NULL;
	    ptr=ll->tag+ll->min_col;
	    if (ll==cl)
	      ptr3=cl->tag+l->cur_data_col;
	    else
	      ptr3=ll->tag+ll->max_col;
	    while (ptr=SearchStr(ptr,l->find_replace->find_text,ss_flags)) {
	      if (ptr>=ptr3)
		break;
	      ptr2=ptr++;
	    }
	    if (ptr2 && ptr2<ptr3) {
	      l->cur_entry=ll;
	      l->cur_data_col=ptr2-ll->tag;
	      if (l->cur_data_col>=ll->max_col)
		l->cur_data_col=ll->max_col-1;
	      if (l->cur_data_col<ll->min_col)
		l->cur_data_col=ll->min_col;
	      LtfCenter(l);
	      if (unlock)
		LtfUnlock(l);
	      return;
	    }
	  }
	}
      }
    }
fn_skip:
    ll= l->find_replace->scan_fwd ? ll->next:ll->last;
  } while (ll!=cl);
  if (unlock)
    LtfUnlock(l);
}

public U0 EdSelectAll(Ltf *l,BoolI8 select)
{
  BoolI8 unlock=LtfLock(l);
  LtfEntry *ll=l->root.next;
  while (ll!=l) {
    if (select)
      ll->type|=LTFLT_SELECTED;
    else
      ll->type&=~LTFLT_SELECTED;
    ll=ll->next;
  }
  if (unlock)
    LtfUnlock(l);
}

#help_index "Ltf/IDE"
public BoolI64 EdFindPaired(Ltf *l,U8 plus,U8 minus,BoolI8 fwd,BoolI8 abort_on_dbl_colon=FALSE)
{
  BoolI8 unlock=LtfLock(l),result=FALSE;
  U8 ch,*ptr,original_col=l->cur_data_col;
  I64 levels=0,colons=0;
  LtfEntry *cl=l->cur_entry,*ll=cl,*original_cl=cl;
  if (abort_on_dbl_colon && EdCurU8(l)==':')
    colons=1;
  else
    colons=0;
  do {
    if (ll!=l) {
      if (Bt(ltf_tag_types,ll->type_u8)&&!(ll->flags&(LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
	if (fwd) {
	  if (ll==cl)
	    ptr=ll->tag+l->cur_data_col+1;
	  else
	    ptr=ll->tag;
	  if (ptr-ll->tag<ll->min_col)
	    ptr=ll->tag+ll->min_col;
	  if (ptr-ll->tag>=ll->max_col)
	    goto pa_skip;
	  while (ch=*ptr++)
	    if (abort_on_dbl_colon && ch==':') {
	      if (++colons==2) {
		l->cur_entry=ll;
		l->cur_data_col=ptr-ll->tag-1;
		EdCursorLeft(l);
		result=FALSE;
		goto pa_done;
	      }
	    } else {
	      colons=0;
	      if (ch==plus)
		levels++;
	      else if (ch==minus) {
		if (!levels--) {
		  l->cur_entry=ll;
		  l->cur_data_col=ptr-ll->tag-1;
		  result=l->cur_entry!=original_cl || l->cur_data_col!=original_col;
		  goto pa_done;
		}
	      }
	    }
	} else {
	  if (ll==cl) {
	    ptr=ll->tag+l->cur_data_col-1;
	    if (ptr-ll->tag>=ll->max_col)
	      ptr=ll->tag+ll->max_col-1;
	  } else
	    ptr=ll->tag+ll->max_col-1;
	  if (ptr-ll->tag<ll->min_col)
	    goto pa_skip;
	  while (ptr>=ll->tag+ll->min_col) {
	    ch=*ptr--;
	    if (abort_on_dbl_colon && ch==':') {
	      if (++colons==2) {
		l->cur_entry=ll;
		l->cur_data_col=ptr-ll->tag+1;
		result=FALSE;
		goto pa_done;
	      }
	    } else {
	      colons=0;
	      if (ch==plus)
		levels++;
	      else if (ch==minus) {
		if (!levels--) {
		  l->cur_entry=ll;
		  l->cur_data_col=ptr-ll->tag+1;
		  result=l->cur_entry!=original_cl || l->cur_data_col!=original_col;
		  goto pa_done;
		}
	      }
	    }
	  }
	}
      }
    }
pa_skip:
    if (fwd)
      ll=ll->next;
    else
      ll=ll->last;
  } while (ll!=cl);
pa_done:
  LtfRecalc(l);
  LtfCenter(l);
  if (unlock)
    LtfUnlock(l);
  return result;
}

public BoolI64 EdGoToFun(Ltf *l,BoolI8 fwd,BoolI8 abort_on_dbl_colon)
{
  BoolI8 unlock=LtfLock(l),result=FALSE;
  I64 ch,levels,colons;
  if (fwd) {
    levels=0;
    colons=0;
    while (l->cur_entry!=l) {
      ch=EdCurU8(l);
      if (abort_on_dbl_colon && ch==':') {
	if (++colons==2) {
	  EdCursorLeft(l);
	  break;
       }
      } else {
	colons=0;
	if (ch=='{')
	  levels++;
	else if (ch=='}' && !levels--)
	  break;
      }
      EdCursorRight(l);
    }
    LtfRecalc(l);
    if (l->cur_entry!=l)
      result=TRUE;
  } else {
    while (EdFindPaired(l,'}','{',FALSE,abort_on_dbl_colon));
    if (l->cur_entry!=l) {
      ch=EdCurU8(l);
      if (abort_on_dbl_colon && ch==':')
	result=TRUE;
      else {
	if (ch=='{')
	  result=TRUE;
      }
    }
  }
  if (unlock)
    LtfUnlock(l);
  return result;
}

public U0 EdSelectFun(Ltf *l,BoolI8 abort_on_dbl_colon=FALSE)
{
  BoolI8 unlock=LtfLock(l);
  U8 ch,*ptr;
  I64 levels=0,colons=0;
  LtfEntry *ll;
  EdSelectAll(l,FALSE);
  EdGoToFun(l,FALSE,abort_on_dbl_colon);
  if (EdCurU8(l)=='{')
    levels--;
  else if (abort_on_dbl_colon && EdCurU8(l)==':') {
    EdCursorRight(l);
    if (EdCurU8(l)==':')
      EdCursorRight(l);
  }
  ll=l->cur_entry;
  while (ll!=l) {
    ll->type|=LTFLT_SELECTED;
    if (ll->type_u8==LTFT_TEXT) {
      ptr=ll->tag;
      if (ll==l->cur_entry)
	ptr+=l->cur_data_col;
      while (ch=*ptr++)
	if (abort_on_dbl_colon && ch==':') {
	  if (++colons==2)
	    goto sf_done;
	} else {
	  colons=0;
	  if (ch=='{')
	    levels++;
	  else if (ch=='}' && !levels--)
	    goto sf_done;
	}
    }
    ll=ll->next;
  }
sf_done:
  LtfRecalc(l);
  if (unlock)
    LtfUnlock(l);
}

#help_index "Ltf/Edit"
U0 EdUndoFilter(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  LtfEntry *cl=l->root.next;
  while (cl!=l) {
    cl->flags&=~LTFLF_FILTER_SKIP;
    cl=cl->next;
  }
  l->find_replace->filter_lines=0;
  if (unlock)
    LtfUnlock(l);
}

I64 EdFindReplace(Ltf *l)
{
  BoolI8 first=TRUE,found,unlock;
  I64 cmd,i,j,plen,rlen,dlen,result=-1;
  U8 *src,*dst,*dst2;
  LtfEntry *cl=l->cur_entry,*ll;
  U64 ss_flags;
  if (l->find_replace->pmt)
    cmd=RSAC_REPLACE;
  else
    cmd=RSAC_ALL;
  if (!l->find_replace->pmt || LtfFormDo(l->find_replace,"EdFindTextStruct")) {
    result=0;
    unlock=LtfLock(l);
    if (l->find_replace->match_case || l->find_replace->local_var)
      ss_flags=0;
    else
      ss_flags=SRF_IGNORE_CASE;
    if (l->find_replace->whole_labels || l->find_replace->local_var)
      ss_flags|=SRF_WHOLE_LABELS;

    if (i=l->find_replace->filter_lines) {
      cl=l->root.next;
      while (cl!=l) {
	if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))&&
	  SearchStr(cl->tag,l->find_replace->find_text,ss_flags)) {
	  cl->type|=LTFLT_SELECTED;
	  result++;
	} else
	  cl->type&=~LTFLT_SELECTED;
	cl=cl->next;
      }
 
      cl=l->root.next;
      while (cl!=l) {
	if (!(cl->flags&(LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
	  found=FALSE;

	  ll=cl;
	  while (ll!=l && ll->y>cl->y-i) {
	    if (ll->type&LTFLT_SELECTED) {
	      found=TRUE;
	      break;
	    } else
	      ll=ll->last;
	  }

	  if (!found) {
	    ll=cl;
	    while (ll!=l && ll->y<cl->y+i) {
	      if (ll->type&LTFLT_SELECTED) {
		found=TRUE;
		break;
	      } else
		ll=ll->next;
	    }
	  }

	  if (!found)
	    cl->flags|=LTFLF_FILTER_SKIP;
	}

	cl=cl->next;
      }
      EdSelectAll(l,FALSE);
      goto fr_unlock_done;
    }

    if (l->find_replace->local_var) {
      EdSelectFun(l);
      cl=l->cur_entry;
    }

    if (!l->find_replace->replace && !l->find_replace->local_var) {
      EdFindNext(l);
      goto fr_unlock_done;
    }
    plen=StrLen(l->find_replace->find_text);
    if (!plen)
      goto fr_unlock_done;
    rlen=StrLen(l->find_replace->replace_text);
    ll=cl;
    while (ll!=cl || first) {
      first=FALSE;
      if (ll->type_u8==LTFT_TEXT && !(ll->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))&&
	   (ll->type & LTFLT_SELECTED || !l->find_replace->scan_selected_text&&!l->find_replace->local_var)) {
	src=ll->tag;
	while (src) {
	  src=SearchStr(src,l->find_replace->find_text,ss_flags);
	  if (src) {
	    l->cur_data_col=src-ll->tag;
	    l->cur_entry=ll;
	    if (cmd!=RSAC_ALL)
	      LtfCenter(l);
	    ll=l->cur_entry;
	    src=l->cur_data_col+ll->tag;
	    if (cmd!=RSAC_ALL) {
	      LtfUnlock(l);
	      cmd=PopUpReplaceSkipAllCancel("");
	      LtfLock(l);
	      if (cmd<0)
		goto fr_unlock_done;
	    }
	    if (cmd==RSAC_REPLACE || cmd==RSAC_ALL) {
	      dlen=StrLen(ll->tag);
	      ll->max_col=dlen+rlen-plen;
	      dst=MAlloc(ll->max_col+1,l->mem_task);
	      dst2=dst;
	      j=src-ll->tag;
	      for (i=0;i<j;i++)
		*dst++=ll->tag[i];
	      for (i=0;i<rlen;i++)
		*dst++=l->find_replace->replace_text[i];
	      src=dst;
	      for (i=j+plen;i<=dlen;i++)
		*dst++=ll->tag[i];
	      Free(ll->tag);
	      ll->tag=dst2;
	      l->cur_data_col=src-ll->tag;
	      l->cur_entry=ll;
	      if (cmd!=RSAC_ALL) {
		LtfRemSoftNewLines(l,l->cur_entry);
		LtfRecalc(l);
	      }
	      ll=l->cur_entry;
	      src=l->cur_data_col+ll->tag;
	      result++;
	    } else
	      src++;
	  }
	}
      }
      if (l->find_replace->scan_fwd) {
	ll=ll->next;
	if (ll==l) {
	  if (cl==l) break;
	  ll=ll->next;
	}
      } else {
	ll=ll->last;
	if (ll==l) {
	  if (cl==l) break;
	  ll=ll->last;
	}
      }
    }
fr_unlock_done:
    LtfRemSoftNewLines(l,NULL);
    LtfRecalc(l);
    LtfCenter(l);
    if (unlock)
      LtfUnlock(l);
  }
  return result;
}

public I64 EdReplace(Ltf *l,U8 *find,U8 *replace,
  BoolI8 selected=TRUE,BoolI8 match_case=TRUE,BoolI8 whole_labels=FALSE)
{
  EdFindTextStruct old_find_replace;
  BoolI8 unlock;
  I64 i,result=-1;
  if (!l) return -1;
  unlock=LtfLock(l);
  MemCpy(&old_find_replace,l->find_replace,sizeof(EdFindTextStruct));
  MemSet(l->find_replace,0,sizeof(EdFindTextStruct));
  i=StrLen(find);
  if (i<sizeof(EdFindTextStruct.find_text)) {
    MemCpy(l->find_replace->find_text,find,i+1);
    i=StrLen(replace);
    if (i<sizeof(EdFindTextStruct.replace_text)) {
      MemCpy(l->find_replace->replace_text,replace,i+1);
      l->find_replace->replace=TRUE;
      l->find_replace->scan_selected_text=selected;
      l->find_replace->match_case=match_case;
      l->find_replace->whole_labels=whole_labels;
      l->find_replace->pmt=FALSE;
      result=EdFindReplace(l);
    }
  }
  MemCpy(l->find_replace,&old_find_replace,sizeof(EdFindTextStruct));
  if (unlock)
    LtfUnlock(l);
  return result;
}

public U0 LtfLineNumGoTo(Ltf *l,I64 linenum) //one based
{
  l->x=0;
  l->y=linenum-1;
  LtfRecalc(l,FALSE,LTF_RECALC_FIND_CURSOR);
  LtfCenter(l);
}

class EdGoToLineStruct
{
  I64 line fmtstr "$DA,256,\"Go to Line:%d\"$";
};

public U0 EdGoToLine(Ltf *l)
{
  EdGoToLineStruct gtl;
  gtl.line=1;
  if (LtfFormDo(&gtl,"EdGoToLineStruct"))
    LtfLineNumGoTo(l,gtl.line);
}

U0 LtfEntryToggle(Ltf *l)
{
  BoolI8 unlock=LtfLock(l);
  LtfEntry *cl=l->cur_entry,*cl1,*cl2;
  U8 ch,*st,*st2;
  I64 i,j,k;
  if (cl!=l && !(l->flags&(LTFF_PLAIN_TEXT|LTFF_PLAIN_TEXT_WITH_TABS))) {
    if (cl->type_u8==LTFT_TEXT && !(cl->flags & ~(LTF_BL_IV_UL|LTFLF_WORD_WRAP|LTFLF_HIGHLIGHT|LTFLF_LOCK_TO_PAGE|LTFLF_LOCK_TO_CENTER|LTFLF_SKIP|LTFLF_HARD_SKIP|LTFLF_FILTER_SKIP))) {
      cl2=cl->last;
      for (k=0;k<20;k++) {
	if (cl2!=l) {
	  cl1=cl2->last;
	  if (cl2->type_u8==LTFT_TEXT && cl->flags==cl2->flags && cl->type==cl2->type) {
	    i=StrLen(cl2->tag);
	    j=StrLen(cl->tag);
	    st=MAlloc(i+j+1,l->mem_task);
	    MemCpy(st,cl2->tag,i);
	    MemCpy(st+i,cl->tag,j+1);
	    Free(cl->tag);
	    cl->tag=st;
	    cl->max_col=i+j;
	    l->cur_data_col+=i;
	    LtfEntryDel(l,cl2);
	  } else if (cl2->type_u8==LTFT_SOFT_NEW_LINE)
	    LtfEntryDel(l,cl2);
	  else
	    break;
	  cl2=cl1;
	} else
	  break;
      }
      cl2=cl->next;
      for (k=0;k<20;k++) {
	if (cl2!=l) {
	  cl1=cl2->next;
	  if (cl2->type_u8==LTFT_TEXT && cl->flags==cl2->flags && cl->type==cl2->type) {
	    i=StrLen(cl->tag);
	    j=StrLen(cl2->tag);
	    st=MAlloc(i+j+1,l->mem_task);
	    MemCpy(st,cl->tag,i);
	    MemCpy(st+i,cl2->tag,j+1);
	    Free(cl->tag);
	    cl->tag=st;
	    cl->max_col=i+j;
	    LtfEntryDel(l,cl2);
	  } else if (cl2->type_u8==LTFT_SOFT_NEW_LINE)
	    LtfEntryDel(l,cl2);
	  else
	    break;
	  cl2=cl1;
	} else
	  break;
      }
      i=l->cur_data_col;
      while (i>cl->min_col && cl->tag[i]!='$')
	i--;
      j=l->cur_data_col+1;
      while (j<cl->max_col && cl->tag[j]!='$')
	j++;
      if (i<j-1 && cl->min_col<=i<j<cl->max_col && cl->tag[i]=='$' && cl->tag[j]=='$') {
	ch=cl->tag[j+1];
	cl->tag[j+1]=0;
	st=StrNew(cl->tag+i);
	cl->tag[j+1]=ch;
	StrCpy(cl->tag+i,cl->tag+j+1);
	l->cur_data_col=i;
	st2=MSPrintF("%q",st);
	if (cl=LtfPutS(l,st2)) {
	  l->cur_entry=cl;
	  l->cur_data_col=cl->min_col;
	}
	Free(st);
	Free(st2);
      }
    } else {
      st=Ltf2PlainText(l,cl);
      LtfEntryDel(l,cl);
      LtfPrintF(l,"$$%Q$$",st);
    }
    LtfRecalc(l);
  }
  if (unlock)
    LtfUnlock(l);
}

U0 LtfFlagsToggle(Ltf *l,I64 tog_flags)
{
  U64 size,start_text_attr=l->start_text_attr;
  BoolI8 unlock=LtfLock(l);
  U8 *st=LtfSave(l,&size);
  U64 flags=l->flags^tog_flags;
  Ltf *menu=l->menu_ltf;
  LtfUndo *u_next,*u_last;

  u_next=l->undo_root.next;
  u_last=l->undo_root.last;
  l->undo_root.next=l->undo_root.last=&l->undo_root;

  LtfRst(l,TRUE);
  l->start_text_attr=start_text_attr;
  l->menu_ltf=menu;
  l->undo_root.next=u_next;
  l->undo_root.last=u_last;
  LtfUndoCntSet(l);
  l->flags=flags & ~(LTF_BL_IV_UL|LTFF_WORD_WRAP);
  LtfLoad(l,st,size);
  LtfCenter(l);
  if (unlock)
    LtfUnlock(l);
  Free(st);
}

#help_index "Ltf/Output"
public U0 LtfPutKey(Ltf *l,U64 ch,U64 sc)
{
  I64 i,x,y,ext_ascii_cnt=0;
  U64 sc2;
  LtfEntry *cl;
  U8 *st;
  BoolI8 unlock;

  if (!l || l->ltf_signature!=LTF_SIGNATURE_VAL)
    return;
  if (l->user_put_key)
    if ((*l->user_put_key)(l,l->user_put_data,ch,sc))
      return;
  unlock=LtfLock(l);
  if (!Bt(ltf_clean_scancodes,sc.u8[0]))
    l->flags|=LTFF_UNDO_DIRTY;
  LtfCaptureUndo(l);
  if (Bt(getkey_chars_bitmap,ch) &&
      !(sc&(SCF_CTRL|SCF_ALT))) {
    if (sc&SCF_KEY_DESC) {
      st=A2KeyName(ch,FALSE);
      SetKeyDesc("Char/'%s'",st);
      Free(st);
    } else
      EdInsertChar(ch,sc,l);
  } else {
    cl=l->cur_entry;
    x=l->x; y=l->y;
    sc2=sc.u8[0];
    switch (ch) {
      case CH_CTRLA:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Save As");
	else
	  LtfWrite(l,TRUE);
	break;
      case CH_CTRLB:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Win/Toggle Border");
	else
	  WinBorder(Bt(&l->win_task->display_flags,DISPLAYf_NO_BORDER),l->win_task);
	break;
      case CH_CTRLC:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Copy to Clipboard");
	else
	  EdCopyToClipboard(l);
	break;
      case CH_CTRLD:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Cmd/Jump to File Manager");
	else {
	  LtfUnlock(l);
	  FileMgr;
	  LtfLock(l);
	}
	break;
      case CH_CTRLF:
	if (sc&SCF_SHIFT) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Cmd/Grep Wizard");
	  else
	    GrepWiz;
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Find & Replace");
	  else
	    EdFindReplace(l);
	}
	break;
      case CH_CTRLG:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/GoTo Line Num");
	else
	  EdGoToLine(l);
	break;
      case CH_BACKSPACE: //<CTRL-H>
	if (sc&SCF_ALT && !(sc&SCF_CTRL)) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Undo");
	  else
	    LtfUndoRestore(l);
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Char/Back Space");
	  else {
	    LtfCaptureUndo(l);
	    cl=l->cur_entry;
	    if (l->cur_data_col<=cl->min_col) {
	      cl=l->cur_entry=cl->last;
	      if (cl!=l && cl->type_u8==LTFT_SOFT_NEW_LINE)
		cl=l->cur_entry=cl->last;
	      if (cl==l || cl->type_u8==LTFT_USER_FOREGROUND) {
		cl=l->cur_entry=cl->next;
		l->cur_data_col=cl->min_col;
	      } else {
		l->cur_data_col=cl->max_col;
		if (l->cur_data_col>cl->min_col)
		  l->cur_data_col--;
		EdDelChar(l);
	      }
	    } else {
	      l->cur_data_col--;
	      EdDelChar(l);
	    }
	  }
	}
	break;
      case CH_CTRLI:
	if (sc&SCF_SHIFT) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Dollar/Indent Out");
	  else
	    LtfPutS(l,"$ID,-2$");
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Dollar/Indent In");
	  else
	    LtfPutS(l,"$ID,2$");
	}
	break;
      case CH_NEW_LINE:
	if (sc&SCF_KEY_DESC) {
	  if (sc&SCF_CTRL && !(sc&SCF_SHIFT))
	    SetKeyDesc("Char/Page Break");
	  else
	    SetKeyDesc("Char/Return");
	} else
	  EdInsertChar(ch,sc,l);
	break;
      case CH_CTRLK:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Toggle Blinking Text");
	else if (sc&SCF_SHIFT)
	  LtfPutS(l,"$BK,0$");
	else
	  LtfPutS(l,"$BK,1$");
	break;
      case CH_CTRLL:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Wizard");
	else {
	  LtfUnlock(l);
	  EdInsertDollarCmd;
	  LtfLock(l);
	}
	break;
      case CH_CARRIAGE_RETURN:
	break;
      case CH_CTRLO:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Managed Code");
	else
	  EdCodeTools(l);
	break;
      case CH_CTRLP:
	if (l->flags & (LTFF_SUPERSCRIPT_MODE | LTFF_SUBSCRIPT_MODE)) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Dollar/Toggle Super or Sub script");
	  else {
	    LtfPutS(l,"$SY,0$");
	    l->flags&=~(LTFF_SUPERSCRIPT_MODE | LTFF_SUBSCRIPT_MODE);
	  }
	} else if (sc&SCF_SHIFT) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Dollar/Toggle Subscript");
	  else {
	    LtfPutS(l,"$SY,3$");
	    l->flags|=LTFF_SUBSCRIPT_MODE;
	  }
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Dollar/Toggle Superscript");
	  else {
	    LtfPutS(l,"$SY,-3$");
	    l->flags|=LTFF_SUPERSCRIPT_MODE;
	  }
	}
	break;
      case CH_CTRLR:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Edit or Create Graphic");
	else {
	  if (!(l->flags&(LTFF_PLAIN_TEXT|LTFF_PLAIN_TEXT_WITH_TABS))) {
	    LtfUnlock(l);
	    if (cl->type_u8==LTFT_PICTURE)
	      EdExistingGraphic(l,
		  (cl->x-l->line_start_col+cl->max_col)*FONT_WIDTH,
		  (cl->y-l->cur_top_line_num)*FONT_HEIGHT);
	    else
	      EdInsertGraphic(l,
		  (cl->x+l->cur_data_col-l->line_start_col)*FONT_WIDTH,
		  (cl->y-l->cur_top_line_num)*FONT_HEIGHT);
	    LtfLock(l);
	  }
	}
	break;
      case CH_CTRLS:
	if (sc&SCF_SHIFT) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Find Misspelled");
	  else
	    WsFindMisspelled(l);
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Save");
	  else
	    LtfWrite(l);
	}
	break;
      case CH_CTRLT:
	if (sc&SCF_SHIFT) {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Single Entry Toggle Plain Text");
	  else
	    LtfEntryToggle(l);
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Toggle Plain Text Display");
	  else
	    if (!(l->flags&LTFF_FORM) || l->flags&(LTFF_PLAIN_TEXT|LTFF_PLAIN_TEXT_WITH_TABS))
	      LtfFlagsToggle(l,LTFF_PLAIN_TEXT);
	}
	break;
      case CH_CTRLU:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Toggle Underline");
	else if (sc&SCF_SHIFT)
	  LtfPutS(l,"$UL,0$");
	else
	  LtfPutS(l,"$UL,1$");
	break;
      case CH_CTRLV:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Paste Clipboard");
	else
	  EdPasteClipboard(l);
	break;
      case CH_CTRLW:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Toggle Word Wrap");
	else if (sc&SCF_SHIFT)
	  LtfPutS(l,"$WW,0$");
	else
	  LtfPutS(l,"$WW,1$");
	break;
      case CH_CTRLX:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Cut To Clipboard");
	else
	  EdCutToClipboard(l);
	break;
      case CH_CTRLY:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Edit/Delete Line");
	else
	  EdDelLine(l);
	break;
      case CH_CTRLZ:
	if (sc&SCF_KEY_DESC)
	  SetKeyDesc("Dollar/Toggle Inverted Text");
	else if (sc&SCF_SHIFT)
	  LtfPutS(l,"$IV,0$");
	else
	  LtfPutS(l,"$IV,1$");
	break;
      case '0'...'9':
	if (!(sc&SCF_ALT)) {
	  if (sc&SCF_CTRL) {
	    if (sc&SCF_KEY_DESC) {
	      if (sc&SCF_SHIFT)
		SetKeyDesc("Edit/Jump to Word Definition");
	      else
		SetKeyDesc("Edit/Autocomplete Word");
	    } else {
	      if (WordStat) {
		LtfUnlock(l);
		if (sc&SCF_SHIFT)
		  WsdDef(ch-'0',Fs);
		else
		  WsdFillin(ch-'0');
		LtfLock(l);
	      }
	    }
	  }
	} else {
	  if (sc&SCF_KEY_DESC)
	    SetKeyDesc("Edit/Enter extended-ASCII");
	  else {
	    if (++l->ext_ascii_cnt==1)
	      l->ext_ascii_val=ch-'0';
	    else
	      l->ext_ascii_val=l->ext_ascii_val*10+ch-'0';
	    if (l->ext_ascii_cnt==3)
	      EdInsertChar(l->ext_ascii_val.u8[0],sc,l);
	    else
	      ext_ascii_cnt=l->ext_ascii_cnt;
	  }
	}
	break;
      case '[':
	if (!(sc&SCF_ALT) && sc&SCF_CTRL) {
	  if (sc&SCF_SHIFT) {
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/GoTo matching brace");
	    else
	      EdFindPaired(l,'}','{',FALSE);
	  } else {
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/GoTo matching bracket");
	    else
	      EdFindPaired(l,']','[',FALSE);
	  }
	}
	break;
      case ']':
	if (!(sc&SCF_ALT) && sc&SCF_CTRL) {
	  if (sc&SCF_SHIFT) {
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/GoTo matching brace");
	    else
	      EdFindPaired(l,'{','}',TRUE);
	  } else {
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/GoTo matching bracket");
	    else
	      EdFindPaired(l,'[',']',TRUE);
	  }
	}
	break;
      default:
	switch (sc2) {
	  case SC_F1...SC_F10:
	    if (!(sc&SCF_ALT)) {
	      if (sc&SCF_CTRL) {
		if (sc&SCF_KEY_DESC) {
		  if (sc&SCF_SHIFT)
		    SetKeyDesc("Edit/Jump to Src of Sym");
		  else
		    SetKeyDesc("Edit/Autocomplete Sym");
		} else {
		  if (WordStat) {
		    LtfUnlock(l);
		    if (sc&SCF_SHIFT)
		      WsMan(sc2-SC_F1+1,Fs);
		    else
		      WsFillIn(sc2-SC_F1+1);
		    LtfLock(l);
		  }
		}
	      } else {
		switch (sc2) {
		  case SC_F1:
		    if (sc&SCF_KEY_DESC) {
		      if (sc&SCF_SHIFT)
			SetKeyDesc("About");
		      else
			SetKeyDesc("Help");
		    } else {
		      if (sc&SCF_SHIFT)
			Ed("::/LT/Doc/Splash.TXZ");
		      else
			Ed("::/LT/Doc/HelpIndex.TXZ");
		    }
		    break;
		  case SC_F2:
		    if (sc&SCF_KEY_DESC) {
		      if (sc&SCF_SHIFT)
			SetKeyDesc("Edit/Play Macro");
		      else
			SetKeyDesc("Edit/Macro");
		    } else {
		      LtfUnlock(l);
		      if (sc&SCF_SHIFT) {
			if (TaskValidate(sys_macro_util_task))
			  SendMsg(sys_macro_util_task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
			SysMacroStripKey(&sys_macro_root,ch,sc);
			PlaySysMacro;
		      } else
			EdMacroUtil;
		      LtfLock(l);
		    }
		    break;
		  case SC_F3:
		    if (sc&SCF_KEY_DESC)
		      SetKeyDesc("Edit/Find Next");
		    else {
		      l->find_replace->scan_fwd=!(sc&SCF_SHIFT);
		      EdFindNext(l);
		    }
		    break;
		  case SC_F4:
		    if (sc&SCF_KEY_DESC) {
		      if (sc&SCF_SHIFT)
			SetKeyDesc("Misc/Insert Directory Name");
		      else
			SetKeyDesc("Misc/Insert File Name");
		    } else {
		      LtfUnlock(l);
		      if (sc&SCF_SHIFT)
			st=PopUpPickDir;
		      else
			st=PopUpPickFile;
		      LtfLock(l);
		      if (st) {
			LtfPutSPartial(l,st);
			Free(st);
		      }
		    }
		    break;
		  case SC_F5:
		    if (sc&SCF_KEY_DESC) {
		      if (sc&SCF_SHIFT)
			SetKeyDesc("Cmd/Adam Include");
		      else
			SetKeyDesc("Cmd/Run (Execute);Dbg/Go");
		    } else {
		      if (IsDbgTask) {
			LtfUnlock(l);
			if (!(sc&SCF_SHIFT))
			  IDEGo;
			LtfLock(l);
		      } else {
			if (cl->flags&LTFLF_LINK) {
			  if (cl->flags & LTFLF_AUX_STR)
			    st=cl->aux_str;
			  else
			    st=cl->tag;
			  st= st[2]==':' ? st+3:st;
			} else {
			  LtfWrite(l);
			  st=l->filename.name;
			}
			if (sc&SCF_SHIFT)
			  AdamFile(st);
			else
			  PopUpFile(st);
		      }
		    }
		    break;
		  case SC_F9:
		    if (sc&SCF_KEY_DESC) {
		      if (sc&SCF_SHIFT)
			SetKeyDesc("Dbg/Clear All Break Points");
		      else
			SetKeyDesc("Dbg/Toggle Break Point");
		    } else {
		      if (IsDbgTask) {
			LtfUnlock(l);
			if (sc&SCF_SHIFT)
			  IDEClearAllBpts(l);
			else
			  IDEToggleBpt(l);
			LtfLock(l);
		      }
		    }
		    break;
		  case SC_F10:
		    if (sc&SCF_KEY_DESC) {
		      if (!(sc&SCF_SHIFT))
			SetKeyDesc("Dbg/Step Over");
		    } else {
		      if (IsDbgTask) {
			LtfUnlock(l);
			if (!(sc&SCF_SHIFT))
			  IDEStepOver(l);
			LtfLock(l);
		      }
		    }
		    break;
		}
	      }
	    }
	    break;
	  case SC_F11:
	  case SC_F12:
	    if (!(sc & (SCF_ALT|SCF_CTRL))) {
	      switch (sc2) {
		case SC_F11:
		  if (sc&SCF_KEY_DESC) {
		    if (!(sc&SCF_SHIFT))
		      SetKeyDesc("Dbg/Step Into");
		  } else {
		    if (IsDbgTask) {
		      LtfUnlock(l);
		      if (!(sc&SCF_SHIFT))
			IDEStepInto(l);
		      LtfLock(l);
		    }
		  }
		  break;
	      }
	    }
	    break;
	  case SC_GUI:
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Cmd/Jump to Menu");
	    else if (l->menu_ltf)
	      LtfMenuDo(l->menu_ltf);
	    break;
	  case SC_CURSOR_DOWN:
	  case SC_END:
	    if (sc2==SC_END || sc&SCF_CTRL) {
	      if (sc&SCF_KEY_DESC)
		SetKeyDesc("Edit/GoTo Doc End");
	      else {
		while (cl!=l) {
		  if (sc&SCF_SHIFT)
		    cl->type|=LTFLT_SELECTED;
		  else
		    cl->type&=~LTFLT_SELECTED;
		  cl=l->cur_entry=cl->next;
		}
		l->cur_data_col=cl->min_col;
		l->recalc_start=l;
		LtfFormBackward(l);
	      }
	    } else if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Cursor Down");
	    else
	      EdLineDown(l,sc);
	    break;
	  case SC_CURSOR_UP:
	  case SC_HOME:
	    if (sc2==SC_HOME || sc&SCF_CTRL) {
	      if (sc&SCF_KEY_DESC)
		SetKeyDesc("Edit/GoTo Top of Doc");
	      else {
		while (cl!=l) {
		  if (sc&SCF_SHIFT)
		    cl->type|=LTFLT_SELECTED;
		  else
		    cl->type&=~LTFLT_SELECTED;
		  cl=l->cur_entry=l->cur_entry->last;
		}
		cl=l->cur_entry=l->root.next;
		l->cur_data_col=cl->min_col;
		l->recalc_start=l;
		LtfFormFwd(l);
	      }
	    } else if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Cursor Up");
	    else
	      EdLineUp(l,sc);
	    break;
	  case SC_PAGE_DOWN:
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Page Down");
	    else {
	      i=cl->y+l->win_task->win_height-1;
	      if (cl->type_u8==LTFT_HEX_ED)
		i+=l->cur_data_col/3/cl->hex_ed_width;
	      while (cl!=l &&
		  (cl->type_u8!=LTFT_HEX_ED && cl->y<i ||
		   cl->type_u8==LTFT_HEX_ED && cl->y+l->cur_data_col/3/cl->hex_ed_width<i) &&
		  cl->y!=l->root.last->y) {
		EdLineDown(l,sc);
		//paranoid check for stuck on same node
		if (l->cur_entry==cl && cl->type_u8!=LTFT_HEX_ED)
		  break;
		cl=l->cur_entry;
	      }
	    }
	    break;
	  case SC_PAGE_UP:
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Page Up");
	    else {
	      i=cl->y-(l->win_task->win_height-1);
	      if (cl->type_u8==LTFT_HEX_ED)
		i+=l->cur_data_col/3/cl->hex_ed_width;
	      while (cl->last!=l &&
		  (cl->type_u8!=LTFT_HEX_ED && cl->y>i ||
		   cl->type_u8==LTFT_HEX_ED && cl->y+l->cur_data_col/3/cl->hex_ed_width>i) &&
		  cl->y!=l->root.next->y) {
		EdLineUp(l,sc);
		//paranoid check for stuck on same node
		if (l->cur_entry==cl && cl->type_u8!=LTFT_HEX_ED)
		  break;
		cl=l->cur_entry;
	      }
	    }
	    break;
	  case SC_CURSOR_LEFT:
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Cursor Left");
	    else
	      EdCursorLeft(l,sc);
	    break;
	  case SC_CURSOR_RIGHT:
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Cursor Right");
	    else
	      EdCursorRight(l,sc);
	    break;
	  case SC_DELETE:
	    if (sc&SCF_KEY_DESC) {
	      if (sc&SCF_SHIFT)
		SetKeyDesc("Edit/Cut To Clipboard");
	      else
		SetKeyDesc("Char/Delete");
	    } else {
	      if (sc&SCF_SHIFT)
		EdCutToClipboard(l);
	      else
		EdDelChar(l);
	    }
	    break;
	  case SC_INSERT:
	    if (sc&SCF_KEY_DESC) {
	      if (sc&SCF_SHIFT)
		SetKeyDesc("Edit/Paste Clipboard");
	      else if (sc&SCF_CTRL)
		SetKeyDesc("Edit/Copy to Clipboard");
	      else
		SetKeyDesc("Edit/Toggle Overstrike");
	    } else {
	      if (sc&SCF_SHIFT)
		EdPasteClipboard(l);
	      else if (sc&SCF_CTRL)
		EdCopyToClipboard(l);
	      else
		l->flags^=LTFF_OVERSTRIKE;
	    }
	    break;
	}  //switch (sc2)
    }
  }
  if (!l->recalc_start)
    l->recalc_start=l->cur_entry->last;
  l->ext_ascii_cnt=ext_ascii_cnt;
  if (unlock)
    LtfUnlock(l);
  if (l==Fs->last_ltf && !(l->flags&LTFF_DONT_SWAP_OUT)) {
    i=GetTimeStamp;
    if (i>l->swap_out_time) {
      l->swap_out_time=i+time_stamp_kHz_freq>>3;
      Yield;
    }
  }
}

BoolI64 AKDLtfPutKey(U64 ch,U64 scancode)
{
  Ltf *l;
  l=Fs->next_ltf;
  while (l!=(&Fs->next_ltf)(U8 *)-offset(Ltf.next_ltf) &&
	 l->ltf_in_queue_signature==LTF_IN_QUEUE_SIGNATURE_VAL) {
    if (!(l->flags&LTFF_DONT_SHOW) && l->flags&LTFF_AUX_LTF && !l->best_d) {
      LtfPutKey(l,ch,scancode);
      return FALSE;
    }
    l=l->next_ltf;
  }
  if (l=LtfCur)
    LtfPutKey(l,ch,scancode);
  return FALSE;
}

public U0 LtfPutSPartial(Ltf *l,U8 *st)
{
  U8 ch,*b,*src,*dst,*ptr=st,*ptr2;
  BoolI8 unlock;
  LtfEntry *cl,*nl;
  I64 i,j;
  if (!st)
    return;
  if (l->user_put_s)
    if ((*l->user_put_s)(l,l->user_put_data,st))
      return;
  unlock=LtfLock(l);
  if (l->cur_entry->type_u8==LTFT_DATA)
    while (ch=*ptr++)
      LtfPutKey(l,ch,0);
  else
    while (ch=*ptr) {
      if (!Bt(safe_dollar_chars_bitmap,ch) ||
	  l->flags & (LTFF_OVERSTRIKE | LTFF_IN_DOLLAR)) {
	LtfPutKey(l,ch,0);
	ptr++;
      } else {
	ptr2=ptr++;
	while (TRUE) {
	  ch=*ptr++;
	  if (!Bt(safe_dollar_chars_bitmap,ch))
	    break;
	}
	ptr--;
	*ptr=0;
	cl=l->cur_entry;
	l->recalc_start=cl->last;
	j=ptr-ptr2;
	if (cl->type_u8==LTFT_TEXT && !(cl->flags&(LTFLF_TAG_CB|LTFLF_DEFINE|LTFLF_HTML_LINK|LTFLF_SCROLLING_X))) {
	  dst=b=MAlloc(cl->max_col+j+1,l->mem_task);
	  src=cl->tag;
	  i=l->cur_data_col;
	  l->cur_data_col+=j;
	  cl->max_col+=j;
	  while (i-->0)
	    *dst++=*src++;
	  while (j-->0)
	    *dst++=*ptr2++;
	  while (*dst++=*src++);
	  Free(cl->tag);
	  cl->tag=b;
	} else {
	  nl=CAlloc(sizeof(LtfEntryBase)+sizeof(U0 *),l->mem_task);
	  nl->type=LTFT_TEXT | l->settings_root.dft_text_attr << 8;
	  nl->tag=MAlloc(j+1,l->mem_task);
	  MemCpy(nl->tag,ptr2,j+1);
	  nl->x=cl->x;
	  nl->y=cl->y;
	  nl->max_col=j;
	  nl->page_line_num=cl->page_line_num;
	  InsQue(nl,cl->last);
	  l->cur_entry=nl;
	  l->cur_data_col=StrLen(ptr2);
	}
	*ptr=ch;
	LtfRemSoftNewLines(l,l->cur_entry);
      }
    }
  if (unlock)
    LtfUnlock(l);
  if (l==Fs->last_ltf &&
      !(l->flags&LTFF_DONT_SWAP_OUT)) {
    i=GetTimeStamp;
    if (i>l->swap_out_time) {
      l->swap_out_time=i+time_stamp_kHz_freq>>3;
      Yield;
    }
  }
}

#help_index "Ltf"
Ltf *LtfFileEd(I64 type,U8 *filename,
	U8 *pattern,I64 num,BoolI8 bail)
{ //"bail" is for [C:/LT/Adam/LinkChk.APZ,71] LinkChk()
  U8 *ptr;
  I64 i,j,flags=0;
  LtfEntry *ll;
  Ltf *l,*m;
  BoolI8 old_silent=Bt(&Fs->display_flags,DISPLAYf_SILENT),
	 found=FALSE;
 
  try {
    switch (type) {
      case LK_PLAIN:
	type=LK_FILE;
	flags=LTFF_PLAIN_TEXT;
	break;
      case LK_PLAIN_FIND:
	type=LK_FILE_FIND;
	flags=LTFF_PLAIN_TEXT;
	break;
      case LK_PLAIN_LINE:
	type=LK_FILE_LINE;
	flags=LTFF_PLAIN_TEXT;
	break;
      case LK_BIBLE_FIND:
	flags=LTFF_PLAIN_TEXT;
	break;
    }
 
    flags|=LTFF_ALLOW_UNDO;
 
    if (!*filename)
      l=LtfCur;
    else {
      Silent(ON);
      //if bail, scan parents
      found=FileFind(filename,NULL,FALSE,TRUE,bail);
      l=LtfRead(filename,flags);
      StrCpy(l->desc,"Edit");
      Silent(old_silent);
      l->flags|=LTFF_ATTR_BY_FILENAME;
      m=LtfRead(ED_MENU_NAME);
      StrCpy(m->desc,"EdMenu");
      m->settings_root.dft_text_attr=WHITE<<4+LTBLUE;
      l->menu_ltf=m;
    }
 
    LtfLock(l);
    switch (type) {
      case LK_FILE_LINE:
	l->y=num-1;
	l->x=0;
	LtfRecalc(l,FALSE,LTF_RECALC_FIND_CURSOR);
	break;
      case LK_FILE_ANCHOR:
	if (!(ll=LtfAnchorFind(l,pattern)))
	  found=FALSE;
	break;
      case LK_FILE_FIND:
	found=FALSE;
	i=num;
	ll=l->root.next;
	while (ll!=l) {
	  if (Bt(ltf_tag_types,ll->type_u8)) {
	    if (ptr=StrIStr(ll->tag,pattern)) { //TODO: handle multientry strings
	      j=ptr-ll->tag;
	      if (!--i) {
		l->cur_entry=ll;
		if (j<ll->min_col) j=ll->min_col;
		if (j>ll->max_col) j=ll->max_col;
		l->cur_data_col=j;
		found=TRUE;
		break;
	      }
	    }
	  }
	  ll=ll->next;
	}
	break;
      case LK_BIBLE_FIND:
	l->y=num-1;
	l->x=0;
	LtfRecalc(l,FALSE,LTF_RECALC_FIND_CURSOR);
	if (pattern && *pattern) {
	  found=FALSE;
	  ll=l->cur_entry;
	  while (ll!=l) {
	    if (Bt(ltf_tag_types,ll->type_u8)) {
	      if (ptr=StrIStr(ll->tag,pattern)) { //TODO: handle multientry strings
		j=ptr-ll->tag;
		l->cur_entry=ll;
		if (j<ll->min_col) j=ll->min_col;
		if (j>ll->max_col) j=ll->max_col;
		l->cur_data_col=j;
		found=TRUE;
		break;
	      }
	    }
	    ll=ll->next;
	  }
	}
	break;
    }
    LtfUnlock(l);
    LtfCenter(l);
    if (!bail && LtfDo(l,DOF_DONT_HOME,NULL)) {
      LtfLock(l);
      ll=l->cur_entry;
      if (ll!=l)
	LtfEntryRun(l,ll,TRUE);
      LtfUnlock(l);
      LtfWrite(l);
    }
    if (Fs->last_ltf!=l) {
      if (!bail || !found) {
	LtfDel(l);
	l=NULL;
      }
    } else
      l=NULL;
  } catch {
    Silent(old_silent);
    l=NULL;
  }
  return l;
}
