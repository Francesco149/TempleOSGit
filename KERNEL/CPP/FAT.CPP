class mbr_partition_entry
{
  BYTE active;	//0x80=active  0x00=inactive
  BYTE start_head;
  WORD start_cyl;
  BYTE type;
  BYTE end_head;
  WORD end_cyl;
  DWORD offset;  //Sectors between MBR and first sector
  DWORD size;	 //Sectors in partition
};

class master_boot_record
{
  BYTE boot_code[446];
  mbr_partition_entry p[4];
  WORD signature;  //55AA
};

class FAT32_boot_record
{
  BYTE jump_and_nop[3];
  BYTE OEM_name[8];
  WORD bytes_per_sector;
  BYTE sectors_per_cluster;
  WORD reserved_sectors;
  BYTE copies_of_FAT;
  WORD max_root_dir_entries;	  //Not used
  WORD old_sectors_in_partition;  //Not used
  BYTE media_descriptor;	  //F8 for hard disk
  WORD old_sectors_per_FAT;	  //Not used
  WORD sectors_per_track;
  WORD num_heads;
  DWORD hidden_sectors;
  DWORD sectors;
  DWORD sectors_per_FAT;
  WORD flags;
  WORD version;
  DWORD root_cluster;
  WORD file_system_info_sector;
  WORD backup_boot_sector;
  BYTE reserved[12];
  BYTE logical_drive_num;
  BYTE unused;
  BYTE extended_signature; //0x29
  DWORD serial_number;
  char volume_name[11];
  BYTE FAT_name[8];
  BYTE code[420];
  WORD signature; //0xAA55
};

class FAT12_boot_record
{
  BYTE jump_and_nop[3];
  BYTE SysName[8];
  WORD BytesPerSector;
  BYTE SectorsPerCluster;
  WORD ReservedSectors;
  BYTE FATcount;
  WORD MaxRootEntries;
  WORD TotalSectors1;
  BYTE MediaDescriptor;
  WORD SectorsPerFAT;
  WORD SectorsPerTrack;
  WORD HeadCount;
  DWORD HiddenSectors;
  DWORD TotalSectors2;
  BYTE DriveNumber;
  BYTE Reserved1;
  BYTE ExtBootSignature;
  DWORD VolumeSerial;
  BYTE VolumeLabel[11];
  BYTE Reserved2[12];
  BYTE code[444];
  WORD signature; //0xAA55
};

class FAT32_file_info_sector
{
  DWORD signature1; //52 52 61 41
  BYTE	unknown[480];
  DWORD signature2; //72 72 41 61
  DWORD free_clusters; // -1 if unknown
  DWORD most_recently_allocated;
  BYTE	reserved[12];
  WORD	unknown2;
  WORD	signature3; //55 AA
}


class FAT12_dir_entry
{
  char name[11];
  BYTE attr;
  BYTE Reserved[10];
  WORD Time;
  WORD Date;
  WORD cluster_lo;
  DWORD size;
};

class FAT32_dir_entry
{
  char name[11];
  BYTE attr;
  BYTE NTres;
  BYTE CrtTimeTenth;
  WORD CrtTime;
  WORD CrtDate;
  WORD LstAccDate;
  WORD cluster_hi;
  WORD WrtTime;
  WORD WRtDate;
  WORD cluster_lo;
  DWORD size;
}

class my_dir_entry
{
  my_dir_entry *next,*sub;
  char name[16];
  BYTE attr;
  DWORD cluster;
  DWORD size;
}

#define mpef_locked 0
class my_partition_entry
{
  DWORD offset;
  DWORD size;
  DWORD type;
  DWORD file_system_info_sector;
  DWORD FAT1;
  DWORD FAT2;
  DWORD root_cluster;
  DWORD spc; //sectors_per_cluster
  DWORD data;
  DWORD flags;
  DWORD owning_task;

  DWORD num_buffered_FAT_blocks;
  DWORD cur_FAT_blk_num;
  DWORD *cur_FAT_blk;
  BYTE	*buffered_FAT_dirty_bits;

  bool fis_dirty;
  FAT32_file_info_sector *cur_FAT32_fis;

  bool rblocks(BYTE *buf,DWORD block,int count);
  bool wblocks(BYTE *buf,DWORD block,int count);
};


DWORD num_partitions=0;
my_partition_entry local_partitions[32];

void alloc_FAT_buffer(int partition,int num_blks)
{
  my_partition_entry *p=&local_partitions[partition];
  p->num_buffered_FAT_blocks=num_blks;
  p->cur_FAT_blk=rmalloc(BLOCK_SIZE*num_blks);
  p->buffered_FAT_dirty_bits=rmalloc((num_blks+7)/8);
  memset(p->buffered_FAT_dirty_bits,0,(num_blks+7)/8);
  p->fis_dirty=false;
}

void clean_FAT_buffer(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD i;

  if (p->fis_dirty) {
    p->fis_dirty=false;
    p->wblocks(p->cur_FAT32_fis,p->file_system_info_sector,1);
  }
  for (i=0;i<p->num_buffered_FAT_blocks;i++)
    if (bt(p->buffered_FAT_dirty_bits,i))
      p->wblocks(p->cur_FAT_blk+i*BLOCK_SIZE,p->FAT1+i+p->cur_FAT_blk_num,1);
  for (i=0;i<p->num_buffered_FAT_blocks;i++)
    if (btr(p->buffered_FAT_dirty_bits,i))
      p->wblocks(p->cur_FAT_blk+i*BLOCK_SIZE,p->FAT2+i+p->cur_FAT_blk_num,1);
}

void lock_drive(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  if (p->owning_task!=fs->absolute_address) {
    while (bts(&p->flags,mpef_locked))
      swap_in_next;
    if (p->type==1)
      get_FDC0;
    p->owning_task=fs->absolute_address;
  }
}

void unlock_drive(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  p->owning_task=0;
  if (p->type==1)
    free_FDC0;
  btr(&p->flags,mpef_locked);
}

