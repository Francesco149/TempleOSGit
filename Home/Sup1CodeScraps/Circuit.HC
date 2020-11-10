
/* <1> <1> (image) */

/* <2> <2> (image) */

/* <3> <3> (image) */

/* <4> <4> (image) */

/* <5> <5> (image) */

/* <6> <6> (image) */

#define A_EAST	0
#define A_NORTH	1
#define A_WEST	2
#define A_SOUTH	3

U0 OrthRot(I64 *_x,I64 *_y,I64 theta)
{
  switch [theta&3] {
    case A_EAST:
      break;
    case A_NORTH:
      *_y=-*_y;
      SwapI64(_x,_y);
      break;
    case A_WEST:
      *_x=-*_x;
      *_y=-*_y;
      break;
    case A_SOUTH:
      *_x=-*_x;
      SwapI64(_x,_y);
      break;
  }
}

#define TERMINALS_NUM	6

#define ET_R		0
#define ET_L		1
#define ET_C		2
#define ET_TYPES_NUM	3

class ElementType
{
  U8 *black_img,*red_img;
  I64 terminals;
  CD2I64 term[TERMINALS_NUM];
  I64 min_x,max_x,min_y,max_y;
};

ElementType et[ET_TYPES_NUM] =
{
    {__BIN_1,__BIN_2,2,{{-12,1},{13,1},{0,0},{0,0},{0,0},{0,0}},0,0,0,0},
	  {__BIN_3,__BIN_4,2,{{-14,0},{15,0},{0,0},{0,0},{0,0},{0,0}},0,0,0,0},
	  {__BIN_5,__BIN_6,2,{{-4 ,0},{6 ,0},{0,0},{0,0},{0,0},{0,0}},0,0,0,0},
};

extern class Element;
class Terminal
{
  Element *e;
  I64 terminal;
};

#define Df_PROCESSED		0
#define Df_JUNCTION		1
#define Df_TERMINAL		2
#define Df_TERMINAL_LEFT	3
#define Df_TERMINAL_RIGHT	4
#define Df_TERMINAL_TOP		5
#define Df_TERMINAL_BOTTOM	6

class Dot  //These are points on a node
{
  Dot *next,*last;
  I64 x,y,saved_x,saved_y;
  Element *e;
  U16  flags[4];
  Dot *left,*right,*top,*bottom;
};

class Node
{
  Node *next,*last;
  I64 cnt;
  Terminal *terms;
  Dot *dot_head;
} node_head;

class Element
{
  Element *next,*last;
  ElementType *type;
  I64 x,y,theta,min_x,max_x,min_y,max_y;
  Node *nodes[TERMINALS_NUM];
} el_head;

#define MD_CONNECT	0
#define MD_MOVE		1
#define MD_MODES_NUM	2
I64 mode;

Element *rubber_band;
I64 rubber_band_terminal,rubber_band_x,rubber_band_y;

U0 DotDel(Dot *head)
{
  if (head) {
    QueDel(head,TRUE);
    Free(head);
  }
}

U0 DotRst(Dot *head)
{
  Dot *tmpd;
  if (head) {
    Bts(head->flags,Df_PROCESSED);
    tmpd=head->next;
    while (tmpd!=head) {
      Btr(tmpd->flags,Df_PROCESSED);
      tmpd=tmpd->next;
    }
  }
}

#define DIR_LT	0
#define DIR_CT	1
#define DIR_RT	2
#define DIR_LC	3
#define DIR_CC	4
#define DIR_RC	5
#define DIR_LB	6
#define DIR_CB	7
#define DIR_RB	8

I64 ElementDistSqr(Element *tmpe,I64 x,I64 y)
{
  I64 i,dx,dy;
  if (x<tmpe->min_x)
    i=0;
  else if (x<=tmpe->max_x)
    i=1;
  else
    i=2;
  if (y>=tmpe->min_y) {
    if (y<=tmpe->max_y)
      i+=3;
    else
      i+=6;
  }
  switch (i) {
    case DIR_LT:
      dx=tmpe->min_x-x;
      dy=tmpe->min_y-y;
      break;
    case DIR_CT:
      dx=0;
      dy=tmpe->min_y-y;
      break;
    case DIR_RT:
      dx=x-tmpe->max_x;
      dy=tmpe->min_y-y;
      break;
    case DIR_LC:
      dx=tmpe->min_x-x;
      dy=0;
      break;
    case DIR_CC:
      dx=0;
      dy=0;
      break;
    case DIR_RC:
      dx=x-tmpe->max_x;
      dy=0;
      break;
    case DIR_LB:
      dx=tmpe->min_x-x;
      dy=y-tmpe->max_y;
      break;
    case DIR_CB:
      dx=0;
      dy=y-tmpe->max_y;
      break;
    case DIR_RB:
      dx=x-tmpe->max_x;
      dy=y-tmpe->max_y;
      break;
  }
  return SqrI64(dx)+SqrI64(dy);
}

#define SPACING 10

