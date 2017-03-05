RegDft("TempleOS/BomberGolf","I64 best_score=99999;\n");
RegExe("TempleOS/BomberGolf");



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


#define MAP_WIDTH	600
#define MAP_HEIGHT	600

#define TREES_NUM	64
class Tree
{
  I64 x,y;
} trees[TREES_NUM];
 

I64 target_cnt,key_cnt;


#define TARGETS_NUM	10

#define MDF_TANK		0
#define MDF_BUNKER	1

#define TANK_V		10.0

class Target
{
  F64	x,y,theta;
  U8	*alive_img,*dead_img1,*dead_img2;
  U8	type;
  Bool	dead,pad[6];
} targets[TARGETS_NUM];

#define FALL_TIME	3.00
#define EXPLODE_TIME	0.25
class Bomb
{
  Bomb *next,*last;
  F64 x,y,t;
  Bool exploding;
} bomb_head;
 
F64 v,x,y,
    theta,thetaf; //thetaf is the final theta. theta is gradually changed until it reaches thetaf.

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,j;
  Bomb *tmpb;
  Target *tmpt;
  F64 ts=tS,dx,dy;

  dc->color=ROPF_DITHER|BROWN<<16|YELLOW;
  GrRect3(dc,0,0,0,dc->width,dc->height);

  dc->x=task->pix_width>>1;
  dc->y=task->pix_height>>1;
  dc->flags|=DCF_TRANSFORMATION;
  Mat4x4TranslationEqu(dc->r,x,y,0);
  Mat4x4RotZ(dc->r,theta);
  dc->color=BLACK;
  GrBorder(dc,-MAP_WIDTH>>1,-MAP_HEIGHT>>1,MAP_WIDTH>>1,MAP_HEIGHT>>1);
  for (i=0;i<TARGETS_NUM;i++) {
    tmpt=&targets[i];
    if (tmpt->dead) {
      if (i&1) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	DCSymmetry3Set(dc,tmpt->x,tmpt->y,0,tmpt->x,tmpt->y,1,
	      tmpt->x+1024*Cos(tmpt->theta),tmpt->y+1024*Sin(tmpt->theta),0);
      }
      if (Blink(15))
	Sprite3ZB(dc,tmpt->x,tmpt->y,0,tmpt->dead_img1,tmpt->theta);
      else
	Sprite3ZB(dc,tmpt->x,tmpt->y,0,tmpt->dead_img2,tmpt->theta);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    } else
      Sprite3ZB(dc,tmpt->x,tmpt->y,0,tmpt->alive_img,tmpt->theta);
  }

  for (i=0;i<TREES_NUM;i++)
    Sprite3(dc,trees[i].x,trees[i].y,0,__BIN_2);

  for (i=0;i<TARGETS_NUM;i++) {
    tmpt=&targets[i];
    if (tmpt->dead) {
      for (j=0;j<40;j++) {
	dc->pen_width=4;
	if (j&1)
	  dc->color=ROPF_DITHER|LTGRAY<<16|BLACK;
	else
	  dc->color=ROPF_DITHER|DKGRAY<<16|LTGRAY;
	dx=15*Sin(ts/4+j<<6)+j>>2;
	dy=10*FullTri(ts/3+j/2.0,20)+j>>2;
	GrPlot3(dc,tmpt->x+5+dx+0.5*dy,tmpt->y+0.5*dx+dy,0);
      }
    }
  }

  tmpb=bomb_head.next;
  while (tmpb!=&bomb_head) {
    if (tmpb->t+FALL_TIME<tS) {
      if (Blink(10))
	Sprite3(dc,tmpb->x,tmpb->y,0,__BIN_9);
      else
	Sprite3(dc,tmpb->x,tmpb->y,0,__BIN_10);
    }
    tmpb=tmpb->next;
  }

  dc->flags&=~DCF_TRANSFORMATION;
  Sprite3(dc,task->pix_width>>1,task->pix_height>>1,0,__BIN_1);
  dc->color=RED;
  GrPrint(dc,0,0,"Targets:%02d	KeyStrokes:%04d Best:%04d",
	target_cnt,key_cnt,best_score);
  if (!target_cnt && Blink(4))
    GrPrint(dc,(task->pix_width-FONT_WIDTH*14)>>1,
	  (task->pix_height-FONT_HEIGHT)>>1-32,"Game Completed");
}
 
U0 BombHit(Bomb *tmpb)
{
  I64 i;
  for (i=0;i<TARGETS_NUM;i++) {
    if (!targets[i].dead &&
	  SqrI64(tmpb->x-targets[i].x)+SqrI64(tmpb->y-targets[i].y)<20*20) {
      targets[i].dead=TRUE;
      target_cnt--;
    }
  }
  QueRem(tmpb);
  Free(tmpb);
}
 
