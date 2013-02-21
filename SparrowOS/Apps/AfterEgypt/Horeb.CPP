




    /* <1> <1> (image) */




    /* <2> <2> (image) */









    /* <3> <3> (image) */





















    /* <4> <4> (image) */

//See [::/SparrowOS/Doc/Acknowledgements.TXT.Z,1] ::/SparrowOS/Doc/Acknowledgements.TXT.Z (8).










	   /*  <5> (image) */


F64 theta,vx,vz;
Bool horeb_done;

#define NUM_OBJS	256

#define NUM_WEIGHED_IMGS	64
U8 *imgs[NUM_WEIGHED_IMGS]=
{__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_4,
__BIN_1,__BIN_1,__BIN_2,__BIN_2,__BIN_3,__BIN_4,__BIN_4,__BIN_5};

class Obj
{
  F64 x,y,z,dx,dy,dz,theta;
  I64 x1,y1,z1;
  Bool sym,moving,pad[2];
  U8 *img;
} objs[NUM_OBJS];


I64 ObjCompare(Obj *o1,Obj *o2)
{
  return o2->z1-o1->z1;
}

U0 DrawHoreb(CTask *task,CDC *dc)
{
  I64 i,j,x,y,z,dx,dy,dz,
      w=task->win_pixel_width,
      h=task->win_pixel_height,r[16],
      cx=w>>1,cy=h>>1;
  F64 s,m,a,x1,y1,x2,y2;
  Obj *o_sort[NUM_OBJS],*tempo;

  GrIdentEqu(r);
  GrRotYEqu(r,-theta);
  GrRotXEqu(r,77*pi/180);

  //Sun
  x=vx; y=0; z=1000000+vz;
  GrRot(r,&x,&y,&z);
  s=100.0/(AbsI64(z)+50);
  if (y<0) {
    dc->color=BROWN;
    GrCircle(dc,s*x+cx,15,15);
    dc->color=YELLOW;
    GrFloodFill(dc,s*x+cx,15);
  }

  for (i=0;i<NUM_OBJS;i++) {
    tempo=&objs[i];
    o_sort[i]=tempo;
    tempo->x1=tempo->x+vx; tempo->y1=tempo->y; tempo->z1=tempo->z+vz;
    GrRot(r,&tempo->x1,&tempo->y1,&tempo->z1);
    if (tempo->moving) {
      dx=100*tempo->dx; dy=100*tempo->dy; dz=100*tempo->dz;
      GrRot(r,&dx,&dy,&dz);
      if (dx<0)
	tempo->sym=TRUE;
      else
	tempo->sym=FALSE;
    }
  }
  QSortI64(o_sort,NUM_OBJS,&ObjCompare);

  for (i=0;i<NUM_OBJS;i++) {
    tempo=o_sort[i];
    if (tempo->z1<0) break;
    s=100.0/(AbsI64(tempo->z1)+50);
    GrIdentEqu(dc->r);
    GrScaleMatEqu(dc->r,s*2);
    GrSetRotMat(dc,dc->r);
    dc->flags|=DCF_TRANSFORMATION;
    if (tempo->sym) {
      dc->flags|=DCF_SYMMETRY|DCF_JUST_MIRROR;
      GrSetSymmetry(dc,s*tempo->x1+cx,s*tempo->y1+h,s*tempo->x1+cx,s*tempo->y1+h+10);
    }
    SpritePlot3b(dc,s*tempo->x1+cx,s*tempo->y1+h,0,tempo->img);
    dc->flags&=~(DCF_SYMMETRY|DCF_JUST_MIRROR);
    if (o_sort[i]==objs) { //draw burning bush
      dc->x=s*tempo->x1+cx;
      dc->y=s*tempo->y1+h;
      dc->z=0;

      dc->pen_width=1;
      dc->color=win_updates&15;
      for (j=0;j<45;j++) {
	m=Sqr(Rand); a=2*pi*Rand;
	P2R(&x1,&y1,m,a);
	m=Sqr(Rand); a=2*pi*Rand;
	P2R(&x2,&y2,m,a);
	GrLine3(dc,20*x1,20*y1-25,0,20*x2,20*y2-25,0);
      }

      if (SqrI64(s*tempo->x1)+SqrI64(s*tempo->y1)<300*300)
	horeb_done=TRUE;
    }
  }

  if (Blink(,tP(task))) {
    dc->color=LTRED;
    GrPutS(dc,cx-21*FONT_WIDTH/2,cy-FONT_HEIGHT/2,"Find the Burning Bush.");
  }
}