F64 DotNetScore(Dot *head)
{ //higher is better
  F64 d,distdist1=0.1,distdist2=0.1,num=1.0,den=0.1;
  I64 dx,dy,nonzero_cnt=1;
  Element *tmpe;
  Dot *tmpd,*tmpd1;
  if (head) {
    tmpd=head->next;
    while (tmpd!=head) {

      //Calculate clearance from elements like parallel resistors
      tmpe=el_head.next;
      while (tmpe!=&el_head) {
	if (tmpd->e!=tmpe) {
	  d=Sqrt(ElementDistSqr(tmpe,tmpd->x,tmpd->y))+0.1;
	  if (d>SPACING) d=SPACING;
	  num*=d;
	  den+=d;
	}
	tmpe=tmpe->next;
      }

      //Calculate clearance from junctions like parallel resistors
      if (Bt(tmpd->flags,Df_TERMINAL)) {
	tmpd1=head->next;
	while (tmpd1!=head) {
	  if (tmpd1!=tmpd && Bt(tmpd1->flags,Df_JUNCTION)) {
	    d=Sqrt(SqrI64(tmpd->x-tmpd1->x)+SqrI64(tmpd->y-tmpd1->y))+0.1;
	    if (d>SPACING) d=SPACING;
	    num*=d;
	    den+=d;
	  }
	  tmpd1=tmpd1->next;
	}
      }

      if (tmpd1=tmpd->left) {
	dx=SqrI64(tmpd->x-tmpd1->x);
	dy=SqrI64(tmpd->y-tmpd1->y);
	if (d=dx+dy) {
	  distdist1+=d;
	  nonzero_cnt++;
	}
	distdist2+=dy;
      }
      if (tmpd1=tmpd->right) {
	dx=SqrI64(tmpd->x-tmpd1->x);
	dy=SqrI64(tmpd->y-tmpd1->y);
	if (d=dx+dy) {
	  distdist1+=d;
	  nonzero_cnt++;
	}
	distdist2+=dy;
      }
      if (tmpd1=tmpd->top) {
	dx=SqrI64(tmpd->x-tmpd1->x);
	dy=SqrI64(tmpd->y-tmpd1->y);
	if (d=dx+dy) {
	  distdist1+=d;
	  nonzero_cnt++;
	}
	distdist2+=dx;
      }
      if (tmpd1=tmpd->bottom) {
	dx=SqrI64(tmpd->x-tmpd1->x);
	dy=SqrI64(tmpd->y-tmpd1->y);
	if (d=dx+dy) {
	  distdist1+=d;
	  nonzero_cnt++;
	}
	distdist2+=dx;
      }
      tmpd=tmpd->next;
    }
  }
  return num/(nonzero_cnt*nonzero_cnt*nonzero_cnt*distdist1*distdist2);
}

Bool NodeAdd(Node *tmpn,Element *tmpe,I64 terminal,CTask *mem_task=NULL)
{
  I64 i;
  Terminal *a;
  for (i=0;i<tmpn->cnt;i++)
    if (tmpn->terms[i].e==tmpe&&tmpn->terms[i].terminal==terminal)
      return TRUE;
  a=MAlloc((tmpn->cnt+1)*sizeof(Terminal),mem_task);
  if (tmpn->terms) {
    MemCpy(a,tmpn->terms,tmpn->cnt*sizeof(Terminal));
    Free(tmpn->terms);
  }
  tmpn->terms=a;
  i=tmpn->cnt++;
  tmpn->terms[i].e=tmpe;
  tmpn->terms[i].terminal=terminal;
  return FALSE;
}

U0 NodeDel(Node *tmpn)
{
  if (tmpn) {
    QueRem(tmpn);
    Free(tmpn->terms);
    if (tmpn->dot_head)
      DotDel(tmpn->dot_head);
    Free(tmpn);
  }
}

extern Dot *DotNew(Node *tmpn,CTask *mem_task=NULL);

U0 NodeReroute(Node *tmpn=NULL,CTask *mem_task=NULL)
{
  if (tmpn) {
    DotDel(tmpn->dot_head);
    tmpn->dot_head=DotNew(tmpn,mem_task);
  } else {
    tmpn=node_head.next;
    while (tmpn!=&node_head) {
      DotDel(tmpn->dot_head);
      tmpn->dot_head=DotNew(tmpn,mem_task);
      tmpn=tmpn->next;
    }
  }
}

U0 ElementExtents(Element *tmpe)
{
  ElementType *tmpt=tmpe->type;
  I64 x,y;
  x=tmpt->min_x; y=tmpt->min_y;
  OrthRot(&x,&y,tmpe->theta);
  x+=tmpe->x;	  y+=tmpe->y;
  tmpe->min_x=tmpe->max_x=x;
  tmpe->min_y=tmpe->max_y=y;
  x=tmpt->max_x; y=tmpt->max_y;
  OrthRot(&x,&y,tmpe->theta);
  x+=tmpe->x;	  y+=tmpe->y;
  if (x<tmpe->min_x) tmpe->min_x=x;
  if (x>tmpe->max_x) tmpe->max_x=x;
  if (y<tmpe->min_y) tmpe->min_y=y;
  if (y>tmpe->max_y) tmpe->max_y=y;
  if (tmpe==rubber_band) {
    tmpe->min_x+=ms.pos.x-rubber_band_x;
    tmpe->max_x+=ms.pos.x-rubber_band_x;
    tmpe->min_y+=ms.pos.y-rubber_band_y;
    tmpe->max_y+=ms.pos.y-rubber_band_y;
  }
}

