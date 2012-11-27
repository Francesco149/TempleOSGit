
/* <ship_null> <1> (image) */


/* <ship_thrust> <2> (image) */



/* <ship_null2> <3> (image) */


/* <ship_thrust2> <4> (image) */



/* <tiny_rock> <5> (image) */


/* <tiny_rock2> <6> (image) */


/* <small_rock> <7> (image) */


/* <small_rock2> <8> (image) */



/* <medium_rock> <9> (image) */


/* <medium_rock2> <10> (image) */




/* <large_rock> <11> (image) */


/* <large_rock2> <12> (image) */


/* <Bullet> <13> (image) */




/* <Cargo> <14> (image) */





#define CMD_NULL	0
#define CMD_SPIN_LEFT	1
#define CMD_SPIN_RIGHT	2
#define CMD_THRUST	3
#define CMD_FIRE	4
#define CMD_GAME_OVER	5
I64 cmd_code=CMD_NULL;

#define NUM_STARS	100
I64 stars_x[NUM_STARS],stars_y[NUM_STARS];

Bool bounce;

#define OT_SHIP		1
#define OT_CARGO	2
#define OT_BULLET	3
#define OT_TINY_ROCK	4
#define OT_SMALL_ROCK	5
#define OT_MEDIUM_ROCK	6
#define OT_LARGE_ROCK	7

#define A_TRANSITION	-1
#define A_INACTIVE	0
#define A_ACTIVE	1

class Order2D2theta
{
  F64 x,y,theta;
  F64 DxDt,DyDt,DthetaDt;
};

class Obj
{
  I32 active;
  I32 type;
  U8 *img;
  F64 mass,radius;
  F64 timeout;
};

#define NUM_OBJECTS	256

CMathODE *ode;
Order2D2theta *o_state;
Obj *objs;

U0 ObjInit(I64 i, I64 type)
{
  objs[i].active=A_ACTIVE;
  objs[i].timeout=0;
  objs[i].type=type;
  switch (type) {
    case OT_SHIP:
      if (bounce)
	objs[i].img=__BIN_1;
      else
	objs[i].img=__BIN_3;
      objs[i].radius=15;
      break;
    case OT_CARGO:
      objs[i].img=__BIN_14;
      objs[i].radius=30;
      break;
    case OT_BULLET:
      objs[i].img=__BIN_15;
      objs[i].radius=7;
      break;
    case OT_TINY_ROCK:
      if (i&1)
	objs[i].img=__BIN_5;
      else
	objs[i].img=__BIN_6;
      objs[i].radius=5;
      break;
    case OT_SMALL_ROCK:
      if (i&1)
	objs[i].img=__BIN_16;
      else
	objs[i].img=__BIN_8;
      objs[i].radius=11;
      break;
    case OT_MEDIUM_ROCK:
      if (i&1)
	objs[i].img=__BIN_17;
      else
	objs[i].img=__BIN_10;
      objs[i].radius=20;
      break;
    case OT_LARGE_ROCK:
      if (i&1)
	objs[i].img=__BIN_18;
      else
	objs[i].img=__BIN_12;
      objs[i].radius=26;
      break;
  }
  objs[i].mass=objs[i].radius*objs[i].radius*objs[i].radius;
}

