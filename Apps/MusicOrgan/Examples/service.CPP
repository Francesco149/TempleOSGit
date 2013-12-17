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
    "[C:/Misc/Bible.TXT.Z,76908] Matthew,20:28\n"
    "[C:/Misc/Bible.TXT.Z,79970] Mark,10:45\n"
    "[C:/Misc/Bible.TXT.Z,85266] John,13:14\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  while (!ScanKey) {
	"$GREEN$Jesus talking...\n$FG$";
	Play("M6/8");
	Play("3eAACADAADACAR",
		" \0 \0I \0 \0came \0 \0 \0to \0 \0serve.\n\0 \0 \0");
	Play("AACADAADACAR",
		" \0 \0I \0 \0give \0 \0 \0my \0 \0word.\n\0 \0 \0");
	Play("FGetFEGABCDAC2qG3sECEC");
  }
  } catch
    PutExcept;
  Snd(0);
}

Song;
