//Uses [C:/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.

RegSetDftEntry("TempleOS/EagleDive","F64 best_score=9999;\n");
RegExeBranch("TempleOS/EagleDive");

//Keep these power of two so shift is used instead of multiply
//to index arrays.
#define MAP_WIDTH		1024
#define MAP_HEIGHT		1024

#define MAP_SCALE		150
#define DISPLAY_SCALE		100
#define CTRLS_SCALE		0.05

//I think I did these so the heads-up showed intelligable numbers.
//Scaling is a mess.
#define COORDINATE_SCALE	256
#define COORDINATE_BITS		8

#define WATER_ELEVATION		15
#define ROCK_ELEVATION		45
#define SNOW_ELEVATION		55

//Too big makes off-screen draws take place.
#define MAX_PANEL_SIZE		16





















			/* <1> <1> (image) */

























			/* <2> <2> (image) */


























			/* <3> <3> (image) */



































			/* <4> <4> (image) */





















































class Obj
{
  Obj *next,*last;
  CD3I64 p;
  U8 *img;
  Bool fish;
};

class Panel
{//Polygon or Obj
  Panel *next;
  CD3I32 *pts;
  I64 cnt;
  U16 update,num_sides;
  CColorROPU32 color;
  Obj *next_obj,*last_obj;
} *panel_head,*panels[MAP_HEIGHT][MAP_WIDTH];

I64 critical_section_flag;
I16 elevations[MAP_HEIGHT][MAP_WIDTH];
CD3 normals[MAP_HEIGHT][MAP_WIDTH];

class MPCtrl {
  I64 init_not_done_flags,update_not_done_flags,app_not_done_flags;
  I64 strip_width[MP_MAX_PROCESSORS];
  Bool app_done;
} mp;

F64 game_t0,game_tf,pitch,roll,heading,phi,speed;
Bool invert_pitch,rolled_over;
I64 strip_height,x,y,z,fish_left;

U0 WrapAngles()
{
  phi=Wrap(phi);
  pitch=Wrap(-phi-pi/2);
  if (Abs(pitch)>pi/2) {
    invert_pitch=TRUE;
    pitch=Wrap(pi-pitch);
  } else
    invert_pitch=FALSE;
  roll=Wrap(roll);
  if (invert_pitch ^^ -pi/2<=roll<pi/2)
    rolled_over=FALSE;
  else
    rolled_over=TRUE;
  heading=Wrap(heading,0);
}

U0 EDTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  I64 zz;
  Mat4x4MulXYZ(dc->r,x,y,z);
  *z=zz=-*z;
  if (zz>0) {
    *x = dc->x + *x * DISPLAY_SCALE/zz; //Foreshortening
    *y = dc->y - *y * DISPLAY_SCALE/zz;
  } else {
    *x = dc->x + *x;
    *y = dc->y - *y;
  }
}

U0 CalcNormals()
{/*Find the normal vect with a curl.

i,j and k are the axis unit vectors,
not to be confused with my local index variables.

i  j  k
0  1  dz2
1  0  dz1

Normal:  dz1*i + dz2*j - k
*/
  I64 i,j;
  for (j=0;j<MAP_HEIGHT-1;j++) {
    for (i=0;i<MAP_WIDTH-1;i++) {
      normals[j][i].x=elevations[j][i+1]-elevations[j][i];
      normals[j][i].y=elevations[j+1][i]-elevations[j][i];
      normals[j][i].z=-1;
      D3Unit(&normals[j][i]);
    }
    MemSet(&normals[j][i],0,sizeof(CD3));
  }
  for (i=0;i<MAP_WIDTH-1;i++)
    MemSet(&normals[j][i],0,sizeof(CD3));
}

Bool TestSameSlope(I64 x,I64 y,I64 w,I64 h)
{
  CD3 p,*s;
  I64 k1,k2;
  if (!(0<=x && x+w<MAP_WIDTH && 0<=y && y+h<MAP_HEIGHT))
    return FALSE;
  s=&normals[y][x];
  for (k2=0;k2<h;k2++)
    for (k1=0;k1<w;k1++)
      if (D3NormSqr(D3Sub(&p,&normals[y+k2][x+k1],s))>.10)
	return FALSE;
  return TRUE;
}

