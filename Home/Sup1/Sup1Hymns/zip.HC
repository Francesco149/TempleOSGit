//5 has graphics


/* <1> <1> (image) */




/* <2> <2> (image) */




F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  U8 *tmps;
  if (dt>=16.0) {
    tmps=SpriteInterpolate(Tri(dt,1.0),__BIN_1,__BIN_2);
    Sprite3(dc,20,20,0,tmps);
    Free(tmps);
  }
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);

    while (!ScanKey) {
      t0=Beat;
      Play("5qGE4eGB5qGetD4B5DeDE4qGetGB5F",
	    "$RED$Wise \0men \0use \0few \0words.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5qGE4eGB5qGetD4B5DeDE4qGetGB5F",
	    "Not \0the \0king \0of \0nerds.\n\0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0");
      Play("5eE4A5sFEFEqF4A5etD4B5E4AAG5qC4A",
	    "Fools \0 \0kinda \0 \0 \0 \0 \0 \0talk \0"
	    " \0 \0all \0day \0long.$FG$\n\0 \0 \0");
      Play("5eE4A5sFEFEqF4A5etD4B5E4AAG5qC4A",
	    "What? \0 \0Tell \0 \0me! \0 \0 \0 \0Is \0"
	    " \0 \0some\0thing \0wrong?\n\0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
