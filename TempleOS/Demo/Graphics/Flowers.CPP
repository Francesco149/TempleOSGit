class Stem
{
  Stem *next,*sub;
  I64 x,y;
  I64 flower_size;
};

class Seed
{
  Seed *next;
  Stem *sub;
  I64 x,y;
  I64 height,type;
  Bool bloom;
} *seed_root;


F64 grow_speed=500.0,adjusted_grow_speed;

Bool GrowProbability(F64 prob)
{
  return prob*adjusted_grow_speed>ToF64(RandU32)/MAX_U32;
}
 


/* <1> <1> (image) */


I64 GrowStem(Stem *temps,I64 parent_x,I64 parent_y,I64 x0,I64 y0,Bool bloom)
{
  I64 x,y,cnt=0,result=parent_y,height;
  Stem *temps1,*temps2=temps;
  while (temps) {
    cnt++;
    x=x0;
    y=y0;
    temps->x+=x;
    temps->y+=y;
    if (GrowProbability(0.40/(Abs(temps->y-parent_y)`.5+10.0))) {
      temps->y++;
      y++;
    }
    if (GrowProbability(0.15/(Abs(temps->x-parent_x)`.5+10.0))) {
      if (2*SignI64(temps->x-parent_x)+SignI64(RandI16)<0) {
	temps->x--;
	x--;
      } else {
	temps->x++;
	x++;
      }
    }

    if (temps->y>result)
      result=temps->y;

    if (temps1=temps->sub)
      height=GrowStem(temps1,temps->x,temps->y,x,y,bloom);
    else {
      if (!temps->flower_size && GrowProbability(0.003)) {
	temps->sub=temps1=CAlloc(sizeof(Stem));
	temps1->x=temps->x;
	height=temps1->y=temps->y+1;
      } else {
	height=0;
	if (!temps->next && cnt==1 && bloom && GrowProbability(0.01) && temps->flower_size<10)
	  temps->flower_size++;
      }
    }
    if (height>result)
      result=height;

    temps=temps->next;
  }
  if (temps2 && !temps2->flower_size && GrowProbability(0.001/(cnt*cnt))) {
    temps1=CAlloc(sizeof(Stem));
    temps1->next=temps2->next;
    temps2->next=temps1;
    temps1->x=temps2->x+SignI64(RandI16);
    temps1->y=temps2->y+1;
  }
  return result;
}

U0 GrowSeeds(Seed *tempe,I64,I64 h)
{
  while (tempe) {
    if (tempe->height<h)
      tempe->height=GrowStem(tempe->sub,tempe->x,tempe->y,0,0,tempe->height>0.75*h);
    tempe=tempe->next;
  }
}

U0 DrawStem(CDC *dc,Seed *tempe,Stem *temps,Stem *parent,I64 w,I64 h)
{
  I64 i;
  dc->color=GREEN;
  dc->pen_width=2;
  while (temps) {
  //We use [C:/TempleOS/Adam/Gr/GrPrimatives.CPP.Z,507] GrLine3() because it has pen_width
    if (parent) {
      dc->color=GREEN;
      dc->pen_width=2;
      GrLine3(dc,temps->x+w/2,h-temps->y,0,parent->x+w/2,h-parent->y,0);
    }
    DrawStem(dc,tempe,temps->sub,temps,w,h);
    if (temps->flower_size) {
      for (i=0;i<temps->flower_size;i++) {
	switch (tempe->type) {
	  case 0:
	    if (i&1)
	      dc->color=RED;
	    else
	      dc->color=YELLOW;
	    break;
	  case 1:
	    if (i&1)
	      dc->color=PURPLE;
	    else
	      dc->color=WHITE;
	    break;
	  case 2:
	    if (i&1)
	      dc->color=BLUE;
	    else
	      dc->color=PURPLE;
	    break;
	  case 3:
	    if (i&1)
	      dc->color=RED;
	    else
	      dc->color=WHITE;
	    break;
	}
	dc->pen_width=2;
	GrCircle3(dc,temps->x+w/2,h-temps->y,0,i+1);
      }
      }
    temps=temps->next;
  }
}

U0 DrawSeeds(CDC *dc,Seed *tempe,I64 w,I64 h)
{
  Stem *temps;
  while (tempe) {
    SpritePlot3(dc,tempe->x+w/2,h-tempe->y,0,__BIN_1);
    //We use [C:/TempleOS/Adam/Gr/GrPrimatives.CPP.Z,507] GrLine3() because it has pen_width
    if (temps=tempe->sub) {
      dc->color=GREEN;
      dc->pen_width=2;
      GrLine3(dc,temps->x+w/2,h-temps->y,0,tempe->x+w/2,h-tempe->y,0);
      DrawStem(dc,tempe,temps,temps,w,h);
    }
    tempe=tempe->next;
  }
}

U0 DrawIt(CTask *task, CDC *dc)
{
  adjusted_grow_speed=grow_speed/win_actual_refresh;
  GrowSeeds(seed_root,task->win_pixel_width,task->win_pixel_height);
  DrawSeeds(dc,seed_root,task->win_pixel_width,task->win_pixel_height);
}

U0 StemsDel(Stem *temps)
{
  Stem *temps1;
  while (temps) {
    temps1=temps->next;
    StemsDel(temps->sub);
    Free(temps);
    temps=temps1;
  }
}

U0 SeedsDel(Seed *tempe)
{
  Seed *tempe1;
  while (tempe) {
    tempe1=tempe->next;
    StemsDel(tempe->sub);
    Free(tempe);
    tempe=tempe1;
  }
}

Seed *SeedNew()
{
  Seed *result=CAlloc(sizeof(Seed));
  result->sub=CAlloc(sizeof(Stem));
  result->type=RandU16&3;
  result->x=result->sub->x=RandI32%(Fs->win_pixel_width/3);
  return result;
}

U0 InitFlowers()
{
  I64 i;
  Seed **tempe=&seed_root;
  SeedsDel(seed_root);
  seed_root=NULL;
  for (i=0;i<5;i++) {
    *tempe=SeedNew;
    tempe=&(*tempe)->next;
  }
}

U0 Flowers()
{
  I64 ch;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;

  seed_root=NULL;
  DocClear;
  try
    do {
      //Hopefully draw_it will not happen during Init()
      WinMgrSync;

      InitFlowers;
      ch=GetChar(NULL,FALSE);
    } while (ch!=CH_SHIFT_ESC && ch!=CH_ESC);
  catch
    CatchAll;
  SettingsPop;
  SeedsDel(seed_root);
}

Flowers;