U0 BombDrop(F64 x,F64 y)
{
  Bomb *tmpb=MAlloc(sizeof(Bomb));
  tmpb->x=x;
  tmpb->y=y;
  tmpb->t=tS;
  tmpb->exploding=FALSE;
  QueIns(tmpb,bomb_head.last);
  Sweep(FALL_TIME*1000,74,62);
}
 
I64 AnimateTask(CTask *)
{
  F64 last_t=tS,dt;
  I64 i;
  Bomb *tmpb,*tmpb1;
  Target *tmpt;
  while (TRUE) {
    dt=tS-last_t;
    last_t=tS;

    if (bomb_head.next==&bomb_head) {
      if (target_cnt)
	Snd(Freq2Ona(100+60*Clamp(0.1*(1.0+Abs(Wrap(thetaf-theta,-pi)))`4.0,-3,3)));
      else if (key_cnt<best_score) {
	best_score=key_cnt;
	Sleep(150); Snd(74);Sleep(150);Snd;
	Sleep(150); Snd(74);Sleep(150);Snd;
      } else
	Snd;
    }

    theta+=dt*(thetaf-theta);
    x+=dt*v*Sin(theta);
    y+=dt*v*Cos(theta);

    for (i=0;i<TARGETS_NUM;i++) {
      tmpt=&targets[i];
      if (!tmpt->dead && tmpt->type==MDF_TANK) {
	tmpt->x+=dt*TANK_V*Cos(tmpt->theta);
	tmpt->y+=dt*TANK_V*Sin(tmpt->theta);
	if (i&1)
	  tmpt->theta+=dt*pi/16;
	else
	  tmpt->theta-=dt*pi/16;
      }
    }

    tmpb=bomb_head.next;
    while (tmpb!=&bomb_head) {
      tmpb1=tmpb->next;
      if (tmpb->t+FALL_TIME+EXPLODE_TIME<tS)
	BombHit(tmpb);
      else if (tmpb->t+FALL_TIME<tS && !tmpb->exploding) {
	Noise(EXPLODE_TIME*1000,62,74);
	tmpb->exploding=TRUE;
      }
      tmpb=tmpb1;
    }

    Sleep(10);
  }
  return 0;
}
 
U0 Init()
{
  I64 i;
  Target *tmpt;

  v=20;
  x=-MAP_WIDTH>>1;
  y=-MAP_HEIGHT>>1;
  thetaf=theta=1*pi/4;

  QueInit(&bomb_head);

  MemSet(trees,0,sizeof(trees));
  for (i=0;i<TREES_NUM;i++) {
    trees[i].x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    trees[i].y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
  }

  MemSet(targets,0,sizeof(targets));
  for (i=0;i<TARGETS_NUM;i++) {
    tmpt=&targets[i];
    tmpt->x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    tmpt->y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
    if (i<TARGETS_NUM/3) {
      tmpt->type=MDF_BUNKER;
      tmpt->theta=(RandU16&3)*pi/2;
      tmpt->alive_img=__BIN_6;
      tmpt->dead_img1=__BIN_7;
      tmpt->dead_img2=__BIN_8;
    } else {
      tmpt->type=MDF_TANK;
      tmpt->theta=Rand*2*pi;
      tmpt->alive_img=__BIN_3;
      tmpt->dead_img1=__BIN_4;
      tmpt->dead_img2=__BIN_5;
    }
  }
  key_cnt=0;
  target_cnt=TARGETS_NUM;
}

U0 CleanUp()
{
  QueDel(&bomb_head,TRUE);
}

U0 BomberGolf()
{
  I64 sc;
  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Faster(,,SC_CURSOR_UP);"
	"  Slower(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"  Bomb(,CH_SPACE);"
	"}"
	);
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  Fs->animate_task=Spawn(&AnimateTask,Fs,"Animate",,Fs);
  Fs->draw_it=&DrawIt;
  Init;
  try {
    while (TRUE)
      switch (GetKey(&sc)) {
	case 0:
	  switch (sc.u8[0]) {
	    case SC_CURSOR_UP:
	      v+=10;
	      if (v>300) v=300;
	      break;
	    case SC_CURSOR_DOWN:
	      v-=10;
	      if (v<20) v=20;
	      break;
	    case SC_CURSOR_LEFT:
	      key_cnt++;
	      thetaf+=1.0/(Abs(Wrap(thetaf-theta,-pi))+2*pi);
	      break;
	    case SC_CURSOR_RIGHT:
	      key_cnt++;
	      thetaf-=1.0/(Abs(Wrap(thetaf-theta,-pi))+2*pi);
	      break;

	  }
	  break;
	case CH_SPACE:
	  key_cnt++;
	  BombDrop(-x-0.8*FALL_TIME*v*Sin(theta),-y-0.8*FALL_TIME*v*Cos(theta));
	  break;
	case '\n':
	  CleanUp;
	  Init;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto bm_done;
      }
bm_done:
  } catch
    PutExcept;
  SettingsPop;
  CleanUp;
  MenuPop;
  RegWrite("TempleOS/BomberGolf","I64 best_score=%d;\n",best_score);
}
 
BomberGolf;
