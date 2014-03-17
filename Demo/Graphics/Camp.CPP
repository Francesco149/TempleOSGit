//This uses [C:/Demo/Lectures/FixedPoint.CPP.Z,1] fixed-point.

#define CO_PERSON	1
#define CO_TENT	2

class Obj
{
  I64 x,y,z;
  I64 dx,dy,dz;
  I64 type;
};


#define MAX_PEOPLE	1024
#define TENT_RATIO	40
#define MAX_TENTS	(MAX_PEOPLE/TENT_RATIO+1)






	/* <1> <1> (image) */





	/* <2> <2> (image) */





	/* <3> <3> (image) */





	/* <4> <4> (image) */





	/* <5> <5> (image) */





	/* <6> <6> (image) */



CSprite *right_imgs[4]={__BIN_2,__BIN_1,__BIN_3,__BIN_1},
	*left_imgs[4]={__BIN_5,__BIN_4,__BIN_6,__BIN_4};















	/* <7> <7> (image) */

CSprite *tent_img=__BIN_7;







	/* <8> <8> (image) */


















/* Mountain <9> (image) */ //Used in quail, clouds, and squirt.



I64 num_objs;
Obj objs[MAX_PEOPLE+MAX_TENTS];
Obj camp_view_pt;

#define CAMP_WIDTH	600
#define CAMP_SCALE	300
#define CAMP_Z_CLIP	10

U0 CTransform(CDC *dc,I64 *x,I64 *y,I64 *z)
{
  *x-=camp_view_pt.x;
  *y-=camp_view_pt.y;
  *z-=camp_view_pt.z;

  GrRot(dc->r,x,y,z);

  *x = *x *-CAMP_SCALE/ *z;
  *y = *y *-CAMP_SCALE/ *z;

  *x=dc->x+*x;
  *y=dc->y-*y;
  *z=dc->z-*z;
}

I64 mp_not_done_flags;
I64 cx,cy,*r1; //could put these in a record and pass in
F64 frame;
I32 *depth_buf;
U0 MPDrawMeeting(CTask *task)
{
  CDC *dc=DCAlias(gr_dc2,task);
  F64 s;
  CSprite *img,*img1,*img2;
  I64 i,x,y,z,*r2,*old_r=dc->r,
     lo= Gs->num   *num_objs/mp_cnt,
     hi=(Gs->num+1)*num_objs/mp_cnt;
  dc->flags|=DCF_TRANSFORMATION;
  dc->depth_buf=depth_buf;
  dc->x=cx;
  dc->y=cy;
  dc->z=0;
  dc->transform=&GrTransform;
  for (i=lo;i<hi;i++) {
    GrSetRotMat(dc,r1);
    dc->x=cx;
    dc->y=cy;
    dc->z=0;
    x=objs[i].x.i32[1];
    y=objs[i].y.i32[1];
    z=objs[i].z.i32[1];
    CTransform(dc,&x,&y,&z);
    if (z>CAMP_Z_CLIP) {
      s=0.5*ToF64(CAMP_SCALE)/z;
      if (objs[i].type==CO_PERSON) {
	if (objs[i].dx>=0) {
	  img1=right_imgs[(i+ToI64(frame))&3];
	  img2=right_imgs[(i+ToI64(frame)+1)&3];
	} else {
	  img1=left_imgs[(i+ToI64(frame))&3];
	  img2=left_imgs[(i+ToI64(frame)+1)&3];
	}
	img=SpriteInterpolate(img1,img2,frame%1.0);
      } else
	img=tent_img;

      r2=GrScaleNew(old_r,s);
      GrSetRotMat(dc,r2);
      Sprite3B(dc,x,y,z,img);
      Free(r2);
      if (objs[i].type==CO_PERSON)
	Free(img);
    }
  }
  dc->r=old_r;
  dc->depth_buf=NULL;
  DCDel(dc);
  LBtr(&mp_not_done_flags,Gs->num);
}

