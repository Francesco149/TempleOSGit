//spaces to tabs and removes trailing white space
void s2t_file(char *name,char *flags)
{
  char *buf=load_text_file(name);
  int i,changed=0,col,space_count,space_count2;
  char *src,*src2,*dst;
  linked_text_file *l=new_linked_text_file;
  ltf_line *cl;
  to_linked_text_file(buf,l);
  cl=l->next;
  while (cl!=l) {
    col=0;
    src=cl->data;
    dst=cl->data;
    while (*src) {
      src2=src;
      space_count=0;
      while (*src2==0x20) {
	space_count++;
	src2++;
      }
      if (space_count<2) {
	if (*src==9)
	  col=0;
	else {
	  col++;
	  if (col==8)
	    col=0;
	}
	*(dst++)=*(src++);
      } else {
	while (col+space_count>=8) {
	  space_count2=8-col;
	  if (space_count2==1)
	    *(dst++)=0x20;
	  else {
	    *(dst++)=9;
	    changed=changed+(space_count2-1);
	  }
	  space_count=space_count-space_count2;
	  col=0;
	}
	for (i=0;i<space_count;i++)
	  *(dst++)=0x20;
	col=space_count;
	src=src2;
      }
    }
    *(dst--)=0;

    while ((dst>=cl->data) && (*dst==0x20 || *dst==9))
      *(dst--)=0;
    cl=cl->next;
  }
  if (changed) {
    printf("Compressed %4d in file %s\r\n",changed,name);
    strcpy(l->filename.name,name);
    if (!occurrences(flags,'n') && !occurrences(flags,'N'))
      edit_save_ltf(l,false);
  }
  delete_ltf(l);
  free(buf);
}

DWORD recurse_s2t;
void s2t_list(my_dir_entry *tempm1,char *flags)
{
  char *buf3;
  my_dir_entry *tempm2;
  void my_s2t_list(my_dir_entry *tempm1,char *flags);

  my_s2t_list=recurse_s2t;
  buf3=new_string(fs->current_directory);
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & my_attr_directory) {
      if (tempm1->sub) {
	cd(tempm1->name);
	? "Scanning Directory:",fs->current_directory,"\r\n";
	my_s2t_list(tempm1->sub,flags);
	cd(buf3);
      }
    } else
      s2t_file(tempm1->name,flags);
    free(tempm1);
    tempm1=tempm2;
  }
  free(buf3);
}
recurse_s2t=&s2t_list;

void s2t(char *wild,char *flags) //flags r=recurse n=no save
{
  char buf[128],buf2[512],*buf3,buf4[64];
  int old_partition=-1;
  my_dir_entry *tempm1=NULL;
  bool recurse;

  if (occurrences(flags,'r') || occurrences(flags,'R'))
    recurse=true;
  else
    recurse=false;

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

  tempm1=find_files(fs->current_partition,fs->current_dir_cluster,buf,recurse);
  s2t_list(tempm1,flags);

  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
}