I64 ObjAlloc(I64 type)
{
  I64 i;
  for (i=0;i<NUM_OBJECTS-1;i++)
    if (objs[i].active==A_INACTIVE)
      break;
  ObjInit(i,type);
  return i;
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,x,y;

  task->text_attr=BLACK<<4+WHITE;

  dc->color=WHITE;
  for (i=0;i<NUM_STARS;i++)
    GrPlot(dc,stars_x[i],stars_y[i]);

  for (i=0;i<NUM_OBJECTS;i++) {
    if (objs[i].active==A_ACTIVE) {
      if (objs[i].timeout && objs[i].timeout<tP(task))
	objs[i].active=A_INACTIVE;
      else {
	x=o_state[i].x;
	y=o_state[i].y;

	if (x<0) {
	  x=GR_WIDTH-AbsI64(x)%GR_WIDTH;
	  o_state[i].x=x;
	} else if (x>GR_WIDTH) {
	  x%=GR_WIDTH;
	  o_state[i].x=x;
	}
	if (y<0) {
	  y=GR_HEIGHT-AbsI64(y)%GR_HEIGHT;
	  o_state[i].y=y;
	} else if (y>GR_HEIGHT) {
	  y%=GR_HEIGHT;
	  o_state[i].y=y;
	}
	switch (objs[i].type) {
	  case OT_SHIP:
	    if (bounce) {
	      if (cmd_code==CMD_THRUST && RandU16&0x8000)
		CSpritePlotRotZ3b(dc,x,y,0,__BIN_2,o_state[i].theta);
	      else
		CSpritePlotRotZ3b(dc,x,y,0,__BIN_1,o_state[i].theta);
	    } else {
	      if (cmd_code==CMD_THRUST && RandU16&0x8000)
		CSpritePlotRotZ3b(dc,x,y,0,__BIN_4,o_state[i].theta);
	      else
		CSpritePlotRotZ3b(dc,x,y,0,__BIN_3,o_state[i].theta);
	    }
	    break;
	  case OT_BULLET:
	  case OT_TINY_ROCK:
	  case OT_SMALL_ROCK:
	  case OT_MEDIUM_ROCK:
	  case OT_LARGE_ROCK:
	  case OT_CARGO:
	    CSpritePlotRotZ3b(dc,x,y,0,objs[i].img,o_state[i].theta);
	    break;
	}
      }
    }
  }
  dc->color=WHITE;
  GrPutS(dc,0,0,"Escort the cargo ship.");
}

U0 Init()
{
  I64 i,j;

  cmd_code=CMD_NULL;

  for (i=0;i<NUM_STARS;i++) {
    stars_x[i]=RandU16%GR_WIDTH;
    stars_y[i]=RandU16%GR_HEIGHT;
  }

  MemSet(objs,0,sizeof(Obj)*NUM_OBJECTS);
  MemSet(o_state,0,sizeof(Order2D2theta)*NUM_OBJECTS);

  i=ObjAlloc(OT_SHIP); //always #0
  objs[i].mass=2.0*objs[i].radius*objs[i].radius*objs[i].radius;
  o_state[i].x=GR_WIDTH/2;
  o_state[i].y=GR_HEIGHT/2;

  i=ObjAlloc(OT_CARGO); //always #1
  objs[i].mass=0.5*objs[i].radius*objs[i].radius*objs[i].radius;
  o_state[i].x=0;
  o_state[i].DxDt=16.0;
  o_state[i].y=GR_HEIGHT/2;
  o_state[i].theta=pi/2;

  for (j=0;j<4;j++) {
    i=ObjAlloc(OT_LARGE_ROCK);
    o_state[i].x=RandU32%GR_WIDTH;
    o_state[i].y=RandU32%GR_HEIGHT;
    o_state[i].DxDt=RandI16/2000.0;
    o_state[i].DyDt=RandI16/2000.0;
    o_state[i].DthetaDt=RandI16/25000.0;
    o_state[i].theta=2*pi*RandI16/16384.0;
  }
}

