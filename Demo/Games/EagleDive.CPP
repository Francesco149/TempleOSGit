//Uses [C:/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.

RegSetDftEntry("TempleOS/EagleDive","I64 best_score=0;\n");
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

class Panel
{
  Panel *next;
  I32 n,   //num polygon sides
     num; //panel number
  CD3I32 *pts;
  I64 color,cnt;
};

class Fish
{
  Fish *next,*last;
  CD3I64 p;
} fish_root;




/* <1> <1> (image) */





I64 critical_section_flag;
U0 CriticalSectionStart()
{
  while (LBts(&critical_section_flag,0))
    Yield;
}
U0 CriticalSectionEnd()
{
  LBtr(&critical_section_flag,0);
}

/*These are **NOT** dealloced after the program ends,
but I did them this way for your benefit
so 2D array indexing is not done by hand.
You can type "EagleDive;" at the command line
to rerun without new allocations.
Normally, just spawn a popup task,
run it and kill it using an icon.
*/
I16 elevations[MAP_HEIGHT][MAP_WIDTH];
I32 normals[MAP_HEIGHT][MAP_WIDTH*2];
Panel *panels[MAP_HEIGHT][MAP_WIDTH*2];

I64 num_panels;
Panel *panel_root;
U8 *panels_processed_bitmap;

F64 game_t0,phi,theta1,theta2,speed;

I64 strip_height,x,y,z,game_score;

Bool game_over;

class MPCtrl {
  I64 mp_cnt;
  I64 init_not_done_flags,update_not_done_flags,app_not_done_flags;
  I64 strip_width[MP_MAX_PROCESSORS];
  Bool app_done;
} mp;

U0 EDTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  I64 zz;
  GrRot(dc->r,x,y,z);
  *z=zz=-*z;
  if (zz>0) {
    *x = *x * DISPLAY_SCALE/ zz; //Foreshortening
    *y = *y * DISPLAY_SCALE/ zz;
  }
  *x+=dc->x;
  *y =dc->y-*y;
}

U0 CalcNormals()
{
/*The idea is that there are two tris
forming every grid square and we find the normal
vect with a curl.

 i  j  k
 0  1  dz2
 1  0  dz1

Normal:
dz1*i + dz2*j - k

  i   j   k
 -1   0  -dz3
  0  -1  -dz4

Normal:
-dz3*i - dz4*j + k

 
Basically, we want to combine grid
locations with the same normals.  These
are not unit vects for one thing.

What I came-up with is dz1/dz2 and dz3/dz4.

It works pretty well.  It combines
more than it should, but that's good
because it would  result in too many
panels.
*/
  I64 i,j,k,dz1,dz2;
  for (j=0;j<MAP_HEIGHT-1;j++) {
    k=0;
    for (i=0;i<MAP_WIDTH-1;i++) {
      dz1=elevations[j][i+1]-elevations[j][i];
      dz2=elevations[j+1][i]-elevations[j][i];
      if (dz2)
	normals[j][k++]=dz1<<16/dz2;
      else {
	if (!dz1)
	  normals[j][k++]=0;
	else if (dz1>0)
	  normals[j][k++]=MAX_I32;
	else
	  normals[j][k++]=MIN_I32;
      }

      dz1=elevations[j+1][i+1]-elevations[j][i+1];
      dz2=elevations[j+1][i+1]-elevations[j+1][i];
      if (dz2)
	normals[j][k++]=dz1<<16/dz2;
      else {
	if (!dz1)
	  normals[j][k++]=0;
	else if (dz1>0)
	  normals[j][k++]=MAX_I32;
	else
	  normals[j][k++]=MIN_I32;
      }
    }
    normals[j][k++]=0;
    normals[j][k++]=0;
  }
  k=0;
  for (i=0;i<MAP_WIDTH-1;i++) {
    normals[j][k++]=0;
    normals[j][k++]=0;
  }
}

