//7 has graphics

/* <1> <1> (image) */

/* <2> <2> (image) */




F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  Sprite3(dc,20,8,0,__BIN_1);
  if (8<dt<8.5) {
    Sprite3(dc,20,24,0,__BIN_2);
    Snd((dt-8)*100);
  }
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
      Play("3eBCsAGAGeAAetGAAsBBBBeCDqDeEA",
	    "Now, \0I \0su\0 \0u\0 \0ure \0 \0 \0 \0"
	    "won't \0do\0o\0o\0o \0that \0a\0gain!\n\0 \0 \0");
      Play("BCsAGAGeAAetGAAsBBBBeCDqDeEA",
	    "Now, \0I \0su\0 \0u\0 \0ure \0 \0 \0 \0"
	    "won't \0do\0o\0o\0o \0that \0a\0gain!\n\0 \0 \0");
      Play("2qG3etAAGBB2G3eEFCDqFeAGqC",
	    " \0 \0 \0 \0 \0 \0 \0Learn\0ing \0from \0 \0"
	    "my \0 \0mis\0takes.\n\0");
      Play("2G3etAAGBB2G3eEFCDqFeAGqC",
	    " \0 \0 \0 \0 \0 \0 \0Learn\0ing \0from \0 \0"
	    "my \0 \0mis\0takes.\n\0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
