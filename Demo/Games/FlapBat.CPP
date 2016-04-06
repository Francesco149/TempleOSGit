RegSetDftEntry("TempleOS/FlapBat","F64 best_score=9999;\n");
RegExeBranch("TempleOS/FlapBat");






	/* <1> <1> (image) */


	/* <2> <2> (image) */





	/* <3> <3> (image) */


	/* <4> <4> (image) */


	/* <5> <5> (image) */




#define BORDER		6
#define AAT_TIME	0.5
#define FLAP_TIME	0.5
#define BIRD_BOX	10

Bool flap_down,flap_up;
F64 flap_phase,delta_phase,bird_y,bird_x,eat_timeout,flap_time;
F64 frame_x,game_t0,game_tf;

#define MAX_BUGS	32
I32 bug_cnt,bugs_x[MAX_BUGS],bugs_y[MAX_BUGS];
Bool bugs_dead[MAX_BUGS];

#define GLOW_PERIOD	3.0
F64 bugs_glow_phase[MAX_BUGS];

CDC *limit_flood_fill_dc; //Prevent uncontrolled flood-fill flicker.

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,y,x,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  CSprite *temps,*temps2,*temps3;
  F64 tt=flap_phase*flap_phase*flap_phase,ts=tS;

  bird_x=task->pix_width>>3;
  Sprite3(dc,(7*task->pix_width)>>3,20,0,__BIN_5);

  dc->color=DKGRAY;
  GrCircle(dc,bird_x+5+20*Saw(15*tS,2),bird_y,10+20*Saw(15*tS,2),5,-pi/4,pi/2);

  temps=SpriteInterpolate(tt,__BIN_1,__BIN_2);
  if (eat_timeout && tS<eat_timeout) {
    temps2=SpriteInterpolate(tt,__BIN_3,__BIN_4);
    temps3=SpriteInterpolate(1.0-(eat_timeout-tS)/AAT_TIME,temps2,temps);
    Free(temps);
    Free(temps2);
    temps=temps3;
  }

  DCFill(limit_flood_fill_dc);
  Sprite3(limit_flood_fill_dc,16,32,0,temps);
  if (GrPeek(limit_flood_fill_dc,0,0)!=TRANSPARENT) {//Did FloodFill go crazy?
    limit_flood_fill_dc->color=TRANSPARENT;
    GrFloodFill(limit_flood_fill_dc,0,0);
  }
  GrBlot(dc,bird_x-16,bird_y-32,limit_flood_fill_dc);

  Free(temps);
  for (i=0;i<MAX_BUGS;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+BORDER;
      y=bugs_y[i]%v+BORDER;
      if (Saw(ts+bugs_glow_phase[i],GLOW_PERIOD)<0.2) {
	if (i&1)
	  dc->color=YELLOW;
	else
	  dc->color=LTGREEN;
      } else
	dc->color=BLACK;
      GrPlot(dc,x,y);
      GrPlot(dc,x+1,y);
      dc->color=BLACK;
      GrPlot(dc,x,y-1);
    }
  if (game_tf) {
    dc->color=RED;
    GrPrint(dc,(task->pix_width-FONT_WIDTH*14)/2,
	  (task->pix_height-FONT_HEIGHT)/2,"Game Completed");
    tt=game_tf;
  } else {
    dc->color=BLACK;
    tt=tS;
  }
  GrPrint(dc,0,0,"Bugs:%3.1f%% Time:%3.2f Best:%3.2f",
	100.0*(MAX_BUGS-bug_cnt)/MAX_BUGS,tt-game_t0,best_score);
}

U0 CheckBugs(CTask *task)
{
  I64 i,x,y,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  if (eat_timeout && eat_timeout-tS<0.75*AAT_TIME) {
    Suspend(task->song_task,FALSE);
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
	Suspend(task->song_task);
	bug_cnt--;
      }
    }
  if (!game_tf && !bug_cnt) {
    game_tf=tS;
    Suspend(task->song_task);
    Snd(0);
    if (game_tf-game_t0<best_score)
      best_score=game_tf-game_t0;
  }
  frame_x-=0.1;
  if (frame_x<0)
    frame_x+=h;
}

U0 Init()
{
  I64 i;
  limit_flood_fill_dc=DCNew(32,40);
  flap_down=flap_up=FALSE;
  flap_phase=0;
  bird_x=Fs->pix_width>>3;
  bird_y=0;
  frame_x=0;
  bug_cnt=MAX_BUGS;
  for (i=0;i<MAX_BUGS;i++) {
    bugs_dead[i]=FALSE;
    bugs_x[i]=RandU16;
    bugs_y[i]=RandU16;
    bugs_glow_phase[i]=GLOW_PERIOD*Rand;
  }
  Suspend(Fs->song_task,FALSE);
  flap_time=eat_timeout=0;
  delta_phase=game_tf=0;
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
  while (TRUE) {
    if (flap_down) {
      flap_down=FALSE;
      delta_phase=-0.005*Min(1.0,(tS-flap_time)/FLAP_TIME);
      flap_time=tS;
    } else if (flap_up) {
      flap_up=FALSE;
      delta_phase= 0.005;
    }
    if (delta_phase<0) {
      bird_y+=75*delta_phase;
      delta_phase+=0.000015;
    } else
      bird_y+=0.15;
    bird_y=Clamp(bird_y,BORDER,Fs->parent_task->pix_height-BORDER);
    flap_phase=Clamp(flap_phase+delta_phase,0.0,1.0);
    CheckBugs(Fs->parent_task);
    Sleep(1);
  }
}

U0 CleanUp()
{
  DCDel(limit_flood_fill_dc);
}

U0 FlapBat()
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
  Fs->text_attr=LTGRAY<<4+WHITE;
  WinBorder(ON);
  WinHorz(1,TEXT_COLS/2);
  WinVert(2,TEXT_ROWS/2);
  DocCursor;
  DocClear;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
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
	      CleanUp;
	      Init;
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
  CleanUp;
  SettingsPop;
  MenuPop;
  RegWriteBranch("TempleOS/FlapBat","F64 best_score=%5.4f;\n",best_score);
}

FlapBat;
