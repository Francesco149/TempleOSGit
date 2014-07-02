














/* Mountain <1> (image) */ //Used in [C:/Demo/Graphics/Camp.CPP.Z,1] Camp, [C:/Apps/AfterEgypt/Quail.CPP.Z,1] Quail, [C:/Apps/AfterEgypt/GodTalking.CPP.Z,1] GodTalking, [C:/Demo/MultiCore/Clouds.CPP.Z,1] Clouds, and [C:/Demo/Games/Squirt.CPP.Z,1] Squirt.





/* <2> <2> (image) */	 /* <3> <3> (image) */	  /* <4> <4> (image) */





/* <5> <5> (image) */	 /* <6> <6> (image) */	  /* <7> <7> (image) */

CSprite *right_imgs[4]={__BIN_2,__BIN_3,__BIN_4,__BIN_3},
	*left_imgs [4]={__BIN_5,__BIN_6,__BIN_5,__BIN_7};
I64 x=0,y=0;

U0 DrawMountain(CTask *task,CDC *dc)
{
  static I64 last_x;
  static Bool last_left;
  I64 cx=task->pix_width/2,cy=task->pix_height/2;
  dc->color=BROWN;
  dc->pen_width=2;

  Sprite3(dc,0,117,0,__BIN_1);
  GrLine3(dc,cx,cy,0,cx+100,cy-20,0);
  GrLine3(dc,cx+100,cy-20,0,cx-90,cy-40,0);
  GrLine3(dc,cx-90,cy-40,0,cx+70,cy-60,0);
  GrLine3(dc,cx+70,cy-60,0,cx-70,cy-60,0);
  GrLine3(dc,cx-70,cy-60,0,cx+60,cy-80,0);
  GrLine3(dc,cx+60,cy-80,0,cx-50,cy-100,0);
  GrLine3(dc,cx-50,cy-100,0,cx+30,cy-120,0);
  GrLine3(dc,cx+30,cy-120,0,cx-15,cy-140,0);

  dc->color=BROWN;
  GrCircle(dc,50,25,15);
  dc->color=YELLOW;
  GrFloodFill(dc,50,25);

  if (Blink) {
    dc->color=BLACK;
    GrPrint(dc,cx-50,80,"Mt. Horeb");
  }

  if (x<last_x)
    last_left=TRUE;
  else if (x>last_x)
    last_left=FALSE;
  dc->pen_width=1;
  if (last_left)
    Sprite3(dc,x+cx,y+cy,0,left_imgs [ToI64(6.0*tS)&3]);
  else
    Sprite3(dc,x+cx,y+cy,0,right_imgs[ToI64(6.0*tS)&3]);
  last_x=x;
}

//This is the path of the man.
CD3I32 mountain[17]={{0,0,0},{100,-20,0},{100,-20,0},{-100,-40,0},{-100,-40,0},
		  {80,-60,0},{80,-60,0},{-80,-60,0},{-80,-60,0},
		  {60,-80,0},{60,-80,0},{-60,-100,0},{-60,-100,0},{40,-120,0},
		  {40,-120,0},{-37,-140,0},{-37,-140,0}};

Bool PlotMan(U8 *,I64 _x,I64 _y,I64)
{
  x=_x; y=_y;
  Sleep(10);
  return !ToBool(ScanChar);
}

U0 Mountain()
{
  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  try {
    Fs->text_attr=YELLOW<<4+BLUE;
    WordStat;
    WinMax;
    WinBorder;
    DocCursor;
    DocClear;
    Fs->draw_it=&DrawMountain;
    BSpline2(NULL,&mountain,17,&PlotMan);  //See [Graphics/Math,1] Graphics/Math.
  } catch
    PutExcept;
  SettingsPop(Fs,TSF_SAME_SONG);
}

//This is so I can use this file stand alone
//or as a component of a program.
#if IncludeDepth<2
Mountain;
#endif
