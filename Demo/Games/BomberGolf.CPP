RegSetDftEntry("TempleOS/BomberGolf","I64 best_score=99999;\n");
RegExeBranch("TempleOS/BomberGolf");



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

#define NUM_TREES	64
class Tree
{
  I64 x,y;
} trees[NUM_TREES];
 

I64 target_cnt,key_cnt;


#define NUM_TARGETS	10

#define TY_TANK		0
#define TY_BUNKER	1

#define TANK_V		10.0

class Target
{
  F64	x,y,theta;
  CSprite *alive_img,*dead_img1,*dead_img2;
  U8	type;
  Bool	dead,pad[6];
} targets[NUM_TARGETS];

#define FALL_TIME	3.00
#define EXPLODE_TIME	0.25
class Bomb
{
  Bomb *next,*last;
  F64 x,y,t;
  Bool exploding;
} bomb_root;
 
F64 v,x,y,
    theta,thetaf; //thetaf is the final theta. theta is gradually changed until it reaches thetaf.

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,j;
  Bomb *tempb;
  Target *tempt;
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
  for (i=0;i<NUM_TARGETS;i++) {
    tempt=&targets[i];
    if (tempt->dead) {
      if (i&1) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	DCSymmetry3Set(dc,tempt->x,tempt->y,0,tempt->x,tempt->y,1,
	      tempt->x+1024*Cos(tempt->theta),tempt->y+1024*Sin(tempt->theta),0);
      }
      if (Blink(15))
	SpriteZ3B(dc,tempt->x,tempt->y,0,tempt->dead_img1,tempt->theta);
      else
	SpriteZ3B(dc,tempt->x,tempt->y,0,tempt->dead_img2,tempt->theta);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    } else
      SpriteZ3B(dc,tempt->x,tempt->y,0,tempt->alive_img,tempt->theta);
  }

  for (i=0;i<NUM_TREES;i++)
    Sprite3(dc,trees[i].x,trees[i].y,0,__BIN_2);

  for (i=0;i<NUM_TARGETS;i++) {
    tempt=&targets[i];
    if (tempt->dead) {
      for (j=0;j<40;j++) {
	dc->pen_width=4;
	if (j&1)
	  dc->color=ROPF_DITHER|LTGRAY<<16|BLACK;
	else
	  dc->color=ROPF_DITHER|DKGRAY<<16|LTGRAY;
	dx=15*Sin(ts/4+j<<6)+j>>2;
	dy=10*FullTri(ts/3+j/2.0,20)+j>>2;
	GrPlot3(dc,tempt->x+5+dx+0.5*dy,tempt->y+0.5*dx+dy,0);
      }
    }
  }

  tempb=bomb_root.next;
  while (tempb!=&bomb_root) {
    if (tempb->t+FALL_TIME<tS) {
      if (Blink(10))
	Sprite3(dc,tempb->x,tempb->y,0,__BIN_9);
      else
	Sprite3(dc,tempb->x,tempb->y,0,__BIN_10);
    }
    tempb=tempb->next;
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
 
U0 BombHit(Bomb *tempb)
{
  I64 i;
  for (i=0;i<NUM_TARGETS;i++) {
    if (!targets[i].dead &&
	  SqrI64(tempb->x-targets[i].x)+SqrI64(tempb->y-targets[i].y)<20*20) {
      targets[i].dead=TRUE;
      target_cnt--;
    }
  }
  QueRem(tempb);
  Free(tempb);
}
 
U0 BombDrop(F64 x,F64 y)
{
  I64 i;
  Bomb *tempb=MAlloc(sizeof(Bomb));
  tempb->x=x;
  tempb->y=y;
  tempb->t=tS;
  tempb->exploding=FALSE;
  QueIns(tempb,bomb_root.last);
  Sweep(FALL_TIME*1000,1000,500);
}
 
I64 AnimateTask(CTask *parent)
{//Starts with preempt OFF
  I64 i,w=parent->pix_width,h=parent->pix_height;
  Bomb *tempb,*tempb1;
  Target *tempt;
  while (TRUE) {
    if (bomb_root.next==&bomb_root) {
      if (target_cnt)
	Snd(100+60*Clamp(0.1*(1.0+Abs(Wrap(thetaf-theta,-pi)))`4.0,-3,3));
      else if (key_cnt<best_score) {
	best_score=key_cnt;
	Sleep(150); Snd(1000);Sleep(150);Snd(0);
	Sleep(150); Snd(1000);Sleep(150);Snd(0);
      } else
	Snd(0);
    }

    theta+=0.01*(thetaf-theta);
    x+=0.01*v*Sin(theta);
    y+=0.01*v*Cos(theta);

    for (i=0;i<NUM_TARGETS;i++) {
      tempt=&targets[i];
      if (!tempt->dead && tempt->type==TY_TANK) {
	tempt->x+=0.01*TANK_V*Cos(tempt->theta);
	tempt->y+=0.01*TANK_V*Sin(tempt->theta);
	if (i&1)
	  tempt->theta+=0.01*pi/16;
	else
	  tempt->theta-=0.01*pi/16;
      }
    }

    tempb=bomb_root.next;
    while (tempb!=&bomb_root) {
      tempb1=tempb->next;
      if (tempb->t+FALL_TIME+EXPLODE_TIME<tS)
	BombHit(tempb);
      else if (tempb->t+FALL_TIME<tS && !tempb->exploding) {
	Noise(EXPLODE_TIME*1000,500,1000);
	tempb->exploding=TRUE;
      }
      tempb=tempb1;
    }

    Sleep(10);
  }
  return 0;
}
 
U0 Init()
{
  I64 i;
  Target *tempt;

  v=20;
  x=-MAP_WIDTH>>1;
  y=-MAP_HEIGHT>>1;
  thetaf=theta=1*pi/4;

  QueInit(&bomb_root);

  MemSet(trees,0,sizeof(trees));
  for (i=0;i<NUM_TREES;i++) {
    trees[i].x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    trees[i].y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
  }

  MemSet(targets,0,sizeof(targets));
  for (i=0;i<NUM_TARGETS;i++) {
    tempt=&targets[i];
    tempt->x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    tempt->y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
    if (i<NUM_TARGETS/3) {
      tempt->type=TY_BUNKER;
      tempt->theta=(RandU16&3)*pi/2;
      tempt->alive_img=__BIN_6;
      tempt->dead_img1=__BIN_7;
      tempt->dead_img2=__BIN_8;
    } else {
      tempt->type=TY_TANK;
      tempt->theta=Rand*2*pi;
      tempt->alive_img=__BIN_3;
      tempt->dead_img1=__BIN_4;
      tempt->dead_img2=__BIN_5;
    }
  }
  key_cnt=0;
  target_cnt=NUM_TARGETS;
}

U0 CleanUp()
{
  QueDel(&bomb_root,TRUE);
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
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  AutoComplete;
  WinMax;
  Preempt;
  Fs->animate_task=Spawn(&AnimateTask,Fs,"Animate",,Fs);
  DocCursor;
  DocClear;
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
  CleanUp;
  SettingsPop;
  MenuPop;
  RegWriteBranch("TempleOS/BomberGolf","I64 best_score=%d;\n",best_score);
}
 
BomberGolf;
