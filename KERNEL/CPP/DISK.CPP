//This changes drives.  Include a letter in single quotes.
void drive(char new_partition)
{
  char d;
  my_partition_entry *p;
  if (new_partition<0x60)
    fs->current_partition=new_partition-'A';
  else
    fs->current_partition=new_partition-'a';
  if (fs->current_partition>num_partitions)  {
    fs->current_partition=0;
    ? systext(st_invalid_drive);
  }
  d='A'+fs->current_partition;
  ? systext(st_current_partition),d,"\r\n";
  free(fs->current_directory);
  fs->current_directory=new_string("/");
  p=&local_partitions[fs->current_partition];
  if (p->type=0x0B) {
    fs->current_dir_cluster=p->cur_FAT32_br->root_cluster;
  }
}

void dir(char *wild="*")
{
  dir_FAT32(wild);
}

void del(char *wild)
{
  del_FAT32_files(wild);
}

void cd(char *name)
{
  char buf[128],buf2[512],buf3[512];
  my_partition_entry *p=&local_partitions[fs->current_partition];
  if (*name=='/') {
    fs->current_dir_cluster=p->cur_FAT32_br->root_cluster;
    free(fs->current_directory);
    fs->current_directory="/";
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
      else
        cd_FAT32("..");
    } else if (!strcmp(buf,".")) {
      ;
    } else if (*buf) {
        cd_FAT32(buf);
        if (strcmp(buf3,"/"))
          strcat(buf3,"/");
        strcat(buf3,buf);
    }
  }
  fs->current_directory=new_string(buf3);
}


BYTE *read_file(char *filename,DWORD *size=NULL)
{
  char buf[128],buf2[512],*buf3;
  BYTE *result;
  my_partition_entry *p=&local_partitions[fs->current_partition];
  buf3=new_string(fs->current_directory);
  if (*filename=='/') {
    cd("/");
    filename++;
  }
  strcpy(buf2,filename);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  result=read_FAT32_file(buf,size);
  cd(buf3);
  free(buf3);
  return result;
}
defext(ext_read_file,&read_file); //Redefined


void save_file(char *filename,BYTE *fbuf,DWORD size)
{
  char buf[128],buf2[512],*buf3;
  my_partition_entry *p=&local_partitions[fs->current_partition];

  buf3=new_string(fs->current_directory);
  if (*filename=='/') {
    cd("/");
    filename++;
  }
  strcpy(buf2,filename);
  remove_last_segment(buf2,'/',buf);
  if (*buf2)
    cd(buf2);
  save_FAT32_file(buf,fbuf,size);
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

 
 
 
 
 
 
 
 