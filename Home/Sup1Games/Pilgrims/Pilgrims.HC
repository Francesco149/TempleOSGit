Cd(__DIR__);;
#include "GrVectMap"

/*
Ideas?

How about clouds and rain for crops?

How about achievement of Ben Franklin Almanac.

How about a shipping company office.

How about something like M.U.L.E.

How about turkey shoot.

Witch trials?  Maybe, have God talk?

Seven Cities of Gold or the game, Civilization,
is what's expected but that's dull.

Rivals Spanish and French?

*/


/* <1> <1> (image) */










/* <2> <2> (image) */








/* <3> <3> (image) */





/* <4> <4> (image) */



/* <5> <5> (image) */





 
   /* <6> <6> (image) */




/* <7> <7> (image) */


#define AFT_MAST	16
#define BOW_MAST	14
#define MAST_HEIGHT	20
#define AFT_LEN		36
#define BOW_LEN		43
#define BERTH		28

F64 scrn_x,scrn_y,d_scrn_x,d_scrn_y,
   wind_speed,

   //This is on a wave close to ship
   //in world coordinates.
   ship_wave_x,ship_wave_y,

   church_x,church_y,

   wave_speed,wave_theta,d_wave_theta,wave_period,
   wave_amplitude;

Bool wave_sound,ship_landed;
F64 song_time,game_over_time;

F64 view_scale,new_view_scale;
CDC *master_map;
U8 *vm;
F64 men;

#define DOLPHINS_NUM	5
#define PIGRAIMS_NUM	10
	
class Obj
{
  F64 x,y,radius,
	theta,d_theta,
	speed;
} ship,dolphins[DOLPHINS_NUM],pilgrims[PIGRAIMS_NUM];

#define BUBBLES_NUM	1024
class Bubble
{
  I32 x,y;
} bubbles[BUBBLES_NUM];

#define GARDENS_NUM	128
class Garden
{
  I32 x,y;
} gardens[GARDENS_NUM];
I64 num_gardens;


F64 WaterHeight(F64 x,F64 y)
{ //Returns wave height at world coordinate pt
  F64 dxn=Cos(wave_theta),
	dyn=Sin(wave_theta),theta;
  theta=2*pi*((x-ship_wave_x)*dxn+(y-ship_wave_y)*dyn)/wave_period;
  return wave_amplitude*Cos(theta);
}

I64 *ShipMat()
{ //Returns rotation matrix for ship bobbing on waves
  F64 x1,y1,z1,x2,y2,z2,a;
  I64 *res;

  x1=ship.x-AFT_LEN/2*Cos(ship.theta)/view_scale;
  y1=ship.y-AFT_LEN/2*Sin(ship.theta)/view_scale;
  z1=WaterHeight(x1,y1);
  x2=ship.x+BOW_LEN/2*Cos(ship.theta)/view_scale;
  y2=ship.y+BOW_LEN/2*Sin(ship.theta)/view_scale;
  z2=WaterHeight(x2,y2);
  a=ASin((z2-z1)/((AFT_LEN/2+BOW_LEN/2)/view_scale));
  res=Mat4x4IdentNew;
  Mat4x4RotY(res,a);

  z1=WaterHeight(ship.x,ship.y);
  x2=ship.x-BERTH/2*Sin(ship.theta)/view_scale;
  y2=ship.y+BERTH/2*Cos(ship.theta)/view_scale;
  z2=WaterHeight(x2,y2);
  a=ASin((z2-z1)/(BERTH/view_scale));
  Mat4x4RotX(res,a);

  Mat4x4RotZ(res,ship.theta);
  return res;
}

U0 BubbleAdd(I64 x,I64 y)
{
  I64 i=RandU16&(BUBBLES_NUM-1);
  bubbles[i].x=x;
  bubbles[i].y=y;
}

