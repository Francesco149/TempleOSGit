//8 has graphics


/* <1> <1> (image) */




/* <2> <2> (image) */




F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  CSprite *temps;
  if (dt>=16.0) {
    temps=SpriteInterpolate(__BIN_1,__BIN_2,Tri(dt,1.0));
    Sprite3(dc,20,20,0,temps);
    Free(temps);
  }
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  "[C:/Misc/Bible.TXT.Z,53793]\n"
	  );
    TaskWait(task);
    SetWinHorz(Fs->win_left,Fs->win_right,task);
    SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);

    while (!ScanKey) {
      t0=Beat;
      Play("3qGE2eG3BqGetDBDeDE2qGetG3BF",
	    "$RED$Wise \0men \0use \0few \0words.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("qGE2eG3BqGetDBDeDE2qGetG3BF",
	    "Not \0the \0king \0of \0nerds.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("eEAsFEFEqFAetDBEAA2G3qCA",
	    "Fools \0 \0kinda \0 \0 \0 \0 \0 \0talk \0 \0 \0all \0day \0long.$FG$\n\0 \0 \0");
      Play("eEAsFEFEqFAetDBEAA2G3qCA",
	    "What? \0 \0Tell \0 \0me! \0 \0 \0 \0Is \0 \0 \0some\0thing \0wrong?\n\0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
