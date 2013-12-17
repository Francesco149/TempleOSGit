/*
This demo uses gr_dc2, so you must add a
draw_it routine because gr_dc2 is not
persistent.  (It gets erased each update by
the system.)
*/








      /* <1> <1> (image) */







CDC *dc;

U0 DrawIt(CTask *,CDC *)
{
//We can change the pen width because it was not
//define in <CTRL-R>.
  dc->pen_width=2;
  DCDepthBufRst(dc);
  SpritePlot3(dc,0,0,0,__BIN_1);
}

U0 SpritePlot3D()
{
  F64 theta=0,phi=0;
  dc=DCAlias(gr_dc2,Fs);

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Preempt(OFF); //We don't want update during matrix change
  dc->x=200;
  dc->y=200;
  dc->z=GR_Z_ALL;
  dc->flags|=DCF_TRANSFORMATION;
  DCAllocDepthBuf(dc);
  while (!ScanChar) {
    GrIdentEqu(dc->r);
    GrRotXEqu(dc->r,phi);
    GrRotZEqu(dc->r,theta);
    GrSetRotMat(dc,dc->r);

    Sleep(40);

    theta+=2*pi/100;
    phi+=2*pi/130;
  }
  SettingsPop;
  DCDel(dc);
}

SpritePlot3D;
