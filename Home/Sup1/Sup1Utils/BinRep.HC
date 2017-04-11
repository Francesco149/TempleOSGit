#help_index "Compiler"
DefineLstLoad("ST_BIN_FILE_TYPES",
"END\0"
" \0"
"REL_I0\0"
"IMM_U0\0"
"REL_I8\0"
"IMM_U8\0"
"REL_I16\0"
"IMM_U16\0"
"REL_I32\0"
"IMM_U32\0"
"REL_I64\0"
"IMM_I64\0"
" \0 \0 \0 \0"
"REL32_EXPORT\0"
"IMM32_EXPORT\0"
"REL64_EXPORT\0"
"IMM64_EXPORT\0"
"ABS_ADDR\0"
"CODE_HEAP\0"
"ZEROED_CODE_HEAP\0"
"DATA_HEAP\0"
"ZEROED_DATA_HEAP\0"
"MAIN\0"
);

public Bool BinRep(U8 *filename)
{//Reports imports, exports, etc for a .BIN file.
//See [C:/Compiler/CMain.HC.Z,1] Patch Table Generation and [C:/Kernel/KLoad.HC.Z,181] Load().
  U8 *fbuf,*ptr,*sptr,*absname;
  I64 i,j,size,etype;
  Bool res=FALSE;
  CBinFile *bfh;
  CHashExport *saved_hash_ptr=NULL;

  fbuf=ExtDft(filename,"BIN.Z");
  if (!(bfh=FileRead(fbuf,&size)))
    goto br_done1;
  if (bfh->bin_signature!=BIN_SIGNATURE_VAL) {
    "Not a TempleOS Binary File\n";
    goto br_done2;
  }
  absname=FileNameAbs(fbuf);
  Free(fbuf);
  fbuf=StrNew(absname);
  FileExtRem(fbuf);
  if (fbuf[1]==':' && StrLen(fbuf)>2)
    "     MODULE_NAME:%s\n",fbuf+2;
  else
    "NO MODULE NAME\n";

  if (bfh->org!=INVALID_PTR)
    "      ORG:%X\n",bfh->org;
  "    MODULE_ALIGN:%X\n",1<<bfh->module_align_bits;

  ptr=bfh(U8 *)+bfh->patch_table_offset;
  while (etype=*ptr++) {
    i=*ptr(U32 *)++;
    sptr=ptr;
    ptr+=StrLen(sptr)+1;
    "%16Z:$GREEN$%s$FG$:%X ",etype,"ST_BIN_FILE_TYPES",sptr,i;
    switch (etype) {
      case IET_REL_I0...IET_IMM_I64:
	if (*sptr)
	  saved_hash_ptr=HashFind(sptr,Fs->hash_table,HTG_ALL);
	if (!saved_hash_ptr)
	  "Unresolved Reference:%s\n",sptr;
	break;
      case IET_ABS_ADDR:
	'\n';
	for (j=0;j<i;j++)
	  "%X ",*ptr(U32 *)++;
	break;

      case IET_CODE_HEAP:
      case IET_ZEROED_CODE_HEAP:
	"Size:%X\n",*ptr(I32 *)++;
	for (j=0;j<i;j++)
	  "%X ",*ptr(U32 *)++;
	break;

      case IET_DATA_HEAP:
      case IET_ZEROED_DATA_HEAP:
	"Size:%X\n",*ptr(I64 *)++;
	for (j=0;j<i;j++)
	  "%X ",*ptr(U32 *)++;
	break;
    }
    '\n';
  }
  res=TRUE;
  Free(absname);
br_done2:
  Free(bfh);
br_done1:
  Free(fbuf);
  return res;
}
