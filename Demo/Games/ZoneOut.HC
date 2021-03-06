RegDft("TempleOS/ZoneOut","F64 best_score=9999;\n");
RegExe("TempleOS/ZoneOut");










		/* <1> <1> (image) */
























		/* <2> <2> (image) */













		   /* <3> <3> (image) */

/*  <4> (image) */










#define THEM_NUM	10

class Obj
{
  Obj *next,*last;
  F64 t0,theta;
  I64 x,y,z;
  Bool hit,pad[7];
} us,them[THEM_NUM],shots;
I64 num_them;
F64 t0,tf;

#define SCRN_SCALE	512
#define TANK_HEIGHT	32

U0 ZOTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  I64 zz;
  Mat4x4MulXYZ(dc->r,x,y,z);
  zz=*z;
  if (zz<1) zz=1;
  *x=SCRN_SCALE/2* *x/zz;
  *y=SCRN_SCALE/2* (*y+TANK_HEIGHT)/zz;
  *x+=dc->x;
  *y+=dc->y;
  *z+=dc->z;
}

U0 DrawIt(CTask *task,CDC *dc)
{
  Obj *tmpo;
  I64 i,dd,y,w=task->pix_width,h=task->pix_height,cx=w>>1,cy=h>>1;
  U8 *img;
  F64 tt,theta;

  theta=640*Wrap(2*us.theta)/pi;
  Sprite3(dc,theta-1280,90,0,__BIN_4);
  Sprite3(dc,theta	   ,90,0,__BIN_4);
  Sprite3(dc,theta+1280,90,0,__BIN_4);

  DCDepthBufAlloc(dc);
  dc->transform=&ZOTransform;
  dc->x=cx;
  dc->y=cy;
  Mat4x4TranslationEqu(dc->r,-us.x,-us.y,-us.z);
  Mat4x4RotY(dc->r,us.theta-pi/2);
  Mat4x4RotX(dc->r,pi/16);

  dc->flags|=DCF_TRANSFORMATION;
  for (i=0;i<THEM_NUM;i++) {
    y=them[i].y;
    tmpo=shots.next;
    while (tmpo!=&shots) {
      dd=SqrI64(them[i].x-tmpo->x)+SqrI64(them[i].z-tmpo->z);
      if (dd<SCRN_SCALE/2*SCRN_SCALE/2) {
	y-=Sqrt(dd);
	if (!them[i].hit) {
	  them[i].hit=TRUE;
	  if (!--num_them) {
	    tf=tS;
	    if (tf-t0<best_score)
	      best_score=tf-t0;
	  }
	}
      }
      tmpo=tmpo->next;
    }
    if (them[i].hit)
      img=__BIN_2;
    else
      img=__BIN_1;
    Sprite3YB(dc,them[i].x,y,them[i].z,img,-them[i].theta);
  }
  tmpo=shots.next;
  while (tmpo!=&shots) {
    Sprite3YB(dc,tmpo->x,tmpo->y,tmpo->z,__BIN_3,-tmpo->theta);
    tmpo=tmpo->next;
  }
  dc->flags&=~DCF_TRANSFORMATION;
  dc->color=LTGREEN;
  GrLine(dc,cx-5,cy,cx+5,cy);
  GrLine(dc,cx,cy-5,cx,cy+5);
  if (tf) {
    dc->color=RED;
    if (Blink)
      GrPrint(dc,cx-(FONT_WIDTH*14)/2,cy-FONT_HEIGHT/2,"Game Completed");
    tt=tf;
  } else {
    dc->color=BLACK;
    GrLine(dc,cx-5,cy,cx+5,cy);
    GrLine(dc,cx,cy-5,cx,cy+5);
    tt=tS;
  }
  dc->color=BLACK;
  GrPrint(dc,0,0,"Enemy:%d Time:%3.2f Best:%3.2f",num_them,tt-t0,best_score);
}

