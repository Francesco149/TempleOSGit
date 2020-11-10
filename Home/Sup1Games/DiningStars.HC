//This uses [C:/Demo/Lectures/FixedPoint.HC.Z,1] fixed-point.


RegDft("TempleOS/DiningStars","F64 best_score=9999;\n");
RegExe("TempleOS/DiningStars");

#define PERSON_SCALE	6.0










		/* <1> <1> (image) */









		/* <2> <2> (image) */








		/* <3> <3> (image) */






#define PLANT_SCALE	6.0



		     /* <4> <4> (image) */














		      /* <5> <5> (image) */







class Wall
{
  Wall *next,*last;
  CD3I32 p1,p2;
  I64 cpu_num;
} walls_head;




#define SCRN_SCALE		1024
#define PLOT_GRID_WIDTH		24
#define PLOT_GRID_HEIGHT	24
#define MAN_HEIGHT		300
#define WALL_HEIGHT		1.0
#define BLUEPRINT_SCALE		4

CDC *blueprint_dc;
I64 blueprint_width,blueprint_height;
U8 *blueprint=NULL,
   *panels_processed=NULL;

I64 man_xx,man_yy;
F64 man_theta;

F64 t0,tf;

#define PERSONS_NUM	16
class Person
{
  I64 x,y;
} persons[PERSONS_NUM];

U0 DSTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  I64 zz;
  Mat4x4MulXYZ(dc->r,x,y,z);
  zz=SCRN_SCALE/3+*z;
  if (zz<1) zz=1;
  *x=SCRN_SCALE/2* *x/zz;
  *y=SCRN_SCALE/2* (*y+MAN_HEIGHT)/zz;
  *x+=dc->x;
  *y+=dc->y;
  *z+=dc->z;
}

public U0 DSLighting(CDC *dc,CD3I32 *p1,CD3I32 *p2,CD3I32 *p3,
	CColorROPU32 color)
{
  CD3I32 v1,v2;
  I64 i,vn_x,vn_y,vn_z;
  F64 d;

  v1.x=p1->x-p2->x;
  v1.y=p1->y-p2->y;
  v1.z=p1->z-p2->z;

  v2.x=p3->x-p2->x;
  v2.y=p3->y-p2->y;
  v2.z=p3->z-p2->z;

  //V1 and V2 are vects along two sides
  //of the tri joined at p2.

  vn_x=v1.y*v2.z-v1.z*v2.y;
  vn_y=v1.z*v2.x-v1.x*v2.z;
  vn_z=v1.x*v2.y-v1.y*v2.x;
  if (d=Sqrt(SqrI64(vn_x)+SqrI64(vn_y)+SqrI64(vn_z)))
    d=1<<16/d;
  vn_x*=d;
  vn_y*=d;
  vn_z*=d;
//Vn is the cross product of V1 and V3
  //which means it is perpendicular.  It
  //is the normal vect to the surface.
  //It has been scaled to length 65536.

  //Light source has been scaled to length 65536.
  i=(vn_x*dc->ls.x+vn_y*dc->ls.y+vn_z*dc->ls.z)>>16;

  i=(0x8000+i)>>1;//Tables are too dark ****TODO****

  //The dot product of the light source
  //vect and the surface normal
  //gives an illumination number.
  //65536*65536>>16=65536

  //TempleOS will generate a random U16
  //and compare to dither_probability_u16 and
  //will pick from two colors.
  //Probability dithering does not work with thick>1 at this time.
  if (color.c0.rop&ROPBF_TWO_SIDED) {
    color.c0.rop&=~ROPBF_TWO_SIDED;
    i=AbsI64(i)<<1;
  } else
    i+=65536;
  if (color.c0.rop&ROPBF_HALF_RANGE_COLOR) {
    color.c0.rop&=~ROPBF_HALF_RANGE_COLOR;
    i>>=1;
    if (color>=8) {
      color-=8;
      i+=65536;
    }
  }
  if (i<65536) {
    dc->color=ROPF_PROBABILITY_DITHER+color<<16+BLACK;
    dc->dither_probability_u16=i;
  } else {
    dc->color=ROPF_PROBABILITY_DITHER+(color^8)<<16+color;
    dc->dither_probability_u16=i-65536;
  }
}

