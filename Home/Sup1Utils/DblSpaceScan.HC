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

I64 DblSpaceScanLst(CDirEntry *tmpde)
{
  I64 res=0;
  CDirEntry *tmpde1;
  while (tmpde) {
    tmpde1=tmpde->next;
    if (tmpde->attr & RS_ATTR_DIR) {
      if (tmpde->sub) {
	"Scanning Directory: %s\n",tmpde->full_name;
	res+=DblSpaceScanLst(tmpde->sub);
      }
    } else
      res+=DblSpaceScanFile(tmpde->full_name);
    DirEntryDel(tmpde);
    tmpde=tmpde1;
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

