//The ball and men were generated
//with [C:/Apps/GrModels/Run.CPP.Z,1].
//They were cut-and-pasted here.


		/* <1> <1> (image) */





		/* <2> <2> (image) */






		/* <3> <3> (image) */






		/* <4> <4> (image) */






		/* <5> <5> (image) */






		/* <6> <6> (image) */






		/* <7> <7> (image) */






		/* <8> <8> (image) */






		/* <9> <9> (image) */






		/* <10> <10> (image) */






		/* <11> <11> (image) */






		/* <12> <12> (image) */





		/* <13> <13> (image) */






		/* <14> <14> (image) */





		/* <15> <15> (image) */





		/* <16> <16> (image) */






		/* <17> <17> (image) */






		/* <18> <18> (image) */






		/* <19> <19> (image) */






		/* <20> <20> (image) */






		/* <21> <21> (image) */






		/* <22> <22> (image) */






		/* <23> <23> (image) */



class Frame
{
  CSprite *img[2];
  F64 dt;
};

#define COURT_BORDER		10
#define COLLISION_DAMP		0.8

/* Viscosity is way bigger than
real air viscosity.  In reality
air is approximated by V and V2
terms and is smaller.  However, I use
this to produce a rolling friction
value, too.  If you want to see my
best attempt at realistic physics
download SimStructure.

http://www.templeos.org/files/SimStrSetUp.zip

This is just a video game. Relax.
*/
#define AIR_VISCOSITY		0.1

#define GRAVITY_ACCELERATION	500
#define SHOT_VELOCITY		400
#define DRIBBLE_T		0.25
#define MAN_VELOCITY		150
#define MAN_SQR_RADIUS		(20*20)
#define FOUL_VELOCITY_THRESHOLD 50
#define JUMP_VELOCITY		250
#define ROLL_VELOCITY_THRESHOLD	100
#define RANDOM_MAN_ACCELERATION	30

#define HEAD_Z_OFFSET	200
#define HAND_X_OFFSET	30
#define HAND_Y_OFFSET	20
#define HAND_SQR_OFFSET	(HAND_X_OFFSET*HAND_X_OFFSET+HAND_Y_OFFSET*HAND_Y_OFFSET)
#define HAND_Z_OFFSET	110

#define FIRST_STANDING		0

#define NUM_RUNNING_IMGS	4
#define FIRST_RUNNING	0
#define LAST_RUNNING	(FIRST_RUNNING+NUM_RUNNING_IMGS-1)

#define NUM_SHOOTING_IMGS	5
#define FIRST_SHOOTING	(LAST_RUNNING+1)
#define LAST_SHOOTING	(FIRST_SHOOTING+NUM_SHOOTING_IMGS-1)

#define NUM_DRIBBLING_IMGS	4
#define FIRST_DRIBBLING	(LAST_SHOOTING+1)
#define LAST_DRIBBLING	(FIRST_DRIBBLING+NUM_DRIBBLING_IMGS-1)

#define NUM_STOPPED_DRIBBLING_IMGS	2
#define FIRST_STOPPED_DRIBBLING	(LAST_DRIBBLING+1)
#define LAST_STOPPED_DRIBBLING	(FIRST_STOPPED_DRIBBLING+NUM_STOPPED_DRIBBLING_IMGS-1)

Frame imgs[LAST_STOPPED_DRIBBLING+1]={
{{__BIN_6,__BIN_7},2*DRIBBLE_T/NUM_RUNNING_IMGS},{{__BIN_2,__BIN_3},2*DRIBBLE_T/NUM_RUNNING_IMGS},
{{__BIN_6,__BIN_7},2*DRIBBLE_T/NUM_RUNNING_IMGS},{{__BIN_4,__BIN_5},2*DRIBBLE_T/NUM_RUNNING_IMGS},
{{__BIN_8,__BIN_9},0.1},{{__BIN_10,__BIN_11},0.2},{{__BIN_12,__BIN_13},0.2},{{__BIN_12,__BIN_13},0.1},{{__BIN_14,__BIN_15},0.1},
{{__BIN_20,__BIN_21},2*DRIBBLE_T/NUM_DRIBBLING_IMGS},{{__BIN_16,__BIN_17},2*DRIBBLE_T/NUM_DRIBBLING_IMGS},
{{__BIN_20,__BIN_21},2*DRIBBLE_T/NUM_DRIBBLING_IMGS},{{__BIN_18,__BIN_19},2*DRIBBLE_T/NUM_DRIBBLING_IMGS},
{{__BIN_20,__BIN_21},DRIBBLE_T/NUM_STOPPED_DRIBBLING_IMGS},{{__BIN_22,__BIN_23},DRIBBLE_T/NUM_STOPPED_DRIBBLING_IMGS},
};

