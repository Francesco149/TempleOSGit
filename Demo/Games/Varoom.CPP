




    /* <1> <1> (image) */



    /* <2> <2> (image) */







    /* <3> <3> (image) */




















    /* <4> <4> (image) */









		 /* <5> <5> (image) */



























		 /* <6> <6> (image) */












RegSetDftEntry("TempleOS/Varoom","F64 best_score=9999;\n");
RegExeBranch("TempleOS/Varoom");

F64 distance,t0,tf;
Bool game_over;

#define BORDER		7500
#define R		10000
#define WIDTH		2000
#define SHOULDER	200
#define D_theta		(2*pi/360)   //Curve track slice is one degree.
#define D_S		(2*pi*R/360) //Straight track slice is degree at 10000.
#define DIPS		5
#define DIP_DEPTH	50
class Track
{
  Track *next,*last;
  I32 num;
  CColorROPU16 c,pad;
  I64 x,z;
  F64 theta,d;
  CD3I32 left[4],center[4],right[4];
} track_root,
  *track_start[MP_MAX_PROCESSORS],*track_end[MP_MAX_PROCESSORS];

CDC *track_map;
#define MAP_BITS	9
I64 t_minx,t_maxx,t_minz,t_maxz;

#define NUM_BUSHES	512
class Bush
{
  CD3I32 p;
  Bool sym,pad[3];
  U8 *img;
} b[NUM_BUSHES];

#define NUM_CARS	8
class Car
{
  CD3I32 p;
  F64 theta,dtheta,speed;
  CSprite *img;
  Track *t;
} c[NUM_CARS];

I64 DipY(I64 x,I64 z)
{
  F64 m,a;
  R2P(&m,&a,x,z);
  return DIP_DEPTH*m*Cos(DIPS*a)/R;
}

#define CAR_LENGTH	400

F64 Diptheta(I64 x,I64 z,F64 theta)
{
  F64 y_front,y_back;
  y_front=DipY(x-CAR_LENGTH/2*Cos(theta),z-CAR_LENGTH/2*Sin(theta));
  y_back =DipY(x+CAR_LENGTH/2*Cos(theta),z+CAR_LENGTH/2*Sin(theta));
  return ASin((y_front-y_back)/CAR_LENGTH);
}

Track *TrackFind(Track *_tempt,I64 x,I64 z)
{
  Track *result=_tempt,*tempt;
  I64 dd,best=SqrI64(result->x-x)+SqrI64(result->z-z);

  tempt=_tempt;
  while (TRUE) {
    tempt=tempt->next;
    if (tempt==&track_root)
      tempt=tempt->next;
    dd=SqrI64(tempt->x-x)+SqrI64(tempt->z-z);
    if (dd<best) {
      best=dd;
      result=tempt;
    } else
      break;
  }
  tempt=_tempt;
  while (TRUE) {
    tempt=tempt->last;
    if (tempt==&track_root)
      tempt=tempt->last;
    dd=SqrI64(tempt->x-x)+SqrI64(tempt->z-z);
    if (dd<best) {
      best=dd;
      result=tempt;
    } else
      break;
  }
  return result;
}

