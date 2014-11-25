U0 ATABlkSelect(CBlkDev *bd,I64 blk,I64 cnt)
{
  if (bd->type!=BDT_ATAPI && bd->base1)
    OutU8(bd->base1+ATAR1_CTRL,0x8);
  if (bd->flags & BDF_EXT_SIZE) { //48 Bit LBA?
    OutU8(bd->base0+ATAR0_NSECT,cnt.u8[1]);
    OutU8(bd->base0+ATAR0_SECT,blk.u8[3]);
    OutU8(bd->base0+ATAR0_LCYL,blk.u8[4]);
    OutU8(bd->base0+ATAR0_HCYL,blk.u8[5]);
    OutU8(bd->base0+ATAR0_NSECT,cnt);
    OutU8(bd->base0+ATAR0_SECT,blk);
    OutU8(bd->base0+ATAR0_LCYL,blk.u8[1]);
    OutU8(bd->base0+ATAR0_HCYL,blk.u8[2]);
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  } else { //28 Bit LBA
    OutU8(bd->base0+ATAR0_NSECT,cnt);
    OutU8(bd->base0+ATAR0_SECT,blk);
    OutU8(bd->base0+ATAR0_LCYL,blk.u8[1]);
    OutU8(bd->base0+ATAR0_HCYL,blk.u8[2]);
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4|blk.u8[3]);
  }
}

Bool ATAWaitNotBUSY(CBlkDev *bd,F64 timeout)
{
  I64 i;
  do {
    for (i=0;i<3;i++)
      if (!(InU8(bd->base0+ATAR0_STAT)&ATAS_BSY))
	return TRUE;
    Yield;
  } while (!(0<timeout<tS));
  return FALSE;
}

Bool ATAWaitDRQ(CBlkDev *bd,F64 timeout)
{
  I64 i;
  do {
    for (i=0;i<3;i++)
      if (InU8(bd->base0+ATAR0_STAT)&ATAS_DRQ)
	return TRUE;
    Yield;
  } while (!(0<timeout<tS));
  return FALSE;
}

Bool ATANop(CBlkDev *bd,F64 timeout)
{
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_FEAT,0);
  OutU8(bd->base0+ATAR0_CMD,ATA_NOP);
  return ATAWaitNotBUSY(bd,timeout);
}

U0 ATACmd(CBlkDev *bd,U8 cmd)
{
  OutU8(bd->base0+ATAR0_FEAT,0);
  OutU8(bd->base0+ATAR0_CMD,cmd);
  bd->last_time=tS;
  PortNop;
}

Bool ATAGetResult(CBlkDev *bd,F64 timeout,U8 *buf,I64 cnt,I64 _avail,Bool one_read)
{
  I64 avail,overflow;
  bd->flags&=~BDF_LAST_WAS_WRITE;
  MemSet(buf,0,cnt);
  while (cnt>0) {
    if (!ATAWaitDRQ(bd,timeout))
      return FALSE;
    if (_avail)
      avail=_avail;
    else
      avail=InU8(bd->base0+ATAR0_HCYL)<<8+InU8(bd->base0+ATAR0_LCYL);
    if (avail) {
      if (avail>cnt) {
	overflow=avail-cnt;
	avail=cnt;
      } else
	overflow=0;
      if (avail&2)
	RepInU16(buf,avail>>1,bd->base0+ATAR0_DATA);
      else
	RepInU32(buf,avail>>2,bd->base0+ATAR0_DATA);
      cnt-=avail;
      buf+=avail;
      while (overflow>0) {
	InU16(bd->base0+ATAR0_DATA);
	overflow-=2;
	if (0<timeout<tS)
	  return FALSE;
      }
      if (one_read)
	break;
    } else
      Yield;
  }
  return ATAWaitNotBUSY(bd,timeout);
}

Bool ATAPIWritePktWord(CBlkDev *bd,F64 timeout,...)
{
  I64 i;
  for (i=0;i<argc;i++) {
    if (!ATAWaitDRQ(bd,timeout))
      return FALSE;
    OutU16(bd->base0+ATAR0_DATA,EndianU16(argv[i]));
    bd->last_time=tS;
  }
  return TRUE;
}

