//These sprites were created with <CTRL-R>
//and can be edited by moving the
//cursor on top and pressing <CTRL-R>
//again.

//<CTRL-T> to see the num of
//the image and <CTRL-T> again to get back.
//Nums are assigned by the editor.








	/* <1> <1> (image) */














	/* <2> <2> (image) */





	/* <3> <3> (image) */









	/* <4> <4> (image) */



AcctRegSetDftEntry("TempleOS/BlackDiamond","I64 best_score=9999;\n");
AcctRegExeBranch("TempleOS/BlackDiamond");

#define MAP_HEIGHT	3000
#define NUM_OBJS	128

I64 x,y,screen_top_y,penalty;
I64 obj_x[NUM_OBJS],obj_y[NUM_OBJS],obj_types[NUM_OBJS];
CSprite *imgs[3]={__BIN_2,__BIN_3,__BIN_4};
Bool game_over;

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 buf[128];
  I64 i;
  //<CTLR-T> to see the syntax of the
  //"Insert Binary" statement in the following.
  for (i=0;i<NUM_OBJS;i++)
    Sprite3(dc,obj_x[i],obj_y[i]-screen_top_y,0,imgs[obj_types[i]]);

  dc->collision_cnt=0;
  dc->color=ROP_COLLISION;
  dc->bkcolor=WHITE;
  Sprite3(dc,x,y-screen_top_y,0,__BIN_1);
  if (!game_over) {
    if(dc->collision_cnt) {
      Snd(400);
      penalty++; //Timing is irregular. Scoring is imperfect. 60fps more or less.
    } else
      Snd(0);
  }

  dc->color=ROP_EQU;
  Sprite3(dc,x,y-screen_top_y,0,__BIN_1);

  dc->color=BLACK;
  GrPrint(dc,0,0,"Penalty:%d Best:%d",penalty,best_score);
  if (game_over && Blink) {
    dc->color=RED;
    GrPrint(dc,(Fs->pix_width-9*FONT_WIDTH)/2,
	       (Fs->pix_height-FONT_HEIGHT)/2,"Game Over");
  }
}

I64 Compare(I64 e1,I64 e2)
{
  return e1-e2;
}

U0 Init()
{
  I64 i;
  for (i=0;i<NUM_OBJS;i++) {
    obj_types[i]=!(i&31)+!(i&7);
    obj_x[i]=RandU32%GR_WIDTH;
    obj_y[i]=RandU32%MAP_HEIGHT;
  }
  QSortI64(obj_y,NUM_OBJS,&Compare); //We break associations.  Doesn't matter.
}


U0 BlackDiamond()
{
  I64 ch,sc;

  MenuPush(
	"File {"
	"  New(,'\n');"
	"  Restart(,CH_SPACE);"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Up(,,SC_CURSOR_UP);"
	"  Down(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  WinBorder;
  WordStat;

  Init;
  try {
    while (TRUE) {
bd_restart:
      game_over=FALSE;
      screen_top_y=0;
      x=Fs->pix_width>>1; y=0;
      penalty=0;
      while (TRUE) {
	if (ScanKey(&ch,&sc)) {
	  switch (ch) {
	    case 0:
	      if (!game_over)
		switch (sc.u8[0]) {
		  case SC_CURSOR_RIGHT:
		    x+=10;
		    if (x>=Fs->pix_width) x-=Fs->pix_width;
		    break;
		  case SC_CURSOR_LEFT:
		    x-=10;
		    if (x<0) x+=Fs->pix_width;
		    break;
		  case SC_CURSOR_UP:
		    y-=10;
		    break;
		  case SC_CURSOR_DOWN:
		    y+=10;
		    break;
		}
	      break;

	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto bd_done;

	    case '\n':
	      Init;
	      goto bd_restart;

	    case CH_SPACE:
	      game_over=TRUE;
	      while (screen_top_y>0||y>0) { //Animate going back to top.
		screen_top_y=MaxI64(0,screen_top_y-4);
		y=MaxI64(0,y-4);
		x+=SignI64(Fs->pix_width>>1-x);
		Sleep(1);
	      }
	      Snd(0);
	      goto bd_restart;
	  }
	  //Don't want keystrokes building-up in the buf.
	  FlushMsgs;
	}
	if (!game_over) {
	  y+=2;
	  screen_top_y++;
	  if (y-screen_top_y>Fs->pix_height) { //Animate scrolling screen.
	    while (y-screen_top_y>Fs->pix_height>>1) {
	      screen_top_y+=2;
	      Sleep(1);
	    }
	  }
	  if (y>=MAP_HEIGHT) {
	    game_over=TRUE;
	    Beep;
	    if (penalty<=best_score) {
	      best_score=penalty;
	      Beep;
	    }
	  }
	}
	Sleep(10);
      }
    }
bd_done:
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
  AcctRegWriteBranch("TempleOS/BlackDiamond","I64 best_score=%d;\n",best_score);
}

//Start game when #included.
BlackDiamond;