U0 TrackSlice(F64 *_x,F64 *_z,F64 theta,F64 d)
{
  F64 x=*_x,z=*_z,c=Cos(theta),s=Sin(theta),dx=d*s,dz=-d*c;
  Track *tempt,*last=track_root.last;
  if (last==&track_root)
    last=NULL;

  tempt=CAlloc(sizeof(Track));
  if (last) {
    MemCpy(&tempt->center[0],&last->center[3],sizeof(CD3I32));
    MemCpy(&tempt->center[1],&last->center[2],sizeof(CD3I32));
  }
  tempt->center[2].x=x+(WIDTH/2)*c+dx;
  tempt->center[2].z=z+(WIDTH/2)*s+dz;
  tempt->center[2].y=DipY(tempt->center[2].x,tempt->center[2].z);
  tempt->center[3].x=x-(WIDTH/2)*c+dx;
  tempt->center[3].z=z-(WIDTH/2)*s+dz;
  tempt->center[3].y=DipY(tempt->center[3].x,tempt->center[3].z);

  if (last) {
    MemCpy(&tempt->left[0],&last->left[3],sizeof(CD3I32));
    MemCpy(&tempt->left[1],&last->left[2],sizeof(CD3I32));
  }
  tempt->left[2].x=x-(WIDTH/2)*c+dx;
  tempt->left[2].z=z-(WIDTH/2)*s+dz;
  tempt->left[2].y=DipY(tempt->left[2].x,tempt->left[2].z);
  tempt->left[3].x=x-(WIDTH/2+SHOULDER)*c+dx;
  tempt->left[3].z=z-(WIDTH/2+SHOULDER)*s+dz;
  tempt->left[3].y=DipY(tempt->left[3].x,tempt->left[3].z);

  if (last) {
    MemCpy(&tempt->right[0],&last->right[3],sizeof(CD3I32));
    MemCpy(&tempt->right[1],&last->right[2],sizeof(CD3I32));
  }
  tempt->right[2].x=x+(WIDTH/2+SHOULDER)*c+dx;
  tempt->right[2].z=z+(WIDTH/2+SHOULDER)*s+dz;
  tempt->right[2].y=DipY(tempt->right[2].x,tempt->right[2].z);
  tempt->right[3].x=x+(WIDTH/2)*c+dx;
  tempt->right[3].z=z+(WIDTH/2)*s+dz;
  tempt->right[3].y=DipY(tempt->right[3].x,tempt->right[3].z);

  tempt->x=x;
  tempt->z=z;
  tempt->theta=theta;
  tempt->num=track_root.last->num+1;
  tempt->d  =track_root.last->d+d;

  QueIns(tempt,track_root.last);

  if (tempt->num&1)
    tempt->c=RED;
  else
    tempt->c=WHITE;

  if (x<t_minx) t_minx=x;
  if (x>t_maxx) t_maxx=x;
  if (z<t_minz) t_minz=z;
  if (z>t_maxz) t_maxz=z;

  x+=dx;
  *_x=x;
  z+=dz;
  *_z=z;

  if (x<t_minx) t_minx=x;
  if (x>t_maxx) t_maxx=x;
  if (z<t_minz) t_minz=z;
  if (z>t_maxz) t_maxz=z;
}

U0 CoupleEnds()
{
  Track *first=track_root.next,*last=track_root.last;
  MemCpy(&first->center[0],&last->center[3],sizeof(CD3I32));
  MemCpy(&first->center[1],&last->center[2],sizeof(CD3I32));
  MemCpy(&first->left[0]  ,&last->left[3]  ,sizeof(CD3I32));
  MemCpy(&first->left[1]  ,&last->left[2]  ,sizeof(CD3I32));
  MemCpy(&first->right[0] ,&last->right[3] ,sizeof(CD3I32));
  MemCpy(&first->right[1] ,&last->right[2] ,sizeof(CD3I32));
}

U0 InitTrack()
{
  I64 i,j;
  Track *tempt;
  F64 x,z,theta,d;

  MemSet(&track_root,0,sizeof(Track));
  QueInit(&track_root);

  t_minx=t_minz=MAX_I64;
  t_maxx=t_maxz=MIN_I64;

  x=0; z=0; theta=0;
  for (d=0;d<6*R;d+=D_S)
    TrackSlice(&x,&z,theta,D_S);
  for (i=0;i<180;i++,theta+=D_theta)
    TrackSlice(&x,&z,theta,D_theta*R);
  for (d=0;d<R;d+=D_S)
    TrackSlice(&x,&z,theta,D_S);
  for (i=0;i<90;i++,theta-=D_theta)
    TrackSlice(&x,&z,theta,D_theta*R);
  for (i=0;i<180;i++,theta+=D_theta)
    TrackSlice(&x,&z,theta,D_theta*R);
  for (i=0;i<90;i++,theta-=D_theta)
    TrackSlice(&x,&z,theta,D_theta*R);
  for (d=0;d<R;d+=D_S)
    TrackSlice(&x,&z,theta,D_S);
  for (i=0;i<180;i++,theta+=D_theta)
    TrackSlice(&x,&z,theta,D_theta*R);

  CoupleEnds;

  tempt=track_root.next;
  for (i=0;i<mp_cnt;i++) {
    j=(i+1)*track_root.last->num/mp_cnt+1;
    track_start[i]=tempt;
    while (tempt!=&track_root && tempt->num!=j)
      tempt=tempt->next;
    track_end[i]=tempt;
  }

  t_minx-=BORDER; t_minz-=BORDER;
  t_maxx+=BORDER; t_maxz+=BORDER;
  track_map=DCNew((t_maxx-t_minx+1<<MAP_BITS-1)>>MAP_BITS,
	(t_maxz-t_minz+1<<MAP_BITS-1)>>MAP_BITS);

  track_map->color=LTGRAY;
  GrRect(track_map,0,0,track_map->width,track_map->height);
  tempt=track_root.next;
  track_map->color=YELLOW;
  track_map->pen_width=3;
  while (tempt!=&track_root) {
    GrPlot3(track_map,track_map->width-(tempt->x-t_minx)>>MAP_BITS,
	  (tempt->z-t_minz)>>MAP_BITS,0);
    tempt=tempt->next;
  }
}

