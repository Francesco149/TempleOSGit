

//This is a test image we are going
//to draw and determine the extents of.



/* <1> <1> (image) */




U0 Extents()
{
  CDC *dc=DCAlias;
  I64 min_x,max_x,min_y,max_y;

//We want screen, not window coordinates.
  I64 h=Fs->pix_left+Fs->scroll_x,v=Fs->pix_top+Fs->scroll_y;

  DocClear;
  DCFill;

//This sets a flag to record the
//extents of what we draw and inits
//the max's and min's.  Vals are
//screen coordinates.
  DCExtentsInit(dc);

  dc->pen_width=3;
  Sprite3(dc,40,GR_HEIGHT>>1,0,__BIN_1);

  dc->color=LTRED;
  dc->pen_width=1;
//This box is the extents.
  GrLine(dc,dc->min_x-h,dc->min_y-v,dc->max_x-h,dc->min_y-v);
  GrLine(dc,dc->min_x-h,dc->min_y-v,dc->min_x-h,dc->max_y-v);
  GrLine(dc,dc->max_x-h,dc->max_y-v,dc->max_x-h,dc->min_y-v);
  GrLine(dc,dc->max_x-h,dc->max_y-v,dc->min_x-h,dc->max_y-v);

  DCDel(dc);
  PressAKey;
  DCFill;

  SpriteExtents(__BIN_1,&min_x,&max_x,&min_y,&max_y);
  "X: %d to %d\n",min_x,max_x;
  "Y: %d to %d\n",min_y,max_y;
}

Extents;
