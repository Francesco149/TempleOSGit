U0 LTDate2Dos(U16 *t,U16 *d,LTDate date)
{
  LTDateStruct ltdt;
  LTDate2LTDateStruct(&ltdt,date+local_time_offset);
  *d=ltdt.day_of_mon+(ltdt.mon+(ltdt.year-1980)<<4)<<5;
  *t=ltdt.sec>>1+(ltdt.min+ltdt.hour<<6)<<5;
}

LTDate Dos2LTDate(U16 t,U16 d)
{
  LTDateStruct ltdt;
  MemSet(&ltdt,0,sizeof(LTDateStruct));
  ltdt.day_of_mon=d&0x1F; d=d>>5;
  ltdt.mon=d&0xF;
  ltdt.year=d>>4+1980;
  ltdt.sec=(t&0x1F)*2; t=t>>5;
  ltdt.min=t&0x3F;
  ltdt.hour=t>>6;
  return LTDateStruct2LTDate(&ltdt)-local_time_offset;
}

U0 FAT32PrtInit(LTPrt *p)
{
  FAT32BootStruct br32;
  BoolI8 unlock;
  try {
    unlock=PrtLock(p);
    p->type=PT_FAT32;
    RBlks(p,&br32,p->offset,1);
    p->file_system_info_sect=p->offset+br32.file_system_info_sect;
    p->map1=p->offset+br32.reserved_sects;
    p->map2=p->map1+br32.sects_per_fat;
    p->data=p->map2+br32.sects_per_fat-2*br32.sects_per_cluster; //Starts at Cluster 2
    p->spc=br32.sects_per_cluster;
    p->root_cluster=br32.root_cluster;
    AllocMAPBuf(p,1);
    Free(p->fis);
    p->fis=AMAlloc(BLK_SIZE);
    RBlks(p,p->fis,p->file_system_info_sect,1);
    if (unlock)
      PrtUnlock(p);
  } catch
    if (unlock)
      PrtUnlock(p);
}

BoolI64 FAT32DrvValidate(U8 drv_let)
{
  LTPrt *p;
  FAT32BootStruct br;
  BoolI8 result=FALSE;
  try {
    p=Drv2Prt(drv_let);
    if (p->type==PT_FAT32 &&
	RBlks(p,&br,p->offset,1) &&
	br.signature==0xAA55 &&
	br.sects==p->size)
      result=TRUE;
  } catch
    Fs->catch_except=TRUE;
  return result;
}

U0 FAT32Fmt(U8 drv_let,BoolI8 quick=TRUE)
{
  FAT32BootStruct *br=CAlloc(BLK_SIZE);
  FAT32FileInfoSect *fis=CAlloc(BLK_SIZE);
  LTPrt *p=Drv2Prt(drv_let);
  I64 i,l;
  try {
    PrtLock(p);
    SetDrvType(drv_let,PT_FAT32);
    p->type=PT_FAT32;
    br->jump_and_nop[0]=0xEB;
    br->jump_and_nop[1]=0x58;
    br->jump_and_nop[2]=0x90;
    MemCpy(br->oem_name,"MSWIN4.1",8);
    br->bytes_per_sect=BLK_SIZE;
    if	  (p->size<= 500000)
      br->sects_per_cluster=1;
    else if (p->size<=2000000)
      br->sects_per_cluster=2;
    else if (p->size<=6000000)
      br->sects_per_cluster=4;
    else if (p->size<=12000000)
      br->sects_per_cluster=8;
    else if (p->size<=33000000)
      br->sects_per_cluster=16;
    else if (p->size<=67000000)
      br->sects_per_cluster=32;
    else
      br->sects_per_cluster=64;
 
    br->reserved_sects=32;
    br->copies_of_fat=2;
    br->media_desc=0xF8;
    br->sects=p->size;
    l=br->sects/br->sects_per_cluster/(BLK_SIZE/4)+1;
    br->sects_per_fat=l;
    br->root_cluster=2;
    br->file_system_info_sect=1;
    br->log_drv_num=0x80;
    br->ext_signature=0x29;
    br->serial_num=RandU32;
    MemCpy(br->vol_name,"NO NAME    ",11);
    MemCpy(br->fat_name,"FAT32   ",8);
    br->signature=0xAA55;
    fis->signature1=0x41615252;
    fis->signature2=0x61417272;
    fis->free_clusters=-1;
    fis->most_recently_allocated=0;
    fis->signature3=0xAA550000;
 
    if (quick)
      i=br->reserved_sects+2*l+4*br->sects_per_cluster;
    else
      i=p->size;
    WZeroBlks(p,p->offset,i);
 
    WBlks(p,fis,p->offset+br->file_system_info_sect,1,TRUE);
    WBlks(p,br,p->offset,1,TRUE);
    FAT32PrtInit(p);
    AllocClusters(p,0,1,FALSE); //Allocate #1
    br->root_cluster=AllocClusters(p,0,1,FALSE);
    WBlks(p,br,p->offset,1,TRUE);
    FAT32PrtInit(p);
    PrtUnlock(p);
  } catch
    PrtUnlock(p);
  Free(br);
  Free(fis);
}


