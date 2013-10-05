/*
Graphics can be placed into src code
files.  The actual data is stored in the
file after the text area.  <CTRL-R> was used
to generate the sprite.  Press <CTRL-T> to
toggle to plain text mode and see
how it is encoded.  You can place any text
you want in the tag quotes.  The text doesn't matter,
but the num does matter.  The num identifies
the sprite and can't be changed.

You make use of a sprite in a program by entering
"insert binary", $IB$ cmds by hand after pressing
<CTRL-T> or using <CTRL-R> and
selecting "Pointer to CSprite Data".
They behave like string consts when compiled.
That is, they are ptrs to data.

SpritePlot.CPP.Z uses the persistent graphic CDC, gr_dc,
while the other demo's use gr_dc2 which must
be redrawn each update of the screen (by the
window mgr task).  This demo is simpler
because you do not need a window update routine.
*/









	    /* <1> <1> (image) */














U0 SpritePlot()
{
  CDC *dc=DCAlias;
  I64 i;
  DocClear;
  for (i=0;i<GR_WIDTH>>2;i++) {
    DCFill;
    SpritePlot3(dc,i,GR_HEIGHT>>1,0,__BIN_1);
    Sleep(3);
  }
  "Image size:%d\n",__BIN_1_SIZE;
  "Image type:%d\n",__BIN_1_TYPE;
  DCDel(dc);
  PressAKey;
  DCFill;
}

SpritePlot;