U0 MPDoPanels(CTask *task)
{
  I64 i,j,l,k1,k2,w,h,threshold,lo,hi;
  Bool cont;
  Panel *tempp,*start_ptr=NULL,*end_ptr=NULL;
  CD3I32 *poly;
  Obj *tempo;
  lo=Gs->num*(MAP_HEIGHT-1)/mp_cnt;
  hi=(Gs->num+1)*(MAP_HEIGHT-1)/mp_cnt;
  for (threshold=8;threshold>=1;threshold--)
    for (j=lo;j<hi;j++) {
      for (i=0;i<MAP_WIDTH-1;i++) {
	if (!panels[j][i]) {
	  w=1;
	  h=1;
	  do {
	    cont=FALSE;
	    if (w<MAX_PANEL_SIZE && TestSameSlope(i,j,w+1,h)) {
	      w++;
	      cont=TRUE;
	    }
	    if (h<MAX_PANEL_SIZE && TestSameSlope(i,j,w,h+1)) {
	      h++;
	      cont=TRUE;
	    }
	  } while (cont);
	  if (w>=threshold || h>=threshold) {
	    tempp=CAlloc(sizeof(Panel),task);
	    QueInit(&tempp->next_obj);
	    l=elevations[j][i];
	    if (l<=WATER_ELEVATION*MAP_SCALE &&
		  elevations[j][i+w-1]<=WATER_ELEVATION*MAP_SCALE &&
		  elevations[j+h-1][i]<=WATER_ELEVATION*MAP_SCALE &&
		  elevations[j+h-1][i+w-1]<=WATER_ELEVATION*MAP_SCALE) {
	      tempp->color=BLUE;
	      if (Rand<0.05) {
		tempo=MAlloc(sizeof(Obj),task);
		tempo->p.x=(i+w/2)*MAP_SCALE;
		tempo->p.y=(j+h/2)*MAP_SCALE;
		if (Rand<0.1) {
		  tempo->fish=FALSE;
		  if (RandI16&1)
		    tempo->img=__BIN_2; //Boat
		  else
		    tempo->img=__BIN_3; //Boat
		  tempo->p.z=(WATER_ELEVATION+2)*MAP_SCALE;
		} else {
		  tempo->fish=TRUE;
		  tempo->img=__BIN_1; //Fish
		  tempo->p.z=WATER_ELEVATION*MAP_SCALE;
		}
		QueIns(tempo,tempp->last_obj);
	      }
	    } else {
	      if (l<ROCK_ELEVATION*MAP_SCALE) {
		if (RandI16&1)
		  tempp->color=LTGREEN;
		else
		  tempp->color=GREEN+LTGREEN<<16+ROPF_DITHER;
		if (Rand<0.03) {
		  tempo=MAlloc(sizeof(Obj),task);
		  tempo->p.x=(i+w/2)*MAP_SCALE;
		  tempo->p.y=(j+h/2)*MAP_SCALE;
		  tempo->p.z=l;
		  tempo->img=__BIN_4; //Tree
		  tempo->fish=FALSE;
		  QueIns(tempo,tempp->last_obj);
		}
	      } else if (l<SNOW_ELEVATION*MAP_SCALE) {
		if (!(RandU16&3)) {
		  if (RandI16&1)
		    tempp->color=LTGRAY;
		  else
		    tempp->color=DKGRAY+LTGRAY<<16+ROPF_DITHER;
		} else {
		  if (RandI16&1)
		    tempp->color=LTGREEN;
		  else
		    tempp->color=GREEN+LTGREEN<<16+ROPF_DITHER;
		}
	      } else {
		if (!(RandU16&3)) {
		  if (RandI16&1)
		    tempp->color=WHITE;
		  else
		    tempp->color=LTGRAY;
		} else {
		  if (RandI16&1)
		    tempp->color=LTGRAY+WHITE<<16+ROPF_DITHER;
		  else
		    tempp->color=DKGRAY+LTGRAY<<16+ROPF_DITHER;
		}
	      }
	    }
	    tempp->num_sides=4;
	    poly=tempp->pts=MAlloc(sizeof(CD3I32)*tempp->num_sides,task);
	    poly[0].x=MAP_SCALE*i;
	    poly[0].y=MAP_SCALE*j;
	    poly[0].z=elevations[j][i];
	    poly[1].x=MAP_SCALE*(i+w);
	    poly[1].y=MAP_SCALE*j;
	    poly[1].z=elevations[j][i+w];
	    poly[2].x=MAP_SCALE*(i+w);
	    poly[2].y=MAP_SCALE*(j+h);
	    poly[2].z=elevations[j+h][i+w];
	    poly[3].x=MAP_SCALE*i;
	    poly[3].y=MAP_SCALE*(j+h);
	    poly[3].z=elevations[j+h][i];
	    tempp->next=start_ptr;
	    start_ptr=tempp;
	    if (!end_ptr)
	      end_ptr=tempp;
	    for (k2=0;k2<h;k2++)
	      for (k1=0;k1<w;k1++)
		panels[j+k2][i+k1]=tempp;
	  }
	}
      }
    }
  if (end_ptr) {
    while (LBts(&critical_section_flag,0))
      Yield;
    if (end_ptr)
      end_ptr->next=panel_head;
    panel_head=start_ptr;
    LBtr(&critical_section_flag,0);
  }
  LBtr(&mp.init_not_done_flags,Gs->num);
}

