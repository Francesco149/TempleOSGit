#define BLOCK_SIZE 512
DWORD drive_heads=16;
DWORD drive_sectors=0x3F;
DWORD MAX_SECTOR=drive_heads*drive_sectors*0x10000;
DWORD MAX_READS_AT_ONCE=0x80;
DWORD MAX_WRITES_AT_ONCE=1;

//The following definitions are for low-level disk access
//If you are interested in making CD or Floppy access they
//might be handy.  Also,to access more than 32GB they may 0be necessary
/* ATA/ATAPI Commands pre T13 Spec */
#define WIN_NOP				0x00
#define CFA_REQ_EXT_ERROR_CODE		0x03 /* CFA Request Extended Error Code */
#define WIN_SRST			0x08 /* ATAPI soft reset command */
#define WIN_DEVICE_RESET		0x08
#define WIN_RESTORE			0x10
#define WIN_READ			0x20 /* 28-Bit */
#define WIN_READ_EXT			0x24 /* 48-Bit */
#define WIN_READDMA_EXT			0x25 /* 48-Bit */
#define WIN_READDMA_QUEUED_EXT		0x26 /* 48-Bit */
#define WIN_READ_NATIVE_MAX_EXT		0x27 /* 48-Bit */
#define WIN_MULTREAD_EXT		0x29 /* 48-Bit */
#define WIN_WRITE			0x30 /* 28-Bit */
#define WIN_WRITE_EXT			0x34 /* 48-Bit */
#define WIN_WRITEDMA_EXT		0x35 /* 48-Bit */
#define WIN_WRITEDMA_QUEUED_EXT		0x36 /* 48-Bit */
#define WIN_SET_MAX_EXT			0x37 /* 48-Bit */
#define CFA_WRITE_SECT_WO_ERASE		0x38 /* CFA Write Sectors without erase */
#define WIN_MULTWRITE_EXT		0x39 /* 48-Bit */
#define WIN_WRITE_VERIFY		0x3C /* 28-Bit */
#define WIN_VERIFY			0x40 /* 28-Bit - Read Verify Sectors */
#define WIN_VERIFY_EXT			0x42 /* 48-Bit */
#define WIN_FORMAT			0x50
#define WIN_INIT			0x60
#define WIN_SEEK			0x70
#define CFA_TRANSLATE_SECTOR		0x87 /* CFA Translate Sector */
#define WIN_DIAGNOSE			0x90
#define WIN_SPECIFY			0x91 /* set drive geometry translation */
#define WIN_DOWNLOAD_MICROCODE		0x92
#define WIN_STANDBYNOW2			0x94
#define WIN_SETIDLE2			0x97
#define WIN_CHECKPOWERMODE2		0x98
#define WIN_SLEEPNOW2			0x99
#define WIN_PACKETCMD			0xA0 /* Send a packet command. */
#define WIN_PIDENTIFY			0xA1 /* identify ATAPI device	*/
#define WIN_QUEUED_SERVICE		0xA2
#define WIN_SMART			0xB0 /* self-monitoring and reporting */
#define CFA_ERASE_SECTORS       	0xC0
#define WIN_MULTREAD			0xC4 /* read sectors using multiple mode*/
#define WIN_MULTWRITE			0xC5 /* write sectors using multiple mode */
#define WIN_SETMULT			0xC6 /* enable/disable multiple mode */
#define WIN_READDMA_QUEUED		0xC7 /* read sectors using Queued DMA transfers */
#define WIN_READDMA			0xC8 /* read sectors using DMA transfers */
#define WIN_WRITEDMA			0xCA /* write sectors using DMA transfers */
#define WIN_WRITEDMA_QUEUED		0xCC /* write sectors using Queued DMA transfers */
#define CFA_WRITE_MULTI_WO_ERASE	0xCD /* CFA Write multiple without erase */
#define WIN_GETMEDIASTATUS		0xDA	
#define WIN_DOORLOCK			0xDE /* lock door on removable drives */
#define WIN_DOORUNLOCK			0xDF /* unlock door on removable drives */
#define WIN_STANDBYNOW1			0xE0
#define WIN_IDLEIMMEDIATE		0xE1 /* force drive to become "ready" */
#define WIN_STANDBY             	0xE2 /* Set device in Standby Mode */
#define WIN_SETIDLE1			0xE3
#define WIN_READ_BUFFER			0xE4 /* force read only 1 sector */
#define WIN_CHECKPOWERMODE1		0xE5
#define WIN_SLEEPNOW1			0xE6
#define WIN_FLUSH_CACHE			0xE7
#define WIN_WRITE_BUFFER		0xE8 /* force write only 1 sector */
#define WIN_FLUSH_CACHE_EXT		0xEA /* 48-Bit */
#define WIN_IDENTIFY			0xEC /* ask drive to identify itself	*/
#define WIN_MEDIAEJECT			0xED
#define WIN_IDENTIFY_DMA		0xEE /* same as WIN_IDENTIFY, but DMA */
#define WIN_SETFEATURES			0xEF /* set special drive features */
#define EXABYTE_ENABLE_NEST		0xF0
#define WIN_SECURITY_SET_PASS		0xF1
#define WIN_SECURITY_UNLOCK		0xF2
#define WIN_SECURITY_ERASE_PREPARE	0xF3
#define WIN_SECURITY_ERASE_UNIT		0xF4
#define WIN_SECURITY_FREEZE_LOCK	0xF5
#define WIN_SECURITY_DISABLE		0xF6
#define WIN_READ_NATIVE_MAX		0xF8 /* return the native maximum address */
#define WIN_SET_MAX			0xF9
#define DISABLE_SEAGATE			0xFB
#define WIN_MULTREAD			0xC4 /* read sectors using multiple mode*/

