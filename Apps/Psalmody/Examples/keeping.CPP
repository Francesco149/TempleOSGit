//8 has graphics


/* <1> <1> (image) */


F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  if (dt>26) {
    dc->flags|=DCF_TRANSFORMATION;
    GrRotZEqu(dc->r,0.2);
    GrScaleEqu(dc->r,(15+Sin(pi*(dt-26)))/15);
    dc->x=330;
    dc->y=35;
    Sprite3(dc,0,0,0,__BIN_1);
  }
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.151;
  music.stacatto_factor= 0.780;
  try {
  while (!ScanKey) {
	t0=Beat;
	Play("3eCsBBeA#BCDCBCsBBeA#BCDCB");
	Play("etDCBeCBCBABetDCBeCBCBAB");
	Play("A2G3sBGBGFCFCBFBFetFGAsFGFGqGD",
		"I \0have \0kept \0 \0the \0 \0Lord's \0 \0day, \0 \0 \0 \0 \0 \0 \0 \0 \0all \0 \0 \0 \0day!\n\0 \0");
	Play("eA2G3sBGBGFCFCBFBFetFGAsFGFGqGD",
		"O\0kay, \0 \0God... \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0where's \0 \0 \0is \0 \0my \0 \0pay?\n\0 \0");
  }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
