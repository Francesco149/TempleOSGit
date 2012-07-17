//7 has words
U0 Song()
{
  TaskStruct *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  task=PopUpViewStr(
    "[Numbers,6:25] Numbers,6:25\n"
    "[Psalms,31:16] Psalms,31:16\n"
    "[Psalms,67:1] Psalms,67:1\n"
    "[Psalms,80:3] Psalms,80:3\n"
    "[Psalms,80:19] Psalms,80:19\n"
    "[Psalms,119:135] Psalms,119:135\n"
    "[Daniel,9:17] Daniel,9:17\n"
    );
  TaskWaitIdle(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  while (!ScanKey) {
	Play("W12qG3eCB2hGqG3eCB2qG3A",
		"Umm... \0God?\n\0 \0 \0Let \0Your \0 \0 \0 \0");
	Play("CeBA2G3DqEeDCBA2qG3A",
		"face \0shine \0on \0down\n\0on \0us, \0Your \0 \0luck\0y \0 \0peo\0");
	Play("eBA2h.G3qE!!CAF",
		" \0ple.\n\0 \0Fath\0e\0r \0a\0");
	Play("FD!A!eEDqE!!CAF",
		"bove,\n\0 \0 \0I \0 \0feel \0 \0 \0Your \0");
	Play("FD!A!eED",
		"love.\n\0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