Bool ATAPISetMaxSpeed(CBlkDev *bd)
{
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_LCYL,0);
  OutU8(bd->base0+ATAR0_HCYL,0);
  ATACmd(bd,ATA_PACKET);
  ATAPIWritePktWord(bd,0,0xBB00,0xFFFF,0xFFFF,0,0,0);
  return ATAWaitNotBUSY(bd,0);
}

Bool ATAPISeek(CBlkDev *bd,I64 native_blk)
{
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_LCYL,0);
  OutU8(bd->base0+ATAR0_HCYL,0);
  ATACmd(bd,ATA_PACKET);
  ATAPIWritePktWord(bd,0,0x2B00,native_blk>>16,native_blk,0,0,0);
  return ATAWaitNotBUSY(bd,0);
}

Bool ATAPIStartStop(CBlkDev *bd,F64 timeout,Bool start)
{
  I64 i;
  if (start)
    i=0x100;
  else
    i=0;
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  ATACmd(bd,ATA_PACKET);
  //Start/Stop
  if (ATAPIWritePktWord(bd,timeout,0x1B00,0,i,0,0,0))
    return ATAWaitNotBUSY(bd,timeout);
  else
    return FALSE;
}

I64 ATAGetDevId(CBlkDev *bd,F64 timeout,U16 **_id_record)
{
  U16 *id_record;
  if (_id_record) *_id_record=NULL;
  if (bd->type!=BDT_ATAPI && bd->base1)
    OutU8(bd->base1+ATAR1_CTRL,0x8);
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  ATACmd(bd,ATA_ID_DEV);
  if (!ATAWaitNotBUSY(bd,timeout))
    return BDT_NULL;
  if (InU8(bd->base0+ATAR0_STAT)&ATAS_ERR)
    return BDT_ATAPI;
  id_record=ACAlloc(512);
  if (!ATAGetResult(bd,timeout,id_record,512,512,FALSE)) {
    Free(id_record);
    return BDT_NULL;
  }
  if (_id_record) *_id_record=id_record;
  return BDT_ATA;
}

I64 ATAReadNativeMax(CBlkDev *bd,F64 timeout)
{//Returns zero on err
  I64 result=0;
  Bool okay=TRUE;

  if (bd->type==BDT_ATAPI) {
    if (bd->flags & BDF_EXT_SIZE)
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
    else
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
    ATACmd(bd,ATA_DEV_RST);
    if (!ATAWaitNotBUSY(bd,0))
      okay=FALSE;
  } else {
    while (InU8(bd->base0+ATAR0_STAT) & ATAS_BSY) {
      if (bd->flags&BDF_LAST_WAS_WRITE)
	OutU16(bd->base0+ATAR0_DATA,0);
      else
	InU16(bd->base0+ATAR0_DATA);
      Yield;
      if (0<timeout<tS)
	return FALSE;
    }

    Free(bd->dev_id_record);
    if (ATAGetDevId(bd,timeout,&bd->dev_id_record)==BDT_NULL)
      okay=FALSE;
    else
      BEqu(&bd->flags,BDf_EXT_SIZE,Bt(&bd->dev_id_record[86],10));
  }
  if (okay) {
    if (bd->flags & BDF_EXT_SIZE && bd->base1) {
      OutU8(bd->base1+ATAR1_CTRL,0x8);
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
      ATACmd(bd,ATA_READ_NATIVE_MAX_EXT);
      if (ATAWaitNotBUSY(bd,timeout)) {
	result.u8[0]=InU8(bd->base0+ATAR0_SECT);
	result.u8[1]=InU8(bd->base0+ATAR0_LCYL);
	result.u8[2]=InU8(bd->base0+ATAR0_HCYL);

	OutU8(bd->base1+ATAR1_CTRL,0x80);
	result.u8[3]=InU8(bd->base0+ATAR0_SECT);
	result.u8[4]=InU8(bd->base0+ATAR0_LCYL);
	result.u8[5]=InU8(bd->base0+ATAR0_HCYL);
 
	if (result>>24==result&0xFFFFFF) {//Kludge to make qemu-kvm work
	  bd->flags&=~BDF_EXT_SIZE;
	  result&=0xFFFFFF;
	}
      }
    } else {
      if (bd->type!=BDT_ATAPI && bd->base1)
	OutU8(bd->base1+ATAR1_CTRL,0x8);
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
      ATACmd(bd,ATA_READ_NATIVE_MAX);
      if (ATAWaitNotBUSY(bd,timeout)) {
	result.u8[0]=InU8(bd->base0+ATAR0_SECT);
	result.u8[1]=InU8(bd->base0+ATAR0_LCYL);
	result.u8[2]=InU8(bd->base0+ATAR0_HCYL);
	result.u8[3]=InU8(bd->base0+ATAR0_SELECT) & 0xF;
      }
    }
  }
  return bd->max_blk=result;
}

