//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("3sGDeCGEqAe.EsCDDDDDGeGFGEG");
	Play("sGDeCGEqAe.EsCDDDDDGeGFGEG");
	Play("qBsBFBFqFDetEDEeCDGGqB");
	Play("BsBFBFqFDetEDEeCDGGqB");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
