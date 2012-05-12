#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x0000007A
#define __BIN_1 "\x1D\x04\x00\x00\x00\x04\x00\x00\x00\xE7\xFF\xFF\xFF\xE7\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\xE7\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\xE7\xFF\xFF\xFF\xF1\xFF\xFF\xFF\x04\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00"
#define __BIN_2_TYPE 0x00000002
#define __BIN_2_SIZE 0x000000D2
#define __BIN_2 "\x1D\x06\x00\x00\x00\x08\x00\x00\x00\xF6\xFF\xFF\xFF\x0F\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\xF6\xFF\xFF\xFF\x00\x00\x00\x00\xF6\xFF\xFF\xFF\xF6\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x19\x00\x00\x00\x06\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x06\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x01\x00\x00\x00\x05\x00\x00\x00\x04\x00\x00\x00\x06\x00\x00\x00\x01\x00\x00\x00\x04\x00\x00\x00\x02\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x04\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x03\x00\x00\x00\x00"
#define __BIN_3_TYPE 0x00000002
#define __BIN_3_SIZE 0x0000009E
#define __BIN_3 "\x1D\x07\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xE7\xFF\xFF\xFF\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xE7\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xE7\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x04\x00\x00\x00\x06\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00\x01\x00\x00\x00\x00"

/*
This is a whimsical program which demonstrates some techniques.

*/

#define BORDER		20

#define PTY_POINT	0
#define PTY_CIRCLE	1
#define PTY_LINE	2
#define PTY_GRSPRITES	3
#define PTY_NUM		4

extern class PObject;

class PPoint
{
  P3I32 p;
};

class PCircle
{
  PObject *p;
  I64 radius;
};

class PLine
{
  PObject *p1,*p2;
};

class PGrSprite
{
  PObject *p;
  U8 *img;
  I64 *r,
     *dr; //Rounding error might eventually screw this up
}


class PObject
{
  PObject *next,*last;
  I64 type,color;
  union {
    PPoint   p;
    PCircle  c;
    PLine    l;
    PGrSprite g;
  };
};

class PickFrame
{
  PObject o_root;
  I64 o_cnts[PTY_NUM];
  I64 cx,cy;
};



#define NUM_IMGS	3



  /* <1> <1> (image) */





  /* <2> <2> (image) */





  /* <3> <3> (image) */



U8 *imgs[NUM_IMGS]={__BIN_1,__BIN_2,__BIN_3};


U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  I64 *r,*old_r;
  PickFrame *pf=NamedFramePtr("PickFrame",task);
  PObject *tempo=pf->o_root.next;

  pf->cx=task->win_pixel_width>>1;
  pf->cy=task->win_pixel_height>>1;

  GrDCAllocDepthBuf(dc);

  dc->color=LTRED;
  dc->pen_width=3;
  GrLineRect4(dc,BORDER,BORDER,2*pf->cx-BORDER,2*pf->cy-BORDER);


  while (tempo!=&pf->o_root) {
    dc->color=tempo->color;
    switch (tempo->type) {
      case PTY_POINT:
	GrLine(dc,pf->cx+tempo->p.p.x+2,pf->cy+tempo->p.p.y+2,pf->cx+tempo->p.p.x-2,p
f->cy+tempo->p.p.y-2);
	GrLine(dc,pf->cx+tempo->p.p.x-2,pf->cy+tempo->p.p.y+2,pf->cx+tempo->p.p.x+2,p
f->cy+tempo->p.p.y-2);
	break;
      case PTY_CIRCLE:
	GrCircle(dc,pf->cx+tempo->c.p->p.p.x,pf->cy+tempo->c.p->p.p.y,tempo->c.radius
);
	break;
      case PTY_LINE:
	GrLine(dc,pf->cx+tempo->l.p1->p.p.x,pf->cy+tempo->l.p1->p.p.y,pf->cx+tempo->l
.p2->p.p.x,pf->cy+tempo->l.p2->p.p.y);
	break;
      case PTY_GRSPRITES:
	old_r=dc->r;
	dc->r=tempo->g.r;
	dc->x=pf->cx+tempo->g.p->p.p.x;
	dc->y=pf->cy+tempo->g.p->p.p.y;
	dc->z=GR_Z_ALL;
	dc->flags|=DCF_TRANSFORMATION;
	GrSpritePlot3(dc,0,0,0,tempo->g.img);
	dc->flags&=~DCF_TRANSFORMATION;
	dc->r=old_r;

	//Updated each refresh, not guarenteed to be uniform.
	//Rounding error might corrupt, as well.
	r=GrMulMat(tempo->g.dr,tempo->g.r,task);
	Free(tempo->g.r);
	tempo->g.r=r;

	break;
    }
    tempo=tempo->next;
  }
}

PObject *PObjectNew(PickFrame *pf,I64 type,I64 color)
{
  PObject *tempo=CAlloc(sizeof(PObject));
  tempo->type=type;
  tempo->color=color;
  pf->o_cnts[type]++;
  InsQue(tempo,pf->o_root.last);
  return tempo;
}

U0 PObjectDel(PickFrame *pf,PObject *tempo)
{
  RemQue(tempo);
  switch (tempo->type) {
    case PTY_GRSPRITES:
      Free(tempo->g.r);
      Free(tempo->g.dr);
      break;
  }
  pf->o_cnts[tempo->type]--;
  Free(tempo);
}

