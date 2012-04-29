U0 RawAccess(I64 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  LTBlkDev *bdev=Drv2BlkDev(drv_let);
  I64 blk,old_offset,old_size;
  U8 *buf=MAlloc(BLK_SIZE);

  PrtRep;
  PressAKey;

  ClassRep("LTPrt",p);
  PressAKey;

  ClassRep("LTBlkDev",bdev);
  PressAKey;

  //Temporarily change partition range to full device.
  //Normally, you are limited to just your partition.
  //This is obviously **dangerous**!
  old_offset=p->offset;
  old_size  =p->size;
  p->offset=bdev->min_blk;
  p->size=bdev->max_blk-bdev->min_blk+1;

  try {
    blk=PmtI64("Blk to Read: ",old_offset,bdev->min_blk,bdev->max_blk);
    RBlks(p,buf,blk,1);
    D(buf,BLK_SIZE);
  } catch
    CatchAll;

  p->offset=old_offset;
  p->size  =old_size;

  Free(buf);
}

RawAccess;

//See [C:/LT/OSMain/Dsk/Dsk1a.CPZ,277] RBlks(), [C:/LT/OSMain/Dsk/Dsk1a.CPZ,312] WBlks(), [C:/LT/OSMain/Dsk/Dsk1a.CPZ,15] PrtLock(), [C:/LT/OSMain/Dsk/Dsk1a.CPZ,47] PrtUnlock()
//[C:/LT/Adam/Dsk/Dsk2a.APZ,3] DBlk(), [C:/LT/Adam/Dsk/Dsk2a.APZ,21] DCluster(), [C:/LT/OSMain/Dsk/Dsk1a.CPZ,1] BlkDevLock() and [C:/LT/OSMain/Dsk/Dsk1a.CPZ,28] BlkDevUnlock().
