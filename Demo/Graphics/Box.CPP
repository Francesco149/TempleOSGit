CDC *dc;

U0 DrawIt(CTask *,CDC *)
{//This technique is weird, but clever.  In Box() we
//created an alias of the system [C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.dc2.
  dc->pen_width=2;
  dc->color=RED;
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
}

U0 Box()
{
  F64 theta=0,phi=0,omega=0,s=1,s1=1.05;
  dc=DCAlias(gr.dc2,Fs);

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  Preempt; //We don't want matrix used while we're changing it.

  dc->x=200;
  dc->y=200;
  dc->flags|=DCF_TRANSFORMATION;
  while (!ScanChar) {
    Mat4x4IdentEqu(dc->r);
    Mat4x4RotZ(dc->r,theta);
    Mat4x4RotX(dc->r,phi);
    Mat4x4RotZ(dc->r,omega);
    Mat4x4Scale(dc->r,s);
    DCMat4x4Set(dc,dc->r);

    Sleep(20);
    theta+=2*pi/70;
    phi+=2*pi/90;
    omega+=2*pi/110;
    s*=s1;
    if ( !(0.2<s<1.4) ) s1=1/s1;
  }
  SettingsPop;
  DCDel(dc);
}

Box;
