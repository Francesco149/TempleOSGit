#define BLOCK_SIZE 512
DWORD FDC_retries=5;
DWORD FDC_DRIVE_HEADS=2;
DWORD FDC_DISK_CYLINDERS=80;
DWORD FDC_DISK_SECTORS=18;
DWORD FDC_MAX_SECTOR=FDC_DRIVE_HEADS*FDC_DISK_CYLINDERS*FDC_DISK_SECTORS;
DWORD FDC_MAX_READS_AT_ONCE=1;
DWORD FDC_MAX_WRITES_AT_ONCE=1;
DWORD FDC_DMA_BUF=0xD000;

DWORD IDE_DRIVE_HEADS=16;
DWORD IDE_DRIVE_SECTORS=0x3F;
DWORD IDE_MAX_SECTOR=IDE_DRIVE_HEADS*IDE_DRIVE_SECTORS*0x10000;
DWORD IDE_MAX_READS_AT_ONCE=0x80;
DWORD IDE_MAX_WRITES_AT_ONCE=1;

#define FDC_p0	      0x3F0
#define FDC_p1	      0x3F1
#define FDC_DOR       0x3F2
#define FDC_p3	      0x3F3
#define FDC_STATUS    0x3F4
#define FDC_DATA      0x3F5
#define FDC_DIR       0x3F7
#define FDC_DCR       0x3F7

BYTE *FDC0_cache_blocks=NULL;
BYTE *FDC0_in_cache_bits=NULL;

void irq06_handler()
{
  bts(SYS_FLAGS,SYSF_FDC0_DONE);
  outp(0x20,0x20);
}
defext(ext_irq06,&irq06_handler);

void wait_FDC0_irq()
{
  DWORD old_flags=get_flags;
  sti;
  while (!btr(SYS_FLAGS,SYSF_FDC0_DONE))
    swap_in_next;
  set_flags(old_flags);
}

BYTE FDC0_status[12];

void flush_FDC_status()
{
  bool cont=true;
  BYTE b;
  int i=0;
  while (cont) {
    b=inp(FDC_STATUS);	  port_nop();
    if (b & 0x80) {
      if (b & 0x40) {
	FDC0_status[i++]=inp(FDC_DATA);	  port_nop();
      } else
	cont=false;
    }
    if (cont)
      swap_in_next;
  }
  FDC0_status[0]=FDC0_status[0] & 0xC8;  //clear non-error bits
}



tss_struct *monitor_floppy_tss=NULL;
DWORD last_FDC0_jiffy=0;

void send_FDC_cmd_byte(BYTE v)
{
  last_FDC0_jiffy=*JIFFIES;
  flush_FDC_status;
  outp(FDC_DATA,v);  port_nop();
}

void seek_FDC0(BYTE cyl, BYTE head)
{
  send_FDC_cmd_byte(0x0F);
  send_FDC_cmd_byte(head<<2);
  send_FDC_cmd_byte(cyl);
  wait_FDC0_irq;
}

void free_FDC0()
{
  btr(SYS_FLAGS,SYSF_FDC0_BUSY);
}

void monitor_floppy_tsk()
{
  preempt(off);
  while (true) {
    if (!bts(SYS_FLAGS,SYSF_FDC0_BUSY)) {
      if (last_FDC0_jiffy) {
	if (*JIFFIES > last_FDC0_jiffy+3*50) {
	  seek_FDC0(0,0);
	  seek_FDC0(0,1);
	  outp(FDC_DOR,0x00);
	  last_FDC0_jiffy=0;
	}
      }
      free_FDC0;
    }
    swap_in_next;
  }
}


void get_FDC0()
{
  BYTE b;

  while (bts(SYS_FLAGS,SYSF_FDC0_BUSY))
    swap_in_next;
  btr(SYS_FLAGS,SYSF_FDC0_DONE);

  if (!last_FDC0_jiffy) {
    last_FDC0_jiffy=*JIFFIES;
    outp(FDC_DOR,0x00);  port_nop();
    outp(FDC_DOR,0x1C); //Motor on
    wait_FDC0_irq;
    outp(FDC_DCR,0x00);   port_nop();//step,Load, Unload specs
    send_FDC_cmd_byte(0x03);
    send_FDC_cmd_byte(0xCF);
    send_FDC_cmd_byte(0x10);
    wait_FDC0_irq;
    send_FDC_cmd_byte(0x07);  //Recalibrate
    send_FDC_cmd_byte(0x00);
    wait_FDC0_irq;
  }
}

