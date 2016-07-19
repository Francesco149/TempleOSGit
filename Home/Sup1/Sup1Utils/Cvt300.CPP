I64 Cvt300Sprite(U8 *elems)
{
  I64 res=0,size;
  CSprite *tempg=elems-offset(CSprite.start);
  while (tempg->type) {
    size=SpriteElemSize(tempg);
    if (tempg->type==SPT_ELLIPSE||
	  tempg->type==SPT_POLYGON||
	  tempg->type==SPT_BITMAP) {
      "IN :W%d H%d (%d,%d)\n",
	    tempg->pwhu.width,tempg->pwhu.height,tempg->pwhu.x1,tempg->pwhu.y1;
      SwapU32(&tempg->pwhu.x1,&tempg->pwhu.width);
      SwapU32(&tempg->pwhu.y1,&tempg->pwhu.height);
      "OUT:(%d,%d) W%d H%d\n",
	    tempg->pwhu.width,tempg->pwhu.height,tempg->pwhu.x1,tempg->pwhu.y1;
      res++;
    }
    tempg(U8 *)+=size;
  }
  return res;
}

I64 Cvt300Doc(CDoc *doc)
{
  Bool unlock=DocLock(doc);
  CDocBin *b;
  I64 res=0;
  DocRecalc(doc);
  b=doc->bin_head.next;
  while (b!=&doc->bin_head) {
    res+=Cvt300Sprite(b->data);
    b=b->next;
  }
  DocRecalc(doc);
  if (unlock)
    DocUnlock(doc);
  return res;
}

I64 Cvt300File(U8 *filename)
{
  I64 res;
  CDoc *doc=DocRead(filename);
  if (res=Cvt300Doc(doc)) {
    "-%d:%s\n",res,doc->filename.name;
    DocWrite(doc);
  }
  DocDel(doc);
  return res;
}
I64 Cvt300Lst(CDirEntry *tempde)
{
  I64 res=0;
  CDirEntry *tempde1;
  while (tempde) {
    tempde1=tempde->next;
    if (tempde->attr & RS_ATTR_DIR) {
      if (tempde->sub) {
	"Scanning Directory: %s\n",tempde->full_name;
	res+=Cvt300Lst(tempde->sub);
      }
    } else
      res+=Cvt300File(tempde->full_name);
    DirEntryDel(tempde);
    tempde=tempde1;
  }
  return res;
}
public I64 Cvt300(U8 *files_find_mask="*",U8 *fu_flags=NULL)
{
  I64 f=0;
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),"+r+T");
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  return Cvt300Lst(FilesFind(files_find_mask,f));
}
