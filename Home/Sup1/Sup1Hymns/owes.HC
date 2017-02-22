//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("4qA5DeDEqFetE4BA5sC4B5C4B5eGFetE4A5D");
      Play("4qA5DeDEqFetE4BA5sC4B5C4B5eGFetE4A5D");
      Play("5qDGetG4A5C4sB5E4B5E4ABAB5etDFEDC4B5qG");
      Play("5DGetG4A5C4sB5E4B5E4ABAB5etDFEDC4B5qG");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