U0 ElementReroute(Element *tmpe,CTask *mem_task=NULL)
{
  I64 i;
  ElementType *tmpt;
  if (tmpe) {
    tmpt=tmpe->type;
    for (i=0;i<tmpt->terminals;i++)
      if (tmpe->nodes[i])
	NodeReroute(tmpe->nodes[i],mem_task);
  }
}

U0 ElementDel(Element *tmpe,CTask *mem_task=NULL)
{
  I64 i,cnt;
  Terminal *src,*dst;
  Node *tmpn=node_head.next,*tmpn1;
  while (tmpn!=&node_head) {
    tmpn1=tmpn->next;
    cnt=0;
    dst=src=tmpn->terms;
    for (i=0;i<tmpn->cnt;i++) {
      if (src->e!=tmpe) {
	if (dst!=src)
	  MemCpy(dst,src,sizeof(Terminal));
	cnt++;
	dst++;
      }
      src++;
    }
    if (tmpn->cnt!=cnt) {
      tmpn->cnt=cnt;
      if (!cnt)
	NodeDel(tmpn);
      else if (cnt==1) {
	tmpn->terms[0].e->nodes[tmpn->terms[0].terminal]=NULL;
	NodeDel(tmpn);
      } else {
	DotDel(tmpn->dot_head);
	tmpn->dot_head=NULL;
	NodeReroute(tmpn,mem_task);
      }
    }
    tmpn=tmpn1;
  }
  QueRem(tmpe);
  Free(tmpe);
}

Node *NodeNew(CTask *mem_task=NULL)
{
  Node *res=CAlloc(sizeof(Node),mem_task);
  QueIns(res,node_head.last);
  return res;
}

Node *NodeCat(Node *dst,Node *src,CTask *mem_task=NULL)
{
  I64 i;
  if (!dst) dst=NodeNew(mem_task);
  if (src && dst!=src) {
    for (i=0;i<src->cnt;i++) {
      NodeAdd(dst,src->terms[i].e,src->terms[i].terminal,mem_task);
      src->terms[i].e->nodes[src->terms[i].terminal]=dst;
    }
    NodeDel(src);
  }
  return dst;
}

U0 TerminalXY(Element *tmpe,I64 terminal,I64 *_x,I64 *_y)
{
  ElementType *tmpt=tmpe->type;
  I64 x=tmpt->term[terminal].x,y=tmpt->term[terminal].y;
  OrthRot(&x,&y,tmpe->theta);
  if (_x)
    *_x=tmpe->x+x;
  if (_y)
    *_y=tmpe->y+y;
}

#define CT_NONE		0
#define CT_LEFT		1
#define CT_RIGHT	2
#define CT_TOP		3
#define CT_BOTTOM	4

I64 DotHVDecideConnect(Dot *tmpd,Dot *tmpd1)
{
  I64 d;
  d=tmpd->x-tmpd1->x;
  if (d<0) {
    if (!tmpd->right  && !tmpd1->left ||
	  tmpd->right==tmpd1	&& tmpd1->left==tmpd)
      return CT_RIGHT;
  } else if (d>0) {
    if (!tmpd->left   && !tmpd1->right ||
	  tmpd->left==tmpd1	&& tmpd1->right==tmpd)
      return CT_LEFT;
  }
  d=tmpd->y-tmpd1->y;
  if (d<0) {
    if (!tmpd->bottom && !tmpd1->top ||
	  tmpd->bottom==tmpd1 && tmpd1->top==tmpd)
      return CT_BOTTOM;
  } else if (d>0) {
    if (!tmpd->top    && !tmpd1->bottom ||
	  tmpd->top==tmpd1	&& tmpd->bottom==tmpd)
      return CT_TOP;
  }
  return CT_NONE;
}

Bool DotHVConnect(Dot *tmpd,Dot *tmpd1)
{
  switch (DotHVDecideConnect(tmpd,tmpd1)) {
    case CT_RIGHT:
      tmpd ->right=tmpd1;
      tmpd1->left =tmpd;
      return TRUE;
    case CT_LEFT:
      tmpd ->left =tmpd1;
      tmpd1->right=tmpd;
      return TRUE;
    case CT_BOTTOM:
      tmpd ->bottom=tmpd1;
      tmpd1->top   =tmpd;
      return TRUE;
    case CT_TOP:
      tmpd ->top   =tmpd1;
      tmpd1->bottom=tmpd;
      return TRUE;
    default:
      return FALSE;
  }
}

