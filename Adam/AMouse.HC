#help_index "Graphics/Misc;Mouse/Ptr"

/* <1> <1> (image) */

gr.empty_sprite=__BIN_1;

/* <2> <2> (image) */

/* <3> <3> (image) */

public U0 DrawGrabMs(CDC *dc,I64 x,I64 y,Bool closed)
{//This is a callback. See [C:/Demo/Graphics/Grid.HC.Z,1].
  dc->flags&=~(DCF_TRANSFORMATION|DCF_SYMMETRY);
  dc->color=ROP_EQU;
  if (closed)
    Sprite3(dc,x,y,0,__BIN_2);
  else
    Sprite3(dc,x,y,0,__BIN_3);
}
gr.fp_draw_grab_ms=&DrawGrabMs;
