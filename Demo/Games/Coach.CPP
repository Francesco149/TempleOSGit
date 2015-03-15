



  /* <1> <1> (image) */








  /* <2> <2> (image) */ /* <3> <3> (image) */ /* <4> <4> (image) */ /* <5> <5> (image) */ /* <6> <6> (image) */ /* <7> <7> (image) */ /* <8> <8> (image) */ /* <9> <9> (image) */ /* <10> <10> (image) */


#define BASE_X		(GR_WIDTH/2)
#define BASE_Y		(3*GR_HEIGHT/4)
#define TEE_HEIGHT	34

#define BATTER_T	0.25
#define NUM_BATTER_IMGS	4
CSprite *batter_imgs[NUM_BATTER_IMGS]={__BIN_2,__BIN_3,__BIN_4,__BIN_5};

#define RUNNER1_T		1.5
#define NUM_RUNNER1_IMGS	6
CSprite *runner1_imgs[NUM_RUNNER1_IMGS]={__BIN_5,__BIN_6,__BIN_7,__BIN_7,__BIN_7,__BIN_8};

#define RUNNER2_T		0.4
#define NUM_RUNNER2_IMGS	4
CSprite *runner2_imgs[NUM_RUNNER2_IMGS]={__BIN_8,__BIN_9,__BIN_8,__BIN_10};

F64 t0,hit_t0;
F64 ball_x,ball_y;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 tt=tS-t0;
  I64 i=(NUM_BATTER_IMGS-1)*tt/BATTER_T;
  CSprite *temps;

  dc->color=WHITE;
  dc->pen_width=3;
  GrLine3(dc,BASE_X,BASE_Y,0,BASE_X-200,BASE_Y-200,0);
  GrLine3(dc,BASE_X,BASE_Y,0,BASE_X+200,BASE_Y-200,0);

  Sprite3(dc,BASE_X,BASE_Y,0,__BIN_1);
  dc->color=WHITE;
  GrCircle(dc,ball_x+BASE_X,ball_y+BASE_Y-TEE_HEIGHT,2);
  if (!t0)
    Sprite3(dc,BASE_X-23,BASE_Y,0,batter_imgs[0]);
  else if (i<NUM_BATTER_IMGS-1) {
    temps=SpriteInterpolate(batter_imgs[i],batter_imgs[i+1],
      ((NUM_BATTER_IMGS-1)/BATTER_T)*(tt%(BATTER_T/(NUM_BATTER_IMGS-1))));
    Sprite3(dc,BASE_X-23,BASE_Y,0,temps);
    Free(temps);
    if (!hit_t0 && tt>BATTER_T/2)
      hit_t0=tS;
  } else {
    tt-=BATTER_T;
    i=(NUM_RUNNER1_IMGS-1)*tt/RUNNER1_T;
    if (i<NUM_RUNNER1_IMGS-1 ) {
      temps=SpriteInterpolate(runner1_imgs[i],runner1_imgs[i+1],
	((NUM_RUNNER1_IMGS-1)/RUNNER1_T)*(tt%(RUNNER1_T/(NUM_RUNNER1_IMGS-1))));
      Sprite3(dc,BASE_X-23,BASE_Y,0,temps);
      Free(temps);
    } else {
      tt-=RUNNER1_T;
      i=NUM_RUNNER2_IMGS*tt/RUNNER2_T;
      if (i<30) {
	temps=SpriteInterpolate(runner2_imgs[i%NUM_RUNNER2_IMGS],runner2_imgs[(i+1)%NUM_RUNNER2_IMGS],
	  (NUM_RUNNER2_IMGS/RUNNER2_T)*(tt%(RUNNER2_T/NUM_RUNNER2_IMGS)));
	Sprite3(dc,BASE_X-23-50*tt,BASE_Y-50*tt,0,temps);
	Free(temps);
      } else {
	t0=0;
	ball_x=0;
	ball_y=0;
      }
    }
  }
}

#define BALL_PATH_PTS	7
CD3I32 ball_path[BALL_PATH_PTS]={
  {0,0,0},{20,-40,0},{40,20,0},
  {40,20,0},{50,-30,0},{55,10,0},
  {55,10,0}
};

Bool PlotBall(U8 *,I64 _x,I64 _y,I64)
{
  ball_x=_x; ball_y=_y;
  Sleep(3);
  return TRUE;
}

U0 AnimateTask(I64)
{
  while (TRUE) {
    if (hit_t0) {
      Noise(25,2000,3000);
      BSpline2(NULL,&ball_path,BALL_PATH_PTS,&PlotBall);
      hit_t0=0;
    }
    Sleep(1);
  }
}

U0 Init()
{
  t0=0;
  hit_t0=0;
  ball_x=0;
  ball_y=0;
}

U0 TeeBall()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+WHITE;
  WinMax;
  WinBorder;
  Preempt;
  AutoComplete;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Swing(,CH_SPACE);"
	"  Restart(,'\n');"
	"}"
	);

  Init;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      switch (GetChar(,FALSE)) {
	case '\n':
	  Init;
	  break;
	case CH_SPACE:
	  t0=tS;
	  break;
	case CH_SHIFT_ESC:
	case CH_ESC:
	  goto tb_done;
      }
    }
tb_done:
    } catch
    PutExcept;
    SettingsPop;
    MenuPop;
  }

TeeBall;