AcctRegSetDftEntry("TempleOS/KeepAway","I64 best_score0=0,best_score1=9999;\n");
AcctRegExeBranch("TempleOS/KeepAway");

F64 game_t_end,foul_t_end;
I64 score0,score1;

#define NUM_PER_SIDE	3
#define NUM_OBJS	(NUM_PER_SIDE*2+1)
Bool someone_shooting;

class Obj
{
  I64 team; //-1 is ball
  F64 x,y,z,DxDt,DyDt,DzDt,ay,radius,stolen_t0;
  F64 last_t0,next_t0,foul_t0;
  I64 last_img,next_img;
  Bool stopped,shooting,has_ball,pad[5];
} objs[NUM_OBJS],*ball,*human;

/*Just to be different, I didn't use the built-in
DCF_TRANSFORMATION flag in this game.
Instead, I chose a 45 degree angle
between Y and Z as the view point.
If I had used the transform, I would
have to make all my men taller.
This is a little simpler, and faster,
but adds lots of factor 2 vals.

I also didn't use the [C:/Kernel/KernelA.HPP.Z,241] CMathODE feat,
just to be different.
*/

U0 DrawObj(CDC *dc,Obj *o,F64 tt)
{
  U8 *temps;
  F64 r1=Max(9-0.1*o->z,1),r2=Max(r1/4,1);

  if (o==human)
    dc->color=LTRED;
  else
    dc->color=BLACK;
  GrEllipse(dc,o->x,o->y/2,r1,r2);
  GrFloodFill(dc,o->x,o->y/2);

  if (o==ball)
    Sprite3(dc,o->x,(o->y-o->z)/2,GR_Z_ALL-o->y,__BIN_1);
  else {
    temps=SpriteInterpolate(imgs[o->last_img].img[o->team],
	imgs[o->next_img].img[o->team],
	(tt-o->last_t0)/(o->next_t0-o->last_t0));
    SpriteY3B(dc,o->x,(o->y-o->z)/2,GR_Z_ALL-o->y,temps,o->ay);
    Free(temps);
  }
}

