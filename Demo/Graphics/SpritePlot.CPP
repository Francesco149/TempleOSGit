/*Graphics can be placed in src code.
The binary graphic data is stored in the
file after the text area.  <CTRL-r> is used
to generate a sprite.  Press <CTRL-t> to
toggle to plain text mode and see its
text place holder $SP...$.  You can place any text
in the tag quotes or none.  The text doesn't matter,
but the BI=num does.  The num identifies
the sprite and can't be changed by you.

Make use of a sprite by entering "insert binary",
$IB...$ cmds by hand after pressing
<CTRL-t> or using <CTRL-r> and
selecting "Pointer to Sprite".
IB's behave like string consts when compiled.
That is, they are ptrs to data.

SpritePlot.CPP.Z uses the persistent graphic [C:/Kernel/KernelA.HPP.Z,3547] CDC, [C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.dc,
while the other demo's use [C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.dc2 which must
be redrawn at 30fps by the window mgr task.
This demo is simpler because you do not need
a window update routine.
*/









	    /* <1> <1> (image) */









U0 SpritePlot()
{
  CDC *dc=DCAlias;
  I64 i;
  DocClear;
  for (i=0;i<GR_WIDTH>>2;i++) {
    DCFill;
    Sprite3(dc,i,GR_HEIGHT>>1,0,__BIN_1);
    Sleep(3);
  }
  "Image size:%d\n",__BIN_1_SIZE;
  DCDel(dc);
  PressAKey;
  DCFill;
}

SpritePlot;
