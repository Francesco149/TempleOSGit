#define EDIT_MENU_NAME "/HOPPY/KERNEL/CPP/EDIT.MEN"
#define ltflf_selected 0

class ltf_line
{
  ltf_line *next,*last;
  DWORD flags;
  BYTE *data;
};

class edit_find_text_record
{
  char	find_text[132]	    format_str "FIND   :%s";
  char	replace_text[132]   format_str "REPLACE:%s";
  DWORD scan_forward	    format_str "FORWARD:  %1d";
  DWORD scan_selected_text  format_str "SELECTION:%1d";
  DWORD match_case	    format_str "CASE     :%1d";
};

class edit_filename_record
{
  char name[256] format_str "FileName:%s";
};

#define ltff_line_nums	   1
#define ltff_line_nums_dec 2
#define ltff_overstrike    4

class linked_text_file
{
  ltf_line *next,*last;
  DWORD flags;
  DWORD line_start_col,line_end_col;
  char *to_text_cb(char *st);
  int calc_cur_data_col_cb(linked_text_file *l,tss_struct *tss);
  int calc_cur_col_cb(linked_text_file *l,tss_struct *tss);
  DWORD cur_col,cur_data_col;
  ltf_line *cur_line;
  DWORD cur_top_line_num;
  ltf_line *insert_point;
  ltf_line *cur_top_line;
  DWORD num_lines,max_lines;
  edit_find_text_record *find_replace;
  ltf_line *base_select,*limit_select;
  linked_text_file *menu_ltf;
  BYTE text_attribute,reserved1[3];
  edit_filename_record filename;
};

void sysfun CMD_TO_LINKED_TEXT_FILE to_linked_text_file(char *st,linked_text_file *l);

DWORD recurse_dltf;
void delete_ltf(linked_text_file *l)
{
  void my_delete_ltf(linked_text_file *l);
  ltf_line *cl,*cl1;
  my_delete_ltf=recurse_dltf;

  if (!l) return;
  cl=l->next;
  while (cl!=l) {
    cl1=cl->next;
    free(cl->data);
    free(cl);
    cl=cl1;
  }
  free(l->find_replace);
  my_delete_ltf(l->menu_ltf);
  free(l);
}
recurse_dltf=&delete_ltf;

char *pretruncate_string(DWORD count, char *st)
{
  DWORD l=strlen(st);
  if (count>l) count=l;
  return st+count;
}

