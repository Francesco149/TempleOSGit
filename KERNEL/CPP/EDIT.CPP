#define ltflf_selected 0

class ltf_line
{
  ltf_line *next,*last;
  DWORD flags;
  BYTE *data;
};

#define ltff_line_nums 1

class linked_text_file 
{
  ltf_line *next,*last;
  char *filename;
  DWORD flags;
  DWORD line_start_col,line_end_col;
  char *to_text_cb(char *st,int cursor_col_in,int *cursor_col_out);
  void calc_cur_data_col_cb(linked_text_file *l,tss_struct *tss);
  DWORD cur_col,cur_data_col;
  ltf_line *cur_line;
  DWORD cur_top_line_num;
  ltf_line *insert_point;
  ltf_line *cur_top_line;
  DWORD num_lines,max_lines;
};

void sysfun CMD_TO_LINKED_TEXT_FILE to_linked_text_file(char *st,linked_text_file *l);

void delete_ltf(linked_text_file *l)
{
  ltf_line *cl,*cl1;
  if (!l) return;
  free(l->filename);
  cl=l->next;
  while (cl!=l) {
    cl1=cl->next;
    free(cl->data);
    free(cl);
    cl=cl1;
  }
  free(l);
}

char *pretruncate_string(DWORD count, char *st)
{
  DWORD l=strlen(st);
  if (count>l) count=l;
  return st+count;
}

char *standard_to_text(char *st,int cursor_col_in,int *cursor_col_out)
{
  char *buf,*buf2,*src=st,*dst,ch;
  int i=0,buf_size=512;
  bool cont=true;

  buf=malloc(buf_size);
  dst=buf;
  *cursor_col_out=0;
  while (cont) {
    while (i<buf_size-16 && (ch=*src++)) {
      if (src-1-st==cursor_col_in) 
        *cursor_col_out=i;
      if (ch==9) { //tab
        *dst++=32;
        i++;
        while (i & 7) {
          *dst++=32;
          i++;
        }
      } else if (ch>31 && ch<127) {
        *dst++=ch;
        i++;
      }
    }
    if (ch) {
      src--;
      buf2=malloc(buf_size*2);
      memcpy(buf2,buf,buf_size);
      dst=dst-buf+buf2;
      free(buf);
      buf=buf2;
      buf_size=buf_size*2;
    } else
      cont=false;
  }
  if (src-1-st==cursor_col_in) 
    *cursor_col_out=i;
  *dst++=0;

  buf2=new_string(buf);
  free(buf);
  return buf2;
}


//Note: due to hidden chars (tab) cur_col!=cur_data_col
//This is not complete and will change eventually 
void standard_calc_cur_data_col(linked_text_file *l,tss_struct *tss)
{
  int cc;
  if (l->flags & ltff_line_nums) 
    l->cur_data_col=l->cur_col-5;
  else
    l->cur_data_col=l->cur_col;
  if (l->cur_data_col>strlen(l->cur_line->data))
    l->cur_data_col=strlen(l->cur_line->data);       
  free(l->to_text_cb(l->cur_line->data,l->cur_data_col,&cc));
  if (cc<l->line_start_col)
    l->line_start_col=cc;
  else {
    if (l->flags & ltff_line_nums) 
      cc=cc+5;
    if (cc-l->line_start_col>
        (tss->window_right-tss->window_left)/2)
      l->line_start_col=cc-(tss->window_right-tss->window_left)/2;
  }
}

DWORD get_key_or_menu(DWORD *scan_code,char *ch)
{ //Returns menu selection line number
  DWORD result;
  btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
  fs->menu_selection=0;  //clear menu
  while (!bt(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY) &&
         !fs->menu_selection)
    swap_in_next;
  *scan_code=*SYS_CURRENT_SCAN_CODE;
  *ch=translate_scan_code;
  result=fs->menu_selection;
  btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
  fs->menu_selection=0;  //clear menu
  return result;
}