/* Bits of HD_STATUS */
#define ERR_STAT		0x01
#define INDEX_STAT		0x02
#define ECC_STAT		0x04	/* Corrected error */
#define DRQ_STAT		0x08
#define SEEK_STAT		0x10
#define WRERR_STAT		0x20
#define READY_STAT		0x40
#define BUSY_STAT		0x80

#define BAD_R_STAT		(BUSY_STAT   | ERR_STAT)
#define BAD_W_STAT		(BAD_R_STAT  | WRERR_STAT)
#define BAD_STAT		(BAD_R_STAT  | DRQ_STAT)
#define DRIVE_READY		(READY_STAT  | SEEK_STAT)
#define DATA_READY		(DRQ_STAT)

#define IDE_DATA_REG    0x1f0
#define IDE_FEATURE_REG 0x1f1
#define IDE_NSECTOR_REG 0x1f2
#define IDE_SECTOR_REG  0x1f3
#define IDE_LCYL_REG    0x1f4
#define IDE_HCYL_REG    0x1f5
#define IDE_SELECT_REG  0x1f6
#define IDE_STATUS      0x1f7
#define IDE_COMMAND     0x1f7
#define IDE_CONTROL_REG 0x3f6


void irq0E_handler()
{
  bts(SYS_FLAGS,SYSF_IDE1_DONE);
  outp(0xa0,0x20);
  outp(0x20,0x20);
}

void ide_select_block(DWORD unit,DWORD block,DWORD count)
{
  DWORD sect,head,cyl,track;
  outp(IDE_CONTROL_REG,0x8);
  track=block / drive_sectors;
  sect =block % drive_sectors+1;
  outp(IDE_SECTOR_REG,sect);
  head =track % drive_heads;
  cyl  =track / drive_heads;
  outp(IDE_FEATURE_REG,0);
  outp(IDE_NSECTOR_REG,count);
  outp(IDE_LCYL_REG,cyl);
  outp(IDE_HCYL_REG,cyl>>8);
  outp(IDE_SELECT_REG,0xa0|(unit<<4)|head);
}