U0 MyDerivative(CMathODE *,F64,Order2D2theta *state,Order2D2theta *DstateDt)
{
  I64 i,j;
  F64 f,d,d1,dd,dx,dy;

  MemSet(DstateDt,0,sizeof(Order2D2theta)*NUM_OBJECTS);
  for (i=0;i<NUM_OBJECTS;i++) {
    if (objs[i].active==A_ACTIVE) {
      DstateDt[i].x=state[i].DxDt;
      DstateDt[i].y=state[i].DyDt;
      DstateDt[i].theta=state[i].DthetaDt;
      for (j=0;j<NUM_OBJECTS;j++) {
	if (i!=j && objs[j].active==A_ACTIVE) {
	  dx=state[i].x-state[j].x;
	  dy=state[i].y-state[j].y;
	  dd=Sqr(dx)+Sqr(dy);
	  d=Sqrt(dd);

	  if (d) {
	    dx/=d;
	    dy/=d;
	  } else {
	    dx=0;
	    dy=0;
	  }

	  d1=objs[i].radius+objs[j].radius;
	  f=0;
	  if (dd<Sqr(d1)) {
	    if (bounce && objs[i].type!=OT_CARGO && objs[j].type!=OT_CARGO) {
	      f=Sqr(Sqr(Sqr(d1-Abs(d)+0.1))); //spring force
	      if (f>1000000.0) f=1000000.0;
	    }
	  } else
	    f=-2.5*objs[i].mass*objs[j].mass/(dd+1.0);

	  DstateDt[i].DxDt+=f*dx/objs[i].mass;
	  DstateDt[i].DyDt+=f*dy/objs[i].mass;
	}
      }
    }
  }
  switch (cmd_code) {
    case CMD_SPIN_LEFT:
      DstateDt[0].DthetaDt-=10.0;
      break;
    case CMD_SPIN_RIGHT:
      DstateDt[0].DthetaDt+=10.0;
      break;
    case CMD_THRUST:
      DstateDt[0].DxDt+=100.0*Cos(state[0].theta-pi/2.0);
      DstateDt[0].DyDt+=100.0*Sin(state[0].theta-pi/2.0);
      break;
  }

  DstateDt[1].DxDt+=2.0*Cos(state[1].theta-pi/2.0);
  DstateDt[1].DyDt+=2.0*Sin(state[1].theta-pi/2.0);
}

