






/* <1> <1> (image) */


#define MAP_HEIGHT	2048
#define MAP_WIDTH	2048

#define NUM_TREES	256

I64 tree_x[NUM_TREES],tree_y[NUM_TREES];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,h,v;

  task->horz_scroll.min=0;
  task->horz_scroll.max=MAP_WIDTH-task->win_pixel_width;
  task->vert_scroll.min=0;
  task->vert_scroll.max=MAP_HEIGHT-task->win_pixel_height;
  TaskDerivedValsUpdate(task);

  h=task->horz_scroll.pos;
  v=task->vert_scroll.pos;

  dc->color=BROWN;
  dc->pen_width=4;
  GrLine3(dc,2-h,2-v,0,MAP_WIDTH-3-h,2-v,0);
  GrLine3(dc,2-h,MAP_HEIGHT-3-v,0,MAP_WIDTH-3-h,MAP_HEIGHT-3-v,0);
  GrLine3(dc,2-h,2-v,0,2-h,MAP_HEIGHT-3-v,0);
  GrLine3(dc,MAP_WIDTH-3-h,2-v,0,MAP_WIDTH-3-h,MAP_HEIGHT-3-v,0);

//Check bounds if you wish better performance.
  for (i=0;i<NUM_TREES;i++)
    SpritePlot3(dc,tree_x[i]-h,tree_y[i]-v,0,__BIN_1);
}

U0 Init()
{
  I64 i;
  for (i=0;i<NUM_TREES;i++) {
    tree_x[i]=RandU16%MAP_WIDTH;
    tree_y[i]=RandU16%MAP_HEIGHT;
  }
}

U0 ScrollBars()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Init;
  DocCursor;
  DocClear;
  DocScroll;
  Fs->draw_it=&DrawIt;
  WinBorder(ON);

  Fs->horz_scroll.pos=0;
  Fs->vert_scroll.pos=0;
  GetChar(,FALSE);
  SettingsPop;
  //If you care, you might want to
  //save the original state of the
  //scroll bars.
}

ScrollBars;