void ide_wait_for_drive()
{
  while (bts(SYS_FLAGS,SYSF_IDE1_BUSY))
    swap_in_next;

  btr(SYS_FLAGS,SYSF_IDE1_DONE);
}


void ide_read_blocks2(BYTE *buf; DWORD count)
{
  DWORD i,words_avail,sectors_avail;
  while (!btr(SYS_FLAGS,SYSF_IDE1_DONE))
    swap_in_next;
  while (count) {
    while (inp(IDE_STATUS) & DATA_READY!=DATA_READY)
      swap_in_next;
    while (!(sectors_avail=count-inp(IDE_NSECTOR_REG)))
      swap_in_next;
    words_avail=sectors_avail*BLOCK_SIZE/2;
    rep_insw(buf,words_avail,IDE_DATA_REG);
    buf=buf+words_avail*2;
    count=count-sectors_avail;
  }

  btr(SYS_FLAGS,SYSF_IDE1_BUSY);
}

void ide_read_blocks(BYTE *buf; DWORD block; DWORD count)
{
  DWORD i,words_avail,sectors_avail;
  ide_wait_for_drive();
  ide_select_block(0,block,count);
  outp(IDE_COMMAND,WIN_MULTREAD);
  ide_read_blocks2(buf,count);
}

void rblocks(BYTE *buf, DWORD block, int count)
{
  DWORD l;
  while (count>0) {
    l=count;
    if (l>MAX_READS_AT_ONCE)
      l=MAX_READS_AT_ONCE;
    ide_read_blocks(buf,block,l);
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
}


class mbr_partition_entry
{
  BYTE active;  //0x80=active  0x00=inactive
  BYTE start_head;
  WORD start_cyl;
  BYTE type;
  BYTE end_head;
  WORD end_cyl;
  DWORD offset;  //Sectors between MBR and first sector
  DWORD size;    //Sectors in partition
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
  WORD max_root_dir_entries;      //Not used
  WORD old_sectors_in_partition;  //Not used
  BYTE media_descriptor;          //F8 for hard disk
  WORD old_sectors_per_FAT;       //Not used
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

class FAT32_file_info_sector
{
  DWORD signature1; //52 52 61 41
  BYTE  unknown[480];
  DWORD signature2; //72 72 41 61
  DWORD free_clusters; // -1 if unknown
  DWORD most_recently_allocated;
  BYTE  reserved[12];
  WORD  unknown2;
  WORD  signature3; //55 AA
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
  DWORD data;
  DWORD flags;
  DWORD owning_task;
  DWORD cur_FAT32_blk_num;
  DWORD *cur_FAT32_blk;
  FAT32_boot_record *cur_FAT32_br;
  FAT32_file_info_sector *cur_FAT32_fis;

};

DWORD num_partitions=0;
my_partition_entry local_partitions[32];

void read_partition_info() //This is incomplete
{
  DWORD i,j,offset=0;
  bool primary=true;
  master_boot_record mbr;
  my_partition_entry *p;
  FAT32_boot_record br;

  num_partitions=0;
  while (true) {
    rblocks(&mbr,offset,1);
    if (mbr.signature!=0xAA55)
      return;
    j=4;
    for (i=0;i<4;i++) {
      if (mbr.p[i].active==0x80 || (!primary && i==0)) {
        p=&local_partitions[num_partitions];
        p->offset=mbr.p[i].offset+offset;
        p->size  =mbr.p[i].size;
        p->type  =mbr.p[i].type;
        p->flags = 0;
        p->owning_task=0;
        p->cur_FAT32_br=NULL;
        p->cur_FAT32_fis=NULL;
        p->cur_FAT32_blk_num=0xFFFFFFFF;
        ? "Part:",num_partitions,
          " Offset:",p->offset,
          " Size:",p->size,
          " Type:",p->type,"\r\n";
        if (p->offset>=MAX_SECTOR)
          return;
        if (p->type==0x0B) {
          rblocks(&br,p->offset,1);
          p->file_system_info_sector=p->offset+br.file_system_info_sector;
          p->FAT1=p->offset+br.reserved_sectors;
          p->FAT2=p->FAT1+br.sectors_per_FAT;
          p->data=p->FAT2+br.sectors_per_FAT-2*br.sectors_per_cluster; //Starts at cluster 2
          p->cur_FAT32_br=malloc(sizeof(FAT32_boot_record));
          rblocks(p->cur_FAT32_br,p->offset,1);
          p->cur_FAT32_fis=malloc(sizeof(FAT32_file_info_sector));
          rblocks(p->cur_FAT32_fis,p->file_system_info_sector,1);
          p->cur_FAT32_blk=malloc(BLOCK_SIZE);
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
    if (offset>=MAX_SECTOR)
      return;
  }
}

void select_default_drive()
{
  int i;
  my_partition_entry *p;
  for (i=0;i<num_partitions;i++) {
    p=&local_partitions[i];
    if (p->type==0x0B) {
      fs->current_partition=i;
      fs->current_directory=new_string("/");
      fs->current_dir_cluster=p->cur_FAT32_br->root_cluster;
      return;
    }
  }
  ? "FATAL ERROR: No FAT32 partition found\r\n";
}

void lock_drive(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  if (p->owning_task!=fs->absolute_address && bts(&p->flags,mpef_locked))
    swap_in_next;
  p->owning_task=fs->absolute_address;
}

void unlock_drive(int partition)
{
  my_partition_entry *p=&local_partitions[partition];
  p->owning_task=0;
  btr(&p->flags,mpef_locked);
}

DWORD next_FAT32_cluster_num(DWORD c)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD FAT_blk_num;
  FAT_blk_num=c/(BLOCK_SIZE/4);
  if (p->cur_FAT32_blk_num!=FAT_blk_num) {
    rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
    p->cur_FAT32_blk_num=FAT_blk_num;
  }
  return p->cur_FAT32_blk[c-FAT_blk_num*(BLOCK_SIZE/4)];
}


void rcluster(BYTE *buf,DWORD c)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  rblocks(buf,p->data+c*p->cur_FAT32_br->sectors_per_cluster,
          p->cur_FAT32_br->sectors_per_cluster);
}

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

#define FAT32_attr_read_only 1
#define FAT32_attr_hidden    2
#define FAT32_attr_system    4
#define FAT32_attr_volume_id 8
#define FAT32_attr_directory 0x10
#define FAT32_attr_archive   0x20
#define FAT32_attr_long_name 0x0F


void to_FAT32_name(char *src,char *dst)
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

void from_FAT32_name(char *src,char *dst)
{
  int i,j,k;
  for (j=7;j>=0 && src[j]==0x20;j--);
  k=0;
  for(i=0;i<=j;i++)
    dst[k++]=src[i];
  if (*src!='.')
    dst[k++]='.';
  for (j=10;j>=8 && src[j]==0x20;j--);
  for(i=8;i<=j;i++)
    dst[k++]=src[i];
  dst[k++]=0;
}

bool find_FAT32_file(char *name,FAT32_dir_entry *result,bool dir_only,bool file_only,bool unlock=true)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_cluster,cur_dir_entry,entries_per_cluster;
  bool cont=true,cont2=false;
  char *ptr,*ptr2,dname[11];
  DWORD i;
  BYTE ch;

  lock_drive(fs->current_partition);
  if (p->type!=0x0B) 
    ? "Not a FAT32 Partition\r\n";
  else {
    to_FAT32_name(name,dname);
    buf=malloc(BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster);
    entries_per_cluster=BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster
        /sizeof(FAT32_dir_entry);
    cur_dir_cluster=fs->current_dir_cluster;
    rcluster(buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
        cur_dir_cluster=next_FAT32_cluster_num(cur_dir_cluster);
        if (cur_dir_cluster==0xFFFFFFF)
          cont=false;
        else {
          rcluster(buf,cur_dir_cluster);
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
              unlock_drive(fs->current_partition);
            return true;
          }
        }
      }
      cur_dir_entry++;
    }
    free(buf);
  }  
  if (unlock)
    unlock_drive(fs->current_partition);
  return false;
}