U0 Fire()
{
  Obj *tmpo=MAlloc(sizeof(Obj));
  tmpo->x=us.x;
  tmpo->y=TANK_HEIGHT;
  tmpo->z=us.z;
  tmpo->theta=us.theta;
  tmpo->t0=tS;
  QueIns(tmpo,shots.last);
}

U0 MoveUs(F64 theta)
{
  us.x+=0.1*SCRN_SCALE*Cos(theta);
  us.z+=0.1*SCRN_SCALE*Sin(theta);
}

U0 AnimateTask(I64)
{
  I64 i;
  Obj *tmpo,*tmpo1;
  while (TRUE) {
    for (i=0;i<THEM_NUM;i++) {
      them[i].x+=SCRN_SCALE/32*Cos(them[i].theta);
      them[i].z+=SCRN_SCALE/32*Sin(them[i].theta);
      them[i].theta+=Rand/100.0;
    }
    tmpo=shots.next;
    while (tmpo!=&shots) {
      tmpo1=tmpo->next;
      if (tS-tmpo->t0>1.0) {
	QueRem(tmpo);
	Free(tmpo);
      } else {
	tmpo->x+=0.25*SCRN_SCALE*Cos(tmpo->theta);
	tmpo->z+=0.25*SCRN_SCALE*Sin(tmpo->theta);
      }
      tmpo=tmpo1;
    }
    Sleep(20);
  }
}

U0 Init()
{
  I64 i;
  DocClear;
  "$BG,LTCYAN$%h12c",'\n';
  QueInit(&shots);
  MemSet(&us,0,sizeof(us));
  MemSet(them,0,sizeof(them));
  num_them=THEM_NUM;
  for (i=0;i<THEM_NUM;i++) {
    them[i].x=10000*Rand-5000;
    them[i].z=10000*Rand-5000;
    them[i].theta=2*pi*Rand;
    them[i].hit=FALSE;
  }
  tf=0;
  t0=tS;
}

U0 CleanUp()
{
  QueDel(&shots,TRUE);
}

U0 SongTask(I64)
{//Randomly generate (by God :-)
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("5sD4B5D4B5qEsG4B5G4B5eD4GsB5F4B5FeD4A5qFG");
    Play("5sD4B5D4B5qEsG4B5G4B5eD4GsB5F4B5FeD4A5qFG");
    Play("5eGDsFGFGqDE4eB5E4sG5D4G5DqF4sGAGA");
    Play("5eGDsFGFGqDE4eB5E4sG5D4G5DqF4sGAGA");
  }
}

U0 ZoneOut()
{
  I64 sc;

  PopUpOk(
	"I refuse to rip-off the original\n"
	"so this is intentionally crappy\n"
	"and included for demonstration\n"
	"purposes.\n\n"
	"Write games, don't play them.\n");

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Fwd(,,SC_CURSOR_UP);"
	"  Bwd(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"  Fire(,CH_SPACE);"
	"}"
	);

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=YELLOW<<4+WHITE;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (GetKey(&sc)) {
	case CH_SPACE:
	  Fire;
	  break;
	case '\n':
	  CleanUp;
	  Init;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto zo_done;
	case 0:
	  switch (sc.u8[0]) {
	    case SC_CURSOR_RIGHT:
	      us.theta-=pi/256;
	      break;
	    case SC_CURSOR_LEFT:
	      us.theta+=pi/256;
	      break;
	    case SC_CURSOR_UP:
	      MoveUs(us.theta);
	      break;
	    case SC_CURSOR_DOWN:
	      MoveUs(us.theta+pi);
	      break;
	  }
	  break;
      }
    }
zo_done:
  } catch
    PutExcept;
  CleanUp;
  DocClear;
  SettingsPop;
  MenuPop;
  RegWrite("TempleOS/ZoneOut","F64 best_score=%5.4f;\n",best_score);
}

ZoneOut;
