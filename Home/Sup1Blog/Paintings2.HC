I64 Paintings2(CDoc *paint1_in,CDoc *paint2_out)
{
  U8 *st,*l1,*l2,*ptr,*ptr2,buf[STR_LEN];
  I64 res=0;
  CDocEntry *doc_e;
  doc_e=paint1_in->head.next;
  while (doc_e!=paint1_in) {
    if (doc_e->type_u8==DOCT_TEXT && *doc_e->tag=='#') {
      if (doc_e->tag[1]) {
	st=doc_e->tag+1;
	if (StrLen(st)>3+7+6) {
	  st+=3;
	  st[6]=0;
	  l1=StrNew(st);
	  st+=7;
	  st+=6;
	  l2=StrNew(st);
	  if (ptr=StrMatch(" (Top Painting",l2)) {
	    if (StrLen(ptr)>15) {
	      *ptr=0;
	      ptr+=15;
	      if (ptr2=StrMatch("/",ptr)) {
		*ptr2=0;
		StrCpy(buf,"0000");
		StrCpy(buf+4-StrLen(ptr),ptr);
		"#%s %s\n",buf,l2;
		"%s\n",l1;
		DocPrint(paint2_out,"#%s %s\n",buf,l2);
		DocPrint(paint2_out,"%s\n",l1);
		res++;
	      }
	    }
	  }
	  Free(l1);
	  Free(l2);
	}
      }
    }
    doc_e=doc_e->next;
  }
  return res;
}

I64 Paintings3(CDoc *paint2_in,CDoc *paint3_out)
{
  I64 i=1,res=0;
  CDocEntry *doc_e;
  doc_e=paint2_in->head.next;
  while (doc_e!=paint2_in) {
    if (doc_e->type_u8==DOCT_TEXT) {
      if (*doc_e->tag=='#') {
	if (StrLen(doc_e->tag)>6)
	  DocPrint(paint3_out,"#%03d %s\n",i++,doc_e->tag+6);
	else
	  DocPrint(paint3_out,"\n");
      } else
	DocPrint(paint3_out,"http://en.most-famous-paintings.com/"
	      "MostFamousPaintings.nsf/A?Open&amp;A=%s\n",doc_e->tag);
      res++;
    }
    doc_e=doc_e->next;
  }
  return res;
}

I64 Main()
{
  I64 res;
  CDoc	*paint1_in=DocRead("~/Paintings1.DD.Z"),
	*paint2_out=DocNew("~/Paintings2.DD.Z"),
	*paint2_in,
	*paint3_out;
  DocMax;
  DocClear;
  if (res=Paintings2(paint1_in,paint2_out)) {
    "Res:%d\n",res;
    DocTop(paint2_out);
    DocWrite(paint2_out);
    Sort("~/Paintings2.DD.Z",,2);
    paint2_in=DocRead("~/Paintings2.DD.Z"),
    paint3_out=DocNew("~/Paintings3.DD.Z"),
    if (res=Paintings3(paint2_in,paint3_out)) {
      "Res:%d\n",res;
      DocTop(paint3_out);
      DocWrite(paint3_out);
    }
    DocDel(paint2_in);
    DocDel(paint3_out);
  }
  DocDel(paint1_in);
  DocDel(paint2_out);
  return res;
}

Main;