BoolI64 FATNameTo(U8 *dst,U8 *src)
{
  I64 i;
  MemSet(dst,CH_SPACE,11);
  if (!FileNameChk(src))
    return FALSE;
  if (!StrCmp(src,"..")) {
    *dst='.';
    dst[1]='.';
    return TRUE;
  } else if (!StrCmp(src,".")) {
    *dst='.';
    return TRUE;
  }
  i=0;
  while (i<8 && *src && *src!='.')
    dst[i++]=ToUpper(*src++);
  i=8;
  if (*src=='.') src++;
  while (*src)
    if (*src!='.')
      dst[i++]=ToUpper(*src++);
    else
      src++;
  return TRUE;
}

I64 FATNameXSum(U8 *src)
{
  I64 i,result=0;
  for (i=0;i<11;i++)
    result.u8[0]=(result&1?0x80:0)+result>>1+*src++;
  return result;
}

BoolI64 FATFromName(U8 *dst,U8 *src)
{
  I64 i,j,k=0;
  for (j=7;j>=0 && src[j]==CH_SPACE;j--);
  for(i=0;i<=j;i++)
    dst[k++]=src[i];
  for (j=10;j>=8 && src[j]==CH_SPACE;j--);
  if (*src!='.' && j!=7)
    dst[k++]='.';
  for(i=8;i<=j;i++)
    dst[k++]=src[i];
  dst[k++]=0;
  return FileNameChk(dst);
}

U8 fat_long_name_map[13]={
  offset(FAT32LongDirEntry.name1),
  offset(FAT32LongDirEntry.name1)+2,
  offset(FAT32LongDirEntry.name1)+4,
  offset(FAT32LongDirEntry.name1)+6,
  offset(FAT32LongDirEntry.name1)+8,
  offset(FAT32LongDirEntry.name2),
  offset(FAT32LongDirEntry.name2)+2,
  offset(FAT32LongDirEntry.name2)+4,
  offset(FAT32LongDirEntry.name2)+6,
  offset(FAT32LongDirEntry.name2)+8,
  offset(FAT32LongDirEntry.name2)+10,
  offset(FAT32LongDirEntry.name3),
  offset(FAT32LongDirEntry.name3)+2
};

BoolI64 LTDirLongNameFill(LTDirEntry *tempm,FAT32LongDirEntry *de,I64 *xsum)
{
  I64 i;
  U8 *ptr=de;
  if (de->ord&0x40) {
    MemSet(tempm,0,sizeof(LTDirEntry));
    *xsum=de->xsum;
  } else if (de->type || de->zero || de->xsum!=*xsum) {
    MemSet(tempm,0,sizeof(LTDirEntry));
    *xsum=0;
    return FALSE;
  }
  switch (de->ord&0x3F) {
    case 1:
      for (i=0;i<13;i++)
	if (!(tempm->name[i]=ptr[fat_long_name_map[i]]))
	  return TRUE;
      break;
    case 2:
      for (i=0;i<12;i++)
	if (!(tempm->name[i+13]=ptr[fat_long_name_map[i]]))
	  return TRUE;
      break;
  }
  return TRUE;
}


BoolI64 FAT32LTDirFill(LTDirEntry *tempm,FAT32DirEntry *de)
{
  BoolI8 result;
  if (*tempm->name)
    result=TRUE;
  else
    result=FATFromName(tempm->name,de->name);
  tempm->cluster=de->cluster_lo+de->cluster_hi<<16;
  tempm->size=de->size;
  tempm->attr=de->attr;
  tempm->datetime=Dos2LTDate(de->WrtTime,de->WrtDate);
  return result;
}

BoolI64 FAT32DirFill(FAT32DirEntry *de,LTDirEntry *tempm,I64 *_de_cnt=NULL)
{ //Fill up to 3 entries and store cnt of entries.
  BoolI8 result;
  I64 de_cnt=0,i,l,xsum,ord;
  U8 dname[16];
  U8 *ptr;
  FAT32LongDirEntry *ld=de;

  MemSet(de,0,sizeof(FAT32DirEntry));
  result=FATNameTo(de->name,tempm->name);
  FATFromName(dname,de->name);
  if (StrCmp(dname,tempm->name)) {
    ord=0x41;
    xsum=FATNameXSum(de->name);
    if ((l=StrLen(tempm->name))>13) {
      ptr=&ld[de_cnt];
      MemSet(ptr,0,sizeof(FAT32LongDirEntry));
      ld[de_cnt].attr=LT_ATTR_LONG_NAME;
      ld[de_cnt].xsum=xsum;
      ld[de_cnt].ord=0x42;
      for (i=13;i<l;i++)
	ptr[fat_long_name_map[i-13]]=tempm->name[i];
      i++;
      for (;i<26;i++)
	ptr[fat_long_name_map[i-13]](U16)=0xFFFF;
      ord=1;
      l=13;
      de_cnt++;
    }
    ptr=&de[de_cnt];
    MemSet(ptr,0,sizeof(FAT32LongDirEntry));
    ld[de_cnt].attr=LT_ATTR_LONG_NAME;
    ld[de_cnt].xsum=xsum;
    ld[de_cnt].ord=ord;
    for (i=0;i<l;i++)
      ptr[fat_long_name_map[i]]=tempm->name[i];
    i++;
    for (;i<13;i++)
      ptr[fat_long_name_map[i]](U16)=0xFFFF;
    de_cnt++;
    MemSet(&de[de_cnt],0,sizeof(FAT32DirEntry));
    result=FATNameTo(de[de_cnt].name,tempm->name);
  }
  de[de_cnt].cluster_lo=tempm->cluster.u16[0];
  de[de_cnt].cluster_hi=tempm->cluster.u16[1];
  if (!(tempm->attr&LT_ATTR_DIR))
    de[de_cnt].size=tempm->size;
  de[de_cnt].attr=tempm->attr;
  if (!tempm->datetime)
    tempm->datetime=GetCurTimeLTDate;
  LTDate2Dos(&de[de_cnt].WrtTime,&de[de_cnt].WrtDate,tempm->datetime);
  if (_de_cnt)
    *_de_cnt=de_cnt+1;
  return result;
}