void change_FDC0_diskette(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT12_boot_record br12;

  p->type  =1;
  lock_drive(partition);
  rfree(p->cur_FAT_blk);
  rfree(p->buffered_FAT_dirty_bits);
  rfree(FDC0_cache_blocks);
  rfree(FDC0_in_cache_bits);

  FDC0_cache_blocks =rmalloc(2880*BLOCK_SIZE);
  memset(FDC0_cache_blocks,0,2880*BLOCK_SIZE);
  FDC0_in_cache_bits=rmalloc(2880/8+1);
  memset(FDC0_in_cache_bits,0,2880/8+1);

  p->rblocks=&FDC0_rblocks;
  p->wblocks=&FDC0_wblocks;
  p->rblocks(&br12,0,1);
  p->size  =2880;
  p->spc=br12.SectorsPerCluster;
  p->offset=0;
  p->FAT1=1;
  p->FAT2=1+br12.SectorsPerFAT;
  p->root_cluster=p->FAT2+br12.SectorsPerFAT;
  p->data=p->root_cluster+(br12.MaxRootEntries*sizeof(FAT12_dir_entry)+BLOCK_SIZE-1)/BLOCK_SIZE
	  -2*p->spc;
  p->root_cluster=p->root_cluster-p->data;
  alloc_FAT_buffer(partition,br12.SectorsPerFAT);
  p->cur_FAT_blk_num=0;
  p->rblocks(p->cur_FAT_blk,p->FAT1,p->num_buffered_FAT_blocks);
  unlock_drive(partition);
  if (!monitor_floppy_tss)
    monitor_floppy_tss=spawn(&monitor_floppy_tsk,"FLOPPY MANAGER",PAGE_SIZE);
}

void read_partition_info() //This is incomplete
{
  DWORD i,j,offset=0;
  bool primary=true;
  master_boot_record mbr;
  my_partition_entry *p;
  FAT32_boot_record br32;
  char ch;

  spaces(200*200);
  *DC_COL=0;

  memset(local_partitions,0,sizeof(my_partition_entry)*32);
  num_partitions=0;
  ? "Drive A: Floppy\r\n";
  ? "Drive B: Floppy (Unsupported)\r\n";
  num_partitions=2;

  while (true) {
    IDE_rblocks(&mbr,offset,1);
    if (mbr.signature!=0xAA55)
      return;
    j=4;
    for (i=0;i<4;i++) {
      if (mbr.p[i].active==0x80 || (!primary && i==0)) {
	p=&local_partitions[num_partitions];
	p->rblocks=&IDE_rblocks;
	p->wblocks=&IDE_wblocks;
	p->offset=mbr.p[i].offset+offset;
	p->size  =mbr.p[i].size;
	p->type  =mbr.p[i].type;
	p->flags = 0;
	p->owning_task=0;
	p->cur_FAT_blk_num=0xFFFFFFFF;
	ch=num_partitions+'A';
	? "Drive ",ch,": Blocks:",p->offset,"-",p->offset+p->size-1;
	if (p->type==0xB)
	  ? "  FAT32\r\n";
	else
	  ? "  Unsupported\r\n";
	if (p->offset>=IDE_MAX_SECTOR)
	  return;
	if (p->type==0x0B) {
	  p->rblocks(&br32,p->offset,1);
	  p->file_system_info_sector=p->offset+br32.file_system_info_sector;
	  p->FAT1=p->offset+br32.reserved_sectors;
	  p->FAT2=p->FAT1+br32.sectors_per_FAT;
	  p->data=p->FAT2+br32.sectors_per_FAT-2*br32.sectors_per_cluster; //Starts at cluster 2
	  p->spc=br32.sectors_per_cluster;
	  p->root_cluster=br32.root_cluster;
	  alloc_FAT_buffer(num_partitions,1);
	  p->cur_FAT32_fis=rmalloc(sizeof(FAT32_file_info_sector));
	  p->rblocks(p->cur_FAT32_fis,p->file_system_info_sector,1);
	}
	num_partitions++;
	j=i+1;
      }
    }
    primary=false;
    if (j>=4)
      return;
    if (mbr.p[j].offset==0)
      return;
    offset=mbr.p[j].offset+offset;
    if (offset>=IDE_MAX_SECTOR)
      return;
  }
}


bool rblocks(int partition,BYTE *buf, DWORD block, int count,bool unlock=true)
{
  bool result;
  lock_drive(partition);
  result=local_partitions[partition].rblocks(buf,block,count);
  if (unlock)
    unlock_drive(partition);
  return result;
}

bool wblocks(int partition,BYTE *buf, DWORD block, DWORD count,bool unlock=true)
{
  bool result;
  lock_drive(partition);
  result=local_partitions[partition].wblocks(buf,block,count);
  if (unlock)
    unlock_drive(partition);
  return result;
}


DWORD next_FAT12_cluster_num(int partition,int c)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_index=(3*c)/2;
  BYTE *FAT=p->cur_FAT_blk;
  if (c<2) { //root_dir
    c++;
    if (c==0)
      return 0xFFF;
    else
      return c;
  }
  if (c & 1) {
    c=FAT[FAT_index+1];
    c=(c<<4) | ((FAT[FAT_index] & 0xF0)>>4);
  } else {
    c=FAT[FAT_index+1];
    c=((c & 0x0F)<<8) | FAT[FAT_index];
  }
  if (c>=0xFF8)
    return 0xFFF;
  else
    return c;
}

DWORD next_FAT32_cluster_num(int partition,DWORD c)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num=c/(BLOCK_SIZE/4);
  if (p->cur_FAT_blk_num!=FAT_blk_num) {
    p->rblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
    p->cur_FAT_blk_num=FAT_blk_num;
  }
  c=p->cur_FAT_blk[c-FAT_blk_num*(BLOCK_SIZE/4)];
  if (c>=0xFFFFFFF)
    return 0xFFFFFFF;
  else
    return c;
}

bool rcluster(int partition,BYTE *buf,DWORD c)
{
  my_partition_entry *p=&local_partitions[partition];
  return p->rblocks(buf,p->data+c*p->spc,p->spc);
}


#define FAT12_attr_read_only 1
#define FAT12_attr_hidden    2
#define FAT12_attr_system    4
#define FAT12_attr_volume_id 8
#define FAT12_attr_directory 0x10
#define FAT12_attr_archive   0x20

#define FAT32_attr_read_only 1
#define FAT32_attr_hidden    2
#define FAT32_attr_system    4
#define FAT32_attr_volume_id 8
#define FAT32_attr_directory 0x10
#define FAT32_attr_archive   0x20
#define FAT32_attr_long_name 0x0F

#define my_attr_read_only 1
#define my_attr_hidden	  2
#define my_attr_system	  4
#define my_attr_volume_id 8
#define my_attr_directory 0x10
#define my_attr_archive   0x20


