//8 has graphics
   /*  <1> (image) */







U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      Play("3etCFC2qG3DsGDGD2qG3C2sG3D2G3D2G3D2G3D");
      Play("etCFC2qG3DsGDGD2qG3C2sG3D2G3D2G3D2G3D");
      Sprite(__BIN_1);
      "$CM,0,8$";
      Play("2qG3EAEADFC",
	    " \0Off \0 \0to \0 \0yon\0der \0star.\n\0");
      Play("2G3EAEADFC",
	    " \0Damn! \0 \0It's \0 \0ver\0y \0far.\n\0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
