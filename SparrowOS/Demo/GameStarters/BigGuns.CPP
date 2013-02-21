//This uses [::/SparrowOS/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.

#define MAP_WIDTH	2048
#define MAP_HEIGHT	(GR_HEIGHT-3*FONT_HEIGHT)

I64 gun_x,gun_y,active_map=0,gun_recoil;
F64 gun_theta;
CDC *maps[2]={NULL,NULL};
I16 elevs[MAP_WIDTH];

F64 wind_x;
#define NUM_DUST	512
I32 dust_x[NUM_DUST],dust_y[NUM_DUST];

CMathODE *ode=NULL;




    /* <1> <1> (image) */




    /* <2> <2> (image) */






U0 DrawIt(CTask *task,CDC *dc)
{
  CDC *map=maps[active_map&1];
  CMass *tempm;
  F64 theta=gun_theta;
  I64 i,x,y,w,
      h=-task->horz_scroll.pos,
      v=-task->vert_scroll.pos;
  Bool old_suspend=Suspend(task);
  task->horz_scroll.min=0;
  task->horz_scroll.max=MAP_WIDTH-task->win_pixel_width;
  task->vert_scroll.min=0;
  task->vert_scroll.max=MAP_HEIGHT-task->win_pixel_height;
  map->flags|=DCF_NO_TRANSPARENTS;
  GrBlot(dc,h,v,map);

  SpritePlot3(dc,gun_x+h,gun_y+v,0,__BIN_2);

  if (theta<-pi/2) {
    dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
    GrSetSymmetry(dc,gun_x+h,0,gun_x+h,1);
    theta=-pi-theta;
  }
  SpritePlotRotZ3b(dc,
      gun_x+h-gun_recoil*Cos(theta),
      gun_y+v-gun_recoil*Sin(theta)-10,0,__BIN_1,theta);
  dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);

  tempm=ode->next_mass;
  dc->color=BLACK;
  map->color=ROP_COLLISION;
  map->bkcolor=LTCYAN;
  while (tempm!=&ode->next_mass) {
    map->collision_cnt=0;
    GrCircle(map,tempm->x,tempm->y,2);
    if (map->collision_cnt)
      tempm->user_data=1;

    GrCircle(dc,tempm->x+h,tempm->y+v,2);

    tempm=tempm->next;
  }

  dc->color=LTGRAY;
  w=tP(task)*wind_x;
  for (i=0;i<NUM_DUST;i++) {
    x=(dust_x[i]+w)%MAP_WIDTH;
    y=dust_y[i];
    if (y<elevs[x])
      GrPlot(dc,x+h,y+v);
  }
  Suspend(task,old_suspend);
}

U0 MyDerivative(CMathODE *ode,F64,COrder2D3 *,COrder2D3 *)
{
  CMass *tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    tempm->DstateDt->DyDt+=1000.0*tempm->mass;
    tempm->DstateDt->DxDt+=25.0*wind_x;
    tempm=tempm->next;
  }
}

U0 DrawMap()
{
  CDC *map=maps[(active_map+1)&1];
  I64 x;

  map->color=LTCYAN;
  GrRect(map,0,0,MAP_WIDTH,MAP_HEIGHT);

  map->color=BLACK;
  for (x=1;x<MAP_WIDTH;x++)
    GrLine(map,x-1,elevs[x-1],x,elevs[x]);

  map->color=BROWN;
  GrFloodFill(map,0,MAP_HEIGHT-1,FALSE);

  active_map++;
}

U0 FireTask(I64)
{ //by default preempt is off on new tasks
  CMass *tempm;
  I64 i;
  if (gun_recoil) return;

  tempm=CAlloc(sizeof(CMass),Fs->parent_task);
  tempm->mass=10.0;
  tempm->drag_profile_factor=0.1;
  tempm->x=gun_x+27*Cos(gun_theta);
  tempm->y=gun_y-15+27*Sin(gun_theta);
  tempm->DxDt=600.0*Cos(gun_theta);
  tempm->DyDt=600.0*Sin(gun_theta);
  tempm->user_data=0;
  while (sys_task_being_screen_updated==Fs->parent_task)
    Yield;
  QueIns(tempm,ode->last_mass);

  Fs->task_end_cb=&SndTaskEndCB;
  for (i=0;i<60;i++) {
    Snd(RandU16&255+15);
    Sleep(2);
    gun_recoil=i/12;
  }
  for (i=0;i<=60;i++) {
    Sleep(1);
    gun_recoil=5-i/12;
  }
}

U0 ManageShots()
{
  I64 i;
  CMass *tempm,*tempm1;
  Bool chged=FALSE;
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    tempm1=tempm->next;
    if (!(0<=tempm->x<MAP_WIDTH) ||
	 tempm->user_data) {
      QueRem(tempm);
      for (i=tempm->x-4;i<=tempm->x+4;i++)
	if (0<=i<MAP_WIDTH)
	  elevs[i]=LimitI64(elevs[i]+10-2*AbsI64(i-tempm->x),0,MAP_HEIGHT-2);
      Free(tempm);
      chged=TRUE;
    }
    tempm=tempm1;
  }
  if (chged)
    DrawMap;
}

