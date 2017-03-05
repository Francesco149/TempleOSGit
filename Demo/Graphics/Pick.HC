
/* <1> <1> (image) */

#define APPLES_NUM 128

I64	x[APPLES_NUM],y[APPLES_NUM],
	cur_apple,cur_dist;
Bool	trigger;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,best_dist=I64_MAX,best_apple=0;

  dc->flags|=DCF_LOCATE_NEAREST;
  dc->cur_x=ms.pos.x;
  dc->cur_y=ms.pos.y;
  dc->cur_z=0;
  for (i=0;i<APPLES_NUM;i++) {
    Sprite3(dc,x[i],y[i],0,__BIN_1);
    if (dc->nearest_dist<=best_dist) {
//Distance to nearest pix, squared
      //[C:/Kernel/KernelB.HH.Z,109] Sqrt() is expensive so we work with distance squared.
      best_dist=dc->nearest_dist;
      best_apple=i;
    }
  }
  if (trigger) {
    cur_apple=best_apple;
    cur_dist=best_dist;
    trigger=FALSE;
  }
  GrPrint(dc,FONT_WIDTH,FONT_HEIGHT,"Dist Squared:%d",cur_dist);
}

U0 Pick()
{
  I64 i,msg_code,arg1,arg2,delta_x,delta_y;
  for (i=0;i<APPLES_NUM;i++) {
    x[i]=RandU16%GR_WIDTH;
    y[i]=RandU16%(GR_HEIGHT-FONT_HEIGHT*3)+FONT_HEIGHT*3;
  }
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  DocCursor;
  DocClear;

  trigger=FALSE;
  cur_apple=cur_dist=0;

  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER;

  try {
    while (TRUE) {
      if (GetMsg(&arg1,&arg2,1<<MSG_KEY_DOWN|1<<MSG_MS_L_DOWN)==MSG_KEY_DOWN)
	goto pi_done;

	//Wait for window mgr to call DrawIt()
      trigger=TRUE;
      do Refresh;
      while (trigger);

      //Use scrn coordinates, not window (arg1,arg2)
      //[C:/Kernel/KGlbls.HC.Z,25] ms.pos.x and [C:/Kernel/KGlbls.HC.Z,25] ms.pos.y are global vars updated
      //updated by the window mgr to hold the
      //scrn coordinates of the mouse.
      delta_x=x[cur_apple]-ms.pos.x;
      delta_y=y[cur_apple]-ms.pos.y;

      do {
	msg_code=GetMsg(&arg1,&arg2,
	      1<<MSG_KEY_DOWN|1<<MSG_MS_MOVE|1<<MSG_MS_L_UP);
	if (msg_code==MSG_KEY_DOWN)
	  goto pi_done;
	x[cur_apple]=ms.pos.x+delta_x;
	y[cur_apple]=ms.pos.y+delta_y;
      } while (msg_code!=MSG_MS_L_UP);
    }
pi_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
}

Pick;