Bool TestSameSlope(I64 x,I64 y,I64 w,I64 h)
{
  I64 k1,k2,xx,yy,s;
  if (!(0<=x && x+w<MAP_WIDTH && 0<=y && y+h<MAP_HEIGHT))
    return FALSE;
    //Works well averaging the two slopes of two tris.
    //If it is too picky, we get too many panels.
  s=normals[y][x*2]+normals[y][x*2+1];
  for (k2=0;k2<h;k2++)
    for (k1=0;k1<w;k1++) {
      xx=(x+k1)*2;
      yy=y+k2;
      if (normals[yy][xx]+normals[yy][xx+1]!=s)
       return FALSE;
    }
  return TRUE;
}

U0 MPDoPanels(CTask *task)
{
  I64 i,j,k,l,k1,k2,w,h,threshold,lo,hi;
  Bool cont;
  Panel *tempp,*start_ptr=NULL,*end_ptr=NULL;
  CD3I32 *poly;
  Fish *tempf;
  lo=Gs->num*(MAP_HEIGHT-1)/mp.mp_cnt;
  hi=(Gs->num+1)*(MAP_HEIGHT-1)/mp.mp_cnt;
  for (threshold=8;threshold>=1;threshold--)
    for (j=lo;j<hi;j++) {
      k=0;
      for (i=0;i<MAP_WIDTH-1;i++) {
	if (!panels[j][k]) {
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
	    lock {tempp->num=num_panels++;}
	    l=elevations[j][i];
	    if (l<=WATER_ELEVATION*MAP_SCALE &&
		elevations[j][i+w-1]<=WATER_ELEVATION*MAP_SCALE &&
		elevations[j+h-1][i]<=WATER_ELEVATION*MAP_SCALE &&
		elevations[j+h-1][i+w-1]<=WATER_ELEVATION*MAP_SCALE) {
	      tempp->color=BLUE;
	      if (Rand<0.05) {
		tempf=MAlloc(sizeof(Fish),task);
		tempf->p.x=(i+w/2)*MAP_SCALE;
		tempf->p.y=(j+h/2)*MAP_SCALE;
		tempf->p.z=l;
		CriticalSectionStart;
		QueIns(tempf,fish_root.last);
		CriticalSectionEnd;
	      }
	    } else {
	      if (RandI16&1) {
		if (l<ROCK_ELEVATION*MAP_SCALE)
		  tempp->color=LTGREEN;
		else if (l<SNOW_ELEVATION*MAP_SCALE) {
		  if (!(RandU16&3))
		    tempp->color=LTGRAY;
		  else
		    tempp->color=LTGREEN;
		} else {
		  if (!(RandU16&3))
		    tempp->color=WHITE;
		  else
		    tempp->color=LTGRAY;
		}
	      } else {
		if (l<ROCK_ELEVATION*MAP_SCALE)
		  tempp->color=GREEN+LTGREEN<<16+ROPF_DITHER;
		else if (l<SNOW_ELEVATION*MAP_SCALE) {
		  if (!(RandU16&3))
		    tempp->color=DKGRAY+LTGRAY<<16+ROPF_DITHER;
		  else
		    tempp->color=GREEN+LTGREEN<<16+ROPF_DITHER;
		} else {
		  if (!(RandU16&3))
		    tempp->color=LTGRAY+WHITE<<16+ROPF_DITHER;
		  else
		    tempp->color=DKGRAY+LTGRAY<<16+ROPF_DITHER;
		}
	      }
	    }
	    tempp->n=4;
	    poly=tempp->pts=MAlloc(sizeof(CD3I32)*tempp->n,task);
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
	      for (k1=0;k1<w;k1++) {
		panels[j+k2][(i+k1)*2]=tempp;
		panels[j+k2][(i+k1)*2+1]=tempp;
	      }
	  }
	}
	k+=2;
      }
    }
  if (end_ptr) {
    CriticalSectionStart;
    if (end_ptr)
      end_ptr->next=panel_root;
    panel_root=start_ptr;
    CriticalSectionEnd;
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
and convert it to panels.  The conversion to panels leaves
gaps the way I did it, but it looks good, except for sky leaking through
There is no wrong way to pick colors if it looks good, unless
you wanted to do lighting.

"panels[][]" holds the flat and sloped panels for each spot, while
"hstep_panels[][]" holds side panels in cases of step-like formations.
They require a separate array because a single "elevation[][]" spot can
have a panel on top and a panel on the side like stairs.
*/
  I64 i,x,y,xx,yy;

  InitElevations;

  MemSet(panels,0,sizeof(panels));
  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);
  z+=elevations[yy][xx]*COORDINATE_SCALE;

  CalcNormals;

  panel_root=NULL;
  num_panels=0;

  mp.init_not_done_flags=1<<mp.mp_cnt-1;
  for (i=0;i<mp.mp_cnt;i++)
    Spawn(&MPDoPanels,Fs,"Do Panels",i);
  while (mp.init_not_done_flags)
    Sleep(1);
  panels_processed_bitmap=MAlloc((num_panels+7)>>3);
}

