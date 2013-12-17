/*

  This will add an outline to these
CSprite and print them to the cmd
line.  They can be cut/pasted from
there.

LTGREEN is supposed to be transparent.
To make it transparent, the cut/pasted
CSprite must be edited with <CTRL-R> and
converted to transparent bitmaps.

*/

/* <1> <1> (image) */



/* <2> <2> (image) */



/* <3> <3> (image) */



/* <4> <4> (image) */



/* <5> <5> (image) */



/* <6> <6> (image) */




/* <7> <7> (image) */



/* <8> <8> (image) */



/* <9> <9> (image) */



/* <10> <10> (image) */



/* <11> <11> (image) */



/* <12> <12> (image) */




#define NUM	12

U8 *in_img[NUM]=
{__BIN_1,__BIN_2,__BIN_3,__BIN_4,__BIN_5,__BIN_6,
 __BIN_7,__BIN_8,__BIN_9,__BIN_10,__BIN_11,__BIN_12};

U0 OutLine()
{
  I64 n,minx,maxx,miny,maxy,w,h,i,j,k,c;
  U8 *out_img;
  CDC *dc;
  for (n=0;n<NUM;n++) {
    SpriteExtents(in_img[n],&minx,&maxx,&miny,&maxy);
    w=maxx-minx+1+2;
    h=maxy-miny+1+2;
    dc=DCNew(w,h);
    dc->color=LTGREEN;
    GrRect(dc,0,0,w,h);
    SpritePlot3(dc,-minx+1,-miny+1,0,in_img[n]);

    for (i=0;i<h;i++)
      for (j=0;j<w;j++)
	if (GrPeek(dc,j,i)==LTGREEN) {
	  for (k=0;k<8;k++) {
	    c=GrPeek(dc,j+gr_x_offsets[k],i+gr_y_offsets[k]);
	    if (c!=-1 && c!=BLACK && c!=LTGREEN) {
	      dc->color=BLACK;
	      GrPlot(dc,j,i);
	      break;
	    }
	  }
	  }

    out_img=DC2Sprite(dc);
    NewLine;
    PutSprite(out_img);
    NewLine(4);
 
    DCDel(dc);
    Free(out_img);
  }
}

OutLine;