linked_text_file *new_linked_text_file()
{
  linked_text_file *l;
  l=malloc(sizeof(linked_text_file));
  l->next=l;
  l->last=l;
  l->filename=NULL;
  l->insert_point=l;
  l->flags=0;
  l->cur_col=0;
  l->cur_data_col=0;
  l->line_start_col=0;
  l->line_end_col=79;
  l->cur_line=l;
  l->to_text_cb=&standard_to_text;
  l->calc_cur_data_col_cb=&standard_calc_cur_data_col;
  l->num_lines=0;
  l->max_lines=0xFFFFFFFF;
  return l;
}

char hex_digits[17];
strcpy(hex_digits,"0123456789ABCDEF");

void BYTE_to_st(BYTE b;char *buf)
{
  int i;
  for (i=0;i<2;i++) {
    buf[1-i]=hex_digits[b & 15];
    b=b>>4;
  }
  buf[2]=0;
}

void WORD_to_st(WORD w;char *buf)
{
  int i;
  for (i=0;i<4;i++) {
    buf[3-i]=hex_digits[w & 15];
    w=w>>4;
  }
  buf[4]=0;
}

void DWORD_to_st(DWORD d;char *buf)
{
  int i;
  for (i=0;i<8;i++) {
    buf[7-i]=hex_digits[d & 15];
    d=d>>4;
  }
  buf[8]=0;
}


void ltf_update_screen(linked_text_file *l)
{
  int cc,vert,c_vert,c_hor,bottom,top;
  ltf_line *cl;
  char *temp,*temp2,buf[5],*temp3;
  WORD line_num=l->cur_top_line_num;

  clear_crt;
  cursor_home;
  top=fs->window_top;
  cl=l->cur_top_line;
  cursor_off;
  while (top<0) {
    top++;
    line_num++;
    fs->cursor_offset=fs->cursor_offset+fs->crt_width;
    fs->cursor_vert++;
    cl=cl->next;
  }
  if (fs->window_bottom<fs->crt_height)
    bottom=fs->window_bottom;
  else
    bottom=fs->crt_height-1;
  for (vert=top;(vert<bottom) && (cl!=l); cl=cl->next,vert++,line_num++) {
    temp=l->to_text_cb(cl->data,l->cur_data_col,&cc);
    cc=cc-l->line_start_col;
    temp2=pretruncate_string(l->line_start_col,temp);
    if (cl==l->cur_line) {
      c_hor=cc*2+fs->window_left;
      c_vert=vert;
    }
    if (l->flags & ltff_line_nums) {
      fs->cursor_hor=0;
      fs->cursor_offset=fs->cursor_vert*fs->crt_width+
                        2*fs->cursor_hor+fs->window_left; 
      WORD_to_st(line_num,buf);
      temp3=malloc(5+strlen(temp2)+1);
      strcpy(temp3,buf);
      strcat(temp3," ");
      strcat(temp3,temp2);
      temp2=temp3;
      free(temp);
      temp=temp3;
    }
    if (bt(&cl->flags,ltflf_selected))
      fs->text_attribute=fs->text_attribute ^ 0xFF;
    crt_line_out(temp2);
    if (bt(&cl->flags,ltflf_selected))
      fs->text_attribute=fs->text_attribute ^ 0xFF;
    free(temp);
  }
  if (l->flags & ltff_line_nums)
    c_hor=c_hor+5*2;
  fs->cursor_offset=c_hor+c_vert*fs->crt_width;
  fs->cursor_vert=c_vert;
  fs->cursor_hor=c_hor;
  cursor_on;
}

void edit_line_up(linked_text_file *l,DWORD sc)
{
  ltf_line *cl=l->cur_line;

  if (bt(&sc,SCF_CTRL))
    bts(&cl->flags,ltflf_selected);
  else
    btr(&cl->flags,ltflf_selected);

  if (cl==l->cur_top_line) {
    cl=cl->last;
    if (cl==l) 
      cl=cl->next;
    else {
      l->cur_top_line_num--;
      l->cur_top_line=cl;
    }
  } else
    cl=cl->last;
  l->cur_line=cl;

  if (l->flags & ltff_line_nums) 
    l->cur_col=l->cur_data_col+5;
  else
    l->cur_col=l->cur_data_col;
  l->calc_cur_data_col_cb(l,fs);
}