I64 ATAPIReadCapacity(CBlkDev *bd,I64 *_blk_size=NULL)
{//Supposedly this can return a result +/- 75 sects.
//Error might just be for music.
  Bool unlock=BlkDevLock(bd);
  U32 buf[2];
  if (ATAWaitNotBUSY(bd,0)) {
    if (bd->flags & BDF_EXT_SIZE)
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
    else
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
    OutU8(bd->base0+ATAR0_LCYL,8);
    OutU8(bd->base0+ATAR0_HCYL,0);
    ATACmd(bd,ATA_PACKET);
    ATAPIWritePktWord(bd,0,0x2500,0,0,0,0,0);
    if (!ATAGetResult(bd,0,buf,8,0,TRUE))
      buf[0]=buf[1]=0;
  } else
    buf[0]=buf[1]=0;

  if (unlock) BlkDevUnlock(bd);
  if (_blk_size) *_blk_size=EndianU32(buf[1]);
  return EndianU32(buf[0]);
}

CATAPITrack *ATAPIReadTrackInfo(CBlkDev *bd,I64 blk)
{
  CATAPITrack *result=CAlloc(sizeof(CATAPITrack));
  Bool unlock=BlkDevLock(bd);
  if (ATAWaitNotBUSY(bd,0)) {
    if (bd->flags & BDF_EXT_SIZE)
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
    else
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
    OutU8(bd->base0+ATAR0_LCYL,sizeof(CATAPITrack)&0xFF);
    OutU8(bd->base0+ATAR0_HCYL,sizeof(CATAPITrack)>>8);
    ATACmd(bd,ATA_PACKET);
    ATAPIWritePktWord(bd,0,0x5200,blk.u16[1],blk.u16[0],
	(sizeof(CATAPITrack)&0xFF00)>>8,(sizeof(CATAPITrack)&0x00FF)<<8,0);
    if (!ATAGetResult(bd,0,result,sizeof(CATAPITrack),0,TRUE)) {
      Free(result);
      result=NULL;
    }
  } else {
    Free(result);
    result=NULL;
  }
  if (unlock) BlkDevUnlock(bd);
  return result;
}

Bool ATAInit(CBlkDev *bd)
{
  Bool unlock=BlkDevLock(bd),okay=FALSE;

  if (bd->type==BDT_ATAPI)
    bd->flags&=~BDF_EXT_SIZE;
  else
    bd->flags|=BDF_EXT_SIZE;

  if (ATAReadNativeMax(bd,tS+0.1)) {

    ATABlkSelect(bd,bd->max_blk,0);
    if (bd->flags&BDF_EXT_SIZE)
      ATACmd(bd,ATA_SET_MAX_EXT);
    else
      ATACmd(bd,ATA_SET_MAX);
    if (ATAWaitNotBUSY(bd,0)) {
      okay=TRUE;
      if (bd->type==BDT_ATAPI) {
	if (ATAPIStartStop(bd,0,TRUE)) {
	  if(!ATAPISetMaxSpeed(bd))
	    okay=FALSE;
	} else
	  okay=FALSE;
      }
    }
  }
  if (unlock) BlkDevUnlock(bd);
  return okay;
}

Bool ATAPIWaitReady(CBlkDev *bd,F64 timeout)
{
  do {
    if (!ATAWaitNotBUSY(bd,timeout) ||
	!ATANop(bd,timeout) ||
	!ATAPIStartStop(bd,timeout,TRUE))
      return FALSE;
    if (InU8(bd->base0+ATAR0_STAT) & ATAS_DRDY &&
	!InU8(bd->base0+ATAR0_FEAT));
      return TRUE;
    ATAInit(bd);
    Yield;
  } while (!(0<timeout<tS));
  return FALSE;
}

