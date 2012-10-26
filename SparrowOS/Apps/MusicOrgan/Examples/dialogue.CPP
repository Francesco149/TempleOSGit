//9 has graphics



/* <1> <1> (image) */






F64 t0=tMBeat;
U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=tMBeat-t0,a_x,a_y;
  if (dt<8) {
    a_y=pi/2;
    a_x=Unwrap(pi*dt/4,0);
  } else if (dt<32) {
    a_x=0;
    a_y=Unwrap(pi*dt/16,0);
    if (pi/2<=a_y<=3*pi/2) a_y=-a_y;
  } else if (dt<44) {
    a_y=0;
    a_x=0;
  } else {
    a_x=0;
    a_y=Unwrap(pi*dt/8+pi/2,0);
    if (pi/2<=a_y<=3*pi/2) a_y=-a_y;
  }
  dc->flags|=DCF_TRANSFORMATION;
  GrRotXEqu(dc->r,a_x);
  GrRotYEqu(dc->r,a_y);
  dc->x=325;
  dc->y=35;
  CSpritePlot3(dc,0,0,0,__BIN_1);
}

U0 Song()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	t0=tMBeat;
	Play("W12qG3eAAqBA2G3BhA",
		"The \0Cre\0 \0a\0tor \0on \0His \0thone,\n\0");
	Play("2qG3eAAqBA2G3BhA",
		"Believe \0or \0 \0not, \0we're \0not \0a\0lone.\n\0");
	Play("qCeBA2qG3A2GFhE",
		"God \0wat\0ches \0al\0ways \0o\0ver \0us.\n\0");
	Play("3qCeBA2qG3A2GF3hA",
		"Is \0there \0 \0some\0thing \0to \0dis\0cuss?\n\0");
	Play("qEeB2G3qAsE2G!!3E2G3qEetBFEqDF",
		"God, \0 \0 \0 \0won't \0 \0You \0 \0talk \0 \0to \0me?\n\0 \0 \0");
	Play("EeB2G3qAsE2G!!3E2G3qEetBFEqDF",
		"God, \0 \0 \0 \0won't \0 \0You \0 \0talk \0 \0to \0me?\n\0 \0 \0");
  }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