Dot *DotHVInterconnect(Dot *tmpd,Dot *tmpd1,CTask *mem_task=NULL)
{
  I64 type;
  Dot *res=CAlloc(sizeof(Dot),mem_task);
  if (Bt(tmpd ->flags,Df_TERMINAL_LEFT)||
	Bt(tmpd ->flags,Df_TERMINAL_RIGHT)) {	 //d horizontal
    type=0;
    if (Bt(tmpd1->flags,Df_TERMINAL_TOP)||
	  Bt(tmpd1->flags,Df_TERMINAL_BOTTOM)) {//d1 vertical
      if (tmpd->y<tmpd1->y && Bt(tmpd1->flags,Df_TERMINAL_TOP)||
	    tmpd->y>tmpd1->y && Bt(tmpd1->flags,Df_TERMINAL_BOTTOM))
	type=1;
    } else {
      if (tmpd->x>tmpd1->x && Bt(tmpd1->flags,Df_TERMINAL_LEFT)||
	    tmpd->x<tmpd1->x && Bt(tmpd1->flags,Df_TERMINAL_RIGHT))
	type=1;
    }
  } else {				     //d vertical
    type=1;
    if (Bt(tmpd1->flags,Df_TERMINAL_LEFT)||
	  Bt(tmpd1->flags,Df_TERMINAL_RIGHT)) { //d1 horizontal
      if (tmpd->x<tmpd1->x && Bt(tmpd1->flags,Df_TERMINAL_LEFT)||
	    tmpd->x>tmpd1->x && Bt(tmpd1->flags,Df_TERMINAL_RIGHT))
	type=0;
    } else {
      if (tmpd->y>tmpd1->y && Bt(tmpd1->flags,Df_TERMINAL_TOP)||
	    tmpd->y<tmpd1->y && Bt(tmpd1->flags,Df_TERMINAL_BOTTOM))
	type=0;
    }
  }
  if (type==1) {
    res->x=tmpd1->x;
    res->y=tmpd ->y;
  } else {
    res->x=tmpd ->x;
    res->y=tmpd1->y;
  }
  return res;
}

Dot *DotNearest(Dot *head,Dot *tmpd,Bool just_processed)
{
  I64 best_dd=I64_MAX,dx,dy,dd;
  Dot *res=NULL,*tmpd1,*tmpd2;
  if (head && tmpd) {
    tmpd1=head->next;
    while (tmpd1!=head) {
      if (tmpd1!=tmpd && (!just_processed||Bt(tmpd1->flags,Df_PROCESSED))) {
	dx=tmpd->x-tmpd1->x;
	dy=tmpd->y-tmpd1->y;
	dd=SqrI64(dx)+SqrI64(dy);
	if (0<dd<best_dd) {
	  if (dx && dy) {
	    tmpd2=DotHVInterconnect(tmpd,tmpd1);
	    if (DotHVDecideConnect(tmpd ,tmpd2) &&
		  DotHVDecideConnect(tmpd2,tmpd1)) {
	      best_dd=dd;
	      res=tmpd1;
	    }
	    Free(tmpd2);
	  } else {
	    if (DotHVDecideConnect(tmpd ,tmpd1)) {
	      best_dd=dd;
	      res=tmpd1;
	    }
	  }
	}
      }
      tmpd1=tmpd1->next;
    }
  }
  return res;
}

Bool DotMarkConnected(Dot *tmpd)
{
  if (tmpd && !Bts(tmpd->flags,Df_PROCESSED)) {
    DotMarkConnected(tmpd->left);
    DotMarkConnected(tmpd->right);
    DotMarkConnected(tmpd->top);
    DotMarkConnected(tmpd->bottom);
    return TRUE;
  } else
    return FALSE;
}

U0 DotSpacersAdd(Dot *head,CTask *mem_task=NULL)
{
  Dot *tmpd,*tmpd1,*tmpd2;
  if (head) {
    tmpd=head->next;
    if (tmpd!=head) {
      tmpd2=head->last;
      do {
	if (Bt(tmpd->flags,Df_TERMINAL)) {
	  tmpd1=CAlloc(sizeof(Dot),mem_task);
	  tmpd1->x=tmpd->x;
	  tmpd1->y=tmpd->y;
	  tmpd1->flags[0]=tmpd->flags[0];
	  tmpd1->e=tmpd->e;
	  if (Bt(tmpd->flags,Df_TERMINAL_LEFT)) {
	    tmpd->right=tmpd1;
	    tmpd1->left=tmpd;
	    tmpd->x--;
	  }
	  if (Bt(tmpd->flags,Df_TERMINAL_RIGHT)) {
	    tmpd->left=tmpd1;
	    tmpd1->right=tmpd;
	    tmpd->x++;
	  }
	  if (Bt(tmpd->flags,Df_TERMINAL_TOP)) {
	    tmpd->bottom=tmpd1;
	    tmpd1->top=tmpd;
	    tmpd->y--;
	  }
	  if (Bt(tmpd->flags,Df_TERMINAL_BOTTOM)) {
	    tmpd->top=tmpd1;
	    tmpd1->bottom=tmpd;
	    tmpd->y++;
	  }
	  tmpd->flags[0]=0;
	  tmpd->e=NULL;
	  QueIns(tmpd1,head->last);
	}
	tmpd=tmpd->next;
      } while (tmpd!=tmpd2);
    }
  }
}

