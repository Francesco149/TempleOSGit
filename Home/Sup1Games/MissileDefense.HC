


  /* <1> <1> (image) */







   /* <2> <2> (image) */
 









#define SHOTS_NUM	64
#define ROUND_AMMO	100
#define EXPLOSION_CNT	50
#define SKY_LINES	47

CDC *dc2;
I64 ammo;

class Shot
{
  I64 x,y,dx,dy,cnt;
} s[SHOTS_NUM];


#define MISSILES_NUM	32
class Missile
{
  I64 x,y,dx,dy,dead;
} m[MISSILES_NUM];


U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,x,radius,site;

  dc->color=BLACK;
  Sprite3(dc,	    200,GR_HEIGHT-100,0,__BIN_1);
  Sprite3(dc,GR_WIDTH-200,GR_HEIGHT-100,0,__BIN_1);
  for (site=0;site<3;site++) {
    x=100+site*((GR_WIDTH-200)/2);
    Sprite3(dc,x,GR_HEIGHT-100,0,__BIN_2);
  }

  for (i=0;i<SHOTS_NUM;i++)
    if (s[i].cnt>0) {
      dc->color=LTGRAY;
      GrPlot(dc,s[i].x.i32[1],s[i].y.i32[1]);
    } else if (s[i].cnt>-EXPLOSION_CNT) {
      dc->color=winmgr.updates&15;
      radius=(EXPLOSION_CNT/2-AbsI64(-EXPLOSION_CNT/2-s[i].cnt))*2;
      dc->thick=radius;
      GrPlot3(dc,s[i].x.i32[1],s[i].y.i32[1],0);
    }
  for (i=0;i<MISSILES_NUM;i++)
    if (!m[i].dead) {
      if (winmgr.updates&1)
	dc2->color=BLACK;
      else
	dc2->color=LTGRAY;
      GrPlot(dc2,m[i].x.i32[1],m[i].y.i32[1]);
      dc->thick=3;
      dc->color=WHITE;
      GrPlot3(dc,m[i].x.i32[1],m[i].y.i32[1],0);
    }

  dc->color=WHITE;
  GrPrint(dc,0,0,"Ammo Left:%d",ammo);
}

U0 AnimateTask(I64)
{
  I64 i,j,radius;
  while (TRUE) {
    for (i=0;i<SHOTS_NUM;i++)
      if (s[i].cnt>=-EXPLOSION_CNT) {
	s[i].x+=s[i].dx;
	s[i].y+=s[i].dy;
	if (!(0<=s[i].x.i32[1]<GR_WIDTH) ||
	      !(0<=s[i].y.i32[1]<GR_HEIGHT))
	  s[i].cnt=I64_MIN;
	else
	  s[i].cnt--;
	if (!s[i].cnt)
	  Noise(200,34,46);
      }
    Snd;
    for (i=0;i<MISSILES_NUM;i++)
      if (!m[i].dead) {
	m[i].x+=m[i].dx;
	if (!(0<=m[i].x.i32[1]<GR_WIDTH))
	  m[i].dead=TRUE;
	else {
	  m[i].y+=m[i].dy;
	  if (m[i].y.i32[1]>SKY_LINES*FONT_HEIGHT) {
	    Noise(1000,62,86);
	    m[i].dead=TRUE;
	  } else {
	    m[i].dy+=I32_MAX>>15;
	    for (j=0;j<SHOTS_NUM;j++)
	      if (-EXPLOSION_CNT<=s[j].cnt<0) {
		radius=(EXPLOSION_CNT/2-AbsI64(-EXPLOSION_CNT/2-s[j].cnt));
		if (SqrI64(m[i].x.i32[1]-s[j].x.i32[1])+
		      SqrI64(m[i].y.i32[1]-s[j].y.i32[1])<=SqrI64(radius))
		  m[i].dead=TRUE;
	      }
	  }
	}
      }
    Sleep(5);
  }
}

U0 FireShot(I64 site)
{
  I64 i,x,y;
  F64 d;
  if (!ammo) return;
  for (i=0;i<SHOTS_NUM;i++)
    if (s[i].cnt<-EXPLOSION_CNT) {
      s[i].x=(100+site*((GR_WIDTH-200)/2))<<32;
      s[i].y=(GR_HEIGHT-100)<<32;
      x=ms.pos.x-Fs->pix_left-Fs->scroll_x;
      y=ms.pos.y-Fs->pix_top-Fs->scroll_y;
      d=Sqrt(SqrI64(s[i].x.i32[1]-x)+SqrI64(s[i].y.i32[1]-y));
      s[i].dx=ToF64(x<<32-s[i].x)/d;
      s[i].dy=ToF64(y<<32-s[i].y)/d;
      s[i].cnt=d;
      Noise(50,74,86);
      ammo--;
      break;
    }
}

U0 Init()
{
  I64 i;
  ammo=ROUND_AMMO;
  for (i=0;i<SHOTS_NUM;i++)
    s[i].cnt=I64_MIN;
  for (i=0;i<MISSILES_NUM;i++) {
    m[i].x=(GR_WIDTH*RandU16/U16_MAX)<<32;
    m[i].y=-(GR_HEIGHT*RandU16/U16_MAX)<<32;
    m[i].dx=(RandU32/2)>>5;
    m[i].dy=(RandU32*2+U32_MAX/2)>>5;
    m[i].dead=FALSE;
  }
  DCFill(dc2);
}

U0 MissileDefense()
{
  I64 ch;

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
	"  FireLeft(,'1');"
	"  FireCenter(,'2');"
	"  FireRight(,'3');"
	"}"
	);
  PopUpOk("Press '1', '2', or '3' to launch.");

  dc2=DCAlias;

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=BROWN<<4;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  "$BG,BLUE$%h*c",SKY_LINES,'\n';
  Init;
  Fs->draw_it=&DrawIt;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);

  try {
    while (TRUE) {
      switch (ch=GetChar(,FALSE)) {
	case '1'...'3':
	  FireShot(ch-'1');
	  break;
	case '\n':
	  Init;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto md_done;
      }
    }
md_done:
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
  DCFill(dc2);
  DCDel(dc2);
  "$CL$$BG$\n";
}

MissileDefense;





// /* MisDef <3> (image) */
