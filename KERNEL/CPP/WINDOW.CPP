#define white_xor 0xFF0F

void blank_crt_text()
{
  memset(*SYS_TEXT_BASE,0,(screen_height/font_height)*(screen_width/font_width)*2);
}


int old_ipx=-1000,old_ipy=-1000;

void win_point_and_sleep()
{
  DWORD win_jiffies;
  bool old_preempt=preempt(off);
  ip_lb_last=ip_lb;
  ip_rb_last=ip_rb;
  old_ipx=ipx;
  old_ipy=ipy;
  if (input_pointer==ip_mouse) {
    ipx=mxx; 
    ipy=myy;
    ip_lb=mouse_buttons[0];
    ip_rb=mouse_buttons[1];
  } else if (input_pointer==ip_joystick0) {
    poll_joystick;
    ipx=joycx; 
    ipy=joycy;
    ip_lb=joyb[0];
    ip_rb=joyb[1];
  }
  iptx=2*(ipx/font_width);
  ipty=ipy/font_height;
  preempt(old_preempt);

  if (ip_on) {
    if (ipx!=old_ipx || ipy!=old_ipy) {
      if (!gr_just_cleared) {
        gr_line(old_ipx,old_ipy,old_ipx+10,old_ipy+10,white_xor);
        gr_line(old_ipx,old_ipy,old_ipx+5,old_ipy,white_xor);
        gr_line(old_ipx,old_ipy,old_ipx,old_ipy+5,white_xor);
      }
      gr_just_cleared=false;
      gr_line(ipx,ipy,ipx+10,ipy+10,white_xor);
      gr_line(ipx,ipy,ipx+5,ipy,white_xor);
      gr_line(ipx,ipy,ipx,ipy+5,white_xor);
    }
  }
  gr_update_screen;
  win_jiffies=*JIFFIES;
  while (*JIFFIES==win_jiffies)
    swap_in_next;
}


void window_to_top(tss_struct *tss)
{
  bool old_preempt=preempt(off);
  tss_struct *next,*last;
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
    deactivate_all;
    bts(&tss->task_flags,TSSF_FOCUS);
  }
  preempt(old_preempt);
}
defext(ext_window_to_top,&window_to_top);

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

bool set_cursor_position(tss_struct *tss)
{
  DWORD i,y;
  linked_text_file *l;
  if (tss->window_top<=ipty && tss->window_bottom>=ipty &&
    tss->window_left<=iptx && tss->window_right>=iptx) {
    window_to_top(tss);
    if (tss->current_ltf) {
      l=tss->current_ltf;
      l->cur_line=l->cur_top_line;
      y=ipty-tss->window_top;
      for (i=0;i<y &&l->cur_line->next!=l;i++)
	l->cur_line=l->cur_line->next;
      l->cur_col=(iptx-tss->window_left)/2;
      //Note: due to hidden chars (tab) cur_col!=cur_data_col
      l->cur_data_col=l->calc_cur_data_col_cb(l,tss);  //See edit.cpp
      l->cur_col=l->calc_cur_col_cb(l,tss);  //See edit.cpp
      return true;
    }
  }
  return false;
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
  int x,y,i;
  linked_text_file *l;
  fs->window_top=0;
  fs->window_bottom=screen_height/font_height-1;
  fs->window_left=0;
  fs->window_right=2*screen_width/font_width-1;
  update_derived_tss_values(fs);

  blank_crt_text;
  preempt(off);
  while (true) {
    tss=fs->last_tss;
    while (tss!=fs) {
      update_derived_tss_values(tss);
      if (ip_lb) {
	if (tss->window_top==ipty+1 &&
	    (iptx>=tss->window_left-2 && iptx<=tss->window_right+2)) {
	  x=iptx-tss->window_left;
	  while (ip_lb && !bt(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED)) {
	    set_window_hor(tss,iptx-x,tss->window_right+(iptx-x)-tss->window_left);
	    set_window_vert(tss,ipty+1,tss->window_bottom+ipty+1-tss->window_top);
	    blank_crt_text;
	    window_to_top(tss);
	    update_derived_tss_values(tss);
	    win_point_and_sleep;
	  }
          ip_lb_last=false; 
	}
	if (tss->window_left==iptx+2 &&
	    (ipty>=tss->window_top-1 && ipty<=tss->window_bottom+1)) {
	  y=ipty-tss->window_top;
	  while (ip_lb && !bt(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED)) {
	    set_window_hor(tss,iptx+2,tss->window_right+iptx+2-tss->window_left);
	    set_window_vert(tss,ipty-y,tss->window_bottom+(ipty-y)-tss->window_top);
	    blank_crt_text;
	    window_to_top(tss);
	    update_derived_tss_values(tss);
	    win_point_and_sleep;
	  }
          ip_lb_last=false; 
	}
	if (tss->window_right+2==iptx && tss->window_bottom+1==ipty) {
	  while (ip_lb && !bt(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED)) {
	    set_window_hor(tss,tss->window_left,iptx-2);
	    set_window_vert(tss,tss->window_top,ipty-1);
	    blank_crt_text;
	    window_to_top(tss);
	    update_derived_tss_values(tss);
	    win_point_and_sleep;
	  }
          ip_lb_last=false; 
	}
	if (tss->window_bottom==ipty-1 &&
	    (iptx>=tss->window_left && iptx<=tss->window_right)) {
	  while (ip_lb && !bt(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED)) {
	    set_window_vert(tss,tss->window_top,ipty-1);
	    blank_crt_text;
	    window_to_top(tss);
	    update_derived_tss_values(tss);
	    win_point_and_sleep;
	  }
          ip_lb_last=false; 
	}
	if (tss->window_right==iptx-2 &&
	    (ipty>=tss->window_top && ipty<=tss->window_bottom)) {
	  while (ip_lb && !bt(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED)) {
	    set_window_hor(tss,tss->window_left,iptx-2);
	    blank_crt_text;
	    window_to_top(tss);
	    update_derived_tss_values(tss);
	    win_point_and_sleep;
	  }
          ip_lb_last=false; 
	}
      }
      if (btr(SYS_FLAGS,SYSF_TASK_LINKS_ALTERED))
        tss=fs->last_tss;
      else {
        if (ip_lb_last && !ip_lb) {
  	  if (set_cursor_position(tss))
	    ip_lb_last=false;
	}

        if (ip_rb_last && !ip_rb) {
	  if ((iptx>=tss->window_left) && (iptx<tss->window_left+8) &&
	    (ipty==tss->window_top-1)) {
            if (bt(&tss->crt_flags,CRTF_HAS_MENU)) {
	      *SYS_CURRENT_SCAN_CODE=(*SYS_CURRENT_SCAN_CODE & 0xFFFFFF00) | 
	        (SC_L_GUI+(1<<SCF_NEW_KEY)+(1<<SCF_KEY_UP));
	      ip_rb_last=false;
	    }
	  }
	  if (ip_rb_last && set_cursor_position(tss)) {
            *SYS_CURRENT_SCAN_CODE=(*SYS_CURRENT_SCAN_CODE & 0xFFFFFF00) | 
              (SC_ESC+(1<<SCF_NEW_KEY)+(1<<SCF_KEY_UP));
	      ip_rb_last=false;
	  }
	}
        update_derived_tss_values(tss);
        tss=tss->last_tss;
      }
    }
    win_point_and_sleep;
  }
}

defext(ext_blank_crt_text,&blank_crt_text);








