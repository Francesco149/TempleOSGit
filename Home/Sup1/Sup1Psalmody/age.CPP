//5 has graphics


	/* <1> <1> (image) */



	/* <2> <2> (image) */


	/* <3> <3> (image) */

F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0,tt=Saw(dt,8);
  U8 *temps;
  if (dt<8.0)
    temps=SpriteInterpolate(tt,__BIN_1,__BIN_2);
  else if (dt<16.0)
    temps=SpriteInterpolate(tt,__BIN_2,__BIN_3);
  else
    temps=SpriteInterpolate(0,__BIN_3,__BIN_3);
  Sprite3(dc,220,33,0,temps);
  Free(temps);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      t0=Beat;
	Play("3eEBDCqB2G3eCBqAeCBqA",
		"Do \0not \0whine \0at \0age.\n\0 \0 \0 \0 \0 \0 \0 \0");
	Play("eEBDCqB2G3eCBqAeCBqA",
		"Do \0not \0whine \0at \0age.\n\0 \0 \0 \0 \0 \0 \0 \0");
	Play("eD2sG3EeAsGDqDsDEeBe.FsBF2G3F2G3qEG");
	Play("eD2sG3EeAsGDqDsDEeBe.FsBF2G3F2G3qEG");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