CDC *main_dc;

U0 DrawIt(CTask *,CDC *dc)
{
  main_dc->flags|=DCF_NO_TRANSPARENTS;
  GrBlot(dc,0,0,main_dc);
}

/*
/* <2> <2> (image) */


















/* <3> <3> (image) */
































Cores render strips that +/- 15%. The cores check the panel map array
and render the panel for each square, marking-it done.

The depth buf is not locked in the graphic routines
so we get some glitches.
*/

I64 update_jiffy_limit;

U0 MPDrawIt(CTask *task,CDC *dc)
{
  I64 j,strip_width,*s2w,x1,y1,z1, xx,yy,
      xh,yh,zh,yh2,xh2,x1h,y1h,x1wa,y1wa,x1wb,y1wb,
      x3,y3,z3,
      cx=task->pix_width>>1,
      cy=task->pix_height>>1;
  Panel reg *tempp;

  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);

  //World to screen coordinates
  GrIdentEqu(dc->r);
  GrRotZEqu(dc->r,theta2);
  GrRotXEqu(dc->r,phi);
  GrRotZEqu(dc->r,theta1);
  GrSetRotMat(dc,dc->r);

  //Screen to world coordinates

//This gives us the vects for stepping through the grid in
//the direction the plane is facing. we step horizontally and vertically
//and use the reciprocal slope principle	y=mx+b and y=(-1/m)x+b are perpendicular.

  s2w=GrIdentNew;
  GrRotZEqu(s2w,-theta1);
  GrRotXEqu(s2w,-phi);
  GrRotZEqu(s2w,-theta2);
  xh=0;
  yh=0;
  zh=-256;
  GrRot(s2w,&xh,&yh,&zh);

//The layer for core#1 is not cleared automatically
//it is persistent.  I have carefully syncronized to the update
//cycle initiated by core#0 to prevent flicker.

  dc->flags|=DCF_TRANSFORMATION;
  dc->transform=&EDTransform;
  dc->x=cx;
  dc->y=cy;

