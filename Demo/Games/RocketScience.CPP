/*If you find this interesting, you might want to check-out
SimStructure, a simulation program I wrote you can downlload.

http://www.templeos.org/files/SimStrSetUp.zip

I did study ctrl systems for 2 years, but lost enthusiasm
when I learned everything in real-life is nonlinear.
You're supposed to linearize around points and apply
control theory.  I just like to appy a PID ctrller
and fiddle intelligently with the feedback coefficients.

Real rockets have dynamics like fuel slosh to consider!
*/

#define THRUST	1000

Bool	blast_off,plane_hit;

CMass	m1, //Bottom of rocket
	m2, //Top of rocket
	m3; //Plane
CSpring s;

#define PT_SPAN	40


   /* <1> <1> (image) */



   /* <2> <2> (image) */



  /* <3> <3> (image) */
 
CDC	*dc2;
CMathODE *ode;

#define STATE_NOZZLE_ANGLE		0
#define STATE_NOZZLE_ANGLE_VELOCITY	1
#define STATE_NUM			2

CD3	target;
F64	my_dbg,antispin_coefficient;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,x,y,cx=task->pix_width/2,cy=(task->win_height-4)*FONT_HEIGHT-PT_SPAN/2;
  F64 theta=Arg(m2.x-m1.x,m2.y-m1.y),
     nozzle_angle=ode->state[STATE_NOZZLE_ANGLE];
  if (blast_off) {
    x=m1.x-10*Cos(theta+nozzle_angle);
    y=m1.y-10*Sin(theta+nozzle_angle);
    for (i=0;i<6;i++) {
      if ((i^winmgr.updates)&1)
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
	case 0: dc2->color=WHITE; break;
	case 1: dc2->color=LTGRAY; break;
	case 2: dc2->color=DKGRAY; break;
	case 3: dc2->color=BLACK; break;
      }
      GrPlot(dc2,cx+(x+RandU16%12-6),cy-(y+RandU16%12-6));
    }
  }

  if (plane_hit)
    Sprite3(dc,cx+m3.x,cy-m3.y,0,__BIN_2);
  else
    Sprite3(dc,cx+m3.x,cy-m3.y,0,__BIN_1);

  if (blast_off && !plane_hit) {
    dc->color=ROP_COLLISION;
    dc->bkcolor=LTCYAN;
    dc->collision_cnt=0;
    SpriteZ3B(dc,cx+(m1.x+m2.x)/2,cy-(m1.y+m2.y)/2,0,__BIN_3,-theta);
    if (dc->collision_cnt>100) {
      Noise(1000,500,1500);
      plane_hit=TRUE;
    } else
      Snd(50);
  } else if (!plane_hit)
    Snd(0);

  dc->color=ROP_EQU;
  SpriteZ3B(dc,cx+(m1.x+m2.x)/2,cy-(m1.y+m2.y)/2,0,__BIN_3,-theta);

  dc->color=RED;
  GrCircle(dc,cx+target.x,cy-target.y,5);

  dc->color=BLUE;
  GrCircle(dc,cx+m3.x,cy-m3.y,5);

  dc->color=BLACK;
  GrPrint(dc,0,FONT_HEIGHT,"%12.6f",my_dbg);
}