void to_FAT_name(char *src,char *dst)
{
  DWORD i;
  char *ptr;
  if (!strcmp(src,"..")) {
    strcpy(dst,src);
    i=2;
  } else if (!strcmp(src,".")) {
    strcpy(dst,src);
    i=1;
  } else {
    ptr=src;
    i=0;
    while (i<8 && *ptr && *ptr!='.') {
      dst[i++]=*ptr++;
    }
    while (i<8)
      dst[i++]=0x20;
    if (*ptr=='.') ptr++;
    while (i<11 && *ptr) {
      dst[i++]=*ptr++;
    }
  }
  while (i<11)
    dst[i++]=0x20;
}

void from_FAT_name(char *src,char *dst)
{
  int i,j,k;
  for (j=7;j>=0 && src[j]==0x20;j--);
  k=0;
  for(i=0;i<=j;i++)
    dst[k++]=src[i];
  if (*src!='.')
    dst[k++]='.';
  for (j=10;j>=8 && src[j]==0x20;j--);
  if (j==7) {
    if (k==1)
       dst[k]=0;
    else {
      if (dst[0]=='.')
	dst[k]=0;
      else
	dst[k-1]=0;
    }
  } else {
    for(i=8;i<=j;i++)
      dst[k++]=src[i];
    dst[k++]=0;
  }
}

bool find_FAT12_file(int partition,DWORD cur_dir_cluster,char *name,
		     FAT12_dir_entry *result,
		     bool dir_only,bool file_only,bool unlock=true)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT12_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster;
  bool cont=true,cont2=false;
  char *ptr,*ptr2,dname[12];
  DWORD i;
  BYTE ch;

  if (p->type!=0x01)
    ? systext(st_not_FAT12);
  else {
    lock_drive(partition);
    to_FAT_name(name,dname);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_cluster=BLOCK_SIZE*p->spc/sizeof(FAT12_dir_entry);
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT12_cluster_num(partition,cur_dir_cluster);
	if (cur_dir_cluster==0xFFF)
	  cont=false;
	else {
	  rcluster(partition,buf,cur_dir_cluster);
	  cur_dir_entry=0;
	}
      }
      if (cont) {
	ch=buf[cur_dir_entry].name[0];
	if (!ch) {
	  cont=false;
	} else if (ch!=0xE5) {
	  if (dir_only && !(buf[cur_dir_entry].attr & FAT12_attr_directory))
	    cont2=false;
	  else if (file_only && (buf[cur_dir_entry].attr & FAT12_attr_directory))
	    cont2=false;
	  else
	    cont2=true;
	  ptr=dname;
	  ptr2=buf[cur_dir_entry].name;
	  for (i=0;i<11 && cont2;i++)
	    if (*ptr++!=*ptr2++)
	      cont2=false;
	  if (cont2) {
	    memcpy(result,&buf[cur_dir_entry],sizeof(FAT12_dir_entry));
	    if (unlock)
	      unlock_drive(partition);
	    return true;
	  }
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    if (unlock)
      unlock_drive(partition);
  }
  return false;
}

bool find_FAT32_file(int partition,DWORD cur_dir_cluster,char *name,FAT32_dir_entry *result,
		     bool dir_only,bool file_only,bool unlock=true)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster;
  bool cont=true,cont2=false;
  char *ptr,*ptr2,dname[12];
  DWORD i;
  BYTE ch;

  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    lock_drive(partition);
    to_FAT_name(name,dname);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_cluster=BLOCK_SIZE*p->spc/sizeof(FAT32_dir_entry);
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT32_cluster_num(partition,cur_dir_cluster);
	if (cur_dir_cluster==0xFFFFFFF)
	  cont=false;
	else {
	  rcluster(partition,buf,cur_dir_cluster);
	  cur_dir_entry=0;
	}
      }
      if (cont) {
	ch=buf[cur_dir_entry].name[0];
	if (!ch) {
	  cont=false;
	} else if (ch!=0xE5) {
	  if (dir_only && !(buf[cur_dir_entry].attr & FAT32_attr_directory))
	    cont2=false;
	  else if (file_only && (buf[cur_dir_entry].attr & FAT32_attr_directory))
	    cont2=false;
	  else
	    cont2=true;
	  ptr=dname;
	  ptr2=buf[cur_dir_entry].name;
	  for (i=0;i<11 && cont2;i++)
	    if (*ptr++!=*ptr2++)
	      cont2=false;
	  if (cont2) {
	    memcpy(result,&buf[cur_dir_entry],sizeof(FAT32_dir_entry));
	    if (unlock)
	      unlock_drive(partition);
	    return true;
	  }
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    if (unlock)
      unlock_drive(partition);
  }
  return false;
}

BYTE *read_FAT12_file(int partition,DWORD cur_dir_cluster,char *filename,DWORD *size=NULL,bool unlock=true)
{
  BYTE *buf=NULL,*ptr;
  FAT12_dir_entry de;
  my_partition_entry *p=&local_partitions[partition];
  DWORD c,cluster_size=BLOCK_SIZE*p->spc,num_clusters;

  if (p->type!=1)
    ? systext(st_not_FAT12);
  else {
    if (size)
      *size=0;
    lock_drive(partition);
    if (!find_FAT12_file(partition,cur_dir_cluster,filename,&de,false,true,false))
      ? systext(st_file_not_found);
    else {
      num_clusters=(de.size+1+cluster_size-1)/cluster_size;
      buf=malloc(num_clusters*cluster_size);
      c=de.cluster_lo;
      ptr=buf;
      while (c!=0x0FFF) {
        *PROGRESS0=(*PROGRESS0 & 0xFFFF)+65536*(num_clusters--);
	rcluster(partition,ptr,c);
	ptr=ptr+cluster_size;
	c=next_FAT12_cluster_num(partition,c);
      }
      buf[de.size]=0; //Terminate
      if (size)
	*size=de.size;
    }
    if (unlock)
      unlock_drive(partition);
  }
  *PROGRESS0=0;
  return buf;
}

