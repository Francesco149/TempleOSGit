void d_common(BYTE *buf,DWORD count,bool memory)  //Hex Dump
{
  DWORD i,j,k;
  for (i=0;i<count;i=i+16) {
    if (memory) 
      ? buf+i," ";
    else
      ? i," ";
    for (j=0;j<16;j++) {
      if (j==8) ? "- ";
      k=i+j;
      if (k<count) 
        ? buf[k]," ";
      else
        ? "   ";
    }
    space;
    for (j=0;j<16;j++) {
      k=i+j;
      if (k<count) {
        if (buf[k]>=0x20 && buf[k]<=0x7E)
          putchar(buf[k]);
        else
          ? ".";
      }
    }
    crlf;
  }
}

void dm(BYTE *buf,DWORD count=0x80)
{
  d_common(buf,count,true);
}

void d(BYTE *buf,DWORD count=0x80)
{
  d_common(buf,count,false);
}

void patch(DWORD block)
{
  BYTE buf[BLOCK_SIZE];
  rblocks(buf,block,1);
  d(buf,BLOCK_SIZE);
}

void press_a_key()
{
  ? systext(st_press_a_key);
  getkey;
}

BYTE *sym(char *name)
{
  sys_hash_entry *h=find_hash_entry(name,fs->hash_table,HTT_SYS_SYMBOL);
  return h->user0;
}

void split()
{
  int i;
  tss_struct *user2_tss;
  cursor_off;
  user2_tss=spawn(COMMAND_LINE,"USER CMD PROMPT");
  for (i=0;i<10;i++) 
    swap_in_next;
  fs->window_top=40;
  fs->text_attribute=0x37;
  fs->border_attribute=0x3F;
  fs->menu_attribute=0x2F;
  clear_crt;
  cursor_home;
  user2_tss->window_bottom=39;
  bts(&fs->task_flags,TSSF_SUSPENDED);
}

void shell()
{
  spawn_user_and_activate;
}

DWORD to_hex8(char *st)
{
  DWORD result=0,i;
  char ch;
  for (i=0;i<8;i++) {
    result=result*16;
    ch=st[i];
    if (ch>='0' && ch <='9')
      result=result+ch-'0';
    else if (ch>='A' && ch<='F')
      result=result+ch-'A'+10;
    else if (ch>='a' && ch<='f')
      result=result+ch-'a'+10;
  }
  return result;
}


void taskstat_line(tss_struct *tss)
{
  ? tss," ",tss->task_descriptor,"\r\n";
}

void taskstat()
{
  tss_struct *tss=root_tss;
  taskstat_line(tss);
  tss=tss->next_tss;
  while (tss!=root_tss) {
    taskstat_line(tss);
    tss=tss->next_tss;
  }
}

defext(ext_dump,&d);



 
 