BYTE *read_FAT32_file(char *filename,DWORD *size=NULL,bool unlock=true)
{
  BYTE *buf=NULL,*ptr;
  FAT32_dir_entry de;
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD c,cluster_size=BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster,num_clusters;

  if (size)
    *size=0;
  lock_drive(fs->current_partition);
  if (!find_FAT32_file(filename,&de,false,true,false))
    ? "ERROR: File not found\r\n";
  else {
    num_clusters=(de.size+1+cluster_size-1)/cluster_size;
    buf=malloc(num_clusters*cluster_size);
    c=de.cluster_lo+(de.cluster_hi<<16);
    ptr=buf;
    while (c!=0x0FFFFFFF) {
      rcluster(ptr,c);
      ptr=ptr+cluster_size;
      c=next_FAT32_cluster_num(c);
    }
    buf[de.size]=0; //Terminate
    if (size)
      *size=de.size;
  }
  if (unlock)
    unlock_drive(fs->current_partition);
  return buf;
}

void cd_FAT32(char *name)
{
  FAT32_dir_entry de;
  if (find_FAT32_file(name,&de,true,false)) {
    fs->current_dir_cluster=de.cluster_lo+(de.cluster_hi << 16);
  } else
    ? "ERROR: File not found\r\n";
}
//This is for low level disk access
//Call wblocks() instead
void ide_write_blocks(BYTE *buf; DWORD block; DWORD count)
{
  DWORD i,words_avail,sectors_avail;

  ide_wait_for_drive();
  ide_select_block(0,block,count);
  outp(IDE_COMMAND,WIN_MULTWRITE);
  while (count) {
    while (inp(IDE_STATUS) & DRIVE_READY!=DRIVE_READY)
      swap_in_next;
    while (inp(IDE_STATUS) & DATA_READY!=DATA_READY)
      swap_in_next;
    sectors_avail=1;
    words_avail=sectors_avail*BLOCK_SIZE/2;
    rep_outsw(buf,words_avail,IDE_DATA_REG);
    buf=buf+words_avail*2;
    count=count-sectors_avail;
  }
  while (!btr(SYS_FLAGS,SYSF_IDE1_DONE))
    swap_in_next;
  btr(SYS_FLAGS,SYSF_IDE1_BUSY);
}