U0 AnimateTask(I64)
{ //Preemption is off after Spawn
  I64 i;
  F64 t;
  Obj *tempo;
  while (TRUE) {
    t=tP(Fs->parent_task);
    for (i=0;i<NUM_OBJS;i++) {
      tempo=&objs[i];
      if (tempo->moving) {
	tempo->x+=tempo->dx;
	tempo->y+=tempo->dy;
	tempo->z+=tempo->dz;
	tempo->dx=5*Cos(0.5*t+tempo->theta);
	tempo->dz=5*Sin(0.5*t+tempo->theta);
      }
    }
    Sleep(20);
  }
}

U0 Init()
{
  Obj *tempo;
  I64 i;
  MemSet(objs,0,sizeof(objs));
  for (i=0;i<NUM_OBJS;i++) {
    tempo=&objs[i];
    tempo->x=RandU32&0x1FFF-0x1000;
    tempo->y=0;
    tempo->z=RandU32&0x1FFF-0x1000;
    tempo->theta=2*pi*Rand;
    tempo->sym=RandU16&1;
    tempo->img=imgs[i%NUM_WEIGHED_IMGS];
    if (i%NUM_WEIGHED_IMGS==NUM_WEIGHED_IMGS-1)
      tempo->moving=TRUE; //sheep
  }
  vx=0; vz=0; theta=0;
  horeb_done=FALSE;
}

#define ANIMATE_STEPS	10

U0 AnimatethetaTask(F64 a)
{
  I64 i;
  for (i=0;i<ANIMATE_STEPS;i++) {
    theta+=a/ANIMATE_STEPS;
    Sleep(10);
  }
}

U0 AnimateVTask(F64 v)
{
  I64 i;
  for (i=0;i<ANIMATE_STEPS;i++) {
    vx+=v/ANIMATE_STEPS*Sin(theta);
    vz+=v/ANIMATE_STEPS*Cos(theta);
    Sleep(10);
  }
}

U0 Horeb()
{
  I64 ch,sc;
  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,CH_NEW_LINE);"
  "  Fwd(,,SC_CURSOR_UP);"
  "  Backward(,,SC_CURSOR_DOWN);"
  "  Left(,,SC_CURSOR_LEFT);"
  "  Right(,,SC_CURSOR_RIGHT);"
      "}"
      );
  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  try {
    Fs->win_inhibit=WIF_ALL-WIF_GRAB_SCROLL-WIF_MENU;
    WordStat;
    WinMax;
    WinBorder;
    Preempt(OFF);

    "$CL$$BG,LTCYAN$";
    NewLine(6);
    "$BG,YELLOW$";
    NewLine(30);
    PutBibleVerse("Exodus,3:1",21);
    Init;
    Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);
    Fs->draw_it=&DrawHoreb;
    while (!horeb_done) {
      while (ScanKey(&ch,&sc))
	switch (ch) {
	  case 0:
	    switch (sc.u8[0]) {
	      case SC_CURSOR_LEFT:
		Spawn(&AnimatethetaTask,(-pi/100)(I64),"Animate theta",Fs);
		break;
	      case SC_CURSOR_RIGHT:
		Spawn(&AnimatethetaTask,( pi/100)(I64),"Animate theta",Fs);
		break;
	      case SC_CURSOR_UP:
		Spawn(&AnimateVTask,(-40.0 )(I64),"Animate V",Fs);
		break;
	      case SC_CURSOR_DOWN:
		Spawn(&AnimateVTask,( 40.0 )(I64),"Animate V",Fs);
		break;
	    }
	    break;
	  case CH_NEW_LINE:
	    Init;
	    break;
	  case CH_SHIFT_ESC:
	  case CH_ESC:
	    goto hr_done;
	}
      WinMgrSync;
    }
hr_done: //Don't goto out of try
  } catch
    Fs->catch_except=TRUE;
  SettingsPop(Fs,TSF_SAME_SONG);
  DocClear;
  "$BG$";
  MenuPop;
}

//This is so I can use this file stand alone
//or as a component of a program.
#if IncludeDepth<2
Horeb;
#endif
