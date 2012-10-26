/*

  You "Adam include" this because you want the wallpaper routine to stay in 
mem even if this task is killed.

  <CTRL-T> to see the hidden text needed for graphic elements.

  <CTRL-R> to add a sprite to a document.

*/



//Fishothos

/* <1> <1> (image) */


/* <2> <2> (image) */


/* <3> <3> (image) */


//Sea Horse
/* <4> <4> (image) */



/* <5> <5> (image) */



/* <6> <6> (image) */


//Clown Fish

/* <7> <7> (image) */


/* <8> <8> (image) */


/* <9> <9> (image) */




#define TYPES_OF_CRITTERS	3
#define FRAMES_PER_CRITTER	4

U8 *imgs[TYPES_OF_CRITTERS][FRAMES_PER_CRITTER]=
{{__BIN_1,__BIN_2,__BIN_3,__BIN_2},{__BIN_4,__BIN_5,__BIN_6,__BIN_5},{__BIN_7,
__BIN_8,__BIN_9,__BIN_8}};

#define NUM_CRITTERS	16
class Critter
{
  I64 x,y,dx,dy,type;
  F64 t_offset;
} wall_crits[NUM_CRITTERS];

U0 (*old_wall_paper)(CTask *task);

U0 MyWallPaper(CTask *task)
{
  I64 i,j,x,y;
  CDC *dc=DCAlias(gr_dc,task);
  Critter *c=wall_crits;
 
  task->text_attr=CYAN<<4+WHITE;

  dc->color=YELLOW;
  for (i=0;i<GR_HEIGHT;i+=20) {
    j=16*Tri(tNP*10,20);
    GrLine(dc,0,GR_HEIGHT-i-j,GR_HEIGHT-i-j,GR_HEIGHT);
  }
  for (i=0;i<NUM_CRITTERS;i++,c++) {
    j=(tNP*4+c->t_offset)%FRAMES_PER_CRITTER;
    x=c->x>>16%GR_WIDTH;
    if (x<0) x+=GR_WIDTH;
    y=c->y>>16%GR_HEIGHT;
    if (y<0) y+=GR_HEIGHT;
    if (c->dx<0) {
      dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
      GrSetSymmetry(dc,x,y-1,x,y+1);
    } else
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    CSpritePlot3(dc,x,y,0,imgs[c->type][j]);
    c->x+=c->dx;  c->y+=c->dy;
  }
  DCDel(dc);

//Uncomment the following if you wish.
//old_wall_paper(task);
}

U0 WallInit()
{
  I64 i;
  Critter *c=wall_crits;
  old_wall_paper=fp_wall_paper;
  for (i=0;i<NUM_CRITTERS;i++,c++) {
    c->x=(RandU16%GR_WIDTH)<<16;
    c->y=(RandU16%GR_HEIGHT)<<16;
    c->type=RandU16%TYPES_OF_CRITTERS;
    c->dx=RandI16;
    if (c->dx<0)
      c->dx-=0x4000;
    else
      c->dx+=0x4000;
    c->dy=RandI16;
    c->t_offset=Rand*FRAMES_PER_CRITTER;
  }
  fp_wall_paper=&MyWallPaper;
}

WallInit;
