//Uses [C:/Demo/Lectures/FixedPoint.HC.Z,1] fixed-point.



/* <1> <1> (image) */



/* <2> <2> (image) */



/* <3> <3> (image) */



/* <4> <4> (image) */



/* <5> <5> (image) */


#define FRAMES 6

U8 *imgs[FRAMES]={
__BIN_1,__BIN_2,__BIN_3,__BIN_4,__BIN_3,__BIN_2};

F64 fire_end_time;

class Trooper
{
  I64 x,y,dx,dy,att,def,rng,player;
  F64 animate_time_base,fire_end_time;
  Trooper *target;
};

#define TROOPERS_NUM 100
Trooper tr[2][TROOPERS_NUM];

Bool time_lapse=FALSE;

#define TAP_MODE_RADIUS	50
Bool tap_mode;

#define AI_NOTHING 0
#define AI_TARGET  1
#define AI_RANDOM  2
#define AI_AI_NUM  3
I64 ai_mode;

I64 ai_targets[10];

U0 DrawTrooper(CTask *,CDC *dc,Trooper *tmpt)
{
  U8 *tmps;
  I64 x,y,gx,gy;
  F64 speed,tt;

  if (tmpt->def>0) {
    x=tmpt->dx>>28;
    y=tmpt->dy>>28;
    speed=0.5*Sqrt(x*x+y*y);

    tt=tmpt->animate_time_base+tS*speed;
    if (time_lapse) {
      x=(tmpt->x+500*tmpt->dx)>>32;
      y=(tmpt->y+500*tmpt->dy)>>32;
    } else {
      x=tmpt->x.i32[1];
      y=tmpt->y.i32[1];
    }
    if (tmpt->target) {
      gx=x;gy=y;
      if (tmpt->dx<0) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	DCSymmetrySet(dc,x,y,x,y+1);
	gx-=13;
	gy-=7;
      } else {
	dc->flags&=~DCF_SYMMETRY|DCF_JUST_MIRROR;
	gx+=13;
	gy-=7;
      }

      dc->color=BLACK;
      Sprite3(dc,x+1,y,0,__BIN_5);

      if (!tmpt->player)
	dc->color=LTCYAN;
      else
	dc->color=LTPURPLE;
      Sprite3(dc,x,y,0,__BIN_5);

      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
      if (!tmpt->player)
	dc->color=WHITE;
      else
	dc->color=LTBLUE;
      GrLine3(dc,gx,gy,0,tmpt->target->x.i32[1],tmpt->target->y.i32[1],0);
    } else {
      if (tmpt->dx<0) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	DCSymmetrySet(dc,x,y,x,y+1);
      } else
	dc->flags&=~DCF_SYMMETRY|DCF_JUST_MIRROR;
      tmps=SpriteInterpolate(tt%1.0,imgs[tt%FRAMES],imgs[(tt+1.0)%FRAMES]);

      dc->color=BLACK;
      Sprite3(dc,x+1,y,0,tmps);

      if (!tmpt->player)
	dc->color=LTCYAN;
      else
	dc->color=LTPURPLE;
      Sprite3(dc,x,y,0,tmps);

      Free(tmps);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    }
  }
}

