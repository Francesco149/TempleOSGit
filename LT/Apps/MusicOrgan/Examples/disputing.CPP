//8 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("M3/4");
	Play("W13eECqAAeECqAA");
	Play("eDCqAAeECqAR");
	Play("M6/8");
	Play("etAB!GqCsGEGE!!etGBDqCsCD!CD");
	Play("etAB!GqCsGEGE!!etGBDqCsCD!CD");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
