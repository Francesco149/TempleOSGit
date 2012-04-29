//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("M9/8");
	Play("W13hDeFhDeFhDC");
	Play("FeDhFeDhFE");
	Play("M4/4");
	Play("qD!!2G3AeBF!FE2qG!!3D!!A");
	Play("D!!2G3AeBF!FE2qG!!3D!!A");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