BYTE *read_FAT32_file(int partition,DWORD cur_dir_cluster,char *filename,DWORD *size=NULL,bool unlock=true)
{
  BYTE *buf=NULL,*ptr;
  FAT32_dir_entry de;
  my_partition_entry *p=&local_partitions[partition];
  DWORD c,cluster_size=BLOCK_SIZE*p->spc,num_clusters;

  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    if (size)
      *size=0;
    lock_drive(partition);
    if (!find_FAT32_file(partition,cur_dir_cluster,filename,&de,false,true,false))
      ? systext(st_file_not_found);
    else {
      num_clusters=(de.size+1+cluster_size-1)/cluster_size;
      buf=malloc(num_clusters*cluster_size);
      c=de.cluster_lo+(de.cluster_hi<<16);
      ptr=buf;
      while (c!=0x0FFFFFFF) {
	*PROGRESS0=(*PROGRESS0 & 0xFFFF)+65536*(num_clusters--);
	rcluster(partition,ptr,c);
	ptr=ptr+cluster_size;
	c=next_FAT32_cluster_num(partition,c);
      }
      buf[de.size]=0; //Terminate
      if (size)
	*size=de.size;
    }
    if (unlock)
      unlock_drive(partition);
  }
  *PROGRESS0=0;
  return buf;
}

void cd_FAT12(char *name)
{
  FAT12_dir_entry de;
  if (local_partitions[fs->current_partition].type!=1)
    ? systext(st_not_FAT12);
  else {
    if (find_FAT12_file(fs->current_partition,fs->current_dir_cluster,
			name,&de,true,false))
      fs->current_dir_cluster=de.cluster_lo;
    else
      ? systext(st_file_not_found);
  }
}

void cd_FAT32(char *name)
{
  FAT32_dir_entry de;
  if (local_partitions[fs->current_partition].type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    if (find_FAT32_file(fs->current_partition,fs->current_dir_cluster,
			name,&de,true,false))
      fs->current_dir_cluster=de.cluster_lo+(de.cluster_hi << 16);
    else
      ? systext(st_file_not_found);
  }
}

//This writes a cluster (typically 8 blocks) and numbered
//from the start of the data area of the partition.
void wcluster(int partition,BYTE *buf,DWORD c)
{
  my_partition_entry *p=&local_partitions[partition];
  p->wblocks(buf,p->data+c*p->spc,p->spc);
}

//This writes several clusters
void wclusters(int partition,BYTE *buf,DWORD c,DWORD num)
{
  my_partition_entry *p=&local_partitions[partition];
  while (num--) {
    *PROGRESS0=(*PROGRESS0&0xFFFF)+65536*num+0x80000000;
    if (p->type==1) {
      if (c>=0xFFF)
        unrecoverable_system_error(st_invalid_cluster);
    } else if (p->type=0xB) {
      if (c>=0xFFFFFF8)
        unrecoverable_system_error(st_invalid_cluster);
    }
    wcluster(partition,buf,c);
    buf=buf+BLOCK_SIZE*p->spc;
    if (p->type==1) 
      c=next_FAT12_cluster_num(partition,c);
    else if (p->type=0xB) 
      c=next_FAT32_cluster_num(partition,c);
  }
  *PROGRESS0=0;
}

//This frees a chain of clusters.  It needs to be
//made more efficient by doing one write for
//all modifications to the same FAT block
void free_FAT12_clusters(int partition,DWORD c,bool unlock)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,FAT_index,next=1;
  BYTE *FAT=p->cur_FAT_blk;
  bool cont=true;
  if (p->type!=1)
    ? systext(st_not_FAT12);
  else {
    lock_drive(partition);
    while (next!=0x0FFF && next!=0) {
      FAT_index=3*c/2;
      FAT_blk_num=FAT_index/BLOCK_SIZE;
      next=next_FAT12_cluster_num(partition,c);
      if (c &1) {
	FAT[FAT_index+1]=0;
	FAT[FAT_index]=FAT[FAT_index] & 0x0F;
      } else {
	FAT[FAT_index+1]=FAT[FAT_index+1] & 0xF0;
	FAT[FAT_index]=0;
      }
      bts(p->buffered_FAT_dirty_bits,FAT_blk_num);
      if ((FAT_index & (BLOCK_SIZE-1))==BLOCK_SIZE-1)
	bts(p->buffered_FAT_dirty_bits,FAT_blk_num+1);
      c=next;
    }
    clean_FAT_buffer(partition);
    if (unlock)
      unlock_drive(partition);
  }
}

void free_FAT32_clusters(int partition,DWORD c,bool unlock)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,next=1;
  bool cont=true;
  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    lock_drive(partition);
    while (next!=0x0FFFFFFF && next!=0) {
      FAT_blk_num=c/(BLOCK_SIZE/4);
      next=next_FAT32_cluster_num(partition,c);  //Loads in FAT
      p->cur_FAT_blk[c-FAT_blk_num*(BLOCK_SIZE/4)]=0;
      p->wblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
      p->wblocks(p->cur_FAT_blk,p->FAT2+FAT_blk_num,1);
      c=next;
    }
    if (unlock)
      unlock_drive(partition);
  }
}

//This is a low level routine you generally don't need
//to call yourself.  save_FAT32_file calls this automatically
int alloc_FAT12_clusters(int partition,int c,DWORD num,bool unlock=true)  //c=0 means first cluster in chain
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num=0,first=0,FAT_index=0,most_recently_allocated=1;
  bool cont;
  BYTE *FAT=p->cur_FAT_blk;
  if (p->type!=0x01) {
    ? systext(st_not_FAT12);
    while (true);
  }
  lock_drive(partition);
  if (c<0) {
    c++;
    if (c*p->spc+num>=0)
      unrecoverable_system_error(st_out_of_root_dir_space);
    else
      return c;
  }
  while (num--) {
    cont=true;
    while (cont) {
      most_recently_allocated++;
      if (most_recently_allocated*p->spc+p->data>=p->size+p->offset)
        unrecoverable_system_error(st_out_of_drive_space);
      FAT_index=3*most_recently_allocated/2;
      FAT_blk_num=FAT_index/BLOCK_SIZE;
      if (most_recently_allocated & 1) {
	if (!FAT[FAT_index+1] && !(FAT[FAT_index] & 0xF0)) {
	  FAT[FAT_index+1]=0xFF;
	  FAT[FAT_index]=FAT[FAT_index] | 0xF0;
	  cont=false;
	}
      } else {
	if (!FAT[FAT_index] && !(FAT[FAT_index+1] & 0xF)) {
	  FAT[FAT_index+1]=FAT[FAT_index+1] | 0x0F;
	  FAT[FAT_index]=0xFF;
	  cont=false;
	}
      }
      if (!cont) {
	bts(p->buffered_FAT_dirty_bits,FAT_blk_num);
	if ((FAT_index & (BLOCK_SIZE-1))==BLOCK_SIZE-1)
	  bts(p->buffered_FAT_dirty_bits,FAT_blk_num+1);
      }
    }
    if (!first)
      first=most_recently_allocated;
    if (c) {
      FAT_index=3*c/2;
      FAT_blk_num=FAT_index/BLOCK_SIZE;
      if (c & 1) {
	FAT[FAT_index+1]=(most_recently_allocated>>4) & 0xFF;
	FAT[FAT_index]=(FAT[FAT_index] & 0xF) | ((most_recently_allocated & 0xF) << 4);
      } else {
	FAT[FAT_index+1]=(FAT[FAT_index+1] & 0xF0) | ((most_recently_allocated>>8) & 0x0F);
	FAT[FAT_index]=most_recently_allocated & 0xFF;
      }
      bts(p->buffered_FAT_dirty_bits,FAT_blk_num);
      if (FAT_index & (BLOCK_SIZE-1)==BLOCK_SIZE-1)
	bts(p->buffered_FAT_dirty_bits,FAT_blk_num+1);
    }
    c=most_recently_allocated;
  }
  clean_FAT_buffer(partition);
  if (unlock)
    unlock_drive(partition);
  return first;
}


