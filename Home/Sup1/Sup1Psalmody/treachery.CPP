//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.736;
  try {
    while (!ScanKey) {
      Play("3qDe2G3AetGD2G3sBDBDqCeDFetABDeFB"
	    "3qDe2G3AetGD2G3sBDBDqCeDFetABDeFB"
	    "3e2G3GDGqDeEADDqACB3e2G3GDGqDeEADDqACB");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
