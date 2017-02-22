//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("5qEeCDE4B5CD4qA5D4eBq.G");
	Play("5qEeCFqEeCD4qA5D4hB");
	Play("5qGEeFGqD4B5eDG4qA5eGF");
	Play("qGEeFGqD4B5eDG4qA5eFG");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