I64 ObjCompare(Obj *o1,Obj *o2)
{
  return o1->y-o2->y;
}

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 tt=tS,d,d_down,d_up;
  I64 i;
  Obj *o_sort[NUM_OBJS],*o;

  DCAllocDepthBuf(dc);
  dc->ls.x=10000;
  dc->ls.y=60000;
  dc->ls.z=10000;
  d=65535/D3I32Norm(&dc->ls);
  dc->ls.x*=d;
  dc->ls.y*=d;
  dc->ls.z*=d;

  dc->pen_width=2;
  dc->color=RED;
  GrBorder(dc,COURT_BORDER,COURT_BORDER,
      task->pix_width -1-COURT_BORDER,
      task->pix_height-1-COURT_BORDER);
  for (i=0;i<NUM_OBJS;i++) {
    o=o_sort[i]=&objs[i];
    if (o!=ball) {
      if (o->has_ball) {
	ball->x=o->x+HAND_X_OFFSET*Cos(o->ay-pi/2)+HAND_Y_OFFSET*Cos(o->ay);
	//The factor 2 is because the man is not transformed.
	ball->y=o->y+HAND_X_OFFSET*Sin(o->ay-pi/2)/2+HAND_Y_OFFSET*Sin(o->ay)/2;
	if (ball->z+ball->radius*2>o->z+HAND_Z_OFFSET)
	  ball->z=o->z+HAND_Z_OFFSET-ball->radius*2;
      } else if (o->shooting) {
	ball->x=o->x;
	ball->y=o->y;
	ball->z=o->z+HEAD_Z_OFFSET;
      }
      if (tt>o->next_t0) {
	if (o->has_ball && (ball->z+ball->radius*2>=o->z+HAND_Z_OFFSET || Abs(ball->DzDt)<30)) {
	//This is an approximation.  My instinct tells me the viscosity term needs an [C:/Kernel/KernelB.HPP.Z,95] Exp().
	//However, we should be syncronized to img frames, so we don't have to be perfect.
	  d_down=1.0;
	  d_up	=1.0/COLLISION_DAMP; //Up bounce takes higher percentage because speed lost in collision.
	  ball->DzDt=-((d_down+d_up)*(o->z+HAND_Z_OFFSET-ball->radius*4)/(1.0-AIR_VISCOSITY)+
	    0.5*GRAVITY_ACCELERATION*(
	      Sqr(DRIBBLE_T*d_up/(d_down+d_up))-
	      Sqr(DRIBBLE_T*d_down/(d_down+d_up)) ))/DRIBBLE_T;
	}
	o->last_t0=tt;
	o->last_img=o->next_img++;
	if (o->stopped) {
	  if (o->has_ball) {
	    if (!(FIRST_STOPPED_DRIBBLING<=o->next_img<=LAST_STOPPED_DRIBBLING))
	      o->next_img=FIRST_STOPPED_DRIBBLING;
	  } else
	    o->next_img=FIRST_STANDING;
	  o->stopped=FALSE;
	} else if (o->shooting) {
	  if (!(FIRST_SHOOTING<=o->last_img<=LAST_SHOOTING))
	    o->next_img=FIRST_SHOOTING;
	  if (o->next_img>LAST_SHOOTING) {
	    o->next_img=FIRST_STANDING;
	    someone_shooting=o->has_ball=o->shooting=FALSE;
	    ball->DxDt=o->DxDt+SHOT_VELOCITY/sqrt2*Cos(o->ay-pi/2);
	    ball->DyDt=o->DyDt+SHOT_VELOCITY/sqrt2*Sin(o->ay-pi/2);
	    ball->DzDt=o->DzDt+SHOT_VELOCITY/sqrt2;
	  } else {
	    ball->DxDt=0;
	    ball->DyDt=0;
	    ball->DzDt=0;
	  }
	} else if (o->has_ball) {
	  if (FIRST_RUNNING<=o->next_img<=LAST_RUNNING)
	    o->next_img+=FIRST_DRIBBLING-FIRST_RUNNING;
	  if (!(FIRST_DRIBBLING<=o->next_img<=LAST_DRIBBLING))
	    o->next_img=FIRST_DRIBBLING;
	} else {
	  if (FIRST_DRIBBLING<=o->next_img<=LAST_DRIBBLING)
	    o->next_img+=FIRST_RUNNING-FIRST_DRIBBLING;
	  if (!(FIRST_RUNNING<=o->next_img<=LAST_RUNNING))
	    o->next_img=FIRST_RUNNING;
	}
	o->next_t0+=imgs[o->last_img].dt;
	if (o->next_t0<=tt)
	  o->next_t0=tt+imgs[o->last_img].dt;
      }
    }
  }

  QSortI64(o_sort,NUM_OBJS,&ObjCompare);
  for (i=0;i<NUM_OBJS;i++)
    DrawObj(dc,o_sort[i],tt);
  tt=(game_t_end-tS)/60;
  if (tt<=0) {
    dc->color=RED;
    tt=0;
    if (Blink)
      GrPrint(dc,(task->pix_width-FONT_WIDTH*9)>>1,
		    (task->pix_height-FONT_HEIGHT)>>1,"Game Over");
  } else {
    if (tS<foul_t_end) {
      dc->color=LTRED;
      if (Blink)
	GrPrint(dc,(task->pix_width-FONT_WIDTH*4)>>1,
		     (task->pix_height-FONT_HEIGHT)>>1,"Foul");
    }
    dc->color=BLACK;
  }
  GrPrint(dc,0,0,"Time:%d:%04.1f  Score:",ToI64(tt),(tt-ToI64(tt))*60);
  GrPrint  (dc,FONT_WIDTH*27,0,"Best Score:");

  dc->color=LTCYAN;
  GrPrint(dc,FONT_WIDTH*20,0,"%02d",score0);
  dc->color=LTPURPLE;
  GrPrint(dc,FONT_WIDTH*23,0,"%02d",score1);

  dc->color=LTCYAN;
  GrPrint(dc,FONT_WIDTH*39,0,"%02d",best_score0);
  dc->color=LTPURPLE;
  GrPrint(dc,FONT_WIDTH*42,0,"%02d",best_score1);
}

U0 Shoot(Obj *o)
{
  if (!someone_shooting && o->has_ball) {
    someone_shooting=o->stopped=o->shooting=TRUE;
    o->has_ball=FALSE;
  }
}

