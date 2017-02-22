//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.824;
  try {
    while (!ScanKey) {
	Play("4etG5FDDCDqFsG4G5eFEFqE4eBqA5eE");
	Play("4etG5FDDCDqFsG4G5eFEFqE4eBqAeB");
	Play("6EFqF5BB6C5A6FD");
	Play("eEFqF5BeBB6qC5A6FD");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