U0 ATAReadBlks(CBlkDev *bd,U8 *buf, I64 blk, I64 cnt)
{
  I64 retries=3;
  Bool unlock=BlkDevLock(bd);

retry:
  ATABlkSelect(bd,blk,cnt);
  if (bd->flags & BDF_EXT_SIZE)
    ATACmd(bd,ATA_READ_MULTI_EXT);
  else
    ATACmd(bd,ATA_READ_MULTI);
  if (!ATAGetResult(bd,tS+1.0,buf,cnt*bd->blk_size,BLK_SIZE,FALSE)) {
    if (retries--) {
      ATAWaitNotBUSY(bd,0);
      goto retry;
    } else
      throw('BlkDev');
  }

  dsk.read_cnt+=(cnt*bd->blk_size)>>BLK_SIZE_BITS;
  if (unlock) BlkDevUnlock(bd);
}

I64 ATAProbe(I64 base0,I64 base1,I64 unit)
{
  CBlkDev bd;
  MemSet(&bd,0,sizeof(CBlkDev));
  bd.type=BDT_ATAPI;
  bd.base0=base0;
  bd.base1=base1;
  bd.unit=unit;
  bd.blk_size=CD_BLK_SIZE;
  return ATAGetDevId(&bd,tS+0.1,NULL);
}

Bool ATAPIReadBlks2(CBlkDev *bd,F64 timeout,U8 *buf, I64 native_blk, I64 cnt,Bool lock)
{
  Bool result=FALSE,unlock;
  if (cnt<=0)
    return FALSE;
  if (lock)
    unlock=BlkDevLock(bd);
  if (ATAPIWaitReady(bd,timeout)) {
    if (bd->flags & BDF_EXT_SIZE)
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
    else
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
    OutU8(bd->base0+ATAR0_LCYL,bd->blk_size);
    OutU8(bd->base0+ATAR0_HCYL,bd->blk_size.u8[1]);
    ATACmd(bd,ATA_PACKET);
    if (ATAPIWritePktWord(bd,timeout,0xA800,
	  native_blk.u16[1],native_blk,cnt.u16[1],cnt,0) &&
	  ATAGetResult(bd,timeout,buf,cnt*bd->blk_size,0,FALSE)) {
      dsk.read_cnt+=(cnt*bd->blk_size)>>BLK_SIZE_BITS;
      result=TRUE;
    }
  }
//  ATAPIStartStop(bd,0,FALSE);
  if (lock && unlock) BlkDevUnlock(bd);
  return result;
}

U0 ATAPIReadBlks(CBlkDev *bd,U8 *buf, I64 blk, I64 cnt)
{
  CDrv *dv=Let2Drv(bd->first_drv_let);
  I64 retry,spc=bd->blk_size>>BLK_SIZE_BITS,n,blk2,
    l2=bd->max_reads<<1+spc<<1;
  U8 *cd_buf=MAlloc(l2<<BLK_SIZE_BITS);
  if (cnt>0) {
    if (blk<=bd->max_reads)
      blk2=0;
    else
      blk2=FloorU64(blk-bd->max_reads,spc);
    if (blk2+l2>dv->size)
      l2=dv->size-blk2;
    n=(l2+spc-1)/spc;

    retry=4;
    while (--retry)
      if (ATAPIReadBlks2(bd,tS+7.0+0.004*n,cd_buf,blk2/spc,n,TRUE))
	//n is 0x800 if max_reads.  Up to 8 additional seconds
	break;
    if (!retry)
      ATAPIReadBlks2(bd,0,cd_buf,blk2/spc,n,TRUE);
    if (bd->flags & BDF_READ_CACHE)
      DskCacheAdd(dv,cd_buf,blk2,n*spc);
    MemCpy(buf,cd_buf+(blk-blk2)<<BLK_SIZE_BITS,cnt<<BLK_SIZE_BITS);
  }
  Free(cd_buf);
}

Bool ATARBlks(CDrv *dv,U8 *buf, I64 blk, I64 cnt)
{
  I64 n;
  CBlkDev *bd=dv->bd;
  while (cnt>0) {
    n=cnt;
    if (n>bd->max_reads)
      n=bd->max_reads;
    if (bd->type==BDT_ATAPI)
      ATAPIReadBlks(bd,buf,blk,n);
    else
      ATAReadBlks(bd,buf,blk,n);
    buf+=n<<BLK_SIZE_BITS;
    blk+=n;
    cnt-=n;
  }
  return TRUE;
}

