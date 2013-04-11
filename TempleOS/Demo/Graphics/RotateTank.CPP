

















		/* <1> <1> (image) */

















U0 RotateTank()
{
  F64 theta,phi,omega;
  CDC *dc=DCAlias;
  DocClear;
  DCAllocDepthBuf(dc);
  dc->flags|=DCF_TRANSFORMATION;
  SetWinHorz(TEXT_COLS/2-12,TEXT_COLS/2+12);
  SetWinVert(TEXT_ROWS/2-12,TEXT_ROWS/2+12);
  try {
    while (!ScanChar) {
      DCDepthBufRst(dc);

      theta=0.25*Sin(tT/10*2*pi)-pi/2;
      phi=0.4*Sin(tT/5*2*pi);
      omega=0.25*Sin(tT/10*2*pi)+pi/2;

      GrIdentEqu(dc->r);
      GrRotYEqu(dc->r,omega);
      GrRotXEqu(dc->r,phi);
      GrRotZEqu(dc->r,theta);
      GrSetRotMat(dc,dc->r);

      dc->x=Fs->win_pixel_width/2;
      dc->y=Fs->win_pixel_height/2;
//This offsets the image Z so that no parts are
//not drawn because they are in front of the screen.
      dc->z=GR_Z_ALL;
 
      SpritePlot3(dc,0,0,0,__BIN_1);
      WinMgrSync;
      DCFill;
    }
  } catch
    CatchAll;

  DCFill;
  DCDel(dc);
}

RotateTank;
