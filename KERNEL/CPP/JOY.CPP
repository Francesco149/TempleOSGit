#define JPORT 0x201
#define CMAX  0x800


int joyv[4],joymax[4],joymin[4],joyrest[4];
bool joyb[4],joy_calibrated=false;
int joysat=100,joynx=0,joyny=0;  //Normalized
int joycx=screen_width/2,joycy=screen_height/2,joycvx=0,joycvy=0;
int joytx=0,joyty=0;  //Text row & col

#define white_xor 0xFF0F

void joystick_tsk()
{ 
  int i, j;
  int mask = 15;
  int scale2=3;
  init_crt;
  btr(&fs->crt_flags,CRTF_SHOW);
  fs->window_left=0;
  fs->window_right=(screen_width/font_width)*2;
  fs->window_top=0;
  fs->window_bottom=(screen_height/font_height);
  while(true) {
    cli;
    for (i=0;i<4;i++)
      joyv[i]=0;
    outp(JPORT, 0);
    for (i=1; mask && i < CMAX; i++) {
      j = inp(JPORT) ^ mask;
      if (j & 1) { joyv[0] = i; mask = mask ^ 1; }
      if (j & 2) { joyv[1] = i; mask = mask ^ 2; }
      if (j & 4) { joyv[2] = i; mask = mask ^ 4; }
      if (j & 8) { joyv[3] = i; mask = mask ^ 8; }
    }
    sti;
    j = inp(JPORT);
    joyb[0] = !(j & 0x10);
    joyb[1] = !(j & 0x20);
    joyb[2] = !(j & 0x40);
    joyb[3] = !(j & 0x80);

    if (joy_calibrated) {
      joynx=joysat*(joyv[0]-joyrest[0])/(joymax[0]-joymin[0]);
      if (abs(joynx)<15)
        joycvx=0;
      else
        joycvx=joycvx+joynx-sign(joynx)*15;
      joyny=joysat*(joyv[1]-joyrest[1])/(joymax[1]-joymin[1]);
      if (abs(joyny)<15)
        joycvy=0;
      else
        joycvy=joycvy+joyny-sign(joyny)*15;
      joycx=joycx+joycvx/scale2;
      joycvx=70*joycvx/100;
      joycy=joycy+joycvy/scale2;
      joycvy=70*joycvy/100;
      if (joycx < 0) joycx=0;
      if (joycx>=screen_width) joycx=screen_width-1;
      if (joycy < 0) joycy=0;
      if (joycy>=screen_height) joycy=screen_height-1;
      joytx=joycx/font_width;
      joyty=joycy/font_height;
      if (input_pointer==ip_joystick0) {
        ipx=joycx; ipy=joycy;
        iptx=ipx/font_width;
        ipty=ipy/font_height;
        ip_lb=joyb[0];
        ip_rb=joyb[1];
        if (ip_on) {
          gr_line(joycx,joycy,joycx+10,joycy+10,white_xor);
          gr_line(joycx,joycy,joycx+5,joycy,white_xor);
          gr_line(joycx,joycy,joycx,joycy+5,white_xor);
          sleep(100);
          gr_line(joycx,joycy,joycx+10,joycy+10,white_xor);
          gr_line(joycx,joycy,joycx+5,joycy,white_xor);
          gr_line(joycx,joycy,joycx,joycy+5,white_xor);
        }
      }
    }
    sleep(50);
  }
}

void cal_joy()
{
  int i;
  int buf[3*4];
  if (joy_calibrated) {
    joy_calibrated=false;
    sleep(500); //Wait for joystick_tsk
  }
  for (i=0;i<4;i++) 
    joyb[i]=0;
  ? "Leave joystick centered and press button.\r\n";
  while (!(joyb[0] || joyb[1]))
    swap_in_next;
  for (i=0;i<4;i++) {
    joyrest[i]=joyv[i];
    joymax[i]=joyv[i];
    joymin[i]=joyv[i];
  }
  while (joyb[0] || joyb[1])
    sleep(500);
  ? "Move all about and press button.\r\n";
  while (!(joyb[0] || joyb[1])) {
    gr_plot(joyv[0]/2,joyv[1]/2,white_xor);
    gr_plot(joyv[2]/2,joyv[3]/2,white_xor);
    for (i=0;i<4;i++) {
      if (joyv[i]>joymax[i]) joymax[i]=joyv[i];
      if (joyv[i]<joymin[i]) joymin[i]=joyv[i];
    }
    swap_in_next;
  }
  gr_clear;
  joy_calibrated=true;

  //Save
  for (i=0;i<4;i++) {
    buf[i*3+0]=joymin[i];
    buf[i*3+1]=joymax[i];
    buf[i*3+2]=joyrest[i];
  }
  save_file("/HOPPY/KERNEL/CPP/JOY.CAL",buf,sizeof(int)*3*4);
}

bool load_joystick_calibration()
{
  DWORD size,i;
  int *ptr,*buf=read_file("/HOPPY/KERNEL/CPP/JOY.CAL",&size);
  if (!size)
    cal_joy;
  else {
    ptr=buf;
    for (i=0;i<4;i++) {
      joymin[i]=ptr[0];
      joymax[i]=ptr[1];
      joyrest[i]=ptr[2];
      ptr=ptr+3*sizeof(int);
      joy_calibrated=true;
    }
    free(buf);
  }
}

 
 
 
 
 
 
 
 
