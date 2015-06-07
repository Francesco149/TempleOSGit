//7 has graphics



/* <1> <1> (image) */


F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 tt=Wrap(pi/2*(Beat-t0),0);
  dc->flags|=DCF_TRANSFORMATION;
  Mat4x4RotX(dc->r,0.3*Sin(tt/4));
  if (pi/2<=tt<=3*pi/2)
    tt=pi-tt;
  Mat4x4RotY(dc->r,tt);
  dc->x=325;
  dc->y=35;
  Sprite3(dc,0,0,0,__BIN_1);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  "$WW+H,1$";
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("3etFG2G3sACACDGDGCCCCBDBDDBDBqDG",
	    "I \0 \0don't \0 \0know \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "where \0 \0I'll \0 \0get \0 \0the \0 \0mon\0ey.\n\0");
      Play("etFG2G3sACACDGDGCCCCBDBDDBDBqDG",
	    "I \0 \0don't \0 \0know \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "where \0 \0I'll \0 \0get \0 \0the \0 \0mon\0ey.\n\0");
      Play("eFDqDsAAAAeFFetBGADABeD2G3qD",
	    "$RED$Have \0no \0fear.  \0 \0 \0 \0 \0 \0 \0"
	    "God's \0 \0got \0 \0a \0 \0hold \0on \0you.\n\0");
      Play("eFDqDsAAAAeFFetBGADABeD2G3qD",
	    "Have \0no \0fear.  \0 \0 \0 \0 \0 \0 \0"
	    "God's \0 \0got \0 \0a \0 \0hold \0on \0you.$FG$\n\0");
      "$GREEN$(Semi-joke: credit card 'hold' ;-)\n$FG$";
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
