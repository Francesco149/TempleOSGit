//8 has graphics


/* <1> <1> (image) */






/* <2> <2> (image) */




F64 t0=tMBeat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=tMBeat;
  U8 *inter;
  if (dt>=16.0) {
    inter=CSpriteInterpolate(__BIN_1,__BIN_2,Abs((4.0*tM%2-1)%1));
    CSpritePlot3(dc,20,20,0,inter);
    Free(inter);
  }
}

U0 Song()
{
  CTask *task;

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,53793]\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,53793]\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,53793]\n"
	"[::/SparrowOS/Misc/Bible.TXT.Z,53793]\n"
	);
    TaskWait(task);
    task->win_right=Fs->win_right;
    task->win_left=Fs->win_left;
    task->win_top=Fs->win_bottom+3;
    task->win_bottom=task->win_top+12;
    TaskDerivedValsUpdate(task);

    while (!ScanKey) {
      t0=tMBeat;
      Play("3qGE2eG3BqGetDBDeDE2qGetG3BF",
	  "Wise \0men \0use \0few \0words.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("qGE2eG3BqGetDBDeDE2qGetG3BF",
	  "Wise \0men \0use \0few \0words.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("eEAsFEFEqFAetDBEAA2G3qCA",
	  "Fools \0 \0kinda \0 \0 \0 \0 \0 \0talk \0 \0 \0all \0day \0long.\n\0 \0 \0");
      Play("eEAsFEFEqFAetDBEAA2G3qCA",
	  "Fools \0 \0kinda \0 \0 \0 \0 \0 \0talk \0 \0 \0all \0day \0long.\n\0 \0 \0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
