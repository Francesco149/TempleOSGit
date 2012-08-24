U0 DollarFile(LTDirEntry *m1)
{
  Ltf *l;
  while (m1) {
    if (m1->attr & LT_ATTR_DIR)
      DollarFile(m1->sub);
    else {
      "" m1->full_name;
      NewLine;
      l=LtfRead(m1->full_name,LTFF_PLAIN_TEXT|LTFF_DBL_DOLLARS);
      LtfWrite(l);
      LtfDel(l);
    }
    m1=m1->next;
  }
}


public U0 Dollar(U8 *files_find_mask,U8 *fu_flags=NULL)
//This is for importing non-"LT" text files.
//**WARNING:**Do not use on files with dollar cmds
// or on binary files. See [C:/LT/OSMain/Dsk/Dsk1b.CPZ,198] FilesFind() for flags.
{
  LTDirEntry *tempm1=NULL;
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tempm1=FilesFind(files_find_mask,fuf_flags);
  DollarFile(tempm1);
  LTDirListDel(tempm1);
}