//This writes blocks directly to disk.  This is dangerous
//but if you can use it for database design, once you
//allocate some blocks in the FAT.
//Blocks are typically 512 bytes
//
//It needs to be fixed so it does more than one block at 
//a time when it calls ide_write_blocks.
void wblocks(BYTE *buf, DWORD block, DWORD count)
{
  BYTE *verify_buf;
  DWORD l,i,verify_errors=0;
  verify_buf=malloc(MAX_WRITES_AT_ONCE*BLOCK_SIZE);
  while (count>0) {
    l=count;
    if (l>MAX_WRITES_AT_ONCE)
      l=MAX_WRITES_AT_ONCE;
    ide_write_blocks(buf,block,l);
    rblocks(verify_buf,block,l);
    for (i=0;i<l*BLOCK_SIZE;i++)
      if (verify_buf[i]!=buf[i])
        verify_errors++;
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
  if (verify_errors)
    ? systext(st_verify_error),verify_errors,"\r\n";
  free(verify_buf);
}


//This writes a cluster (typically 8 blocks) and numbered
//from the start of the data area of the partition.
void wcluster(BYTE *buf,DWORD c)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  wblocks(buf,p->data+c*p->cur_FAT32_br->sectors_per_cluster,
          p->cur_FAT32_br->sectors_per_cluster);
}

