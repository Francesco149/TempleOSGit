//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("3eEEEBDDDACCCDECqA");
	Play("eCEEAFEDEEDCDECqE");
	Play("eGAqAGe.CsCqFeABA2G3BsFG");
	Play("eGAqAGe.CsCqFeABACBsFG");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
