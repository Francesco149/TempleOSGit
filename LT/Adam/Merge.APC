#help_index "Utils;Cmd Line (Typically)"

LTDirEntry MCMatchFind(LTDirEntry *pattern,LTDirEntry *list)
{
  while (list) {
    if (!StrCmp(pattern->name,list->name))
      return list;
    list=list->next;
  }
  return NULL;
}

U0 MergeChkList(LTDirEntry *m1,LTDirEntry *m2,U64 *_fuf_flags,U64 *df_flags)
{
  I64 i;
  LTDirEntry *tempm2;
  while (m1 && !(*df_flags & DF_ABORT_ALL_FILES)) {
    tempm2=MCMatchFind(m1,m2);
    if (!tempm2)
      "$FG,BROWN$Does Not Exist:%s$FG$\n",m1->full_name;
    else {
      if (m1->attr & LT_ATTR_DIR)
	MergeChkList(m1->sub,tempm2->sub,_fuf_flags,df_flags);
      else {
	i=m1->datetime-tempm2->datetime;
	if (AbsI64(i)>LTDATE_FREQ*2) { //Filedates can shift due to conversion
	  "" m1->full_name;
	  NewLine;
	  if (Bt(_fuf_flags,FUf_DIFF)) {
	    if (IsTextFile(m1->full_name))
	      *df_flags=Diff(tempm2->full_name,m1->full_name,*df_flags);
	  }
	}
      }
    }
    m1=m1->next;
  }
}

public U0 MergeChk(
  U8 *dst_files_find_mask="/LT/*",
  U8 *src_files_find_mask="/LT/*",
  U8 *fu_flags=NULL)
//"+d"=Do [C:/LT/Adam/Diff.APZ,203] Diff() on files
{
  U64 df_flags=0,fuf_flags=0;
  LTDirEntry *tempm1=NULL,*tempm2=NULL;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  PrintFWarn("This is based strictly on file dates.\n");
  tempm1=FilesFind(src_files_find_mask,fuf_flags);
  tempm2=FilesFind(dst_files_find_mask,fuf_flags);
  MergeChkList(tempm1,tempm2,&fuf_flags,&df_flags);
  LTDirListDel(tempm1);
  LTDirListDel(tempm2);
}

#help_index "Utils;Cmd Line (Typically)"

U0 ListNewFilesList(LTDirEntry *tempm,LTDate datetime)
{
  while (tempm) {
    if (tempm->datetime>datetime) {
      "" tempm->full_name;
      NewLine;
    }
    ListNewFilesList(tempm->sub,datetime);
    tempm=tempm->next;
  }
}

public U0 ListNewFiles(U8 *files_find_mask="/LT/*",LTDate datetime=0,
  U8 *fu_flags=NULL)
//"+t"=is just text files
{
  U64 fuf_flags=0;
  LTDirEntry *tempm1=NULL;
  if (!datetime)
    datetime=sys_compile_time;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+t");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (tempm1=FilesFind(files_find_mask,fuf_flags)) {
    ListNewFilesList(tempm1,datetime);
    LTDirListDel(tempm1);
  }
}

#help_index ""
