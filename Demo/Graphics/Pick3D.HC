//This is a whimsical program which demonstrates some techniques.

#define BORDER		20

#define PTY_PT	0
#define PTY_CIRCLE	1
#define PTY_LINE	2
#define PTY_SPRITE	3
#define PTY_NUM		4

extern class PObj;

class PPt
{
  CD3I32 p;
};

class PCircle
{
  PObj	*p;
  I64	radius;
};

class PLine
{
  PObj	*p1,*p2;
};

class PCSprite
{
  PObj	*p;
  U8	*img;
  I64	*r,
	*dr; //Rounding error might eventually screw this up
}

class PObj
{
  PObj	*next,*last;
  I64	type,color;
  union {
    PPt		p;
    PCircle	c;
    PLine	l;
    PCSprite	g;
  };
};

class PickFrame
{
  PObj	o_head;
  I64	o_cnts[PTY_NUM];
  I64	cx,cy;
};

#define IMGS_NUM	3



  /* <1> <1> (image) */



  /* <2> <2> (image) */




  /* <3> <3> (image) */


U8 *imgs[IMGS_NUM]={__BIN_1,__BIN_2,__BIN_3};

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 *r,*old_r;
  PickFrame *pf=FramePtr("PickFrame",task);
  PObj *tmpo=pf->o_head.next;

  pf->cx=task->pix_width>>1;
  pf->cy=task->pix_height>>1;

  DCDepthBufAlloc(dc);

  dc->color=LTRED;
  dc->pen_width=3;
  GrBorder(dc,BORDER,BORDER,2*pf->cx-BORDER,2*pf->cy-BORDER);

  while (tmpo!=&pf->o_head) {
    dc->color=tmpo->color;
    switch (tmpo->type) {
      case PTY_PT:
	GrLine(dc,pf->cx+tmpo->p.p.x+2,pf->cy+tmpo->p.p.y+2,
	      pf->cx+tmpo->p.p.x-2,pf->cy+tmpo->p.p.y-2);
	GrLine(dc,pf->cx+tmpo->p.p.x-2,pf->cy+tmpo->p.p.y+2,
	      pf->cx+tmpo->p.p.x+2,pf->cy+tmpo->p.p.y-2);
	break;
      case PTY_CIRCLE:
	GrCircle(dc,pf->cx+tmpo->c.p->p.p.x,pf->cy+tmpo->c.p->p.p.y,
	      tmpo->c.radius);
	break;
      case PTY_LINE:
	GrLine(dc,pf->cx+tmpo->l.p1->p.p.x,pf->cy+tmpo->l.p1->p.p.y,
	      pf->cx+tmpo->l.p2->p.p.x,pf->cy+tmpo->l.p2->p.p.y);
	break;
      case PTY_SPRITE:
	old_r=dc->r;
	dc->r=tmpo->g.r;
	dc->x=pf->cx+tmpo->g.p->p.p.x;
	dc->y=pf->cy+tmpo->g.p->p.p.y;
	dc->z=GR_Z_ALL;
	dc->flags|=DCF_TRANSFORMATION;
	Sprite3(dc,0,0,0,tmpo->g.img);
	dc->flags&=~DCF_TRANSFORMATION;
	dc->r=old_r;

	//Updated each refresh, not guarenteed to be uniform.
	//Rounding error might corrupt, as well.
	r=Mat4x4MulMat4x4New(tmpo->g.dr,tmpo->g.r,task);
	Free(tmpo->g.r);
	tmpo->g.r=r;

	break;
    }
    tmpo=tmpo->next;
  }
}

PObj *PObjNew(PickFrame *pf,I64 type,I64 color)
{
  PObj *tmpo=CAlloc(sizeof(PObj));
  tmpo->type=type;
  tmpo->color=color;
  pf->o_cnts[type]++;
  QueIns(tmpo,pf->o_head.last);
  return tmpo;
}

U0 PObjDel(PickFrame *pf,PObj *tmpo)
{
  QueRem(tmpo);
  switch (tmpo->type) {
    case PTY_SPRITE:
      Free(tmpo->g.r);
      Free(tmpo->g.dr);
      break;
  }
  pf->o_cnts[tmpo->type]--;
  Free(tmpo);
}

PObj *PPtNew(PickFrame *pf,I64 x,I64 y)
{
  PObj *tmpo=PObjNew(pf,PTY_PT,BLACK);
  tmpo->p.p.x=x;
  tmpo->p.p.y=y;
  return tmpo;
}

PObj *PPtNum(PickFrame *pf,I64 num)
{
  PObj *tmpo=pf->o_head.next;
  while (tmpo!=&pf->o_head) {
    if (tmpo->type==PTY_PT && !num--)
      return tmpo;
    tmpo=tmpo->next;
  }
  return NULL;
}

