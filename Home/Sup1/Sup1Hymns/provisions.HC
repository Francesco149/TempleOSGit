//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("5qDetF4GG5qC4sGBGB5qFsGDGDqCF");
      Play("5DetF4GG5qC4sGBGB5qFsGDGDqCF");
      Play("4B5eG4GGGAAqG5etFDDeC4G5qC");
      Play("4B5eG4GGGAAqG5etFDDeC4G5qC");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