#define HORIZON_DIP	200

Bool PrepPoly(CD3I32 *p,I64 *r,I64 cx,I64 h,CD3I32 *poly)
{
  I64 x,y,z,i;
  F64 s;
  for (i=0;i<4;i++) {
    x=p[i].x-c[0].p.x;
    y=p[i].y-c[0].p.y;
    z=p[i].z-c[0].p.z;
    Mat4x4MulXYZ(r,&x,&y,&z);
    s=100.0/(AbsI64(z)+50);
    poly[i].y=s*y+h;
    if (z<-200 || !(-h<poly[i].y<2*h))
      return FALSE;
    poly[i].x=s*x+cx;
    poly[i].z=z+GR_Z_ALL;
  }
  return TRUE;
}

I64 mp_not_done_flags;

U0 MPUpdateWin(CDC *dc2)
{
  CTask *task=dc2->win_task;
  I64	i,x,y,z,
	w=task->pix_width,h=task->pix_height,r[16],cx=w>>1;
  F64	s,dip_theta=Diptheta(c[0].p.x,c[0].p.z,c[0].theta);
  Car	*tempc;
  CD3I32 poly[4];
  Track	*tempt,*tempt1;
  CDC	*dc=DCAlias(gr.dc2,task);

  Mat4x4IdentEqu(r);
  Mat4x4RotY(r,pi-c[0].theta);
  Mat4x4RotX(r,75*pi/180-dip_theta);

  dc->depth_buf=dc2->depth_buf;

  //Track
  tempt =track_start[Gs->num];
  tempt1=track_end  [Gs->num];
  while (tempt!=tempt1) {
    dc->color=DKGRAY;
    if (PrepPoly(&tempt->center,r,cx,h,poly)) {
      GrFillPoly3(dc,4,poly);
      dc->color=tempt->c;
      if (PrepPoly(&tempt->left,r,cx,h,poly))
	GrFillPoly3(dc,4,poly);
      if (PrepPoly(&tempt->right,r,cx,h,poly))
	GrFillPoly3(dc,4,poly);
    }
    tempt=tempt->next;
  }

  dc->flags|=DCF_TRANSFORMATION;
  for (i=Gs->num;i<NUM_BUSHES;i+=mp_cnt) {
    x=b[i].p.x-c[0].p.x; y=b[i].p.y-c[0].p.y; z=b[i].p.z-c[0].p.z;
    Mat4x4MulXYZ(r,&x,&y,&z);
    if (z>0) {
      s=100.0/(AbsI64(z)+50);
      Mat4x4IdentEqu(dc->r);
      Mat4x4Scale(dc->r,s*2);
      DCMat4x4Set(dc,dc->r);
      if (b[i].sym) {
	dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
	DCSymmetrySet(dc,s*x+cx,s*y+h,s*x+cx,s*y+h+10);
      }
      Sprite3B(dc,s*x+cx,s*y+h,z+GR_Z_ALL,b[i].img);
      dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    }
  }
  for (i=Gs->num+1;i<NUM_CARS;i+=mp_cnt) {
    tempc=&c[i];
    x=tempc->p.x-c[0].p.x; y=tempc->p.y-c[0].p.y; z=tempc->p.z-c[0].p.z;
    Mat4x4MulXYZ(r,&x,&y,&z);
    if (z>0) {
      s=100.0/(AbsI64(z)+50);
      Mat4x4IdentEqu(dc->r);
      Mat4x4Scale(dc->r,s*2);
      Mat4x4RotX(dc->r,Diptheta(tempc->p.x,tempc->p.z,-tempc->theta));
      Mat4x4RotY(dc->r,tempc->theta-c[0].theta);
      DCMat4x4Set(dc,dc->r);
      Sprite3B(dc,s*x+cx,s*y+h,z+GR_Z_ALL,tempc->img);
    }
  }
  dc->depth_buf=NULL;
  DCDel(dc);
  LBtr(&mp_not_done_flags,Gs->num);
}

