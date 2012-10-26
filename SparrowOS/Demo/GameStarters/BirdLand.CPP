AcctRegSetDftEntry("SparrowOS/BirdLand","F64 best_score=9999;\n");
AcctRegExecuteBranch("SparrowOS/BirdLand");


  /* <1> <1> (image) */





  /* <2> <2> (image) */




  /* <3> <3> (image) */





  /* <4> <4> (image) */


#define EAT_TIME	0.5

BoolI8 flap_down,flap_up,rst;
F64 t_flap,yy,xx,eat_timeout;
F64 frame_x,percent_completed,t0,tf;

#define MAX_BUGS	32
I32 bugs_x[MAX_BUGS],bugs_y[MAX_BUGS];
BoolI8 bugs_dead[MAX_BUGS];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,y,x,
    h=task->win_pixel_width-10,
    v=task->win_pixel_height-10;
  U8 *inter,*inter2,*inter3;
  F64 tt;
  if (h<1) h=1;
  if (v<1) v=1;
  tt=t_flap*t_flap*t_flap;
  inter=CSpriteInterpolate(__BIN_1,__BIN_2,tt);
  if (eat_timeout && tP(task)<eat_timeout) {
    inter2=CSpriteInterpolate(__BIN_3,__BIN_4,tt);
    inter3=CSpriteInterpolate(inter2,inter,1.0-(eat_timeout-tP(task))/EAT_TIME);
    Free(inter);
    Free(inter2);
    inter=inter3;
  }
  CSpritePlot3(dc,xx,yy,0,inter);
  Free(inter);
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+5;
      y=bugs_y[i]%v+5;
      dc->color=BLACK;
      GrPlot(dc,x,y);
      GrPlot(dc,x+1,y);
      dc->color=RED;
      GrPlot(dc,x,y-1);
    }
  if (tf) {
    dc->color=RED;
    GrPutS(dc,(task->win_pixel_width-FONT_WIDTH*14)/2,
		(task->win_pixel_height-FONT_HEIGHT)/2,"Game Completed");
    tt=tf;
  } else {
    dc->color=GREEN;
    tt=tP(task);
  }
  GrPrintF(dc,0,0,"Bugs:%3.1f%% Time:%3.2f Best:%3.2f",
    percent_completed,tt-t0,best_score);
}

U0 CheckBugs(CTask *task,CTask *song_task)
{
  I64 i,j=0,x,y,
    h=task->win_pixel_width-10,
    v=task->win_pixel_height-10;
  if (h<1) h=1;
  if (v<1) v=1;

  if (eat_timeout && eat_timeout-tP(task)<0.75*EAT_TIME) {
    LBtr(&song_task->task_flags,TASKf_SUSPENDED);
    if (tP(task)>=eat_timeout)
      eat_timeout=0;
  }
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+5;
      y=bugs_y[i]%v+5;
      if (Abs(x-xx)<10 && Abs(y-yy)<10) {
	bugs_dead[i]=TRUE;
	eat_timeout=tP(task)+EAT_TIME;
	Snd(1000);
	LBts(&song_task->task_flags,TASKf_SUSPENDED);
	j++;
      }
    } else
      j++;
  if (!tf && j==MAX_BUGS) {
    tf=tP(task);
    LBts(&song_task->task_flags,TASKf_SUSPENDED);
    Snd(0);
    if (tf-t0<best_score)
      best_score=tf-t0;
  }
  percent_completed=j*100.0/MAX_BUGS;
  frame_x-=0.1;
  if (frame_x<0)
    frame_x+=h;
}

U0 Init(CTask *song_task)
{
  I64 i;
  flap_down=FALSE;
  flap_up=TRUE;
  t_flap=0;
  yy=0;
  xx=100;
  frame_x=0;
  for (i=0;i<MAX_BUGS;i++) {
    bugs_dead[i]=FALSE;
    bugs_x[i]=RandU16;
    bugs_y[i]=RandU16;
  }
  LBtr(&song_task->task_flags,TASKf_SUSPENDED);
  eat_timeout=0;
  tf=0;
  t0=tP;
}


U0 SongTask(U64)
{ //Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("W13eBEBCBEsEFqEeBEBCBEsEF");
    Play("qEeADABADsDCqDeBEBCB");
    Play("EsEDqE");
  }
}


U0 AnimateTask(U64)
{
  F64 tt=0;
  while (TRUE) {
    if (rst) {
      Init(Fs->parent_task->song_task);
      rst=FALSE;
    }
    if (flap_down) {
      flap_down=FALSE;
      tt=-0.005;
    } else if (flap_up) {
      flap_up=FALSE;
      tt= 0.005;
    }
    if (tt<0) {
      yy+=75*tt;
      tt+=0.000015;
    } else
      yy+=0.15;
    yy=Limit(yy,10,Fs->parent_task->win_pixel_height-10);
    t_flap=Limit(t_flap+tt,0.0,1.0);
    CheckBugs(Fs->parent_task,Fs->parent_task->song_task);
    Sleep(1);
  }
}

U0 BirdLand()
{
  BoolI8 rst_space=TRUE;
  I64 p1,p2;

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,CH_NEW_LINE);"
  "  Flap(,CH_SPACE);"
      "}"
      );

  PopUpOk("Use $FG,GREEN$<SPACE BAR>$FG$ to flap.\nHold down to glide.");

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush

  WinBorder(ON);
  Fs->win_left=1;
  Fs->win_right=TEXT_COLS/2;
  Fs->win_top=2;
  Fs->win_bottom=TEXT_ROWS/2;
  TaskDerivedValsUpdate;

  Fs->song_task=Spawn(&SongTask,NULL,"Song",Fs);

  rst=TRUE;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);
  while (rst)
    Yield;

  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      switch (GetMsg(&p1,&p2,1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case CH_SPACE:
	      if (rst_space) {
		flap_down=TRUE;
		rst_space=FALSE;
	      }
	      break;
	    case CH_NEW_LINE:
	      rst=TRUE;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto bl_done;
	  }
	  break;
	case MSG_KEY_UP:
	  switch (p1) {
	    case CH_SPACE:
	      flap_up=TRUE;
	      rst_space=TRUE;
	      break;
	  }
	  break;
      }
    }
bl_done:
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
  AcctRegWriteBranch("SparrowOS/BirdLand","F64 best_score=%5.4f;\n",best_score);
}

BirdLand;
