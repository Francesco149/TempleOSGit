//5 has graphics
/* <1> <1> (image) */

/* <2> <2> (image) */
F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0,tt;
  U8 *tmps;
  if (dt<16.0)
    tt=0;
  else if (dt<18.0)
    tt=((dt-16)/2)%1;
  else
    tt=1.0;
  tmps=SpriteInterpolate(tt,__BIN_1,__BIN_2);
  Sprite3(dc,180,40,0,tmps);
  Free(tmps);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=3.0;
  try {
    while (!ScanKey) {
      t0=Beat;
      DocClear;
      Play("5qF4eRRqAeRA5qGD4etBA5CeC4B",
	    "God \0 \0 \0is \0 \0a \0good \0God.  \0"
	    "(Thank \0 \0 \0God!!)\n\0 \0");
      Play("5qF4eRRqAeRA5qGD4etBA5CeC4B",
	    "God \0 \0 \0is \0 \0a \0good \0God.  \0"
	    "(Thank \0 \0 \0God!!)\n\0 \0");
      Play("5qGsCECEeC4GqR5FeEDqEF",
	    " \0 \0 \0 \0 \0 \0 \0 \0He \0is \0a \0good \0God.\n\0");
      Play("5GsCECEeC4GqR5FeEDqEF",
	    " \0 \0 \0 \0 \0 \0 \0 \0He \0is \0a \0good \0God.\n\0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