U0 DrawIt(CTask *task,CDC *dc)
{
  Trooper *tmpt=tr;
  I64 i,j,cnt[2],
	x=ms.pos.x-task->pix_left-task->scroll_x,
	y=ms.pos.y-task->pix_top-task->scroll_y;
  Bool repulsive=ms.pos.z>0 ^^ ms.rb,
	active=!winmgr.grab_scroll && (ms.lb||ms.rb);

  for (j=0;j<2;j++) {
    cnt[j]=0;
    for (i=0;i<TROOPERS_NUM;i++,tmpt++) {
      if (tmpt->def>0) {
	DrawTrooper(task,dc,tmpt);
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
    GrCircle(dc,x,y,AbsI64(ms.pos.z));
  }
  dc->color=LTCYAN;
  GrPrint(dc,(task->win_right-8)*FONT_WIDTH,0,"%03d",cnt[0]);
  dc->color=LTPURPLE;
  GrPrint(dc,(task->win_right-4)*FONT_WIDTH,0,"%03d",cnt[1]);
}

U0 DoAiTarget()
{
  I64 i,j;
  Trooper *tmpt1,*tmpt0;
  for (i=0;i<10;i++) {
    tmpt0=&tr[0][ai_targets[i]];
    for (j=0;j<10;j++) {
      tmpt1=&tr[1][i*10+j];
      tmpt1->dx=(tmpt0->x-tmpt1->x)>>11;
      tmpt1->dy=(tmpt0->y-tmpt1->y)>>11;
    }
  }
}

U0 UpdatePos()
{
  I64 i,j;
  Trooper *tmpt=tr;
  for (j=0;j<2;j++)
    for (i=0;i<TROOPERS_NUM;i++,tmpt++) {
      tmpt->x+=tmpt->dx;
      if (tmpt->x>=GR_WIDTH<<32) tmpt->x-=GR_WIDTH<<32;
      if (tmpt->x<0) tmpt->x+=GR_WIDTH<<32;
      tmpt->y+=tmpt->dy;
      if (tmpt->y>=GR_HEIGHT<<32) tmpt->y-=GR_HEIGHT<<32;
      if (tmpt->y<0) tmpt->y+=GR_HEIGHT<<32;
    }
}

U0 ResolveFiring()
{
  I64 i,j,dd,dx,dy;
  Trooper *tmpt=tr,*tmpt0,*tmpt1;

  for (j=0;j<2;j++)
    for (i=0;i<TROOPERS_NUM;i++,tmpt++) {
      if (tmpt->target && tmpt->fire_end_time<tS) {
	tmpt->target->def-=tmpt->att;
	tmpt->fire_end_time=0;
	tmpt->target=NULL;
      }
    }

  for (i=0;i<TROOPERS_NUM;i++) {
    tmpt0=&tr[0][i];
    for (j=0;j<TROOPERS_NUM;j++) {
      tmpt1=&tr[1][j];
      if (tmpt0->def>0 && tmpt1->def>0) {
	dx=(tmpt0->x-tmpt1->x)>>32;
	dy=(tmpt0->y-tmpt1->y)>>32;
	dd=dx*dx+dy*dy;
	if (dd<tmpt0->rng && !tmpt0->target) {
	  fire_end_time=tmpt0->fire_end_time=tS+0.125;
	  Snd(86);
	  tmpt0->target=tmpt1;
	}
	if (dd<tmpt1->rng && !tmpt1->target) {
	  fire_end_time=tmpt1->fire_end_time=tS+0.125;
	  Snd(86);
	  tmpt1->target=tmpt0;
	}
      }
    }
  }

  if (tS>=fire_end_time)
    Snd;
}

U0 UpdateHumanVelocities()
{
  F64 intensity;
  I64 i,j,dx,dy,d,
	x=ms.pos.x-Fs->pix_left-Fs->scroll_x,
	y=ms.pos.y-Fs->pix_top-Fs->scroll_y;
  Bool active=!winmgr.grab_scroll&&(ms.lb||ms.rb);
  Trooper *tmpt=&tr[0][0];
  if (tap_mode) {
    for (i=0;i<TROOPERS_NUM;i++,tmpt++) {
      dx=x- tmpt->x.i32[1];
      dy=y- tmpt->y.i32[1];
      if ((d=dx*dx+dy*dy) && d<TAP_MODE_RADIUS*TAP_MODE_RADIUS) {
	intensity=SqrI64(SqrI64(SqrI64(TAP_MODE_RADIUS)-d));
	dx=intensity*dx/d;
	dy=intensity*dy/d;
	tmpt->dx=tmpt->dx-dx;
	tmpt->dy=tmpt->dy-dy;
      } else {
	tmpt->dx-=0.2*tmpt->dx;
	tmpt->dy-=0.2*tmpt->dy;
      }
    }
  } else if (active) {
    j=400000000*ms.pos.z;
    if (ms.rb)
      j=-j;
    for (i=0;i<TROOPERS_NUM;i++,tmpt++) {
      dx=x- tmpt->x.i32[1];
      dy=y- tmpt->y.i32[1];
      if (d=dx*dx+dy*dy) {
	dx=j*dx/d;
	dy=j*dy/d;
	tmpt->dx=tmpt->dx-dx;
	tmpt->dy=tmpt->dy-dy;
      }
    }
  }
}

U0 Init()
{
  I64 i,j,dx,dy;
  Trooper *tmpt;
  MsSet(,,25);
  tap_mode=FALSE;
  time_lapse=FALSE;
  fire_end_time=0;
  MemSet(tr,0,sizeof(tr));
  for (i=0;i<10;i++) {
    ai_targets[i]=RandU16%TROOPERS_NUM;
    dx=RandI32;
    dy=RandI32;
    for (j=0;j<10;j++) {
      tmpt=&tr[0][i*10+j];
      tmpt->x=(GR_WIDTH-100-i*10)<<32;
      tmpt->y=(GR_HEIGHT>>1-50+j*10)<<32;
      tmpt->att=3;
      tmpt->def=10;
      tmpt->rng=50*50;
      tmpt->animate_time_base=10*Rand;
      tmpt->player=0;

      tmpt=&tr[1][i*10+j];
      tmpt->x=(100+i*10)<<32;
      tmpt->y=(GR_HEIGHT>>1-50+j*10)<<32;
      if (ai_mode==AI_RANDOM) {
	tmpt->dx=dx;
	tmpt->dy=dy;
      }
      tmpt->att=3;
      tmpt->def=10;
      tmpt->rng=50*50;
      tmpt->animate_time_base=10*Rand;
      tmpt->player=1;
    }
  }
  ai_mode=RandU16%AI_AI_NUM;
}

U0 BattleLines()
{
  CMenuEntry *tmpse;

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

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+WHITE;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  Init;
  Fs->draw_it=&DrawIt;
  PopUpOk("The mouse wheel controls\n"
	"the command force.\n"
	"Attract or repel with\n"
	"left or right bttn.\n\n");

  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS
	-WIF_SELF_BORDER-WIF_FOCUS_TASK_MENU;
  try {
    while (TRUE) {
      switch (ScanChar) {
	case '1':
	  time_lapse=!time_lapse;
	  tmpse=MenuEntryFind(Fs->cur_menu,"Play/TimeLapse");
	  tmpse->checked=time_lapse;
	  break;
	case '2':
	  tap_mode=!tap_mode;
	  tmpse=MenuEntryFind(Fs->cur_menu,"Play/TapMode");
	  tmpse->checked=tap_mode;
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