U0 ATAWriteBlks(CBlkDev *bd,U8 *buf, I64 blk, I64 cnt)
{//For low level disk access.
//Use WBlks() instead.
  I64 i,U32s_avail,sects_avail,retries=3;
  F64 timeout;
  Bool unlock=BlkDevLock(bd);
retry:
  ATABlkSelect(bd,blk,cnt);
  if (bd->flags&BDF_EXT_SIZE)
    ATACmd(bd,ATA_WRITE_MULTI_EXT);
  else
    ATACmd(bd,ATA_WRITE_MULTI);
  bd->flags|=BDF_LAST_WAS_WRITE;
  while (cnt>0) {
    timeout=tS+1.0;
    while (TRUE) {
      i=InU8(bd->base0+ATAR0_STAT);
      if (!(i & ATAS_DRDY)||!(i & ATAS_DRQ)) {
	Yield;
      } else
	break;
      if (/* i&ATAS_ERR||*/ tS>timeout) {
	if (retries--) {
	  ATAWaitNotBUSY(bd,0);
	  goto retry;
	} else
	  throw('BlkDev');
      }
    }
    sects_avail=1;
    U32s_avail=sects_avail<<BLK_SIZE_BITS>>2;
    RepOutU32(buf,U32s_avail,bd->base0+ATAR0_DATA);
    buf+=U32s_avail<<2;
    cnt-=sects_avail;
    retries=3;
  }
  ATAWaitNotBUSY(bd,0);
  if (unlock) BlkDevUnlock(bd);
}

Bool ATAPISync(CBlkDev *bd)
{
  Bool okay=TRUE;
  if (!ATAWaitNotBUSY(bd,0))
    okay=FALSE;
  else {
    if (bd->flags & BDF_EXT_SIZE)
      OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
    else
      OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
    OutU8(bd->base0+ATAR0_LCYL,0);
    OutU8(bd->base0+ATAR0_HCYL,0);
    ATACmd(bd,ATA_PACKET);
    ATAPIWritePktWord(bd,0,0x3500,0,0,0,0,0);
    if (!ATAWaitNotBUSY(bd,0))
      okay=FALSE;
  }
  return okay;
}

U0 ATAPIClose(CBlkDev *bd,I64 close_field=0x200,I64 track=0)
{//0x200 CD/DVD part 1
// 0x300    DVD part 2
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_LCYL,0);
  OutU8(bd->base0+ATAR0_HCYL,0);
  ATACmd(bd,ATA_PACKET);
  ATAPIWritePktWord(bd,0,0x5B00,close_field,track,0,0,0);
  ATAWaitNotBUSY(bd,0);
}

U0 ATAPIWriteBlks(CBlkDev *bd,U8 *buf, I64 native_blk, I64 cnt)
{
  I64 U32s_avail;
  U8 *buf2;
  ATAWaitNotBUSY(bd,0);
  ATAPISeek(bd,native_blk);

  OutU8(bd->base0+ATAR0_FEAT,0);
  OutU8(bd->base0+ATAR0_LCYL,bd->blk_size);
  OutU8(bd->base0+ATAR0_HCYL,bd->blk_size.u8[1]);
  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_CMD,ATA_PACKET);
  ATAPIWritePktWord(bd,0,0x0400,native_blk.u16[1],native_blk,cnt.u16[1],cnt,0);
  bd->flags|=BDF_LAST_WAS_WRITE;
  ATAWaitNotBUSY(bd,0);

  ATAPISeek(bd,native_blk);

  if (bd->flags & BDF_EXT_SIZE)
    OutU8(bd->base0+ATAR0_SELECT,0xEF|bd->unit<<4);
  else
    OutU8(bd->base0+ATAR0_SELECT,0xE0|bd->unit<<4);
  OutU8(bd->base0+ATAR0_LCYL,bd->blk_size);
  OutU8(bd->base0+ATAR0_HCYL,bd->blk_size.u8[1]);
  ATACmd(bd,ATA_PACKET);
  ATAPIWritePktWord(bd,0,0xAA00,native_blk.u16[1],native_blk,cnt.u16[1],cnt,0);
  buf2=buf+bd->blk_size*cnt;
  while (buf<buf2) {
    ATAWaitDRQ(bd,0);
    U32s_avail=(InU8(bd->base0+ATAR0_HCYL)<<8+InU8(bd->base0+ATAR0_LCYL))>>2;
    if (buf+U32s_avail<<2>buf2)
      U32s_avail=(buf2-buf)>>2;
    if (U32s_avail) {
      RepOutU32(buf,U32s_avail,bd->base0+ATAR0_DATA);
      buf+=U32s_avail<<2;
      dsk.write_cnt+=U32s_avail>>(BLK_SIZE_BITS-2);
    }
  }
  ATAWaitNotBUSY(bd,0);
}

