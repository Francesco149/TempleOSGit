//This changes drives.	Include a letter in single quotes.
void drive(char drive_letter)
{
  char d;
  my_partition_entry *p;
  fs->current_partition=toupper(drive_letter)-'A';
  if (fs->current_partition>num_partitions)  {
    fs->current_partition=0;
    ? systext(st_invalid_drive);
    return;
  }
  p=&local_partitions[fs->current_partition];
  d='A'+fs->current_partition;
//  ? systext(st_current_partition),d,"\r\n";
  if (fs->current_partition<2)
    change_FDC0_diskette(fs->current_partition);
  free(fs->current_directory);
  fs->current_directory=new_string("/");
  if (p->type==0x01 || p->type==0x0B)
    fs->current_dir_cluster=p->root_cluster;
}

void mark_bad_sectors(char drive_letter)
{
  my_partition_entry *p;
  drive(drive_letter);
  p=&local_partitions[fs->current_partition];
  if (p->type==1 && fs->current_partition==0)
    mark_bad_FAT12_sectors(fs->current_partition);
  else
    ? "Not implemented on drive\r\n";
}


void cd(char *name)
{
  char buf[128],buf2[512],buf3[512];
  my_partition_entry *p;
  if (*name && name[1]==':') {
    drive(*name);
    name=name+2;
  }
  p=&local_partitions[fs->current_partition];
  if (*name=='/') {
    fs->current_dir_cluster=p->root_cluster;
    free(fs->current_directory);
    fs->current_directory=new_string("/");
    name++;
  }
  strcpy(buf2,name);
  strcpy(buf3,fs->current_directory);
  while (*buf2) {
    remove_first_segment(buf2,'/',buf);
    if (!strcmp(buf,"..")) {
      remove_last_segment(buf3,'/',buf);
      if (!*buf3)
	strcpy(buf3,"/");
      else {
	if (p->type==1)
	  cd_FAT12("..");
	else if (p->type==0x0B)
	  cd_FAT32("..");
      }
    } else if (!strcmp(buf,".")) {
      ;
    } else if (*buf) {
	if (p->type==1)
	  cd_FAT12(buf);
	else if (p->type==0x0B)
	  cd_FAT32(buf);
	if (strcmp(buf3,"/"))
	  strcat(buf3,"/");
	strcat(buf3,buf);
    }
  }
  free(fs->current_directory);
  fs->current_directory=new_string(buf3);
}

void dir(char *wild="*")
{
  char buf[128],buf2[512],*buf3;
  int old_partition=-1;
  buf3=new_string(fs->current_directory);
  if (*wild && wild[1]==':') {
    old_partition=fs->current_partition;
    drive(*wild);  //UNFINSHED changes dir on same drive
    wild=wild+2;
  }
  if (*wild=='/') {
    cd("/");
    wild++;
  }
  strcpy(buf2,wild);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  dir_FAT(fs->current_partition,fs->current_dir_cluster,buf);
  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
}



void del(char *wild)
{
  char buf[128],buf2[512],*buf3;
  int old_partition=-1;
  buf3=new_string(fs->current_directory);
  if (*wild && wild[1]==':') {
    old_partition=fs->current_partition;
    drive(*wild);  //UNFINSHED changes dir on same drive
    wild=wild+2;
  }
  if (*wild=='/') {
    cd("/");
    wild++;
  }
  strcpy(buf2,wild);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  if (local_partitions[fs->current_partition].type==1)
    del_FAT12_files(fs->current_partition,fs->current_dir_cluster,buf);
  else if (local_partitions[fs->current_partition].type==0x0B)
    del_FAT32_files(fs->current_partition,fs->current_dir_cluster,buf);
  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
}


BYTE *read_file(char *filename,DWORD *size=NULL)
{
  char buf[128],buf2[512],*buf3;
  BYTE *result;
  int old_partition=-1;
  my_partition_entry *p;
  buf3=new_string(fs->current_directory);
  if (*filename && filename[1]==':') {
    old_partition=fs->current_partition;
    drive(*filename);  //UNFINISHED dir on current drive
    filename=filename+2;
  }
  p=&local_partitions[fs->current_partition];
  if (*filename=='/') {
    cd("/");
    filename++;
  }
  strcpy(buf2,filename);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  if (p->type==1)
    result=read_FAT12_file(fs->current_partition,fs->current_dir_cluster,buf,size);
  else if (p->type==0x0B)
    result=read_FAT32_file(fs->current_partition,fs->current_dir_cluster,buf,size);
  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
  return result;
}
defext(ext_read_file,&read_file); //Redefined


void save_file(char *filename,BYTE *fbuf,DWORD size)
{
  char buf[128],buf2[512],*buf3;
  int old_partition=-1;
  my_partition_entry *p;
  buf3=new_string(fs->current_directory);
  if (*filename && filename[1]==':') {
    old_partition=fs->current_partition;
    drive(*filename);  //UNFINISHED dir on current drive
    filename=filename+2;
  }
  p=&local_partitions[fs->current_partition];
  if (*filename=='/') {
    cd("/");
    filename++;
  }
  strcpy(buf2,filename);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  if (p->type==1)
    save_FAT12_file(fs->current_partition,fs->current_dir_cluster,buf,fbuf,size);
  else if (p->type==0x0B)
    save_FAT32_file(fs->current_partition,fs->current_dir_cluster,buf,fbuf,size);
  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
}

void save_text_file(char *filename,char *st)
{
  save_file(filename,st,strlen(st));
}

void type(char *name)
{
  char *st;
  st=load_text_file(name);
  ? st;
  free(st);
}


int default_partition=-1;
void select_default_drive()
{
  int i;
  char ch;
  my_partition_entry *p;

  while (default_partition<0 || default_partition>=num_partitions) {
    ? "\r\nContinue Boot to Which Drive? ";
    i=toupper(getkey)-'A';
    ? "\r\n";
    if (i<0 || i>=num_partitions)
      ? "Invalid Partition\r\n";
    else {
      p=&local_partitions[i];
      if (i<2) {
	ch=i+'A';
	? "Insert Diskette in Drive ",ch,"\r\n";
	default_partition=i;
      } else {
	if (p->type!=0x0B)
	  ? "Only FAT32 hard drive partitions allowed.\r\n";
	else
	  default_partition=i;
      }
    }
  }
  drive('A'+default_partition);
  cd("/HOPPY/KERNEL/CPP");
}
defext(ext_select_default_drive,&select_default_drive);







