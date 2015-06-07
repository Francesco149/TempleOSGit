//7 has graphics





	/* <1> <1> (image) */








	/* <2> <2> (image) */




F64 t0=Beat;
U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  CSprite *temps;
  DCDepthBufAlloc(dc);
  dc->x=280;
  dc->y=30;
  dc->z=GR_Z_ALL;
  dc->flags|=DCF_TRANSFORMATION;
  if (dt<16) {
    Mat4x4RotZ(dc->r,(dt+5)*pi/2);
    Mat4x4RotY(dc->r,FullTri(dt,8)*pi/8);
  } else
    Mat4x4RotZ(dc->r,5*pi/2);
  Mat4x4RotX(dc->r,-11*pi/8);
  Mat4x4Scale(dc->r,0.75);
  if (dt<16)
    Sprite3(dc,0,0,0,__BIN_1);
  else if (16<dt<17 || 23<dt<25 || 31<dt) {
    temps=SpriteInterpolate(__BIN_1,__BIN_2,Tri(dt-16,2.0));
    Sprite3(dc,0,0,0,temps);
    Free(temps);
  } else
    Sprite3(dc,0,0,0,__BIN_2);
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
      Play("3qEDeCCetEAAqEetCDCAGFqG",
	    "I \0don't \0know \0 \0 \0 \0 \0 \0 \0 \0"
	    "what \0 \0to \0 \0pray.\n\0");
      Play("EDeCCetEAAqEetCDCAGFqG");
      Play("eGEetFDAqFsGFGFGFGFetBGFqGeDE",
	    "It's \0a \0blah \0 \0 \0day.\n\0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("GEetFDAqFsGFGFGFGFetBGFqGeDE");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