void reset_FDC0()
{
  DWORD target_jiffies=*JIFFIES+10;

  outp(0x0A,0x42);  //Shut down old DMA
  outp(0x09,0x02);
  outp(FDC_DOR,0x00);
  while (*JIFFIES<target_jiffies)
    swap_in_next;
  last_FDC0_jiffy=0;
  btr(SYS_FLAGS,SYSF_FDC0_BUSY);
  get_FDC0;
}

bool FDC0_read_track(BYTE cyl, BYTE head,DWORD block)
{
  int i;
  DWORD size=BLOCK_SIZE*FDC_DISK_SECTORS-1;
  bool old_preempt=preempt(off);

  for (i=0;i<FDC_retries;i++) { //Retry up to 10 times
    seek_FDC0(cyl,head);

    outp(0x0A,0x42);  //Shut down old DMA
    outp(0x09,0x02);
    outp(0x08,0x00);  //ctrl
    outp(0x0B,0x56);  //mode Read
    outp(0x0C,0);
    outp(0x04,FDC_DMA_BUF & 0xFF);  //addr
    outp(0x04,(FDC_DMA_BUF / 256) & 0xFF);
    outp(0x81,FDC_DMA_BUF /65536);     //set page
    outp(0x0C,0);
    outp(0x05,size & 0xFF);  //count
    outp(0x05,size/256);
    outp(0x0A,0x02);  //Start new DMA
    outp(0x09,0x06);

    send_FDC_cmd_byte(0x62);
    send_FDC_cmd_byte(head<<2);
    send_FDC_cmd_byte(cyl);
    send_FDC_cmd_byte(head);
    send_FDC_cmd_byte(1); //sector
    send_FDC_cmd_byte(2);  //size (512)
    send_FDC_cmd_byte(FDC_DISK_SECTORS);	//last sec
    send_FDC_cmd_byte(0x1B);  //gap
    send_FDC_cmd_byte(0xFF);
    wait_FDC0_irq;
    flush_FDC_status;
    if (FDC0_status[0] || FDC0_status[1] || FDC0_status[2]) {
      ? "FDC0 Read Error:",FDC0_status[2],FDC0_status[1],FDC0_status[0],"\r\n";
      reset_FDC0;
    } else {
      memcpy(FDC0_cache_blocks+BLOCK_SIZE*block,mz+FDC_DMA_BUF,size+1);
      for (i=0;i<FDC_DISK_SECTORS;i++)
        bts(FDC0_in_cache_bits,block+i);
      preempt(old_preempt);
      return true;
    }
  }
  ? "FDC0 Read Failed!!!  Block:",block,"\r\n";
  preempt(old_preempt);
  return false;
}

