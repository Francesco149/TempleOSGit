


/* <1> <1> (image) */





#define NUM_APPLES 128

I64 x[NUM_APPLES],y[NUM_APPLES],
   cur_apple,cur_sprite_elem_num,cur_dist,
   cur_x=0,cur_y=0;

U8 buf[128];

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,best_dist=MAX_I64,best_apple=0,best_sprite_elem_num=0;
 
  dc->flags|=DCF_LOCATE_NEAREST;
  dc->cur_x=cur_x;
  dc->cur_y=cur_y;
  dc->cur_z=0;
  for (i=0;i<NUM_APPLES;i++) {
    dc->nearest_sprite_elem_num=0;
    dc->nearest_dist=MAX_I64;
    SpritePlot3(dc,x[i],y[i],0,__BIN_1);
    if (dc->nearest_dist<=best_dist) {
      //distance to nearest pixel, squared
      //[C:/TempleOS/Kernel/Adam1b.HPP.Z,230] Sqrt() is expensive so we work with distance squared.
      best_dist=dc->nearest_dist;
      best_apple=i;

      //Elements are the individual lines or whatever of the CSprite.
      best_sprite_elem_num=dc->nearest_sprite_elem_num;
    }
  }
//Note: window mgr does not get preempted and
//this code runs in the window manger task.

  cur_apple=best_apple;
  cur_dist=best_dist;
  cur_sprite_elem_num=best_sprite_elem_num;
  dc->color=WHITE;
  GrRect(dc,5,5,StrLen(buf)*FONT_WIDTH,FONT_HEIGHT);
  dc->color=BLACK;
  GrPrint(dc,5,5,buf);
}


U0 Main()
{
  I64 i,msg_code,p1,p2,best_i=0,old_x,old_y;
  for (i=0;i<NUM_APPLES;i++) {
    x[i]=RandU16%GR_WIDTH;
    y[i]=RandU16%GR_HEIGHT;
  }
  *buf=0;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WordStat;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_FOCUS-WIF_BORDER;

  do {
    msg_code=GetMsg(&p1,&p2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN);

    //Use screen coordinates, not window (p1,p2)
    //[C:/TempleOS/Kernel/KGlbls.CPP.Z,80] ipx and [C:/TempleOS/Kernel/KGlbls.CPP.Z,80] ipy are global vars updated
    //updated by the window mgr to hold the
    //screen coordinates of the input ptr (mouse).
    cur_x=ipx; cur_y=ipy;

    if (msg_code==MSG_IP_L_DOWN) {
      //Wait for window mgr to call UpdateWin
      WinMgrSync;
      best_i=cur_apple;
      old_x=x[best_i];
      old_y=y[best_i];
      SPrint(buf,"Elem #%d Dist Squared:%d",cur_sprite_elem_num,cur_dist);
      do {
	msg_code=GetMsg(&p1,&p2,1<<MSG_KEY_DOWN|1<<MSG_IP_MOVE|1<<MSG_IP_L_UP);
	if (msg_code!=MSG_KEY_DOWN) {
	  x[best_i]=ipx-cur_x+old_x;
	  y[best_i]=ipy-cur_y+old_y;
	}
      } while (msg_code!=MSG_KEY_DOWN && msg_code!=MSG_IP_L_UP);
    }
  } while (msg_code!=MSG_KEY_DOWN || !p1);
  GetMsg(,,1<<MSG_KEY_UP);
  SettingsPop;
}

Main;