U0 InitElevations()
{
  I64 i,j,l,x,y,xx,yy,x1,y1,x2,y2;
  MemSet(elevations,0,sizeof(elevations));
  for (i=0;i<MAP_WIDTH*MAP_HEIGHT/128;i++) {
    x=RandU32%MAP_WIDTH;
    y=RandU32%MAP_HEIGHT;
    j=1<<(RandU32%6);
    l=0;
    while (j--) {
      if (!l && RandU16<MAX_U16/4)
	l=RandU16%(j+1);
      if (l) {
	x1=ClampI64(x-j,0,MAP_WIDTH-1);
	x2=ClampI64(x+j,0,MAP_WIDTH-1);
	y1=ClampI64(y-j,0,MAP_HEIGHT-1);
	y2=ClampI64(y+j,0,MAP_HEIGHT-1);
	for (yy=y1;yy<y2;yy++)
	  for (xx=x1;xx<x2;xx++)
	    elevations[yy][xx]+=MAP_SCALE/2;
	l--;
      }
    }
  }

  for (j=0;j<MAP_HEIGHT;j++)
    for (i=0;i<MAP_WIDTH;i++)
      if (elevations[j][i]<WATER_ELEVATION*MAP_SCALE)
	elevations[j][i]=WATER_ELEVATION*MAP_SCALE;
}

U0 InitMap()
{/*We make a topographic data structure "elevations[][]"
and convert it to panels. "panels[][]" holds the panels
for each spot.
*/
  I64 i;
  InitElevations;
  MemSet(panels,0,sizeof(panels));
  CalcNormals;
  panel_head=NULL;
  mp.init_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    Spawn(&MPDoPanels,Fs,"Do Panels",i);
  while (mp.init_not_done_flags)
    Sleep(1);
}

F64 claws_down;

#define HAND_X	(1.0-claws_down)*(0.3*w)+claws_down*(0.4*w)
#define HAND_Y	(1.0-claws_down)*(h-125)+claws_down*h




/* <5> <5> (image) */
/* <6> <6> (image) */



U0 ClawDraw(CDC *dc,I64 x1,I64 y1,I64 x2,I64 y2,I64 w,I64 segments,Bool talon)
{
  I64 i,j;
  for (i=0,j=segments;i<segments;i++,j--) {
    dc->pen_width=w;
    dc->color=BLACK;
    GrLine3(dc,
	j*x1/segments+i*x2/segments,
	j*y1/segments+i*y2/segments,0,
	(j-1)*x1/segments+(i+1)*x2/segments,
	(j-1)*y1/segments+(i+1)*y2/segments,0);
    dc->pen_width=w-2;
    dc->color=YELLOW;
    GrLine3(dc,
	j*x1/segments+i*x2/segments,
	j*y1/segments+i*y2/segments,0,
	(j-1)*x1/segments+(i+1)*x2/segments,
	(j-1)*y1/segments+(i+1)*y2/segments,0);
  }
  if (talon) {
    if (y1<y2)
      Sprite3B(dc,x1,y1,0,__BIN_5);
    else
      Sprite3B(dc,x1,y1,0,__BIN_6);
  }
}

