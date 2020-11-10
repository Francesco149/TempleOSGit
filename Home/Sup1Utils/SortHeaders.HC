class CSHEntry
{
  CDocEntry *next,*last;
};

class CSHSection
{
  CDocEntry *next,*last;
};

I64 SHGetChar(CDoc *doc,CDocEntry **_doc_e,U8 **_st)
{
  I64 ch;
  U8 *st=*_st;
  CDocEntry *doc_e=*_doc_e;
  while (TRUE) {
    if (st) {
      if (ch=*st) {
	*_st=++st;
	return ch;
      } else
	st--;
    }
    if (doc_e==doc) {
      *_st=NULL;
      return 0;
    } else {
      *_doc_e=doc_e=doc_e->next;
      if (doc_e->type_u8==DOCT_TEXT)
	st=doc_e->tag;
    }
  }
}

U8 *SHSkipDataType(CDoc *doc,CDocEntry **_doc_e,U8 *st)
{
  I64 ch;
  CDocEntry *last_doc_e=*_doc_e;
  U8 *last_st=st;
  if (!(ch=SHGetChar(doc,_doc_e,&st)))
    return NULL;
  while (ch && Bt(char_bmp_white_space,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  if (ch=='#')
    return st;
  while (ch && Bt(char_bmp_alpha_numeric,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  while (ch && Bt(char_bmp_white_space,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  while (ch=='*') {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  *_doc_e=last_doc_e; st=last_st;
  return st;
}

U8 *SHSkipLabel(CDoc *doc,CDocEntry **_doc_e,U8 *st)
{
  I64 ch;
  CDocEntry *last_doc_e=*_doc_e;
  U8 *last_st=st;
  if (!(ch=SHGetChar(doc,_doc_e,&st)))
    return NULL;
  while (ch && Bt(char_bmp_white_space,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  while (ch && Bt(char_bmp_alpha_numeric,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  while (ch && Bt(char_bmp_white_space,ch)) {
    last_doc_e=*_doc_e; last_st=st;
    ch=SHGetChar(doc,_doc_e,&st);
  }
  *_doc_e=last_doc_e; st=last_st;
  return st;
}

U8 *SHNewEntry(CDoc *doc,CDocEntry **_doc_e,I64 *_type=NULL)
{
  U8 *st;
  if ((*_doc_e)->type_u8==DOCT_TEXT && (st=(*_doc_e)->tag)) {
    if (!StrNCmp("#help_index",st,11)) {
      if (_type) *_type=0;
      return st+11;
    } else if (!StrNCmp("#help_file",st,10)) {
      if (_type) *_type=1;
      return st+10;//'#' makes it first alphabetically.
    } else if (!StrNCmp("import",st,6)) {
      if (_type) *_type=2;
      return st+6;
    } else if (!StrNCmp("public import",st,13)) {
      if (_type) *_type=2;
      return st+13;
    } else if (!StrNCmp("_import",st,7)) {
      if (_type) *_type=2;
      return SHSkipLabel(doc,_doc_e,st+7);
    } else if (!StrNCmp("public _import",st,14)) {
      if (_type) *_type=2;
      return SHSkipLabel(doc,_doc_e,st+14);
    } else if (!StrNCmp("_intern",st,7)) {
      if (_type) *_type=3;
      return SHSkipLabel(doc,_doc_e,st+7);
    } else if (!StrNCmp("public _intern",st,14)) {
      if (_type) *_type=3;
      return SHSkipLabel(doc,_doc_e,st+14);
    } else if (!StrNCmp("extern",st,6)) {
      if (_type) *_type=4;
      return st+6;
    } else if (!StrNCmp("public extern",st,13)) {
      if (_type) *_type=4;
      return st+13;
    } else if (!StrNCmp("_extern",st,7)) {
      if (_type) *_type=4;
      return SHSkipLabel(doc,_doc_e,st+7);
    } else if (!StrNCmp("public _extern",st,14)) {
      if (_type) *_type=4;
      return SHSkipLabel(doc,_doc_e,st+14);
    }
  }
  if (_type) *_type=-1;
  return NULL;
}

I64 SHCompareEntry(CSHEntry *e1,CSHEntry *e2)
{
  U8 *st1,*st2;
  I64 t1,t2;
  CDocEntry *doc_e1,*doc_e2;
  if (e1 && e2) {
    doc_e1=e1->next;
    doc_e2=e2->next;
    if (doc_e1!=e1 && doc_e2!=e2) {
      if (doc_e1->type_u8==DOCT_TEXT && doc_e2->type_u8==DOCT_TEXT) {
	st1=SHNewEntry(e1,&doc_e1,&t1);
	st2=SHNewEntry(e2,&doc_e2,&t2);
	if (t1!=t2)
	  return t1-t2;
	else {
	  if (st1&&st2) {
	    st1=SHSkipDataType(e1,&doc_e1,st1);
	    st2=SHSkipDataType(e2,&doc_e2,st2);
	    if (st1&&st2)
	      return StrCmp(st1,st2);
	    else
	      return SignI64(st1-st2);
	  } else
	    return SignI64(st1-st2);
	}
      } else
	return SignI64(e1->next-e2->next);
    }
  }
  return SignI64(e1-e2);
}

U0 SHSortSection(CSHSection *tmps)
{
  I64 i=0,cnt;
  U8 *st;
  CSHEntry **a;
  CDocEntry *doc_e,*doc_e1,*doc_e2,*sec_start=tmps->next;

  while (sec_start!=tmps && sec_start->type_u8==DOCT_TEXT) {
    st=sec_start->tag;
    if (*st) {
      st+=StrLen(st)-1;
      if (*st=='\\')
	sec_start=sec_start->next;
      else
	break;
    } else
      break;
  }
  sec_start=sec_start->next;

  cnt=0;
  doc_e=sec_start;
  while (doc_e!=tmps) {
    doc_e2=doc_e;
    if (SHNewEntry(tmps,&doc_e2))
      cnt++;
    doc_e=doc_e->next;
  }
  cnt++;

  a=MAlloc(sizeof(CSHEntry *)*cnt);
  for (i=0;i<cnt;i++) {
    a[i]=MAlloc(sizeof(CSHEntry));
    QueInit(a[i]);
  }

  i=0;
  doc_e=sec_start;
  while (doc_e!=tmps) {
    doc_e1=doc_e->next;
    QueRem(doc_e);
    doc_e2=doc_e;
    if (SHNewEntry(tmps,&doc_e2))
      i++;
    QueIns(doc_e,a[i]->last);
    doc_e=doc_e1;
  }

  QSortI64(a,cnt,&SHCompareEntry);

  for (i=0;i<cnt;i++) {
    doc_e=a[i]->next;
    while (doc_e!=a[i]) {
      doc_e1=doc_e->next;
      QueRem(doc_e);
      QueIns(doc_e,tmps->last);
      doc_e=doc_e1;
    }
    Free(a[i]);
  }
  Free(a);
}

U8 *SHNewSection(CDocEntry *doc_e)
{
  U8 *st;
  if (doc_e->type_u8==DOCT_TEXT && (st=doc_e->tag)) {
    if (!StrNCmp(st,"#help_index",11))
      return st+11;
  }
  return NULL;
}

I64 SHCompareSection(CSHSection *e1,CSHSection *e2)
{
  I64 i1,i2;
  U8 *st1,*st2;
  if (e1 && e2 && e1->next!=e1 && e2->next!=e2) {
    if (e1->next->type_u8==DOCT_TEXT && e2->next->type_u8==DOCT_TEXT) {
      st1=SHNewSection(e1->next);
      st2=SHNewSection(e2->next);
      if (st1&&st2) {
	if (StrMatch("\"\"",st1))
	  i1=1;
	else
	  i1=0;
	if (StrMatch("\"\"",st2))
	  i2=1;
	else
	  i2=0;
	if (i1!=i2)
	  return i1-i2;
	else
	  return StrCmp(st1,st2);
      } else
	return SignI64(st1-st2);
    } else
      return SignI64(e1->next-e2->next);
  } else
    return SignI64(e1-e2);
}

public U0 SortHeader(U8 *name)
{
  CDoc *doc;
  I64 i=0,cnt;
  CSHSection **a;
  CDocEntry *doc_e,*doc_e1;

  if (!name || !*name || !FileFind(name))
    return;

  doc=DocRead(name,DOCF_PLAIN_TEXT_TABS|DOCF_NO_CURSOR);

  cnt=1;
  doc_e=doc->head.next;
  while (doc_e!=doc) {
    if (SHNewSection(doc_e))
      cnt++;
    doc_e=doc_e->next;
  }

  a=MAlloc(sizeof(CSHSection *)*cnt);
  for (i=0;i<cnt;i++) {
    a[i]=MAlloc(sizeof(CSHSection));
    QueInit(a[i]);
  }

  i=0;
  doc_e=doc->head.next;
  while (doc_e!=doc) {
    doc_e1=doc_e->next;
    if (doc_e->type_u8==DOCT_TEXT && doc_e->tag && *doc_e->tag) {
      QueRem(doc_e);
      if (SHNewSection(doc_e))
	i++;
      QueIns(doc_e,a[i]->last);
    } else
      DocEntryDel(doc,doc_e);
    doc_e=doc_e1;
  }

  QSortI64(a,cnt,&SHCompareSection);

  for (i=0;i<cnt;i++) {
    if (a[i]->next!=a[i]) {
      SHSortSection(a[i]);
      doc_e=a[i]->next;
      while (doc_e!=a[i]) {
	doc_e1=doc_e->next;
	QueRem(doc_e);
	QueIns(doc_e,doc->head.last);
	if (doc_e->type_u8==DOCT_TEXT && doc_e->tag) {
	  doc->cur_entry=doc_e;
	  doc->cur_col=StrLen(doc_e->tag);
	  DocPrint(doc,"\n");
	}
	doc_e=doc_e1;
      }
      if (i+1<cnt && a[i+1]->next!=a[i+1])
	DocPrint(doc,"\n");
    }
    Free(a[i]);
  }
  Free(a);
  DocWrite(doc);
  DocDel(doc);
}

public U0 SortHeaders()
{
  "Sorting Headers...\n";
  SortHeader("/Kernel/KernelB.HH.Z");
  SortHeader("/Kernel/KernelC.HH.Z");
  SortHeader("/Kernel/KExts.HC.Z");
  SortHeader("/Compiler/CExts.HC.Z");
  SortHeader("/Compiler/CompilerB.HH.Z");
  SortHeader("/Adam/AExts.HC.Z");
}

SortHeaders;
