#define THRUST	100

Bool blast_off;
CMass m1, //Bottom of rocket
	   m2; //Top of rocket
CSpring s;

#define PT_SPAN	40



  /* <1> <1> (image) */

CDC *dc2;


U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,x,y,cx=task->win_pixel_width/2,cy=(task->win_height-4)*FONT_HEIGHT-PT_SPAN/2;
  Bool engine_on;
  F64 nozzle_angle,theta=Arg(m2.x-m1.x,m2.y-m1.y);

  if (Bt(key_down_bitmap,SC_CURSOR_UP)) {
    nozzle_angle=0;
    engine_on=TRUE;
  } else if (Bt(key_down_bitmap,SC_CURSOR_LEFT)) {
    nozzle_angle=pi/8;
    engine_on=TRUE;
  } else if (Bt(key_down_bitmap,SC_CURSOR_RIGHT)) {
    nozzle_angle=-pi/8;
    engine_on=TRUE;
  } else
    engine_on=FALSE;

  if (engine_on) {
    x=m1.x-10*Cos(theta+nozzle_angle);
    y=m1.y-10*Sin(theta+nozzle_angle);
    for (i=0;i<6;i++) {
      if ((i^win_updates)&1)
	dc->color=YELLOW;
      else
	dc->color=RED;
      GrLine(dc,cx+(m1.x+i*Cos(theta-pi/2)),
		  cy-(m1.y+i*Sin(theta-pi/2)),
		  cx+x,cy-y);
      GrLine(dc,cx+(m1.x+i*Cos(theta+pi/2)),
		  cy-(m1.y+i*Sin(theta+pi/2)),
		  cx+x,cy-y);
    }

    for (i=0;i<10;i++) {
      switch (RandU16&3) {
	case 0: dc2->color=ROP_EQU+WHITE; break;
	case 1: dc2->color=ROP_EQU+LTGRAY; break;
	case 2: dc2->color=ROP_EQU+DKGRAY; break;
	case 3: dc2->color=ROP_EQU+BLACK; break;
      }
      GrPlot(dc2,cx+(x+RandU16%12-6),cy-(y+RandU16%12-6));
    }
    Snd(50);
  } else
    Snd(0);
  SpritePlotRotZ3b(dc,cx+(m1.x+m2.x)/2,cy-(m1.y+m2.y)/2,0,__BIN_1,-theta);
}

U0 MyDerivative(CMathODE *,F64,COrder2D3 *,COrder2D3 *)
{
  Bool engine_on;
  F64 nozzle_angle,theta=Arg(m2.state->x-m1.state->x,m2.state->y-m1.state->y);

  if (Bt(key_down_bitmap,SC_CURSOR_UP)) {
    nozzle_angle=0;
    engine_on=TRUE;
  } else if (Bt(key_down_bitmap,SC_CURSOR_LEFT)) {
    nozzle_angle=pi/8;
    engine_on=TRUE;
  } else if (Bt(key_down_bitmap,SC_CURSOR_RIGHT)) {
    nozzle_angle=-pi/8;
    engine_on=TRUE;
  } else
    engine_on=FALSE;

  if (engine_on) {
    m1.DstateDt->DxDt+=THRUST*Cos(theta+nozzle_angle);
    m1.DstateDt->DyDt+=THRUST*Sin(theta+nozzle_angle);
  }
  if (blast_off) {
    m1.DstateDt->DyDt-=25; //Gravity
    m2.DstateDt->DyDt-=25;
  }
}

U0 Init()
{
  blast_off=FALSE;

//We don't clear que links.
  MemSet(&m1.start_saved_area,0,offset(CMass.end_saved_area)-offset(CMass.start_saved_area));
  m1.y=-PT_SPAN/2;

  MemSet(&m2.start_saved_area,0,offset(CMass.end_saved_area)-offset(CMass.start_saved_area));
  m2.y=PT_SPAN/2;

  MemSet(&s.start_saved_area,0,offset(CSpring.end_saved_area)-offset(CSpring.start_saved_area));
  s.end1=&m1;
  s.end2=&m2;
  s.rest_len=PT_SPAN;
  s.constant=10000;

  DCFill;
}

U0 TaskEndCB()
{
  DCFill;
  SndTaskEndCB;
}

U0 Rocket()
{
  CMathODE *ode=ODENew(0,1e-2,ODEF_HAS_MASSES);

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocCursor;
  WordStat;

//The ode nodes are traversed by win mgr.
  Preempt(OFF);

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,'\n');"
"  Up(,,SC_CURSOR_UP);"
"  UpLeft(,,SC_CURSOR_LEFT);"
"  UpRight(,,SC_CURSOR_RIGHT);"
"}"
);

  WinMax;
  dc2=DCAlias;
  Fs->task_end_cb=&TaskEndCB;

  "$CL$$BG,LTCYAN$$FG,GREEN$Up, Left, Right$FG$";
  NewLine(Fs->win_height-4);
  "$BG,YELLOW$\n";
 
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.00001;
  ode->acceleration_limit=5e3;

  Init;
  QueIns(&m1,ode->last_mass);
  QueIns(&m2,ode->last_mass);
  QueIns(&s,ode->last_spring);

  QueIns(ode,Fs->last_ode);

  Fs->draw_it=&DrawIt;

  try {
    GetKey;
    blast_off=TRUE;
    while (TRUE) {
      switch (GetChar(NULL,FALSE)) {
	case '\n':
	  Init;
	  GetKey;
	  blast_off=TRUE;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto rk_done;
      }
    }
rk_done:
  } catch
    CatchAll;
  QueRem(ode);
  ODEDel(ode);
  "$CL$$BG$$FG$\n";
  SettingsPop;
  DCFill;
  DCDel(dc2);
  MenuPop;
}

Rocket;
