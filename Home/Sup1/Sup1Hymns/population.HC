//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.780;
  try {
    while (!ScanKey) {
      Play("5eCD4B5C4AFFF5C4F5E4FAF5sEEeD");
      Play("5F4FFF5qCeF4F5F4FFF5qCeF4F");
      Play("5qGsE4A5E4AqB5etDCF4qG5DsGDGDetCRC");
      Play("5qGsE4A5E4AqB5etDCF4qG5DsGDGDetCRC");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
