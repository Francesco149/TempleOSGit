//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("5e.C4sG5qEe.F4sB5eCCsFG4eBAAetA5REeEsC4B");
      Play("5e.C4sG5qEe.F4sB5eCCsFG4eBAAetA5REeEsC4B");
      Play("5eGE4qG5sCDCDe.EsGqCsDCDC4e.A5sReGE");
      Play("5GE4qG5sCDCDe.EsGqCsDCDC4e.A5sReGE");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
