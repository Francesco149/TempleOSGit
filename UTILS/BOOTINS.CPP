//This makes the current partitian bootable
#define BOOT_IMG "BOOT.IMG"

void create_boot_image()
{
  BYTE *img;
  DWORD size,i;
  ? "Creating ",BOOT_IMG," ";
  cd("/");
  size=*SYS_BOOT_IMAGE_END-*SYS_BOOT_IMAGE_START;
  ?"Size:",size,"\r\n";
  img=malloc(size);
  for (i=0;i<size;i++)
    img[i]=mz[*SYS_BOOT_IMAGE_START+i];
  save_FAT32_file(BOOT_IMG,img,size,true);
  free(img);
}

void install_boot()
{
  my_partition_entry *p=&local_partitions[fs->current_partition];
  FAT32_boot_record br;
  FAT32_dir_entry de;
  DWORD i,*dd;
  create_boot_image();
  cd("/");
  rblocks(&br,p->offset,1);
  if (!find_FAT32_file(BOOT_IMG,&de,false,true))
    ? "No image\r\n";
  else {
    ? "Modifying partition boot record.\r\n";
    dd=mz+*SYS_BOOT_CODE+BOOT_IMG_BLOCK_LO-(BOOT_CODE-START_BOOT_CODE);
    *dd=p->data+(de.cluster_lo+(de.cluster_hi<<16))*p->cur_FAT32_br->sectors_per_cluster;
    br.jump_and_nop[0]=0xEB;
    br.jump_and_nop[1]=0x58;
    br.jump_and_nop[2]=0x90;
    for (i=0;i<(END_BOOT_CODE-BOOT_CODE);i++)
      br.code[i]=mz[*SYS_BOOT_CODE+i];
    wblocks(&br,p->offset,1);
  }
}

install_boot;;

 
 
 
 
 
 