//dc->x and the translation part of dc->r are identical in effect,
//but we use both to show-off.	We could add offsets together and use one or the other.

  x1=-x>>COORDINATE_BITS;
  y1=-y>>COORDINATE_BITS;
  z1=-z>>COORDINATE_BITS;
  GrRot(dc->r,&x1,&y1,&z1);
  GrSetTranslation(dc->r,x1,y1,z1);

  //This is a refinement.
  if (Abs(phi*180/pi)>90) {
    x3=0;
    y3=-cy;
    z3=0;
    GrRot(s2w,&x3,&y3,&z3);
    xx+=x3;
    yy+=y3;
  }

  if (Gs->num&1) {//alternate left-right,right-left
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
  for (strip_width=0;cnts.jiffies<update_jiffy_limit;strip_width++) {
    x1h=x1wa;
    y1h=y1wa;
    for (j=0;j<strip_height && cnts.jiffies<update_jiffy_limit;j++) {
      x1=x1h>>8; y1=y1h>>8;
      if (0<=x1<MAP_WIDTH && 0<=y1<MAP_HEIGHT) {
	  if ((tempp=panels[y1][x1*2]) && !LBts(panels_processed_bitmap,tempp->num)) {
	    if (tempp->cnt>8*(1.1-Gs->idle_factor)) {
	      dc->color=tempp->color;
	      tempp->cnt=GrFillPoly3(dc,tempp->n,tempp->pts);
	    } else
	      tempp->cnt++;
	  }
      }
      x1h+=xh;
      y1h+=yh;
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
  }

  Free(s2w);
  mp.strip_width[Gs->num]=strip_width;
  LBtr(&mp.update_not_done_flags,Gs->num);
}

U0 APEagleDiveTask(CTask *master_task)
{
  CDC *dc=DCAlias(main_dc,master_task);
  Preempt(ON);
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
  I64 x1,y1,z1,x2,y2,z2,xh,yh,zh,*s2w=GrIdentNew,
      cx=Fs->pix_width>>1,
      cy=Fs->pix_height>>1;
  CD3I32 p[4];
  I32 *old_db=dc->depth_buf;
  dc->depth_buf=NULL;

  GrIdentEqu(dc->r);
  GrRotZEqu(dc->r,theta2);
  GrRotXEqu(dc->r,phi);
  GrRotZEqu(dc->r,theta1);
  GrSetRotMat(dc,dc->r);
  dc->flags&=~DCF_TRANSFORMATION;
  dc->transform=&EDTransform;
  dc->x=cx;
  dc->y=cy;

  GrRotZEqu(s2w,-theta1);
  GrRotXEqu(s2w,-phi);
  GrRotZEqu(s2w,-theta2);
  xh=0;
  yh=0;
  zh=-256;
  GrRot(s2w,&xh,&yh,&zh);
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
    if (-pi/2<=theta1<pi/2 ^^ -pi/2<=Wrap(phi-pi/2)<pi/2)
      dc->color=LTCYAN;
    else
      dc->color=LTGREEN;
    GrFillPoly3(dc,4,p);
    p[0].y=dc->height-1;
    p[1].y=dc->height-1;
    if (-pi/2<=theta1<pi/2 ^^ -pi/2<=Wrap(phi-pi/2)<pi/2)
      dc->color=LTGREEN;
    else
      dc->color=LTCYAN;
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
      if (x1<x2 ^^ -pi/2<=theta1<pi/2 ^^ -pi/2<=Wrap(phi-pi/2)<pi/2)
	dc->color=LTCYAN;
      else
	dc->color=LTGREEN;
      GrFillPoly3(dc,4,p);
      p[0].x=dc->width-1;
      p[1].x=dc->width-1;
      if (x1<x2 ^^ -pi/2<=theta1<pi/2 ^^ -pi/2<=Wrap(phi-pi/2)<pi/2)
	dc->color=LTGREEN;
      else
	dc->color=LTCYAN;
      GrFillPoly3(dc,4,p);
    }
  }
  dc->depth_buf=old_db;
}

