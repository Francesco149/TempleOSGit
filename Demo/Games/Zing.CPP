I64 box_x_min,box_x_max,box_y_min,box_y_max;

class Arrow
{
  Arrow *next,*last;
  F64 x,y,dx,dy;
} root;

   /* <1> <1> (image) */

Bool bow_drawn;
F64 bow_x,bow_y,bow_theta;

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 theta,x,y,dx,dy,
	   str_w,str_h,draw_len;
  Arrow *tempa;
  CD3I32 ctrl[5];
  dc->color=RED;
  GrBorder(dc,box_x_min,box_y_min,box_x_max,box_y_max);

  x=ClampI64(ip.pos.x-task->pix_left-task->scroll_x,box_x_min,box_x_max);
  y=ClampI64(ip.pos.y-task->pix_top-task->scroll_y, box_y_min,box_y_max);
  dx=bow_x-x;
  dy=bow_y-y;

  if (bow_drawn && (dx|dy))
    bow_theta=Arg(dx,dy);
  else {
    bow_x=x;
    bow_y=y;
  }

  draw_len=Sqrt(dx*dx+dy*dy);
  str_w=draw_len/3;
  str_h=Sqrt(60*60-str_w*str_w);

  dc->color=BLACK;
  GrLine(dc,x-str_h/2*Cos(bow_theta+pi/2)+str_w*Cos(bow_theta),
	      y-str_h/2*Sin(bow_theta+pi/2)+str_w*Sin(bow_theta),
	      x,y);
  GrLine(dc,x+str_h/2*Cos(bow_theta+pi/2)+str_w*Cos(bow_theta),
	      y+str_h/2*Sin(bow_theta+pi/2)+str_w*Sin(bow_theta),
	      x,y);

  MemSet(ctrl,0,sizeof(ctrl));
  ctrl[0].x=x-str_h/2*Cos(bow_theta+pi/2)+str_w*Cos(bow_theta);
  ctrl[0].y=y-str_h/2*Sin(bow_theta+pi/2)+str_w*Sin(bow_theta);
  ctrl[1].x=x-0.75*str_h/2*Cos(bow_theta+pi/2)+draw_len/2*Cos(bow_theta)+str_w*Cos(bow_theta);
  ctrl[1].y=y-0.75*str_h/2*Sin(bow_theta+pi/2)+draw_len/2*Sin(bow_theta)+str_w*Sin(bow_theta);
  ctrl[2].x=x+draw_len/2*Cos(bow_theta)+str_w*Cos(bow_theta);
  ctrl[2].y=y+draw_len/2*Sin(bow_theta)+str_w*Sin(bow_theta);
  ctrl[3].x=x+0.75*str_h/2*Cos(bow_theta+pi/2)+draw_len/2*Cos(bow_theta)+str_w*Cos(bow_theta);
  ctrl[3].y=y+0.75*str_h/2*Sin(bow_theta+pi/2)+draw_len/2*Sin(bow_theta)+str_w*Sin(bow_theta);
  ctrl[4].x=x+str_h/2*Cos(bow_theta+pi/2)+str_w*Cos(bow_theta);
  ctrl[4].y=y+str_h/2*Sin(bow_theta+pi/2)+str_w*Sin(bow_theta);

  dc->color=BROWN;
  dc->pen_width=2;
  Gr2BSpline3(dc,ctrl,5);
  dc->pen_width=1;

  if (bow_drawn)
    SpriteZ3B(dc,x,y,0,__BIN_1,bow_theta);

  tempa=root.next;
  while (tempa!=&root) {
    theta=Arg(tempa->dx,tempa->dy);
    SpriteZ3B(dc,tempa->x,tempa->y,0,__BIN_1,theta);
    tempa=tempa->next;
  }
}

#define ANIMATE_SLEEP_MS	10

U0 AnimateTask(I64)
{
  I64 x,y;
  Arrow *tempa,*tempa1;
  F64 dt,t0=tS;
  while (TRUE) {
    dt=tS-t0;
    t0=tS;
 
    x=ClampI64(ip.pos.x-Fs->parent_task->pix_left-Fs->parent_task->scroll_x,
       box_x_min,box_x_max)+Fs->parent_task->pix_left+Fs->parent_task->scroll_x;
    y=ClampI64(ip.pos.y-Fs->parent_task->pix_top-Fs->parent_task->scroll_y,
       box_y_min,box_y_max)+Fs->parent_task->pix_top+Fs->parent_task->scroll_y;
    if (ip.pos.x!=x || ip.pos.y!=y)
      SetIP(x,y);

    tempa=root.next;
    while (tempa!=&root) {
      tempa1=tempa->next;
      tempa->x+=tempa->dx*dt;
      tempa->y+=tempa->dy*dt;
      if (!(-Fs->parent_task->scroll_x<=tempa->x<Fs->parent_task->pix_width-Fs->parent_task->scroll_x) ||
	  !(-Fs->parent_task->scroll_y<=tempa->y<Fs->parent_task->pix_height-Fs->parent_task->scroll_y)) {
	QueRem(tempa);
	Free(tempa);
      }
      tempa=tempa1;
    }
    WinMgrSync;
  }
}

U0 Init()
{
  I64 w=Fs->pix_width,
      h=Fs->pix_height;
  QueInit(&root);
  bow_drawn=FALSE;
  box_x_min=7*w/16;
  box_y_min=6*h/8;
  box_x_max=9*w/16;
  box_y_max=7*h/8;
  bow_theta=-pi/2;
  bow_x=(box_x_min+box_x_max)/2;
  bow_y=(box_y_min+box_y_max)/2;
  SetIP(bow_x+Fs->pix_left+Fs->scroll_x,
	bow_y+Fs->pix_top+Fs->scroll_y);
}

U0 CleanUp()
{
  QueDel(&root,TRUE);
}

U0 Zing()
{
  I64 a1,a2;
  Arrow *tempa;
  MenuPush(
    "File {"
    "  Abort(,CH_SHIFT_ESC);"
    "  Exit(,CH_ESC);"
    "}"
    "Play {"
    "  Restart(,'\n');"
    "}"
  );
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Preempt(OFF);
  WinBorder;
  WinMax;
  WordStat;

  DocClear;

  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_GRAB_SCROLL;
  try {
    while (TRUE)
      switch (GetMsg(&a1,&a2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN|1<<MSG_IP_L_UP)) {
	case MSG_KEY_DOWN:
	  switch (a1) {
	    case '\n':
	      CleanUp;
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto zi_done;
	  }
	  break;
	case MSG_IP_L_DOWN:
	  bow_x=a1;
	  bow_y=a2;
	  bow_drawn=TRUE;
	  break;
	case MSG_IP_L_UP:
	  if(a1-bow_x || a2-bow_y) {
	    tempa=MAlloc(sizeof(Arrow));
	    tempa->dx=10.0*(bow_x-a1);
	    tempa->dy=10.0*(bow_y-a2);
	    tempa->x=a1;
	    tempa->y=a2;
	    QueIns(tempa,root.last);
	    Noise(50,8000,10000);
	  }
	  bow_drawn=FALSE;
	  break;
      }
zi_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  CleanUp;
  MenuPop;
}

Zing;
