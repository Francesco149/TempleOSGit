RegSetDftEntry("TempleOS/BugBird","F64 best_score=9999;\n");
RegExeBranch("TempleOS/BugBird");

  /* <1> <1> (image) */



  /* <2> <2> (image) */


  /* <3> <3> (image) */



  /* <4> <4> (image) */

#define BORDER		5
#define AAT_TIME	0.5
#define FLAP_TIME	0.5
#define BIRD_BOX	10

Bool flap_down,flap_up,game_reset;
F64 flap_phase,bird_y,bird_x,eat_timeout,flap_time;
F64 frame_x,game_t0,game_tf;

#define MAX_BUGS	32
I32 bug_cnt,bugs_x[MAX_BUGS],bugs_y[MAX_BUGS];
Bool bugs_dead[MAX_BUGS];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,y,x,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  CSprite *temps,*temps2,*temps3;
  F64 tt=flap_phase*flap_phase*flap_phase;
  temps=SpriteInterpolate(__BIN_1,__BIN_2,tt);
  if (eat_timeout && tS<eat_timeout) {
    temps2=SpriteInterpolate(__BIN_3,__BIN_4,tt);
    temps3=SpriteInterpolate(temps2,temps,1.0-(eat_timeout-tS)/AAT_TIME);
    Free(temps);
    Free(temps2);
    temps=temps3;
  }
  Sprite3(dc,bird_x,bird_y,0,temps);
  Free(temps);
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+BORDER;
      y=bugs_y[i]%v+BORDER;
      dc->color=BLACK;
      GrPlot(dc,x,y);
      GrPlot(dc,x+1,y);
      dc->color=LTPURPLE;
      GrPlot(dc,x,y-1);
    }
  if (game_tf) {
    dc->color=RED;
    GrPrint(dc,(task->pix_width-FONT_WIDTH*14)/2,
	  (task->pix_height-FONT_HEIGHT)/2,"Game Completed");
    tt=game_tf;
  } else {
    dc->color=GREEN;
    tt=tS;
  }
  GrPrint(dc,0,0,"Bugs:%3.1f%% Time:%3.2f Best:%3.2f",
	100.0*(MAX_BUGS-bug_cnt)/MAX_BUGS,tt-game_t0,best_score);
}

U0 CheckBugs(CTask *task,CTask *song_task)
{
  I64 i,x,y,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  if (eat_timeout && eat_timeout-tS<0.75*AAT_TIME) {
    Suspend(song_task,FALSE);
    if (tS>=eat_timeout)
      eat_timeout=0;
  }
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+BORDER;
      y=bugs_y[i]%v+BORDER;
      if (AbsI64(x-bird_x)<BIRD_BOX && AbsI64(y-bird_y)<BIRD_BOX) {
	bugs_dead[i]=TRUE;
	eat_timeout=tS+AAT_TIME;
	Snd(1000);
	Suspend(song_task);
	bug_cnt--;
      }
    }
  if (!game_tf && !bug_cnt) {
    game_tf=tS;
    Suspend(song_task);
    Snd(0);
    if (game_tf-game_t0<best_score)
      best_score=game_tf-game_t0;
  }
  frame_x-=0.1;
  if (frame_x<0)
    frame_x+=h;
}

U0 Init(CTask *song_task)
{
  I64 i;
  flap_down=flap_up=FALSE;
  flap_phase=0;
  bird_x=100;
  bird_y=0;
  frame_x=0;
  bug_cnt=MAX_BUGS;
  for (i=0;i<MAX_BUGS;i++) {
    bugs_dead[i]=FALSE;
    bugs_x[i]=RandU16;
    bugs_y[i]=RandU16;
  }
  Suspend(song_task,FALSE);
  flap_time=eat_timeout=0;
  game_tf=0;
  game_t0=tS;
}

U0 SongTask(I64)
{//Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3eBEBCBEsEFqEeBEBCBEsEF");
    Play("qEeADABADsDCqDeBEBCB");
    Play("EsEDqE");
  }
}

U0 AnimateTask(I64)
{
  F64 tt=0;
  while (TRUE) {
    if (game_reset) {
      Init(Fs->parent_task->song_task);
      tt=0;
      game_reset=FALSE;
    }
    if (flap_down) {
      flap_down=FALSE;
      tt=-0.005*Min(1.0,(tS-flap_time)/FLAP_TIME);
      flap_time=tS;
    } else if (flap_up) {
      flap_up=FALSE;
      tt= 0.005;
    }
    if (tt<0) {
      bird_y+=75*tt;
      tt+=0.000015;
    } else
      bird_y+=0.15;
    bird_y=Clamp(bird_y,BORDER,Fs->parent_task->pix_height-BORDER);
    flap_phase=Clamp(flap_phase+tt,0.0,1.0);
    CheckBugs(Fs->parent_task,Fs->parent_task->song_task);
    Sleep(1);
  }
}

U0 BugBird()
{
  Bool rst_space=TRUE;
  I64 a1,a2;

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Flap(,CH_SPACE);"
	"}"
	);

  PopUpOk("Use $GREEN$<SPACE>$FG$ to flap.\nHold down to glide.");

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinBorder(ON);
  WinHorz(1,TEXT_COLS/2);
  WinVert(2,TEXT_ROWS/2);
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  game_reset=TRUE;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  while (game_reset)
    Yield;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (GetMsg(&a1,&a2,1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (a1) {
	    case CH_SPACE:
	      if (rst_space) {
		flap_down=TRUE;
		rst_space=FALSE;
	      }
	      break;
	    case '\n':
	      game_reset=TRUE;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto bl_done;
	  }
	  break;
	case MSG_KEY_UP:
	  switch (a1) {
	    case CH_SPACE:
	      flap_up=TRUE;
	      rst_space=TRUE;
	      break;
	  }
	  break;
      }
    }
bl_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
  RegWriteBranch("TempleOS/BugBird","F64 best_score=%5.4f;\n",best_score);
}

BugBird;
