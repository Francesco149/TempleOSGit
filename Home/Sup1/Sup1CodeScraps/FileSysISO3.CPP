//ISO1 is ISO9660
//ISO2 is ISO13490
//ISO3 is ISO13346

//ISO13346 tag id types
#define ISO3T_PRI_VOL_DESC		1
#define ISO3T_ANCHOR_VOL_DESC_PTR	2
#define ISO3T_VOL_DESC_PTR		3
#define ISO3T_IMPLEMENTATION_VOL_DESC	4
#define ISO3T_DRV_DESC			5
#define ISO3T_LOG_VOL_DESC		6
#define ISO3T_UNALLOCED_SPACE_DESC	7
#define ISO3T_TERMINATOR_DESC		8
#define ISO3T_LOG_VOL_INTEGRITY_DESC	9
#define ISO3T_FILE_SET_DESC		256
#define ISO3T_FILE_ID_DESC		257
#define ISO3T_ALLOC_EXTENT_DESC		258
#define ISO3T_INDIRECT_ENTRY		259
#define ISO3T_TERMINAL_ENTRY		260
#define ISO3T_FILE_ENTRY		261
#define ISO3T_EXT_ATTR_HEADER_DESC	262
#define ISO3T_UNALLOC_SPACE_ENTRY	263
#define ISO3T_SPACE_BITMAP_DESC		264
#define ISO3T_DRV_INTEGRITY_ENTRY	265
#define ISO3T_EXT_FILE_ENTRY		266

class CISO3Tag
{
  U16 id;
  U16 version;
  U8 xsum;
  U8 reserved;
  U16 serial_num;
  U16 desc_crc;
  U16 desc_crc_len;
  U32 loc;
};

class CISO3Date
{
  U16 time_zone;
  I16 year;
  U8 mon;
  U8 day;
  U8 hour;
  U8 min;
  U8 sec;
  U8 sec100;
  U8 sec10000;
  U8 sec1000000;
};

class CISO3DString32
{
  U8 body[31];
  U8 len;
};

class CISO3DString128
{
  U8 body[127];
  U8 len;
};

class CISO3CharSpec
{
  U8 type;
  U8 body[63];
};

class CISO3RegId
{
  U8 flags;
  U8 id[23];
  U8 id_suffix[8];
};

class CISO3ExtentAD
{
  U32 len;
  U32 loc;
};

class CISO3LBAdd
{
  U32 log_blk_num;
  U16 drv_ref_num;
};

class CISO3LongAD
{
  U32 len;
  CISO3LBAdd loc;
  U8 implementation_use[6];
};

class CISO3ICBTag
{
  U32 prior_record_direct_entries;
  U16 strategy_type;
  U16 strategy_param;
  U16 max_entries;
  U8 reserved;
  U8 file_type;
  CISO3LBAdd parent_icb_loc;
  U16 flags;
};

class CISO3PriDesc
{
  CISO3Tag tag;
  U32 vol_desc_seq_num;
  U32 pri_vol_desc_num;
  CISO3DString32 vol_id;
  U16 vol_seq_num;
  U16 max_vol_seq_num;
  U16 interchg_level;
  U16 max_interchg_level;
  U32 char_set_lst;
  U32 max_char_set_lst;
  CISO3DString128 vol_set_id;
  CISO3CharSpec desc_char_set;
  CISO3CharSpec explanatory_char_set;
  CISO3ExtentAD vol_abstract;
  CISO3ExtentAD vol_copyright_notice;
  CISO3RegId app_id;
  CISO3Date recording_date_time;
  CISO3RegId implementation_id[32];
  U8 implementation_use[64];
  U32 pred_vol_desc_seq_loc;
  U16 flags;
  U8 reserved[22];
};

class CISO3DrvMap
{
  U8 type;
  U8 len;
  U0 mapping;
};

class CISO3DrvMap1
{
  U8 type;
  U8 len;
  U16 vol_seq_num;
  U16 drv_num;
};

class CISO3DrvMap2
{
  U8 type;
  U8 len;
  U8 drv_id[62];
};

class CISO3LogVolDesc
{
  CISO3Tag tag;
  U32 vol_desc_seq_num;
  CISO3CharSpec desc_char_set;
  CISO3DString128 log_vol_id;
  U32 blk_size;
  CISO3RegId domain_id;
  U8 log_vol_contents_use[16];
  U32 map_table_len;
  U32 num_drv_maps;
  CISO3RegId implementation_id;
  U8 implementation_use[128];
  CISO3ExtentAD integrity_seq_extent;
  U0 drv_maps;
};