U0 ClawsDraw(CTask *task,CDC *dc)
{
  F64 claws_up=1.0-claws_down;
  I64 w=task->pix_width,h=task->pix_height;
  dc->flags|=DCF_SYMMETRY;
  DCSymmetrySet(dc,w>>1,0,w>>1,1);

  ClawDraw(dc,HAND_X-30,HAND_Y-50*claws_up,HAND_X-5,HAND_Y,	22,4,TRUE);
  ClawDraw(dc,HAND_X-10,HAND_Y-60*claws_up,HAND_X,HAND_Y,	22,4,TRUE);
  ClawDraw(dc,HAND_X+10,HAND_Y-60*claws_up,HAND_X,HAND_Y,	22,4,TRUE);
  ClawDraw(dc,HAND_X+30,HAND_Y-50*claws_up,HAND_X+5,HAND_Y,	22,4,TRUE);
  ClawDraw(dc,HAND_X+25,HAND_Y+40*claws_up,HAND_X+5,HAND_Y,	22,4,TRUE);

  ClawDraw(dc,HAND_X,HAND_Y,6*w/20,h,38,5,FALSE);
}

CDC *main_dc;
U0 DrawIt(CTask *task,CDC *dc)
{
  main_dc->flags|=DCF_NO_TRANSPARENTS;
  GrBlot(dc,0,0,main_dc);
  if (claws_down)
    ClawsDraw(task,dc);
}

/*
/* <7> <7> (image) */


















/* <8> <8> (image) */
































Cores render strips that +/- 15%. The cores check the panel map array
and render the panel for each square, marking-it done.

The depth buf is not locked in the graphic routines
so we get some glitches.
*/

I64 update_jiffy_limit;

U0 MPDrawIt(CTask *task,CDC *dc)
{
  I64 j,update=winmgr.updates&65535,strip_width,*s2w,x1,y1,z1,xx,yy,
	xh,yh,zh,yh2,xh2,x1h,y1h,x1wa,y1wa,x1wb,y1wb,
	x3,y3,z3,dd,dd_old,
	cx=task->pix_width>>1,
	cy=task->pix_height>>1;
  Panel reg *tempp;
  Obj	*tempo;
  Bool	w_on_map,h_on_map;

  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);

  //World to screen coordinates
  Mat4x4IdentEqu(dc->r);
  Mat4x4RotZ(dc->r,heading);
  Mat4x4RotX(dc->r,phi);
  Mat4x4RotZ(dc->r,roll);
  DCMat4x4Set(dc,dc->r);

  //Screen to world coordinates

  //This gives us the vects for stepping through the grid in
  //the direction the plane is facing. we step horizontally and vertically
  //and use the reciprocal slope principle
  //y=mx+b and y=(-1/m)x+b are perpendicular.

  s2w=Mat4x4IdentNew;
  Mat4x4RotZ(s2w,-roll);
  Mat4x4RotX(s2w,-phi);
  Mat4x4RotZ(s2w,-heading);

  xh=0;
  yh=0;
  zh=-256;
  Mat4x4MulXYZ(s2w,&xh,&yh,&zh);

  //The layer for core1 is not cleared automatically
  //it is persistent.  I have carefully syncronized to the update
  //cycle initiated by core0 to prevent flicker.

  dc->flags|=DCF_TRANSFORMATION;
  dc->transform=&EDTransform;
  dc->x=cx;
  dc->y=cy;

  //dc->x and the translation part of dc->r are ident in effect,
  //but we use both to show-off.	We could add offsets together and
  //use one or the other.

  x1=-x>>COORDINATE_BITS;
  y1=-y>>COORDINATE_BITS;
  z1=-z>>COORDINATE_BITS;
  Mat4x4MulXYZ(dc->r,&x1,&y1,&z1);
  Mat4x4TranslationEqu(dc->r,x1,y1,z1);

  //This is a refinement.
  if (Abs(phi*180/pi)>90) {
    x3=0;
    y3=-cy;
    z3=0;
    Mat4x4MulXYZ(s2w,&x3,&y3,&z3);
    xx+=x3;
    yy+=y3;
  }

  if (Gs->num&1) {//alt left-right,right-left
    yh2=-yh;
    xh2=-xh;
  } else {
    yh2=yh;
    xh2=xh;
  }

  //Calc starting point.
  x1wa=xx<<8+xh*strip_height>>1/1.3*(Gs->num+1.15);
  y1wa=yy<<8+yh*strip_height>>1/1.3*(Gs->num+1.15);
  x1wb=0;
  y1wb=0;

  xh=-xh; //Back to front to help with depth.
  yh=-yh;

  //Take half steps to cover whole grid.
  xh>>=1;  yh>>=1;
  xh2>>=1; yh2>>=1;
  w_on_map=FALSE;
  dd_old=MAX_I64;
  for (strip_width=0;cnts.jiffies<update_jiffy_limit;strip_width++) {
    x1h=x1wa;
    y1h=y1wa;
    h_on_map=FALSE;
    for (j=0;j<strip_height && cnts.jiffies<update_jiffy_limit;j++) {
      x1=x1h>>8; y1=y1h>>8;
      if (0<=x1<MAP_WIDTH && 0<=y1<MAP_HEIGHT) {
	if ((tempp=panels[y1][x1]) && tempp->update!=update) {
	  tempp->update=update;
	  if (tempp->cnt>8*(1.1-Gs->idle_factor)) {
	    dc->color=tempp->color;
	    tempp->cnt=GrFillPoly3(dc,tempp->num_sides,tempp->pts);
	  } else
	    tempp->cnt++;
	  tempo=tempp->next_obj;
	  while (tempo!=&tempp->next_obj) {
	    Sprite3(dc,tempo->p.x,tempo->p.y,tempo->p.z,tempo->img);
	    tempo=tempo->next;
	  }
	}
	h_on_map=TRUE;
      } else if (h_on_map)
	break;
      x1h+=xh;
      y1h+=yh;
    }
    if (h_on_map)
      w_on_map=TRUE;
    else if (w_on_map) {
      strip_width=MAX_I64;
      break;
    }
    x1wb-=yh2;
    y1wb+=xh2;
    if (strip_width&1) {
      x1wa-=x1wb;
      y1wa-=y1wb;
    } else {
      x1wa+=x1wb;
      y1wa+=y1wb;
    }
    if (!w_on_map) {
      dd=SqrI64(x1wa>>8-MAP_WIDTH>>1)+SqrI64(y1wa>>8-MAP_HEIGHT>>1);
      if (dd>dd_old)
	break;
      dd_old=dd;
    }
  }

  Free(s2w);
  mp.strip_width[Gs->num]=strip_width;
  LBtr(&mp.update_not_done_flags,Gs->num);
}

