







      /* <1> <1> (image) */









I64 glbl_r[4][4];

U0 DrawIt(CTask *,CDC *dc)
{
  I64 *old_r=dc->r;
  dc->x=200;
  dc->y=200;
  dc->z=GR_Z_ALL;
  dc->flags|=DCF_TRANSFORMATION;
  DCDepthBufAlloc(dc);
  DCMat4x4Set(dc,glbl_r); //This assigns to dc->r and sets r_norm.
  DCDepthBufRst(dc);
  Sprite3(dc,0,0,0,__BIN_1);
  dc->r=old_r;
}

U0 SpritePlot3D()
{
  F64 theta=0,phi=0;

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  while (!ScanChar) {
    Mat4x4IdentEqu(glbl_r);
    Mat4x4RotX(glbl_r,phi);
    Mat4x4RotZ(glbl_r,theta);

    Sleep(40);
    theta+=2*pi/100;
    phi+=2*pi/130;
  }
  SettingsPop;
}

SpritePlot3D;
