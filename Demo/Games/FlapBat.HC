RegDft("TempleOS/FlapBat","F64 best_score=9999;\n");
RegExe("TempleOS/FlapBat");






	/* <1> <1> (image) */


	/* <2> <2> (image) */





	/* <3> <3> (image) */


	/* <4> <4> (image) */


	/* <5> <5> (image) */




#define BORDER		6
#define EAT_TIME	0.5
#define FLAP_TIME	0.5
#define BAT_BOX	10

Bool flap_down,flap_up;
F64 flap_phase,delta_phase,bat_y,bat_x,eat_timeout,flap_time;
F64 frame_x,game_t0,game_tf;

#define BUGS_NUM	32
I32 bug_cnt,bugs_x[BUGS_NUM],bugs_y[BUGS_NUM];
Bool bugs_dead[BUGS_NUM];

#define GLOW_PERIOD	3.0
F64 bugs_glow_phase[BUGS_NUM];

CDC *limit_flood_fill_dc; //Prevent uncontrolled flood-fill flicker.

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,y,x,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  U8 *tmps,*tmps2,*tmps3;
  F64 tt=flap_phase*flap_phase*flap_phase,ts=tS;

  bat_x=task->pix_width>>3;
  Sprite3(dc,(7*task->pix_width)>>3,20,0,__BIN_5);

  dc->color=DKGRAY;
  GrCircle(dc,bat_x+5+20*Saw(15*tS,2),bat_y,10+20*Saw(15*tS,2),5,-pi/4,pi/2);

  tmps=SpriteInterpolate(tt,__BIN_1,__BIN_2);
  if (eat_timeout && tS<eat_timeout) {
    tmps2=SpriteInterpolate(tt,__BIN_3,__BIN_4);
    tmps3=SpriteInterpolate(1.0-(eat_timeout-tS)/EAT_TIME,tmps2,tmps);
    Free(tmps);
    Free(tmps2);
    tmps=tmps3;
  }

  DCFill(limit_flood_fill_dc);
  Sprite3(limit_flood_fill_dc,16,32,0,tmps);
  if (GrPeek(limit_flood_fill_dc,0,0)!=TRANSPARENT) {//Did FloodFill go crazy?
    limit_flood_fill_dc->color=TRANSPARENT;
    GrFloodFill(limit_flood_fill_dc,0,0);
  }
  GrBlot(dc,bat_x-16,bat_y-32,limit_flood_fill_dc);

  Free(tmps);
  for (i=0;i<BUGS_NUM;i++)
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
	100.0*(BUGS_NUM-bug_cnt)/BUGS_NUM,tt-game_t0,best_score);
}

U0 CheckBugs(CTask *task)
{
  I64 i,x,y,
	h=MaxI64(1,task->pix_width-2*BORDER),
	v=MaxI64(1,task->pix_height-2*BORDER);
  if (eat_timeout && eat_timeout-tS<0.75*EAT_TIME) {
    Suspend(task->song_task,FALSE);
    if (tS>=eat_timeout)
      eat_timeout=0;
  }
  for (i=0;i<BUGS_NUM;i++)
    if (!bugs_dead[i]) {
      x=(bugs_x[i]+frame_x)%h+BORDER;
      y=bugs_y[i]%v+BORDER;
      if (AbsI64(x-bat_x)<BAT_BOX && AbsI64(y-bat_y)<BAT_BOX) {
	bugs_dead[i]=TRUE;
	eat_timeout=tS+EAT_TIME;
	Snd(74);
	Suspend(task->song_task);
	bug_cnt--;
      }
    }
  if (!game_tf && !bug_cnt) {
    game_tf=tS;
    Suspend(task->song_task);
    Snd;
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
  bat_x=Fs->pix_width>>3;
  bat_y=0;
  frame_x=0;
  bug_cnt=BUGS_NUM;
  for (i=0;i<BUGS_NUM;i++) {
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
    Play("4eB5E4B5C4B5EsEFqE4eB5E4B5C4B5EsEF");
    Play("5qE4eA5D4ABA5DsDCqD4eB5E4B5C4B");
    Play("5EsEDqE");
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
      bat_y+=75*delta_phase;
      delta_phase+=0.000015;
    } else
      bat_y+=0.15;
    bat_y=Clamp(bat_y,BORDER,Fs->parent_task->pix_height-BORDER);
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
  I64 arg1,arg2;

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

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
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
      switch (GetMsg(&arg1,&arg2,1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (arg1) {
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
	  switch (arg1) {
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
  CleanUp;
  MenuPop;
  RegWrite("TempleOS/FlapBat","F64 best_score=%5.4f;\n",best_score);
}

FlapBat;
