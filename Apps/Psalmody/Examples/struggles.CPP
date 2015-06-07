//9 has graphics





/* <1> <1> (image) */

F64 t0=Beat;
#define CENTER_X	200

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  dc->flags|=DCF_SYMMETRY;
  DCSymmetrySet(dc,CENTER_X,0,CENTER_X,1);
  Sprite3(dc,CENTER_X-40,75+dt,0,__BIN_1);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("3sFGFGetDCFqD2G3DGDetDFB",
	    "God, \0 \0 \0 \0please \0 \0 \0help \0me.\n\0 \0 \0 \0 \0 \0 \0");
      Play("sFGFGetDCFqD2G3DGDetDFB",
	    "God, \0 \0 \0 \0please \0 \0 \0help \0me.\n\0 \0 \0 \0 \0 \0 \0");
      Play("qACADetF2G3B2sG3F2G3FetCDFsDFDF",
	    "Thy \0will \0be \0done.\n\0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0");
      Play("qACADetF2G3B2sG3F2G3FetCDFsDFDF",
	    "Thy \0will \0be \0done.\n\0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