PObj *PPtFind(PickFrame *pf,I64 x,I64 y)
{
  I64 dd,best_dd=I64_MAX;
  PObj *tmpo=pf->o_head.next,*res=NULL;
  while (tmpo!=&pf->o_head) {
    if (tmpo->type==PTY_PT) {
      dd=SqrI64(tmpo->p.p.x-x)+SqrI64(tmpo->p.p.y-y);
      if (dd<best_dd) {
	best_dd=dd;
	res=tmpo;
      }
    }
    tmpo=tmpo->next;
  }
  return res;
}

PObj *PCircleNew(PickFrame *pf,I64 p_num,I64 r)
{
  PObj *tmpo=PObjNew(pf,PTY_CIRCLE,RED);
  tmpo->c.p=PPtNum(pf,p_num);
  tmpo->c.radius=r;
  return tmpo;
}

PObj *PLineNew(PickFrame *pf,I64 p1_num,I64 p2_num)
{
  PObj *tmpo=PObjNew(pf,PTY_LINE,GREEN);
  tmpo->l.p1=PPtNum(pf,p1_num);
  tmpo->l.p2=PPtNum(pf,p2_num);
  return tmpo;
}

PObj *PCSpriteNew(PickFrame *pf,U8 *img,I64 p_num,I64 *r,I64 *dr)
{
  PObj *tmpo=PObjNew(pf,PTY_SPRITE,BLACK);
  tmpo->g.p=PPtNum(pf,p_num);
  tmpo->g.img=img;
  tmpo->g.r=r;
  tmpo->g.dr=dr;
  return tmpo;
}

PickFrame *Init()
{
  PickFrame *pf=CAlloc(sizeof(PickFrame));
  I64 i,*r,*dr;

  pf->cx=Fs->pix_width>>1;
  pf->cy=Fs->pix_height>>1;

  pf->o_head.next=pf->o_head.last=&pf->o_head;
  for (i=0;i<50;i++)
    PPtNew(pf,RandI32%(pf->cx-BORDER),RandI32%(pf->cy-BORDER));
  for (i=0;i<20;i++)
    PCircleNew(pf,pf->o_cnts[PTY_PT]*RandU16/U16_MAX,6);
  for (i=0;i<20;i++)
    PLineNew(pf,pf->o_cnts[PTY_PT]*RandU16/U16_MAX,
	  pf->o_cnts[PTY_PT]*RandU16/U16_MAX);
  for (i=0;i<10;i++) {
    r=Mat4x4IdentNew;
    dr=Mat4x4IdentNew;
    Mat4x4RotZ(dr,0.05*2*(Rand-0.5));
    Mat4x4RotY(dr,0.05*2*(Rand-0.5));
    Mat4x4RotX(dr,0.05*2*(Rand-0.5));
    PCSpriteNew(pf,imgs[IMGS_NUM*RandU16/U16_MAX],
	  pf->o_cnts[PTY_PT]*RandU16/U16_MAX,r,dr);
  }
  FramePtrSet("PickFrame",pf);
  return pf;
}

U0 CleanUp(PickFrame *pf)
{
  PObj *tmpo=pf->o_head.next,*tmpo1;
  while (tmpo!=&pf->o_head) {
    tmpo1=tmpo->next;
    PObjDel(pf,tmpo);
    tmpo=tmpo1;
  }
  Free(pf);
}

U0 Pick3D()
{
  I64 msg_code,arg1,arg2;
  PObj *tmpo;
  PickFrame *pf=NULL;

  FramePtrAdd("PickFrame");

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"}"
	);
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocClear;
  "$BK,1$Move things around.$BK,0$\n";
  pf=Init;
  tmpo=NULL;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS
	-WIF_SELF_CTRLS-WIF_FOCUS_TASK_MENU;
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (msg_code=GetMsg(&arg1,&arg2,
	    1<<MSG_KEY_DOWN|1<<MSG_MS_L_DOWN|1<<MSG_MS_L_UP|1<<MSG_MS_MOVE)) {
	case MSG_KEY_DOWN:
	  switch (arg1) {
	    case '\n':
	      CleanUp(pf);
	      pf=Init;
	      tmpo=NULL;
	      break;
	    case CH_SHIFT_ESC:
	    case CH_ESC:
	      goto pd_done;
	  }
	  break;
	case MSG_MS_L_DOWN:
	  tmpo=PPtFind(pf,arg1-pf->cx,arg2-pf->cy);
	  break;
	case MSG_MS_L_UP:
	  if (tmpo) {
	    tmpo->p.p.x=arg1-pf->cx;
	    tmpo->p.p.y=arg2-pf->cy;
	    tmpo=NULL;
	  }
	  break;
	case MSG_MS_MOVE:
	  if (tmpo) {
	    tmpo->p.p.x=arg1-pf->cx;
	    tmpo->p.p.y=arg2-pf->cy;
	  }
	  break;
      }
    }
pd_done:
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
  CleanUp(pf);
  FramePtrDel("PickFrame");
}

Pick3D;