U0 DotAddSpace(Dot *head)
{
  F64 score,best_score;
  I64 i,x,y,best_x,best_y;
  Dot *tmpd,*tmpd1;
  if (head) {
    tmpd=head->next;
    while (tmpd!=head) {
      tmpd->saved_x=tmpd->x;
      tmpd->saved_y=tmpd->y;
      tmpd=tmpd->next;
    }
    best_score=DotNetScore(head);
    tmpd=head->next;
    while (tmpd!=head) {
      if (!Bt(tmpd->flags,Df_TERMINAL)) {
	best_x=0;
	best_y=0;
	for (i=0;i<4*SPACING;i++) {
	  switch [i&3] {
	    case A_EAST:  x= i>>2; y=0; break;
	    case A_NORTH: y= i>>2; x=0; break;
	    case A_WEST:  x=-i>>2; y=0; break;
	    case A_SOUTH: y=-i>>2; x=0; break;
	  }
	  tmpd->x=tmpd->saved_x+x;
	  tmpd1=tmpd->top;
	  while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	    tmpd1->x=tmpd1->saved_x+x;
	    tmpd1=tmpd1->top;
	  }
	  tmpd1=tmpd->bottom;
	  while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	    tmpd1->x=tmpd1->saved_x+x;
	    tmpd1=tmpd1->bottom;
	  }
	  tmpd->y=tmpd->saved_y+y;
	  tmpd1=tmpd->left;
	  while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	    tmpd1->y=tmpd1->saved_y+y;
	    tmpd1=tmpd1->left;
	  }
	  tmpd1=tmpd->right;
	  while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	    tmpd1->y=tmpd1->saved_y+y;
	    tmpd1=tmpd1->right;
	  }
	  score=DotNetScore(head);
	  if (score>best_score) {
	    best_score=score;
	    best_x=x;
	    best_y=y;
	  }
	}
	tmpd->saved_x=tmpd->x=tmpd->saved_x+best_x;
	tmpd1=tmpd->top;
	while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	  tmpd1->saved_x=tmpd1->x=tmpd1->saved_x+best_x;
	  tmpd1=tmpd1->top;
	}
	tmpd1=tmpd->bottom;
	while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	  tmpd1->saved_x=tmpd1->x=tmpd1->saved_x+best_x;
	  tmpd1=tmpd1->bottom;
	}
	tmpd->saved_y=tmpd->y=tmpd->saved_y+best_y;
	tmpd1=tmpd->left;
	while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	  tmpd1->saved_y=tmpd1->y=tmpd1->saved_y+best_y;
	  tmpd1=tmpd1->left;
	}
	tmpd1=tmpd->right;
	while (tmpd1 && !Bt(tmpd1->flags,Df_TERMINAL)) {
	  tmpd1->saved_y=tmpd1->y=tmpd1->saved_y+best_y;
	  tmpd1=tmpd1->right;
	}
      }
      tmpd=tmpd->next;
    }
  }
}

U0 DotJunctions(Dot *head)
{
  I64 cnt;
  Dot *tmpd;
  tmpd=head->next;
  while (tmpd!=head) {
    if (Bt(tmpd->flags,Df_TERMINAL))
      cnt=1;
    else
      cnt=0;
    if (tmpd->left)   cnt++;
    if (tmpd->right)  cnt++;
    if (tmpd->top)    cnt++;
    if (tmpd->bottom) cnt++;
    BEqu(tmpd->flags,Df_JUNCTION,cnt>2);
    tmpd=tmpd->next;
  }
}

Dot *DotTerminalsNew(Node *tmpn,CTask *mem_task=NULL)
{
  I64 i,min_x,max_x,min_y,max_y,dx,dy,x,y;
  Element *tmpe;
  Dot *res=CAlloc(sizeof(Dot),mem_task),*tmpd;
  QueInit(res);
  for (i=0;i<tmpn->cnt;i++)
    if (tmpe=tmpn->terms[i].e) {
      tmpd=CAlloc(sizeof(Dot),mem_task);
      Bts(tmpd->flags,Df_TERMINAL);
      tmpd->e=tmpe;
      TerminalXY(tmpe,tmpn->terms[i].terminal,&tmpd->x,&tmpd->y);
      x=tmpe->x;
      y=tmpe->y;
      if (mode==MD_MOVE && tmpe==rubber_band) {
	tmpd->x+=ms.pos.x-rubber_band_x;
	tmpd->y+=ms.pos.y-rubber_band_y;
	x+=ms.pos.x-rubber_band_x;
	y+=ms.pos.y-rubber_band_y;
      }

      min_x=AbsI64(tmpd->x-tmpe->min_x);
      max_x=AbsI64(tmpd->x-tmpe->max_x);
      if (min_x<max_x)
	dx=min_x;
      else
	dx=max_x;

      min_y=AbsI64(tmpd->y-tmpe->min_y);
      max_y=AbsI64(tmpd->y-tmpe->max_y);
      if (min_y<max_y)
	dy=min_y;
      else
	dy=max_y;

      if (dx<=dy) {
	if (tmpd->x<=x)
	  Bts(tmpd->flags,Df_TERMINAL_LEFT);
	else
	  Bts(tmpd->flags,Df_TERMINAL_RIGHT);
      } else {
	if (tmpd->y<=y)
	  Bts(tmpd->flags,Df_TERMINAL_TOP);
	else
	  Bts(tmpd->flags,Df_TERMINAL_BOTTOM);
      }
      QueIns(tmpd,res->last);
    }
  return res;
}

