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
    x1=ipx;  y1=ipy;
    while (ip_lb) {
      if (bt(SYS_CURRENT_SCAN_CODE,16))
        return;
      x2=ipx; y2=ipy;
      gr_line(x1,y1,x2,y2,-1);
      sleep(60);
      gr_line(x1,y1,x2,y2,-1);
      sleep(60);
    }
    gr_line(x1,y1,x2,y2,-1);
  }
}
doodle;;

 