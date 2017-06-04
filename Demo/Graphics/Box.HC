I64 glbl_r[4][4];

U0 DrawIt(CTask *,CDC *dc)
{
  I64 *old_r=dc->r;
  dc->thick=2;
  dc->color=RED;
  dc->x=200;
  dc->y=200;
  dc->flags|=DCF_TRANSFORMATION;
  DCMat4x4Set(dc,glbl_r); //This assigns to dc->r and sets r_norm.
  GrLine3(dc,-100,-100,-100, -100, 100,-100);
  GrLine3(dc,-100, 100,-100,  100, 100,-100);
  GrLine3(dc, 100, 100,-100,  100,-100,-100);
  GrLine3(dc, 100,-100,-100, -100,-100,-100);
  GrLine3(dc,-100,-100, 100, -100, 100, 100);
  GrLine3(dc,-100, 100, 100,  100, 100, 100);
  GrLine3(dc, 100, 100, 100,  100,-100, 100);
  GrLine3(dc, 100,-100, 100, -100,-100, 100);
  GrLine3(dc,-100,-100, 100, -100,-100,-100);
  GrLine3(dc,-100, 100, 100, -100, 100,-100);
  GrLine3(dc, 100, 100, 100,  100, 100,-100);
  GrLine3(dc, 100,-100, 100,  100,-100,-100);
  dc->r=old_r;
}

U0 Box()
{
  F64 theta=0,phi=0,omega=0,s=1,s1=1.05;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  while (!ScanChar) {
    Mat4x4IdentEqu(glbl_r);
    Mat4x4RotZ(glbl_r,theta);
    Mat4x4RotX(glbl_r,phi);
    Mat4x4RotZ(glbl_r,omega);
    Mat4x4Scale(glbl_r,s);

    Sleep(20);
    theta+=2*pi/70;
    phi+=2*pi/90;
    omega+=2*pi/110;
    s*=s1;
    if ( !(0.2<s<1.4) ) s1=1/s1;
  }
  SettingsPop;
}

Box;
