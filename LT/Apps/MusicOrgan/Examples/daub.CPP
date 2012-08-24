//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	"$FG,GREEN$God talking...\n$FG$";
	Play("W12eEDA!1G2GG!1qG2CFD!eDG",
		"Let \0me \0dry \0your \0tears.\n\0 \0 \0 \0 \0 \0 \0 \0");
	Play("EDA!1G2GG!1qG2CFD!eDG",
		"Let \0you \0dra\0w \0near.\n\0 \0 \0 \0 \0 \0 \0 \0");
	Play("qGA!AeGGqGFEE",
		"There.\n\0 \0 \0On \0your \0way.\n\0 \0 \0 \0");
	Play("GA!AeGGqGFEE",
		"There.\n\0 \0 \0On \0your \0way.\n\0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
