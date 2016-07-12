//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("3eFDEBCAB2G3EEFG4AA3qG");
	Play("eFDEBCAB2G3EEsFFeG4B3GqE");
	Play("2sG3F2G3FeADCAqFeFsEDetFD2G3sBDBD2qG");
	Play("sG3F2G3FeADCAqFeFsEDetFD2G3sBDBDqE");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