Dot *DotNew(Node *tmpn,CTask *mem_task=NULL)
{
  I64 cnt,pass=0,last_cnt=0;
  Bool cont;
  Dot *tmpd,*tmpd1,*tmpd2,*tmpd3;
  Dot *res=DotTerminalsNew(tmpn,mem_task);
  do {
    pass++;
    tmpd=res->next;
    while (tmpd!=res) {
      if (!Bt(tmpd->flags,Df_PROCESSED)) {
	if (tmpd1=DotNearest(res,tmpd,pass>1)) {
	  if (tmpd->x==tmpd1->x || tmpd->y==tmpd1->y) {
	    if (tmpd->x!=tmpd1->x || tmpd->y!=tmpd1->y) {
	      if (DotHVConnect(tmpd,tmpd1))
		DotMarkConnected(tmpd);
	    }
	  }else {
	    tmpd2=DotHVInterconnect(tmpd,tmpd1,mem_task);
	    tmpd3=res->next;
	    while (tmpd3!=res) {
	      if (tmpd2->x==tmpd3->x && tmpd2->y==tmpd3->y)
		break;
	      tmpd3=tmpd3->next;
	    }
	    if (tmpd3!=res) {
	      Free(tmpd2);
	      tmpd2=tmpd3;
	    } else
	      QueIns(tmpd2,res->last);

	    if (DotHVConnect(tmpd,tmpd2)) {
	      DotMarkConnected(tmpd);
	      if (DotHVConnect(tmpd2,tmpd1))
		DotMarkConnected(tmpd1);
	    } else
	      DotHVConnect(tmpd2,tmpd1);
	  }
	}
      }
      tmpd=tmpd->next;
    }

    tmpd=res->next;
    while (tmpd!=res) {
      tmpd1=tmpd->next;
      while (tmpd1!=res) {
	if (tmpd1->y==tmpd->y) {
	  if ((tmpd2=tmpd->left  ) && tmpd2->x<tmpd1->x<tmpd ->x) {
	    tmpd ->left =tmpd1;
	    tmpd1->right=tmpd ;
	    tmpd1->left =tmpd2;
	    tmpd2->right=tmpd1;
	  }
	  if ((tmpd2=tmpd->right ) && tmpd ->x<tmpd1->x<tmpd2->x) {
	    tmpd2->left =tmpd1;
	    tmpd1->right=tmpd2;
	    tmpd1->left =tmpd ;
	    tmpd ->right=tmpd1;
	  }
	}
	if (tmpd1->x==tmpd->x) {
	  if ((tmpd2=tmpd->top	 ) && tmpd2->y<tmpd1->y<tmpd ->y) {
	    tmpd ->top	=tmpd1;
	    tmpd1->bottom=tmpd ;
	    tmpd1->top	=tmpd2;
	    tmpd2->bottom=tmpd1;
	  }
	  if ((tmpd2=tmpd->bottom) && tmpd ->y<tmpd1->y<tmpd2->y) {
	    tmpd2->top	=tmpd1;
	    tmpd1->bottom=tmpd2;
	    tmpd1->top	=tmpd ;
	    tmpd ->bottom=tmpd1;
	  }
	}
	tmpd1=tmpd1->next;
      }
      tmpd=tmpd->next;
    }

    cont=FALSE;
    DotRst(res);
    cnt=0;
    tmpd=res->next;
    if (tmpd!=res) {
      DotMarkConnected(tmpd);
      while (tmpd!=res) {
	if (Bt(tmpd->flags,Df_PROCESSED))
	  cnt++;
	else
	  cont=TRUE;
	tmpd=tmpd->next;
      }
      if (last_cnt==cnt)
	break;
      last_cnt=cnt;
    }
  } while (cont);
  DotJunctions(res);
  DotSpacersAdd(res,mem_task);
  DotAddSpace(res);
  DotJunctions(res);
  return res;
}

