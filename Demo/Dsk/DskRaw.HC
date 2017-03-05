U0 RawAccess(I64 drv_let=0)
{
  CDrv *dv=Let2Drv(drv_let);
  CBlkDev *bd=Let2BlkDev(drv_let);
  I64 blk,old_offset,old_size;
  U8 *buf=MAlloc(BLK_SIZE);

  DrvRep;
  PressAKey;

  ClassRep(dv);
  PressAKey;

  ClassRep(bd);
  PressAKey;

  //Temporarily change partition range to full device.
  //Normally, you are limited to just your partition.
  //This is obviously **dangerous**!
  old_offset=dv->drv_offset;
  old_size  =dv->size;
  dv->drv_offset=0;
  dv->size=bd->max_blk+1;

  try {
    blk=GetI64("Blk to Read: ",old_offset,0,bd->max_blk);
    BlkRead(dv,buf,blk,1);
    D(buf,BLK_SIZE);
  } catch
    PutExcept;

  dv->drv_offset=old_offset;
  dv->size  =old_size;

  Free(buf);
}

RawAccess;

//See [C:/Kernel/BlkDev/DskBlk.HC.Z,31] BlkRead(), [C:/Kernel/BlkDev/DskBlk.HC.Z,71] BlkWrite(), [C:/Kernel/BlkDev/DskDrv.HC.Z,1] DrvLock(), [C:/Kernel/BlkDev/DskDrv.HC.Z,14] DrvUnlock()
//[C:/Adam/ABlkDev/ADskA.HC.Z,2] DBlk(), [C:/Adam/ABlkDev/ADskA.HC.Z,21] DClus(), [C:/Kernel/BlkDev/DskBlkDev.HC.Z,1] BlkDevLock() and [C:/Kernel/BlkDev/DskBlkDev.HC.Z,15] BlkDevUnlock().
