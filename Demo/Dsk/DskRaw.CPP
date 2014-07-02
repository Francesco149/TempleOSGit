U0 RawAccess(I64 drv_let=0)
{
  CPrt *p=Drv2Prt(drv_let);
  CBlkDev *bdev=Drv2BlkDev(drv_let);
  I64 blk,old_offset,old_size;
  U8 *buf=MAlloc(BLK_SIZE);

  PrtRep;
  PressAKey;

  ClassRep(p);
  PressAKey;

  ClassRep(bdev);
  PressAKey;

  //Temporarily change partition range to full device.
  //Normally, you are limited to just your partition.
  //This is obviously **dangerous**!
  old_offset=p->offset;
  old_size  =p->size;
  p->offset=bdev->min_blk;
  p->size=bdev->max_blk-bdev->min_blk+1;

  try {
    blk=GetI64("Blk to Read: ",old_offset,bdev->min_blk,bdev->max_blk);
    RBlks(p,buf,blk,1);
    D(buf,BLK_SIZE);
  } catch
    PutExcept;

  p->offset=old_offset;
  p->size  =old_size;

  Free(buf);
}

RawAccess;

//See [C:/Kernel/Dsk/Dsk1a.CPP.Z,311] RBlks(), [C:/Kernel/Dsk/Dsk1a.CPP.Z,347] WBlks(), [C:/Kernel/Dsk/Dsk1a.CPP.Z,15] PrtLock(), [C:/Kernel/Dsk/Dsk1a.CPP.Z,44] PrtUnlock()
//[C:/Adam/Dsk/Dsk2a.CPP.Z,3] DBlk(), [C:/Adam/Dsk/Dsk2a.CPP.Z,22] DCluster(), [C:/Kernel/Dsk/Dsk1a.CPP.Z,1] BlkDevLock() and [C:/Kernel/Dsk/Dsk1a.CPP.Z,28] BlkDevUnlock().