U0 DotDraw(CDC *dc,Dot *head)
{
  Dot *tmpd,*tmpd1;
  if (head) {
    tmpd=head->next;
    while (tmpd!=head) {
      Bts(tmpd->flags,Df_PROCESSED);
      if ((tmpd1=tmpd->right)  && !Bt(tmpd1->flags,Df_PROCESSED))
	GrLine(dc,tmpd->x,tmpd->y,tmpd1->x,tmpd1->y);
      if ((tmpd1=tmpd->left)	 && !Bt(tmpd1->flags,Df_PROCESSED))
	GrLine(dc,tmpd->x,tmpd->y,tmpd1->x,tmpd1->y);
      if ((tmpd1=tmpd->top)	 && !Bt(tmpd1->flags,Df_PROCESSED))
	GrLine(dc,tmpd->x,tmpd->y,tmpd1->x,tmpd1->y);
      if ((tmpd1=tmpd->bottom) && !Bt(tmpd1->flags,Df_PROCESSED))
	GrLine(dc,tmpd->x,tmpd->y,tmpd1->x,tmpd1->y);
      if (Bt(tmpd->flags,Df_JUNCTION)) {
	dc->thick=3;
	GrPlot3(dc,tmpd->x,tmpd->y,0);
	dc->thick=1;
      }
      tmpd=tmpd->next;
    }
  }
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,x1,y1,x2,y2;
  U8 *img;
  Node *tmpn=node_head.next;
  Element *tmpe=el_head.next;
  while (tmpe!=&el_head) {
    x1=tmpe->x;
    y1=tmpe->y;
    if (mode==MD_MOVE && tmpe==rubber_band) {
      img=tmpe->type->red_img;
      x1+=ms.pos.x-rubber_band_x;
      y1+=ms.pos.y-rubber_band_y;
    } else
      img=tmpe->type->black_img;
    Sprite3ZB(dc,x1,y1,0,img,tmpe->theta*pi/2);
    tmpe=tmpe->next;
  }
  while (tmpn!=&node_head) {
    dc->color=BLUE;
    if (mode==MD_MOVE)
      for (i=0;i<tmpn->cnt;i++)
	if (tmpn->terms[i].e==rubber_band) {
	  dc->color=RED;
	  break;
	}
    DotRst(tmpn->dot_head);
    DotDraw(dc,tmpn->dot_head);
    tmpn=tmpn->next;
  }
  if (mode==MD_CONNECT && rubber_band && rubber_band_terminal>=0) {
    TerminalXY(rubber_band,rubber_band_terminal,&x1,&y1);
    x2=ms.pos.x-task->pix_left-task->scroll_x;
    y2=ms.pos.y-task->pix_top -task->scroll_y;
    dc->color=RED;
    GrLine(dc,x1,y1,x2,y2);
  }

  dc->color=BLACK;
  GrPrint(dc,0,0,"Right Mouse Change Modes");
  switch (mode) {
    case MD_CONNECT:
      GrPrint(dc,20,FONT_HEIGHT,"Connect Components");
      break;
    case MD_MOVE:
      GrPrint(dc,20,FONT_HEIGHT,"Move Components");
      break;
  }
}

U0 RubberReset()
{
  rubber_band=NULL; rubber_band_terminal=-1;
  rubber_band_x=0; rubber_band_y=0;
}

U0 Init()
{
  I64 i;
  ElementType *tmpt;
  for (i=0;i<ET_TYPES_NUM;i++) {
    tmpt=&et[i];
    SpriteExtents(tmpt->black_img,&tmpt->min_x,&tmpt->max_x,
	  &tmpt->min_y,&tmpt->max_y);
  }
  mode=MD_CONNECT;
  RubberReset;
  QueInit(&el_head);
  QueInit(&node_head);
}

U0 CleanUp()
{
  Element *tmpe=el_head.next,*tmpe1;
  Node *tmpn=node_head.next,*tmpn1;
  while (tmpe!=&el_head) {
    tmpe1=tmpe->next;
    QueRem(tmpe);
    Free(tmpe);
    tmpe=tmpe1;
  }
  while (tmpn!=&node_head) {
    tmpn1=tmpn->next;
    NodeDel(tmpn);
    tmpn=tmpn1;
  }
}

Element *ElementNew(I64 t,CTask *mem_task=NULL)
{
  Element *tmpe=CAlloc(sizeof(Element),mem_task);
  tmpe->type=&et[t];
  tmpe->x=ms.pos.x-Fs->pix_left-Fs->scroll_x;
  tmpe->y=ms.pos.y-Fs->pix_top -Fs->scroll_y;
  QueIns(tmpe,el_head.last);
  ElementExtents(tmpe);
  return tmpe;
}

Element *ElementFind(I64 x,I64 y,I64 *_terminal)
{
  I64 i,x2,y2,best_dd=I64_MAX,dd;
  Element *res=NULL,*tmpe=el_head.next;
  if (_terminal) *_terminal=0;
  while (tmpe!=&el_head) {
    for (i=0;i<tmpe->type->terminals;i++) {
      TerminalXY(tmpe,i,&x2,&y2);
      dd=SqrI64(x-x2)+SqrI64(y-y2);
      if (dd<best_dd) {
	best_dd=dd;
	res=tmpe;
	if (_terminal) *_terminal=i;
      }
    }
    tmpe=tmpe->next;
  }
  return res;
}

