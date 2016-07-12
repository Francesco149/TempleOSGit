//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("3eCCDDqEeCsDDqCEB2G");
	Play("3eCsCCeDDqEeCsDDqCEB2G");
	Play("3eDGsEFEFe.EsAeEsB2G3eDE2G3GFG2qG");
	Play("3eDGsEFEFe.EsAeEsB2G3eDE2G3GFG2qG");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