Bool ATAWBlks(CDrv *dv,U8 *buf, I64 blk, I64 cnt)
{
  I64 n,spc;
  CBlkDev *bd=dv->bd;
  Bool unlock;
  spc=bd->blk_size>>BLK_SIZE_BITS;
  if (bd->type==BDT_ATAPI) {
    unlock=BlkDevLock(bd);
    ATAPIWaitReady(bd,0);
  }
  while (cnt>0) {
    n=cnt;
    if (n>bd->max_writes)
      n=bd->max_writes;
    if (bd->type==BDT_ATAPI)
      ATAPIWriteBlks(bd,buf,blk/spc,(n+spc-1)/spc);
    else
      ATAWriteBlks(bd,buf,blk,n);
    buf+=n<<BLK_SIZE_BITS;
    blk+=n;
    cnt-=n;
    dsk.write_cnt+=n;
  }
  if (bd->type==BDT_ATAPI) {
    ATAPISync(bd);
//    ATAPIStartStop(bd,0,FALSE);
    if (unlock) BlkDevUnlock(bd);
  }
  return TRUE;
}

Bool BootCdProbe(CBlkDev *bd)
{
  CKernel *img=CAlloc(CD_BLK_SIZE);
  I64 i;
  Bool result=FALSE;
  "Port: %04X,%04X Unit: %02X  ",bd->base0,bd->base1,bd->unit;
  if (ATAProbe(bd->base0,bd->base1,bd->unit)==BDT_ATAPI) {
    " ATAPI";
    if (ATAPIStartStop(bd,tS+5.0,TRUE)) {
      " Started";
      for (i=0;i<2;i++) {//Retry
	if (ATAPIReadBlks2(bd,tS+7.0,img,sys_boot_src.u16[1],1,FALSE)) {
	  if (img->compile_time==sys_compile_time) {
	    " Found";
	    return TRUE;
	  } else
	    " Read";
	} else
	  " NoRead";
      }
    }
  }
  " Nope\n";
  Free(img);
  return result;
}

Bool BootCdProbeAll(CBlkDev *bd)
{
  I64 d1,d2,i,j,k;

  bd->base1=0;
  for (k=0;k<256;k++) {
    i=-1;
    while (TRUE) {
      j=PCIClassFind(0x010100+k,++i);
      if (j<0)
	break;
      "Subcode:0x%X Bus:0x%X Dev:0x%X Fun:0x%X\n",k,j.u8[2],j.u8[1],j.u8[0];
      d1=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x10);
      d2=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x14);
      if (d1&1 && d2&1) {
	if (bd->base0=d1&~7) {
	  bd->unit=0;
	  if (BootCdProbe(bd))
	    return TRUE;
	  bd->unit=1;
	  if (BootCdProbe(bd))
	    return TRUE;
	}
      }
      d1=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x18);
      d2=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x1C);
      if (d1&1 && d2&1) {
	if (bd->base0=d1&~7) {
	  bd->unit=0;
	  if (BootCdProbe(bd))
	    return TRUE;
	  bd->unit=1;
	  if (BootCdProbe(bd))
	    return TRUE;
	}
      }
    }
  }

  d1=0x1F0;
  d2=0x3F6;
  if (bd->base0=d1&~7) {
    bd->unit=0;
    if (BootCdProbe(bd))
      return TRUE;
    bd->unit=1;
    if (BootCdProbe(bd))
      return TRUE;
  }

  d1=0x170;
  d2=0x376;
  if (bd->base0=d1&~7) {
    bd->unit=0;
    if (BootCdProbe(bd))
      return TRUE;
    bd->unit=1;
    if (BootCdProbe(bd))
      return TRUE;
  }
}