U0 MyDerivative(CMathODE *,F64,F64 *state,F64 *DstateDt)
{
  F64 d,discriminant,v,a,theta=Arg(m2.state->x-m1.state->x,m2.state->y-m1.state->y),
     DthetaDt,
     collision_estimate_t,
     target_heading,target_angle_error,
     desired_nozzle_angle;
  CD3 p,p_target,p_body;

  //Unit vect pointing to top of rocket from bottom.
  D3Sub(&m2.state->x,&m1.state->x,&p_body);
  D3Unit(&p_body);

  //DthetaDt lets us prevent too spin.
  DthetaDt=antispin_coefficient*
    (m2.state->DyDt*p_body.x-m2.state->DxDt*p_body.y-
     m1.state->DyDt*p_body.x+m1.state->DxDt*p_body.y)/PT_SPAN;

  //p_target is vect from top of rocket to plane.
  D3Sub(&m3.state->x,&m2.state->x,&p_target);

  //d=0.5at^2+vt
  d=D3Norm(&p_target);
 
  D3Copy(&p,&p_target);
  D3Unit(&p);
  v=(m2.state->DxDt*p.x+m2.state->DyDt*p.y)-
    (m3.state->DxDt*p.x+m3.state->DyDt*p.y);

  a=THRUST/(m1.mass+m2.mass);

  discriminant=v*v+4*0.5*a*d;
  if (discriminant>0)
    collision_estimate_t=(-v+Sqrt(discriminant))/a;
  else
    collision_estimate_t=0;
  my_dbg=collision_estimate_t;

  //Aim for projected pos of plane at time of impact.
  D3Copy(&p,&m3.state->DxDt);
  D3MulEqu(&p,collision_estimate_t);
  D3AddEqu(&p_target,&p);

  D3Copy(&target,&p_target);
  D3AddEqu(&target,&m2.state->x);

  target_heading=Arg(p_target.x,p_target.y);
  target_angle_error=Wrap(theta-target_heading); //Force to range [-pi,pi)
  desired_nozzle_angle=Clamp(50.0*DthetaDt+750*target_angle_error,-pi/8,pi/8);

  //For realism we limit the speed the nozzle angle can change.
  DstateDt[STATE_NOZZLE_ANGLE]=state[STATE_NOZZLE_ANGLE_VELOCITY];
  DstateDt[STATE_NOZZLE_ANGLE_VELOCITY]=
    Clamp(10000*(desired_nozzle_angle-state[STATE_NOZZLE_ANGLE]),-1000,1000)+
    -10.0*state[STATE_NOZZLE_ANGLE_VELOCITY]; //Damping

  if (blast_off) {
    m1.DstateDt->DxDt+=THRUST*Cos(theta+state[STATE_NOZZLE_ANGLE]);
    m1.DstateDt->DyDt+=THRUST*Sin(theta+state[STATE_NOZZLE_ANGLE]);

    m1.DstateDt->DyDt-=25; //Gravity
    m2.DstateDt->DyDt-=25;
  }

  //For more realism reduce the mass of the rocket because of fuel.
  //You might also factor-in fuel slosh in the tank.

  //To do this, you would have to set-up state vars for mass and
  //do A=F/m manually instead of relyin on [C:/Adam/MathODE.CPP.Z,323] ODECallDerivative() to divide
  //by mass.
}

U0 Init()
{
  blast_off=FALSE;
  plane_hit=FALSE;

  do antispin_coefficient=PopUpRangeF64Exp(0.1,10.001,Sqrt(10),
    "%9.4f","Anti-spin Coefficient\n\n");
  while (!(0.1<=antispin_coefficient<10.001));

//We don't clear que links.
  MemSet(&m1.start_saved_area,0,offset(CMass.end_saved_area)-offset(CMass.start_saved_area));
  m1.y=-PT_SPAN/2;
  m1.mass=1.0;

  MemSet(&m2.start_saved_area,0,offset(CMass.end_saved_area)-offset(CMass.start_saved_area));
  m2.y=PT_SPAN/2;
  m2.mass=1.0;

  MemSet(&m3.start_saved_area,0,offset(CMass.end_saved_area)-offset(CMass.start_saved_area));
  m3.y=380;
  m3.x=-300;
  m3.DxDt=50;
  m3.mass=1.0;

  MemSet(&s.start_saved_area,0,offset(CSpring.end_saved_area)-offset(CSpring.start_saved_area));
  s.end1=&m1;
  s.end2=&m2;
  s.rest_len=PT_SPAN;
  s.const=10000;

  ode->state[STATE_NOZZLE_ANGLE]=0;
  ode->state[STATE_NOZZLE_ANGLE_VELOCITY]=0;

  DCFill;
}

U0 TaskEndCB()
{
  DCFill;
  SndTaskEndCB;
}

U0 RocketScience()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=YELLOW<<4+BLUE;
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
"  Launch(,CH_SPACE);"
"}"
);

  WinMax;
  dc2=DCAlias;
  Fs->task_end_cb=&TaskEndCB;

  DocClear;
  "$BG,LTCYAN$%h*c",Fs->win_height-4,'\n';
 
  ode=ODENew(STATE_NUM,1e-6,ODEF_HAS_MASSES);
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.00001;
  ode->acceleration_limit=5e3;

//  ode->t_scale=0.1; //Uncomment this to go in slow motion.

  Init;
  QueIns(&m1,ode->last_mass);
  QueIns(&m2,ode->last_mass);
  QueIns(&m3,ode->last_mass);
  QueIns(&s,ode->last_spring);

  QueIns(ode,Fs->last_ode);

  Fs->draw_it=&DrawIt;

  try {
    GetKey;
    blast_off=TRUE;
    while (TRUE) {
      switch (GetChar(,FALSE)) {
	case '\n':
	  Init;
	  GetKey;
	  blast_off=TRUE;
	  break;
	case CH_SHIFT_ESC:
	case CH_ESC:
	  goto rs_done;
      }
    }
rs_done:
  } catch
    PutExcept;
  QueRem(ode);
  ODEDel(ode);
  DocClear;
  SettingsPop;
  DCFill;
  DCDel(dc2);
  MenuPop;
}

RocketScience;
