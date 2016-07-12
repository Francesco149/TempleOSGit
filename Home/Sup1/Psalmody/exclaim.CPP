//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.736;
  try {
    while (!ScanKey) {
	Play("3eEEEFGEGG4qA3G4A3F");
	Play("eEsEEeEFqGeGG4qA3GhE");
	Play("eA2sGG3eADqBEsBBeBetC2G3D2sG3ECBeFE");
	Play("A2sGG3eADqBEsBBeBetC2G3D2eG3CqF");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