#define LOS_SCALE	4

Bool LOSPlot(U8 *,I64 x,I64 y,I64)
{
  if (blueprint[(y/LOS_SCALE)*blueprint_width+(x/LOS_SCALE)]==WHITE)
    return FALSE;
  else
    return TRUE;
}

Bool LOS(I64 x1,I64 y1,I64 x2,I64 y2)
{ //Line of sight
  return Line(NULL,x1*LOS_SCALE/SCRN_SCALE,y1*LOS_SCALE/SCRN_SCALE,0,
	x2*LOS_SCALE/SCRN_SCALE,y2*LOS_SCALE/SCRN_SCALE,0,&LOSPlot);
}

I64 mp_not_done_flags;
I64 *r1,*r2,*r3,*s2w,cx,cy,xh,yh,zh,x1h,y1h;

U0 MPDrawWalls(CDC *_dc)
{
  CD3I32 poly[4];
  I64 i,j,n,cpu_num=Gs->num,xx,yy,x,y,x1,y1,z1,x2,y2,_x1h,_y1h,
	dx,dy,xx1,yy1,xx2,yy2,c,x1w,y1w,*_r3=Mat4x4New(r3);
  Wall *tmpw;
  CDC *dc=MAllocIdent(_dc);
  dc->r=Mat4x4New(_dc->r);
  _x1h=x1h-xh*cpu_num;
  _y1h=y1h-yh*cpu_num;
  for (j=cpu_num;j<PLOT_GRID_HEIGHT*2;j+=mp_cnt) {
    x1w=_x1h;
    y1w=_y1h;
    for (i=0;i<PLOT_GRID_WIDTH*4;i++) {
      xx=x1w/SCRN_SCALE; yy=y1w/SCRN_SCALE;
      x=xx*SCRN_SCALE-man_xx; y=yy*SCRN_SCALE-man_yy;
      if (1<=xx<blueprint_width-1 && 1<=yy<blueprint_height-1 &&
	    !LBts(panels_processed,yy*blueprint_width+xx)) {
	if ((c=blueprint[yy*blueprint_width+xx]) &&
	      LOS(xx*SCRN_SCALE+SCRN_SCALE/2,
	      yy*SCRN_SCALE+SCRN_SCALE/2,
	      man_xx,man_yy)) {
	  if (c==WHITE||c==LTBLUE)
	    dc->color=BROWN;
	  else
	    dc->color=c;
	  poly[0].x=x;
	  poly[0].y=y;
	  poly[0].z=0;
	  poly[1].x=x+SCRN_SCALE;
	  poly[1].y=y;
	  poly[1].z=0;
	  poly[2].x=x+SCRN_SCALE;
	  poly[2].y=y+SCRN_SCALE;
	  poly[2].z=0;
	  poly[3].x=x;
	  poly[3].y=y+SCRN_SCALE;
	  poly[3].z=0;
	  GrFillPoly3(dc,4,poly);
	  if (!((yy*blueprint_width+xx)%3)) {
	    if (c==GREEN) {
	      x1=x+SCRN_SCALE/2;
	      y1=y+SCRN_SCALE/2;
	      z1=0;
	      DCTransform(dc,&x1,&y1,&z1);
	      if (z1>0)
		Sprite3Mat4x4B(dc,x+SCRN_SCALE/2,y+SCRN_SCALE/2,0,__BIN_4,_r3);
	    } else if (c==BROWN) {
	      x1=x+SCRN_SCALE/2;
	      y1=y+SCRN_SCALE/2;
	      z1=0;
	      DCTransform(dc,&x1,&y1,&z1);
	      if (z1>0)
		Sprite3Mat4x4B(dc,x+SCRN_SCALE/2,y+SCRN_SCALE/2,0,__BIN_5,_r3);
	    }
	  }
	}
      }
      x1w-=yh;
      y1w+=xh;
    }
    _x1h-=xh*mp_cnt;
    _y1h-=yh*mp_cnt;
  }

  tmpw=walls_head.next;
  while (tmpw!=&walls_head) {
    if (tmpw->cpu_num==cpu_num) {
      dc->color=YELLOW;
      x1=tmpw->p1.x*SCRN_SCALE/BLUEPRINT_SCALE-man_xx;
      y1=tmpw->p1.y*SCRN_SCALE/BLUEPRINT_SCALE-man_yy;
      x2=tmpw->p2.x*SCRN_SCALE/BLUEPRINT_SCALE-man_xx;
      y2=tmpw->p2.y*SCRN_SCALE/BLUEPRINT_SCALE-man_yy;

      //The foreshortening distorts Z coordinates in a funny way,
      //so we do the wall in tiny pieces.
      n=2*(AbsI64(x2-x1)+AbsI64(y2-y1))/SCRN_SCALE;
      if (n<1) n=1;
      dx=(x2-x1)<<16/n;
      dy=(y2-y1)<<16/n;
      xx1=x1<<16;
      yy1=y1<<16;
      for (i=0;i<n;i++) {
	xx2=xx1+dx;
	yy2=yy1+dy;
	poly[0].x=xx1>>16;
	poly[0].y=yy1>>16;
	poly[0].z=0;
	poly[1].x=xx2>>16;
	poly[1].y=yy2>>16;
	poly[1].z=0;
	poly[2].x=xx2>>16;
	poly[2].y=yy2>>16;
	poly[2].z=WALL_HEIGHT*SCRN_SCALE;
	poly[3].x=xx1>>16;
	poly[3].y=yy1>>16;
	poly[3].z=WALL_HEIGHT*SCRN_SCALE;
	GrFillPoly3(dc,4,poly);
	xx1=xx2; yy1=yy2;
      }
      dc->color=BLACK;
      dc->thick=1;
      GrLine3(dc,x1,y1,0,x1,y1,WALL_HEIGHT*SCRN_SCALE);
      GrLine3(dc,x2,y2,0,x2,y2,WALL_HEIGHT*SCRN_SCALE);
    }
    tmpw=tmpw->next;
  }
  Free(_r3);
  Free(dc->r);
  Free(dc);
  LBtr(&mp_not_done_flags,cpu_num);
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,j,xx,yy,zz,x,y;
  U8 *tmps;
  F64 tt;

  cx=task->pix_width/2;
  cy=task->pix_height/2;

  DCDepthBufAlloc(dc);
  MemSet(panels_processed,0,(blueprint_width*blueprint_height+7)>>3);

  //World to scrn
  Mat4x4RotZ(dc->r,man_theta+pi/2);
  Mat4x4RotX(dc->r,pi/2);
  DCMat4x4Set(dc,dc->r);

  xh=-man_xx/SCRN_SCALE; yh=-man_yy/SCRN_SCALE; zh=0;
  Mat4x4MulXYZ(dc->r,&xh,&yh,&zh);
  Mat4x4TranslationEqu(dc->r,xh,yh,zh);

  //Scrn to world
  s2w=Mat4x4IdentNew(task);
  Mat4x4RotX(s2w,-pi/2);
  Mat4x4RotZ(s2w,-man_theta-pi/2);
  xh=0; yh=0; zh=SCRN_SCALE;
  Mat4x4MulXYZ(s2w,&xh,&yh,&zh);

  //Rotate light source
  xx=dc->ls.x; yy=dc->ls.y; zz=-dc->ls.z;
  (*dc->transform)(dc,&xx,&yy,&zz);
  dc->ls.x=xx; dc->ls.y=yy; dc->ls.z=zz;

  dc->flags|=DCF_TRANSFORMATION;
  dc->transform=&DSTransform;
  dc->lighting=&DSLighting;
  dc->x=cx;
  dc->y=cy;
  r1=Mat4x4IdentNew(task);
  Mat4x4RotX(r1,-pi/2);
  Mat4x4RotZ(r1,tS);
  Mat4x4Scale(r1,PERSON_SCALE);

  r2=Mat4x4IdentNew(task);
  Mat4x4Scale(r2,PERSON_SCALE);

  r3=Mat4x4IdentNew(task);
  Mat4x4RotX(r3,-pi/2);
  Mat4x4Scale(r3,PLANT_SCALE);

  x1h=man_xx+yh*PLOT_GRID_WIDTH/2+xh*(PLOT_GRID_HEIGHT-1);
  y1h=man_yy-xh*PLOT_GRID_WIDTH/2+yh*(PLOT_GRID_HEIGHT-1);
  xh>>=1; yh>>=1;

  mp_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    JobQue(&MPDrawWalls,dc,i);
  while (mp_not_done_flags)
    Yield;

    //Draw Persons
  for (i=0;i<PERSONS_NUM;i++) {
    x=persons[i].x;
    y=persons[i].y;
    if (LOS(x,y,man_xx,man_yy)) {
      x-=man_xx;
      y-=man_yy;
      xx=x;
      yy=y;
      zz=0;
      DCTransform(dc,&xx,&yy,&zz);
      if (zz>0) {
	tt=2*Tri(tS,1.0);
	if (tt<1.0)
	  tmps=SpriteInterpolate(tt,__BIN_2,__BIN_1);
	else
	  tmps=SpriteInterpolate(tt-1.0,__BIN_1,__BIN_3);
	Sprite3Mat4x4B(dc,x,y,0,tmps,r1);
	Free(tmps);
      }
    }
  }
  Free(r1);
  Free(r2);
  Free(r3);

  //Draw BluePrint heads-up display, scaled 2 pixels
  Free(dc->r);
  DCMat4x4Set(dc,Mat4x4IdentNew(task));
  dc->x=task->pix_width -2*blueprint_width;
  dc->y=task->pix_height-2*blueprint_height;
  dc->z=0;
  dc->transform=&DCTransform;
  dc->thick=2;
  for (i=0;i<blueprint_height;i++)
    for (j=0;j<blueprint_width;j++) {
      dc->color=blueprint[(blueprint_height-1-i)*blueprint_width+j];
      GrPlot3(dc,2*j,2*i,0);
    }

    //Draw Things on heads-up BluePrint
  dc->color=LTPURPLE;
  for (i=0;i<PERSONS_NUM;i++)
    GrPlot3(dc,2*(persons[i].x/SCRN_SCALE),
	  2*(blueprint_height-1-persons[i].y/SCRN_SCALE),0);
  dc->color=LTCYAN;
  GrPlot3(dc,2*(man_xx/SCRN_SCALE),
	2*(blueprint_height-1-man_yy/SCRN_SCALE),0);

  dc->color=LTGREEN;
  GrLine(dc,cx-5,cy,cx+5,cy);
  GrLine(dc,cx,cy-5,cx,cy+5);
  Free(s2w);
}

