//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13qAeCDqCBAeCDqEB");
	Play("AeDCqBBAeCBhA");
	Play("eFAqEetG!EG!sF!D!FD!qEsG!!DGDqGetBDD");
	Play("eFAqEetG!EG!sF!D!FD!qEsG!!DGDqGetBDD");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
