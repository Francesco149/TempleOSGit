/*[C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.dc is the persistent layer.  You can access
it directly but two tasks accessing it simultaneously
with no [C:/Adam/Gr/GrDC.CPP.Z,167] DCAlias() will screw-up color and stuff.
*/

U0 SunMoon()
{
  I64 i,x,y,radius;

  WinMax;	//Maximize window
  DocClear;	//Clear command-line text
  WinBorder;	//No Border
  WordStat;	//WordStat Off
  Fs->text_attr=BLACK<<+WHITE;	//Current [C:/Kernel/Adam1a.HPP.Z,3213] CTask is Fs segment register.

  for (i=0;i<0x8000;i+=0x200) {
    //Sun
    x=0.2*GR_WIDTH; y=0.5*GR_HEIGHT; radius=120;
    gr.dc->color=DKGRAY;
    GrCircle(gr.dc,x,y,radius);
    gr.dc->color=ROPF_DITHER+RED<<16+YELLOW;
    GrFloodFill(gr.dc,x,y);

    //Moon
    x=0.8*GR_WIDTH-i/0x200; y=0.5*GR_HEIGHT; radius=75;
    gr.dc->color=DKGRAY;
    GrCircle(gr.dc,x,y,radius);
    //<CTRL-ALT-a> for the "pi" character.
    GrCircle(gr.dc,x+0.5*radius,y,1.2*radius,,pi-.31*pi,0.62*pi);
    gr.dc->color=ROPF_PROBABILITY_DITHER+LTGRAY<<16+BLACK;
    gr.dc->dither_probability_u16=i;	//U16 can be [0,65535]
    GrFloodFill(gr.dc,x+0.5*radius,y);
    gr.dc->dither_probability_u16=2*i;	//U16 can be [0,65535]
    GrFloodFill(gr.dc,x-0.75*radius,y);

    WinMgrSync; //Typically 60 fps or 16.6mS
    DCFill;
  }

  Fs->text_attr=WHITE<<4+BLUE;
}

SunMoon;