class CISO3Drv
{
  CISO3Tag tag;
  U32 vol_desc_seq_num;
  U16 flags;
  U16 num;
  CISO3RegId contents;
  U8 contents_use[128];
  U32 access_type;
  U32 starting_loc;
  U32 len;
  CISO3RegId implementation_id;
  U8 implementation_use[128];
  U8 reserved[156];
};

class CISO3FileSetDesc
{
  CISO3Tag tag;
  CISO3Date recording_date_time;
  U16 interchg_level;
  U16 max_interchg_level;
  U32 char_set_lst;
  U32 max_char_set_lst;
  U32 file_set_num;
  U32 file_set_desc_num;
  CISO3CharSpec log_vol_id_char_set;
  CISO3DString128 log_vol_id;
  CISO3CharSpec	file_set_char_set;
  CISO3DString32 file_set_id;
  CISO3DString32 copyright_file_id;
  CISO3DString32 abstract_file_id;
  CISO3LongAD root_dir_icb;
  CISO3RegId domain_id;
  CISO3LongAD next_extent;
  CISO3LongAD sys_stream_dir_icb;
  U8 reserved[32];
};

class CISO3FileIdDesc
{
  CISO3Tag tag;
  U16 version;
  U8 characteristics;
  U8 len_of_file_id;
  CISO3LongAD icb;
  U16 len_of_implementation_use;
  U0 implementation_use;
  U0 file_id;
};

class CISO3File
{
  CISO3Tag tag;
  CISO3ICBTag icb_tag;
  U32 uid;
  U32 gid;
  U32 permissions;
  U16 file_link_cnt;
  U8 record_fmt;
  U8 record_display_attr;
  U32 record_len;
  I64 info_len;
  I64 log_blks_recorded;
  CISO3Date access_date_time;
  CISO3Date modification_date_time;
  CISO3Date attr_date_time;
  U32 chk_pt;
  CISO3LongAD ext_attr_icb;
  CISO3RegId implementation_id;
  I64 unique_id;
  U32 ext_attr_len;
  U32 alloc_desc_len;
  U0 ext_attr;
  U0 alloc_desc;
};

class CISO3FileExt
{
  CISO3Tag tag;
  CISO3ICBTag icb_tag;
  U32 uid;
  U32 gid;
  U32 permissions;
  U16 file_link_cnt;
  U8 record_fmt;
  U8 record_display_attr;
  U32 record_len;
  I64 info_len;
  I64 object_size;
  I64 log_blks_recorded;
  CISO3Date access_date_time;
  CISO3Date modification_date_time;
  CISO3Date creation_date_time;
  CISO3Date attr_date_time;
  U32 chk_pt;
  U32 reserved;
  CISO3LongAD ext_attr_icb;
  CISO3LongAD stream_dir_icb;
  CISO3RegId implementation_id;
  I64 unique_id;
  U32 ext_attr_len;
  U32 alloc_desc_len;
  U0 ext_attr;
  U0 alloc_desc;
};

import U0 ClassRep(U8 *class_name,U8 *d,I64 max_depth=2,
	Bool fun=FALSE,I64 offset=0);

Bool ISO3DrvInitDrv(CDrv *dv,I64 blk)
{
  CBlkDev *bd=dv->bd;
  I64 spc=bd->blk_size>>BLK_SIZE_BITS,i=blk/spc;
  CISO3FileSetDesc *iso=MAlloc(bd->blk_size);
  Bool unlock,res=FALSE;
  try {
    unlock=DrvLock(dv);
    while (TRUE) {
      dv->size=MaxI64(dv->size,(i+1)*spc);
      RBlks(dv,iso,i*spc,spc);
      "%X:%X\n",i,iso->tag.id;
      switch (iso->tag.id) {
	case ISO3T_FILE_SET_DESC:
	  ClassRep("CISO3FileSetDesc",iso,3);
//  dv->drv_offset=blk;
	  //  dv->root_cluster=iso->root_dir_icb.loc.log_blk_num+
	  //dv->drv_offset/spc;
	  res=TRUE;
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
	case ISO3T_DRV_INTEGRITY_ENTRY:
	  break;
	default:
	  goto i3pip_done;
      }
      i++;
    }
i3pip_done:
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  } catch {
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  }
  return res;
}

