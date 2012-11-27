AcctRegSetDftEntry("SparrowOS/Bomber","I64 best_score=99999;\n");
AcctRegExecuteBranch("SparrowOS/Bomber");


 
 

/* <1> <1> (image) */

 






/* <2> <2> (image) */





/* <3> <3> (image) */





/* <4> <4> (image) */




/* <5> <5> (image) */



/* <6> <6> (image) */



/* <7> <7> (image) */



#define MAP_WIDTH	600
#define MAP_HEIGHT	600

#define NUM_TREES	64
class Tree
{
  I64 x,y;
} tr[NUM_TREES];
 

I64 tank_cnt,key_cnt;

#define NUM_TANKS	10
class Tank
{
  F64 x,y,theta;
  Bool dead,pad[7];
} tk[NUM_TANKS];
 
F64 v,x,y,theta,thetaf,bt,bx,by;
 
U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i;
  for (i=0;i<NUM_TANKS;i++)
    if (tk[i].dead) {
      if (i&1) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	GrSetSymmetry(dc,tk[i].x,tk[i].y,tk[i].x+1024*Cos(tk[i].theta),tk[i].y-1024*Sin(tk[i].theta));
      }
      if (Blink(15,tP(task)))
	CSpritePlotRotZ3b(dc,tk[i].x,tk[i].y,0,__BIN_4,tk[i].theta);
      else
	CSpritePlotRotZ3b(dc,tk[i].x,tk[i].y,0,__BIN_5,tk[i].theta);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    } else
      CSpritePlotRotZ3b(dc,tk[i].x,tk[i].y,0,__BIN_3,tk[i].theta);
  for (i=0;i<NUM_TREES;i++)
    CSpritePlot3(dc,tr[i].x,tr[i].y,0,__BIN_2);
  if (bt) {
    if (bt+.25<tP(task))
      bt=0;
    else {
      if (Blink(10,tP(task)))
	CSpritePlot3(dc,bx,by,0,__BIN_6);
      else
	CSpritePlot3(dc,bx,by,0,__BIN_7);
    }
  }
  CSpritePlotRotZ3b(dc,x,y,0,__BIN_1,theta);
  dc->color=RED;
  GrPrintF(dc,-task->win_scroll_x,-task->win_scroll_y,
    "Tanks:%02d  KeyStrokes:%04d Best:%04d",tank_cnt,key_cnt,best_score);
  if (!tank_cnt && Blink(4,tP(task)))
    GrPrintF(dc,(task->win_pixel_width-FONT_WIDTH*9)>>1-task->win_scroll_x,
		  (task->win_pixel_height-FONT_HEIGHT)>>1-task->win_scroll_y,"Game Over");
}
 
U0 BombDrop()
{
  I64 i;
  bx=x;
  by=y;
  bt=tP(Fs);
  Noise(200,500,1000);
  for (i=0;i<NUM_TANKS;i++) {
    if (!tk[i].dead) {
      if (SqrI64(bx-tk[i].x)+SqrI64(by-tk[i].y)<20*20) {
	tk[i].dead=TRUE;
	if (!--tank_cnt && key_cnt<best_score) {
	   best_score=key_cnt;
	   Sleep(300); //Wait for end of Noise() above
	   Snd(1000);Sleep(150);Snd(0);
	   Sleep(150);
	   Snd(1000);Sleep(150);Snd(0);
	}
      }
    }
  }
}
 
I64 AnimateTask(CTask *parent)
{ //Starts with preempt OFF
  I64 w=parent->win_pixel_width,
      h=parent->win_pixel_height;
  while (TRUE) {
    if (tank_cnt)
      Snd(60+0.001*(1.0+Abs(Wrap(thetaf-theta,-pi)))`8.0);
    if (x+parent->win_scroll_x<0.25*w)
      parent->win_scroll_x+=3;
    if (x+parent->win_scroll_x>0.75*w)
      parent->win_scroll_x-=3;
    if (y+parent->win_scroll_y<0.25*h)
      parent->win_scroll_y+=3;
    if (y+parent->win_scroll_y>0.75*h)
      parent->win_scroll_y-=3;
    theta+=.01*(thetaf-theta);
    x+=v*Cos(theta);
    y+=v*Sin(theta);
    Sleep(10);
  }
  return 0;
}
 
U0 Init()
{
  I64 i;
  bt=0;
  v=0.2;
  Fs->win_scroll_x=Fs->win_pixel_width >>1;
  Fs->win_scroll_y=Fs->win_pixel_height>>1;
  x=0;
  y=0;
  thetaf=theta=0;

  MemSet(tr,0,sizeof(tr));
  for (i=0;i<NUM_TREES;i++) {
    tr[i].x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    tr[i].y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
  }

  MemSet(tk,0,sizeof(tk));
  for (i=0;i<NUM_TANKS;i++) {
    tk[i].x=RandU32%MAP_WIDTH -MAP_WIDTH >>1;
    tk[i].y=RandU32%MAP_HEIGHT-MAP_HEIGHT>>1;
    tk[i].theta=Rand*2*pi;
  }
  key_cnt=0;
  tank_cnt=NUM_TANKS;
}

U0 Bomber()
{
  I64 ch,sc;
  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,CH_NEW_LINE);"
  "  Faster(,,SC_CURSOR_UP);"
  "  Slower(,,SC_CURSOR_DOWN);"
  "  Left(,,SC_CURSOR_LEFT);"
  "  Right(,,SC_CURSOR_RIGHT);"
  "  Bomb(,CH_SPACE);"
      "}"
      );
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WordStat;
  WinMax;
  Preempt(OFF);
  Fs->animate_task=Spawn(&AnimateTask,Fs,"Animate",Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Init;
  while (TRUE) {
    ch=GetKey(&sc);
    switch (ch) {
      case 0:
	switch (sc.u8[0]) {
	   case SC_CURSOR_UP:
	     v+=0.1;
	     if (v>3.0) v=3.0;
	     break;
	   case SC_CURSOR_DOWN:
	     v-=0.1;
	     if (v<0.2) v=0.2;
	     break;
	   case SC_CURSOR_LEFT:
	     key_cnt++;
	     thetaf-=0.5;
	     break;
	   case SC_CURSOR_RIGHT:
	     key_cnt++;
	     thetaf+=0.5;
	     break;
 
	}
	break;
      case CH_SPACE:
	key_cnt++;
	BombDrop;
	break;
      case CH_NEW_LINE:
	Init;
	break;
      case CH_ESC:
      case CH_SHIFT_ESC:
	goto bm_done;
    }
  }
bm_done:
  SettingsPop;
  MenuPop;
  AcctRegWriteBranch("SparrowOS/Bomber","I64 best_score=%d;\n",best_score);
}
 
Bomber;
 
