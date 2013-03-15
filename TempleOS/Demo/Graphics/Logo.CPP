








		   /*  <1> (image) */













U0 Logo()
{
  CDC *dc=DCAlias;
  dc->flags|=DCF_TRANSFORMATION;
  DocClear;
  while (!ScanChar) {
    dc->x=Fs->win_pixel_width>>1;
    dc->y=Fs->win_pixel_height>>1;

//[E://Export.CPP,71] Wrap() converts an angle to an
//equivalent angle in a range starting
//from [arg2,arg2+pi*2).
    GrIdentEqu(dc->r);
    GrRotYEqu(dc->r,Wrap(tT,-pi/2));
    GrRotXEqu(dc->r,30.0*pi/180);
    GrSetRotMat(dc,dc->r);

    SpritePlot3(dc,0,0,0,__BIN_1);
//See also [C:/TempleOS/Adam/Gr/GrSpritePlot.CPP.Z,318] SpritePlotRotX3b(),
//[C:/TempleOS/Adam/Gr/GrSpritePlot.CPP.Z,325] SpritePlotRotY3b(),[C:/TempleOS/Adam/Gr/GrSpritePlot.CPP.Z,332] SpritePlotRotZ3b().

    WinMgrSync;
    DCFill(dc);
  }
  DCDel(dc);
}

Logo;
