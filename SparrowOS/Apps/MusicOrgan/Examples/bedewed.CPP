//8 has graphics
/* <1> <1> (image) */











U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,94869] Philippians,4:8\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  "$WW+H,1$";
  while (!ScanKey) {
	Play("3eEGqACetCFGeEFqGetGFEqA",
		"Look \0at \0that!  \0 \0 \0 \0Dew \0 \0 \0on \0flow\0 \0 \0ers.\n\0");
	Play("eEGqACetCFGeEFqGetGFEqA",
		"Look \0at \0that!  \0 \0 \0 \0Dew \0 \0 \0on \0flow\0 \0 \0ers.\n\0");
	PutSprite(__BIN_1);
	"$CM,0,7$";
	Play("eDEqEAEFCCetCAD",
		"Pret\0ty \0when \0 \0they \0catch \0the \0light.\n\0 \0 \0 \0");
	Play("eDEqEAEFCCetCAD",
		"Pret\0ty \0when \0 \0they \0catch \0the \0light.\n\0 \0 \0 \0");
	"$FG,GREEN$(It's easy to be dark and profound all the time in arrogance.  "
	"Sometimes, I search for self-evident goodness.)\n$FG$";
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
