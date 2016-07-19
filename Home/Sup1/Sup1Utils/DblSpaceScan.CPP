I64 DblSpaceScanFile(U8 *filename)
{
  CDoc *doc=DocRead(filename);
  I64 i,res=0;
  CDocEntry *doc_e=doc->head.next;
  while (doc_e!=doc) {
    if (doc_e->type_u8==DOCT_NEW_LINE &&
	  doc_e->next->type_u8==DOCT_NEW_LINE &&
	  doc_e->next->next->type_u8==DOCT_NEW_LINE) {
      i=0;
      while (doc_e->type_u8==DOCT_NEW_LINE) {
	i++;
	doc_e=doc_e->next;
      }
      "%02d:",i;
      PutFileLink(doc->filename.name,,doc_e->y);
      '\n';
      res+=i;
    } else
      doc_e=doc_e->next;
  }
  DocDel(doc);
  return res;
}

I64 DblSpaceScanLst(CDirEntry *tempde)
{
  I64 res=0;
  CDirEntry *tempde1;
  while (tempde) {
    tempde1=tempde->next;
    if (tempde->attr & RS_ATTR_DIR) {
      if (tempde->sub) {
	"Scanning Directory: %s\n",tempde->full_name;
	res+=DblSpaceScanLst(tempde->sub);
      }
    } else
      res+=DblSpaceScanFile(tempde->full_name);
    DirEntryDel(tempde);
    tempde=tempde1;
  }
  return res;
}

public I64 DblSpaceScan(U8 *files_find_mask="*",U8 *fu_flags=NULL)
{
  I64 f=0;
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),"+r+S");
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  return DblSpaceScanLst(FilesFind(files_find_mask,f));
}

