//7 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.9;
  try {
  while (!ScanKey) {
	Play("3e.GsGe.GsFe.EsG4e.CsDe.EsEe.EsDqCe.CsB");
	Play("e.AsAe.AsBe.CsBe.CsA3e.G4sA3e.GsEqGe.GsG");
	Play("e.GsGe.GsFe.EsG4e.CsDe.EsEe.EsDqCC");
	Play("DDCBh.CqR");
	Play("3(Ge.G");
	Play("sFe.EsG4e.CsDhEq.CeR");
	Play("q(Ae.A");
	Play("sBe.CsBe.CsA3hGq.EeR");
	Play("q(Ge.G");
	Play("sFe.EsG4e.CsDhEqCC");
	Play("DDCBh.CqR");
	Play("!etEEGqCsDCDCFCFCqAetCDBBD3G4F!CF");
	Play("!EEGqCsDCDCFCFCqAetCDBBD3G4F!CF");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
