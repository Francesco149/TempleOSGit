//Uses [C:/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.



/* <1> <1> (image) */



/* <2> <2> (image) */



/* <3> <3> (image) */



/* <4> <4> (image) */



/* <5> <5> (image) */


#define FRAMES 6

CSprite *imgs[FRAMES]={
__BIN_1,__BIN_2,__BIN_3,__BIN_4,__BIN_3,__BIN_2};

F64 fire_end_time;

class Trooper
{
  I64 x,y,dx,dy,att,def,rng,player;
  F64 animate_time_base,fire_end_time;
  Trooper *target;
};

#define NUM_TROOPERS 100
Trooper tr[2][NUM_TROOPERS];

Bool time_lapse=FALSE;

#define TAP_MODE_RADIUS	50
Bool tap_mode;

#define AI_NOTHING 0
#define AI_TARGET  1
#define AI_RANDOM  2
#define AI_NUM_AI  3
I64 ai_mode;

I64 ai_targets[10];

U0 DrawTrooper(CTask *,CDC *dc,Trooper *tempt)
{
  CSprite *temps;
  I64 x,y,gx,gy;
  F64 speed,tt;

  if (tempt->def>0) {
    x=tempt->dx>>28;
    y=tempt->dy>>28;
    speed=0.5*Sqrt(x*x+y*y);

    tt=tempt->animate_time_base+tS*speed;
    if (time_lapse) {
      x=(tempt->x+500*tempt->dx)>>32;
      y=(tempt->y+500*tempt->dy)>>32;
    } else {
      x=tempt->x.i32[1];
      y=tempt->y.i32[1];
    }
    if (!tempt->player)
      dc->color=LTCYAN;
    else
      dc->color=LTPURPLE;
    if (tempt->target) {
      gx=x;gy=y;
      if (tempt->dx<0) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	GrSetSymmetry(dc,x,y,x,y+1);
	gx-=13;
	gy-=7;
      } else {
	dc->flags&=~DCF_SYMMETRY|DCF_JUST_MIRROR;
	gx+=13;
	gy-=7;
      }
      Sprite3(dc,x,y,0,__BIN_5);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
      if (!tempt->player)
	dc->color=WHITE;
      else
	dc->color=LTBLUE;
      GrLine3(dc,gx,gy,0,tempt->target->x.i32[1],tempt->target->y.i32[1],0);
    } else {
      if (tempt->dx<0) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	GrSetSymmetry(dc,x,y,x,y+1);
      } else
	dc->flags&=~DCF_SYMMETRY|DCF_JUST_MIRROR;
      temps=SpriteInterpolate(imgs[tt%FRAMES],imgs[(tt+1.0)%FRAMES],tt%1.0);
      Sprite3(dc,x,y,0,temps);
      Free(temps);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    }
  }
}

U0 DrawIt(CTask *task,CDC *dc)
{
  Trooper *tempt=tr;
  I64 i,j,cnt[2],
      x=ip.pos.x-task->pix_left-task->scroll_x,y=ip.pos.y-task->pix_top-task->scroll_y;
  Bool repulsive=ip.throttle>0 ^^ ip.rb,
	 active=!winmgr.grab_scroll && (ip.lb||ip.rb);

  for (j=0;j<2;j++) {
    cnt[j]=0;
    for (i=0;i<NUM_TROOPERS;i++,tempt++) {
      if (tempt->def>0) {
	DrawTrooper(task,dc,tempt);
	cnt[j]++;
      }
    }
  }
  if (tap_mode) {
    dc->color=YELLOW;
    GrCircle(dc,x,y,TAP_MODE_RADIUS);
  } else {
    if (repulsive) {
      if (active)
	dc->color=LTRED;
      else
	dc->color=RED;
    } else {
      if (active)
	dc->color=LTBLUE;
      else
	dc->color=BLUE;
    }
    GrCircle(dc,x,y,AbsI64(64*ip.throttle/(MAX_I32+1)));
  }
  dc->color=LTCYAN;
  GrPrint(dc,(task->win_right-8)*FONT_WIDTH,0,"%03d",cnt[0]);
  dc->color=LTPURPLE;
  GrPrint(dc,(task->win_right-4)*FONT_WIDTH,0,"%03d",cnt[1]);
}

U0 DoAiTarget()
{
  I64 i,j;
  Trooper *tempt1,*tempt0;
  for (i=0;i<10;i++) {
    tempt0=&tr[0][ai_targets[i]];
    for (j=0;j<10;j++) {
      tempt1=&tr[1][i*10+j];
      tempt1->dx=(tempt0->x-tempt1->x)>>11;
      tempt1->dy=(tempt0->y-tempt1->y)>>11;
    }
  }
}

U0 UpdatePos()
{
  I64 i,j;
  Trooper *tempt=tr;
  for (j=0;j<2;j++)
    for (i=0;i<NUM_TROOPERS;i++,tempt++) {
      tempt->x+=tempt->dx;
      if (tempt->x>=GR_WIDTH<<32) tempt->x-=GR_WIDTH<<32;
      if (tempt->x<0) tempt->x+=GR_WIDTH<<32;
      tempt->y+=tempt->dy;
      if (tempt->y>=GR_HEIGHT<<32) tempt->y-=GR_HEIGHT<<32;
      if (tempt->y<0) tempt->y+=GR_HEIGHT<<32;
    }
}

