#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x000000B5
#define __BIN_1 "\x02\x00\x08\x01\x00\x00\x00\x17\x00\x00\x00\x00\x17\x00\x00\x00\x57\x65\x6C\x63\x6F\x6D\x65\x20\x54\x6F\x20\x4C\x6F\x73\x65\x74\x68\x6F\x73\x00\x05\x48\x00\x00\x00\x13\x00\x00\x00\x48\x00\x00\x00\xFC\xFF\xFF\xFF\x05\x48\x00\x00\x00\x13\x00\x00\x00\x58\x00\x00\x00\xF3\xFF\xFF\xFF\x05\x48\x00\x00\x00\x12\x00\x00\x00\x38\x00\x00\x00\xF2\xFF\xFF\xFF\x05\x38\x00\x00\x00\xF2\xFF\xFF\xFF\x48\x00\x00\x00\xFC\xFF\xFF\xFF\x05\x48\x00\x00\x00\xFC\xFF\xFF\xFF\x58\x00\x00\x00\xF3\xFF\xFF\xFF\x05\x58\x00\x00\x00\xF3\xFF\xFF\xFF\x38\x00\x00\x00\xF2\xFF\xFF\xFF\x02\x0C\x14\x42\x00\x00\x00\x02\x00\x00\x00\x14\x39\x00\x00\x00\xF3\xFF\xFF\xFF\x02\x0E\x14\x4E\x00\x00\x00\xFD\xFF\xFF\xFF\x02\x01\x14\x4C\x00\x00\x00\xF7\xFF\xFF\xFF\x00"

/*

Graphics can be placed into src code
files.  The actual data is stored in the
file after the text area.  <CTRL-R> was used
to generate the graphic.  A picture $PI$
entry was created by <CTRL-R>.  Press <CTRL-T> to
toggle to plain text mode and see
how it is encoded.  You can place any text
you want in the tag quotes.  The text doesn't matter,
but the num does matter.  The num identifies
the picture and can't be changed.

You make use of a picture in a program by entering
"insert binary", $IB$ cmds by hand after pressing
<CTRL-T> or using <CTRL-R> and
selecting "Pointer to GrSprite Data".
They behave like string constants when compiled.
That is, they are ptrs to data.

GrSprite1.CPZ uses the persistent graphic dc, gr_dc2,
while the other demo's use gr_dc which must
be redrawn each update of the screen (by the
window manager task).  This demo is simpler
because you do not need a window update routine.

*/


/* <1> <1> (image) */




U0 GrSprite1()
{
  GrDC *dc=GrDCAlias(gr_dc2,Fs);
  I64 i;
  "$CL$";
  for (i=0;i<GR_WIDTH>>2;i++) {
    GrDCClear;
    GrSpritePlot3(dc,i,GR_HEIGHT>>1,0,__BIN_1);
    Sleep(3);
  }
  "Image size:%d\n",__BIN_1_SIZE;
  "Image type:%d\n",__BIN_1_TYPE;
  GrDCDel(dc);
  PressAKey;
  GrDCClear;
}

GrSprite1;