U0 MoveTask(I64)
{
  static F64 quit_time=0;
  if (quit_time)
    quit_time=tNP+0.1;
  else {
    Snd(100);
    Fs->task_end_cb=&SndTaskEndCB;
    quit_time=tNP+0.1;
    while (quit_time>tNP)
      Yield;
    quit_time=0;
  }
}


U0 Init()
{
  CDC *map;
  I64 i,x,y,dy;
  Bool old_preempt;
  if (!maps[0])
    maps[0]=DCNew(DCT_COLOR8,MAP_WIDTH,MAP_HEIGHT);
  if (!maps[1])
    maps[1]=DCNew(DCT_COLOR8,MAP_WIDTH,MAP_HEIGHT);
  map=maps[active_map&1];
  Fs->horz_scroll.pos=0;
  Fs->vert_scroll.pos=0;

  y=ToI64(0.7*MAP_HEIGHT)<<32;
  dy=0;
  for (x=0;x<MAP_WIDTH;x++) {
    dy=LimitI64(SignI64(RandI16)<<30+dy,-3<<32,3<<32);
    y=LimitI64(y+dy,ToI64(0.3*MAP_HEIGHT)<<32,(MAP_HEIGHT-2)<<32);
    elevs[x]=y.i32[1];
  }
  gun_x=RandU32%(MAP_WIDTH-100)+50;
  gun_y=elevs[gun_x];
  gun_theta=0;
  gun_recoil=0;
  for (x=gun_x-20;x<=gun_x+20;x++)
    elevs[x]=gun_y;

  wind_x=RandI16/250.0;
  for (i=0;i<NUM_DUST;i++) {
    dust_x[i]=RandU16%MAP_WIDTH;
    dust_y[i]=RandU16%MAP_HEIGHT;
  }

  old_preempt=Preempt(OFF);
  ode=ODENew(0,1e-4,ODEF_HAS_MASSES);
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.0001;
  ode->acceleration_limit=5e5;
  QueIns(ode,Fs->last_ode);
  Fs->horz_scroll.min=0;
  Fs->horz_scroll.max=MAP_WIDTH-Fs->win_pixel_width;
  Fs->horz_scroll.pos=gun_x-Fs->win_pixel_width/2;
  Fs->vert_scroll.min=0;
  Fs->vert_scroll.max=MAP_HEIGHT-Fs->win_pixel_height;
  Fs->vert_scroll.pos=0;
  TaskDerivedValsUpdate;
  Preempt(old_preempt);

  DrawMap;
}

U0 CleanUp(CMathODE *ode)
{
  if (ode) {
    QueRem(ode);
    QueDel(&ode->next_mass,TRUE);
    ODEDel(ode);
  }
}

U0 BigGuns()
{
  I64 ch,sc;

  PopUpOk(
	"I refuse to rip-off the original\n"
	"so this is intentionally crappy\n"
	"and included for demonstration\n"
	"purposes.\n\n"
	"Write games, don't play them.\n");

  PopUpOk("The map scrolls.\n");

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Preempt(OFF);

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,CH_NEW_LINE);"
"  Fire(,CH_SPACE);"
"  Left(,,SC_CURSOR_LEFT);"
"  Right(,,SC_CURSOR_RIGHT);"
"}"
);

  WinMax;
  WinBorder(ON);
  WordStat;
  Init;
  DocCursor;
  DocClear;
  DocScroll;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      while (ScanKey(&ch,&sc))
	switch (ch) {
	  case 0:
	    switch (sc.u8[0]) {
	      case SC_CURSOR_RIGHT:
		gun_theta+=2.0*pi/180;
		if (gun_theta>0)
		  gun_theta=0;
		else
		  Spawn(&MoveTask,NULL,"Move",Fs);
		break;
	      case SC_CURSOR_LEFT:
		gun_theta-=2.0*pi/180;
		if (gun_theta<-pi)
		  gun_theta=-pi;
		else
		  Spawn(&MoveTask,NULL,"Move",Fs);
		break;
	    }
	    break;
	  case CH_NEW_LINE:
	    CleanUp(ode);
	    Init;
	    break;
	  case CH_SPACE:
	    Spawn(&FireTask,NULL,"Fire",Fs);
	    break;
	  case CH_SHIFT_ESC:
	  case CH_ESC:
	    goto bg_done;
	}
      ManageShots;
      WinMgrSync;
    }
bg_done:
  } catch
    CatchAll;
  SettingsPop;
  DCDel(maps[0]); maps[0]=NULL;
  DCDel(maps[1]); maps[1]=NULL;
  CleanUp(ode);
  MenuPop;
}

BigGuns;