U0 AnimateTask(CTask *parent_task)
{
  F64 d,dx,dy,dt,dx2,dy2,t0=tS;
  I64 i,j,team;
  Bool gets_ball,ball_stolen;
  Obj *o;
  while (TRUE) {
    dt=tS-t0;
    t0=tS;

    if (game_t_end && game_t_end<t0) {
      game_t_end=0;
      Beep;
      if (score0-score1>best_score0-best_score1) {
	best_score0=score0;
	best_score1=score1;
	Snd(2000);Sleep(100); Snd(0);Sleep(100);
	Snd(2000);Sleep(100); Snd(0);Sleep(100);
      }
    }

    if (game_t_end)
      for (i=0;i<NUM_OBJS;i++) {
	o=&objs[i];

	if (o==ball) {
	  o->x+=dt*o->DxDt;
	  o->y+=dt*o->DyDt;
	  if (!someone_shooting)
	    o->z+=dt*(o->DzDt-0.5*GRAVITY_ACCELERATION*dt);
	} else {
	  if (!o->has_ball) {
	    if (t0-o->stolen_t0>2.0 && !someone_shooting) {
	      dx=ball->x-o->x;
	      dy=ball->y-o->y;
	      if (dx*dx+dy*dy<HAND_SQR_OFFSET && ball->z<o->z+HAND_Z_OFFSET) {
		gets_ball=TRUE;
		ball_stolen=FALSE;
		for (j=0;j<NUM_PER_SIDE*2;j++)
		  if (j!=i && objs[j].has_ball) {
		    if (Rand<2.0*dt) {
		      objs[j].stolen_t0=t0;
		      objs[j].has_ball=FALSE;
		      ball_stolen=TRUE;
		      team=objs[j].team;
		    } else
		      gets_ball=FALSE;
		  }
		if (gets_ball) {
		  o->has_ball=TRUE;
		  if (o==human) {
		    if (ball_stolen && team) {
		      Noise(250,2000,2000);
		      score0+=2;
		    }
		  } else {
		    if (o->team) {
		      Noise(250,1000,1000);
		      score1+=2;
		    } else {
		      Noise(250,2000,2000);
		      score0+=2;
		    }
		  }
		}
	      }
	    }
	  } else if (o!=human && Rand<0.25*dt)
	    Shoot(o);
	  if (!o->shooting) {
	    if (o==human) {
	      dx=(ip.pos.x-parent_task->pix_left-parent_task->scroll_x)-o->x;
	      dy=(ip.pos.y-parent_task->pix_top-parent_task->scroll_y)*2-o->y;
	    } else {
	      dx=o->DxDt+=RANDOM_MAN_ACCELERATION/sqrt2*RandI16/MAX_I16*dt;
	      dy=o->DyDt+=RANDOM_MAN_ACCELERATION/sqrt2*RandI16/MAX_I16*dt;
	    }
	    d=Sqrt(dx*dx+dy*dy);
	    if (d>=1.0) {
	      o->ay=Arg(dx,dy)+pi/2;
	      dx*=MAN_VELOCITY/sqrt2*dt/d;
	      dy*=MAN_VELOCITY/sqrt2*dt/d;
	      for (j=0;j<NUM_PER_SIDE*2;j++)
		if (j!=i) {
		  dx2=objs[j].x-o->x;
		  dy2=objs[j].y-o->y;
		  if ((d=Sqr(dx2)+Sqr(dy2))<MAN_SQR_RADIUS) {
		    if (d) {
		      d=Sqrt(d);
		      dx2/=d;
		      dy2/=d;
		    }
		    if (t0>o->foul_t0+0.15) {
		      d=(dx-objs[j].DxDt)*dx2+(dy-objs[j].DyDt)*dy2;
		      if (o==human && t0>o->foul_t0+1.0 &&
			  dt && d/dt>FOUL_VELOCITY_THRESHOLD && objs[j].team) {
			Noise(250,500,500);
			score1+=1;
			foul_t_end=t0+1.0;
		      }
		      o->foul_t0=t0;
		    }
		  }
		}
	      if (t0<o->foul_t0+0.15) {
		dx=-dx;
		dy=-dy;
	      }
	      o->x+=dx;
	      o->y+=dy;
	      o->stopped=FALSE;
	    } else
	      o->stopped=TRUE;
	  }
	  if (o->DzDt)
	    o->z+=dt*(o->DzDt-0.5*GRAVITY_ACCELERATION*dt);
	}

	if (o->x+o->radius>=parent_task->pix_width-COURT_BORDER) {
	  o->x=parent_task->pix_width-COURT_BORDER-1-o->radius;
	  o->DxDt=-COLLISION_DAMP*o->DxDt;
	  if (o==ball)
	    Noise(10,1000,2000);
	}
	if (o->x-o->radius<COURT_BORDER) {
	  o->x=COURT_BORDER+o->radius;
	  o->DxDt=-COLLISION_DAMP*o->DxDt;
	  if (o==ball)
	    Noise(10,1000,2000);
	}

	if (o->y+o->radius*2>=(parent_task->pix_height-COURT_BORDER)*2) {
	  o->y=(parent_task->pix_height-COURT_BORDER)*2-1-o->radius*2;
	  o->DyDt=-COLLISION_DAMP*o->DyDt;
	  if (o==ball)
	    Noise(10,1000,2000);
	}
	if (o->y-o->radius*2<2*COURT_BORDER) {
	  o->y=COURT_BORDER*2+o->radius*2;
	  o->DyDt=-COLLISION_DAMP*o->DyDt;
	  if (o==ball)
	    Noise(10,1000,2000);
	}

	if (o->z-o->radius*2<0) {
	  o->z=o->radius*2;
	  o->DzDt=-COLLISION_DAMP*o->DzDt;
	  if (o->DzDt>ROLL_VELOCITY_THRESHOLD)
	    Noise(10,1000,2000);
	  if (o!=ball)
	    o->DzDt=0;
	} else if (o->z-o->radius*2>0)
	  o->DzDt-=GRAVITY_ACCELERATION*dt;
	if (o==ball) {
	  d=Exp(-AIR_VISCOSITY*dt);
	  o->DxDt*=d;
	  o->DyDt*=d;
	  o->DzDt*=d;
	}
      }

      WinMgrSync;
  }
}