U0 Collision(I64 i,I64 j)
{
  I64 i2;
  switch (objs[i].type) {
    case OT_SHIP:
      if (!bounce) {
	cmd_code=CMD_GAME_OVER;
	objs[i].active=A_INACTIVE;
      }
      break;
    case OT_CARGO:
      cmd_code=CMD_GAME_OVER;
      objs[i].active=A_INACTIVE;
      break;
    case OT_TINY_ROCK:
      if (objs[j].type==OT_BULLET && !bounce)
	objs[i].active=A_INACTIVE;
      break;
    case OT_BULLET:
      if (!bounce)
	objs[i].active=A_INACTIVE;
      break;
    case OT_SMALL_ROCK:
    case OT_MEDIUM_ROCK:
      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x+objs[i2].radius+2.0;
      o_state[i2].y=o_state[i].y;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x-objs[i2].radius-2.0;
      o_state[i2].y=o_state[i].y;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      objs[i].active=A_INACTIVE;
      break;
    case OT_LARGE_ROCK:
      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x+objs[i2].radius+4.0;
      o_state[i2].y=o_state[i].y+objs[i2].radius+4.0;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x-objs[i2].radius-4.0;
      o_state[i2].y=o_state[i].y-objs[i2].radius-4.0;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x-objs[i2].radius-4.0;
      o_state[i2].y=o_state[i].y+objs[i2].radius+4.0;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      i2=ObjAlloc(objs[i].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[i].x+objs[i2].radius+4.0;
      o_state[i2].y=o_state[i].y-objs[i2].radius-4.0;
      o_state[i2].theta=o_state[i].theta;
      o_state[i2].DxDt=o_state[i].DxDt;
      o_state[i2].DyDt=o_state[i].DyDt;
      o_state[i2].DthetaDt=o_state[i].DthetaDt;

      objs[i].active=A_INACTIVE;
      break;
  }

  switch (objs[j].type) {
    case OT_SHIP:
      if (!bounce) {
	cmd_code=CMD_GAME_OVER;
	objs[j].active=A_INACTIVE;
      }
      break;
    case OT_CARGO:
      cmd_code=CMD_GAME_OVER;
      objs[j].active=A_INACTIVE;
      break;
    case OT_TINY_ROCK:
      if (objs[i].type==OT_BULLET && !bounce)
	objs[j].active=A_INACTIVE;
      break;
    case OT_BULLET:
      if (!bounce)
	objs[j].active=A_INACTIVE;
      break;
    case OT_SMALL_ROCK:
    case OT_MEDIUM_ROCK:
      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x+objs[i2].radius+2.0;
      o_state[i2].y=o_state[j].y;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x-objs[i2].radius-2.0;
      o_state[i2].y=o_state[j].y;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      objs[j].active=A_INACTIVE;
      break;

    case OT_LARGE_ROCK:
      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x+objs[i2].radius+4.0;
      o_state[i2].y=o_state[j].y+objs[i2].radius+4.0;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x-objs[i2].radius-4.0;
      o_state[i2].y=o_state[j].y-objs[i2].radius-4.0;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x-objs[i2].radius-4.0;
      o_state[i2].y=o_state[j].y+objs[i2].radius+4.0;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      i2=ObjAlloc(objs[j].type-1);
      objs[i2].active=A_TRANSITION;
      o_state[i2].x=o_state[j].x+objs[i2].radius+4.0;
      o_state[i2].y=o_state[j].y-objs[i2].radius-4.0;
      o_state[i2].theta=o_state[j].theta;
      o_state[i2].DxDt=o_state[j].DxDt;
      o_state[i2].DyDt=o_state[j].DyDt;
      o_state[i2].DthetaDt=o_state[j].DthetaDt;

      objs[j].active=A_INACTIVE;
      break;
  }
}

U0 CheckCollisions()
{
  I64 i,j;
  F64 d,d1,dd,dx,dy;
  CDC *dc;
  for (i=0;i<NUM_OBJECTS;i++) {
    if (objs[i].active==A_ACTIVE) {
      for (j=i+1;j<NUM_OBJECTS;j++) {
	if (objs[j].active==A_ACTIVE) {
	  dx=o_state[i].x-o_state[j].x;
	  dy=o_state[i].y-o_state[j].y;
	  dd=Sqr(dx)+Sqr(dy);
	  d=Sqrt(dd);

	  if (d) {
	    dx/=d;
	    dy/=d;
	  } else {
	    dx=0;
	    dy=0;
	  }

	  d1=objs[i].radius+objs[j].radius;
	  if (dd<Sqr(d1)) {
	    dc=DCNew(DCT_COLOR8,64,64,Fs);
	    CSpritePlotRotZ3b(dc,32,32,0,objs[i].img,o_state[i].theta);
	    dc->color=ROP_COLLISION;
	    dc->bkcolor=BLACK;
	    CSpritePlotRotZ3b(dc,o_state[j].x-o_state[i].x+32,
			       o_state[j].y-o_state[i].y+32,0,
			       objs[j].img,o_state[j].theta);
	    if (dc->collision_cnt>0)
	      Collision(i,j);
	    DCDel(dc);
	  }
	}
      }
    }
  }
  for (i=0;i<NUM_OBJECTS;i++)
    if (objs[i].active==A_TRANSITION)
      objs[i].active=A_ACTIVE;
}

#define MUZZLE_VELOCITY 500.0

U0 Fire()
{
  F64 theta=o_state[0].theta;
  I64 i=ObjAlloc(OT_BULLET);
  objs[i].timeout=tP+0.5;
  objs[i].mass*=10.0;
  o_state[i].x=o_state[0].x+(objs[0].radius+objs[i].radius+1.0)*Cos(theta-pi/2.0);
  o_state[i].y=o_state[0].y+(objs[0].radius+objs[i].radius+1.0)*Sin(theta-pi/2.0);
  o_state[i].DxDt=o_state[0].DxDt+MUZZLE_VELOCITY*Cos(theta-pi/2.0);
  o_state[i].DyDt=o_state[0].DyDt+MUZZLE_VELOCITY*Sin(theta-pi/2.0);
  o_state[i].DthetaDt=RandI16/3000.0;

//not real physics with the 0.005 factor
  o_state[0].DxDt-=0.005*MUZZLE_VELOCITY*Cos(theta-pi/2.0)*objs[i].mass/objs[0].mass;
  o_state[0].DyDt-=0.005*MUZZLE_VELOCITY*Sin(theta-pi/2.0)*objs[i].mass/objs[0].mass;
}

U0 CheckCrazy()
{
  I64 i;
  F64 d;
  for (i=0;i<NUM_OBJECTS;i++)
    if (objs[i].active==A_ACTIVE) {
      d=Sqrt(Sqr(o_state[i].DxDt)+Sqr(o_state[i].DyDt));
      if (d>MUZZLE_VELOCITY*5.0)
	Auto(" ");
    }
}

U0 SongTask(I64)
{ //Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("W13eGGsGGeGGGe.GeCDsEDeCBAe.AeG");
    Play("GsGGeGGGe.GeFEsDEeDCBe.AeFD");
    Play("EDFDEDECDCECDCsGGeG");
    Play("GG2hG3sGGeGGG2hG");
  }
}

