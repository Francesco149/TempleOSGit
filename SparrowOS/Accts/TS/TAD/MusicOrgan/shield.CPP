//7 has graphics








/* <1> <1> (image) */






U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,94514] Ephesians,6:11\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,76908] Matthew,5:10\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,93323] 2 Corinthians,6:7\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  while (!ScanKey) {
	Play("3qDCDeEGBDetBBFqDeDB",
	"God, \0my \0breast \0plate, \0 \0won't \0be \0put \0to \0shame.\n\0 \0 \0 \0");
	NewLine;
	PutSprite(__BIN_1);
	NewLine(5);
	Play("qDCDeEGBDetBBFqDeDB",
	"God, \0my \0breast \0plate, \0 \0won't \0be \0put \0to \0shame.\n\0 \0 \0 \0");
	NewLine;
	PutSprite(__BIN_1);
	NewLine(5);
	Play("FAetAFDeDFqAetDFBeDFetFDEqD",
	"Mocked \0 \0 \0a \0lit\0tle, \0 \0 \0badge \0 \0of \0 \0 \0cour\0 \0 \0age.\n\0");
	Play("eFAetAFDeDFqAetDFBeDFetFDEqD",
	    "Mocked \0 \0 \0a \0lit\0tle, \0 \0 \0badge \0 \0of \0 \0 \0cour\0 \0 \0age.\n\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