U0 Init()
{
  I64 i;
  someone_shooting=FALSE;
  MemSet(&objs,0,sizeof(objs));
  for (i=0;i<NUM_PER_SIDE*2;i++) {
    objs[i].team=i&1;
    objs[i].x=Fs->pix_width/2;
    objs[i].y=2*Fs->pix_height/2;
    objs[i].next_img=objs[i].last_img=FIRST_RUNNING;
  }
  human=&objs[0];
  ball =&objs[i];
  ball->team=-1;
  ball->x=0.5*Fs->pix_width/2;
  ball->y=0.5*2*Fs->pix_height/2;
  ball->radius=11;
  ball->z=ball->radius;
  score0=score1=0;
  game_t_end=tS+3*60;
  foul_t_end=0;
}

U0 CleanUp()
{
}

U0 KeepAway()
{
  I64 msg_code,a1,a2;

  PopUpOk(
    "$RED$Pass or hand-off to score points.$FG$\n\n"
    "Left-Click to pass.\n"
    "Right-Click to jump.\n");
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=BLACK+YELLOW<<4;
  WinMax;
  WinBorder;
  Preempt(OFF);

  Fs->win_inhibit|=WIG_DBL_CLICK;
  AutoComplete;

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,'\n');"
"  Shoot(,CH_SPACE);"
"  Jump(,'j');"
"}"
  );

  Init;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->animate_task=Spawn(&AnimateTask,Fs,"Animate",,Fs);

  try {
    while (TRUE) {
      msg_code=GetMsg(&a1,&a2,1<<MSG_IP_L_DOWN|1<<MSG_IP_R_DOWN|1<<MSG_KEY_DOWN);
      switch (msg_code) {
	case MSG_IP_L_DOWN:
ka_shoot:
	  Shoot(human);
	  break;
	case MSG_IP_R_DOWN:
ka_jump:
	  human->DzDt=JUMP_VELOCITY;
	  break;
	case MSG_KEY_DOWN:
	  switch (a1) {
	    case '\n':
	      CleanUp;
	      Init;
	      break;
	    case 'j':
	      goto ka_jump;
	    case CH_SPACE:
	      goto ka_shoot;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto ka_done;
	  }
	  break;
      }
    }
ka_done: //Don't goto out of try
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  AcctRegWriteBranch("TempleOS/KeepAway","I64 best_score0=%d,best_score1=%d;\n",
    best_score0,best_score1);
  CleanUp;
  MenuPop;
}
