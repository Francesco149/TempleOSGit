





		/*  <1> (image) */








		/*  <2> (image) */







		/* <3> <3> (image) */




F64 start_time=0,
    enemy_destroyed_time=0,
    player_destroyed_time=0,
    phasor_time=0;
I64 x=100,y=100;

#define NUM_STARS	100
I64 stars_x[NUM_STARS],stars_y[NUM_STARS];

U0 PhasorTask(I64)
{
  I64 i;
  for (i=2000;i>100;i-=6) {
    Snd(i);
    Sleep(2);
  }
  Snd(0);
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i;
  I64 xx=ipx-task->win_pixel_left-task->win_scroll_x,
     yy=ipy-task->win_pixel_top-task->win_scroll_y;
 
  task->text_attr=BLACK+BLACK<<4;
  dc->color=WHITE;
  for (i=0;i<NUM_STARS;i++)
    GrPlot(dc,stars_x[i],stars_y[i]);

  dc->color=ROP_EQU;
  if (enemy_destroyed_time) {
    SpritePlot3(dc,x,y,0,__BIN_3);
    if (tT>=enemy_destroyed_time)
      enemy_destroyed_time=0;
  } else
    SpritePlot3(dc,x,y,0,__BIN_1);

  if (tT>=start_time+0.2 && !player_destroyed_time) {
    dc->collision_cnt=0;
    dc->color=ROP_COLLISION;
    dc->bkcolor=BLACK;
    SpritePlot3(dc,xx,yy,0,__BIN_2);
    if (dc->collision_cnt>0x20) {
      player_destroyed_time=tT+1.0;  //one second
      Noise(700,150,200);
    }
  }

  dc->color=ROP_EQU;
  if (player_destroyed_time) {
    SpritePlot3(dc,xx,yy,0,__BIN_3);
    if (tT>=player_destroyed_time)
      player_destroyed_time=0;
  } else
    SpritePlot3(dc,xx,yy,0,__BIN_2);

  if (phasor_time) {
    dc->collision_cnt=0;
    dc->color=ROP_COLLISION;
    dc->bkcolor=BLACK;
    GrLine(dc,xx+45,yy,xx+300,yy);
    if (!enemy_destroyed_time && dc->collision_cnt>8){
      enemy_destroyed_time=tT+1.0;	//one second
      Noise(700,150,200);
    }

    dc->color=BLUE;
    GrLine(dc,xx+45,yy,xx+300,yy);
    if (tT>=phasor_time)
      phasor_time=0;
  }

}


U0 SpaceShip()
{
  I64 i;
  start_time=tT;
  for (i=0;i<NUM_STARS;i++) {
    stars_x[i]=RandU16%GR_WIDTH;
    stars_y[i]=RandU16%GR_HEIGHT;
  }
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Preempt(OFF);
  Fs->win_inhibit=WIF_ALL-WIF_BORDER;
  WinBorder;
  WordStat;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try {
    while (!ScanChar) {
      x-=3; y-=2;
      if (x<0) x=GR_WIDTH;
      if (y<0) y=GR_HEIGHT;
      if (ip_lb) {  //left button?
	phasor_time=tT+0.5;  //half second
	Spawn(&PhasorTask);
      }
      Sleep(50);
    }
  } catch
    CatchAll;
  SettingsPop;
}

SpaceShip;