//This writes several clusters
void wclusters(BYTE *buf,DWORD c,DWORD num)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  while (num--) {
    wcluster(buf,c);
    buf=buf+BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster;
    c=next_FAT32_cluster_num(c);
  }
}

//This frees a chain of clusters.  It needs to be
//made more efficient by doing one write for
//all modifications to the same FAT block
void free_FAT32_clusters(DWORD c,bool unlock)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD FAT_blk_num,next=0;
  bool cont=true;
  lock_drive(fs->current_partition);
  while (next!=0x0FFFFFFF && next!=0) {
    FAT_blk_num=c/(BLOCK_SIZE/4);
    if (p->cur_FAT32_blk_num!=FAT_blk_num) {
      rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
      p->cur_FAT32_blk_num=FAT_blk_num;
    }
    next=p->cur_FAT32_blk[c-FAT_blk_num*(BLOCK_SIZE/4)];
    p->cur_FAT32_blk[c-FAT_blk_num*(BLOCK_SIZE/4)]=0;
    wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
    wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);
    c=next;
  }
  if (unlock)
    unlock_drive(fs->current_partition);
}

//This is a low level routine you generally don't need
//to call yourself.  save_FAT32_file calls this automatically
DWORD alloc_FAT32_clusters(DWORD c,DWORD num,bool unlock=true)  //c=0 means first cluster in chain
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD FAT_blk_num,first=0;
  bool cont;
  lock_drive(fs->current_partition);
  while (num--) {
    cont=true;
    while (cont) {
      p->cur_FAT32_fis->most_recently_allocated++;
      p->cur_FAT32_fis->free_clusters=0xFFFFFFFF;
      FAT_blk_num=p->cur_FAT32_fis->most_recently_allocated/(BLOCK_SIZE/4);
      if (p->cur_FAT32_blk_num!=FAT_blk_num) {
        rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
        p->cur_FAT32_blk_num=FAT_blk_num;
      }
      if (!p->cur_FAT32_blk[p->cur_FAT32_fis->most_recently_allocated-
        FAT_blk_num*(BLOCK_SIZE/4)]) {
        p->cur_FAT32_blk[p->cur_FAT32_fis->most_recently_allocated-
          FAT_blk_num*(BLOCK_SIZE/4)]=0x0FFFFFFF;
        wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
        wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);
        wblocks(p->cur_FAT32_fis,p->file_system_info_sector,1);
        cont=false;
      }
    }
    if (!first)
      first=p->cur_FAT32_fis->most_recently_allocated;
    if (c) {
      FAT_blk_num=c/(BLOCK_SIZE/4);
      if (p->cur_FAT32_blk_num!=FAT_blk_num) {
        rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
        p->cur_FAT32_blk_num=FAT_blk_num;
      }
      p->cur_FAT32_blk[c-FAT_blk_num*(BLOCK_SIZE/4)]=p->cur_FAT32_fis->most_recently_allocated;
      wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
      wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);
    }
    c=p->cur_FAT32_fis->most_recently_allocated;
  }
  if (unlock)
    unlock_drive(fs->current_partition);
  return first;
}

