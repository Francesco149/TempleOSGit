//7 has words
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,76908] Matthew,20:28\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,79970] Mark,10:45\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,85266] John,13:14\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  while (!ScanKey) {
	"$FG,GREEN$Jesus talking...\n$FG$";
	Play("M6/8");
	Play("W13eAACADAADACAR",
		" \0 \0I \0 \0came \0 \0 \0to \0 \0serve.\n\0 \0 \0");
	Play("AACADAADACAR",
		" \0 \0I \0 \0give \0 \0 \0my \0 \0word.\n\0 \0 \0");
	Play("FG!etFEGA!!B!C!DA!C2qG!3sECEC");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