void mark_bad_FAT12_sectors(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,FAT_index,track;
  int blk,c;
  BYTE *FAT=p->cur_FAT_blk;
  BYTE *buf=malloc(FDC_DISK_SECTORS*BLOCK_SIZE);
  if (p->type!=1)
    ? systext(st_not_FAT12);
  else {
    lock_drive(partition);
    for (track=0;track<p->size/FDC_DISK_SECTORS;track++) {
      ? "Track:",track;
      if (p->rblocks(buf,track*FDC_DISK_SECTORS,FDC_DISK_SECTORS))
	? " Okay\r\n";
      else {
	? " Bad\r\n";
        for (blk=track*FDC_DISK_SECTORS;
	     blk<(track+1)*FDC_DISK_SECTORS;blk++) {
	  c=(blk-p->data)/p->spc;
	  if (c<0)
	    ? "Bad System Sector, disk not usable.\r\n";
	  else {
            FAT_index=3*c/2;
            FAT_blk_num=FAT_index/BLOCK_SIZE;
            if (c &1) {
	      FAT[FAT_index+1]=0xFF;
	      FAT[FAT_index]=FAT[FAT_index] || 0xF0;
	    }  else {
	      FAT[FAT_index+1]=FAT[FAT_index+1] || 0x0F;
	      FAT[FAT_index]=0xFF;
	    }
            bts(p->buffered_FAT_dirty_bits,FAT_blk_num);
            if ((FAT_index & (BLOCK_SIZE-1))==BLOCK_SIZE-1)
	      bts(p->buffered_FAT_dirty_bits,FAT_blk_num+1);
	  }
	}
      }
    }
    clean_FAT_buffer(partition);
    unlock_drive(partition);
  }
  free(buf);
}

//This searches for a contiguous space of blocks
//You typically don't need to call this yourself, unless
//you are setting-up an area for a database.
DWORD alloc_contiguous_FAT12_clusters(int partition,DWORD num, bool unlock=true)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,FAT_index,first=1,i;
  BYTE *FAT=p->cur_FAT_blk;
  bool cont;
  if (!num) {
    ? "Attempt to alloc zero\r\n";
    while (true);
  }
  if (p->type!=0x01) {
    ? systext(st_not_FAT12);
    while (true);
  }
  lock_drive(partition);
  while (true) {
    first++;
    i=0;
    cont=true;
    while (cont && i<num) {
      if ((i+first)*p->spc+p->data>=p->size+p->offset)
	unrecoverable_system_error(st_out_of_drive_space);
      FAT_index=3*(first+i)/2;
      if ((first+i) & 1) {
	if (FAT[FAT_index+1] || (FAT[FAT_index] & 0xF0))
	  cont=false;
	else
	  i++;
      } else {
	if (FAT[FAT_index] || (FAT[FAT_index+1] & 0xF))
	  cont=false;
	else
	  i++;
      }
    }
    if (!cont)
      first=first+i;
    else {
      for (i=0;i<num;i++) {
	FAT_index=3*(first+i)/2;
	FAT_blk_num=FAT_index/BLOCK_SIZE;
	bts(p->buffered_FAT_dirty_bits,FAT_blk_num);
	if (FAT_index & (BLOCK_SIZE-1)==BLOCK_SIZE-1)
	  bts(p->buffered_FAT_dirty_bits,FAT_blk_num+1);
	if (i+1==num) {
	  if ((first+i) & 1) {
	    FAT[FAT_index+1]=0xFF;
	    FAT[FAT_index]=FAT[FAT_index] | 0xF0;
	  } else {
	    FAT[FAT_index+1]=FAT[FAT_index+1] | 0x0F;
	    FAT[FAT_index]=0xFF;
	  }
	} else {
	  if ((first+i) & 1) {
	    FAT[FAT_index+1]=((first+i+1)>>4) & 0xFF;
	    FAT[FAT_index]=(FAT[FAT_index] & 0xF) | (((first+i+1) & 0xF) << 4);
	  } else {
	    FAT[FAT_index+1]=(FAT[FAT_index+1] & 0xF0) | (((first+i+1)>>8) & 0x0F);
	    FAT[FAT_index]=(first+i+1) & 0xFF;
	  }
	}
      }
      clean_FAT_buffer(partition);
      if (unlock)
	unlock_drive(partition);
      return first;
    }
  }
}

 //c=0 means first cluster in chain