U0 CoreAPEagleDiveTask(CTask *master_task)
{
  CDC *dc=DCAlias(main_dc,master_task);
  while (!mp.app_done) {
    while (!Bt(&mp.update_not_done_flags,Gs->num) && !mp.app_done)
      Sleep(1);
    if (!mp.app_done)
      MPDrawIt(master_task,dc);
  }

  //We made an alias of this we don't want freed.
  dc->depth_buf=NULL;

  DCDel(dc);
  LBtr(&mp.app_not_done_flags,Gs->num);
}

U0 DrawHorizon(CDC *dc)
{
  I64 x1,y1,z1,x2,y2,z2,xh,yh,zh,*s2w=Mat4x4IdentNew,
	cx=Fs->pix_width>>1,
	cy=Fs->pix_height>>1;
  CD3I32 p[4];
  I32 *old_db=dc->depth_buf;
  dc->depth_buf=NULL;
  CColorROPU32 ground_color;

  if (game_tf && fish_left)
    DCFill(dc,BLACK);
  else if (-pi/4<=Wrap(phi-pi)<pi/4)
    DCFill(dc,LTCYAN);
  else {
    if (z/COORDINATE_SCALE<(WATER_ELEVATION+3)*MAP_SCALE)
      ground_color=BLUE;
    else
      ground_color=LTGREEN;

    Mat4x4IdentEqu(dc->r);
    Mat4x4RotZ(dc->r,heading);
    Mat4x4RotX(dc->r,phi);
    Mat4x4RotZ(dc->r,roll);

    DCMat4x4Set(dc,dc->r);
    dc->flags&=~DCF_TRANSFORMATION;
    dc->transform=&EDTransform;
    dc->x=cx;
    dc->y=cy;

    Mat4x4RotZ(s2w,-roll);
    Mat4x4RotX(s2w,-phi);
    Mat4x4RotZ(s2w,-heading);

    xh=0;
    yh=0;
    zh=-256;
    Mat4x4MulXYZ(s2w,&xh,&yh,&zh);
    Free(s2w);

    x1=xh+yh*32; y1=yh-xh*32; z1=0;
    (*dc->transform)(dc,&x1,&y1,&z1);
    x2=xh-yh*32; y2=yh+xh*32; z2=0;
    (*dc->transform)(dc,&x2,&y2,&z2);
    DCClipLine(dc,&x1,&y1,&x2,&y2);

    MemSet(p,0,sizeof(p));
    if (x2<x1) {
      SwapI64(&x1,&x2);
      SwapI64(&y1,&y2);
    }
    if (!x1 && x2==dc->width-1) {
      p[0].x=0;
      p[0].y=0;
      p[1].x=dc->width-1;
      p[1].y=0;
      p[2].x=dc->width-1;
      p[2].y=y2;
      p[3].x=0;
      p[3].y=y1;
      if (rolled_over)
	dc->color=ground_color;
      else
	dc->color=LTCYAN;
      GrFillPoly3(dc,4,p);
      p[0].y=dc->height-1;
      p[1].y=dc->height-1;
      if (rolled_over)
	dc->color=LTCYAN;
      else
	dc->color=ground_color;
      GrFillPoly3(dc,4,p);
    } else {
      if (y2<y1) {
	SwapI64(&x1,&x2);
	SwapI64(&y1,&y2);
      }
      if (!y1 && y2==dc->height-1) {
	p[0].x=0;
	p[0].y=0;
	p[1].x=0;
	p[1].y=dc->height-1;
	p[2].x=x2;
	p[2].y=dc->height-1;
	p[3].x=x1;
	p[3].y=0;
	if (x1<x2 ^^ rolled_over)
	  dc->color=ground_color;
	else
	  dc->color=LTCYAN;
	GrFillPoly3(dc,4,p);
	p[0].x=dc->width-1;
	p[1].x=dc->width-1;
	if (x1<x2 ^^ rolled_over)
	  dc->color=LTCYAN;
	else
	  dc->color=ground_color;
	GrFillPoly3(dc,4,p);
      } else
	DCFill(dc,LTCYAN); //Not correct.
    }
  }
  dc->depth_buf=old_db;
}

