asm("BOOTHD");
asm("BOOTFD");
load("BOOTHD");
load("BOOTFD");
//This makes the current partitian bootable
#define BOOT_IMG   "BOOT.IMG"
#define FLOPPY_IMG "FLOPPY.IMG"
#define BOOT_LOAD_BASE (0x30000)

void create_boot_image(char src_drive,char dst_drive,DWORD *out_size,DWORD *out_offset)
{
  my_partition_entry *p;
  BYTE *buf;
  DWORD *dd;

  drive(src_drive);
  ? "Recompiling operating system? ";
  if (y_or_n) {
    cd("/HOPPY/KERNEL/ASM");
    asm("OS");
    buf=read_file("OS.BIN",out_size);
  } else {
    cd("/");
    buf=read_file(BOOT_IMG,out_size);
  }

  ? "Creating "BOOT_IMG" from OS.BIN\r\n";
  drive(dst_drive);
  cd("/");
  p=&local_partitions[fs->current_partition];

  dd=buf;
  *out_offset=*dd;
  printf("Size: %dK   Code Offset:%08X\r\n",(*out_size+1023)/1024,*out_offset);
  if (p->type==1)
    save_FAT12_file(fs->current_partition,fs->current_dir_cluster,BOOT_IMG,buf,*out_size,true);
  else if (p->type==0x0B)
    save_FAT32_file(fs->current_partition,fs->current_dir_cluster,BOOT_IMG,buf,*out_size,true);

  free(buf);
  drive(src_drive);
  del("/HOPPY/KERNEL/ASM/OS.BIN");
}

void install_hd_boot(char src_drive,char dst_drive)
{
  my_partition_entry *p;
  FAT32_boot_record br;
  FAT32_dir_entry de;
  DWORD i,*dd,size,offset,syscode;
  BYTE *bb;
  WORD *ww;
  drive(dst_drive);
  p=&local_partitions[fs->current_partition];
  if (p->type!=0x0B)
    ? systext(st_not_FAT32);
  else {
    create_boot_image(src_drive,dst_drive,&size,&offset);
    drive(dst_drive);
    rblocks(fs->current_partition,&br,p->offset,1);
    if (!find_FAT32_file(fs->current_partition,fs->current_dir_cluster,BOOT_IMG,&de,false,true))
      ? "No image\r\n";
    else {
      ? "Modifying partition boot record.\r\n";
      ww=BHD_INT13;
      *ww=0x13CD;
      ww=BHD_RESTART+1;
      *ww=(size+BLOCK_SIZE-1)/BLOCK_SIZE;
      syscode=(BOOT_LOAD_BASE+offset)>>4;
      ww=BHD_JMP_START+2;
      *ww=syscode;
      ww=BHD_LBA_PACKET+6;
      *ww=(BOOT_LOAD_BASE>>4);
      dd=BHD_LBA_PACKET+8;
      *dd=p->data+(de.cluster_lo+(de.cluster_hi<<16))*p->spc;

      br.jump_and_nop[0]=0xEB;
      br.jump_and_nop[1]=0x58;
      br.jump_and_nop[2]=0x90;
      bb=BHD_CODE;
      for (i=0;i<(BHD_END-BHD_CODE);i++)
	br.code[i]=bb[i];
      for (;i<420;i++)
	br.code[i]=0;
      wblocks(fs->current_partition,&br,p->offset,1);
    }
  }
}

void install_fd_boot(char src_drive,char dst_drive)
{
  my_partition_entry *p;
  FAT12_boot_record br;
  FAT12_dir_entry de;
  DWORD i,*dd,size,offset,syscode;
  BYTE *bb;
  WORD *ww;
  drive(dst_drive);
  p=&local_partitions[fs->current_partition];
  if (p->type!=0x01)
    ? systext(st_not_FAT12);
  else {
    create_boot_image(src_drive,dst_drive,&size,&offset);
    drive(dst_drive);
    rblocks(fs->current_partition,&br,0,1);
    if (!find_FAT12_file(fs->current_partition,fs->current_dir_cluster,BOOT_IMG,&de,false,true))
      ? "No image\r\n";
    else {
      ? "Modifying partition boot record.\r\n";
      ww=BFD_INT13_1;
      *ww=0x13CD;
      ww=BFD_INT13_2;
      *ww=0x13CD;
      ww=BFD_RESTART+1;
      *ww=(size+BLOCK_SIZE-1)/BLOCK_SIZE;
      syscode=(BOOT_LOAD_BASE+offset)>>4;
      ww=BFD_JMP_START+2;
      *ww=syscode;
      ww=BFD_RESTART+4;
      *ww=p->data+de.cluster_lo*p->spc;
      ww=BFD_RESTART+7;
      *ww=(BOOT_LOAD_BASE>>4);

      br.jump_and_nop[0]=0xEB;
      br.jump_and_nop[1]=0x40;
      br.jump_and_nop[2]=0x90;

      bb=BFD_CODE;
      for (i=0;i<(BFD_END-BFD_CODE);i++)
	br.code[i]=bb[i];
      for (;i<444;i++)
	br.code[i]=0;
      wblocks(fs->current_partition,&br,0,1);
    }
  }
}

void fetch_floppy_image(char src_drive, char dst_drive)
{
  BYTE *buf=malloc(2880*BLOCK_SIZE);
  drive(src_drive);
  rblocks(toupper(src_drive)-'A',buf,0,2880);
  drive(dst_drive);
  save_file(FLOPPY_IMG,buf,2880*BLOCK_SIZE);
  free(buf);
}

void store_floppy_image(char src_drive, char dst_drive)
{
  BYTE *buf;
  drive(src_drive);
  buf=read_file(FLOPPY_IMG);
  wblocks(toupper(dst_drive)-'A',buf,0,2880);
  free(buf);
}



