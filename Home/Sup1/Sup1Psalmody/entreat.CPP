//0 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("M3/4");
	Play("3qACDM2/4",
		"Piz\0za's \0good.\n\0");
	Play("eEsCEeBDqDG");
	Play("sGFeGDECDB2G");
	Play("3qDEhC");
	Play("M3/4");
	Play("qACDM2/4",
		"$RED$God \0is \0good.$FG$\n\0");
	Play("eEsCEeBDqDG");
	Play("sGFeGDECDB2G");
	Play("3qDEhC");
	Play("e.FsAeGsACeBBGsBG",
		" \0 \0 \0 \0 \0 \0 \0I \0 \0like \0");
	Play("eFADBBFsCGEC",
		"pork.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("e.FsAeGsACeBBGsBG",
		" \0 \0 \0 \0 \0 \0 \0$RED$You're \0 \0a \0");
	Play("eFADBBFsCECE",
		"dork.\n$FG$\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
