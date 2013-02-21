U0 DollarFile(CDirEntry *m1)
{
  CDoc *doc;
  while (m1) {
    if (m1->attr & _ATTR_DIR)
      DollarFile(m1->sub);
    else {
      "" m1->full_name;
      NewLine;
      doc=DocRead(m1->full_name,DOCF_PLAIN_TEXT|DOCF_DBL_DOLLARS);
      DocWrite(doc);
      DocDel(doc);
    }
    m1=m1->next;
  }
}


public U0 Dollar(U8 *files_find_mask,U8 *fu_flags=NULL)
//This is for importing non-"SparrowOS" text files.
//**WARNING:**Do not use on files with dollar cmds
// or on binary files. See [C:/SparrowOS/OSMain/Dsk/Dsk1b.CPP.Z,196] FilesFind() for flags.
{
  CDirEntry *tempm1=NULL;
  I64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tempm1=FilesFind(files_find_mask,fuf_flags);
  DollarFile(tempm1);
  CDirLstDel(tempm1);
}
