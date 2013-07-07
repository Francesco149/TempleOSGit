//9 has graphics;











	   /* <1> <1> (image) */







F64 t0=tMBeat;
U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=tMBeat-t0;
  dc->flags|=DCF_TRANSFORMATION;
  DCAllocDepthBuf(dc);
  dc->x=320;
  dc->y=27;
  dc->z=GR_Z_ALL;
  GrRotXEqu(dc->r,0.140*pi);
  GrRotZEqu(dc->r,0.250*pi*Sin(pi*dt));
  GrRotYEqu(dc->r,0.230*pi);
  if (dt>16)
    GrRotYEqu(dc->r,0.5*Sin(pi/2*dt));
  SpritePlot3(dc,0,0,0,__BIN_1);
}

U0 Song()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	t0=tMBeat;
	"$FG,GREEN$To God...$FG$\n";
	Play("3qGBAsEDEDqCGBC",
		"Fa\0ther, \0 \0what \0 \0 \0 \0You \0been \0do\0ing?\n\0");
	Play("GBAsEDEDqCGBC",
		"Watch\0-it!  \0 \0Or, \0 \0 \0 \0they'll \0be \0su\0ing.\n\0");
	Play("eGDDBGBqBeEAEDFE2qG",
		"They \0don't \0real\0ly \0un\0der\0stand\n\0What \0 \0You're \0 \0up \0 \0to.\n\0");
	Play("3eGDDBGBqBeEAEDFE2qG",
		"High \0a\0bove \0our \0ways,\n\0 \0 \0If \0 \0they \0 \0on\0ly \0knew.\n\0");
  }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