char *standard_to_text(char *st)
{
  char *buf,*buf2,*src=st,*dst,ch;
  int i=0,buf_size=512;
  bool cont=true;

  buf=malloc(buf_size);
  dst=buf;
  while (cont) {
    while (i<buf_size-16 && (ch=*src++)) {
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
  *dst++=0;

  buf2=new_string(buf);
  free(buf);
  return buf2;
}

int standard_calc_cur_col(linked_text_file *l,tss_struct *tss)
//This converts cur_col to cur_data_col
{
  int i,cc=0;
  char ch,*ptr=l->cur_line->data;
  for (i=0;i<l->cur_data_col;i++) {
    ch=ptr[i];
    if (ch==9) 
      cc=(cc+8) & 0xFFFFFFF8;
    else if (ch>31 && ch<127)
      cc++;
  }
  if (l->flags & ltff_line_nums)
    cc=cc+5;
  if (cc<l->line_start_col) {
    l->line_start_col=cc;
    cc=0;
  } else {
    if (cc-l->line_start_col>
	(tss->window_right-tss->window_left)/2) 
      l->line_start_col=cc-(tss->window_right-tss->window_left)/2;
    cc=cc-l->line_start_col;
  }
  return cc;
}

int standard_calc_cur_data_col(linked_text_file *l,tss_struct *tss)
//This converts cur_col to cur_data_col
{
  int i,cc,cdc;
  bool cont=true;
  char ch,*ptr=l->cur_line->data;

  cc=l->cur_col+l->line_start_col;
  if (l->flags & ltff_line_nums)
    cc=cc-5;

  i=0;
  while ((ch=*(ptr++)) && cont) {
    if (ch==9) 
      i=(i+8) & 0xFFFFFFF8;
    else if (ch>31 && ch<127)
      i++;
    if (i>cc) { 
      cdc=ptr-1-l->cur_line->data;
      cont=false;
    }
  }
  if (cont)
    cdc=strlen(l->cur_line->data);
  return cdc;
}



linked_text_file *new_linked_text_file()
{
  linked_text_file *l;
  l=malloc(sizeof(linked_text_file));
  l->next=l;
  l->last=l;
  strcpy(l->filename.name,"TEMP.TXT");
  l->insert_point=l;
  l->flags=0;
  l->cur_col=0;
  l->cur_data_col=0;
  l->line_start_col=0;
  l->line_end_col=79;
  l->cur_line=l;
  l->to_text_cb=&standard_to_text;
  l->calc_cur_data_col_cb=&standard_calc_cur_data_col;
  l->calc_cur_col_cb=&standard_calc_cur_col;
  l->num_lines=0;
  l->max_lines=0xFFFFFFFF;
  l->find_replace=malloc(sizeof(edit_find_text_record));
  l->find_replace->find_text[0]=0;
  l->find_replace->replace_text[0]=0;
  l->find_replace->scan_forward=1;
  l->find_replace->scan_selected_text=0;
  l->find_replace->match_case=1;
  l->menu_ltf=NULL;
  l->text_attribute=(WHITE<<4)+BLUE;
  return l;
}


void ltf_update_window(linked_text_file *l)
{
  int line_num,vert,c_vert,c_hor,bottom,top;
  ltf_line *cl;
  char *temp,*temp2,buf[8],*temp3;

  if (!l) return;
  line_num=l->cur_top_line_num;

  clear_crt;
  cursor_home;
  top=fs->window_top;
  cl=l->cur_top_line;
  cursor_off;
  c_hor=l->calc_cur_col_cb(l,fs)*2+fs->window_left;
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
    temp=l->to_text_cb(cl->data);
    if (cl==l->cur_line) 
      c_vert=vert;
    temp2=pretruncate_string(l->line_start_col,temp);
    if (l->flags & ltff_line_nums) {
      if (l->flags & ltff_line_nums_dec)
	sprintf(buf,"%04d ",line_num);
      else
	sprintf(buf,"%04X ",line_num);
      temp3=malloc(5+strlen(temp2)+1);
      strcpy(temp3,buf);
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
  fs->cursor_offset=c_hor+c_vert*fs->crt_width;
  fs->cursor_vert=c_vert;
  fs->cursor_hor=c_hor;
  cursor_on;
}

void ltf_update_current_ltf_window()
{
  ltf_update_window(fs->current_ltf);
}


void edit_line_up(linked_text_file *l,DWORD sc)
{
  ltf_line *cl=l->cur_line;

  l->cur_col=l->calc_cur_col_cb(l,fs);  
  if (bt(&sc,SCF_SHIFT))
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

  l->cur_data_col=l->calc_cur_data_col_cb(l,fs);
  l->cur_col=l->calc_cur_col_cb(l,fs);  
}

void edit_line_down(linked_text_file *l,DWORD sc)
{
  ltf_line *cl=l->cur_line;
  int i;

  l->cur_col=l->calc_cur_col_cb(l,fs);  
  if (bt(&sc,SCF_SHIFT))
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

  l->cur_data_col=l->calc_cur_data_col_cb(l,fs);
  l->cur_col=l->calc_cur_col_cb(l,fs);  
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

  if (l->flags & ltff_overstrike) {
    if (ch==13) {
      l->cur_line=cl->next;
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
	if (!cl->data[l->cur_data_col]) {
	  b=malloc(strlen(cl->data)+2);
	  strcpy(b,cl->data);
	  free(cl->data);
	  cl->data=b;
	  cl->data[l->cur_data_col+1]=0;
	}
	cl->data[l->cur_data_col]=ch;
	l->cur_data_col++;
      }
    }
    return;
  }

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
  DWORD count=1;
  char *result,*dst,*src;

  for (cl=l->next;cl!=l;cl=cl->next)
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
    if (btr(&cl->flags,ltflf_selected)) {
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
    if (btr(&cl->flags,ltflf_selected)) {
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
    if (l->num_lines>l->max_lines) {
      nl=l->cur_line;
      l->cur_line=l->next;
      if (nl!=l->cur_line) {
	edit_delete_line(l);
	l->cur_line=nl;
      } else
	edit_delete_line(l);
    }
    cl=cl->next;
  }
}

bool do_ltf(linked_text_file *l)
{
  linked_text_file *old_ltf=fs->current_ltf;
  DWORD sc=0;
  int ms=0,old_attribute=fs->text_attribute;
  char ch;
  void my_ltf_putchar(linked_text_file *l,char ch, DWORD sc);
  DWORD *ext=*SYS_EXTERN_TABLE;
  DWORD old_update=fs->update_window;
  my_ltf_putchar=ext[ext_ltf_putchar];


  fs->current_ltf=l;
  fs->text_attribute=l->text_attribute;
  if (l->menu_ltf)
    bts(&fs->crt_flags,CRTF_HAS_MENU);
  else
    btr(&fs->crt_flags,CRTF_HAS_MENU);
  fs->update_window=&ltf_update_current_ltf_window;
  while (ch!=27 && ch!=0x11 && ms!=MU_EXIT && ms!=MU_ABORT) {
    ch=getchar2(&sc);
    ms=my_ltf_putchar(l,ch,sc);
  }
  fs->current_ltf=old_ltf;
  if (old_ltf->menu_ltf)
    bts(&fs->crt_flags,CRTF_HAS_MENU);
  else
    btr(&fs->crt_flags,CRTF_HAS_MENU);
  fs->text_attribute=old_attribute;
  fs->update_window=old_update;
  if (ch==27 || ms==MU_EXIT)
    return true;
  else
    return false;
}


bool do_form(BYTE *d,char *class_name)
{
  bool result=false;
  member_list_node *ml;
  ltf_line *ll;
  int *i_ptr;
  class_node *cn;
  char buf[512],ch=0;
  class_node *cl=find_local_hash(class_name,HTT_CLASS);
  linked_text_file *l;
  if (!cl) return false;
  l=new_linked_text_file;
  to_linked_text_file("\r\n",l);
  l->flags=l->flags | ltff_overstrike;
  ml=cl->member_list;
  while (ml) {
    if (ml->format_str) {
      ll=malloc(sizeof(ltf_line));
      i_ptr=d+ml->offset;
      cn=ml->member_class;
      if (cn->type==HTT_INTERNAL_TYPE && ml->count==0) {
	if (ml->format_data)
	  sprintf(buf,ml->format_str,*i_ptr,ml->format_data);
	else
	  sprintf(buf,ml->format_str,*i_ptr);
      } else {
	if (ml->format_data)
	  sprintf(buf,ml->format_str,i_ptr,ml->format_data);
	else
	  sprintf(buf,ml->format_str,i_ptr);
      }
      ll->data=new_string(buf);
      ll->flags=0;
      insque(ll,l->last);
    }
    ml=ml->next;
  }
  if (l->next!=l->last)  {
    l->cur_line=l->next;
    l->cur_top_line=l->next;
    edit_delete_line(l);  //Remove blank
    l->cur_data_col=strlen(l->cur_line->data);
    l->cur_col=l->calc_cur_col_cb(l,fs);  
    if (do_ltf(l)) {
      ml=cl->member_list;
      ll=l->next;
      while (ml) {
	if (ml->format_str) {
	  i_ptr=d+ml->offset;
	  cn=ml->member_class;
	  if (cn->type==HTT_INTERNAL_TYPE && ml->count==0) {
	    if (ml->format_data)
	      getf(ll->data,ml->format_str,i_ptr,ml->format_data);
	    else
	      getf(ll->data,ml->format_str,i_ptr);
	  } else {
	    if (ml->format_data)
	      getf(ll->data,ml->format_str,i_ptr,ml->format_data);
	    else
	      getf(ll->data,ml->format_str,i_ptr);
	  }
	}
	ml=ml->next;
	ll=ll->next;
      }
      result=true;
    }
  }
  delete_ltf(l);
  return result;
}

int fetch_menu_selection(linked_text_file *m)
{
  if (m) {
    if (m->cur_line!=m) {
      return atoi(m->cur_line->data);
    }
  }
  return 0;
}

DWORD do_menu(linked_text_file *m)
{
  DWORD result=0;
  if (m) {
    if (do_ltf(m))
      result=fetch_menu_selection(m);
  }
  return result;
}

void edit_find_next(linked_text_file *l)
{
  ltf_line *ll,*cl=l->cur_line;
  ll=cl->next;
  while (ll!=cl) {
    if (ll!=l) {
      if (stristr(ll->data,l->find_replace->find_text)) {
	l->cur_line=ll;
	l->cur_top_line=ll;
	return;
      }
    }
    ll=ll->next;
  }
}

void edit_find_last(linked_text_file *l)
{
  ltf_line *ll,*cl=l->cur_line;
  ll=cl->last;
  while (ll!=cl) {
    if (ll!=l) {
      if (stristr(ll->data,l->find_replace->find_text)) {
	l->cur_line=ll;
	l->cur_top_line=ll;
	return;
      }
    }
    ll=ll->last;
  }
}

void edit_find(linked_text_file *l)
{
  if (do_form(l->find_replace,"edit_find_text_record"))
    edit_find_next(l);
}

class edit_go_to_line_record
{
  int line format_str "Go to Line (hex):0x%04X";
};

void edit_go_to_line(linked_text_file *l)
{
  edit_go_to_line_record gtl;
  int i;
  gtl.line=1;
  if (do_form(&gtl,"edit_go_to_line_record")) {
    while (l->cur_line->last != l)
      edit_line_up(l,0);
    for (i=1;(i<gtl.line) && (l->cur_line->next != l);i++)
      edit_line_down(l,0);
  }
}

void edit_save_ltf(linked_text_file *l,bool prompt)
{
  char *buf=edit_save(l);
  if (prompt) {
    if (do_form(&l->filename,"edit_filename_record"))
      save_text_file(l->filename.name,buf);
  } else
    save_text_file(l->filename.name,buf);
  free(buf);
}

DWORD ltf_putchar(linked_text_file *l,char ch,DWORD sc)
{
  int sc2,sc3,i,ms=0;
  bool old_preempt=preempt(off);
  if (ch!=27 && ch!=0x11) {
    l->cur_data_col=l->calc_cur_data_col_cb(l,fs);
    l->cur_col=l->calc_cur_col_cb(l,fs);  

    sc2=sc & 0x17F;
    sc3=sc & 0x7F;  //Useful for doubled-up keys
    if (sc2==SC_R_GUI || sc2==SC_L_GUI) {
      if (l->menu_ltf) 
	ms=do_menu(l->menu_ltf);
    }
    if (sc3==SC_CURSOR_DOWN) {
      if (bt(&sc,SCF_CTRL)) {
	while (l->cur_line->next != l)
	  edit_line_down(l,sc);
      } else
	edit_line_down(l,sc);
    } else if (sc3==SC_CURSOR_UP) {
      if (bt(&sc,SCF_CTRL)) {
	while (l->cur_line->last != l)
	  edit_line_up(l,sc);
      } else
	edit_line_up(l,sc);
    } else if (sc3==SC_PAGE_DOWN) {
      i=fs->window_bottom-fs->window_top;
      while (i-- >0)
	edit_line_down(l,sc);
    } else if (sc3==SC_PAGE_UP) {
      i=fs->window_bottom-fs->window_top;
      while (i-- >0)
	edit_line_up(l,sc);
    } else if (sc3==SC_CURSOR_LEFT) {
      if (bt(&sc,SCF_CTRL))
	l->cur_data_col=0;
      else {
	if (l->cur_data_col>0)
	  l->cur_data_col--;
	else {
	  edit_line_up(l,sc);
	  l->cur_data_col=strlen(l->cur_line->data);
	}
      }
    } else if (sc3==SC_CURSOR_RIGHT) {
      if (bt(&sc,SCF_CTRL))
	l->cur_data_col=strlen(l->cur_line->data);
      else {
	l->cur_data_col++;
	if (l->cur_data_col>strlen(l->cur_line->data)) {
	  l->cur_data_col=0;
	  edit_line_down(l,sc);
	}
      }
    } else if (sc3==SC_DELETE) {
      if (bt(&sc,SCF_SHIFT))
	edit_cut_to_clipboard(l);
      else
	edit_delete_char(l);
    } else if (ch==CH_BACKSPACE) {
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
    } else if (ch==25 || ch==12) //ctrl Y ctrl L
      edit_delete_line(l);
    else if (ch==14 || ms==MU_TOGGLE_NUMS) {   //ctrl N
      if (l->flags & ltff_line_nums) {
	if (l->flags & ltff_line_nums_dec)
	  l->flags=l->flags & ~(ltff_line_nums_dec|ltff_line_nums);
	else
	  l->flags=l->flags | ltff_line_nums_dec;
      } else
	  l->flags=l->flags | ltff_line_nums;
    } else if (sc3==SC_INSERT) {
      if (bt(&sc,SCF_SHIFT))
	edit_paste_clipboard(l);
      else if (bt(&sc,SCF_CTRL))
	edit_copy_to_clipboard(l);
      else
	l->flags=l->flags ^ ltff_overstrike;
    } else if (ch==3 || ms==MU_COPY) //CTRL C
      edit_copy_to_clipboard(l);
    else if (ch==24 || ms==MU_CUT) //CTRL X
      edit_cut_to_clipboard(l);
    else if (ch==22 || ms==MU_PASTE) //CTRL V
      edit_paste_clipboard(l);
    else if (ch==6 || ms==MU_FIND) //CTRL F
      edit_find(l);
    else if (sc2==SC_F3 || ms==MU_FIND_NEXT) {
      if (bt(&sc,SCF_SHIFT))
	edit_find_last(l);
      else
	edit_find_next(l);
    } else if (ch==7 || ms==MU_GO_TO_LINE) //CTRL G
      edit_go_to_line(l);
    else if (ch==19 || ms==MU_SAVE) { // CTRL S
      edit_save_ltf(l,false);
    } else if (ch==1 || ms==MU_SAVE_AS) {//CTRL A
      edit_save_ltf(l,true);
    } else if (ms==0 && ch!=0x11)
      edit_insert_char(ch,l);

    l->cur_col=l->calc_cur_col_cb(l,fs);  
  }
  preempt(old_preempt);
  return ms;
}
defext(ext_ltf_putchar,&ltf_putchar);

char *edit_string_file(char **filename,char *st, DWORD *save_flag)
{
  char *result,*temp;
  linked_text_file *l=new_linked_text_file,*m=new_linked_text_file;
  strcpy(l->filename.name,*filename);
  if (!*st)
    to_linked_text_file("\r\n",l);
  else
    to_linked_text_file(st,l);

  temp=load_text_file(EDIT_MENU_NAME);
  strcpy(m->filename.name,EDIT_MENU_NAME);
  to_linked_text_file(temp,m);
  m->line_start_col=5;
  m->cur_data_col=5;
  m->text_attribute=(WHITE<<4)+GREEN;
  free(temp);

  l->menu_ltf=m;
  if (!do_ltf(l)) {
    *save_flag=0;
    delete_ltf(l);
    return 0;
  } else {
    *save_flag=1;
    free(*filename);
    *filename=new_string(l->filename.name);
    result=edit_save(l);
    delete_ltf(l);
    return result;
  }
}

void edit(char *filename)
{
  char *st_in,*st_out;
  DWORD save_flag;
  char *temp_filename=new_string(filename);

  st_in=load_text_file(filename);
  st_out=edit_string_file(&temp_filename,st_in,&save_flag);
  clear_crt;
  cursor_home;
  if (save_flag)
    save_text_file(temp_filename,st_out);
  free(st_in);
  free(st_out);
  free(temp_filename);
}


void use_console_ltf(char *menu_file)
{
  bool old_preempt;
  char *st;
  linked_text_file *l=new_linked_text_file,
		   *m=new_linked_text_file;
  to_linked_text_file("\r\n",l);
  l->max_lines=1000;

  if (menu_file) {
    l->menu_ltf=m;
    bts(&fs->crt_flags,CRTF_HAS_MENU);
    strcpy(m->filename.name,menu_file);
    m->line_start_col=5;
    m->cur_data_col=5;
    st=load_text_file(menu_file);
    to_linked_text_file(st,m);
    free(st);
  }
  old_preempt=preempt(off);
  fs->update_window=&ltf_update_current_ltf_window;
  fs->current_ltf=l;
  preempt(old_preempt);
}