void edit_line_down(linked_text_file *l,DWORD sc)
{
  ltf_line *cl=l->cur_line; 
  int i;

  if (bt(&sc,SCF_CTRL))
    bts(&cl->flags,ltflf_selected);
  else
    btr(&cl->flags,ltflf_selected);

  for (i=fs->window_top,cl=l->cur_top_line;cl!=l->cur_line;cl=cl->next) 
    i++;
  cl=cl->next;
  if (i==fs->window_bottom-1) {
    if (cl==l) 
      cl=cl->last;
    else {
      l->cur_top_line=l->cur_top_line->next;
      l->cur_top_line_num++;
    }
  } else {
    if (cl==l)
      cl=cl->last;
  }
  l->cur_line=cl;

  if (l->flags & ltff_line_nums) 
    l->cur_col=l->cur_data_col+5;
  else
    l->cur_col=l->cur_data_col;
  l->calc_cur_data_col_cb(l,fs);
}

void edit_delete_char(linked_text_file *l)
{
  char *b;
  ltf_line *cl=l->cur_line,*nl;

  if (l->cur_data_col==strlen(cl->data)) {
    nl=l->cur_line->next;
    if (nl!=l) {
      b=malloc(strlen(cl->data)+strlen(nl->data)+1);
      strcpy(b,cl->data);
      strcat(b,nl->data);
      free(cl->data);
      cl->data=b;
      free(nl->data);
      remque(nl);
      free(nl);
    }
  } else 
    strcpy(cl->data+l->cur_data_col,cl->data+l->cur_data_col+1);
  
}

void edit_insert_char(char ch,linked_text_file *l)
{
  int i;
  char *b,*src,*dst;
  ltf_line *cl=l->cur_line,*nl,*templ;

  if (ch==13) {
    nl=malloc(sizeof(ltf_line));
    nl->flags=0;
    l->num_lines++;
    if (l->max_lines && l->num_lines>=l->max_lines) {
      l->num_lines--;
      templ=l->next;
      if (l->cur_top_line==templ)
        l->cur_top_line=templ->next;
      if (l->cur_line==templ)
        l->cur_line=templ->next;
      remque(templ);
      free(templ);
    }
    dst=cl->data+l->cur_data_col;
    nl->data=new_string(dst);
    *dst=0;
    insque(nl,cl);
    l->cur_line=nl;
    l->cur_data_col=0;
    l->line_start_col=0;
    i=fs->window_top;
    templ=l->cur_top_line;
    while (templ!=l->cur_line) {
      i++;
      templ=templ->next;
    }
    if (i>=fs->window_bottom) {
      l->cur_top_line=l->cur_top_line->next;
      l->cur_top_line_num++;
    }
  } else {
    if ((ch>31 && ch<127) || ch==9) {
      b=malloc(strlen(cl->data)+2);
      dst=b;
      src=cl->data;
      i=l->cur_data_col;
      while (i-->0) 
        *dst++=*src++;
      *dst++=ch;
      while (*dst++=*src++);
      free(cl->data);
      cl->data=b;
      l->cur_data_col++;
    }
  }
}

void edit_delete_line(linked_text_file *l)
{
  ltf_line *cl,*new_cl;
  DWORD top_line_num=l->cur_top_line_num;
  cl=l->cur_line;
  new_cl=cl->next;
  if (new_cl==l) {
    new_cl=cl->last;
    top_line_num--;
  }
  if (new_cl!=l) {
    l->cur_line=new_cl;
    if (l->cur_top_line==cl) {
      l->cur_top_line=new_cl;
      l->cur_top_line_num=top_line_num;
    }
    remque(cl);
    free(cl->data);
    free(cl);
  }
}

