//0 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("3sCGCGeEC2qG3eEAqAeFsAGetGGFqD");
	Play("sCGCGeEC2qG3eEAqAeFsAGe.GsFqD");
	Play("sABeBqCAeGsDA2eG3GetG2G3AeDEqD");
	Play("sABeBqCAeGsDA2eG3GetG2G3AeDEqD");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