U0 DrawMeeting(CTask *task,CDC *dc)
{
  static Bool calf=FALSE,determined=FALSE;

  I64 i,x,y,z,*r2,*old_r;
  F64 s;

  frame=4.0*Saw(tS,0.5);
  cx=task->pix_width/2;
  cy=task->pix_height/2;

  camp_view_pt.x=0;
  camp_view_pt.y=200-100*Sin(tS);
  camp_view_pt.z=20+100*Cos(tS);

  DCAllocDepthBuf(dc);
  depth_buf=dc->depth_buf;
  r1=GrRotXNew(30.0*pi/180.0,task);
  old_r=dc->r;

  dc->pen_width=2;
  dc->transform=&CTransform;
  dc->x=cx;
  dc->y=cy;
  dc->z=0;
  GrSetRotMat(dc,r1);

  x=0;y=0;z=-16*CAMP_WIDTH;
  dc->transform(dc,&x,&y,&z);
  Sprite3(dc,0,y,0,__BIN_9);
  dc->flags|=DCF_TRANSFORMATION;

  dc->color=BLACK;
  GrLine3(dc,-CAMP_WIDTH/2,0,0	  ,CAMP_WIDTH/2,0,0);
  GrLine3(dc,-CAMP_WIDTH/2,0,-CAMP_WIDTH,CAMP_WIDTH/2,0,-CAMP_WIDTH);
  GrLine3(dc,-CAMP_WIDTH/2,0,0,-CAMP_WIDTH/2,0,-CAMP_WIDTH);
  GrLine3(dc, CAMP_WIDTH/2,0,0, CAMP_WIDTH/2,0,-CAMP_WIDTH);

  dc->transform=&GrTransform;

  if (tS%10<5) {
    calf=FALSE;
    determined=FALSE;
  } else {
    if (!determined) {
      determined=TRUE;
      if (RandU16<MAX_U16/2)
	calf=TRUE;
    }
  }
 
  if (calf) {
    x=0;
    y=0;
    z=-CAMP_WIDTH/2;
    CTransform(dc,&x,&y,&z);
    s=0.5*ToF64(CAMP_SCALE)/z;
    r2=GrScaleNew(old_r,s,task);
    GrSetRotMat(dc,r2);
    Sprite3B(dc,x,y,z,__BIN_8);
    Free(r2);
  }

  mp_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    JobQue(&MPDrawMeeting,task,i);

  GrSetRotMat(dc,old_r);
  if (calf && Blink(4.0)) {
    dc->color=YELLOW;
    GrPrint(dc,cx-(15*FONT_WIDTH)/2,cy-3*FONT_HEIGHT,"!!Golden Calf!!");
  }
 
  while (mp_not_done_flags)
    Yield;
  Free(r1);
}

#define MAX_SPEED	2<<32

U0 AnimateTask(I64)
{
  I64 i,x,z,dx,dz;
  F64 d;
  while (TRUE) {
    WinMgrSync;
    for (i=0;i<num_objs;i++)
      if (objs[i].type==CO_PERSON) {
	if (tS%10<5) {
	  objs[i].dx+=RandI32;
	  objs[i].dz+=RandI32;
	} else { //Form circle
	  x=CAMP_WIDTH/4*Cos((1.0+1.0/TENT_RATIO)*i*pi*2/num_objs);
	  z=CAMP_WIDTH/4*Sin((1.0+1.0/TENT_RATIO)*i*pi*2/num_objs)-CAMP_WIDTH/2;
	  dx=x-objs[i].x.i32[1];
	  dz=z-objs[i].z.i32[1];
	  d=Sqrt(SqrI64(dx)+SqrI64(dz));
	  objs[i].dx=MAX_SPEED*(dx/d);
	  objs[i].dz=MAX_SPEED*(dz/d);
	}
	objs[i].dx=ClampI64(objs[i].dx,-MAX_SPEED,MAX_SPEED);
	objs[i].dy=ClampI64(objs[i].dy,-MAX_SPEED,MAX_SPEED);
	objs[i].dz=ClampI64(objs[i].dz,-MAX_SPEED,MAX_SPEED);
	objs[i].x+=objs[i].dx;
	objs[i].y+=objs[i].dy;
	objs[i].z+=objs[i].dz;
	if (objs[i].x.i32[1]<-CAMP_WIDTH>>1) {
	  objs[i].x=-CAMP_WIDTH>>1<<32;
	  objs[i].dx=-objs[i].dx;
	}
	if (objs[i].x.i32[1]>CAMP_WIDTH>>1) {
	  objs[i].x=CAMP_WIDTH>>1<<32;
	  objs[i].dx=-objs[i].dx;
	}
	if (objs[i].z.i32[1]<-CAMP_WIDTH) {
	  objs[i].z=-CAMP_WIDTH<<32;
	  objs[i].dz=-objs[i].dz;
	}
	if (objs[i].z.i32[1]>0) {
	  objs[i].z=0;
	  objs[i].dz=-objs[i].dz;
	}
      }
    Sleep(20);
  }
}

U0 Camp(I64 num_people=100)
{
  I64 i,j=0,num_tents;

  if (num_people>MAX_PEOPLE)
    num_people=MAX_PEOPLE;

  MemSet(objs,0,sizeof(objs));
  for (i=0;i<num_people;i++) {
    objs[j].x=(RandU32%CAMP_WIDTH-CAMP_WIDTH>>1)<<32;
    objs[j].y=0;
    objs[j].z=-(RandU32%CAMP_WIDTH)<<32;
    objs[j].type=CO_PERSON;
    j++;
  }
  num_tents=(num_people+TENT_RATIO-1)/TENT_RATIO;
  for (i=0;i<num_tents;i++) {
    objs[j].x=(RandU32%CAMP_WIDTH-CAMP_WIDTH>>1)<<32;
    objs[j].y=0;
    objs[j].z=-(RandU32%CAMP_WIDTH)<<32;
    objs[j].type=CO_TENT;
    j++;
  }
  num_objs=j;

  DocClear;

  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=LTGREEN<<4+WHITE; //bk/fg
  try {
    WordStat;
    Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
    DocCursor;
    DocClear;
    Fs->draw_it=&DrawMeeting;

    GetChar;
  } catch
    PutExcept;
  SettingsPop(Fs,TSF_SAME_SONG);
}

//This is so I can use this file stand alone
//or as a component of a program.
#if IncludeDepth<2
WinMax;
WinBorder;
Camp;
#endif