char *edit_save(linked_text_file *l)
{
  ltf_line *cl;
  DWORD count;
  char *result,*dst,*src;

  for (count=1,cl=l->next;cl!=l;cl=cl->next)
    count=count+strlen(cl->data)+2;
  result=malloc(count);
  dst=result;
  for (cl=l->next;cl!=l;cl=cl->next) {
    src=cl->data;
    while (*dst++=*src++);
    dst--;
    *dst++=13;
    *dst++=10;
  } 
  *dst=0;
  return result;
}

void delete_clipboard()
{
  linked_text_file *b=*SYS_CLIPBOARD_LTF;
  ltf_line *cl1,*cl=b->next;
  while (cl!=b) {
    cl1=cl->next;
    remque(cl);
    free(cl->data);
    free(cl);
    cl=cl1;
  }
}

void edit_copy_to_clipboard(linked_text_file *l)
{
  linked_text_file *b=*SYS_CLIPBOARD_LTF;
  ltf_line *cl=l->next,*nl;
  delete_clipboard();
  while (cl!=l) {
    if (bt(&cl->flags,ltflf_selected)) {
      nl=malloc(sizeof(ltf_line));
      nl->flags=0;
      nl->data=new_string(cl->data);
      insque(nl,b->last);
    }
    cl=cl->next;
  }
}

void edit_cut_to_clipboard(linked_text_file *l)
{
  linked_text_file *b=*SYS_CLIPBOARD_LTF;
  ltf_line *cl=l->next,*cl1,*nl;
  delete_clipboard();
  while (cl!=l) {
    cl1=cl->next;
    if (bt(&cl->flags,ltflf_selected)) {
      l->cur_line=cl;
      nl=malloc(sizeof(ltf_line));
      nl->flags=0;
      nl->data=new_string(cl->data);
      insque(nl,b->last);
      edit_delete_line(l);
    }
    cl=cl1;
  }
}

void edit_paste_clipboard(linked_text_file *l)
{
  linked_text_file *b=*SYS_CLIPBOARD_LTF;
  ltf_line *nl,*cl=b->next; 
  while (cl!=b) {
    nl=malloc(sizeof(ltf_line));
    nl->flags=0;
    nl->data=new_string(cl->data);
    insque(nl,l->cur_line->last);
    l->num_lines++;
    if (l->num_lines>l->max_lines)
      edit_delete_line(l->next);
    cl=cl->next;
  }
}

#define MU_FIND        1
#define MU_REPLACE     2
#define MU_SAVE        3
#define MU_SAVE_AS     4
#define MU_ABORT       5
#define MU_EXIT        6
#define MU_TOGGLE_NUMS 7
#define MU_CUT         8
#define MU_COPY        9
#define MU_PASTE      10

