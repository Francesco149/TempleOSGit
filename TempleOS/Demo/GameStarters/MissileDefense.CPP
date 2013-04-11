//This uses [C:/TempleOS/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.




  /* <1> <1> (image) */







   /* <2> <2> (image) */
 









#define MAX_SHOTS	64
#define ROUND_AMMO	100
#define EXPLOSION_CNT	50
#define SKY_LINES	47

CDC *dc2;
I64 ammo;

class Shot
{
  I64 x,y,dx,dy,cnt;
} s[MAX_SHOTS];


#define NUM_MISSILES	32
class Missile
{
  I64 x,y,dx,dy,dead;
} m[NUM_MISSILES];


U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,x,radius,site;

  dc->color=BLACK;
  SpritePlot3(dc,	    200,GR_HEIGHT-100,0,__BIN_1);
  SpritePlot3(dc,GR_WIDTH-200,GR_HEIGHT-100,0,__BIN_1);
  for (site=0;site<3;site++) {
      x=100+site*((GR_WIDTH-200)/2);
      SpritePlot3(dc,x,GR_HEIGHT-100,0,__BIN_2);
  }

  for (i=0;i<MAX_SHOTS;i++)
    if (s[i].cnt>0) {
      dc->color=LTGRAY;
      GrPlot(dc,s[i].x.i32[1],s[i].y.i32[1]);
    } else if (s[i].cnt>-EXPLOSION_CNT) {
      dc->color=win_updates&15;
      radius=(EXPLOSION_CNT/2-AbsI64(-EXPLOSION_CNT/2-s[i].cnt))*2;
      dc->pen_width=radius;
      GrPlot3(dc,s[i].x.i32[1],s[i].y.i32[1],0);
    }
  for (i=0;i<NUM_MISSILES;i++)
    if (!m[i].dead) {
      if (win_updates&1)
	dc2->color=BLACK;
      else
	dc2->color=LTGRAY;
      GrPlot(dc2,m[i].x.i32[1],m[i].y.i32[1]);
      dc->pen_width=3;
      dc->color=WHITE;
      GrPlot3(dc,m[i].x.i32[1],m[i].y.i32[1],0);
    }

  dc->color=WHITE;
  GrPrintF(dc,0,0,"Ammo Left:%d",ammo);
}

U0 AnimateTask(I64)
{
  I64 i,j,radius;
  while (TRUE) {
    for (i=0;i<MAX_SHOTS;i++)
      if (s[i].cnt>=-EXPLOSION_CNT) {
	s[i].x+=s[i].dx;
	s[i].y+=s[i].dy;
	if (!(0<=s[i].x.i32[1]<GR_WIDTH) ||
	    !(0<=s[i].y.i32[1]<GR_HEIGHT))
	  s[i].cnt=MIN_I64;
	else
	  s[i].cnt--;
	if (!s[i].cnt)
	  Noise(200,100,200);
      }
    Snd(0);
    for (i=0;i<NUM_MISSILES;i++)
      if (!m[i].dead) {
	m[i].x+=m[i].dx;
	if (!(0<=m[i].x.i32[1]<GR_WIDTH))
	  m[i].dead=TRUE;
	else {
	  m[i].y+=m[i].dy;
	  if (m[i].y.i32[1]>SKY_LINES*FONT_HEIGHT) {
	    Noise(1000,500,2000);
	    m[i].dead=TRUE;
	  } else {
	    m[i].dy+=MAX_I32>>15;
	    for (j=0;j<MAX_SHOTS;j++)
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
  for (i=0;i<MAX_SHOTS;i++)
    if (s[i].cnt<-EXPLOSION_CNT) {
      s[i].x=(100+site*((GR_WIDTH-200)/2))<<32;
      s[i].y=(GR_HEIGHT-100)<<32;
//([C:/TempleOS/Kernel/KGlbls.CPP.Z,82] ipx, [C:/TempleOS/Kernel/KGlbls.CPP.Z,82] ipy) is the input pointer -- mouse --location.
//[C:/TempleOS/Kernel/KGlbls.CPP.Z,91] ip_lb,[C:/TempleOS/Kernel/KGlbls.CPP.Z,92] ip_rb is the button state.
      x=ipx-Fs->win_pixel_left-Fs->win_scroll_x;
      y=ipy-Fs->win_pixel_top-Fs->win_scroll_y;
      d=Sqrt(SqrI64(s[i].x.i32[1]-x)+SqrI64(s[i].y.i32[1]-y));
      s[i].dx=ToF64(x<<32-s[i].x)/d;
      s[i].dy=ToF64(y<<32-s[i].y)/d;
      s[i].cnt=d;
      Noise(50,1000,2000);
      ammo--;
      break;
    }
}

U0 Init()
{
  I64 i;
  ammo=ROUND_AMMO;
  for (i=0;i<MAX_SHOTS;i++)
    s[i].cnt=MIN_I64;
  for (i=0;i<NUM_MISSILES;i++) {
    m[i].x=(GR_WIDTH*RandU16/MAX_U16)<<32;
    m[i].y=-(GR_HEIGHT*RandU16/MAX_U16)<<32;
    m[i].dx=(RandU32/2)>>5;
    m[i].dy=(RandU32*2+MAX_U32/2)>>5;
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

  "$CL$$BG,BLUE$";
  NewLine(SKY_LINES);
  "$BG,BROWN$";

  dc2=DCAlias;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Init;
  WinBorder;
  WordStat;
  Fs->draw_it=&DrawIt;
  DocCursor;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);

  try {
    while (TRUE) {
      switch (ch=GetChar(NULL,FALSE)) {
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
    CatchAll;
  SettingsPop;
  MenuPop;
  DCFill(dc2);
  DCDel(dc2);
  "$CL$$BG$\n";
}

MissileDefense;
