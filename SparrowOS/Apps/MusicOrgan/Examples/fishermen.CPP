//8 has graphics


/* <1> <1> (image) */





U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,76908]\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,81941]\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);

  while (!ScanKey) {
	Play("3qEsGEGEeF2G3BCqCeDEqBsERER",
		"$FG$Lord, \0I \0 \0am \0 \0feel\0ing \0just \0a \0bit \0un\0in\0spired.\n\0With \0 \0 \0 \0");
	Play("qEsGEGEeF2G3BCqCeDEqBsERER",
		"net \0or \0 \0with \0 \0reel\0ing \0 \0I \0keep \0get\0ting \0mired.\n\0 \0 \0 \0 \0");
	Play("qDetRAFqDGetFC2G3sBCBCqDF",
		"$FG,RED$Throw \0 \0out \0your \0net.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("DetRAFqDGetFC2G3sBCBCqDF",
		"Make \0 \0you \0a \0bet.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	PutSprite(__BIN_1);
	NewLine(5);
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