char *ltf_putchar(linked_text_file *l,char ch,DWORD sc,DWORD ms)
{
  int sc2,i,old_flags=get_flags;
  cli;
  if (ch!=27 && ch!=0x11) {
    if (l->flags & ltff_line_nums) 
      l->cur_col=l->cur_data_col+5;
    else
      l->cur_col=l->cur_data_col;
    l->calc_cur_data_col_cb(l,fs);

    sc2=sc & 0x7F;
    if (sc2==SC_CURSOR_DOWN)
      edit_line_down(l,sc);
    else if (sc2==SC_CURSOR_UP)
      edit_line_up(l,sc);
    else if (sc2==SC_PAGE_DOWN) {
      i=fs->window_bottom-fs->window_top;
      while (i-- >0) 
        edit_line_down(l,sc);
    } else if (sc2==SC_PAGE_UP) {
      i=fs->window_bottom-fs->window_top;
      while (i-- >0) 
        edit_line_up(l,sc);
    } else if (sc2==SC_CURSOR_LEFT) {
      if (l->cur_data_col>0) 
        l->cur_data_col--;
      else {
        edit_line_up(l,sc);
        l->cur_data_col=strlen(l->cur_line->data);
      }
    } else if (sc2==SC_CURSOR_RIGHT) {
      l->cur_data_col++;
    } else if (sc2==SC_DELETE)
      edit_delete_char(l);
    else if (ch==CH_BACKSPACE) {
      if (l->cur_data_col==0) {
        l->cur_line=l->cur_line->last;
        if (l->cur_line==l) 
          l->cur_line=l->cur_line->next;
        else {
          l->cur_data_col=strlen(l->cur_line->data);
          edit_delete_char(l);
        }
      } else {
        l->cur_data_col--;
        edit_delete_char(l);
      }
      
    } else if (ch==25) //ctrl Y
      edit_delete_line(l);
    else if (ch==14 || ms==MU_TOGGLE_NUMS)   //ctrl N
      l->flags=l->flags ^ ltff_line_nums;
    else if (ch==3 || ms==MU_COPY) //CTRL C
      edit_copy_to_clipboard(l);
    else if (ch==24 || ms==MU_CUT) //CTRL X
      edit_cut_to_clipboard(l);
    else if (ch==22 || ms==MU_PASTE) //CTRL V
      edit_paste_clipboard(l);
    else if (ms==0)
      edit_insert_char(ch,l);

    if (l->flags & ltff_line_nums) 
      l->cur_col=l->cur_data_col+5;
    else
      l->cur_col=l->cur_data_col;
    l->calc_cur_data_col_cb(l,fs);
  }
  set_flags(old_flags);
}

char *edit_string_file(char *st, DWORD *save_flag)
{
  char ch=0,*result,*temp;
  DWORD sc=0,ms=0,old_menu_attr;
  linked_text_file *old_l,*old_menu,*l=new_linked_text_file,*m=new_linked_text_file;
  int i;

  if (!strlen(st))
    to_linked_text_file("\r\n",l);
  else
    to_linked_text_file(st,l);
  old_l=fs->console_ltf;
  old_menu=fs->menu_ltf;
  old_menu_attr=fs->menu_attribute;
  
  temp=load_text_file("/HOPPY/KERNEL/CPP/EDIT.MEN");
  to_linked_text_file(temp,m);
  m->line_start_col=5;
  m->cur_data_col=5;
  free(temp);

  fs->menu_ltf=m;
  fs->console_ltf=l;
  while (ch!=27 && ch!=0x11 && ms!=MU_ABORT && ms!=MU_EXIT) {
    ms=get_key_or_menu(&sc,&ch);
    ltf_putchar(l,ch,sc,ms);
  }
  fs->console_ltf=old_l;
  fs->menu_ltf=old_menu;
  delete_ltf(m);
  if (ch==0x11 || ms==MU_ABORT) {
    *save_flag=0;
    delete_ltf(l);
    return 0;
  } else {
    *save_flag=1;
    result=edit_save(l);
    delete_ltf(l);
    return result;
  }
}

void edit(char *filename)
{
  char *st_in,*st_out;
  DWORD save_flag;

  st_in=load_text_file(filename);
  st_out=edit_string_file(st_in,&save_flag);
  clear_crt;
  cursor_home;
  if (save_flag)
    save_text_file(filename,st_out);
  free(st_in);
  free(st_out);
}

void use_console_ltf(char *menu_file)
{
  DWORD old_flags=get_flags;
  char *st;
  linked_text_file *l;
  cli;
  fs->console_ltf=new_linked_text_file;
  l=fs->console_ltf;
  to_linked_text_file("\r\n",l);
  l->max_lines=200;

  if (menu_file) {
    fs->menu_ltf=new_linked_text_file;
    l=fs->menu_ltf;
    l->line_start_col=5;
    l->cur_data_col=5;
    st=load_text_file(menu_file);
    to_linked_text_file(st,l);
    free(st);
  }
  set_flags(old_flags);
}
 
defext(ext_ltf_putchar,&ltf_putchar);
defext(ext_ltf_update_screen,&ltf_update_screen);
 
 
 
 
 
 
 
 
 
 
 
