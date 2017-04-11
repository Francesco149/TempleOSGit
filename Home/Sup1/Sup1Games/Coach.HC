



  /* <1> <1> (image) */








  /* <2> <2> (image) */ /* <3> <3> (image) */ /* <4> <4> (image) */ /* <5> <5> (image) */ /* <6> <6> (image) */ /* <7> <7> (image) */ /* <8> <8> (image) */ /* <9> <9> (image) */ /* <10> <10> (image) */


#define BASE_X		(GR_WIDTH/2)
#define BASE_Y		(3*GR_HEIGHT/4)
#define TEE_HEIGHT	34

#define BATTER_T	0.25
#define BATTER_IMGS_NUM	4
U8 *batter_imgs[BATTER_IMGS_NUM]={__BIN_2,__BIN_3,__BIN_4,__BIN_5};

#define RUNNER1_T		1.5
#define RUNNER1_IMGS_NUM	6
U8 *runner1_imgs[RUNNER1_IMGS_NUM]={__BIN_5,__BIN_6,__BIN_7,__BIN_7,__BIN_7,__BIN_8};

#define RUNNER2_T		0.4
#define RUNNER2_IMGS_NUM	4
U8 *runner2_imgs[RUNNER2_IMGS_NUM]={__BIN_8,__BIN_9,__BIN_8,__BIN_10};

F64 t0,hit_t0;
F64 ball_x,ball_y;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 tt=tS-t0;
  I64 i=(BATTER_IMGS_NUM-1)*tt/BATTER_T;
  U8 *tmps;

  dc->color=WHITE;
  dc->thick=3;
  GrLine3(dc,BASE_X,BASE_Y,0,BASE_X-200,BASE_Y-200,0);
  GrLine3(dc,BASE_X,BASE_Y,0,BASE_X+200,BASE_Y-200,0);

  Sprite3(dc,BASE_X,BASE_Y,0,__BIN_1);
  dc->color=WHITE;
  GrCircle(dc,ball_x+BASE_X,ball_y+BASE_Y-TEE_HEIGHT,2);
  if (!t0)
    Sprite3(dc,BASE_X-23,BASE_Y,0,batter_imgs[0]);
  else if (i<BATTER_IMGS_NUM-1) {
    tmps=SpriteInterpolate(
	  ((BATTER_IMGS_NUM-1)/BATTER_T)*(tt%(BATTER_T/(BATTER_IMGS_NUM-1))),
	  batter_imgs[i],batter_imgs[i+1]);
    Sprite3(dc,BASE_X-23,BASE_Y,0,tmps);
    Free(tmps);
    if (!hit_t0 && tt>BATTER_T/2)
      hit_t0=tS;
  } else {
    tt-=BATTER_T;
    i=(RUNNER1_IMGS_NUM-1)*tt/RUNNER1_T;
    if (i<RUNNER1_IMGS_NUM-1 ) {
      tmps=SpriteInterpolate(
	    ((RUNNER1_IMGS_NUM-1)/RUNNER1_T)*
	    (tt%(RUNNER1_T/(RUNNER1_IMGS_NUM-1))),
	    runner1_imgs[i],runner1_imgs[i+1]);
      Sprite3(dc,BASE_X-23,BASE_Y,0,tmps);
      Free(tmps);
    } else {
      tt-=RUNNER1_T;
      i=RUNNER2_IMGS_NUM*tt/RUNNER2_T;
      if (i<30) {
	tmps=SpriteInterpolate(
	      (RUNNER2_IMGS_NUM/RUNNER2_T)*(tt%(RUNNER2_T/RUNNER2_IMGS_NUM)),
	      runner2_imgs[i%RUNNER2_IMGS_NUM],
	      runner2_imgs[(i+1)%RUNNER2_IMGS_NUM]);
	Sprite3(dc,BASE_X-23-50*tt,BASE_Y-50*tt,0,tmps);
	Free(tmps);
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
      Noise(25,86,93);
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
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+WHITE;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
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
