//Set snap to 4 and width to 4
//if you edit this map.

//Don't forget to change the
//starting pos.


/* <1> <1> (image) */









/* <2> <2> (image) */



/* <3> <3> (image) */


/* <4> <4> (image) */	//This is a template you can copy and use to make a tile.




/* <5> <5> (image) */


/* <6> <6> (image) */


/* <7> <7> (image) */


/* <8> <8> (image) */



 
/* <9> <9> (image) */

/* <10> <10> (image) */

/* <11> <11> (image) */





//These are indexed by color #.
//See [C:/Kernel/Adam1a.HPP.Z,3166] COLORS.

U8 *tiles1[16]=
{NULL,__BIN_7 ,__BIN_5 ,NULL,
 __BIN_6 ,NULL,NULL,NULL,
 NULL,NULL,NULL,NULL,
 NULL,NULL,NULL,NULL};

U8 *tiles2[16]=
{NULL,__BIN_8 ,__BIN_5 ,NULL,
 __BIN_6 ,NULL,NULL,NULL,
 NULL,NULL,NULL,NULL,
 NULL,NULL,NULL,NULL};



#define SCREEN_SCALE	24
#define SCREEN_WIDTH	24
#define SCREEN_HEIGHT	24
I64 screen_x,screen_y;

#define MAP_SCALE	4
I64 map_width,map_height;
U8 *map=NULL;

I64 man_x,man_y,man_dx,man_dy;
Bool man_attack;
F64 man_attack_t0;

#define NUM_MONSTERS	16
class Monster
{
  I64 x,y,dx,dy;
  Bool dead,pad[7];
} monsters[NUM_MONSTERS];

#define LOS_SCALE	4

Bool LOSPlot(U8 *,I64 x,I64 y,I64)
{
  if (!map[(y/LOS_SCALE)*map_width+(x/LOS_SCALE)])
    return FALSE;
  else
    return TRUE;
}

Bool LOS(I64 x1,I64 y1,I64 x2,I64 y2)
{ //Line of sight
  return Line(NULL,x1*LOS_SCALE+LOS_SCALE/2,y1*LOS_SCALE+LOS_SCALE/2,0,
	       x2*LOS_SCALE+LOS_SCALE/2,y2*LOS_SCALE+LOS_SCALE/2,0,&LOSPlot);
}