U0 BluePrintDrawIt(CTask *task,CDC *dc)
{
  task->text_attr=BLUE<<4+WHITE;
  GrBlot(dc,(task->pix_width-blueprint_dc->width)>>1,
	(task->pix_height-blueprint_dc->height)>>1,blueprint_dc);
}

#define MAN_START_X	0
#define MAN_START_Y	1

U0 MakeBluePrint()
{
  I64 msg_code,i,x1,y1,x2,y2,arg1,arg2,color=WHITE,cpu_num=0;
  Wall *tmpw;

  DCFill(blueprint_dc,LTBLUE);

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER;
  DocClear;

  "Draw Floor Plan:

Walls  in WHITE
Tables in BROWN
Plants in GREEN
  ";

  AutoComplete;
  WinBorder;
  WinMax;
  Fs->draw_it=&BluePrintDrawIt;

  do {
    msg_code=GetMsg(&arg1,&arg2,
	  1<<MSG_KEY_DOWN+1<<MSG_MS_L_DOWN+
	  1<<MSG_MS_R_UP);
    switch (msg_code) {
      case MSG_MS_R_UP:
	i=PopUpColor;
	if (i>=0) color=i;
	break;
      case MSG_MS_L_DOWN:
	if (color==WHITE) {
	  arg1-=(Fs->pix_width-blueprint_dc->width)>>1;
	  arg2-=(Fs->pix_height-blueprint_dc->height)>>1;
	  x1=arg1; y1=arg2;
	  x2=arg1; y2=arg2;
	  blueprint_dc->color=ROP_XOR+color^LTBLUE;
	  while (msg_code!=MSG_MS_L_UP) {
	    GrLine(blueprint_dc,x1,y1,x2,y2);
	    msg_code=GetMsg(&arg1,&arg2,
		  1<<MSG_MS_L_UP+1<<MSG_MS_MOVE);
	    arg1-=(Fs->pix_width-blueprint_dc->width)>>1;
	    arg2-=(Fs->pix_height-blueprint_dc->height)>>1;
	    GrLine(blueprint_dc,x1,y1,x2,y2);
	    x2=arg1; y2=arg2;
	  }
	  blueprint_dc->color=color;
	  GrLine(blueprint_dc,x1,y1,x2,y2);

	  tmpw=CAlloc(sizeof(Wall));
	  tmpw->p1.x=x1;
	  tmpw->p1.y=blueprint_dc->height-y1;
	  tmpw->p2.x=x2;
	  tmpw->p2.y=blueprint_dc->height-y2;
	  tmpw->cpu_num=cpu_num++%mp_cnt;
	  QueIns(tmpw,walls_head.last);
	} else {
	  arg1-=(Fs->pix_width-blueprint_dc->width)>>1;
	  arg2-=(Fs->pix_height-blueprint_dc->height)>>1;
	  x1=arg1; y1=arg2;
	  x2=arg1; y2=arg2;
	  blueprint_dc->color=ROP_XOR+color^LTBLUE;
	  while (msg_code!=MSG_MS_L_UP) {
	    GrRectB(blueprint_dc,x1,y1,x2,y2);
	    msg_code=GetMsg(&arg1,&arg2,
		  1<<MSG_MS_L_UP+1<<MSG_MS_MOVE);
	    arg1-=(Fs->pix_width-blueprint_dc->width)>>1;
	    arg2-=(Fs->pix_height-blueprint_dc->height)>>1;
	    GrRectB(blueprint_dc,x1,y1,x2,y2);
	    x2=arg1; y2=arg2;
	  }
	  blueprint_dc->color=color;
	  GrRectB(blueprint_dc,x1,y1,x2,y2);
	}
	break;
      case MSG_KEY_DOWN:
	break;
    }
  } while (msg_code!=MSG_KEY_DOWN || !arg1);
  GetMsg(,,1<<MSG_KEY_UP);

  SettingsPop;
}

