//5 has graphics



/* <1> <1> (image) */


F64 t0=Beat;
U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0,a_x,a_y;
  if (dt<8) {
    a_y=pi/2;
    a_x=Wrap(pi*dt/4,0);
  } else if (dt<32) {
    a_x=0;
    a_y=Wrap(pi*dt/16,0);
    if (pi/2<=a_y<=3*pi/2) a_y=-a_y;
  } else if (dt<44) {
    a_y=0;
    a_x=0;
  } else {
    a_x=0;
    a_y=Wrap(pi*dt/8+pi/2,0);
    if (pi/2<=a_y<=3*pi/2) a_y=-a_y;
  }
  dc->flags|=DCF_TRANSFORMATION;
  Mat4x4RotX(dc->r,a_x);
  Mat4x4RotY(dc->r,a_y);
  dc->x=325;
  dc->y=35;
  Sprite3(dc,0,0,0,__BIN_1);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("4qGeAAqBAGBhA",
	    "The \0Cre\0 \0a\0tor \0on \0His \0throne,\n\0");
      Play("4qGeAAqBAGBhA",
	    "Believe \0or \0 \0not, \0we're \0not \0a\0lone.\n\0");
      Play("5qC4eBAqGAGFhE",
	    "God \0wat\0ches \0al\0ways \0o\0ver \0us.\n\0");
      Play("5qC4eBAqGAGFhA",
	    "Is \0there \0 \0some\0thing \0to \0dis\0cuss?\n\0");
      Play("5qE4eBGqA5sE4G5E4G5qE4etB5FEqDF",
	    "God, \0 \0 \0 \0won't \0 \0You \0 \0"
	    "talk \0 \0to \0me?\n\0 \0 \0");
      Play("5E4eBGqA5sE4G5E4G5qE4etB5FEqDF",
	    "God, \0 \0 \0 \0won't \0 \0You \0 \0"
	    "talk \0 \0to \0me?\n\0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