U0 PosChurch(F64 x,F64 y)
{
  I64 i;
  CDC *dc=DCAlias(master_map,Fs);
  dc->color  =ROP_COLLISION;
  dc->bkcolor=BLACK;
  while (x>0) {
    dc->collision_cnt=0;
    Sprite3(dc,x,y,0,__BIN_2);
    if (dc->collision_cnt>2000)
      break;
    x--;
  }
  church_x=x; church_y=y;
  DCDel(dc);
  for (i=0;i<PIGRAIMS_NUM;i++) {
    pilgrims[i].x=church_x;
    pilgrims[i].y=church_y;
    pilgrims[i].theta=2*pi*Rand;
    pilgrims[i].d_theta=0;
    pilgrims[i].speed=10.0;
  }
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,
	w=task->pix_width, cx=w>>1,
	h=task->pix_height,cy=h>>1,
	c=1.05*(MaxI64(cx,cy)+wave_period),
	*ship_r,x,y,z;
  F64 d,xx,yy,dx,dy,dx2,dy2,wave_x,wave_y;
  Bool bow_down;

  task->text_attr=LTBLUE<<4;

  //Draw Bubbles
  dc->color=BLUE;
  for (i=0;i<BUBBLES_NUM;i++)
    GrPlot(dc,bubbles[i].x*view_scale-scrn_x,
	  bubbles[i].y*view_scale-scrn_y);

    //Draw Waves
  wave_x=ship_wave_x*view_scale-scrn_x;
  wave_y=ship_wave_y*view_scale-scrn_y;

  dx=wave_period*Cos(wave_theta)*view_scale;
  dy=wave_period*Sin(wave_theta)*view_scale;
  dx2=-dy*c;
  dy2=dx*c;

  dc->color=BLUE;
  if (Abs(dx)>Abs(dy)) {
    xx=wave_x; yy=wave_y;
    while (xx>-c) {
      GrLine(dc,xx-dx2,yy-dy2,xx+dx2,yy+dy2);
      if (dx>0) { xx-=dx; yy-=dy;
      } else {	  xx+=dx; yy+=dy; }
    }
    xx=wave_x; yy=wave_y;
    while (xx<w+c) {
      GrLine(dc,xx-dx2,yy-dy2,xx+dx2,yy+dy2);
      if (dx<0) { xx-=dx; yy-=dy;
      } else {	  xx+=dx; yy+=dy; }
    }
  } else {
    xx=wave_x; yy=wave_y;
    while (yy>-c) {
      GrLine(dc,xx-dx2,yy-dy2,xx+dx2,yy+dy2);
      if (dy>0) { xx-=dx; yy-=dy;
      } else {	  xx+=dx; yy+=dy; }
    }
    xx=wave_x; yy=wave_y;
    while (yy<h+c) {
      GrLine(dc,xx-dx2,yy-dy2,xx+dx2,yy+dy2);
      if (dy<0) { xx-=dx; yy-=dy;
      } else {	  xx+=dx; yy+=dy; }
    }
  }

  //Draw Dolphins
  for (i=0;i<DOLPHINS_NUM;i++)
    Sprite3ZB(dc,dolphins[i].x*view_scale-scrn_x,
	  dolphins[i].y*view_scale-scrn_y,0,__BIN_7,dolphins[i].theta);

    //Draw Land
  dc->color=ROP_MONO+YELLOW;
  VectMapBlotMP(dc,-scrn_x,-scrn_y,vm,view_scale);

  //Draw Ship
  dc->color=ROP_EQU;
  ship_r=ShipMat;
  Sprite3Mat4x4B(dc,ship.x*view_scale-scrn_x,ship.y*view_scale-scrn_y,
	0,__BIN_6,ship_r);

  //Rudder
  x=-AFT_LEN; y=0; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  dc->color=BLACK;
  dc->thick=2;
  GrLine3(dc,x-scrn_x,y-scrn_y,0,
	x-8*Cos(ship.theta-2.5*ship.d_theta)-scrn_x,
	y-8*Sin(ship.theta-2.5*ship.d_theta)-scrn_y,0);
  dc->thick=1;

  //Wake
  x=(-(AFT_LEN+2)+RandU16%13-6)/view_scale; y=(RandU16%13-6)/view_scale; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x; y+=ship.y;
  BubbleAdd(x,y);
  for (i=0;i<DOLPHINS_NUM;i++) {
    if (!(RandU16&15)) {
      x=dolphins[i].x-(7*Cos(dolphins[i].theta)+RandU16%3-1)/view_scale;
      y=dolphins[i].y-(7*Sin(dolphins[i].theta)+RandU16%3-1)/view_scale;
      BubbleAdd(x,y);
    }
  }


  //Draw Sails
  d=Wrap(ship.theta-Wrap(wave_theta,ship.theta-pi));
  if (Abs(d)<pi)
    d=(Abs(d)-pi)*Sign(-d);
  dx=-1.5*BERTH/2*Sin(ship.theta+d/2);
  dy= 1.5*BERTH/2*Cos(ship.theta+d/2);

  x=BOW_MAST; y=0; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  if (z<0)
    bow_down=TRUE;
  else
    bow_down=FALSE;

    //Aft Sail
  x=-AFT_MAST; y=0; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  dc->color=BLACK;
  GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);

  xx=x-scrn_x; yy=y-scrn_y;

  if (!bow_down) {
    x=-AFT_MAST; y=0; z=MAST_HEIGHT;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    dc->color=DKGRAY;
    dc->thick=4;
    GrLine3(dc,x-scrn_x,y-scrn_y,0,xx,yy,0);
  }

  dc->thick=2;
  for (d=0;d<MAST_HEIGHT;d+=0.5) {
    x=-AFT_MAST; y=0; z=d;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    x-=1.5*Cos(wave_theta); y-=1.5*Sin(wave_theta);
    dc->color=WHITE;
    GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);
  }
  dc->thick=1;

  x=-AFT_MAST; y=0; z=MAST_HEIGHT;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  dc->color=BLACK;
  GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);

  if (bow_down) {
    x=-AFT_MAST; y=0; z=MAST_HEIGHT;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    dc->color=DKGRAY;
    dc->thick=4;
    GrLine3(dc,x-scrn_x,y-scrn_y,0,xx,yy,0);
  }

  dc->color=BLACK;
  dc->thick=1;
  GrCircle3(dc,x-scrn_x,y-scrn_y,0,2);

  //Bow Sail
  x=BOW_MAST; y=0; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  dc->color=BLACK;
  GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);

  xx=x-scrn_x; yy=y-scrn_y;

  if (!bow_down) {
    x=BOW_MAST; y=0; z=MAST_HEIGHT;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    dc->color=DKGRAY;
    dc->thick=4;
    GrLine3(dc,x-scrn_x,y-scrn_y,0,xx,yy,0);
  }

  dc->thick=2;
  for (d=0;d<MAST_HEIGHT;d+=0.5) {
    x=BOW_MAST; y=0; z=d;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    x-=1.5*Cos(wave_theta); y-=1.5*Sin(wave_theta);
    dc->color=WHITE;
    GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);
  }
  dc->thick=1;

  x=BOW_MAST; y=0; z=MAST_HEIGHT;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  dc->color=BLACK;
  GrLine3(dc,x-dx-scrn_x,y-dy-scrn_y,0,x+dx-scrn_x,y+dy-scrn_y,0);

  if (bow_down) {
    x=BOW_MAST; y=0; z=MAST_HEIGHT;
    Mat4x4MulXYZ(ship_r,&x,&y,&z);
    x+=ship.x*view_scale; y+=ship.y*view_scale;
    dc->color=DKGRAY;
    dc->thick=4;
    GrLine3(dc,x-scrn_x,y-scrn_y,0,xx,yy,0);
  }

  dc->color=BLACK;
  dc->thick=1;
  GrCircle3(dc,x-scrn_x,y-scrn_y,0,2);

  dc->color=BLACK;
  GrPrint(dc,0,0,"Speed:%5.1f Knots",ship.speed);
  GrPrint(dc,0,FONT_HEIGHT,"Men:%f",men);

  //Check for wreck
  x=BOW_LEN; y=0; z=0;
  Mat4x4MulXYZ(ship_r,&x,&y,&z);
  x+=ship.x*view_scale; y+=ship.y*view_scale;
  if (!ship_landed && GrPeek(dc,x-scrn_x,y-scrn_y)==YELLOW) {
    Noise(500,62,74);
    song_time	  =tS+0.5;
    game_over_time=tS+1.0;
    wave_sound=FALSE;
    ship_landed=TRUE;
    PosChurch(x/view_scale,y/view_scale);
  }

  if (ship_landed) {
//Draw Gardens
    for (i=0;i<GARDENS_NUM;i++)
      Sprite3(dc,gardens[i].x*view_scale-scrn_x,
	    gardens[i].y*view_scale-scrn_y,0,__BIN_5);

      //Draw Church
    Sprite3(dc,church_x*view_scale-scrn_x,church_y*view_scale-scrn_y,0,__BIN_2);
//Draw Pilgrims
    for (i=0;i<PIGRAIMS_NUM;i++)
      if (i&1)
	Sprite3(dc,pilgrims[i].x*view_scale-scrn_x,
	      pilgrims[i].y*view_scale-scrn_y,0,__BIN_3);
      else
	Sprite3(dc,pilgrims[i].x*view_scale-scrn_x,
	      pilgrims[i].y*view_scale-scrn_y,0,__BIN_4);
    if (tS>game_over_time && Blink) {
      dc->color=LTRED;
      GrPrint(dc,(task->pix_width-14*FONT_WIDTH)/2,
	    (task->pix_height-FONT_HEIGHT)/2,"Game Completed");
    }
  }
  Free(ship_r);
}


