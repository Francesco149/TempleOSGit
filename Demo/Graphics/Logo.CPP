









		/*  <1> (image) */











U0 Logo()
{
  CDC *dc=DCAlias;
  dc->flags|=DCF_TRANSFORMATION;
  DocClear;
  while (!ScanChar) {
    dc->x=Fs->pix_width>>1;
    dc->y=Fs->pix_height>>1;

//[E://Export.CPP,88] Wrap() converts an angle to an
//equivalent angle in a range starting
//from [arg2,arg2+pi*2).
    GrIdentEqu(dc->r);
    GrRotYEqu(dc->r,Wrap(tS,-pi/2));
    GrRotXEqu(dc->r,30.0*pi/180);
    GrSetRotMat(dc,dc->r);

    Sprite3(dc,0,0,0,__BIN_1);
//See also [C:/Adam/Gr/GrSpritePlot.CPP.Z,284] SpriteX3B(),
//[C:/Adam/Gr/GrSpritePlot.CPP.Z,291] SpriteY3B(),[C:/Adam/Gr/GrSpritePlot.CPP.Z,298] SpriteZ3B().

    WinMgrSync;
    DCFill(dc);
  }
  DCDel(dc);
}

Logo;