U0 Core0EagleDive()
{
  CDC *dc=DCAlias(main_dc,Fs);
  I64 i,xx,yy,elev,height,cx=Fs->pix_width>>1,cy=Fs->pix_height>>1;
  F64 min_strip_width,tt;

  update_jiffy_limit=cnts.jiffies+JIFFY_FREQ/40;

  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);
  if (0<=xx<MAP_WIDTH && 0<=yy<MAP_HEIGHT)
    elev=elevations[yy][xx];
  else
    elev=MAX_I64;

  height=z/COORDINATE_SCALE-elev;
  if (height<0 && elev>WATER_ELEVATION*MAP_SCALE && !game_tf) {
    music.mute=TRUE;
    Beep;
    game_tf=tS;
    music.mute=FALSE;
  }

  DrawHorizon(dc);

  if (game_tf) {
    tt=game_tf-game_t0;
    if (Blink) {
      dc->color=RED;
      GrPrint(dc,(Fs->pix_width-9*FONT_WIDTH)/2,
	    (Fs->pix_height-FONT_HEIGHT)/2,"Game Over");
    }
  } else {
    DCDepthBufRst(dc);
    mp.update_not_done_flags=1<<mp_cnt-1;
    MPDrawIt(Fs,dc);
    while (mp.update_not_done_flags)
      Sleep(1);

    min_strip_width=MAX_F64;
    for (i=0;i<mp_cnt;i++)
      min_strip_width=Min(min_strip_width,32.0*mp.strip_width[i]/(i+32.0));
    strip_height=ClampI64(
	  strip_height*Clamp(0.25*min_strip_width/strip_height,0.9,1.1),
	  64,1024);

    tt=tS-game_t0;
    if (tt<5.0 && Blink) {
      dc->color=WHITE;
      GrPrint(dc,(Fs->pix_width-13*FONT_WIDTH)/2,
	    (Fs->pix_height-FONT_HEIGHT)/2-4*FONT_HEIGHT,"Catch 10 Fish");
    }
  }

  dc->pen_width=2;
  if (game_tf && fish_left)
    dc->color=WHITE;
  else
    dc->color=BLACK;
  dc->flags&=~DCF_TRANSFORMATION;
  GrLine3(dc,cx+5,cy,0,cx-5,cy,0);
  GrLine3(dc,cx,cy+5,0,cx,cy-5,0);
  if (invert_pitch)
    GrPrint(dc,0,0,"Pitch:%5.1f Roll:%5.1f Heading:%5.1f "
	  "Height:%5d [Core Strip:%3d]",
	  pitch*180/pi,Wrap(roll+pi)*180/pi,Wrap(heading+pi,0)*180/pi,
	  height,strip_height);
  else
    GrPrint(dc,0,0,"Pitch:%5.1f Roll:%5.1f Heading:%5.1f "
	  "Height:%5d [Core Strip:%3d]",
	  pitch*180/pi,roll*180/pi,heading*180/pi,height,strip_height);
  GrPrint(dc,0,FONT_HEIGHT,"Fish Remaining:%d Time:%3.2f Best:%3.2f",
	fish_left,tt,best_score);

  //We made an alias of this we don't want freed.
  dc->depth_buf=NULL;
  DCDel(dc);
  WinMgrSync;
}