U0 Circuit()
{
  I64 arg1,arg2,terminal1=-1,terminal2=-1;
  Element *tmpe1=NULL,*tmpe2=NULL;
  Node *tmpn;

  PopUpOk(
	"This is not useful.\n"
	"It just lets you draw\n"
	"a circuit.\n");

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  MenuPush(
	"File {"
	"  New(,'\n');"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Edit {"
	"  Rotate(,CH_SPACE);"
	"  DelElement(,,SC_DELETE);"
	"}"
	"Mode {"
	"  Connect(,0,SC_F2);"
	"  Move(,0,SC_F3);"
	"}"
	"PlaceElement {"
	"  Resistor(,'r');"
	"  Inductor(,'l');"
	"  Capacitor(,'c');"
	"}"
	);
  Init;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS
	-WIF_SELF_BORDER-WIF_FOCUS_TASK_MENU-WIF_SELF_GRAB_SCROLL;
  try {
    while (TRUE) {
      switch (GetMsg(&arg1,&arg2,1<<MSG_KEY_DOWN+1<<MSG_MS_MOVE+
	    1<<MSG_MS_L_DOWN+1<<MSG_MS_L_UP+
	    1<<MSG_MS_R_DOWN)) {
	case MSG_KEY_DOWN:
	  switch (arg1) {
	    case 0:
	      switch (arg2.u8[0]) {
		case SC_F2:
		  mode=MD_CONNECT;
		  tmpe1=NULL; terminal1=-1;
		  tmpe2=NULL; terminal2=-1;
		  RubberReset;
		  break;
		case SC_F3:
		  mode=MD_MOVE;
		  tmpe1=NULL; terminal1=-1;
		  tmpe2=NULL; terminal2=-1;
		  RubberReset;
		  break;
		case SC_DELETE:
		  tmpe1=ElementFind(ms.pos.x-Fs->pix_left-Fs->scroll_x,
			ms.pos.y-Fs->pix_top -Fs->scroll_y,&terminal1);
		  ElementDel(tmpe1,Fs);
		  tmpe1=NULL; terminal1=-1;
		  tmpe2=NULL; terminal2=-1;
		  RubberReset;
		  break;
	      }
	      break;
	    case '\n':
	      CleanUp;
	      Init;
	      break;
	    case CH_ESC:
	    case CH_SHIFT_ESC:
	      goto cs_done;
	    case 'r':
	      tmpe1=ElementNew(ET_R);
	      terminal1=-1;
	      break;
	    case 'l':
	      tmpe1=ElementNew(ET_L);
	      terminal1=-1;
	      break;
	    case 'c':
	      tmpe1=ElementNew(ET_C);
	      terminal1=-1;
	      break;
	    case CH_SPACE:
	      if (tmpe1) {
		tmpe1->theta=(tmpe1->theta+1)&3;
		ElementExtents(tmpe1);
		ElementReroute(tmpe1,Fs);
	      }
	      break;
	  }
	  break;
	case MSG_MS_MOVE:
	  if (tmpe1) {
	    ElementExtents(tmpe1);
	    ElementReroute(tmpe1,Fs);
	  }
	  break;
	case MSG_MS_L_DOWN:
	  tmpe1=ElementFind(arg1,arg2,&terminal1);
	  rubber_band=tmpe1; rubber_band_terminal=terminal1;
	  rubber_band_x=ms.pos.x; rubber_band_y=ms.pos.y;
	  break;
	case MSG_MS_L_UP:
	  rubber_band=NULL;
	  switch (mode) {
	    case MD_CONNECT:
	      if (tmpe1 && terminal1>=0) {
		if ((tmpe2=ElementFind(arg1,arg2,&terminal2)) &&
		      (tmpe2!=tmpe1 || terminal2!=terminal1)) {
		  if (!(tmpn=tmpe1->nodes[terminal1]) &&
			!(tmpn=tmpe2->nodes[terminal2]))
		    tmpn=NodeNew;

		  NodeAdd(tmpn,tmpe1,terminal1);
		  NodeAdd(tmpn,tmpe2,terminal2);

		  if (tmpe1->nodes[terminal1])
		    NodeCat(tmpn,tmpe1->nodes[terminal1]);
		  tmpe1->nodes[terminal1]=tmpn;

		  if (tmpe2->nodes[terminal2])
		    NodeCat(tmpn,tmpe2->nodes[terminal2]);
		  tmpe2->nodes[terminal2]=tmpn;

		  NodeReroute(tmpn,Fs);
		}
	      }
	      break;
	    case MD_MOVE:
	      if (tmpe1 && terminal1>=0) {
		tmpe1->x+=ms.pos.x-rubber_band_x;
		tmpe1->y+=ms.pos.y-rubber_band_y;
		ElementExtents(tmpe1);
		ElementReroute(tmpe1,Fs);
	      }
	      break;
	  }
	  tmpe1=NULL; terminal1=-1;
	  tmpe2=NULL; terminal2=-1;
	  RubberReset;
	  break;
	case MSG_MS_R_DOWN:
	  if (mode+1==MD_MODES_NUM)
	    mode=0;
	  else
	    mode++;
	  tmpe1=NULL; terminal1=-1;
	  tmpe2=NULL; terminal2=-1;
	  RubberReset;
	  break;
      }
    }
cs_done:
  } catch
    PutExcept;
  SettingsPop;
  CleanUp;
  MenuPop;
}

Circuit;