U0 BSPEagleDive()
{
  CDC *dc=DCAlias(main_dc,Fs);
  I64 i,xx,yy,height,bkcolor,
      cx=Fs->pix_width>>1,
      cy=Fs->pix_height>>1;
  F64 min_strip_width;
  Fish *tempf;

  update_jiffy_limit=cnts.jiffies+JIFFY_FREQ/40;
  MemSet(panels_processed_bitmap,0,(num_panels+7)>>3);

  xx=x/(MAP_SCALE*COORDINATE_SCALE);
  yy=y/(MAP_SCALE*COORDINATE_SCALE);
  height=z/COORDINATE_SCALE-elevations[yy][xx];
  if (height<0) {
    if (elevations[yy][xx]>WATER_ELEVATION*MAP_SCALE) {
      dc->color=bkcolor=BLACK;
      if (!game_over) {
	GrRect(dc,0,0,dc->width,dc->height);
	music.mute=TRUE;
	Beep;
	game_over=TRUE;
	music.mute=FALSE;
      }
    } else
      bkcolor=BLUE;
  } else
    bkcolor=LTCYAN;

  if (height>=0 && !(-pi/4<=Wrap(phi-pi)<pi/4)) {
    DrawHorizon(dc);
    DCDepthBufRst(dc);

    mp.update_not_done_flags=1<<mp.mp_cnt-1;
    MPDrawIt(Fs,dc);

    xx=x/COORDINATE_SCALE;
    yy=y/COORDINATE_SCALE;
    tempf=fish_root.next;
    while (tempf!=&fish_root) {
      if (SqrI64(tempf->p.x-xx)+SqrI64(tempf->p.y-yy)<MAP_SCALE*MAP_SCALE*20*20)
	Sprite3(dc,tempf->p.x,tempf->p.y,tempf->p.z,__BIN_1);
      tempf=tempf->next;
    }

    while (mp.update_not_done_flags)
      Sleep(1);

    min_strip_width=MAX_F64;
    for (i=0;i<mp.mp_cnt;i++)
      min_strip_width=Min(min_strip_width,32.0*mp.strip_width[i]/(i+32.0));
    strip_height=ClampI64(strip_height*Clamp(0.25*min_strip_width/strip_height,0.9,1.1),64,1024);
  } else {
    dc->color=bkcolor;
    GrRect(dc,0,0,dc->width,dc->height);
  }
  dc->pen_width=2;
  if (bkcolor==BLUE)
    dc->color=WHITE;
  else
    dc->color=BLACK;
  dc->flags&=~DCF_TRANSFORMATION;
  GrLine3(dc,cx+5,cy,0,cx-5,cy,0);
  GrLine3(dc,cx,cy+5,0,cx,cy-5,0);
  GrPrint(dc,0,0,"theta1:%5.1f phi:%5.1f theta2:%5.1f Strip:%5d",
      theta1*180/pi,phi*180/pi,theta2*180/pi,strip_height);
  GrPrint(dc,0,FONT_HEIGHT,"x:%5.1f y:%5.1f z:%5.1f height:%3d score:%d high:%d",
      ToF64(x)/COORDINATE_SCALE,ToF64(y)/COORDINATE_SCALE,ToF64(z)/COORDINATE_SCALE,
      height,game_score,best_score);

  if (game_over && Blink) {
    dc->color=RED;
    GrPrint(dc,(Fs->pix_width-9*FONT_WIDTH)/2,
	(Fs->pix_height-FONT_HEIGHT)/2,"Game Over");
  } else if (tS-game_t0<5.0 && Blink) {
    dc->color=WHITE;
    GrPrint(dc,(Fs->pix_width-10*FONT_WIDTH)/2,
	(Fs->pix_height-FONT_HEIGHT)/2,"Catch Fish");
  }
//We made an alias of this we don't want freed.
  dc->depth_buf=NULL;
  DCDel(dc);
  WinMgrSync;
}

#define ANIMATE_MS	10