DWORD alloc_FAT32_clusters(int partition,DWORD c,DWORD num,bool unlock=true)  //c=0 means first cluster in chain
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,first=0;
  bool cont;
  if (p->type!=0x0B) {
    ? systext(st_not_FAT32);
    while (true);
  }
  lock_drive(partition);
  while (num--) {
    cont=true;
    while (cont) {
      p->cur_FAT32_fis->most_recently_allocated++;
      if (p->cur_FAT32_fis->most_recently_allocated*p->spc+p->data>=p->size+p->offset)
	unrecoverable_system_error(st_out_of_drive_space);
      p->cur_FAT32_fis->free_clusters=0xFFFFFFFF;
      FAT_blk_num=p->cur_FAT32_fis->most_recently_allocated/(BLOCK_SIZE/4);
      if (p->cur_FAT_blk_num!=FAT_blk_num) {
	p->rblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	p->cur_FAT_blk_num=FAT_blk_num;
      }
      if (!p->cur_FAT_blk[p->cur_FAT32_fis->most_recently_allocated-
	FAT_blk_num*(BLOCK_SIZE/4)]) {
	p->cur_FAT_blk[p->cur_FAT32_fis->most_recently_allocated-
	  FAT_blk_num*(BLOCK_SIZE/4)]=0x0FFFFFFF;
	p->wblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	p->wblocks(p->cur_FAT_blk,p->FAT2+FAT_blk_num,1);
	p->wblocks(p->cur_FAT32_fis,p->file_system_info_sector,1);
	cont=false;
      }
    }
    if (!first)
      first=p->cur_FAT32_fis->most_recently_allocated;
    if (c) {
      FAT_blk_num=c/(BLOCK_SIZE/4);
      if (p->cur_FAT_blk_num!=FAT_blk_num) {
	p->rblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	p->cur_FAT_blk_num=FAT_blk_num;
      }
      p->cur_FAT_blk[c-FAT_blk_num*(BLOCK_SIZE/4)]=p->cur_FAT32_fis->most_recently_allocated;
      p->wblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
      p->wblocks(p->cur_FAT_blk,p->FAT2+FAT_blk_num,1);
    }
    c=p->cur_FAT32_fis->most_recently_allocated;
  }
  if (unlock)
    unlock_drive(partition);
  return first;
}

DWORD alloc_contiguous_FAT32_clusters(int partition,DWORD num, bool unlock=true)
{
  my_partition_entry *p=&local_partitions[partition];
  DWORD FAT_blk_num,first=1,i;
  bool cont;
  if (!num) {
    ? "Attempt to alloc zero\r\n";
    while (true);
  }
  if (p->type!=0x0B) {
    ? systext(st_not_FAT32);
    while (true);
  }
  lock_drive(partition);
  while (true) {
    first++;
    i=0;
    cont=true;
    while (cont && i<num) {
      if ((first+i)*p->spc+p->data>=p->size+p->offset)
	unrecoverable_system_error(st_out_of_drive_space);
      FAT_blk_num=(first+i)/(BLOCK_SIZE/4);
      if (p->cur_FAT_blk_num!=FAT_blk_num) {
	p->rblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	p->cur_FAT_blk_num=FAT_blk_num;
      }
      if (p->cur_FAT_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)])
	cont=false;
      else
	i++;
    }
    if (!cont)
      first=first+i;
    else {
      for (i=0;i<num;i++) {
	FAT_blk_num=(first+i)/(BLOCK_SIZE/4);
	if (p->cur_FAT_blk_num!=FAT_blk_num) {
	  p->rblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	  p->cur_FAT_blk_num=FAT_blk_num;
	}
	if (i+1==num)
	  p->cur_FAT_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)]=0x0FFFFFFF;
	else
	  p->cur_FAT_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)]=first+i+1;
	if ((first+i+1)/(BLOCK_SIZE/4)!=p->cur_FAT_blk_num) {
	  p->wblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
	  p->wblocks(p->cur_FAT_blk,p->FAT2+FAT_blk_num,1);
	}
      }
      p->wblocks(p->cur_FAT_blk,p->FAT1+FAT_blk_num,1);
      p->wblocks(p->cur_FAT_blk,p->FAT2+FAT_blk_num,1);
      if (unlock)
	unlock_drive(partition);
      return first;
    }
  }
}

//You typically don't need to call this yourself.
//save_FAT32_file calls it automatically.
bool new_FAT12_dir_entry(int partition,int cur_dir_cluster,FAT12_dir_entry *de)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT12_dir_entry *buf;
  int c,cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true,cont2,written=false;
  char *ptr,*ptr2,dname[12];
  DWORD i;
  BYTE ch;

  if (p->type!=0x01)
    ? systext(st_not_FAT12);
  else {
    lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_block=BLOCK_SIZE/sizeof(FAT12_dir_entry);
    entries_per_cluster=entries_per_block*p->spc;
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	c=next_FAT12_cluster_num(partition,cur_dir_cluster);
	if (c==0xFFF) {
	  c=alloc_FAT12_clusters(partition,cur_dir_cluster,1,false);
	  memset(buf,0,BLOCK_SIZE*p->spc);
	  wcluster(partition,buf,c);
	} else
          rcluster(partition,buf,c);
	cur_dir_cluster=c;
	cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch) {
	if (!written) {
	  memcpy(&buf[cur_dir_entry],de,sizeof(FAT12_dir_entry));
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  cur_dir_entry++;
	  if (cur_dir_entry==entries_per_cluster) {
            cur_dir_cluster=alloc_FAT12_clusters(partition,cur_dir_cluster,1);
	    cur_dir_entry=0;
	    memset(buf,0,BLOCK_SIZE*p->spc);
	    wcluster(partition,buf,cur_dir_entry);
	  } else {
	    buf[cur_dir_entry].name[0]=0;
	    p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	       p->data+cur_dir_cluster*p->spc
	       +cur_dir_entry/entries_per_block,1);
	  }
  	  written=true;
	}
	cont=false;
      } else if (ch==0xE5) {
	if (!written) {
	  memcpy(&buf[cur_dir_entry],de,sizeof(FAT12_dir_entry));
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	    p->data+cur_dir_cluster*p->spc
	    +cur_dir_entry/entries_per_block,1);
	  written=true;
	}
      } else {
	cont2=true;
	ptr=de->name;
	ptr2=buf[cur_dir_entry].name;
	for (i=0;i<11 && cont2;i++)
	  if (*ptr++!=*ptr2++)
	    cont2=false;
	if (cont2) { //Delete ond entry with same name
	  buf[cur_dir_entry].name[0]=0xE5;
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  free_FAT12_clusters(partition,buf[cur_dir_entry].cluster_lo,false);
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    unlock_drive(partition);
  }
  return false;
}

