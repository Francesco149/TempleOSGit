class MyMass:MassStruct
{
  F64 radius;
};

class MySpring:SpringStruct
{
};

Ode *ode=NULL;


U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  BoolI8 old_suspend;
  MyMass   *tempm;
  MySpring *temps;

  old_suspend=LBts(&task->task_flags,TASKf_SUSPENDED);
  dc->color=RED;
  temps=ode->next_spring;
  while (temps!=&ode->next_spring) {
    GrLine(dc,temps->end1->x,temps->end1->y,
		temps->end2->x,temps->end2->y);
    temps=temps->next;
  }

  dc->color=BLACK;
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    GrCircle(dc,tempm->x,tempm->y,tempm->radius);
    tempm=tempm->next;
  }
  LBEqu(&task->task_flags,TASKf_SUSPENDED,old_suspend);
}


U0 MyDerivative(Ode *ode,F64,Order2D3 *,Order2D3 *)
{
//The forces due to springs and drag are
//automatically handled by the
//ode code.  We can add new forces
//here.
  F64 d,dd;
  D3 p;
  MyMass *tempm1,*tempm2;

  tempm1=ode->next_mass;
  while (tempm1!=&ode->next_mass) {
    tempm2=tempm1->next;
    while (tempm2!=&ode->next_mass) {
      D3Sub(&tempm2->state->x,&tempm1->state->x,&p);
      dd=D3SqrNorm(&p);
      if (dd<=Sqr(tempm1->radius+tempm2->radius)) {
	d=Sqrt(dd)+0.0001;
	dd=10.0*Sqr(Sqr(Sqr(tempm1->radius+tempm2->radius)-dd));
	D3MulEqu(&p,dd/d);
	D3AddEqu(&tempm2->DstateDt->DxDt,&p);
	D3SubEqu(&tempm1->DstateDt->DxDt,&p);
      }
      tempm2=tempm2->next;
    }
    tempm1=tempm1->next;
  }
}

U0 PlaceMass(I64 x, I64 y)
{
  MyMass *tempm=CAlloc(sizeof(MyMass));
  tempm->mass=1.0;
  tempm->drag_profile_factor=100.0;
  tempm->x=x;
  tempm->y=y;
  tempm->radius=10*(Rand+0.25);
  QueIns(tempm,ode->last_mass);
}

U0 PlaceSpring(MyMass *tempm1,MyMass *tempm2)
{
  MySpring *temps=CAlloc(sizeof(MySpring));
  temps->end1=tempm1;
  temps->end2=tempm2;
  temps->constant=10000;
  temps->rest_len=100;
  QueIns(temps,ode->last_spring);
}

U0 Init()
{
  ode=OdeNew(0,1e-4,ODEF_HAS_MASSES);
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.00001;
  ode->acceleration_limit=5e3;

  QueIns(ode,Fs->last_ode);
}

U0 CleanUp()
{
  QueRem(ode);
  QueDel(&ode->next_mass,TRUE);
  QueDel(&ode->next_spring,TRUE);
  OdeDel(ode);
}

U0 MassSpringDemo()
{
  I64 msg_code,p1,p2;
  MyMass *tempm1=NULL,*tempm2=NULL;

  PopUpOk("Left-Click to place mas\n"
	  "Right-Click and drag to\n"
	  "connect with spring.\n\n"
	  "Springs are 100 pixels long.\n");
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;

//This Preempt() eliminates a lot of hassles.
//The concern is about changing
//que links while the winmgr task
//is updating the ode or screen.

//The Window mgr suspends this
//task while updating the ode.  It
//will not be interrupted and with this
//preempt, we will not be interrupted.
//There is little downside to Preempt()
//since we will yield during GetMsg().
  Preempt(OFF);


  Fs->win_inhibit|=WIF_DBL_CLICK;
  WordStat;

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,CH_NEW_LINE);"
"}"
  );

  Init;
  LtfCursor;
  LtfClear;
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      msg_code=GetMsg(&p1,&p2,1<<MSG_IP_L_DOWN|1<<MSG_IP_R_DOWN|1<<MSG_IP_R_UP|1<<MSG_KEY_DOWN);
      switch (msg_code) {
	case MSG_IP_L_DOWN:
	  PlaceMass(p1,p2);
	  break;
	case MSG_IP_R_DOWN:
	  tempm1=MassFind(ode,p1,p2);
	  tempm2=NULL;
	  break;
	case MSG_IP_R_UP:
	  if (tempm1 && (tempm2=MassFind(ode,p1,p2)) && tempm1!=tempm2)
	    PlaceSpring(tempm1,tempm2);
	  tempm1=tempm2=NULL;
	  break;
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case CH_NEW_LINE:
	      CleanUp;
	      Init;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto ms_done;
	  }
	  break;
      }
    }
ms_done: //Don't goto out of try
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    CatchAll;
  SettingsPop;
  CleanUp;
  MenuPop;
}

MassSpringDemo;
