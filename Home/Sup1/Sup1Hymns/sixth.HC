//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("5eCCDDqEeCsDDqCE4BG");
	Play("5eCsCCeDDqEeCsDDqCE4BG");
	Play("5eDGsEFEFe.E4sA5eE4sBG5eDE4G5GFG4qG");
	Play("5eDGsEFEFe.E4sA5eE4sBG5eDE4G5GFG4qG");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