CColorROPU32 pray_colors[1]={RED};
CTask *pray_task;

U0 LeftClickPrayBttn(CCtrl *,I64,I64,Bool down)
{
  I64 i,size,l=512;
  U8 *buf;
  if (down && !TaskValidate(pray_task) &&(buf=FileRead(BIBLE_FILENAME,&size))) {
    i=GetTSC>>4%(size-l);
    buf[i+l]=0;
    pray_task=PopUpViewPrint(buf+i);
    Free(buf);
  }
}

U0 Init()
{
  I64 i;
  Fs->draw_it=NULL;
  Kill(Fs->song_task);
  Fs->song_task=NULL;
  Refresh;

  men=102.0;
  pray_task=NULL;

  scrn_x=400;	d_scrn_x=0;
  scrn_y=500;	d_scrn_y=0;
  view_scale=1.0; new_view_scale=1.0;
  MemSetU32(&bubbles,I32_MAX,sizeof(bubbles)/sizeof(U32));

  num_gardens=0;
  MemSetU32(&gardens,I32_MAX,sizeof(gardens)/sizeof(U32));

  MemSet(&dolphins,0,sizeof(dolphins));
  for (i=0;i<DOLPHINS_NUM;i++) {
    dolphins[i].radius =5;
    dolphins[i].x=scrn_x+Rand*(GR_WIDTH-2*dolphins[i].radius)+
	  dolphins[i].radius;
    dolphins[i].y=scrn_y+Rand*(GR_WIDTH-2*dolphins[i].radius)+
	  dolphins[i].radius;
    dolphins[i].theta=Rand*2*pi;
    dolphins[i].d_theta=0;
    dolphins[i].speed  =15;
  }

  MemSet(&ship,0,sizeof(ship));
  ship.radius =MaxI64(AFT_LEN,BOW_LEN);
  ship.x	    =scrn_x+GR_WIDTH/2-ship.radius;
  ship.y	    =scrn_y+GR_HEIGHT/2;
  ship.theta  =pi;
  ship.d_theta=0;
  ship.speed  =1.0;
  ship_wave_x=0; ship_wave_y=0;

  wave_theta=Rand*pi*2;
  d_wave_theta=0;
  wave_amplitude=10;
  wave_period=75;
  wave_speed=20*2*pi/wave_period;
  wind_speed=25;
  wave_sound=TRUE;
  ship_landed=FALSE;
  song_time=0;
  game_over_time=0;
  Fs->draw_it=&DrawIt;
}