U0 DrawIt(CTask *task,CDC *dc)
{
  CDC *dc_t=DCAlias(gr_dc2,task);
  I64 i,x,y,xx,yy,x1,y1,z1,color;
  CD3I32 poly[4];
  U8 **_tiles;

  if (Blink(5))
    _tiles=tiles1;
  else
    _tiles=tiles2;

  GrRotXEqu(dc_t->r,60*2*pi/360);
  GrRotZEqu(dc_t->r,15*2*pi/360);
  GrSetRotMat(dc_t,dc_t->r);
  dc_t->x=task->pix_width/2;
  dc_t->y=task->pix_height/2;
  dc_t->flags|=DCF_TRANSFORMATION;

  //You could make it much more efficient
  //if you did it like [C:/Demo/Games/BigGuns.CPP.Z,1]
  //with a [C:/Kernel/Adam1a.HPP.Z,3220] CDC.

  for (y=-SCREEN_HEIGHT/2;y<SCREEN_HEIGHT/2;y++) {
    yy=y+screen_y;
    if (0<=yy<map_height)
      for (x=-SCREEN_WIDTH/2;x<SCREEN_WIDTH/2;x++) {
	xx=x+screen_x;
	if (0<=xx<map_width) {
	  if ((color=map[yy*map_width+xx]) &&
	      LOS(xx,yy,man_x,man_y)) {
	    if (_tiles[color]) {
	      x1=x*SCREEN_SCALE;
	      y1=y*SCREEN_SCALE;
	      z1=0;
	      GrTransform(dc_t,&x1,&y1,&z1);
	      Sprite3(dc,x1,y1,z1,_tiles[color]);
	    } else { //If no tile defined, do solid color.
	      poly[0].x=x*SCREEN_SCALE;
	      poly[0].y=y*SCREEN_SCALE;
	      poly[0].z=0;
	      poly[1].x=(x+1)*SCREEN_SCALE-1;
	      poly[1].y=y*SCREEN_SCALE;
	      poly[1].z=0;
	      poly[2].x=(x+1)*SCREEN_SCALE;
	      poly[2].y=(y+1)*SCREEN_SCALE-1;
	      poly[2].z=0;
	      poly[3].x=x*SCREEN_SCALE-1;
	      poly[3].y=(y+1)*SCREEN_SCALE-1;
	      poly[3].z=0;
	      dc_t->color=color;
	      GrFillPoly3(dc_t,4,poly);
	    }
	  }
	}
      }
      }

  dc_t->color=LTGRAY;
  for (x=-SCREEN_WIDTH/2;x<=SCREEN_WIDTH/2;x++)
    GrLine3(dc_t,x*SCREEN_SCALE,-SCREEN_HEIGHT/2*SCREEN_SCALE,0,x*SCREEN_SCALE,SCREEN_HEIGHT/2*SCREEN_SCALE-1,0);
  for (y=-SCREEN_HEIGHT/2;y<=SCREEN_HEIGHT/2;y++)
    GrLine3(dc_t,-SCREEN_WIDTH/2*SCREEN_SCALE,y*SCREEN_SCALE,0,SCREEN_WIDTH/2*SCREEN_SCALE,y*SCREEN_SCALE-1,0);

  for (y=-SCREEN_HEIGHT/2;y<SCREEN_HEIGHT/2;y++) {
    yy=y+screen_y;
    if (0<=yy<map_height)
      for (x=-SCREEN_WIDTH/2;x<SCREEN_WIDTH/2;x++) {
	xx=x+screen_x;
	if (0<=xx<map_width) {
	  if (!map[yy*map_width+xx]) {
	    if (yy+1<map_height && LOS(xx,yy+1,man_x,man_y)) {
	      x1=x*SCREEN_SCALE;
	      y1=y*SCREEN_SCALE;
	      z1=0;
	      GrTransform(dc_t,&x1,&y1,&z1);
	      Sprite3(dc,x1,y1,z1,__BIN_10);
	    }
	    if (xx+1<map_width && LOS(xx+1,yy,man_x,man_y)) {
	      x1=x*SCREEN_SCALE;
	      y1=y*SCREEN_SCALE;
	      z1=0;
	      GrTransform(dc_t,&x1,&y1,&z1);
	      Sprite3(dc,x1,y1,z1,__BIN_11);
	    }
	  }
	}
      }
      }

  for (i=0;i<NUM_MONSTERS;i++)
    if (!monsters[i].dead && LOS(monsters[i].x,monsters[i].y,man_x,man_y)) {
      x1=(monsters[i].x-screen_x)*SCREEN_SCALE+SCREEN_SCALE/2;
      y1=(monsters[i].y-screen_y)*SCREEN_SCALE+SCREEN_SCALE/2;
      z1=0;
      GrTransform(dc_t,&x1,&y1,&z1);
      if (monsters[i].dx<0) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	GrSetSymmetry(dc,x1,y1,x1,y1+1);
      } else
	dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
      Sprite3(dc,x1,y1,z1,__BIN_9);
    }

  x1=(man_x-screen_x)*SCREEN_SCALE+SCREEN_SCALE/2;
  y1=(man_y-screen_y)*SCREEN_SCALE+SCREEN_SCALE/2;
  z1=0;
  if (tS-man_attack_t0<0.2) {
    x1+=Tri(tS-man_attack_t0,0.2)*SCREEN_SCALE*man_dx;
    y1+=Tri(tS-man_attack_t0,0.2)*SCREEN_SCALE*man_dy;
  }
  GrTransform(dc_t,&x1,&y1,&z1);
  if (man_dx<0) {
    dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
    GrSetSymmetry(dc,x1,y1,x1,y1+1);
  } else
    dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);

  if (tS-man_attack_t0<0.2)
    Sprite3(dc,x1,y1,z1,__BIN_3);
  else
    Sprite3(dc,x1,y1,z1,__BIN_2);

  DCDel(dc_t);
}


U0 Attack()
{
  I64 i;
  man_attack_t0=tS;
  Noise(100,300,1000);
  for (i=0;i<NUM_MONSTERS;i++) {
    if (!monsters[i].dead &&
	 man_x+man_dx==monsters[i].x && man_y+man_dy==monsters[i].y)
      monsters[i].dead=TRUE;
  }
}


U0 Init()
{
  I64 i,x,y,minx,maxx,miny,maxy;
  CDC *dc;
  SpriteExtents(__BIN_1,&minx,&maxx,&miny,&maxy);
  map_width =(maxx-minx+1)/MAP_SCALE;
  map_height=(maxy-miny+1)/MAP_SCALE;
  Free(map);
  map=MAlloc(map_width*map_height*sizeof(U8));
  dc=DCNew(map_width*MAP_SCALE,map_height*MAP_SCALE);
  Sprite3(dc,-minx,-miny,0,__BIN_1);
  for (y=0;y<map_height;y++)
    for (x=0;x<map_width;x++)
       map[y*map_width+x]=GrPeek(dc,x*MAP_SCALE,y*MAP_SCALE);
  DCDel(dc);

  for (i=0;i<NUM_MONSTERS;i++) {
    monsters[i].dead=FALSE;
    monsters[i].dx=0;
    monsters[i].dy=0;
    do {
      monsters[i].x=RandU64%(map_width-2)+1;
      monsters[i].y=RandU64%(map_height-2)+1;
    } while (!map[(monsters[i].y)*map_width+monsters[i].x]);
  }
  man_attack_t0=tS;
  man_attack=FALSE;
  man_x=0;
  man_y=4;
  man_dx=0;
  man_dy=0;
  screen_x=0;
  screen_y=0;
}

