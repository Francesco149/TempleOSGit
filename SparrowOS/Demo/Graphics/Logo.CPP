








		   /*  <1> (image) */













U0 Logo()
{
  CDC *dc=DCAlias;
  dc->flags|=DCF_TRANSFORMATION;
  DocClear;
  while (!ScanChar) {
    dc->x=Fs->win_pixel_width>>1;
    dc->y=Fs->win_pixel_height>>1;

//[E:/Export.CPP,71] Wrap() converts an angle to an
//equivalent angle in a range starting
//from [arg2,arg2+pi*2).
    GrIdentEqu(dc->r);
    GrRotYEqu(dc->r,Wrap(tP,-pi/2));
    GrRotXEqu(dc->r,30.0*pi/180);
    GrSetRotMat(dc,dc->r);

    CSpritePlot3(dc,0,0,0,__BIN_1);
//See also [C:/SparrowOS/Adam/Gr/GrSpritePlot.CPP.Z,315] CSpritePlotRotX3b(),
//[C:/SparrowOS/Adam/Gr/GrSpritePlot.CPP.Z,322] CSpritePlotRotY3b(),[C:/SparrowOS/Adam/Gr/GrSpritePlot.CPP.Z,329] CSpritePlotRotZ3b().

    WinMgrSync;
    DCFill(dc);
  }
  DCDel(dc);
}

Logo;