U0 VRTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  I64 zz;
  Mat4x4MulXYZ(dc->r,x,y,z);
  zz=400+*z;
  if (zz<1) zz=1;
  *x=400* *x/zz;
  *y=400* *y/zz;
  *x+=dc->x;
  *y+=dc->y;
  *z+=dc->z;
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64	i,x,y,z,
	w=task->pix_width,
	h=task->pix_height,r[16],
	cx=w>>1;
  F64	s,dip_theta=Diptheta(c[0].p.x,c[0].p.z,c[0].theta);
  Car	*tempc=&c[0];

  dc->color=LTCYAN;
  GrRect(dc,0,0,w,HORIZON_DIP*Sin(dip_theta)+FONT_HEIGHT*4.5);

  Mat4x4IdentEqu(r);
  Mat4x4RotY(r,pi-c[0].theta);
  Mat4x4RotX(r,75*pi/180-dip_theta);

  DCDepthBufAlloc(dc);

  //Sun
  x=c[0].p.x; y=0; z=1000000-c[0].p.z;
  Mat4x4MulXYZ(r,&x,&y,&z);
  s=100.0/(AbsI64(z)+50);
  if (y<0) {
    dc->color=BROWN;
    GrCircle(dc,s*x+cx,15+HORIZON_DIP*Sin(dip_theta),15);
    dc->color=YELLOW;
    GrFloodFill(dc,s*x+cx,15+HORIZON_DIP*Sin(dip_theta));
  }

  mp_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    JobQue(&MPUpdateWin,dc,i);
  while (mp_not_done_flags)
    Yield;

  Mat4x4IdentEqu(r);
  Mat4x4RotY(r,tempc->dtheta);
  Mat4x4RotX(r,0.4-8*dip_theta); //Made this up
  dc->transform=&VRTransform;
  dc->x=task->pix_width>>1;
  dc->y=task->pix_height-150;
  dc->z=GR_Z_ALL;
  SpriteMat3B(dc,0,0,-100,c[0].img,r);

  //Map
  GrBlot(dc,w-track_map->width,h-track_map->height,track_map);

  dc->pen_width=2;
  for (i=0;i<NUM_CARS;i++) {
    if (i)
      dc->color=LTPURPLE;
    else
      dc->color=LTCYAN;
    GrPlot3(dc,w-(c[i].p.x-t_minx)>>MAP_BITS,
	       h-track_map->height+(c[i].p.z-t_minz)>>MAP_BITS,0);
  }

  if (game_over) {
    dc->color=LTRED;
    if (tf) {
      s=tf-t0;
      if (Blink)
	GrPrint(dc,(w-FONT_WIDTH*14)/2,(h-FONT_HEIGHT)/2,"Game Completed");
    } else {
      s=99.9;
      if (Blink)
	GrPrint(dc,(w-FONT_WIDTH*9)/2,(h-FONT_HEIGHT)/2,"Game Over");
    }
  } else
    s=tS-t0;
  dc->color=BLACK;
  GrPrint(dc,0,0,"%0.1f%% Time:%0.2f Best:%0.2f",
	100.0*distance/track_root.last->d,s,best_score);
}

U0 AnimateTask(I64)
{
  Car  *tempc;
  I64 i,x,z;
  Bool on_track;
  Track *tempt,*tempt2;
  while (TRUE) {
    if (!game_over)
      Snd(0.01*c[0].speed*3+10);
    else
      Snd(0);
    for (i=0;i<NUM_CARS;i++) {
      tempc=&c[i];
      tempc->p.x-=0.01*tempc->speed*Cos(tempc->theta-pi/2);
      tempc->p.z+=0.01*tempc->speed*Sin(tempc->theta-pi/2);
      tempt=TrackFind(tempc->t,tempc->p.x,tempc->p.z);
      if (i) {
	if (tempt!=tempc->t) {
	  tempt2=tempt->next;
	  if (tempt2==&track_root)
	    tempt2=tempt2->next;
	  tempc->theta=Arg(-tempt2->z+tempc->p.z,-tempt2->x+tempc->p.x);
	}
      } else {
	tempc->theta+=0.01*tempc->dtheta;
	x=track_map->width-(tempc->p.x-t_minx)>>MAP_BITS;
	z=(tempc->p.z-t_minz)>>MAP_BITS;
	if (GrPeek(track_map,x,z)!=YELLOW) {
	  on_track=FALSE;
	  tempc->speed-=0.01*tempc->speed;
	  if (tempc->speed<0) tempc->speed=0;
	} else
	  on_track=TRUE;
      }
      tempc->t=tempt;
      tempc->p.y=DipY(tempc->p.x,tempc->p.z);
    }
    if (!game_over && on_track) {
      for (i=1;i<NUM_CARS;i++)
	if (D3I32DistSqr(&c[i].p,&c[0].p)<CAR_LENGTH>>1*CAR_LENGTH>>1) {
	  game_over=TRUE;
	  Noise(500,50,100);
	  Sleep(500);
	  break;
	}
      if (!game_over) {
	distance+=0.01*c[0].speed;
	if (distance>track_root.last->d&& c[0].t->num<track_root.last->num>>1) {
	  tf=tS;
	  game_over=TRUE;
	  Beep;
	  if (tf-t0<best_score) {
	    best_score=tf-t0;
	    Beep;
	  }
	}
      }
    }
    Sleep(10);
  }
}