U0 ATARepEntry(I64 base0,I64 base1,I64 unit,I64 irq,U8 *msg,CATARep **_root,I64 *num_hints)
{
  I64 type;
  base0&=-8;
  base1&=-4;
  CATARep *tempha;
  if (type=ATAProbe(base0,base1,unit)) {
    *num_hints+=1;
    "\n\n$PURPLE$ $BT+X,\"%d\",LM=\"%d\\n\"$$FG$$LM,4$",*num_hints,*num_hints;
    if (type==BDT_ATA)
      "$RED$ATA   ";
    else
      "$RED$ATAPI ";
    "$LTBLUE$%s$FG$\nBase0:0x%04X    Base1:0x%04X\nIrq:0x%02X Unit:%d$LM,0$\n",msg,base0,base1,irq,unit;
    if (_root) {
      tempha=CAlloc(sizeof(CATARep));
      tempha->next=*_root;
      *_root=tempha;
      tempha->num=*num_hints;
      tempha->type=type;
      tempha->base0=base0;
      tempha->base1=base1;
      tempha->unit=unit;
    }
  }
}

Bool ATARepExitAllApplications()
{
	"\nWe're going to probe hardware.\n"
	"$RED$Exit all other applications.$FG$\n"
	"Press '$PURPLE$p$FG$' to probe or '$PURPLE$s$FG$' to skip.\n";
  if (ToUpper(GetChar(,FALSE))=='S')
    return TRUE;
  else
    return FALSE;
}

public I64 ATARep(Bool pmt=TRUE,Bool just_ide=FALSE,CATARep **_root=NULL)
{//Report possible ATA devices by probing.  Hard disks and CD/DVDs.
  I64 d1,d2,d3,i,j,k,cnt=0,unlock_flags=0,num_hints=0;
#assert NUM_BLKDEVS<=64
  Bool old_preempt;

  if (_root) *_root=NULL;

  if (pmt && ATARepExitAllApplications)
    return 0;

  old_preempt=Preempt(OFF);
  for (i=0;i<NUM_BLKDEVS;i++)
    if (dsk.blkdevs[i].bd_signature==BD_SIGNATURE_VAL)
      BEqu(&unlock_flags,i,
	    BlkDevLock(&dsk.blkdevs[i]));

  if (!just_ide)
    for (k=0;k<256;k++) {
      i=-1;
      while (TRUE) {
	j=PCIClassFind(0x010100+k,++i);
	if (j<0)
	  break;

	"\nSubcode:0x%X Bus:0x%X Dev:0x%X Fun:0x%X\n",k,j.u8[2],j.u8[1],j.u8[0];
	cnt++;

	d1=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x10);
	d2=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x14);
	d3=PCIReadU8(j.u8[2],j.u8[1],j.u8[0],0x3C);
	if (d1&1 && d2&1) {
	  ATARepEntry(d1,d2,0,d3,"Primary IDE",_root,&num_hints);
	  ATARepEntry(d1,d2,1,d3,"Primary IDE",_root,&num_hints);
	} else {
	  d1=0x1F0; d2=0x3F6; d3=14;
	  ATARepEntry(d1,d2,0,d3,"Primary IDE",_root,&num_hints);
	  ATARepEntry(d1,d2,1,d3,"Primary IDE",_root,&num_hints);
	}
	d1=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x18);
	d2=PCIReadU32(j.u8[2],j.u8[1],j.u8[0],0x1C);
	if (d1&1 && d2&1) {
	  ATARepEntry(d1,d2,0,d3,"Secondary IDE",_root,&num_hints);
	  ATARepEntry(d1,d2,1,d3,"Secondary IDE",_root,&num_hints);
	} else {
	  d1=0x170; d2=0x376; d3=15;
	  ATARepEntry(d1,d2,0,d3,"Secondary IDE",_root,&num_hints);
	  ATARepEntry(d1,d2,1,d3,"Secondary IDE",_root,&num_hints);
	}
      }
    }
  if (!cnt) {
    d1=0x1F0; d2=0x3F6; d3=14;
    ATARepEntry(d1,d2,0,d3,"Primary IDE",_root,&num_hints);
    ATARepEntry(d1,d2,1,d3,"Primary IDE",_root,&num_hints);

    d1=0x170; d2=0x376; d3=15;
    ATARepEntry(d1,d2,0,d3,"Secondary IDE",_root,&num_hints);
    ATARepEntry(d1,d2,1,d3,"Secondary IDE",_root,&num_hints);
  }
  '\n\n';
  for (i=0;i<NUM_BLKDEVS;i++)
    if (Bt(&unlock_flags,i))
      BlkDevUnlock(&dsk.blkdevs[i]);
  Preempt(old_preempt);
  return num_hints;
}

