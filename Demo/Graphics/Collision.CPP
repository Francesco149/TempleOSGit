

/* <1> <1> (image) */


/* <2> <2> (image) */

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 x,y;

  x=ip.pos.x-task->pix_left;
  y=ip.pos.y-task->pix_top;
  Sprite3(dc,x,y,0,__BIN_1);

  x=task->pix_width>>1;
  y=task->pix_height>>1;

  dc->color=ROP_COLLISION;
  dc->bkcolor=WHITE;
  dc->collision_cnt=0;
  Sprite3(dc,x,y,0,__BIN_1);

  dc->color=ROP_EQU;
  GrPrint(dc,0,(task->pix_height-FONT_HEIGHT)>>1,
	"Collisions:%d",dc->collision_cnt);

  if (!dc->collision_cnt)
    Sprite3(dc,x,y,0,__BIN_1);
  else
    Sprite3(dc,x,y,0,__BIN_2);
}

U0 Collision()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  PressAKey;
  SettingsPop;
}

Collision;