CSprite *imgs[8]={__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4};


U0 InitBushes()
{
  Bush *tempb;
  I64 i,j,x,z;
  track_map->color=LTGREEN;
  track_map->pen_width=1;
  for (i=0;i<NUM_BUSHES;i++) {
    tempb=&b[i];

ib_restart:
    tempb->p.x=Rand*(t_maxx-t_minx)+t_minx;
    tempb->p.z=Rand*(t_maxz-t_minz)+t_minz;
    x=track_map->width-(tempb->p.x-t_minx)>>MAP_BITS;
    z=(tempb->p.z-t_minz)>>MAP_BITS;
    for (j=0;j<8;j++)
      if (GrPeek(track_map,x+gr_x_offsets[j],z+gr_y_offsets[j])!=LTGRAY)
	goto ib_restart;

    GrPlot(track_map,x,z);
    tempb->p.y=DipY(tempb->p.x,tempb->p.z);
    tempb->sym=RandU16&1;
    tempb->img=imgs[i&7];
  }
}


U0 Init()
{
  Car	*tempc;
  Track *tempt;
  F64	d;
  I64	i;

  InitTrack;
  InitBushes;
  tempt=track_root.next;
  for (i=0;i<NUM_CARS;i++) {
    tempc=&c[i];
    tempc->t=tempt;
    tempc->p.x=tempt->x;
    tempc->p.z=tempt->z;
    tempc->p.y=DipY(tempc->p.x,tempc->p.z);
    tempc->theta  =-tempt->theta;
    tempc->dtheta=0;
    if (!i) {
      tempc->img=__BIN_5;
      tempc->speed=0;
    } else {
      tempc->img=__BIN_6;
      tempc->speed=2500.0;
    }
    d=(i+1)*track_root.last->d/NUM_CARS;
    while (tempt->next!=&track_root && tempt->d<d)
      tempt=tempt->next;
  }
  distance=0;
  tf=0;
  t0=tS;
  game_over=FALSE;
}

U0 CleanUp()
{
  while (mp_not_done_flags)
    Yield;
  QueDel(&track_root,TRUE);
  DCDel(track_map);
}

U0 Varoom()
{
  I64 sc;
  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  Accelerator(,,SC_CURSOR_UP);"
	"  Brake(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  try {
    Fs->text_attr=YELLOW<<4+BLUE;
    Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS
	  -WIF_SELF_GRAB_SCROLL-WIF_FOCUS_TASK_MENU;
    AutoComplete;
    WinBorder;
    WinMax;
    DocCursor;
    DocClear;
    Init;
    Fs->draw_it=&DrawIt;
    Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
    while (TRUE)
      switch (GetKey(&sc)) {
	case 0:
	  switch (sc.u8[0]) {
	    case SC_CURSOR_LEFT:
	      c[0].dtheta-=pi/60;
	      break;
	    case SC_CURSOR_RIGHT:
	      c[0].dtheta+=pi/60;
	      break;
	    case SC_CURSOR_UP:
	      c[0].speed+=300;
	      break;
	    case SC_CURSOR_DOWN:
	      c[0].speed-=900;
	      if (c[0].speed<0) c[0].speed=0;
	      break;
	  }
	  break;
	case '\n':
	  CleanUp;
	  Init;
	  break;
	case CH_SHIFT_ESC:
	case CH_ESC:
	  goto vr_done;
      }
vr_done: //Don't goto out of try
  } catch
    PutExcept;
  CleanUp;
  SettingsPop;
  MenuPop;
  RegWriteBranch("TempleOS/Varoom","F64 best_score=%5.4f;\n",best_score);
}

Varoom;