U0 AnimateObj(Obj *tmpo,F64 dt)
{
  tmpo->x+=tmpo->speed*dt*Cos(tmpo->theta);
  tmpo->y+=tmpo->speed*dt*Sin(tmpo->theta);
  tmpo->theta=Wrap(tmpo->theta+tmpo->d_theta*dt);
  tmpo->d_theta=LowPass1(0.5,tmpo->d_theta,0,dt);
}

U0 SongTask(I64)
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  Play("M/");
  Play("hR5qDhGeBG");
  Play("5hBqAhGqE");
  Play("5hDqDhGeBG");
  Play("5hBqA6h.DhD");
  Fs->parent_task->song_task=NULL;
}

#define SCRN_BUMP	100

U0 AnimateTask(I64)
{
  I64 i,j,
	w=Fs->parent_task->pix_width,  cx=w/2,
	h=Fs->parent_task->pix_height, cy=h/2;
  F64 die_timer=0,d,dx,dy,dx2,dy2,dxn,dyn,t0=tS,dt,old_view_scale;

  while (TRUE) {
    if (TaskValidate(pray_task)) {
      DeathWait(&pray_task);
      pray_task=NULL;
      t0=tS;
    }

    dt=tS-t0;
    t0=tS;

    for (die_timer+=dt;die_timer>0.1;die_timer-=0.1)
      if (Rand<0.05)
	men*=Exp(-0.05*(Rand+0.05)`20);

    old_view_scale=view_scale;
    view_scale=Exp(0.5*dt*(Ln(new_view_scale)-Ln(view_scale))+Ln(view_scale));
    scrn_x=(scrn_x+cx)*view_scale/old_view_scale-cx;
    scrn_y=(scrn_y+cy)*view_scale/old_view_scale-cy;

    for (i=0;i<DOLPHINS_NUM;i++) {
      dolphins[i].d_theta+=10.0*(Rand-0.5)*dt;
      AnimateObj(&dolphins[i],dt);
      if (dolphins[i].x-scrn_x<dolphins[i].radius)
	dolphins[i].x+=(w-2*dolphins[i].radius)/view_scale;
      if (dolphins[i].x-scrn_x>w-1-dolphins[i].radius)
	dolphins[i].x-=(w-2*dolphins[i].radius)/view_scale;
      if (dolphins[i].y-scrn_y<dolphins[i].radius)
	dolphins[i].y+=(h-2*dolphins[i].radius)/view_scale;
      if (dolphins[i].y-scrn_y>h-1-dolphins[i].radius)
	dolphins[i].y-=(h-2*dolphins[i].radius)/view_scale;
    }

    if (ship_landed) {
      for (i=0;i<PIGRAIMS_NUM;i++) {
	pilgrims[i].d_theta+=10.0*(Rand-0.5)*dt;
	AnimateObj(&pilgrims[i],dt);
	if (GrPeek(master_map,pilgrims[i].x,pilgrims[i].y)!=1) {
	  pilgrims[i].speed=-pilgrims[i].speed;
	  AnimateObj(&pilgrims[i],dt);
	} else if (!(ToI64(pilgrims[i].x)&15) && !(ToI64(pilgrims[i].y)&15)) {
	  for (j=0;j<GARDENS_NUM;j++)
	    if (gardens[j].x==ToI64(pilgrims[i].x)&&
		  gardens[j].y==ToI64(pilgrims[i].y))
	      break;
	  if (j==GARDENS_NUM) {
	    gardens[num_gardens  &(GARDENS_NUM-1)].x=pilgrims[i].x;
	    gardens[num_gardens++&(GARDENS_NUM-1)].y=pilgrims[i].y;
	  }
	}
      }
      if (church_x*view_scale-scrn_x-0.9*d_scrn_x<w/4)
	d_scrn_x-=SCRN_BUMP;
      if (church_x*view_scale-scrn_x-0.9*d_scrn_x>w-1-w/4)
	d_scrn_x+=SCRN_BUMP;
      if (church_y*view_scale-scrn_y-0.9*d_scrn_y<h/4)
	d_scrn_y-=SCRN_BUMP;
      if (church_y*view_scale-scrn_y-0.9*d_scrn_y>h-1-h/4)
	d_scrn_y+=SCRN_BUMP;
    } else {
      d=Wrap(ship.theta-Wrap(wave_theta,ship.theta-pi));
      if (Abs(d)<pi)
	d=(Abs(d)-pi)*Sign(-d);
      d=wind_speed*Cos(d/2);
      ship.speed=LowPass1(0.25,ship.speed,d,dt);
      AnimateObj(&ship,dt);
      if (ship.x*view_scale-scrn_x-0.9*d_scrn_x<w/4)
	d_scrn_x-=SCRN_BUMP;
      if (ship.x*view_scale-scrn_x-0.9*d_scrn_x>w-1-w/4)
	d_scrn_x+=SCRN_BUMP;
      if (ship.y*view_scale-scrn_y-0.9*d_scrn_y<h/4)
	d_scrn_y-=SCRN_BUMP;
      if (ship.y*view_scale-scrn_y-0.9*d_scrn_y>h-1-h/4)
	d_scrn_y+=SCRN_BUMP;
      if (ship.x<0) ship.x=0;
      if (ship.y<0) ship.y=0;
    }

    if (scrn_x<0) {
      scrn_x=0;
      d_scrn_x=0;
    }
    if (scrn_y<0) {
      scrn_y=0;
      d_scrn_x=0;
    }

    wave_theta=Wrap(wave_theta+d_wave_theta*dt);
    d_wave_theta=Clamp(LowPass1(1.0,d_wave_theta,
	  d_wave_theta+1.0*(Rand-0.5),dt),-0.05,0.05);

    scrn_x+=0.9*d_scrn_x*dt;
    d_scrn_x-=0.9*d_scrn_x*dt;
    scrn_y+=0.9*d_scrn_y*dt;
    d_scrn_y-=0.9*d_scrn_y*dt;

    dxn=Cos(wave_theta);
    dyn=Sin(wave_theta);
    dx=wave_period*dxn;
    dy=wave_period*dyn;
    ship_wave_x-=wave_speed*dt/(2*pi)*dx;
    ship_wave_y-=wave_speed*dt/(2*pi)*dy;
    if (Abs(dx)>Abs(dy)) {
      i=(ship_wave_x-ship.x)/dx;
      ship_wave_x-=i*dx;
      ship_wave_y-=i*dy;
      dx2=ship_wave_x-ship.x;
      dy2=ship_wave_y-ship.y;
      d=dyn*dx2-dxn*dy2;
      ship_wave_x-=dyn*d;
      ship_wave_y+=dxn*d;
    } else {
      i=(ship_wave_y-ship.y)/dy;
      ship_wave_x-=i*dx;
      ship_wave_y-=i*dy;
      dx2=ship_wave_x-ship.x;
      dy2=ship_wave_y-ship.y;
      d=dyn*dx2-dxn*dy2;
      ship_wave_x-=dyn*d;
      ship_wave_y+=dxn*d;
    }

    if (wave_sound)
      Snd(30+2*WaterHeight(ship.x,ship.y));
    else if (song_time && tS>song_time) {
      Kill(Fs->parent_task->song_task);
      Fs->parent_task->song_task=Spawn(&SongTask,NULL,"Song",,Fs->parent_task);
      song_time=0;
    }
    Refresh;
  }
}

U0 Pilgrims()
{
  I64 msg_code,ch,sc;
  CCtrl *bt_pray;

  "Initializing Maps...\n";
  master_map=GRRead("NewEngland");

  vm=DC2VectMap(master_map);
  PopUpOk("This is about the pilgrims who fled\n"
	"religious persecution in England.\n"
	"They sailed in 1620 on the Mayflower\n"
	"and landed at Plymouth, Massachusetts.\n"
	"\n"
	"This game is comic, mostly.  Be sure\n"
	"to try 'z' or 'Z' for zooming.\n");
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Pray(,'p');"
	"  Port(,,SC_CURSOR_LEFT);"
	"  Starboard(,,SC_CURSOR_RIGHT);"
	"}"
	"View {"
	"  ZoomIn(,'z');"
	"  ZoomOut(,'Z');"
	"}"
	);


  bt_pray=CtrlBttnNew(0,2.5*FONT_HEIGHT,,,,"Pray\0",pray_colors),
	bt_pray->left_click=&LeftClickPrayBttn;

  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  Fs->song_task=NULL;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  try {
    while (TRUE) {
      switch (msg_code=GetMsg(&ch,&sc,1<<MSG_KEY_DOWN|1<<MSG_KEY_UP)) {
	case MSG_KEY_DOWN:
	  switch (ch) {
	    case 0:
	      switch (sc.u8[0]) {
		case SC_CURSOR_LEFT:
		  ship.d_theta=Clamp(ship.d_theta-5*pi/180,-.5,.5);
		  break;
		case SC_CURSOR_RIGHT:
		  ship.d_theta=Clamp(ship.d_theta+5*pi/180,-.5,.5);
		  break;
	      }
	      break;
	    case 'p':
	      LeftClickPrayBttn(NULL,0,0,TRUE);
	      break;
	    case 'z':
	      if (new_view_scale<10.0)
		new_view_scale*=1.5;
	      break;
	    case 'Z':
	      if (new_view_scale>0.1)
		new_view_scale/=1.5;
	      break;
	    case '\n':
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto pilgrim_done;
	  }
	  break;
      }
    }
pilgrim_done:
  } catch
    PutExcept;
  MenuPop;
  SettingsPop;
  CtrlBttnDel(bt_pray);
  VectMapDel(vm);
  DCDel(master_map);
}

//7) The pilgrim game map is from US Geological Survey.
//Scrn captured and edited with MS Paint and saved as .BMP and
//imported into TempleOS and converted with a program I wrote.)

Pilgrims;





///* Pilgrims <8> (image) */
