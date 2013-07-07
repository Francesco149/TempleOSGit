

/* <1> <1> (image) */



/* <2> <2> (image) */


U0 DrawIt(CTask *task,CDC *dc)
{
  I64 x,y;

  x=ipx-task->win_pixel_left;
  y=ipy-task->win_pixel_top;
  SpritePlot3(dc,x,y,0,__BIN_1);

  x=task->win_pixel_width>>1;
  y=task->win_pixel_height>>1;

  dc->color=ROP_COLLISION;
  dc->bkcolor=WHITE;
  dc->collision_cnt=0;
  SpritePlot3(dc,x,y,0,__BIN_1);

  dc->color=ROP_EQU;
  GrPrintF(dc,0,(task->win_pixel_height-FONT_HEIGHT)>>1,"Collisions:%d",dc->collision_cnt);

  if (!dc->collision_cnt)
    SpritePlot3(dc,x,y,0,__BIN_1);
  else
    SpritePlot3(dc,x,y,0,__BIN_2);
}

U0 Main()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  PressAKey;
  SettingsPop;
}

Main;
