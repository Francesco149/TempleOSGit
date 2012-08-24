//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 3.966;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13qABeCCqABAAR");
	Play("ABeCCqABAAR");
	Play("DeCCCCqCDCCR");
	Play("eAAqBeCCqAeBBqAAR");
	Play("GD!G!!eGA2G3FqF!!eDRqB");
	Play("GD!G!!eGA2G3FqF!!eDRqB");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