U0 Init()
{
  I64 i,j,x,y,c,c1;
  QueInit(&walls_head);

  blueprint_dc=DCNew(BLUEPRINT_SCALE*64,BLUEPRINT_SCALE*64);
  MakeBluePrint;

  DocClear;
  "$BG,LTGRAY$%h*c",TEXT_ROWS/2-1,'\n';

  blueprint_width =blueprint_dc->width/BLUEPRINT_SCALE;
  blueprint_height=blueprint_dc->height/BLUEPRINT_SCALE;
  Free(blueprint);
  Free(panels_processed);
  blueprint=MAlloc(blueprint_width*blueprint_height*sizeof(U8));
  MemSet(blueprint,WHITE,blueprint_width*blueprint_height);
  panels_processed=MAlloc((blueprint_width*blueprint_height+7)>>3);

  for (y=1;y<blueprint_height-1;y++)
    for (x=1;x<blueprint_width-1;x++) {
      c=LTBLUE;
      for (i=0;i<BLUEPRINT_SCALE;i++)
	for (j=0;j<BLUEPRINT_SCALE;j++)
	  if ((c1=GrPeek(blueprint_dc,
		x*BLUEPRINT_SCALE+i,y*BLUEPRINT_SCALE+j))!=LTBLUE)
	    c=c1;
      blueprint[(blueprint_height-1-y)*blueprint_width+x]=c;
    }
  DCDel(blueprint_dc);

  man_xx=(1+MAN_START_X)*SCRN_SCALE;
  man_yy=(blueprint_height-1-MAN_START_Y)*SCRN_SCALE;
  man_theta=0;

  for (i=0;i<PERSONS_NUM;i++) {
    do {
      persons[i].x=RandU64%((blueprint_width-2)*SCRN_SCALE)+SCRN_SCALE;
      persons[i].y=RandU64%((blueprint_height-2)*SCRN_SCALE)+SCRN_SCALE;
    } while (blueprint[(persons[i].y/SCRN_SCALE)*blueprint_width+
	  persons[i].x/SCRN_SCALE]==WHITE);
  }
  tf=0;
  t0=tS;
}

