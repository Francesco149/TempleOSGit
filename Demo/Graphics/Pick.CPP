
/* <1> <1> (image) */

#define NUM_APPLES 128

I64	x[NUM_APPLES],y[NUM_APPLES],
	cur_apple,cur_sprite_elem_num,cur_dist;
Bool	trigger;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,best_dist=MAX_I64,best_apple=0,best_sprite_elem_num=0;

  dc->flags|=DCF_LOCATE_NEAREST;
  dc->cur_x=ip.pos.x;
  dc->cur_y=ip.pos.y;
  dc->cur_z=0;
  for (i=0;i<NUM_APPLES;i++) {
    Sprite3(dc,x[i],y[i],0,__BIN_1);
    if (dc->nearest_dist<=best_dist) {
//Distance to nearest pix, squared
      //[C:/Kernel/KernelB.HPP.Z,108] Sqrt() is expensive so we work with distance squared.
      best_dist=dc->nearest_dist;
      best_apple=i;
//Elements are the individual lines or whatever of the CSprite.u.
      best_sprite_elem_num=dc->nearest_sprite_elem_num;
    }
  }
  if (trigger) {
    cur_apple=best_apple;
    cur_dist=best_dist;
    cur_sprite_elem_num=best_sprite_elem_num;
    trigger=FALSE;
  }
  GrPrint(dc,FONT_WIDTH,FONT_HEIGHT,
	"Elem #%d Dist Squared:%d",cur_sprite_elem_num,cur_dist);
}

U0 Pick()
{
  I64 i,msg_code,a1,a2,delta_x,delta_y;
  for (i=0;i<NUM_APPLES;i++) {
    x[i]=RandU16%GR_WIDTH;
    y[i]=RandU16%(GR_HEIGHT-FONT_HEIGHT*3)+FONT_HEIGHT*3;
  }
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Preempt;
  AutoComplete;
  DocCursor;
  DocClear;

  trigger=FALSE;
  cur_apple=cur_sprite_elem_num=cur_dist=0;

  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER;

  try {
    while (TRUE) {
      if (GetMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN)==MSG_KEY_DOWN)
	goto pi_done;

	//Wait for window mgr to call DrawIt()
      trigger=TRUE;
      do WinMgrSync;
      while (trigger);

      //Use screen coordinates, not window (a1,a2)
      //[C:/Kernel/KGlbls.CPP.Z,18] ip.pos.x and [C:/Kernel/KGlbls.CPP.Z,18] ip.pos.y are global vars updated
      //updated by the window mgr to hold the
      //screen coordinates of the input ptr (mouse).
      delta_x=x[cur_apple]-ip.pos.x;
      delta_y=y[cur_apple]-ip.pos.y;

      do {
	msg_code=GetMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_IP_MOVE|1<<MSG_IP_L_UP);
	if (msg_code==MSG_KEY_DOWN)
	  goto pi_done;
	x[cur_apple]=ip.pos.x+delta_x;
	y[cur_apple]=ip.pos.y+delta_y;
      } while (msg_code!=MSG_IP_L_UP);
    }
pi_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
}

Pick;