BoolI64 FAT32FileFind(LTPrt *p,U64 cur_dir_cluster,U8 *name,LTDirEntry *result,
		     BoolI8 dir_only,BoolI8 file_only)
{
  BoolI8 res=FALSE;
  FAT32DirEntry *buf;
  I64 xsum=0,attr,cur_dir_entry,entries_per_cluster;
  U8 ch;
  BoolI8 unlock;
  LTDirEntry long_name;
  U8 dname[LT_MAX_FILENAME_LEN+1];
 
  MemSet(result,0,sizeof(LTDirEntry));
  MemSet(&long_name,0,sizeof(LTDirEntry));
  PrtChk(p);
  if (p->type!=PT_FAT32)
    PutDefine("ST_NOT_FAT32");
  else if (!LTFileNameTo(dname,name))
    PutDefine("ST_INVALID_FILENAME");
  else {
    try {
      unlock=PrtLock(p);
      buf=MAlloc(BLK_SIZE*p->spc);
      entries_per_cluster=FAT32_ENTRIES_PER_BLK*p->spc;
      RClusters(p,buf,cur_dir_cluster,1);
      cur_dir_entry=0;
      while (ch=*buf[cur_dir_entry].name) {
	attr=buf[cur_dir_entry].attr;
	if (ch!=0xE5) {
	  if (attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
	    LTDirLongNameFill(&long_name,&buf[cur_dir_entry],&xsum);
	  else {
	    if (!(attr&LT_ATTR_VOL_ID)) {
	      if (xsum==FATNameXSum(buf[cur_dir_entry].name))
		MemCpy(result,&long_name,sizeof(LTDirEntry));
	      else
		MemSet(result,0,sizeof(LTDirEntry));
	      if (!(dir_only && !(attr & LT_ATTR_DIR)) &&
	      !(file_only && attr & LT_ATTR_DIR) &&
	      FAT32LTDirFill(result,&buf[cur_dir_entry]) &&
	      !StrCmp(dname,result->name)) {
			      res=TRUE;
		goto exit_this;
	      }
	    }
	    MemSet(&long_name,0,sizeof(LTDirEntry));
	  }
	} else
	  MemSet(&long_name,0,sizeof(LTDirEntry));
	if (++cur_dir_entry==entries_per_cluster) {
	  cur_dir_cluster=ClusterNumNext(p,cur_dir_cluster);
	  if (cur_dir_cluster==INVALID_CLUSTER||!cur_dir_cluster)
	    break;
	  else {
	    RClusters(p,buf,cur_dir_cluster,1);
	    cur_dir_entry=0;
	  }
	}
      }
      MemSet(result,0,sizeof(LTDirEntry));
exit_this:
      Free(buf);
      if (unlock)
	PrtUnlock(p);
    } catch
      if (unlock)
	PrtUnlock(p);
  }
  return res;
}

U8 *FAT32FileRead(LTPrt *p,U8 *cur_dir,U8 *filename,I64 *size,
  I64 *attr=NULL,BoolI8 raw,ArcCompressStruct **ac2)
{
  U8 *buf=NULL;
  LTDirEntry de;
  I64 c,blk_cnt,cur_dir_cluster;
  ArcCompressStruct *ac;
 
  PrtChk(p);
  if (size)
    *size=0;
  if (ac2)
    *ac2=NULL;
  if (attr)
    *attr=0;
  if (p->type!=PT_FAT32)
    PutDefine("ST_NOT_FAT32");
  else {
    try {
      PrtLock(p);
      cur_dir_cluster=Name2DirCluster(p,cur_dir);
      if (FAT32FileFind(p,cur_dir_cluster,filename,&de,FALSE,TRUE)) {
	if (attr)
	  *attr=de.attr;
	blk_cnt=(de.size+BLK_SIZE-1)>>BLK_SIZE_BITS;
	buf=MAlloc(blk_cnt<<BLK_SIZE_BITS+1);
	c=de.cluster;
	if (!c || c>=0x0FFFFFF8)
	  c=0x0FFFFFFF;
	else
	  c=RClustersBlks(p,buf,c,blk_cnt);
	if (FileAttr(de.name) & LT_ATTR_COMPRESSED && !raw) {
	  ac=buf;
	  if (attr)
	    *attr=FileAttr(de.name,*attr);
	  if (size)
	    *size=ac->expanded_size;
	  if (ac2) {
	    *ac2=ac;
	    buf=ExpandBuf(ac);
	  } else {
	    buf=ExpandBuf(ac);
	    Free(ac);
	  }
	} else {
	  buf[de.size]=0; //Terminate
	  if (size)
	    *size=de.size;
	}
	if (attr)
	  *attr=FileAttr(de.name,*attr);
      }
      PrtUnlock(p);
    } catch
      PrtUnlock(p);
  }
  return buf;
}

BoolI64 FAT32Cd(U8 *name,U64 cur_dir_cluster)
{
  LTDirEntry de;
  if (Fs->cur_prt->type!=PT_FAT32)
    PutDefine("ST_NOT_FAT32");
  else {
    if (FAT32FileFind(Fs->cur_prt,cur_dir_cluster,name,&de,TRUE,FALSE))
      return TRUE;
    else {
      "%s ",name;
      PutDefineErr("ST_FILE_NOT_FOUND");
    }
  }
  return FALSE;
}


U0 FAT32FreeClusters(LTPrt *p,I64 c)
{
  I64 next,saved_c=c;
  BoolI8 unlock,unlock_break;
  PrtChk(p);
  if (!c || c>=0x0FFFFFF8) return;
  if (p->type!=PT_FAT32)
    PutDefine("ST_NOT_FAT32");
  else {
    try {
      unlock_break=BreakLock;
      unlock=PrtLock(p);
      CleanMAPBuf(p);
      do {
	SetLTBuf(p,c,0);
	next=p->cur_map_blk[c&(BLK_SIZE/4-1)];
	p->cur_map_blk[c&(BLK_SIZE/4-1)]=0;
	LBts(p->buffered_map_dirty_bits,0);
	c=next;
      } while (c && c<0x0FFFFFF8);
      CleanMAPBuf(p,0);
 
      c=saved_c;
      do {
	SetLTBuf(p,c,1);
	next=p->cur_map_blk[c&(BLK_SIZE/4-1)];
	p->cur_map_blk[c&(BLK_SIZE/4-1)]=0;
	LBts(p->buffered_map_dirty_bits,0);
	c=next;
      } while (c && c<0x0FFFFFF8);
      CleanMAPBuf(p,1);
      if (unlock)
	PrtUnlock(p);
      if (unlock_break)
	BreakUnlock;
    } catch {
      if (unlock)
	PrtUnlock(p);
      if (unlock_break)
	BreakUnlock;
    }
  }
}

I64 FAT32AllocClusters(LTPrt *p,I64 c,I64 cnt)
{
  BoolI8 wrap_around=FALSE;
  I64 first=INVALID_CLUSTER,j,l;
  BoolI8 unlock,unlock_break;
 
  if (!cnt) return 0x0FFFFFFF;
  try {
    unlock_break=BreakLock;
    unlock=PrtLock(p);
    l=(p->size+p->offset-p->data)/p->spc-1;
    j=p->fis->most_recently_allocated;
    while (cnt--) {
      while (TRUE) {
	j++;
	if (j<1) j=1;
	if (j>=l) {
	  if (wrap_around)
	    Dbg;
	  j=1;
	  wrap_around=TRUE;
	}
	SetLTBuf(p,j);
	if (!p->cur_map_blk[j&(BLK_SIZE/4-1)])
	  break;
      }
      if (first==INVALID_CLUSTER)
	first=j;
      if (c>0 && c<l) {
	SetLTBuf(p,c);
	p->cur_map_blk[c&(BLK_SIZE/4-1)]=j;
	LBts(p->buffered_map_dirty_bits,0);
      }
      c=j;
    }
 
    if (c>0 && c<l) {
      SetLTBuf(p,c);
      p->cur_map_blk[c&(BLK_SIZE/4-1)]=0x0FFFFFFF;
      LBts(p->buffered_map_dirty_bits,0);
    }
    CleanMAPBuf(p);
 
    p->fis->most_recently_allocated=j;
    p->fis->free_clusters=-1;
    WBlks(p,p->fis,p->file_system_info_sect,1,TRUE);
  } catch {
    if (unlock)
      PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  }
  if (unlock)
    PrtUnlock(p);
  if (unlock_break)
    BreakUnlock;
  return first;
}


I64 FAT32AllocContiguousClusters(LTPrt *p,I64 cnt)
{
  I64 i,first=1;
  BoolI8 cont,unlock,unlock_break;
 
  if (!cnt) return 0x0FFFFFFF;
  try {
    unlock_break=BreakLock;
    unlock=PrtLock(p);
    while (TRUE) {
      first++;
      i=0;
      cont=TRUE;
      while (cont && i<cnt) {
	if ((first+i+1)*p->spc+p->data>p->size+p->offset)
	  Dbg;
	SetLTBuf(p,first+i);
	if (p->cur_map_blk[(first+i)&(BLK_SIZE/4-1)])
	  cont=FALSE;
	else
	  i++;
      }
      if (!cont)
	first=first+i;
      else {
	CleanMAPBuf(p);
 
	for (i=0;i<cnt;i++) {
	  SetLTBuf(p,first+i,0);
	  if (i+1==cnt)
	    p->cur_map_blk[(first+i)&(BLK_SIZE/4-1)]=0x0FFFFFFF;
	  else
	    p->cur_map_blk[(first+i)&(BLK_SIZE/4-1)]=first+i+1;
	  LBts(p->buffered_map_dirty_bits,0);
	}
	CleanMAPBuf(p,0);
 
	for (i=0;i<cnt;i++) {
	  SetLTBuf(p,first+i,1);
	  if (i+1==cnt)
	    p->cur_map_blk[(first+i)&(BLK_SIZE/4-1)]=0x0FFFFFFF;
	  else
	    p->cur_map_blk[(first+i)&(BLK_SIZE/4-1)]=first+i+1;
	  LBts(p->buffered_map_dirty_bits,0);
	}
	CleanMAPBuf(p,1);
	break;
      }
    }
  } catch {
    if (unlock)
      PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  }
  if (unlock)
    PrtUnlock(p);
  if (unlock_break)
    BreakUnlock;
  return first;
}

BoolI64 FAT32DirEntryNew(LTPrt *p,U8 *cur_dir,LTDirEntry *tempm,BoolI8 free_old_chain)
{
//See [::/LT/Doc/CutCorners.TXZ] ::/LT/Doc/CutCorners.TXZ.
  FAT32DirEntry *buf,*last_buf,*temp_buf,de[3];
  I64 i,attr,avail_cnt,de_cnt,c,
      cur_dir_entry,entries_per_cluster,
      cur_dir_cluster,xsum=0,last_dir_cluster=INVALID_CLUSTER;
  U8 ch;
  BoolI8 written=FALSE,unlock,unlock_break;
  LTDirEntry long_name;
  FAT32DirFill(&de,tempm,&de_cnt);
  MemSet(&long_name,0,sizeof(LTDirEntry));
  try {
    unlock_break=BreakLock;
    unlock=PrtLock(p);
    cur_dir_cluster=Name2DirCluster(p,cur_dir);
    buf     =MAlloc(BLK_SIZE*p->spc);
    last_buf=CAlloc(BLK_SIZE*p->spc);
    entries_per_cluster=FAT32_ENTRIES_PER_BLK*p->spc;
    RClusters(p,buf,cur_dir_cluster,1);
    cur_dir_entry=0;
    while (ch=*buf[cur_dir_entry].name) {
      attr=buf[cur_dir_entry].attr;
      if (ch!=0xE5 && attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
	LTDirLongNameFill(&long_name,&buf[cur_dir_entry],&xsum);
      else {
	avail_cnt=FAT32_ENTRIES_PER_BLK-cur_dir_entry & (FAT32_ENTRIES_PER_BLK-1);
	for (i=0;i<avail_cnt;i++)
	  if (*buf[cur_dir_entry+i].name!=0xE5) {
	    if (*buf[cur_dir_entry+i].name)
	      avail_cnt=i;
	    break;
	  }
	if (ch==0xE5 && !written && avail_cnt>=de_cnt) {
	  MemCpy(&buf[cur_dir_entry],&de,de_cnt*sizeof(FAT32DirEntry));
	  WBlks(p,&buf[cur_dir_entry & -FAT32_ENTRIES_PER_BLK],
	      p->data+cur_dir_cluster*p->spc
	      +cur_dir_entry/FAT32_ENTRIES_PER_BLK,1,TRUE);
	  cur_dir_entry+=de_cnt-1; //gets inc'ed
	  written=TRUE;
	} else if (ch!=0xE5 && !(attr&LT_ATTR_VOL_ID)) {
	  if (xsum!=FATNameXSum(buf[cur_dir_entry].name))
	    MemSet(&long_name,0,sizeof(LTDirEntry));
	  if (!*long_name.name)
	    FATFromName(long_name.name,buf[cur_dir_entry].name);
	    //Del old entry with same name
	  if (!StrCmp(long_name.name,tempm->name)) {
	    if (free_old_chain)
	      FAT32FreeClusters(p,buf[cur_dir_entry].cluster_lo+
		  buf[cur_dir_entry].cluster_hi<<16);
	    if (!written) {
	      MemCpy(&buf[cur_dir_entry],&de[de_cnt-1],sizeof(FAT32DirEntry));
	      WBlks(p,&buf[cur_dir_entry & -FAT32_ENTRIES_PER_BLK],
		  p->data+cur_dir_cluster*p->spc
		  +cur_dir_entry/FAT32_ENTRIES_PER_BLK,1,TRUE);
	      written=TRUE;
	    } else {
	      *buf[cur_dir_entry].name=0xE5;
	      i=1;
	      while (i<=cur_dir_entry &&
		  buf[cur_dir_entry-i].attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
		*buf[cur_dir_entry-i++].name=0xE5;
	      i--;
	      WBlks(p,&buf[(cur_dir_entry-i)&-FAT32_ENTRIES_PER_BLK],
		  p->data+cur_dir_cluster*p->spc
		  +(cur_dir_entry-i)/FAT32_ENTRIES_PER_BLK,
		  (i+FAT32_ENTRIES_PER_BLK)/FAT32_ENTRIES_PER_BLK,TRUE);
	      if (i==cur_dir_entry && last_dir_cluster!=INVALID_CLUSTER) {
		i=1;
		while (i<=entries_per_cluster &&
		    last_buf[entries_per_cluster-i].attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
		  *last_buf[entries_per_cluster-i++].name=0xE5;
		if (--i>0)
		  WBlks(p,&buf[(entries_per_cluster-i)&-FAT32_ENTRIES_PER_BLK],
		      p->data+last_dir_cluster*p->spc
		      +(entries_per_cluster-i)/FAT32_ENTRIES_PER_BLK,
		      (i+FAT32_ENTRIES_PER_BLK-1)/FAT32_ENTRIES_PER_BLK,TRUE);
	      }
	    }
	    break;
	  }
	}
	MemSet(&long_name,0,sizeof(LTDirEntry));
      }
      if (++cur_dir_entry==entries_per_cluster) {
	last_dir_cluster=cur_dir_cluster;
	temp_buf=buf; buf=last_buf; last_buf=temp_buf;
	c=ClusterNumNext(p,cur_dir_cluster);
	if (c==INVALID_CLUSTER || !c) {
	  c=AllocClusters(p,cur_dir_cluster,1,FALSE);
	  MemSet(buf,0,BLK_SIZE*p->spc);
	  WClusters(p,buf,c,1,TRUE);
	} else
	  RClusters(p,buf,c,1);
	cur_dir_cluster=c;
	cur_dir_entry=0;
      }
    }
    if (!written) {
      avail_cnt=FAT32_ENTRIES_PER_BLK-cur_dir_entry & (FAT32_ENTRIES_PER_BLK-1);
      if (avail_cnt<de_cnt) {
	for (i=0;i<avail_cnt;i++)
	  *buf[cur_dir_entry+i].name=0xE5;
	WBlks(p,&buf[cur_dir_entry &-FAT32_ENTRIES_PER_BLK],
	    p->data+cur_dir_cluster*p->spc
	    +cur_dir_entry/FAT32_ENTRIES_PER_BLK,1,TRUE);
	cur_dir_entry+=avail_cnt;
	if (cur_dir_entry==entries_per_cluster) {
	  last_dir_cluster=cur_dir_cluster;
	  temp_buf=buf; buf=last_buf; last_buf=temp_buf;
	  cur_dir_cluster=AllocClusters(p,cur_dir_cluster,1);
	  cur_dir_entry=0;
	  MemSet(buf,0,BLK_SIZE*p->spc);
	  WClusters(p,buf,cur_dir_cluster,1,TRUE);
	}
      }
      MemCpy(&buf[cur_dir_entry],&de,de_cnt*sizeof(FAT32DirEntry));
      WBlks(p,&buf[cur_dir_entry &-FAT32_ENTRIES_PER_BLK],
	  p->data+cur_dir_cluster*p->spc+
	  cur_dir_entry/FAT32_ENTRIES_PER_BLK,1,TRUE);
      cur_dir_entry+=de_cnt;
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=AllocClusters(p,cur_dir_cluster,1);
	MemSet(buf,0,BLK_SIZE*p->spc);
	WClusters(p,buf,cur_dir_cluster,1,TRUE);
      } else {
	MemSet(&buf[cur_dir_entry],0,sizeof(FAT32DirEntry));
	WBlks(p,&buf[cur_dir_entry &-FAT32_ENTRIES_PER_BLK],
	    p->data+cur_dir_cluster*p->spc
	    +cur_dir_entry/FAT32_ENTRIES_PER_BLK,1,TRUE);
      }
    }
    Free(last_buf);
    Free(buf);
    if (unlock)
      PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  } catch {
    if (unlock)
      PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  }
  return FALSE;
}

BoolI64 FAT32FilesDel(LTPrt *p,U8 *cur_dir,U8 *files_find_mask,U64 fuf_flags,
		     BoolI8 del_dir,BoolI8 print_msg)
{
  FAT32DirEntry *buf,*last_buf,*temp_buf;
  I64 i,attr,xsum=0,last_dir_cluster=INVALID_CLUSTER,
  cur_dir_entry,entries_per_cluster,cur_dir_cluster;
  U8 ch;
  BoolI8 unlock_break;
  LTDirEntry long_name;
  MemSet(&long_name,0,sizeof(LTDirEntry));
  try {
    unlock_break=BreakLock;
    PrtLock(p);
    cur_dir_cluster=Name2DirCluster(p,cur_dir);
    buf     =MAlloc(BLK_SIZE*p->spc);
    last_buf=CAlloc(BLK_SIZE*p->spc);
    entries_per_cluster=FAT32_ENTRIES_PER_BLK*p->spc;
    RClusters(p,buf,cur_dir_cluster,1);
    cur_dir_entry=0;
    while (ch=*buf[cur_dir_entry].name) {
      attr=buf[cur_dir_entry].attr;
      if (ch!=0xE5 && ch!='.') {
	if (attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
	  LTDirLongNameFill(&long_name,&buf[cur_dir_entry],&xsum);
	else {
	  if (!(attr & LT_ATTR_VOL_ID) &&
	  (del_dir || !(attr & LT_ATTR_DIR))) {
	    if (xsum!=FATNameXSum(buf[cur_dir_entry].name))
	      MemSet(&long_name,0,sizeof(LTDirEntry));
	    if (!*long_name.name)
	      FATFromName(long_name.name,buf[cur_dir_entry].name);
	    if (FilesFindMatch(files_find_mask,long_name.name,fuf_flags)) {
	      if (print_msg) {
		PutDefine("ST_DELETE");
		"" long_name.name;
		NewLine;
	      }
	      *buf[cur_dir_entry].name=0xE5;
	      i=1;
	      while (i<=cur_dir_entry &&
	      buf[cur_dir_entry-i].attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
		*buf[cur_dir_entry-i++].name=0xE5;
	      i--;
	      WBlks(p,&buf[(cur_dir_entry-i)&-FAT32_ENTRIES_PER_BLK],
	      p->data+cur_dir_cluster*p->spc
	      +(cur_dir_entry-i)/FAT32_ENTRIES_PER_BLK,
	      (i+FAT32_ENTRIES_PER_BLK)/FAT32_ENTRIES_PER_BLK,TRUE);
	      if (i==cur_dir_entry && last_dir_cluster!=INVALID_CLUSTER) {
		i=1;
		while (i<=entries_per_cluster &&
		last_buf[entries_per_cluster-i].attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
		  *last_buf[entries_per_cluster-i++].name=0xE5;
		if (--i>0)
		  WBlks(p,&buf[(entries_per_cluster-i)&-FAT32_ENTRIES_PER_BLK],
		  p->data+last_dir_cluster*p->spc
		  +(entries_per_cluster-i)/FAT32_ENTRIES_PER_BLK,
		  (i+FAT32_ENTRIES_PER_BLK-1)/FAT32_ENTRIES_PER_BLK,TRUE);
	      }
	      FAT32FreeClusters(p,buf[cur_dir_entry].cluster_lo+
	      buf[cur_dir_entry].cluster_hi<<16);
	    }
	  }
	  MemSet(&long_name,0,sizeof(LTDirEntry));
	}
      } else
	MemSet(&long_name,0,sizeof(LTDirEntry));
      if (++cur_dir_entry==entries_per_cluster) {
	last_dir_cluster=cur_dir_cluster;
	cur_dir_cluster=ClusterNumNext(p,cur_dir_cluster,1);
	temp_buf=buf; buf=last_buf; last_buf=temp_buf;
	RClusters(p,buf,cur_dir_cluster,1);
	cur_dir_entry=0;
      }
    }
    Free(buf);
    Free(last_buf);
    PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  } catch {
    PrtUnlock(p);
    if (unlock_break)
      BreakUnlock;
  }
  return FALSE;
}

I64 FAT32FileWrite(LTPrt *p,U8 *cur_dir,U8 *name,U8 *buf,I64 size,
		LTDate date=0,U64 attr=0,BoolI8 raw=FALSE,
		ArcCompressStruct **ac2=NULL)
{
  LTDirEntry de;
  I64 c=0,blk_cnt;
  ArcCompressStruct *ac=NULL;
  BoolI8 contiguous;
  MemSet(&de,0,sizeof(LTDirEntry));
  if (ac2) *ac2=NULL;
  if (size<0) size=0;
  if (!date)
    date=GetCurTimeLTDate;
  if (p->type!=PT_FAT32)
    PutDefine("ST_NOT_FAT32");
  else if (!LTFileNameTo(de.name,name))
    PutDefine("ST_INVALID_FILENAME");
  else {
    FAT32FilesDel(p,cur_dir,de.name,0,FALSE,FALSE);
    attr=FileAttr(name,attr);
    if (attr & LT_ATTR_CONTIGUOUS)
      contiguous=TRUE;
    else
      contiguous=FALSE;
    if (attr & LT_ATTR_COMPRESSED && !raw) {
      ac=CompressBuf(buf,size);
      buf=ac;
      size=ac->compressed_size;
    }
    de.size=size;
    if (blk_cnt=(size+BLK_SIZE-1)>>BLK_SIZE_BITS)
      c=AllocClusters(p,0,(blk_cnt+p->spc-1)/p->spc,contiguous);
    else
      c=0x0FFFFFFF;
    de.cluster=c;
    de.attr=attr;
    de.datetime=date;
    if (blk_cnt)
      WClustersBlks(p,buf,c,blk_cnt,FALSE);
    FAT32DirEntryNew(p,cur_dir,&de,TRUE);
    if (ac2)
      *ac2=ac;
    else
      Free(ac);
  }
  return c;
}

LTDirEntry *FAT32FilesFind(U8 *files_find_mask,U64 fuf_flags,LTDirEntry *parent=NULL,U64 *_dir_size=NULL)
{
  LTPrt *p=Fs->cur_prt;
  FAT32DirEntry *buf;
  I64 attr,xsum=0,dir_size=0,sub_dir_size,
  cur_dir_cluster,cur_dir_entry,entries_per_cluster;
  U8 ch;
  LTDirEntry *result=NULL,*tempm,long_name;
  try {
    MemSet(&long_name,0,sizeof(LTDirEntry));
    PrtLock(p);
    cur_dir_cluster=Name2DirCluster(p,Fs->cur_dir);
    buf=MAlloc(BLK_SIZE*p->spc);
    entries_per_cluster=FAT32_ENTRIES_PER_BLK*p->spc;
    RClusters(Fs->cur_prt,buf,cur_dir_cluster,1);
    dir_size+=Fs->cur_prt->spc*BLK_SIZE;
    cur_dir_entry=0;
    while (ch=*buf[cur_dir_entry].name) {
      attr=buf[cur_dir_entry].attr;
      if (ch!=0xE5) {
	if (attr&LT_ATTR_LONG_NAME_MASK==LT_ATTR_LONG_NAME)
	  LTDirLongNameFill(&long_name,&buf[cur_dir_entry],&xsum);
	else {
	  if (!(attr&LT_ATTR_VOL_ID)) {
	    tempm=MAlloc(sizeof(LTDirEntry));
	    if (xsum==FATNameXSum(buf[cur_dir_entry].name))
	      MemCpy(tempm,&long_name,sizeof(LTDirEntry));
	    else
	      MemSet(tempm,0,sizeof(LTDirEntry));
	    if (FAT32LTDirFill(tempm,&buf[cur_dir_entry])) {
	      tempm->parent=parent;
	      if (Bt(&fuf_flags,FUf_RECURSE) && attr&LT_ATTR_DIR && *tempm->name!='.') {
		tempm->next=result;
		result=tempm;
		tempm->full_name=DirNameAbs(tempm->name);
		PrtUnlock(p);
		if (Cd(tempm->name)) {
		  tempm->sub=FAT32FilesFind(files_find_mask,fuf_flags,tempm,&sub_dir_size);
		  tempm->size=tempm->expanded_size=sub_dir_size;
		  Cd("..");
		}
		PrtLock(p);
	      } else if ((attr&LT_ATTR_DIR || !Bt(&fuf_flags,FUf_JUST_DIRS)) &&
	      !(Bt(&fuf_flags,FUf_RECURSE) && *tempm->name=='.' && attr&LT_ATTR_DIR) &&
		FilesFindMatch(files_find_mask,tempm->name,fuf_flags)) {
		tempm->next=result;
		result=tempm;
		tempm->full_name=FileNameAbs(tempm->name);
	      } else
		LTDirEntryDel(tempm);
	    } else
	      LTDirEntryDel(tempm);
	  }
	  MemSet(&long_name,0,sizeof(LTDirEntry));
	}
      } else
	MemSet(&long_name,0,sizeof(LTDirEntry));
      if (++cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=ClusterNumNext(p,cur_dir_cluster);
	if (cur_dir_cluster==INVALID_CLUSTER)
	  break;
	else {
	  RClusters(p,buf,cur_dir_cluster,1);
	  dir_size+=Fs->cur_prt->spc*BLK_SIZE;
	  cur_dir_entry=0;
	}
      }
    }
    Free(buf);
    PrtUnlock(p);
  } catch
    PrtUnlock(p);
  if (_dir_size)
    *_dir_size=dir_size;
  return result;
}


BoolI64 FAT32MkDir(LTPrt *p,U8 *cur_dir,U8 *name)
{
  I64 size=p->spc<<BLK_SIZE_BITS,
  cur_dir_cluster=Name2DirCluster(p,cur_dir);
  U8 *buf=CAlloc(size);
  LTDirEntry dLT;
  FAT32DirEntry *dFAT=buf;
  I64 c;
  BoolI8 unlock_break;
 
  try {
    unlock_break=BreakLock;
    c=FileWrite(name,buf,size,0,LT_ATTR_DIR,FALSE);
    MemSet(&dLT,0,sizeof(LTDirEntry));
    dLT.attr=LT_ATTR_DIR;
    *dLT.name='.';
    dLT.name[1]=0;
    dLT.cluster=c;
    dLT.size=0;
    dLT.datetime=GetCurTimeLTDate;
    FAT32DirFill(dFAT,&dLT);
    dFAT++;
 
    MemSet(&dLT,0,sizeof(LTDirEntry));
    dLT.attr=LT_ATTR_DIR;
    *dLT.name='.';
    dLT.name[1]='.';
    dLT.name[2]=0;
    dLT.cluster=cur_dir_cluster;
    dLT.size=0;
    dLT.datetime=GetCurTimeLTDate;
    FAT32DirFill(dFAT,&dLT);
    WClusters(p,buf,c,1,TRUE);
    Free(buf);
    if (unlock_break)
      BreakUnlock;
  } catch
    if (unlock_break)
      BreakUnlock;
  return TRUE;
}