CATARep *ATAIDDrvs(CATARep *root,U8 _sys_acct_drv,CATARep **_ata_drv,CATARep **_atapi_drv)
{//This is for when trying to sort-out main hard drives and CD-ROM drives.
  CATARep *result=NULL,*tempha=root,*ata_drv=NULL,*atapi_drv=NULL;
  CBlkDev *bd;
  Bool was_silent=Silent(ON);
  try
    bd=Let2BlkDev(_sys_acct_drv);
  catch {
    Fs->catch_except=TRUE;
    bd=NULL;
  }
  Silent(was_silent);
  while (tempha) {
    if (!result && bd && bd->type==tempha->type) {
      if (bd->type==BDT_ATAPI &&
	  bd->base0==tempha->base0 && bd->unit==tempha->unit)
	result=atapi_drv=tempha;
      else if (bd->type==BDT_ATA && bd->base0==tempha->base0 &&
	  bd->base1==tempha->base1 && bd->unit==tempha->unit)
	result=ata_drv=tempha;
    }
    if (!result || result->type!=tempha->type) {
      if (tempha->type==BDT_ATA) {
	if (!ata_drv || tempha->unit<ata_drv->unit ||
	  tempha->unit==ata_drv->unit && tempha->num<ata_drv->num)
	  ata_drv=tempha;
      } else if (tempha->type==BDT_ATAPI) {
	if (!atapi_drv || tempha->unit<atapi_drv->unit ||
	  tempha->unit==atapi_drv->unit && tempha->num<atapi_drv->num)
	  atapi_drv=tempha;
      }
    }
    tempha=tempha->next;
  }
  if (_ata_drv)   *_ata_drv  =ata_drv;
  if (_atapi_drv) *_atapi_drv=atapi_drv;
  return result;
}

CBlkDev *ATAMount(U8 first_drv_let,I64 type,I64 base0,I64 base1,I64 unit)
{
  CBlkDev *result;
  if (0<=first_drv_let-'A'<NUM_DRVS && (type==BDT_ATA || type==BDT_ATAPI) &&
      0<=unit<=1) {
    result=BlkDevNextFree(first_drv_let,type);
    result->unit=unit;
    result->base0=base0;
    result->base1=base1;
    if (BlkDevAdd(result,FALSE,FALSE))
      return result;
  }
  return NULL;
}

I64 AutoMountIDE()
{//Try to mount hard drive and CD/DVD, automatically. (Kernel.Cfg option).
//It uses 'C' and 'T' as first drive letters or whatever you set
//in config when compiling Kernel.BIN.
  I64 result=0;
  CATARep *root=NULL,*ata_drv=NULL,*atapi_drv=NULL,*tempha;
  ATARep(FALSE,TRUE,&root);
  ATAIDDrvs(root,*sys_acct,&ata_drv,&atapi_drv);
  if (ata_drv && ATAMount(dsk.first_hd_drv_let,BDT_ATA,
	ata_drv->base0,ata_drv->base1,ata_drv->unit))
    result++;
  if (atapi_drv && ATAMount(dsk.first_cd_drv_let,BDT_ATAPI,
      atapi_drv->base0,atapi_drv->base1,atapi_drv->unit))
    result++;
  tempha=root;
  while (tempha) {
    if (tempha!=ata_drv && tempha!=atapi_drv) {
      if (tempha->type==BDT_ATA && ATAMount(dsk.first_hd_drv_let,BDT_ATA,
	  tempha->base0,tempha->base1,tempha->unit))
	result++;
      else if (tempha->type==BDT_ATAPI && ATAMount(dsk.first_cd_drv_let,BDT_ATAPI,
	  tempha->base0,tempha->base1,tempha->unit))
	result++;
    }
    tempha=tempha->next;
  }
  LinkedLstDel(root);
  dsk.auto_mount_ide_cnt=result;
  return result;
}
