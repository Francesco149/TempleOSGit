//8 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.736;
  try {
  while (!ScanKey) {
	Play("W13sBBBBqCq.BeAqBh.A");
	Play("M2/4");
	Play("eECqAM4/4");
	Play("sBBBBqCq.BeAqBh.A");
	Play("M2/4");
	Play("eDDqDM4/4");
	Play("DeEDqG!CeF2G3qG!!sFGFG!!eD!C");
	Play("qDeEDqG!CeF2G3qG!!sFGFG!!eD!C");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
