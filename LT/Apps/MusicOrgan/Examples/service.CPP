//8 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("M6/8");
	Play("W13eAACADAADACAR");
	Play("AACADAADACAR");
	Play("FG!etFEGA!!B!C!DA!C2qG!3sECEC");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