U0 AnimateTask(I64)
{
  I64 i,x,y,dd;
  while (TRUE) {
    dd=0.25*SCRN_SCALE*Sin(tS/2);
    for (i=0;i<PERSONS_NUM;i++) {
      x=persons[i].x;
      y=persons[i].y;
      if (i&1)
	x+=dd;
      else
	y+=dd;
      if (0<=x<=blueprint_width*SCRN_SCALE &&
	    0<=y<=blueprint_height*SCRN_SCALE &&
	    blueprint[(y/SCRN_SCALE)*blueprint_width+x/SCRN_SCALE]!=WHITE) {
	if (blueprint[(y/SCRN_SCALE)*blueprint_width+x/SCRN_SCALE+1]==
	      WHITE && x-RoundI64(x,SCRN_SCALE)>SCRN_SCALE/2 ||
	      blueprint[(y/SCRN_SCALE)*blueprint_width+x/SCRN_SCALE-1]==
	      WHITE && x-RoundI64(x,SCRN_SCALE)<SCRN_SCALE/2)
	  x=RoundI64(x,SCRN_SCALE)+SCRN_SCALE/2;
	if (blueprint[(y/SCRN_SCALE+1)*blueprint_width+x/SCRN_SCALE]==
	      WHITE && y-RoundI64(y,SCRN_SCALE)>SCRN_SCALE/2 ||
	      blueprint[(y/SCRN_SCALE-1)*blueprint_width+x/SCRN_SCALE]==
	      WHITE && y-RoundI64(y,SCRN_SCALE)<SCRN_SCALE/2)
	  y=RoundI64(y,SCRN_SCALE)+SCRN_SCALE/2;
	persons[i].x=x;
	persons[i].y=y;
      }
    }
    Sleep(20);
  }
}

