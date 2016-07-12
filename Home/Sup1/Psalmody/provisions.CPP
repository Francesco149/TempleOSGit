//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3qDetF2GG3qC2sG3B2G3BqFsGDGDqCF");
      Play("DetF2GG3qC2sG3B2G3BqFsGDGDqCF");
      Play("BeG2GGG3AA2qG3etFDDeC2G3qC");
      Play("BeG2GGG3AA2qG3etFDDeC2G3qC");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
