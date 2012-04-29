//8 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13sC#C#D#D#C#C#CCC#C#D#D#C#C#CCEEDDC#C#DDEEDDC#C#DD");
	Play("C#C#D#D#C#C#CCC#C#D#D#C#C#CCEEDDC#C#DDEEDDC#C#DD");
	Play("eB!!DetFGAeF!EBE!etE!FEqCeFEDA");
	Play("B!!DetFGAeF!EBE!etE!FEqCeFEDA");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
