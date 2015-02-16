











		/* <1> <1> (image) */











U0 RotateTank()
{
  CDC *dc=DCAlias;
  DocClear;
  DCAllocDepthBuf(dc);
  dc->flags|=DCF_TRANSFORMATION;
  WinHorz(TEXT_COLS/2-12,TEXT_COLS/2+12);
  WinVert(TEXT_ROWS/2-12,TEXT_ROWS/2+12);
  try {
    while (!ScanChar) {
      DCDepthBufRst(dc);
      GrIdentEqu(dc->r);
      GrRotYEqu(dc->r,0.25*Sin(tS/10*2*pi)+pi/2);
      GrRotXEqu(dc->r,0.4*Sin(tS/5*2*pi));
      GrRotZEqu(dc->r,0.25*Sin(tS/10*2*pi)-pi/2);
      GrScaleEqu(dc->r,0.25+Saw(tS,5));
      GrSetRotMat(dc,dc->r);//Not needed--Sets scale for pen-width

      dc->x=Fs->pix_width/2;
      dc->y=Fs->pix_height/2;
	//Offsets the image Z so that no parts are clipped by
	//the conceptual plane of the screen.
      dc->z=GR_Z_ALL;
 
      Sprite3(dc,0,0,0,__BIN_1);
      WinMgrSync;
      DCFill;
    }
  } catch
    PutExcept;

  DCFill;
  DCDel(dc);
}

RotateTank;