U0 ResolveFiring()
{
  I64 i,j,dd,dx,dy;
  Trooper *tempt=tr,*tempt0,*tempt1;

  for (j=0;j<2;j++)
    for (i=0;i<NUM_TROOPERS;i++,tempt++) {
      if (tempt->target && tempt->fire_end_time<tS) {
	tempt->target->def-=tempt->att;
	tempt->fire_end_time=0;
	tempt->target=NULL;
      }
    }

  for (i=0;i<NUM_TROOPERS;i++) {
    tempt0=&tr[0][i];
    for (j=0;j<NUM_TROOPERS;j++) {
      tempt1=&tr[1][j];
      if (tempt0->def>0 && tempt1->def>0) {
	dx=(tempt0->x-tempt1->x)>>32;
	dy=(tempt0->y-tempt1->y)>>32;
	dd=dx*dx+dy*dy;
	if (dd<tempt0->rng && !tempt0->target) {
	  fire_end_time=tempt0->fire_end_time=tS+0.125;
	  Snd(2000);
	  tempt0->target=tempt1;
	}
	if (dd<tempt1->rng && !tempt1->target) {
	  fire_end_time=tempt1->fire_end_time=tS+0.125;
	  Snd(2000);
	  tempt1->target=tempt0;
	}
      }
    }
  }

  if (tS>=fire_end_time)
    Snd(0);
}

U0 UpdateHumanVelocities()
{
  F64 intensity;
  I64 i,j,x=ip.pos.x-Fs->pix_left-Fs->scroll_x,y=ip.pos.y-Fs->pix_top-Fs->scroll_y,dx,dy,d;
  Bool active=!winmgr.grab_scroll&&(ip.lb||ip.rb);
  Trooper *tempt=&tr[0][0];
  if (tap_mode) {
    for (i=0;i<NUM_TROOPERS;i++,tempt++) {
      dx=x- tempt->x.i32[1];
      dy=y- tempt->y.i32[1];
      if ((d=dx*dx+dy*dy) && d<TAP_MODE_RADIUS*TAP_MODE_RADIUS) {
	intensity=SqrI64(SqrI64(SqrI64(TAP_MODE_RADIUS)-d));
	dx=intensity*dx/d;
	dy=intensity*dy/d;
	tempt->dx=tempt->dx-dx;
	tempt->dy=tempt->dy-dy;
      } else {
	tempt->dx-=0.2*tempt->dx;
	tempt->dy-=0.2*tempt->dy;
      }
    }
  } else if (active) {
    j=8*ip.throttle;
    if (ip.rb)
      j=-j;
    for (i=0;i<NUM_TROOPERS;i++,tempt++) {
      dx=x- tempt->x.i32[1];
      dy=y- tempt->y.i32[1];
      if (d=dx*dx+dy*dy) {
	dx=j*dx/d;
	dy=j*dy/d;
	tempt->dx=tempt->dx-dx;
	tempt->dy=tempt->dy-dy;
      }
    }
  }
}

U0 Init()
{
  I64 i,j,dx,dy;
  Trooper *tempt;
  ip.throttle=MAX_I32;
  ip.throttle_step=(MAX_I32+1)>>2;
  tap_mode=FALSE;
  time_lapse=FALSE;
  fire_end_time=0;
  MemSet(tr,0,sizeof(tr));
  for (i=0;i<10;i++) {
    ai_targets[i]=RandU16%NUM_TROOPERS;
    dx=RandI32;
    dy=RandI32;
    for (j=0;j<10;j++) {
      tempt=&tr[0][i*10+j];
      tempt->x=(GR_WIDTH-100-i*10)<<32;
      tempt->y=(GR_HEIGHT>>1-50+j*10)<<32;
      tempt->att=3;
      tempt->def=10;
      tempt->rng=50*50;
      tempt->animate_time_base=10*Rand;
      tempt->player=0;

      tempt=&tr[1][i*10+j];
      tempt->x=(100+i*10)<<32;
      tempt->y=(GR_HEIGHT>>1-50+j*10)<<32;
      if (ai_mode==AI_RANDOM) {
	tempt->dx=dx;
	tempt->dy=dy;
      }
      tempt->att=3;
      tempt->def=10;
      tempt->rng=50*50;
      tempt->animate_time_base=10*Rand;
      tempt->player=1;
    }
  }
  ai_mode=RandU16%AI_NUM_AI;
}

U0 BattleLines()
{
  CMenuEntry *tempse;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+WHITE;
  Preempt(OFF);

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,'\n');"
  "  TimeLapse(,'1');"
  "  TapMode(,'2');"
      "}"
      );
  WordStat;
  WinMax;
  WinBorder;
  Preempt(OFF);

  Init;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  PopUpOk("The mouse wheel controls\n"
	"the command force.\n"
	"Attract or repel with\n"
	"left or right button.\n\n");

  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER-WIF_FOCUS_TASK_MENU;
  try {
    while (TRUE) {
      switch (ScanChar) {
	case '1':
	  time_lapse=!time_lapse;
	  tempse=MenuEntryFind(Fs->cur_menu,"Play/TimeLapse");
	  tempse->checked=time_lapse;
	  break;
	case '2':
	  tap_mode=!tap_mode;
	  tempse=MenuEntryFind(Fs->cur_menu,"Play/TapMode");
	  tempse->checked=tap_mode;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto wg_done;
	case '\n':
	  Init;
	  break;
      }
      Sleep(40);
      if (ai_mode==AI_TARGET)
	DoAiTarget;
      UpdateHumanVelocities;
      UpdatePos;
      ResolveFiring;
    }
wg_done:
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
}

BattleLines;