bool new_FAT32_dir_entry(int partition,DWORD cur_dir_cluster,FAT32_dir_entry *de)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT32_dir_entry *buf;
  DWORD c,cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true,cont2,written=false;
  char *ptr,*ptr2,dname[12];
  DWORD i;
  BYTE ch;

  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_block=BLOCK_SIZE/sizeof(FAT32_dir_entry);
    entries_per_cluster=entries_per_block*p->spc;
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	c=next_FAT32_cluster_num(partition,cur_dir_cluster);
	if (c==0xFFFFFFF) {
	  c=alloc_FAT32_clusters(partition,cur_dir_cluster,1,false);
	  memset(buf,0,BLOCK_SIZE*p->spc);
	  wcluster(partition,buf,c);
	} else
          rcluster(partition,buf,c);
	cur_dir_cluster=c;
	cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch) {
	if (!written) {
	  memcpy(&buf[cur_dir_entry],de,sizeof(FAT32_dir_entry));
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  cur_dir_entry++;
	  if (cur_dir_entry==entries_per_cluster) {
            cur_dir_cluster=alloc_FAT32_clusters(partition,cur_dir_cluster,1);
	    cur_dir_entry=0;
	    memset(buf,0,BLOCK_SIZE*p->spc);
	    wcluster(partition,buf,cur_dir_entry);
	  } else {
  	    buf[cur_dir_entry].name[0]=0;
	    p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	       p->data+cur_dir_cluster*p->spc
	       +cur_dir_entry/entries_per_block,1);
	  }
	  written=true;
	}
	cont=false;
      } else if (ch==0xE5) {
	if (!written) {
	  memcpy(&buf[cur_dir_entry],de,sizeof(FAT32_dir_entry));
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	    p->data+cur_dir_cluster*p->spc
	    +cur_dir_entry/entries_per_block,1);
	  written=true;
	}
      } else {
	cont2=true;
	ptr=de->name;
	ptr2=buf[cur_dir_entry].name;
	for (i=0;i<11 && cont2;i++)
	  if (*ptr++!=*ptr2++)
	    cont2=false;
	if (cont2) { //Delete ond entry with same name
	  buf[cur_dir_entry].name[0]=0xE5;
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  free_FAT32_clusters(partition,buf[cur_dir_entry].cluster_lo+
	    (buf[cur_dir_entry].cluster_hi<<16),false);
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    unlock_drive(partition);
  }
  return false;
}

//This is a low level routine.
//del() calls this
bool del_FAT12_files(int partition,DWORD cur_dir_cluster,char *wild)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT12_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true;
  char cbuf[16];
  BYTE ch;

  if (p->type!=0x01)
    ? systext(st_not_FAT12);
  else {
    lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_block=BLOCK_SIZE/sizeof(FAT32_dir_entry);
    entries_per_cluster=entries_per_block*p->spc;
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT12_cluster_num(partition,cur_dir_cluster);
	rcluster(partition,buf,cur_dir_cluster);
	cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch)
	cont=false;
      else if (ch!=0xE5) {
	from_FAT_name(buf[cur_dir_entry].name,cbuf);
	if (wild_filename_list(wild,cbuf)) {
	  ? systext(st_delete),cbuf,"\r\n";
	  buf[cur_dir_entry].name[0]=0xE5;
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  free_FAT12_clusters(partition,buf[cur_dir_entry].cluster_lo,false);
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    unlock_drive(partition);
  }
  return false;
}

bool del_FAT32_files(int partition,DWORD cur_dir_cluster,char *wild)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true;
  char cbuf[16];
  BYTE ch;

  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_block=BLOCK_SIZE/sizeof(FAT32_dir_entry);
    entries_per_cluster=entries_per_block*p->spc;
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT32_cluster_num(partition,cur_dir_cluster);
	rcluster(partition,buf,cur_dir_cluster);
	cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch)
	cont=false;
      else if (ch!=0xE5) {
	from_FAT_name(buf[cur_dir_entry].name,cbuf);
	if (wild_filename_list(wild,cbuf)) {
	  ? systext(st_delete),cbuf,"\r\n";
	  buf[cur_dir_entry].name[0]=0xE5;
	  p->wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
	     p->data+cur_dir_cluster*p->spc
	     +cur_dir_entry/entries_per_block,1);
	  free_FAT32_clusters(partition,buf[cur_dir_entry].cluster_lo+
	    (buf[cur_dir_entry].cluster_hi<<16),false);
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    unlock_drive(partition);
  }
  return false;
}

//Save text file calls this
void save_FAT12_file(int partition,DWORD cur_dir_cluster,char *name,BYTE *buf,DWORD size,bool contiguous=true)
{
  FAT12_dir_entry de;
  my_partition_entry *p=&local_partitions[partition];
  DWORD c,cluster_count,
	cluster_size=BLOCK_SIZE*p->spc;
  if (p->type!=1)
    ? systext(st_not_FAT12);
  else {
    to_FAT_name(name,de.name);
    de.size=size;
    cluster_count=(size+cluster_size-1)/cluster_size;
    if (contiguous)
      c=alloc_contiguous_FAT12_clusters(partition,cluster_count);
    else
      c=alloc_FAT12_clusters(partition,0,cluster_count);
    de.cluster_lo=c;
    de.attr=0;
    memset(de.Reserved,0,10);
    de.Date=0;
    de.Time=0;
    new_FAT12_dir_entry(partition,cur_dir_cluster,&de);
    wclusters(partition,buf,c,cluster_count);
  }
}

void save_FAT32_file(int partition,DWORD cur_dir_cluster,char *name,BYTE *buf,DWORD size,bool contiguous=true)
{
  FAT32_dir_entry de;
  my_partition_entry *p=&local_partitions[partition];
  DWORD c,cluster_count,
	cluster_size=BLOCK_SIZE*p->spc;
  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    to_FAT_name(name,de.name);
    de.size=size;
    cluster_count=(size+cluster_size-1)/cluster_size;
    if (contiguous)
      c=alloc_contiguous_FAT32_clusters(partition,cluster_count);
    else
      c=alloc_FAT32_clusters(partition,0,cluster_count);
    de.cluster_lo=c & 0xFFFF;
    de.cluster_hi=c >> 16;
    de.attr=0;
    de.CrtDate=0;
    de.CrtTime=0;
    de.CrtTimeTenth=0;
    de.LstAccDate=0;
    de.NTres=0;
    de.WRtDate=0;
    de.WrtTime=0;
    new_FAT32_dir_entry(partition,cur_dir_cluster,&de);
    wclusters(partition,buf,c,cluster_count);
  }
}