U0 Rocks()
{
  I64 ch,sc,msg_code;
  CMenuEntry *tempse;

  PopUpOk(
	"I refuse to rip-off the original\n"
	"so this is intentionally unplayable\n"
	"and included for demonstration\n"
	"purposes.\n\n"
	"Write games, don't play them.\n");

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->song_task=Spawn(&SongTask,NULL,"Song",Fs);
  Preempt(OFF),
  WinBorder;
  WordStat;

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,CH_NEW_LINE);"
"  Fire(,CH_SPACE);"
"  Fwd(,,SC_CURSOR_UP);"
"  Left(,,SC_CURSOR_LEFT);"
"  Right(,,SC_CURSOR_RIGHT);"
"  ToggleBounce(,'b');"
"}"
);
  tempse=MenuEntryFind(Fs->cur_menu,"Play/ToggleBounce");
  tempse->checked=bounce=TRUE;

  ode=ODENew(NUM_OBJECTS*6,0.01);
  ode->derivative=&MyDerivative;
  ode->min_tolerance=0.001*ode->max_tolerance;
  o_state=ode->state;
  objs=MAlloc(NUM_OBJECTS*sizeof(Obj));

  WinMax;

  Init;
  QueIns(ode,Fs->last_ode);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      while (msg_code=ScanMsg(&ch,&sc,1<<MSG_KEY_DOWN|1<<MSG_KEY_UP)) {
	if (msg_code==MSG_KEY_DOWN)
	  switch (ch) {
	    case 0:
	      switch (sc.u8[0]) {
		case SC_CURSOR_RIGHT:
		  cmd_code=CMD_SPIN_RIGHT;
		  break;
		case SC_CURSOR_LEFT:
		  cmd_code=CMD_SPIN_LEFT;
		  break;
		case SC_CURSOR_UP:
		  cmd_code=CMD_THRUST;
		  break;
	      }
	      break;
	    case CH_SPACE:
	      Fire;
	      break;
	    case CH_NEW_LINE:
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto rk_done;
	    case 'b':
	      tempse=MenuEntryFind(Fs->cur_menu,"Play/ToggleBounce");
	      tempse->checked=bounce=!bounce;
	      break;
	  }
	else //MSG_KEY_UP
	  cmd_code=CMD_NULL;
      }
      CheckCollisions;
      CheckCrazy;
      if (cmd_code==CMD_GAME_OVER) {
	Sleep(1000);
	Init;
      }
      WinMgrSync; //msgs are only qued by winmngr
    }
rk_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    CatchAll;

  QueRem(ode);
  ODEDel(ode);
  Free(objs);
  MenuPop;
 
  SettingsPop;
}

Rocks;