PObject *PPointNew(PickFrame *pf,I64 x,I64 y)
{
  PObject *tempo=PObjectNew(pf,PTY_POINT,BLACK);
  tempo->p.p.x=x;
  tempo->p.p.y=y;
  return tempo;
}

PObject *PPointNum(PickFrame *pf,I64 num)
{
  PObject *tempo=pf->o_root.next;
  while (tempo!=&pf->o_root) {
    if (tempo->type==PTY_POINT && !num--)
      return tempo;
    tempo=tempo->next;
  }
  return NULL;
}

PObject *PPointFind(PickFrame *pf,I64 x,I64 y)
{
  I64 d,best_d=MAX_I64;
  PObject *tempo=pf->o_root.next,*result=NULL;
  while (tempo!=&pf->o_root) {
    if (tempo->type==PTY_POINT) {
      d=SqrI64(tempo->p.p.x-x)+SqrI64(tempo->p.p.y-y);
      if (d<best_d) {
	best_d=d;
	result=tempo;
      }
    }
    tempo=tempo->next;
  }
  return result;
}

PObject *PCircleNew(PickFrame *pf,I64 p_num,I64 r)
{
  PObject *tempo=PObjectNew(pf,PTY_CIRCLE,RED);
  tempo->c.p=PPointNum(pf,p_num);
  tempo->c.radius=r;
  return tempo;
}

PObject *PLineNew(PickFrame *pf,I64 p1_num,I64 p2_num)
{
  PObject *tempo=PObjectNew(pf,PTY_LINE,GREEN);
  tempo->l.p1=PPointNum(pf,p1_num);
  tempo->l.p2=PPointNum(pf,p2_num);
  return tempo;
}

PObject *PGrSpriteNew(PickFrame *pf,U8 *img,I64 p_num,I64 *r,I64 *dr)
{
  PObject *tempo=PObjectNew(pf,PTY_GRSPRITES,BLACK);
  tempo->g.p=PPointNum(pf,p_num);
  tempo->g.img=img;
  tempo->g.r=r;
  tempo->g.dr=dr;
  return tempo;
}

PickFrame *Init()
{
  PickFrame *pf=CAlloc(sizeof(PickFrame));
  I64 i,*r,*dr;

  pf->cx=Fs->win_pixel_width>>1;
  pf->cy=Fs->win_pixel_height>>1;

  pf->o_root.next=pf->o_root.last=&pf->o_root;
  for (i=0;i<50;i++)
    PPointNew(pf,RandI32%(pf->cx-BORDER),RandI32%(pf->cy-BORDER));
  for (i=0;i<20;i++)
    PCircleNew(pf,pf->o_cnts[PTY_POINT]*RandU16/MAX_U16,6);
  for (i=0;i<20;i++)
   
PLineNew(pf,pf->o_cnts[PTY_POINT]*RandU16/MAX_U16,pf->o_cnts[PTY_POINT]*RandU1
6/MAX_U16);
  for (i=0;i<10;i++) {
    r=GrIdent;
    dr=GrIdent;
    GrRotZEqu(dr,0.05*2*(Rand-0.5));
    GrRotYEqu(dr,0.05*2*(Rand-0.5));
    GrRotXEqu(dr,0.05*2*(Rand-0.5));
   
PGrSpriteNew(pf,imgs[NUM_IMGS*RandU16/MAX_U16],pf->o_cnts[PTY_POINT]*RandU16/M
AX_U16,r,dr);
  }
  NamedFramePtrSet("PickFrame",pf);
  return pf;
}

U0 CleanUp(PickFrame *pf)
{
  PObject *tempo=pf->o_root.next,*tempo1;
  while (tempo!=&pf->o_root) {
    tempo1=tempo->next;
    PObjectDel(pf,tempo);
    tempo=tempo1;
  }
  Free(pf);
}

U0 PickDemo2()
{
  I64 msg_code,p1,p2;
  PObject *tempo;
  PickFrame *pf=NULL;

  NamedFramePtrAdd("PickFrame",pf);

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Play {"
"  Restart(,CH_NEW_LINE);"
"}"
);
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  WinBorder;
  LtfClear;
  "$BK,1$Move things around.$BK,0$\n";
  WordStat;
  Preempt(OFF);
  pf=Init;
  tempo=NULL;
  Fs->win_inhibit=WIF_ALL-WIF_CTRLS-WIF_MENU;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch
(msg_code=GetMsg(&p1,&p2,1<<MSG_KEY_DOWN|1<<MSG_IP_L_DOWN|1<<MSG_IP_L_UP|1<<MS
G_IP_MOVE)) {
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case CH_NEW_LINE:
	      CleanUp(pf);
	      pf=Init;
	      tempo=NULL;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto pd_done;
	  }
	  break;
	case MSG_IP_L_DOWN:
	  tempo=PPointFind(pf,p1-pf->cx,p2-pf->cy);
	  break;
	case MSG_IP_L_UP:
	  if (tempo) {
	    tempo->p.p.x=p1-pf->cx;
	    tempo->p.p.y=p2-pf->cy;
	    tempo=NULL;
	  }
	  break;
	case MSG_IP_MOVE:
	  if (tempo) {
	    tempo->p.p.x=p1-pf->cx;
	    tempo->p.p.y=p2-pf->cy;
	  }
	  break;
      }
    }
pd_done:
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
  CleanUp(pf);
  NamedFramePtrDel("PickFrame");
}

PickDemo2;