bool FDC0_rblocks(BYTE *buf; DWORD block; DWORD count)
{
  bool okay=true;
  DWORD l,blk;
  BYTE cyl,head,sec;
  while (count>0) {
    *PROGRESS0=*PROGRESS0+1;
    l=count;
    if (l>FDC_MAX_READS_AT_ONCE)
      l=FDC_MAX_READS_AT_ONCE;
    blk=block;
    sec=blk%FDC_DISK_SECTORS+1;blk=blk/FDC_DISK_SECTORS;
    head=blk%FDC_DRIVE_HEADS;
    cyl=blk/FDC_DRIVE_HEADS;
    if (!bt(FDC0_in_cache_bits,block)) {
      if (!FDC0_read_track(cyl,head,block-(sec-1)))
	okay=false;
    }
    memcpy(buf,FDC0_cache_blocks+BLOCK_SIZE*block,l*BLOCK_SIZE);
//    FDC0_read_block2(buf,cyl,head,sec);
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
  *PROGRESS0=(*PROGRESS0 & 0xFFFF0000);
  return okay;
}

bool FDC0_write_block2(BYTE *buf; BYTE cyl, BYTE head,BYTE sec,DWORD block)
{
  int i;
  DWORD size=BLOCK_SIZE*FDC_MAX_WRITES_AT_ONCE-1;
  bool old_preempt;

  memcpy(FDC0_cache_blocks+BLOCK_SIZE*block,buf,size+1);
  bts(FDC0_in_cache_bits,block);

  old_preempt=preempt(off);
  for (i=0;i<FDC_retries;i++) { //Retry up to 10 times
    seek_FDC0(cyl,head);

    memcpy(mz+FDC_DMA_BUF,buf,size+1);
    outp(0x0A,0x42);  //Shut down old DMA
    outp(0x09,0x02);
    outp(0x08,0x00);  //ctrl
    outp(0x0B,0x5A);  //mode Write
    outp(0x0C,0);
    outp(0x04,FDC_DMA_BUF & 0xFF);  //addr
    outp(0x04,(FDC_DMA_BUF / 256) & 0xFF);
    outp(0x81,FDC_DMA_BUF /65536);     //set page
    outp(0x0C,0);
    outp(0x05,size & 0xFF);  //count
    outp(0x05,size/256);
    outp(0x0A,0x02);  //Start new DMA
    outp(0x09,0x06);

    send_FDC_cmd_byte(0xC5);
    send_FDC_cmd_byte(head<<2);
    send_FDC_cmd_byte(cyl);
    send_FDC_cmd_byte(head);
    send_FDC_cmd_byte(sec);
    send_FDC_cmd_byte(2);  //size (512)
    send_FDC_cmd_byte(FDC_DISK_SECTORS);	//last sec
    send_FDC_cmd_byte(0x1B);  //gap
    send_FDC_cmd_byte(0xFF);
    wait_FDC0_irq;
    flush_FDC_status;
    if (FDC0_status[2] || FDC0_status[1] || FDC0_status[0]) {
      ? "FDC0 Write Error:",FDC0_status[0],FDC0_status[1],FDC0_status[2],"\r\n";
      reset_FDC0;
    } else {
      preempt(old_preempt);
      return true;
    }
  }
  ? "FDC0 Write Failed!!!  Block:",block,"\r\n";
  preempt(old_preempt);
  return false;
}

bool FDC0_wblocks(BYTE *buf, DWORD block, DWORD count)
{
  bool okay=true;
  DWORD l,i,blk;
  BYTE cyl,head,sec;
  while (count>0) {
    *PROGRESS0=*PROGRESS0+1;
    l=count;
    if (l>FDC_MAX_WRITES_AT_ONCE)
      l=FDC_MAX_WRITES_AT_ONCE;
    blk=block;
    sec=blk%FDC_DISK_SECTORS+1;blk=blk/FDC_DISK_SECTORS;
    head=blk%FDC_DRIVE_HEADS;
    cyl=blk/FDC_DRIVE_HEADS;
    if (!FDC0_write_block2(buf,cyl,head,sec,block))
      okay=false;
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
  *PROGRESS0=(*PROGRESS0 & 0xFFFF0000);
  return okay;
}


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
#define WIN_PIDENTIFY			0xA1 /* IDEntify ATAPI device	*/
#define WIN_QUEUED_SERVICE		0xA2
#define WIN_SMART			0xB0 /* self-monitoring and reporting */
#define CFA_ERASE_SECTORS		0xC0
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
#define WIN_STANDBY			0xE2 /* Set device in Standby Mode */
#define WIN_SETIDLE1			0xE3
#define WIN_READ_BUFFER			0xE4 /* force read only 1 sector */
#define WIN_CHECKPOWERMODE1		0xE5
#define WIN_SLEEPNOW1			0xE6
#define WIN_FLUSH_CACHE			0xE7
#define WIN_WRITE_BUFFER		0xE8 /* force write only 1 sector */
#define WIN_FLUSH_CACHE_EXT		0xEA /* 48-Bit */
#define WIN_IDENTIFY			0xEC /* ask drive to IDEntify itself	*/
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

#define IDE_DATA_REG	0x1f0
#define IDE_FEATURE_REG 0x1f1
#define IDE_NSECTOR_REG 0x1f2
#define IDE_SECTOR_REG	0x1f3
#define IDE_LCYL_REG	0x1f4
#define IDE_HCYL_REG	0x1f5
#define IDE_SELECT_REG	0x1f6
#define IDE_STATUS	0x1f7
#define IDE_COMMAND	0x1f7
#define IDE_CONTROL_REG 0x3f6


void irq0E_handler()
{
  bts(SYS_FLAGS,SYSF_IDE1_DONE);
  outp(0xa0,0x20);
  outp(0x20,0x20);
}

void IDE_select_block(DWORD unit,DWORD block,DWORD count)
{
  DWORD sect,head,cyl,track;
  outp(IDE_CONTROL_REG,0x8);
  track=block / IDE_DRIVE_SECTORS;
  sect =block % IDE_DRIVE_SECTORS+1;
  outp(IDE_SECTOR_REG,sect);
  head =track % IDE_DRIVE_HEADS;
  cyl  =track / IDE_DRIVE_HEADS;
  outp(IDE_FEATURE_REG,0);
  outp(IDE_NSECTOR_REG,count);
  outp(IDE_LCYL_REG,cyl);
  outp(IDE_HCYL_REG,cyl>>8);
  outp(IDE_SELECT_REG,0xa0|(unit<<4)|head);
}

void IDE_wait_for_drive()
{
  while (bts(SYS_FLAGS,SYSF_IDE1_BUSY))
    swap_in_next;
  btr(SYS_FLAGS,SYSF_IDE1_DONE);
}


void IDE_read_blocks2(BYTE *buf; DWORD count)
{
  DWORD i,words_avail,sectors_avail;
  DWORD old_flags=get_flags;
  sti;
  while (!btr(SYS_FLAGS,SYSF_IDE1_DONE))
    swap_in_next;
  set_flags(old_flags);
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

void IDE_read_blocks(BYTE *buf; DWORD block; DWORD count)
{
  bool old_preempt=preempt(off);
  IDE_wait_for_drive();
  IDE_select_block(0,block,count);
  outp(IDE_COMMAND,WIN_MULTREAD);
  IDE_read_blocks2(buf,count);
  preempt(old_preempt);
}

void IDE_rblocks(BYTE *buf, DWORD block, int count)
{
  DWORD l;
  while (count>0) {
    *PROGRESS0=*PROGRESS0+1;
    l=count;
    if (l>IDE_MAX_READS_AT_ONCE)
      l=IDE_MAX_READS_AT_ONCE;
    IDE_read_blocks(buf,block,l);
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
  *PROGRESS0=(*PROGRESS0 & 0xFFFF0000);
}

//This is for low level disk access
//Call wblocks() instead
void IDE_write_blocks(BYTE *buf; DWORD block; DWORD count)
{
  DWORD i,words_avail,sectors_avail;
  bool old_preempt=preempt(off);
  IDE_wait_for_drive();
  IDE_select_block(0,block,count);
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
  preempt(old_preempt);
}


//This writes blocks directly to disk.	This is dangerous
//but if you can use it for database design, once you
//allocate some blocks in the FAT.
//Blocks are typically 512 bytes
//
//It needs to be fixed so it does more than one block at
//a time when it calls IDE_write_blocks.
void IDE_wblocks(BYTE *buf, DWORD block, DWORD count)
{
  BYTE *verify_buf;
  DWORD l,i,verify_errors=0;
  verify_buf=malloc(IDE_MAX_WRITES_AT_ONCE*BLOCK_SIZE);
  while (count>0) {
    *PROGRESS0=*PROGRESS0+1;
    l=count;
    if (l>IDE_MAX_WRITES_AT_ONCE)
      l=IDE_MAX_WRITES_AT_ONCE;
    IDE_write_blocks(buf,block,l);
    IDE_rblocks(verify_buf,block,l);
    for (i=0;i<l*BLOCK_SIZE;i++)
      if (verify_buf[i]!=buf[i])
	verify_errors++;
    buf=buf+l*BLOCK_SIZE;
    block=block+l;
    count=count-l;
  }
  *PROGRESS0=(*PROGRESS0 & 0xFFFF0000);
  if (verify_errors)
    ? systext(st_verify_error),verify_errors,"\r\n";
  free(verify_buf);
}

