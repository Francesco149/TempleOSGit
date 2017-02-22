//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("5eEEE4B5DDD4A5CCCDEC4qA");
	Play("5eCEE4A5FEDEEDCDECqE");
	Play("5eG4AqA5Ge.CsCqF4eABAGB5sFG");
	Play("5eG4AqA5Ge.CsCqF4eABA5C4B5sFG");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