Obj *FishFind(I64 x1,I64 y1,I64 *_dd)
{
  I64 dd,best_dd=MAX_I64;
  Obj *res=NULL,*tempo;
  Panel *tempp=panel_head;
  while (tempp) {
    tempo=tempp->next_obj;
    while (tempo!=&tempp->next_obj) {
      if (tempo->fish) {
	dd=SqrI64(tempo->p.x-x1)+SqrI64(tempo->p.y-y1);
	if (dd<best_dd) {
	  best_dd=dd;
	  res=tempo;
	}
      }
      tempo=tempo->next;
    }
    tempp=tempp->next;
  }
  *_dd=best_dd;
  return res;
}

#define ANIMATE_MS	10

U0 AnimateTask(I64)
{//Steadily moves the airplane fwd.
  I64 *s2w,x1,y1,z1,dx,dy,dz,dd;
  F64 t0=tS,ms,theta,d;
  Obj *tempo;
  while (TRUE) {
    ms=1000*(tS-t0);
    t0=tS;
    if (!game_tf) {
//Screen to world coordinates
      s2w=Mat4x4IdentNew;
      Mat4x4RotZ(s2w,-roll);
      Mat4x4RotX(s2w,-phi);
      Mat4x4RotZ(s2w,-heading);

      dx=0;dy=0;dz=1<<16;
      Mat4x4MulXYZ(s2w,&dx,&dy,&dz);
      x-=speed*ms*COORDINATE_SCALE*dx/1<<16;
      y-=speed*ms*COORDINATE_SCALE*dy/1<<16;
      z-=speed*ms*COORDINATE_SCALE*dz/1<<16;
      Free(s2w);
      x1=x/COORDINATE_SCALE; y1=y/COORDINATE_SCALE; z1=z/COORDINATE_SCALE;
      if (z1<(WATER_ELEVATION+3)*MAP_SCALE) {
	if (z1<WATER_ELEVATION*MAP_SCALE) {
	  z=WATER_ELEVATION*MAP_SCALE*COORDINATE_SCALE;
	  if (invert_pitch) {
	    if (pitch<-pi/8)
	      phi+=ms/1000.0*Sign(3*pi/8-phi);
	  } else if (pitch<-pi/8)
	    phi+=ms/1000.0*Sign(-3*pi/8-phi);
	  WrapAngles;
	}
	speed=1.0;
	if (rolled_over || !(tempo=FishFind(x1,y1,&dd)))
	  claws_down=0;
	else {
	  d=Sqrt(dd);
	  x1-=tempo->p.x;
	  y1-=tempo->p.y;
	  theta=ACos((dx*x1+dy*y1)/(d*1<<16));
	  if (theta>0 && d<MAP_SCALE*4)
	    claws_down=Saw(d,MAP_SCALE*4);
	  else
	    claws_down=0.01;
	  if (d<MAP_SCALE*2) {
	    if (!--fish_left) {
	      game_tf=tS;
	      if (game_tf-game_t0<best_score)
		best_score=game_tf-game_t0;
	    }
	    QueRem(tempo);
	    Free(tempo);
	    music.mute=TRUE;
	    Snd(1000); Sleep(200); Snd(0);
	    music.mute=FALSE;
	  }
	}
      } else {
	claws_down=0;
	if (-pi/4<=phi<=pi/4)
	  speed+=0.0005;
	else if (-3*pi/4<=phi<=3*pi/4)
	  speed+=0.0001;
	else
	  speed-=0.0001;
	speed=Clamp(speed+(0.0005-0.0002*Abs(phi)/(pi/4)),0.1,5.0);
      }
    }
    WinMgrSync;
  }
}