//This searches for a contiguous space of blocks
//You typically don't need to call this yourself, unless
//you are setting-up an area for a database.
DWORD alloc_contiguous_FAT32_clusters(DWORD num, bool unlock=true)  //c=0 means first cluster in chain
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD FAT_blk_num,first=1,i;
  bool cont;
  if (!num) return;
  lock_drive(fs->current_partition);
  while (true) {
    first++;
    i=0;
    cont=true;
    while (cont && i<num) {
      FAT_blk_num=(first+i)/(BLOCK_SIZE/4);
      if (p->cur_FAT32_blk_num!=FAT_blk_num) {
        rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
        p->cur_FAT32_blk_num=FAT_blk_num;
      }
      if (p->cur_FAT32_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)])
        cont=false;
      else
        i++;
    }
    if (cont) {
      for (i=0;i<num;i++) {
        FAT_blk_num=(first+i)/(BLOCK_SIZE/4);
        if (p->cur_FAT32_blk_num!=FAT_blk_num) {
          rblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
          p->cur_FAT32_blk_num=FAT_blk_num;
        }
        if (i+1==num)
          p->cur_FAT32_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)]=0x0FFFFFFF;
        else
          p->cur_FAT32_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)]=first+i+1;
        if ((first+i+1)/(BLOCK_SIZE/4)!=p->cur_FAT32_blk_num) {
          wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
          wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);
        }
      }
      wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);
      wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);
      return first;
    }
  }
  if (unlock)
    unlock_drive(fs->current_partition);
}

//You typically don't need to call this yourself.
//save_FAT32_file calls it automatically.
bool new_FAT32_dir_entry(FAT32_dir_entry *de)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_cluster,cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true,cont2,written=false;
  char *ptr,*ptr2,dname[11];
  DWORD i;
  BYTE ch;

  lock_drive(fs->current_partition);
  if (p->type!=0x0B) 
    ? systext(st_not_FAT32);
  else {
    buf=malloc(BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster);
    entries_per_block=BLOCK_SIZE/sizeof(FAT32_dir_entry);
    entries_per_cluster=entries_per_block*p->cur_FAT32_br->sectors_per_cluster;
    cur_dir_cluster=fs->current_dir_cluster;
    rcluster(buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
        cur_dir_cluster=next_FAT32_cluster_num(cur_dir_cluster);
        rcluster(buf,cur_dir_cluster);
        cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch) {
        if (!written) {
          memcpy(&buf[cur_dir_entry],de,sizeof(FAT32_dir_entry));
          wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
             p->data+cur_dir_cluster*p->cur_FAT32_br->sectors_per_cluster
             +cur_dir_entry/entries_per_block,1);
          cur_dir_entry++;
          if (cur_dir_entry==entries_per_cluster) {
            cur_dir_cluster=alloc_FAT32_clusters(cur_dir_cluster,1);
            cur_dir_entry=0;
          }
          buf[cur_dir_entry].name[0]=0;
          wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
             p->data+cur_dir_cluster*p->cur_FAT32_br->sectors_per_cluster
             +cur_dir_entry/entries_per_block,1);
        }
        cont=false;
      } else if (ch==0xE5) {
        if (!written) {
          memcpy(&buf[cur_dir_entry],de,sizeof(FAT32_dir_entry));
          wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
            p->data+cur_dir_cluster*p->cur_FAT32_br->sectors_per_cluster
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
          wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
             p->data+cur_dir_cluster*p->cur_FAT32_br->sectors_per_cluster
             +cur_dir_entry/entries_per_block,1);
          free_FAT32_clusters(buf[cur_dir_entry].cluster_lo+
            (buf[cur_dir_entry].cluster_hi<<16),false);
        }
      }
      cur_dir_entry++;
    }
    free(buf);
  }  
  unlock_drive(fs->current_partition);
  return false;
}

