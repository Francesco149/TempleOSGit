I64 sc_1,sc_2;

#define NUM_DROPS	0x2000

class Drop
{
  I32 x,y;
} drops[NUM_DROPS];


GrDC *cur_dc;

U0 DrawFrame(TaskStruct *task,GrDC *dc)
{
  I64 w=task->win_pixel_width,h=task->win_pixel_height,
     cx=w>>1,cy=h>>1;

  dc->color=LTRED;
  GrLine(dc,cx-20,cy-50,cx,cy-150);
  GrLine(dc,cx+20,cy-50,cx,cy-150);

  dc->color=LTRED;
  GrLine(dc,cx-100,cy-100,cx,cy);
  GrLine(dc,cx+100,cy-100,cx,cy);
  if (Bt(key_down_bitmap,sc_1)) {
    dc->color=WHITE;
    GrRect(dc,cx-2,cy-2,5,5);
  }
  dc->color=BLACK;
  GrPutS(dc,cx+10,cy,"Press '1'");

  dc->color=LTRED;
  GrRect(dc,cx-20,cy+60,41,21);

  dc->color=LTRED;
  GrLine(dc,cx-200,cy,cx,cy+100);
  GrLine(dc,cx+200,cy,cx,cy+100);
  if (Bt(key_down_bitmap,sc_2)) {
    dc->color=WHITE;
    GrRect(dc,cx-2,cy+100-2,5,5);
  }
  dc->color=BLACK;
  GrPutS(dc,cx+10,cy+100,"Press '2'");

  dc->color=LTRED;
  GrLine(dc,cx+400,cy+100,cx-250,cy+200);
}

U0 DrawDrops(TaskStruct *task,GrDC *dc)
{
  nounusedwarn task;
  I64 i;
  dc->color=BLUE;
  for (i=0;i<NUM_DROPS;i++)
    if (drops[i].y>=0 && GrPeek(dc,drops[i].x,drops[i].y)==WHITE)
      GrPlot(dc,drops[i].x,drops[i].y);
}

U0 UpdateCurBase(TaskStruct *task,GrDC *dc)
{
  BoolI8 old_preempt=Preempt(OFF);
  cur_dc->color=ROP_EQU;
  dc->flags|=DCF_NO_TRANSPARENTS;
  GrDCBlot(cur_dc,-task->win_pixel_left-task->win_scroll_x,-task->win_pixel_top-task->win_scroll_y,dc);
  dc->flags&=~DCF_NO_TRANSPARENTS;
  Preempt(old_preempt);
}

U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  DrawFrame(task,dc);
  DrawDrops(task,dc);
  UpdateCurBase(task,dc);
}


U0 AnimateTask(U64 dummy=0)
{
  nounusedwarn dummy;
  I64 i,j,cur_drop=0,c1,c2,x0,y0,x1,y1,w,h;
  while (TRUE) {
    w=Fs->parent_task->win_pixel_width;
    h=Fs->parent_task->win_pixel_height;

    for (i=0;i<2;i++) {
      j=0;
      do {
	if (++cur_drop>=NUM_DROPS)
	  cur_drop=0;
	if (drops[cur_drop].y<0)
	  break;
	j++;
      } while (j<=NUM_DROPS);
 
      drops[cur_drop].x=RandU32%w;
      drops[cur_drop].y=0;
    }

    for (i=0;i<NUM_DROPS;i++) {
      if (drops[i].y>=0) {
	if (drops[i].y>=h-1)
	  drops[i].y=-MAX_I32;
	else {
	  x0=x1=drops[i].x;
	  y0=y1=drops[i].y;
	  if (GrPeek(cur_dc,x0,y0+1)==WHITE)
	    y1++;
	  else {
	    c1=GrPeek(cur_dc,x0+1,y0);
	    c2=GrPeek(cur_dc,x0-1,y0);
	    if (c1==WHITE && c2!=WHITE)
	      x1++;
	    else if (c2==WHITE && c1!=WHITE)
	      x1--;
	    else if (c1==WHITE && c2==WHITE) {
	      c1=GrPeek(cur_dc,x0+1,y0+1);
	      c2=GrPeek(cur_dc,x0-1,y0+1);
	      if (c1==WHITE && c2!=WHITE)
		x1++;
	      else if (c2==WHITE && c1!=WHITE)
		x1--;
	      else if (RandI16>=0)
		x1++;
	      else
		x1--;
	    }
	    if (GrPeek(cur_dc,x1,y1+1)==WHITE)
	      y1++;
	  }
	  if (GrPeek(cur_dc,x0,y0)==BLUE) {
	    cur_dc->color=WHITE;
	    GrPlot(cur_dc,x0,y0);
	  }
	  cur_dc->color=BLUE;
	  GrPlot(cur_dc,x1,y1);
	  drops[i].x=x1;
	  drops[i].y=y1;
	}
      }
    }
    Sleep(10);
  }
}

U0 Init()
{
  I64 i;
  MemSet(drops,0,sizeof(drops));
  for (i=0;i<NUM_DROPS;i++)
    drops[i].y=-MAX_I32;
}

U0 RainDrops()
{
  U64 ch,sc;
  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,CH_NEW_LINE);"
  "  OpenUpper(,'1');"
  "  OpenLower(,'2');"
      "}"
      );
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  cur_dc=GrDCNew(DCT_COLOR8,GR_WIDTH,GR_HEIGHT);
  sc_1=A2ScanCode('1');
  sc_2=A2ScanCode('2');
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);
  LtfCursor;
  LtfClear;
  Fs->draw_it=&DrawIt;
  WordStat;
  try {
    do {
      ch=GetKey(&sc);
      switch (ch) {
	case CH_NEW_LINE:
	  Init;
	  break;
      }
    } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
  GrDCDel(cur_dc);
}

RainDrops;
