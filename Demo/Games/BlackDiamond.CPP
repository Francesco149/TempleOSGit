//These images were created with <CTRL-R>
//and can be edited by moving the
//cursor on top and pressing <CTRL-R>
//again.

//<CTRL-T> to see the num of
//the image and <CTRL-T> again to get back.
//Nums are assigned by the editor.




/* <1> <1> (image) */













/* <2> <2> (image) */

 















#define NUM_TREES	128

I64 x=100,y=100;
I64 trees_x[NUM_TREES],trees_y[NUM_TREES];
I64 cur_screen_y=0;
I64 penalty=0;

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 buf[128];
  I64 i;
//<CTLR-T> to see the syntax of the
//"Insert Binary" statement in the following.
  for (i=0;i<NUM_TREES;i++)
    SpritePlot3(dc,trees_x[i],trees_y[i]-cur_screen_y,0,__BIN_2);

  dc->collision_cnt=0;
  dc->color=ROP_COLLISION;
  dc->bkcolor=WHITE;
  SpritePlot3(dc,x,y,0,__BIN_1);
  if (dc->collision_cnt) {
    Snd(400);
    penalty++;
  } else
    Snd(0);

  dc->color=ROP_EQU;
  SpritePlot3(dc,x,y,0,__BIN_1);
//Text attr include forground in the
//lowest 4 bits and background in the upper 4.
  SPrint(buf,"Penalty:%d",penalty);
  PlotWinStr(task,buf,BLACK+WHITE*16,1,1);
}

U0 Init()
{
  I64 i;
  for (i=0;i<NUM_TREES;i++) {
    trees_x[i]=RandU32%GR_WIDTH;
    trees_y[i]=RandU32&2047;
  }
}


U0 BlackDiamond()
{
  I64 ch,sc;

  MenuPush(
  "File {"
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
    do {
      if (ScanKey(&ch,&sc)) {
	switch (sc.u8[0]) {
	  case SC_CURSOR_RIGHT:
	    x+=10;
	    if (x>=GR_WIDTH) x-=GR_WIDTH;
	    break;
	  case SC_CURSOR_LEFT:
	    x-=10;
	    if (x<0) x+=GR_WIDTH;
	    break;
	  case SC_CURSOR_UP:
	    y-=10;
	    if (y<0) y+=GR_HEIGHT;
	    break;
	  case SC_CURSOR_DOWN:
	    y+=10;
	    if (y>=GR_HEIGHT) y-=GR_HEIGHT;
	    break;
	}
	//We don't want keystrokes building-up
	//in the buf.
	FlushMsgs;
      }
      y=(y+1)%GR_HEIGHT;
      cur_screen_y=(cur_screen_y+1)%(2048-GR_HEIGHT);

      Sleep(20);
    } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
}

//Start game when #included.
BlackDiamond;
