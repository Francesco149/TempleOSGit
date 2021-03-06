//See [TextBase Layer,1] TextBase Layer.

#define ATTR (BLACK<<12+WHITE<<8)

U32 text[TEXT_ROWS][TEXT_COLS];

U0 DrawIt(CTask *task,CDC *)
{ //[C:/Adam/Gr/GrGlbls.HC.Z,3] gr.text_base gets clear 30fps, so we must use our own permanent text array.
  MemCpy(gr.text_base+TEXT_COLS,text,(TEXT_ROWS-1)*TEXT_COLS*sizeof(U32));

  // You can copy it this way, if you like:
  //  I64 i,j;
  //  for (j=0;j<TEXT_ROWS;j++)
  //	for (i=0;i<TEXT_COLS;i++)
  //	 TextChar(task,,i,j,text[j][i]);

  TextPrint(task,0,0,ATTR>>8,"Draw a maze with left bttn.");
  TextPrint(task,0,1,ATTR>>8,"Solve maze starting at right click.");
}

#define STK_SIZE 2048
//We would put these as local vars
//in SolveMaze() but the system stk size
//is limited, so it's a bad habit.  The heap
//is the normal TempleOS technique, but
//it's a pain in this case.
I64	stk_ptr,
	stk_x  [STK_SIZE],
	stk_y  [STK_SIZE],
	stk_dir[STK_SIZE];

//Four directions:
//  0=Up,1=right,2=down,3=left
I64	dir_x[4]={ 0,+1, 0,-1},  // Could use [C:/Adam/Gr/GrMath.HC.Z,5] gr_x_offsets2,[C:/Adam/Gr/GrMath.HC.Z,6] gr_y_offsets2
	dir_y[4]={+1, 0,-1, 0};

U0 SolveMaze(I64 x,I64 y)
{
  I64 dir=0;
  stk_ptr=0;
  stk_x[stk_ptr]=x;
  stk_y[stk_ptr]=y;
  stk_dir[stk_ptr++]=dir;
  while (TRUE) {
    if (!(0<=x<MinI64(Fs->win_width,TEXT_COLS)) ||
	  !(0<=y<MinI64(Fs->win_height,TEXT_ROWS)) ) {
      Beep;Beep;
      break;
    }
    if (!text[y][x].u8[0])
      text[y][x]='.'+ATTR;
    x+=dir_x[dir];
    y+=dir_y[dir];
//u8.[0] is the ASCII
    if (text[y][x].u8[0]) {
      x-=dir_x[dir];
      y-=dir_y[dir];
      if (++dir==4) {
	if (--stk_ptr<0) return;
	x=stk_x[stk_ptr];
	y=stk_y[stk_ptr];
	dir=stk_dir[stk_ptr];
      }
    } else {
      dir=0;
      stk_x[stk_ptr]=x;
      stk_y[stk_ptr]=y;
      stk_dir[stk_ptr++]=dir;
      if (stk_ptr==STK_SIZE) return;
      Sleep(100);
      if (ScanChar)
	throw;
    }
  }
}

U0 Maze()
{
  I64 ch,x,y;

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER;

  try
    do {
      MemSet(text,0,sizeof(text));
      while (!(ch=ScanChar)) {
	x=ms.pos_text.x-Fs->win_left-Fs->scroll_x/FONT_WIDTH;
	y=ms.pos_text.y-Fs->win_top-Fs->scroll_y/FONT_HEIGHT;
	if (ms.lb&&!winmgr.grab_scroll)
	  text[y][x]=CH_SPACE+ATTRF_INVERT+ATTR;
	if (ms.rb&&!winmgr.grab_scroll) {
	  text[y][x]='*'+ATTR;
	  SolveMaze(x,y);
	  ch=GetChar;
	  break;
	}
	Refresh;
      }
    } while (ch!=CH_SHIFT_ESC && ch!=CH_ESC);
  catch
    PutExcept;
  SettingsPop;
}

Maze;
