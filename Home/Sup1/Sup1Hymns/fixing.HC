//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.758;
  try {
    while (!ScanKey) {
      Play("5sDD4eB5C4sAAeBGAF5sCCeDsFFeEsFFEEeDE");
      Play("5sDD4eB5C4sAAeBGAF5sCCeDsFFeEsFFEEeDE");
      Play("5sG4B5G4BetG5EGqFED4GeBA5DG");
      Play("5sG4B5G4BetG5EGqFED4GeBA5DG");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