Bool ISO3DrvInit(CDrv *dv,I64 blk)
{
  CBlkDev *bd=dv->bd;
  I64 spc=bd->blk_size>>BLK_SIZE_BITS,i=blk/spc,offset=0;
  CISO3Drv *iso=MAlloc(bd->blk_size);
  Bool unlock,res=FALSE;

  try {
    unlock=DrvLock(dv);
    while (TRUE) {
      dv->size=MaxI64(dv->size,(i+1)*spc);
      RBlks(dv,iso,i*spc,spc);
      "%X:%X\n",i,iso->tag.id;
      switch (iso->tag.id) {
	case ISO3T_DRV_DESC:
	  ClassRep("CISO3Drv",iso,3);
	  offset=iso->starting_loc*spc; //one partition for now
	  break;
	case ISO3T_PRI_VOL_DESC:
	  ClassRep("CISO3PriDesc",iso,3);
	  break;
	case ISO3T_LOG_VOL_DESC:
	  ClassRep("CISO3LogVolDesc",iso,3);
	  D(&iso(CISO3LogVolDesc *)->drv_maps);
	  break;
	case ISO3T_ANCHOR_VOL_DESC_PTR:
	case ISO3T_VOL_DESC_PTR:
	case ISO3T_IMPLEMENTATION_VOL_DESC:
	case ISO3T_UNALLOCED_SPACE_DESC:
	case ISO3T_LOG_VOL_INTEGRITY_DESC:
	  break;
	case ISO3T_TERMINATOR_DESC:
	  goto i3pi_done;
	default:
	  PrintErr("File System Not Supported\n");
	  throw('Drv');
      }
      i++;
    }
i3pi_done:
    if (offset)
      res=ISO3DrvInitDrv(dv,offset);
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  } catch {
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  }
  return res;
}

/****
U0 CDate2ISO3(CISO3Date *dst,CDate spdt)
{
  CDateStruct ds;
  CDate2CDateStruct(&ds,spdt);
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

Bool ISODrvInit(CDrv *dv,I64 blk)
{
  CBlkDev *bd=dv->bd;
  I64 spc=bd->blk_size>>BLK_SIZE_BITS,i=blk/spc;
  CISO1PriDesc *iso=MAlloc(bd->blk_size);
  CISO1DirEntry *de;
  Bool unlock,res=FALSE;
  U8 buf[8];

  try {
    unlock=DrvLock(dv);
    dv->fs_type=FSt_ISO9660; //default
    dv->spc=spc;
    dv->drv_offset=dv->data_area=dv->root_cluster=dv->size=0;
    while (TRUE) {
      dv->size=MaxI64(dv->size,(i+1)*spc);
      RBlks(dv,iso,i*spc,spc);
      buf[0](U32)=iso->id[0](U32);
      buf[4](U16)=iso->id[4](U8);
      switch (LstEntryMatch(buf,
	    "CD001\0CDW02\0BEA01\0BOOT2\0NSR02\0NSR03\0TEA01\0",LEF_EXACT)) {
	case 0:
	  dv->fs_type=FSt_ISO9660;
	  switch (iso->type) {
	    case ISO1T_SUPPLEMENTARY_DESC:
	      dv->data_area=0;
	      de=&iso->root_dir_record;
	      dv->root_cluster=de->loc.little;
	      dv->size=iso->vol_space_size.little*bd->blk_size>>BLK_SIZE_BITS;
	      res=TRUE;
	      goto ipi_done;
	    case ISO1T_TERMINATOR:
	      throw('Drv');
	  }
	  break;
	case 2...6:
	  dv->fs_type=FSt_ISO13346;
	  break;
	case 1: //FSt_ISO13490 (Not supported)
	default: //Its normal for ISO13346 to read NULL blk as terminator
	  switch (dv->fs_type) {
	    case FSt_ISO13346:
	      i++; //Terminator is blk after last valid.  Advance to next blk
	      if (ISO3DrvInit(dv,i*spc)) {
		res=TRUE;
		goto ipi_done;
	      }
	      break;
	  }
	  PrintErr("File System Not Supported\n");
	  throw('Drv');
      }
      i++;
    }
ipi_done:
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  } catch {
    dv->fs_type=FSt_ISO9660;
    dv->spc=spc;
    dv->drv_offset=dv->data_area=dv->root_cluster=0;
    Free(iso);
    if (unlock)
      DrvUnlock(dv);
  }
  return res;
}
