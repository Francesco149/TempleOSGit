//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.780;
  try {
  while (!ScanKey) {
	Play("M2/4");
	Play("W12eEFE3B2EFE3B");
	Play("2E3BA2sGG3eA2sGGeE3B");
	Play("2qE3B!!sBEBE!qD");
	Play("F!BM4/4");
	Play("sDFD!FeBDsBAB!!A!qF!!sBEBE!qDF!B");
	Play("sDFD!FeBDsBAB!!A!qF");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