//This is a low level routine.
//del() calls this
bool del_FAT32_files(char *wild)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_cluster,cur_dir_entry,entries_per_cluster,entries_per_block;
  bool cont=true;
  char cbuf[16];
  BYTE ch;

  lock_drive(fs->current_partition);
  if (p->type!=0x0B) 
    ? systext(st_not_FAT32);
  else {
    buf=malloc(BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster);
    entries_per_block=BLOCK_SIZE/sizeof(FAT32_dir_entry);
    entries_per_cluster=entries_per_block*p->cur_FAT32_br->sectors_per_cluster;
    cur_dir_cluster=fs->current_dir_cluster;
    rcluster(buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
        cur_dir_cluster=next_FAT32_cluster_num(cur_dir_cluster);
        rcluster(buf,cur_dir_cluster);
        cur_dir_entry=0;
      }
      ch=buf[cur_dir_entry].name[0];
      if (!ch)
        cont=false;
      else if (ch!=0xE5) {
        from_FAT32_name(buf[cur_dir_entry].name,cbuf);
        if (wild_card_match(wild,cbuf)) {
          ? systext(st_delete),cbuf,"\r\n";
          buf[cur_dir_entry].name[0]=0xE5;
          wblocks(&buf[cur_dir_entry & (0-entries_per_block)],
             p->data+cur_dir_cluster*p->cur_FAT32_br->sectors_per_cluster
             +cur_dir_entry/entries_per_block,1);
          free_FAT32_clusters(buf[cur_dir_entry].cluster_lo+
            (buf[cur_dir_entry].cluster_hi<<16),false);
        }
      }
      cur_dir_entry++;
    }
    free(buf);
  }  
  unlock_drive(fs->current_partition);
  return false;
}

//Save text file calls this
void save_FAT32_file(char *name,BYTE *buf,DWORD size,bool contiguous=true)
{
  FAT32_dir_entry de;
  my_partition_entry *p=&local_partitions[fs->current_partition];
  DWORD c,cluster_count,
        cluster_size=BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster;
  to_FAT32_name(name,de.name);
  de.size=size;
  cluster_count=(size+cluster_size-1)/cluster_size;
  if (contiguous)
    c=alloc_contiguous_FAT32_clusters(cluster_count);
  else
    c=alloc_FAT32_clusters(0,cluster_count);
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
  new_FAT32_dir_entry(&de);
  wclusters(buf,c,cluster_count);
}

void dir_FAT32(char *wild)
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  FAT32_dir_entry *buf;
  DWORD cur_dir_cluster,cur_dir_entry,entries_per_cluster;
  bool cont=true;
  char *ptr,cbuf[16];
  DWORD i;
  BYTE ch;

  lock_drive(fs->current_partition);
  if (p->type!=0x0B) 
    ? systext(st_not_FAT32);
  else {
    ? "Directory of ",fs->current_directory,"\r\n";
    spaces(16); ? "  SIZE   CLUSTER\r\n";
    buf=malloc(BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster);
    entries_per_cluster=BLOCK_SIZE*p->cur_FAT32_br->sectors_per_cluster
        /sizeof(FAT32_dir_entry);
    cur_dir_cluster=fs->current_dir_cluster;
    rcluster(buf,cur_dir_cluster);
    cur_dir_entry=0;
    while (cont) {
      if (cur_dir_entry==entries_per_cluster) {
        cur_dir_cluster=next_FAT32_cluster_num(cur_dir_cluster);
        if (cur_dir_cluster==0xFFFFFFF)
          cont=false;
        else {
          rcluster(buf,cur_dir_cluster);
          cur_dir_entry=0;
        }
      }
      if (cont) {
        ch=buf[cur_dir_entry].name[0];
        if (!ch) {
          cont=false;
        } else if (ch!=0xE5) {
          from_FAT32_name(buf[cur_dir_entry].name,cbuf);
          if (wild_card_match(wild,cbuf)) {
            i=buf[cur_dir_entry].cluster_lo+(buf[cur_dir_entry].cluster_hi<<16);
            ? cbuf;
            spaces(16-strlen(cbuf));
            ? buf[cur_dir_entry].size," ",i,"\r\n";
          }
        }
      }
      cur_dir_entry++;
    }
    free(buf);
  }  
  unlock_drive(fs->current_partition);
}
 
BYTE *read_file(char *filename,DWORD *size=NULL)
{
  return read_FAT32_file(filename,size);
}

defext(ext_wblocks,&wblocks);
defext(ext_rblocks,&rblocks);
defext(ext_irq0E,&irq0E_handler);
defext(ext_read_file,&read_file);
defext(ext_select_default_drive,&select_default_drive);
 
 