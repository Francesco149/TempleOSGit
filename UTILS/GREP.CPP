void grep_file(char *pattern,char *name,bool ignore_case)
{
  char *buf=load_text_file(name);
  linked_text_file *l=new_linked_text_file;
  ltf_line *cl;
  int line_num=1;
  to_linked_text_file(buf,l);
  cl=l->next;
  while (cl!=l) {
    if (ignore_case) {
      if (stristr(cl->data,pattern))
	printf("%12s,%04X %s\r\n",name,line_num,cl->data);
    } else {
      if (strstr(cl->data,pattern))
	printf("%12s,%04X %s\r\n",name,line_num,cl->data);
    }
    cl=cl->next;
    line_num++;
  }
  delete_ltf(l);
  free(buf);
}

DWORD recurse_gl;
void grep_list(char *pattern,my_dir_entry *tempm1,char *flags)
{
  char *buf3;
  my_dir_entry *tempm2;
  bool ignore_case;
  void my_grep_list(char *pattern,my_dir_entry *tempm1,char *flags);

  my_grep_list=recurse_gl;
  if (occurrences(flags,'i') || occurrences(flags,'I'))
    ignore_case=true;
  else
    ignore_case=false;
  buf3=new_string(fs->current_directory);
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & my_attr_directory) {
      if (tempm1->sub) {
	cd(tempm1->name);
	? "Scanning Directory:",fs->current_directory,"\r\n";
	my_grep_list(pattern,tempm1->sub,flags);
	cd(buf3);
      }
    } else
      grep_file(pattern,tempm1->name,ignore_case);
    free(tempm1);
    tempm1=tempm2;
  }
  free(buf3);
}
recurse_gl=&grep_list;

void grep(char *pattern,char *wild="*.ASM;*.CPP;*.TXT",char *flags="ri") //r=recurse i=ignore case
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
  grep_list(pattern,tempm1,flags);

  if (old_partition>=0)
    drive(old_partition+'A');
  cd(buf3);
  free(buf3);
}


