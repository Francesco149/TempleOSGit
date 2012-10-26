//ISO1 is ISO9660
//ISO2 is ISO13490
//ISO3 is ISO13346

import U0 ClassRep(U8 *class_name,U8 *d,I64 max_depth=2,BoolI8 fun=FALSE,I64 offset=0);

BoolI64 ISO3PrtInitPrt(CPrt *p,U64 blk)
{
  CBlkDev *bdev=p->bdev;
  U64 spc=bdev->blk_size>>BLK_SIZE_BITS,i=blk/spc;
  CISO3FileSetDesc *iso=MAlloc(bdev->blk_size);
  BoolI8 unlock,result=FALSE;

  try {
    unlock=PrtLock(p);
    while (TRUE) {
      p->size=MaxI64(p->size,(i+1)*spc);
      RBlks(p,iso,i*spc,spc);
      "%X:%X\n",i,iso->tag.id;
      switch (iso->tag.id) {
	case ISO3T_FILE_SET_DESC:
	  ClassRep("CISO3FileSetDesc",iso,3);
//	  p->offset=blk;
//	  p->root_cluster=iso->root_dir_icb.loc.log_blk_num+p->offset/spc;
	  result=TRUE;
	  break;
	case ISO3T_EXT_FILE_ENTRY:
	  ClassRep("CISO3FileExt",iso,3);
	  break;
	case ISO3T_FILE_ENTRY:
	  ClassRep("CISO3File",iso,3);
	  break;
	case ISO3T_FILE_ID_DESC:
	  ClassRep("CISO3FileIdDesc",iso,3);
	  break;
	case ISO3T_ALLOC_EXTENT_DESC:
	case ISO3T_INDIRECT_ENTRY:
	case ISO3T_TERMINAL_ENTRY:
	case ISO3T_EXT_ATTR_HEADER_DESC:
	case ISO3T_UNALLOC_SPACE_ENTRY:
	case ISO3T_SPACE_BITMAP_DESC:
	case ISO3T_PRT_INTEGRITY_ENTRY:
	  break;
	default
	  goto i3pip_done;
      }
      i++;
    }
i3pip_done:
    Free(iso);
    if (unlock)
      PrtUnlock(p);
  } catch {
    Free(iso);
    if (unlock)
      PrtUnlock(p);
  }
}

BoolI64 ISO3PrtInit(CPrt *p,U64 blk)
{
  CBlkDev *bdev=p->bdev;
  U64 spc=bdev->blk_size>>BLK_SIZE_BITS,i=blk/spc,offset=0;
  CISO3Prt *iso=MAlloc(bdev->blk_size);
  BoolI8 unlock,result=FALSE;

  try {
    unlock=PrtLock(p);
    while (TRUE) {
      p->size=MaxI64(p->size,(i+1)*spc);
      RBlks(p,iso,i*spc,spc);
"%X:%X\n",i,iso->tag.id;
      switch (iso->tag.id) {
	case ISO3T_PRT_DESC:
	  ClassRep("CISO3Prt",iso,3);
	  offset=iso->starting_loc*spc; //one partition for now
	  break;
	case ISO3T_PRI_VOL_DESC:
	  ClassRep("CISO3PriDesc",iso,3);
	  break;
	case ISO3T_LOG_VOL_DESC:
	  ClassRep("CISO3LogVolDesc",iso,3);
	  D(&iso(CISO3LogVolDesc *)->prt_maps);
	  break;
	case ISO3T_ANCHOR_VOL_DESC_PTR:
	case ISO3T_VOL_DESC_PTR:
	case ISO3T_IMPLEMENTATION_VOL_DESC:
	case ISO3T_UNALLOCATED_SPACE_DESC:
	case ISO3T_LOG_VOL_INTEGRITY_DESC:
	  break;
	case ISO3T_TERMINATOR_DESC:
	  goto i3pi_done;
	default:
	  PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	  throw(EXCEPT_PRT,12);
      }
      i++;
    }
i3pi_done:
    if (offset)
      result=ISO3PrtInitPrt(p,offset);
    Free(iso);
    if (unlock)
      PrtUnlock(p);
  } catch {
    Free(iso);
    if (unlock)
      PrtUnlock(p);
  }
}


/****
U0 CDate2ISO3(CISO3Date *dst,CDate ltdt)
{
  CDateStruct ds;
  CDate2CDateStruct(&ds,ltdt);
  MemSet(dst,0,sizeof(CISO3Date));
  dst->year=ds.year;
  dst->mon=ds.mon;
  dst->day=ds.day_of_mon;
  dst->hour=ds.hour;
  dst->min=ds.min;
  dst->sec=ds.sec;
  dst->sec100=ds.sec100;
}

CDate ISO3Date2CDate(CISO3Date *dt)
{
  CDateStruct ds;
  MemSet(&ds,0,sizeof(CDateStruct));
  ds.day_of_mon=dt->day;
  ds.mon=dt->mon;
  ds.year=dt->year;
  ds.sec100=dt->sec100;
  ds.sec=dt->sec;
  ds.min=dt->min;
  ds.hour=dt->hour;
  return CDateStruct2CDate(&ds);
}
****/
