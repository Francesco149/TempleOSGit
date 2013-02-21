//7 has graphics





/* <1> <1> (image) */




U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.814;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,18:26 Try other translations.\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,12005] Numbers,11:1 Read the whole chapter.\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,29345] 1 Kings,22:1 Read the whole chapter.\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  while (!ScanKey) {
	Play("W13sC!!AC!AeABqAeFAetDF2GeG3FqG!2G",
		"$CL$Oh, \0 \0my \0 \0 \0 \0Gosh!\n\0 \0 \0 \0 \0You \0 \0 \0lied!\n\0$FG,GREEN$God talking...\n$FG$\0");
	Play("3sC!!AC!AeABqAeFAetDF2GeG3FqG!2G",
		"$FG,RED$Just \0 \0 \0 \0 \0 \0joshed.\n\0 \0 \0Shhh. \0 \0 \0 \0Con\0fide.\n\0 \0");
	Spaces(40);
	PutSprite(__BIN_1);
	Play("3eDCqD2G!3etE2G3G!qD!DeGAqD",
		"\nI \0am \0God.\n\0 \0I \0 \0can \0lie.\n\0 \0 \0 \0 \0");
	Play("eDCqD2G!3etE2G3G!qD!DeGAqD",
		"I \0am \0God.\n\0 \0I \0 \0can \0lie.$FG$\n\0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