U0 CleanUp()
{
  Free(map);
  map=NULL;
}

U0 AnimateTask(I64)
{
  I64 i,x,y,dx,dy;
  while (TRUE) {
    for (i=0;i<NUM_MONSTERS;i++)
      if (!monsters[i].dead) {
	dx=RandU16%3-1;
	dy=RandU16%3-1;
	x=monsters[i].x+dx;
	y=monsters[i].y+dy;
	if (0<=x<=map_width && 0<=y<=map_height &&
	    map[y*map_width+x]) {
	  monsters[i].x=x;
	  monsters[i].y=y;
	  monsters[i].dx=dx;
	  monsters[i].dy=dy;
	}
      }
    Sleep(1000);
  }
}

U0 DunGen()
{
  I64 ch,sc;

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,'\n');"
  "  Up(,,SC_CURSOR_UP);"
  "  Down(,,SC_CURSOR_DOWN);"
  "  Left(,,SC_CURSOR_LEFT);"
  "  Right(,,SC_CURSOR_RIGHT);"
  "  Attack(,CH_SPACE);"
      "}"
      );

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=BLACK<<4+WHITE;
  Preempt(OFF);
  WinMax;
  WordStat;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      switch (GetMsg(&ch,&sc,1<<MSG_KEY_DOWN|1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (ch) {
	    case '\n':
	      Init;
	      break;
	    case CH_SPACE:
	      man_attack=TRUE;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto dg_done;
	    case 0:
	      switch (sc.u8[0]) {
		case SC_CURSOR_RIGHT:
		  if (man_attack) {
		    man_dx=1;
		    man_dy=0;
		    Attack;
		  } else
		    if (man_x+1<map_width && map[man_y*map_width+(man_x+1)]==RED) {
		    man_x++;
		    if (man_x-screen_x>SCREEN_WIDTH/2-3) {
		      screen_x+=SCREEN_WIDTH/2;
		      if (screen_x+SCREEN_WIDTH/2>map_width)
			screen_x=map_width-SCREEN_WIDTH/2;
		    }
		  }
		  break;
		case SC_CURSOR_LEFT:
		  if (man_attack) {
		    man_dx=-1;
		    man_dy=0;
		    Attack;
		  } else
		    if (man_x-1>=0 && map[man_y*map_width+(man_x-1)]==RED) {
		    man_x--;
		    if (man_x-screen_x<-SCREEN_WIDTH/2+3) {
		      screen_x-=SCREEN_WIDTH/2;
		      if (screen_x-SCREEN_WIDTH/2<0)
			screen_x=SCREEN_WIDTH/2;
		    }
		  }
		  break;
		case SC_CURSOR_UP:
		  if (man_attack) {
		    man_dx=0;
		    man_dy=-1;
		    Attack;
		  } else
		    if (man_y-1>=0 && map[(man_y-1)*map_width+man_x]==RED) {
		    man_y--;
		    if (man_y-screen_y<-SCREEN_HEIGHT/2+3) {
		      screen_y-=SCREEN_HEIGHT/2;
		      if (screen_y-SCREEN_HEIGHT/2<0)
			screen_y=SCREEN_HEIGHT/2;
		    }
		  }
		  break;
		case SC_CURSOR_DOWN:
		  if (man_attack) {
		    man_dx=0;
		    man_dy=1;
		    Attack;
		  } else
		    if (man_y+1<map_height && map[(man_y+1)*map_width+man_x]==RED) {
		    man_y++;
		    if (man_y-screen_y>SCREEN_HEIGHT/2-3) {
		      screen_y+=SCREEN_HEIGHT/2;
		      if (screen_y+SCREEN_HEIGHT/2>map_height)
			screen_y=map_height-SCREEN_HEIGHT/2;
		    }
		  }
		  break;
	      }
	  }
	  break;
	case MSG_KEY_UP:
	  if (ch==CH_SPACE)
	    man_attack=FALSE;
	  break;
      }
    }
dg_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
  CleanUp;
}

DunGen;
