//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 1.820;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13eC2G3DDEDqCeEEGFGFqE");
	Play("sFFeGFEFEqDsCCeDCBqBB");
	Play("etB2GG!3qE!2eG3F!EEetFBA!qFDetF!CC");
	Play("B2GG!3qE!2eG3F!EEetFBA!qFDetF!CC");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