U0 CleanUp()
{
  Free(blueprint);
  blueprint=NULL;
  Free(panels_processed);
  panels_processed=NULL;
  QueDel(&walls_head);
}

U0 SongTask(I64)
{ //Song by Terry A. Davis
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3q.A#eGAeA#qAq.A#eGeAeA#qA");
    Play("3q.A#eGA#AqGq.A#eGA#AqG");
    Play("4eA#AqGeA#AqGeA#AGAA#AqG");
  }
}

U0 MoveMan(F64 theta)
{
  I64 x,y,color,step=SCRN_SCALE/2;
  do {
    x=man_xx+step*Cos(theta);
    y=man_yy-step*Sin(theta);
    x=Clamp(x,0,blueprint_width*SCRN_SCALE);
    y=Clamp(y,0,blueprint_height*SCRN_SCALE);
    color=blueprint[y/SCRN_SCALE*blueprint_width+x/SCRN_SCALE];
    if (color==LTBLUE || color==GREEN) {
      man_xx=x;
      man_yy=y;
      break;
    } else
      step>>=1;
  } while (step);
}

#define MICRO_STEPS	4
U0 RotateMan(F64 d)
{
  I64 i;
  for (i=0;i<MICRO_STEPS;i++) {
    man_theta+=d/MICRO_STEPS;
    Sleep(15);
  }
}

U0 DiningStars()
{
  I64 sc;
  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Fwd(,,SC_CURSOR_UP);"
	"  Bwd(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=BROWN<<4+WHITE;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Fs->draw_it=&DrawIt;

  try {
    while (TRUE) {
      switch (GetKey(&sc)) {
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto fs_done;
	case 0:
	  switch (sc.u8[0]) {
	    case SC_CURSOR_RIGHT:
	      Spawn(&RotateMan,(pi/32)(I64));
	      break;
	    case SC_CURSOR_LEFT:
	      Spawn(&RotateMan,(-pi/32)(I64));
	      break;
	    case SC_CURSOR_UP:
	      MoveMan(man_theta);
	      break;
	    case SC_CURSOR_DOWN:
	      MoveMan(man_theta+pi);
	      break;
	  }
	  break;
      }
    }
fs_done:
  } catch
    PutExcept;
  DocClear;
  SettingsPop;
  CleanUp;
  MenuPop;
  RegWrite("TempleOS/DiningStars","F64 best_score=%5.4f;\n",best_score);
}

DiningStars;
