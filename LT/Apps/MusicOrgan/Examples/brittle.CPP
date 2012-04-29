//8 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13etCCCqCBAetCCCqCBD");
	Play("etCCCqCBAetEDCqCBE");
	Play("etFFFqFEFetFFFqFEF");
	Play("W1!3etGEEeFDqC!F!F!GED");
	Play("!etGEEeFDqC!F!F!GED");
	Play("etBCG2sG!GGG3EEE!EetC!!A!!CqD!etE!EE!sFCF!C2etG3F!B");
	Play("BCG2sG!GGG3EEE!EetC!!A!!CqD!etE!EE!sFCF!C2etG3F!B");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
