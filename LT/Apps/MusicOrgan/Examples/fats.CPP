//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 3.306;
  music_stacatto_factor= 0.890;
  try {
  while (!ScanKey) {
	Play("M2/4");
	Play("W13q.BeCwD");
	Play("q.BeChD");
	Play("Cq.EeC");
	Play("wB");
	Play("eBq.BeChB");
	Play("wA");
	Play("!!eEqBM2/4");
	Play("!eCq.F!qEeF!G");
	Play("CCqB!!eEqBM2/4");
	Play("!eCq.F!qEeF!G");
	Play("qCB");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
