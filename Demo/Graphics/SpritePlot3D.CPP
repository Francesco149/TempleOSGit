/*This uses [C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.dc2.  It must be redrawn 30 fps
by the winmgr task with a Fs->draw_it() callback.
*/








      /* <1> <1> (image) */







CDC *dc;

U0 DrawIt(CTask *,CDC *)
{
  DCDepthBufRst(dc);
  Sprite3(dc,0,0,0,__BIN_1);
}

U0 SpritePlot3D()
{
  F64 theta=0,phi=0;
  dc=DCAlias(gr.dc2,Fs);

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Preempt(OFF); //We don't want update during matrix change, mid draw.
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
