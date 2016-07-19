//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3eDM6/8");
      Play("qGeG4qAeBqDeBqAeC");
      Play("qB3eGqG4eBqA3eFqDeD");
      Play("qGeG4qAeBqDeBqAeC");
      Play("qB3eGG4ABq.A3qGeD");
      Play("4sD3G4D3GeGF4qCeDCBD3GF");
      Play("qD4sD3G4D3GeGF4qCeDCBD");
      Play("3GFD");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
