//This is a drawing program

void doodle()
{
  int x1,y1,x2,y2;
  gr_clear;
  btr(SYS_CURRENT_SCAN_CODE,16);
  while (!bt(SYS_CURRENT_SCAN_CODE,16)) {
    while (!ip_lb) {
      if (bt(SYS_CURRENT_SCAN_CODE,16))
	return;
      swap_in_next;
    }
    x1=ipx-(fs->window_left-1)*font_width;
    y1=ipy-fs->window_top*font_height;
    while (ip_lb) {
      if (bt(SYS_CURRENT_SCAN_CODE,16))
	return;
      x2=ipx-(fs->window_left-1)*font_width;
      y2=ipy-fs->window_top*font_height;
      gr_line(x1,y1,x2,y2,-1);
      sleep(60);
      gr_line(x1,y1,x2,y2,-1);
      sleep(60);
    }
    gr_line(x1,y1,x2,y2,-1);
  }
}
doodle;;


