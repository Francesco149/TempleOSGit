void fill_menu_selection(tss_struct *tss)
{
  linked_text_file *l;
  ltf_line *cl;
  DWORD line_num;
  l=tss->menu_ltf;
  if (l && l->cur_line!=l) {
    cl=l->cur_line;
    tss->menu_selection=atoi(cl->data);
  } else
    tss->menu_selection=0;
}

void blank_crt_text()
{
  memset(*SYS_TEXT_BASE,0,(screen_height/font_height)*(screen_width/font_width)*2);
}

void window_to_top(tss_struct *tss)
{
  DWORD old_flags=get_flags;
  tss_struct *next,*last;
  cli;
  //REMOVE
  next=tss->next_tss;
  last=tss->last_tss;
  next->last_tss=last;
  last->next_tss=next;
  //INSQUE
  last=update_screen_tss->last_tss;
  next=update_screen_tss;
  last->next_tss=tss;
  next->last_tss=tss;
  tss->next_tss=next;
  tss->last_tss=last;
  if (bt(&tss->task_flags,TSSF_LOCAL_USER)) {
    deactivate_user;
    btr(&tss->task_flags,TSSF_SUSPENDED);
  }
  set_flags(old_flags);
}

void set_window_hor(tss_struct *tss,int i,int j)
{
  int d=j-i;
  if (i>=tss->crt_width) {
    i=tss->crt_width-2;
    j=i+d;
  }
  if (j<0) {
    j=0;
    i=j-d;
  }
  if (i>j) {
    if (i>0)
      j=i;
    else
      i=j;
  }
  tss->window_left=i;
  tss->window_right=j;
}

void set_window_vert(tss_struct *tss,int i,int j)
{
  int d=j-i;
  if (i>=tss->crt_height) {
    i=tss->crt_height-1;
    j=i+d;
  }
  if (j<0) {
    j=0;
    i=j-d;
  }
  if (i>j) {
    if (i>0)
      j=i;
    else
      i=j;
  }
  tss->window_top=i;
  tss->window_bottom=j;
}

void set_cursor_position(tss_struct *tss)
{
  DWORD i,y;
  linked_text_file *l;
  if (tss->window_top<=ipty && tss->window_bottom>=ipty &&
    tss->window_left<=iptx && tss->window_right>=iptx) {
    if (tss->current_ltf) {
      l=tss->current_ltf;
      l->cur_line=l->cur_top_line;
      y=ipty-tss->window_top;
      for (i=0;i<y &&l->cur_line->next!=l;i++) 
        l->cur_line=l->cur_line->next;
      l->cur_col=iptx-tss->window_left/2+l->line_start_col;
      if (l->flags & ltff_line_nums)
        l->cur_col=l->cur_col+5;
      //Note: due to hidden chars (tab) cur_col!=cur_data_col
      l->calc_cur_data_col_cb(l,tss);  //See edit.cpp
    }
  }
}

void update_derived_tss_values(tss_struct *tss)
{
  tss->window_pixel_left=font_width*tss->window_left/2;
  tss->window_pixel_right=font_width*tss->window_right/2;
  tss->window_pixel_top=font_height*tss->window_top;
  tss->window_pixel_bottom=font_height*tss->window_bottom;
}

void window_manager_tsk()
{
  tss_struct *tss;
  linked_text_file *l,*m;
  int x,y,i;
  char ch;
  bool ip_lb_last=false,ip_rb_last=false;
  DWORD sc,menu_attribute,text_attribute;
  blank_crt_text;
  cli;
  while (true) {
    tss=fs->next_tss;
    while (tss!=fs) {
      update_derived_tss_values(tss);
      if (ip_lb) {
        if (tss->window_top==ipty+1 &&
            (iptx*2>=tss->window_left-2 && iptx*2<=tss->window_right+2)) {
          x=iptx*2-tss->window_left;
          while (ip_lb) {
            set_window_hor(tss,2*iptx-x,tss->window_right+(2*iptx-x)-tss->window_left);
            set_window_vert(tss,ipty+1,tss->window_bottom+ipty+1-tss->window_top);
            blank_crt_text;
            window_to_top(tss);
            update_derived_tss_values(tss);
            gr_update_screen;
            swap_in_next;
          }
        }
        if (tss->window_left==2*iptx+2 && 
            (ipty>=tss->window_top-1 && ipty<=tss->window_bottom+1)) {
          y=ipty-tss->window_top;
          while (ip_lb) {
            set_window_hor(tss,2*iptx+2,tss->window_right+2*iptx+2-tss->window_left);
            set_window_vert(tss,ipty-y,tss->window_bottom+(ipty-y)-tss->window_top);
            blank_crt_text;
            window_to_top(tss);
            update_derived_tss_values(tss);
            gr_update_screen;
            swap_in_next;
          }
        }
        if (tss->window_right+2==2*iptx && tss->window_bottom+1==ipty) {
          while (ip_lb) {
            set_window_hor(tss,tss->window_left,2*iptx-2);
            set_window_vert(tss,tss->window_top,ipty-1);
            blank_crt_text;
            window_to_top(tss);
            update_derived_tss_values(tss);
            gr_update_screen;
            swap_in_next;
          }
        }
        if (tss->window_bottom==ipty-1 && 
            (iptx*2>=tss->window_left && iptx*2<=tss->window_right)) {
          while (ip_lb) {
            set_window_vert(tss,tss->window_top,ipty-1);
            blank_crt_text;
            update_derived_tss_values(tss);
            gr_update_screen;
            window_to_top(tss);
            swap_in_next;
          }
        }
        if (tss->window_right==2*iptx-2 && 
            (ipty>=tss->window_top && ipty<=tss->window_bottom)) {
          while (ip_lb) {
            set_window_hor(tss,tss->window_left,2*iptx-2);
            blank_crt_text;
            window_to_top(tss);
            update_derived_tss_values(tss);
            gr_update_screen;
            swap_in_next;
          }
        }
      }
      if (ip_rb || bt(SYS_CURRENT_SCAN_CODE,SCF_WIN)) {
        if (tss->window_top<=ipty && tss->window_bottom>=ipty &&
          tss->window_left<=iptx && tss->window_right>=iptx) {
          if (tss->menu_ltf) {
            l=tss->current_ltf;
            text_attribute=tss->text_attribute;
            m=tss->menu_ltf;
            menu_attribute=tss->menu_attribute;
            tss->current_ltf=m;
            tss->text_attribute=menu_attribute;
            while (ip_rb || bt(SYS_CURRENT_SCAN_CODE,SCF_WIN)) {
              window_to_top(tss);
              if (ip_lb_last && !ip_lb) 
                set_cursor_position(tss);
              else {
                if (btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY)) {
                  sc=*SYS_CURRENT_SCAN_CODE & 0xFF;
                  ch=translate_scan_code;
                  ltf_putchar(tss->current_ltf,ch,sc,0);
                }
              }
              ip_lb_last=ip_lb;
              ip_rb_last=ip_rb;
              update_derived_tss_values(tss);
              gr_update_screen;
              swap_in_next;
            }
            fill_menu_selection(tss);
            tss->text_attribute=text_attribute;
            tss->current_ltf=l;
          }
        }
      }
      if (ip_lb_last && !ip_lb) 
        set_cursor_position(tss);
      update_derived_tss_values(tss);
      tss=tss->next_tss;
    }
    ip_lb_last=ip_lb;
    ip_rb_last=ip_rb;
    gr_update_screen;
    swap_in_next;
  }
}

defext(ext_blank_crt_text,&blank_crt_text);

 
 
 
 
 
 
 