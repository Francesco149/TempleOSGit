

	/* <1> <1> (image) */












	/* <2> <2> (image) */


	/* <3> <3> (image) */















#define BIRD_X 100
F64 dy,y,scroll_x;
Bool game_over;

#define PIPES_NUM	16
#define GAP		100

class Pipe
{
  I64 x,y;
} pipes[PIPES_NUM];

U0 DrawIt(CTask *task, CDC *dc)
{
  I64 i;
  Pipe *p;
  for (i=0;i<PIPES_NUM;i++) {
    p=&pipes[i];
    Sprite3(dc,p->x-scroll_x,p->y+task->pix_height>>1+GAP>>1,0,__BIN_3);
    Sprite3(dc,p->x-scroll_x,p->y+task->pix_height>>1-GAP>>1,0,__BIN_2);
  }

  dc->collision_cnt=0;
  dc->color=ROP_COLLISION;
  dc->bkcolor=WHITE;
  Sprite3ZB(dc,BIRD_X,y,0,__BIN_1,-0.2*dy);
  if (dc->collision_cnt)
    game_over=TRUE;

  dc->color=ROP_EQU;
  Sprite3ZB(dc,BIRD_X,y,0,__BIN_1,-0.2*dy);
}

U0 Init()
{
  I64 i;
  Pipe *p;
  for (i=0;i<PIPES_NUM;i++) {
    p=&pipes[i];
    p->x=i*125+200;
    p->y=80*Sin(i);
  }
  scroll_x=0;
  y=Fs->pix_height>>1;
  dy=0;
  game_over=FALSE;
}

U0 AnimateTask(I64)
{
  while (TRUE) {
    if (!game_over) {
      scroll_x++;
      y=y-dy+1;
      dy*=0.5;
    }
    Sleep(25);
  }
}

U0 Flappy()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocClear;
  Refresh(2);
  Init;
  Fs->draw_it=&DrawIt;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  try {
    while (TRUE) {
      switch (GetChar(,FALSE)) {
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto fp_done;
	case '\n':
	  Init;
	  break;
	case CH_SPACE:
	  Noise(20,74,86);
	  dy+=20;
	  break;
      }
    }
fp_done:
  } catch
    PutExcept;
  SettingsPop;
}

Flappy;