DWORD recurse_ff12;
my_dir_entry *find_FAT12_files(int partition,DWORD cur_dir_cluster,char *wild,bool recurse,
			       bool lock)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT12_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster;
  bool cont=true;
  char *ptr,cbuf[16];
  DWORD i;
  BYTE ch;
  my_dir_entry *result=NULL,*tempm;
  my_dir_entry *my_find_FAT12_files(int partition,DWORD cur_dir_cluster,char *wild,bool recurse,
			       bool lock);

  my_find_FAT12_files=recurse_ff12;
  if (p->type!=0x01)
    ? systext(st_not_FAT12);
  else {
    if (lock)
      lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_cluster=BLOCK_SIZE*p->spc/sizeof(FAT12_dir_entry);
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT12_cluster_num(partition,cur_dir_cluster);
	if (cur_dir_cluster==0xFFF)
	  cont=false;
	else {
	  rcluster(partition,buf,cur_dir_cluster);
	  cur_dir_entry=0;
	}
      }
      if (cont) {
	ch=buf[cur_dir_entry].name[0];
	if (!ch) {
	  cont=false;
	} else if (ch!=0xE5) {
	  from_FAT_name(buf[cur_dir_entry].name,cbuf);
	  if (recurse && (buf[cur_dir_entry].attr & FAT12_attr_directory) && cbuf[0]!='.') {
	    tempm=malloc(sizeof(my_dir_entry));
	    tempm->next=result;
	    result=tempm;
	    strcpy(tempm->name,cbuf);
	    tempm->cluster=buf[cur_dir_entry].cluster_lo;
	    tempm->size=buf[cur_dir_entry].size;
	    tempm->attr=buf[cur_dir_entry].attr;
	    tempm->sub=my_find_FAT12_files(partition,tempm->cluster,wild,recurse,false);
	  } else if ((!(buf[cur_dir_entry].attr & FAT12_attr_directory) || !recurse) &&
		     wild_filename_list(wild,cbuf)) {
	    tempm=malloc(sizeof(my_dir_entry));
	    tempm->next=result;
	    result=tempm;
	    strcpy(tempm->name,cbuf);
	    tempm->cluster=buf[cur_dir_entry].cluster_lo;
	    tempm->size=buf[cur_dir_entry].size;
	    tempm->attr=buf[cur_dir_entry].attr;
	    tempm->sub=NULL;
	  }
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    if (lock)
      unlock_drive(partition);
  }
  return result;
}
recurse_ff12=&find_FAT12_files;



DWORD recurse_ff32;
my_dir_entry *find_FAT32_files(int partition,DWORD cur_dir_cluster,char *wild,bool recurse,
			       bool lock)
{
  my_partition_entry *p=&local_partitions[partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_entry,entries_per_cluster;
  bool cont=true;
  char *ptr,cbuf[16];
  DWORD i;
  BYTE ch;
  my_dir_entry *result=NULL,*tempm;
  my_dir_entry *my_find_FAT32_files(int partition,DWORD cur_dir_cluster,char *wild,bool recurse,
			       bool lock);

  my_find_FAT32_files=recurse_ff32;
  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    if (lock)
      lock_drive(partition);
    buf=malloc(BLOCK_SIZE*p->spc);
    entries_per_cluster=BLOCK_SIZE*p->spc/sizeof(FAT32_dir_entry);
    rcluster(partition,buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
	cur_dir_cluster=next_FAT32_cluster_num(partition,cur_dir_cluster);
	if (cur_dir_cluster==0xFFFFFFF)
	  cont=false;
	else {
	  rcluster(partition,buf,cur_dir_cluster);
	  cur_dir_entry=0;
	}
      }
      if (cont) {
	ch=buf[cur_dir_entry].name[0];
	if (!ch) {
	  cont=false;
	} else if (ch!=0xE5) {
	  from_FAT_name(buf[cur_dir_entry].name,cbuf);
	  if (recurse && (buf[cur_dir_entry].attr & FAT32_attr_directory) && cbuf[0]!='.') {
	    tempm=malloc(sizeof(my_dir_entry));
	    tempm->next=result;
	    result=tempm;
	    strcpy(tempm->name,cbuf);
	    tempm->cluster=buf[cur_dir_entry].cluster_lo+
			  (buf[cur_dir_entry].cluster_hi<<16);
	    tempm->size=buf[cur_dir_entry].size;
	    tempm->attr=buf[cur_dir_entry].attr;
	    tempm->sub=my_find_FAT32_files(partition,tempm->cluster,wild,recurse,false);
	  } else if ((!(buf[cur_dir_entry].attr & FAT32_attr_directory) || !recurse) &&
		     wild_filename_list(wild,cbuf)) {
	    tempm=malloc(sizeof(my_dir_entry));
	    tempm->next=result;
	    result=tempm;
	    strcpy(tempm->name,cbuf);
	    tempm->cluster=buf[cur_dir_entry].cluster_lo+
			  (buf[cur_dir_entry].cluster_hi<<16);
	    tempm->size=buf[cur_dir_entry].size;
	    tempm->attr=buf[cur_dir_entry].attr;
	    tempm->sub=NULL;
	  }
	}
      }
      cur_dir_entry++;
    }
    free(buf);
    if (lock)
      unlock_drive(partition);
  }
  return result;
}
recurse_ff32=&find_FAT32_files;


my_dir_entry *find_files(int partition,DWORD cur_dir_cluster,char *wild,bool recurse)
{
  my_partition_entry *p=&local_partitions[partition];
  my_dir_entry *result;

  if (p->type==0x01)
    result=find_FAT12_files(partition,cur_dir_cluster,wild,recurse,true);
  else if (p->type==0xB)
    result=find_FAT32_files(partition,cur_dir_cluster,wild,recurse,true);
  else {
    ? systext(st_file_system_not_supported);
    return;
  }
  return result;
}


void dir_FAT(int partition,DWORD cur_dir_cluster,char *wild)
{
  my_partition_entry *p=&local_partitions[partition];
  int i;
  my_dir_entry *tempm1=NULL,*tempm2;

  tempm1=find_files(partition,cur_dir_cluster,wild,false);
  if (tempm1) {
    ? "Directory of ",fs->current_directory,"\r\n";
    spaces(16); ? "  SIZE	 CLUSTER\r\n";
  } else {
    ? systext(st_no_matching_entries);
    return;
  }
  while (tempm1) {
    tempm2=tempm1->next;
    ? tempm1->name;
    spaces(16-strlen(tempm1->name));
    ? tempm1->size," ",tempm1->cluster,"\r\n";
    free(tempm1);
    tempm1=tempm2;
  }
}


BYTE *read_file(char *filename,DWORD *size=NULL)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  if (p->type==1)
    return read_FAT12_file(fs->current_partition,fs->current_dir_cluster,filename,size);
  else if (p->type==0x0B)
    return read_FAT32_file(fs->current_partition,fs->current_dir_cluster,filename,size);
}


defext(ext_wblocks,&wblocks);
defext(ext_rblocks,&rblocks);
defext(ext_irq0E,&irq0E_handler);
defext(ext_read_file,&read_file);


