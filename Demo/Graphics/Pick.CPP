
/* <1> <1> (image) */

#define NUM_APPLES 128

I64	x[NUM_APPLES],y[NUM_APPLES],
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
    Sprite3(dc,x[i],y[i],0,__BIN_1);
    if (dc->nearest_dist<=best_dist) {
	//Distance to nearest pix, squared
	//[C:/Kernel/KernelB.HPP.Z,82] Sqrt() is expensive so we work with distance squared.
      best_dist=dc->nearest_dist;
      best_apple=i;

      //Elements are the individual lines or whatever of the CSprite.u.
      best_sprite_elem_num=dc->nearest_sprite_elem_num;
    }
  }
  cur_apple=best_apple;
  cur_dist=best_dist;
  cur_sprite_elem_num=best_sprite_elem_num;
  dc->color=WHITE;
  GrRect(dc,5,5,StrLen(buf)*FONT_WIDTH,FONT_HEIGHT);
  dc->color=BLACK;
  GrPrint(dc,5,5,buf);
}

U0 Pick()
{
  I64 i,msg_code,a1,a2,best_i=0,old_x,old_y;
  for (i=0;i<NUM_APPLES;i++) {
    x[i]=RandU16%GR_WIDTH;
    y[i]=RandU16%GR_HEIGHT;
  }
  *buf=0;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Preempt;
  AutoComplete;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER;

  try {
    while (TRUE) {
      if (GetMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN)==MSG_KEY_DOWN)
	goto pi_done;

      //Use screen coordinates, not window (a1,a2)
      //[C:/Kernel/KGlbls.CPP.Z,18] ip.pos.x and [C:/Kernel/KGlbls.CPP.Z,18] ip.pos.y are global vars updated
      //updated by the window mgr to hold the
      //screen coordinates of the input ptr (mouse).
      cur_x=ip.pos.x; cur_y=ip.pos.y;

      //Wait for window mgr to call DrawIt()
      WinMgrSync;
      best_i=cur_apple;
      old_x=x[best_i];
      old_y=y[best_i];
      StrPrint(buf,"Elem #%d Dist Squared:%d",cur_sprite_elem_num,cur_dist);
      do {
	msg_code=GetMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_IP_MOVE|1<<MSG_IP_L_UP);
	if (msg_code==MSG_KEY_DOWN)
	  goto pi_done;
	x[best_i]=ip.pos.x-cur_x+old_x;
	y[best_i]=ip.pos.y-cur_y+old_y;
      } while (msg_code!=MSG_IP_L_UP);
    }
pi_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
}

Pick;