U0 AnimateTask(I64)
{//Steadily moves the airplane fwd.
  I64 *s2w,x1,y1,z1;
  F64 t0=tS,mS;
  Fish *tempf,*tempf1;
  while (TRUE) {
    mS=1000*(tS-t0);
    t0=tS;
    if (!game_over) {
      //Screen to world coordinates
      s2w=GrIdentNew;
      GrRotZEqu(s2w,-theta1);
      GrRotXEqu(s2w,-phi);
      GrRotZEqu(s2w,-theta2);
      x1=0;y1=0;z1=-speed*mS*COORDINATE_SCALE;
      GrRot(s2w,&x1,&y1,&z1);
      x+=x1;
      y+=y1;
      z+=z1;
      Free(s2w);
      x1=x/COORDINATE_SCALE; y1=y/COORDINATE_SCALE; z1=z/COORDINATE_SCALE;

      if (z1<WATER_ELEVATION*MAP_SCALE) {
	speed=0.5;
	tempf=fish_root.next;
	while (tempf!=&fish_root) {
	  tempf1=tempf->next;
	  if (SqrI64(tempf->p.x-x1)+SqrI64(tempf->p.y-y1)<MAP_SCALE*MAP_SCALE*5*5) {
	    game_score++;
	    if (game_score>best_score)
	      best_score=game_score;
	    QueRem(tempf);
	    Free(tempf);
	    music.mute=TRUE;
	    Snd(1000); Sleep(200); Snd(0);
	    music.mute=FALSE;
	  }
	  tempf=tempf1;
	}
      } else {
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
  mp.app_not_done_flags=1<<mp.mp_cnt-1-1;
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
{//Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.stacatto_factor=0.2;
  while (TRUE) {
	Play("3eBDEDBDEDFEEDFEED");
	Play("BDEDBDEDFEEDFEED");
  }
}

U0 Init(I64 _mp_cnt)
{
  I64 i;
  main_dc=DCNew(GR_WIDTH,GR_HEIGHT);
  critical_section_flag=0;
  game_over=FALSE;
  game_score=0;
  MemSet(&mp,0,sizeof(MPCtrl));
  if (_mp_cnt<=mp_cnt)
    mp.mp_cnt=_mp_cnt;
  else
    mp.mp_cnt=mp_cnt;
  QueInit(&fish_root);
  InitMap;
  DCAllocDepthBuf(main_dc);

  strip_height=128;
  phi	=-90.0*pi/180.0;
  theta1=  0.0*pi/180.0;
  theta2=  0.0*pi/180.0;
  speed =  2.5;

  x=MAP_WIDTH>>1 *COORDINATE_SCALE*MAP_SCALE;
  y=MAP_HEIGHT>>1*COORDINATE_SCALE*MAP_SCALE;
  z=64		 *COORDINATE_SCALE*MAP_SCALE;

  for (i=1;i<mp.mp_cnt;i++)
    Spawn(&APEagleDiveTask,Fs,"AP EagleDive",i);
  Fs->task_end_cb=&TaskEndCB;
  game_t0=tS;
}

U0 CleanUp()
{
  Fish *tempf=fish_root.next,*tempf1;
  Panel *tempp=panel_root,*tempp1;

  MPEnd;

  while (tempf!=&fish_root) {
    tempf1=tempf->next;
    QueRem(tempf);
    Free(tempf);
    tempf=tempf1;
  }
  while (tempp) {
    tempp1=tempp->next;
    Free(tempp->pts);
    Free(tempp);
    tempp=tempp1;
  }
  Free(panels_processed_bitmap);
  DCDel(main_dc);
}

U0 EagleDive(I64 _mp_cnt=MP_MAX_PROCESSORS)
{
  I64 ch,sc;
  F64 pp,tt1,tt2;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  AutoComplete;
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
  WinMax;
  WinBorder;
  "Initializing...\n";
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Init(_mp_cnt);
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try //in case <CTRL-ALT-c> is pressed.
    do {
      if (ScanKey(&ch,&sc)) {
	pp=phi; tt1=theta1; tt2=theta2;
	switch (ch) {
	  case 0:
	    switch (sc.u8[0]) {
	      case SC_CURSOR_DOWN:
		phi +=-CTRLS_SCALE*Cos(tt1);
		theta2+=-CTRLS_SCALE*Sin(tt1)*Sin(phi);
		break;
	      case SC_CURSOR_UP:
		phi -=-CTRLS_SCALE*Cos(tt1);
		theta2-=-CTRLS_SCALE*Sin(tt1)*Sin(phi);
		break;
	      case SC_CURSOR_RIGHT:
		theta1+=CTRLS_SCALE;
		break;
	      case SC_CURSOR_LEFT:
		theta1-=CTRLS_SCALE;
		break;
	    }
	    break;
	  case '\n':
	    Fs->draw_it=NULL;
	    CleanUp;
	    WinMgrSync;
	    Init(_mp_cnt);
	    Fs->draw_it=&DrawIt;
	    break;
	  }
	theta1=Wrap(theta1);
	phi =Wrap(phi);
	theta2=Wrap(theta2);
      } else
	BSPEagleDive;
    } while (ch!=CH_SHIFT_ESC && ch!=CH_ESC);
  catch
    PutExcept;
  SettingsPop;
  CleanUp;
#ifjit
  RegWriteBranch("TempleOS/EagleDive","I64 best_score=%d;\n",best_score);
#endif
  MenuPop;
}

EagleDive;
