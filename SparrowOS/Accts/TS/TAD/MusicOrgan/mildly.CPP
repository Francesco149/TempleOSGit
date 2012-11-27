//7 has graphics


/*  <1> (image) */










/*  <2> (image) */















F64 t0=tMBeat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 y,dt=tMBeat-t0;
  if (dt>=24.0)
    CSpritePlot3(dc,0,0,0,__BIN_2);
  else {
    y=Sin(2*pi*(tM%1.0-0.5)/1.0);
    CSpritePlot3(dc,0,5.0*y,0,__BIN_1);
    CSpritePlot3(dc,0,-5.0*y,0,__BIN_2);
  }
}

U0 Song()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  CTask *task=PopUpViewStr(
      "[::/SparrowOS/Misc/Bible.TXT.Z,79970]\n"
      );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  music_tempo=2.5;
  try {
    while (!ScanKey) {
      t0=tMBeat;
      DocClear;
      "$FG,WHITE$$BG,CYAN$\n";
      Play("3qEsGEGEqDsCCCCqBsDEDEqFG",
	  "Lord, \0there's \0 \0a \0 \0storm \0u\0 \0 \0 \0pon \0the \0 \0 \0 \0sea.\n\0 \0");
      Play("EsGEGEqDsCCCCqBsDEDEqFG",
	  "Lord, \0there's \0 \0a \0 \0storm \0u\0 \0 \0 \0pon \0the \0 \0 \0 \0sea.\n\0 \0");
      Play("GEC2G3etB2G3FGGEDBBeBE",
	  "Re\0lax \0fel\0las.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("qGEC2G3etB2G3FGGEDBBeBE",
	  "(Sea \0be\0came \0glass.)\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
