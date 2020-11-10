I64 CvtSprite300Sprite(U8 *elems)
{
  I64 res=0,size;
  CSprite *tmpg=elems-offset(CSprite.start);
  while (tmpg->type) {
    size=SpriteElemSize(tmpg);
    if (tmpg->type==SPT_ELLIPSE||
	  tmpg->type==SPT_POLYGON||
	  tmpg->type==SPT_BITMAP) {
      "IN :W%d H%d (%d,%d)\n",
	    tmpg->pwhu.width,tmpg->pwhu.height,tmpg->pwhu.x1,tmpg->pwhu.y1;
      SwapU32(&tmpg->pwhu.x1,&tmpg->pwhu.width);
      SwapU32(&tmpg->pwhu.y1,&tmpg->pwhu.height);
      "OUT:(%d,%d) W%d H%d\n",
	    tmpg->pwhu.width,tmpg->pwhu.height,tmpg->pwhu.x1,tmpg->pwhu.y1;
      res++;
    }
    tmpg(U8 *)+=size;
  }
  return res;
}

I64 CvtSprite300Doc(CDoc *doc)
{
  Bool unlock=DocLock(doc);
  CDocBin *b;
  I64 res=0;
  DocRecalc(doc);
  b=doc->bin_head.next;
  while (b!=&doc->bin_head) {
    res+=CvtSprite300Sprite(b->data);
    b=b->next;
  }
  DocRecalc(doc);
  if (unlock)
    DocUnlock(doc);
  return res;
}

I64 CvtSprite300File(U8 *filename)
{
  I64 res;
  CDoc *doc=DocRead(filename);
  if (res=CvtSprite300Doc(doc)) {
    "-%d:%s\n",res,doc->filename.name;
    DocWrite(doc);
  }
  DocDel(doc);
  return res;
}
I64 CvtSprite300Lst(CDirEntry *tmpde)
{
  I64 res=0;
  CDirEntry *tmpde1;
  while (tmpde) {
    tmpde1=tmpde->next;
    if (tmpde->attr & RS_ATTR_DIR) {
      if (tmpde->sub) {
	"Scanning Directory: %s\n",tmpde->full_name;
	res+=CvtSprite300Lst(tmpde->sub);
      }
    } else
      res+=CvtSprite300File(tmpde->full_name);
    DirEntryDel(tmpde);
    tmpde=tmpde1;
  }
  return res;
}
public I64 CvtSprite300(U8 *files_find_mask="*",U8 *fu_flags=NULL)
{
  I64 f=0;
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),"+r+$");
  ScanFlags(&f,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  return CvtSprite300Lst(FilesFind(files_find_mask,f));
}
