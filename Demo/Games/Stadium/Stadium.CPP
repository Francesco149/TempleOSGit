/*This shows how you can make a
background very easily.
*/


/* <1> <1> (image) */

/* <2> <2> (image) */

CDC *background;

#define BALL_TIME	0.2
#define MAX_FANS 10
I64 x[MAX_FANS],y[MAX_FANS];
Bool hit[MAX_FANS];
F64 theta[MAX_FANS],ball_t;
I64 target_x,target_y,pitcher_x,pitcher_y;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,*r;
  F64 xx,yy,t0;
  background->flags|=DCF_NO_TRANSPARENTS;
  GrBlot(dc,0,0,background);
  for (i=0;i<MAX_FANS;i++)
    SpriteZ3B(dc,x[i],y[i],0,__BIN_1,theta[i]);
  if (ball_t) {
    t0=(tS-ball_t)/BALL_TIME;
    if (t0>1.0)
      ball_t=0;
    else {
      xx=t0*target_x+(1.0-t0)*pitcher_x;
      yy=t0*target_y+(1.0-t0)*pitcher_y;
      xx/=1.5-t0;
      yy/=1.5-t0;
      r=GrScaleNew(dc->r,1.5-t0,dc->mem_task);
      Free(dc->r);
      GrSetRotMat(dc,r);
      dc->flags|=DCF_TRANSFORMATION;
      SpriteZ3B(dc,xx,yy,0,__BIN_2,t0);
    }
  }
  dc->color=RED;
  GrPrint(dc,FONT_WIDTH,FONT_HEIGHT,"Peg the Fans");
}

U0 AnimateTask(I64)
{
  I64 i;
  F64 xx,yy,t0;
  while (TRUE) {
    if (ball_t) {
      t0=(tS-ball_t)/BALL_TIME;
      xx=t0*target_x+(1.0-t0)*pitcher_x;
      yy=t0*target_y+(1.0-t0)*pitcher_y;
    }
    for (i=0;i<MAX_FANS;i++) {
      if (ball_t)
	if (Sqr(x[i]-xx)+Sqr(y[i]-yy)<200) {
	  hit[i]=TRUE;
	  theta[i]=-pi/2;
	}
      if (!hit[i]) {
	x[i]+=SignI64(RandI16);
	y[i]+=SignI64(RandI16);
	theta[i]+=Sign(RandI16)/25.0;
	if (!(0<=x[i]<GR_WIDTH)) x[i]=GR_WIDTH/2;
	if (!(10<=y[i]<100)) y[i]=50;
	if (!(-0.75<=theta[i]<0.75)) theta[i]=0;
      }
    }
    Sleep(10);
  }
}

U0 Init()
{
  I64 i;
  for (i=0;i<MAX_FANS;i++) {
    x[i]=RandU16%GR_WIDTH;
    y[i]=50;
    theta[i]=0;
    hit[i]=FALSE;
  }
}

U0 Stadium()
{
  I64 msg_code,a1,a2,ch=0;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Cd(__DIR__);
  Fs->win_inhibit|=WIG_DBL_CLICK;

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,'\n');"
"}"
);
  WinMax;
  WinBorder;
  AutoComplete;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);

  background=GRARead("StadiumBG");

  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  do {
    msg_code=GetMsg(&a1,&a2,1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN);
    if (msg_code==MSG_KEY_DOWN) {
      ch=a1;
      if (ch=='\n')
	Init;
    } else {
      target_x=a1; target_y=a2;
      pitcher_x=GR_WIDTH/2; pitcher_y=GR_HEIGHT;
      ball_t=tS;
    }
  } while (ch!=CH_SHIFT_ESC && ch!=CH_ESC);
  GetMsg(,,1<<MSG_KEY_UP);

  DCDel(background);
  SettingsPop;
  MenuPop;
}

Stadium;