U0 MPEnd()
{
  update_jiffy_limit=0;
  mp.app_not_done_flags=1<<mp_cnt-1-1;
  mp.app_done=TRUE;
//Wait for all cores to exit
  while (mp.app_not_done_flags)
    Sleep(1);
}

U0 TaskEndCB()
{
  MPEnd;
  Exit;
}

U0 SongTask(I64)
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3eCGFsDAe.C2sG3eGDqCDeGsGG2qG");
    Play("3eCGFsDAe.C2sG3eGDqCDeGsGG2qG");
    Play("3eGECGCAFCsCBCBe.GsG2qG3B");
    Play("eGECGCAFCsCBCBe.GsG2qG3B");
  }
}

U0 Init()
{
  I64 i,xx,yy;
  main_dc=DCNew(GR_WIDTH,GR_HEIGHT);
  critical_section_flag=0;
  game_tf=0;
  fish_left=10;

  MemSet(&mp,0,sizeof(MPCtrl));
  InitMap;
  DCDepthBufAlloc(main_dc);

  strip_height=128;

  phi	=-90.0*pi/180.0;
  roll	=0;
  heading=0;
  speed =2.5;
  claws_down=0;
  WrapAngles;

  x=MAP_WIDTH>>1 *COORDINATE_SCALE*MAP_SCALE;
  y=MAP_HEIGHT>>1*COORDINATE_SCALE*MAP_SCALE;
  z=64		 *COORDINATE_SCALE*MAP_SCALE;

  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);
  z+=elevations[yy][xx]*COORDINATE_SCALE;

  for (i=1;i<mp_cnt;i++)
    Spawn(&CoreAPEagleDiveTask,Fs,"AP EagleDive",i);
  Fs->task_end_cb=&TaskEndCB;
  game_t0=tS;
}

U0 CleanUp()
{
  Panel *tempp=panel_head,*tempp1;
  MPEnd;
  while (tempp) {
    tempp1=tempp->next;
    QueDel(&tempp->next_obj);
    Free(tempp->pts);
    Free(tempp);
    tempp=tempp1;
  }
  DCDel(main_dc);
}

U0 EagleDive()
{
  I64 ch,sc;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Down(,,SC_CURSOR_UP);"
	"  Up(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  "Initializing...\n";
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  Fs->draw_it=&DrawIt;
  try //in case <CTRL-ALT-c> is pressed.
    do {
      if (ScanKey(&ch,&sc)) {
	switch (ch) {
	  case 0:
	    switch (sc.u8[0]) {
	      start:
		case SC_CURSOR_DOWN:
		  phi-=CTRLS_SCALE*Cos(roll);
		  heading-=CTRLS_SCALE*Sin(roll)*Sin(phi);
		  break;
		case SC_CURSOR_UP:
		  phi+=CTRLS_SCALE*Cos(roll);
		  heading+=CTRLS_SCALE*Sin(roll)*Sin(phi);
		  break;
		case SC_CURSOR_RIGHT:
		  roll+=CTRLS_SCALE;
		  break;
		case SC_CURSOR_LEFT:
		  roll-=CTRLS_SCALE;
		  break;
	      end:
		WrapAngles;
	    }
	    break;
	  case '\n':
	    Fs->draw_it=NULL;
	    CleanUp;
	    WinMgrSync;
	    Init;
	    Fs->draw_it=&DrawIt;
	    break;
	}
      } else
	Core0EagleDive;
    } while (ch!=CH_SHIFT_ESC && ch!=CH_ESC);
  catch
    PutExcept;
  SettingsPop;
  CleanUp;
  RegWriteBranch("TempleOS/EagleDive","F64 best_score=%5.4f;\n",best_score);
  MenuPop;
}

EagleDive;
