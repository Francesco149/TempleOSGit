//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3qADeDEqFetEBAsCBCBeGFetEAD");
      Play("qADeDEqFetEBAsCBCBeGFetEAD");
      Play("qDGetGACsBEBEABABetDFEDCBqG");
      Play("DGetGACsBEBEABABetDFEDCBqG");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
