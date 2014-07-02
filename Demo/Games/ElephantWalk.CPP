//Press <CTRL-t>.













				/*  <1> (image) */
















//The sprite was created with <CTRL-r>.

U0 ElephantWalk()
{
  I64 sc,
      x=Fs->pix_width>>1,y=Fs->pix_height>>1, //Fs is current [C:/Kernel/Adam1a.HPP.Z,3213] CTask struct.
      x_last=x;
  DocClear;
  WordStat;
  WinMax;
  WinBorder;
  while (TRUE) {
    DCFill;
    if (x>x_last) {
       GrSetSymmetry(,x,y,x,y-1);
       gr.dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
    } else
       gr.dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    x_last=x;
    SpriteZ3B(gr.dc,x,y,0,__BIN_1,Sin(8*tS)/4);//Use <CTRL-r> "pointer to sprite"
    switch (GetKey(&sc)) {
      case 0:
	switch (sc.u8[0]) {//Scan code [C:/Doc/CharOverview.TXT.Z,1] lowest byte
	  case SC_CURSOR_UP:
	    y-=32;
	    if (y<0)
	      y=Fs->pix_height;
	    break;
	  case SC_CURSOR_DOWN:
	    y+=32;
	    if (y>Fs->pix_height)
	      y=0;
	    break;
	  case SC_CURSOR_LEFT:
	    x-=32;
	    if (x<0)
	      x=Fs->pix_width;
	    break;
	  case SC_CURSOR_RIGHT:
	    x+=32;
	    if (x>Fs->pix_width)
	      x=0;
	    break;
	}
	break;
      case CH_ESC:
      case CH_SHIFT_ESC:
	goto ew_done;
    }
  }
ew_done:
  DCFill;
}

ElephantWalk;
