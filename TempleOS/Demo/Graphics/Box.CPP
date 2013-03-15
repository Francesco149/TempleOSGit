CDC *dc;

U0 DrawIt(CTask *,CDC *)
{
  dc->pen_width=2;
  dc->color=RED;
  GrLine3(dc,-100,-100,-100, -100, 100,-100);
  GrLine3(dc,-100, 100,-100,	100, 100,-100);
  GrLine3(dc, 100, 100,-100,	100,-100,-100);
  GrLine3(dc, 100,-100,-100, -100,-100,-100);
  GrLine3(dc,-100,-100, 100, -100, 100, 100);
  GrLine3(dc,-100, 100, 100,	100, 100, 100);
  GrLine3(dc, 100, 100, 100,	100,-100, 100);
  GrLine3(dc, 100,-100, 100, -100,-100, 100);
  GrLine3(dc,-100,-100, 100, -100,-100,-100);
  GrLine3(dc,-100, 100, 100, -100, 100,-100);
  GrLine3(dc, 100, 100, 100,	100, 100,-100);
  GrLine3(dc, 100,-100, 100,	100,-100,-100);
}

U0 Box()
{
  F64 theta=0,phi=0,omega=0,s=1,s1=1.05;
  dc=DCAlias(gr_dc,Fs);

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  Preempt(OFF); //We don't want matrix used while we're changing it.

  dc->x=200;
  dc->y=200;
  dc->flags|=DCF_TRANSFORMATION;
  while (!ScanChar) {
    GrIdentEqu(dc->r);
    GrRotZEqu(dc->r,theta);
    GrRotXEqu(dc->r,phi);
    GrRotZEqu(dc->r,omega);
    GrScaleMatEqu(dc->r,s);
    GrSetRotMat(dc,dc->r);

    Sleep(20);
    theta+=2*pi/70;
    phi  +=2*pi/90;
    omega +=2*pi/110;
    s *=s1;
    if ( !(0.2<s<1.4) ) s1=1/s1;
  }
  SettingsPop;
  DCDel(dc);
}

Box;
