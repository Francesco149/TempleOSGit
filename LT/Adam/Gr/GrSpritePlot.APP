#help_index "Graphics/GrSprite"
/*
[C:/LT/Adam/Gr/Gr.APZ,55] GrSprites are stored as a sequence of var
length operations with a 1-byte type leading
each operation.  The types available are listed
here, [C:/LT/Adam/Gr/Gr.APZ,6] GRST_POINT.  They are stored, one after another,
in a chunk of memory terminated by a type [C:/LT/Adam/Gr/Gr.APZ,2] GRST_END.

[C:/LT/Adam/Gr/GrSpritePlot.APZ,27] GrSpritePlot3() and [C:/LT/Adam/Gr/GrSpriteVect.APZ,221] GrSpriteVect() shows how the
unions are used.  It's kind-of a mess using a union.

[C:/LT/Adam/Gr/GrSpriteNew.APZ,122] GrSpriteSize() will return the size of a single
operation, while [C:/LT/Adam/Gr/GrSpriteNew.APZ,122] GrSpriteSize() with an 's' will
return the size of an entire list.  Look at
[C:/LT/Adam/Gr/GrSpriteNew.APZ,5] gr_sprite_base_size.

To get familiar, type <CTRL-R> on picture in
a document and select "Display as Text".  It
will show the raw data for each GrSprite.  This
is how they are stored in files, as well.

See ::/LT/Apps/GrModels
`Cd("::/LT/Apps/GrModels");Dir;View;
`
 for an example of
making GrSprite by hand.  Those are type
[C:/LT/Adam/Gr/Gr.APZ,31] GRST_MESH, one of the most complicated.
*/

public U0 GrSpritePlot3(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems,BoolI8 just_one=FALSE)
{
  GrSprite *tempg=elems-offset(GrSprite.start);
  I64 i,j,k;
  I64 x1,y1,z1,x2,y2;
  I32 *ptr;
  I64 cur_sprite_elem_num=0,*old_r,*r2;
  U64 old_flags=dc->flags;
  ColorROPU32 old_color=dc->color;
  I64 old_pen_width=dc->pen_width;
  GrDC *img;
  P3I32 *p;
  GrSymStruct old_sym;
  MemCpy(&old_sym,&dc->sym,sizeof(GrSymStruct));
  while (tempg->type) {
    dc->cur_sprite_elem_num=cur_sprite_elem_num++;
    switch (tempg->type) {
      case GRST_COLOR8:
	dc->color=dc->color&~(COLORROP_COLORS_MASK|ROPF_DITHER)|tempg->color8;
	break;
      case GRST_DITHER_COLOR8:
	dc->color=dc->color&~COLORROP_COLORS_MASK|
		  tempg->dither_color8.u8[0]|
		  tempg->dither_color8.u8[1]<<COLORROP_BITS|ROPF_DITHER;
	break;
      case GRST_WIDTH:
	dc->pen_width=tempg->width;
	GrScalePenWidth(dc);
	break;
      case GRST_TRANSFORM_ON:
	if (!(dc->flags&DCF_TRANSFORMATION)) {
	  x-=dc->x;
	  y-=dc->y;
	  z-=dc->z;
	}
	dc->flags|=DCF_TRANSFORMATION;
	break;
      case GRST_TRANSFORM_OFF:
	if (dc->flags&DCF_TRANSFORMATION) {
	  x+=dc->x;
	  y+=dc->y;
	  z+=dc->z;
	}
	dc->flags&=~DCF_TRANSFORMATION;
	break;
      case GRST_POINT:
	GrPlot3(dc,tempg->x1+x,tempg->y1+y,z);
	break;
      case GRST_TEXT:
	GrPutS3(dc,tempg->x1+x,tempg->y1+y,z,tempg->st);
	break;
      case GRST_TEXT_BOX:
	GrTextBox3(dc,tempg->x1+x,tempg->y1+y,z,tempg->st);
	break;
      case GRST_TEXT_DIAMOND:
	GrTextDiamond3(dc,tempg->x1+x,tempg->y1+y,z,tempg->st);
	break;
      case GRST_FLOOD_FILL:
	GrFloodFill3(dc,tempg->x1+x,tempg->y1+y,z,FALSE);
	break;
      case GRST_FLOOD_FILL_NOT:
	i=dc->color;
	dc->color=dc->color.c0;
	GrFloodFill3(dc,tempg->x1+x,tempg->y1+y,z,TRUE);
	dc->color=i;
	break;
      case GRST_SHIFT:
	x+=tempg->x1;
	y+=tempg->y1;
	break;
      case GRST_LINE:
	GrLine3(dc,tempg->x1+x,tempg->y1+y,z,tempg->x2+x,tempg->y2+y,z);
	break;
      case GRST_ARROW:
	GrArrow3(dc,tempg->x1+x,tempg->y1+y,z,tempg->x2+x,tempg->y2+y,z);
	break;
      case GRST_PLANAR_SYMMETRY:
	if (GrSetSymmetry3(dc,tempg->x1+x,tempg->y1+y,z,tempg->x2+x,tempg->y2+y,z,tempg->x2+x,tempg->y2+y,z+1))
	  dc->flags|=DCF_SYMMETRY;
	else
	  dc->flags&=~DCF_SYMMETRY;
	break;
      case GRST_BITMAP8:
	img=CAlloc(sizeof(GrDC));
	img->type=DCT_COLOR8;
	img->width=tempg->width;
	img->width_internal=(tempg->width+7)&~7;
	img->height=tempg->height;
	img->plane_size=img->width_internal*img->height;
	img->body=tempg(U8 *)+offset(GrSprite.linespeed);
	img->dc_signature=DCS_SIGNATURE_VAL;
	GrDCBlot3(dc,tempg->x2+x,tempg->y2+y,z,img);
	Free(img);
	break;
      case GRST_RECT:
	GrRect3(dc,tempg->x1+x,tempg->y1+y,z,tempg->x2-tempg->x1,tempg->y2-tempg->y1);
	break;
      case GRST_ROTATED_RECT:
	x1=tempg->x1+x;
	y1=tempg->y1+y;
	z1=z;
	GrRot(dc->r,&x1,&y1,&z1);
	old_r=dc->r;
	dc->flags|=DCF_TRANSFORMATION;
	r2=GrRotZ(-tempg->rot_angle);
	GrSetTranslation(r2,x1,y1,z1);
	GrSetRotMat(dc,GrMulMat(old_r,r2));
	GrRect3(dc,0,0,0,tempg->x2-tempg->x1,tempg->y2-tempg->y1);
	Free(dc->r);
	Free(r2);
	GrSetRotMat(dc,old_r);
	dc->flags=dc->flags&~DCF_TRANSFORMATION|old_flags;
	break;
      case GRST_CIRCLE:
	GrCircle3(dc,tempg->x1+x,tempg->y1+y,z,tempg->x2);
	break;
      case GRST_ELLIPSE:
	GrEllipse3(dc,tempg->x2+x,tempg->y2+y,z,tempg->width,tempg->height,tempg->rot_angle);
	break;
      case GRST_REG_POLY:
	GrRegPoly3(dc,tempg->x2+x,tempg->y2+y,z,tempg->width,tempg->height,tempg->sides,tempg->rot_angle);
	break;
      case GRST_POLYLINE:
	ptr=&tempg->y1;
	x1=ptr[0];
	y1=ptr[1];
	for (i=1;i<tempg->num;i++) {
	  x2=ptr[i<<1];
	  y2=ptr[i<<1+1];
	  GrLine3(dc,x1+x,y1+y,z,x2+x,y2+y,z);
	  x1=x2;y1=y2;
	}
	break;
      case GRST_SPEEDLINE:
	ptr=&tempg->y1;
	x1=ptr[0];
	y1=ptr[1];
	for (i=1;i<tempg->num;i++) {
	  x2=ptr[i<<2];
	  y2=ptr[i<<2+1];
	  GrSpeedLine3(dc,x1+x,y1+y,z,x2+x,y2+y,z,ptr[i<<2+2](F64));
	  x1=x2;y1=y2;
	}
	break;
      case GRST_POLYPOINT:
	ptr=&tempg->y1;
	x1=ptr[0];
	y1=ptr[1];
	ptr+=2;
	k=tempg->num*3;
	GrPlot3(dc,x1+x,y1+y,z);
	for (i=0;i<k;i+=3) {
	  j=BFieldExtU32(ptr,i,3);
	  x1+=gr_x_offsets[j];
	  y1+=gr_y_offsets[j];
	  GrPlot3(dc,x1+x,y1+y,z);
	}
	break;
      case GRST_BSPLINE2:
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x+=x;
	  p->y+=y;
	  p->z+=z;
	}
	Gr2BSpline3(dc,&tempg->y1,tempg->num,FALSE);
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x-=x;
	  p->y-=y;
	  p->z-=z;
	}
	break;
      case GRST_BSPLINE3:
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x+=x;
	  p->y+=y;
	  p->z+=z;
	}
	Gr3BSpline3(dc,&tempg->y1,tempg->num,FALSE);
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x-=x;
	  p->y-=y;
	  p->z-=z;
	}
	break;
      case GRST_BSPLINE2_CLOSED:
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x+=x;
	  p->y+=y;
	  p->z+=z;
	}
	Gr2BSpline3(dc,&tempg->y1,tempg->num,TRUE);
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x-=x;
	  p->y-=y;
	  p->z-=z;
	}
	break;
      case GRST_BSPLINE3_CLOSED:
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x+=x;
	  p->y+=y;
	  p->z+=z;
	}
	Gr3BSpline3(dc,&tempg->y1,tempg->num,TRUE);
	p=&tempg->y1;
	for (i=0;i<tempg->num;i++,p++) {
	  p->x-=x;
	  p->y-=y;
	  p->z-=z;
	}
	break;
      case GRST_MESH:
	p=&tempg->x2;
	for (i=0;i<tempg->vertex_cnt;i++,p++) {
	  p->x+=x;
	  p->y+=y;
	  p->z+=z;
	}
	Gr3Mesh(dc,tempg->vertex_cnt,&tempg->x2,tempg->triangle_cnt,(&tempg->x2)(U8 *)+sizeof(P3I32)*tempg->vertex_cnt);
	p=&tempg->x2;
	for (i=0;i<tempg->vertex_cnt;i++,p++) {
	  p->x-=x;
	  p->y-=y;
	  p->z-=z;
	}
	break;
      case GRST_SHIFTABLE_MESH:
	if (dc->flags&DCF_TRANSFORMATION) {
	  dc->x+=tempg->x1;
	  dc->y+=tempg->y1;
	  dc->z+=tempg->z1;
	  x1=x;
	  y1=y;
	  z1=z;
	} else {
	  x1=tempg->x1+x;
	  y1=tempg->y1+y;
	  z1=tempg->z1+z;
	}
	p=(&tempg->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	for (i=0;i<tempg->shiftable_vertex_cnt;i++,p++) {
	  p->x+=x1;
	  p->y+=y1;
	  p->z+=z1;
	}
	p=(&tempg->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	Gr3Mesh(dc,tempg->shiftable_vertex_cnt,p,tempg->shiftable_triangle_cnt,p(U8 *)+sizeof(P3I32)*tempg->shiftable_vertex_cnt);
	p=(&tempg->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	for (i=0;i<tempg->shiftable_vertex_cnt;i++,p++) {
	  p->x-=x1;
	  p->y-=y1;
	  p->z-=z1;
	}
	if (dc->flags&DCF_TRANSFORMATION) {
	  dc->x-=tempg->x1;
	  dc->y-=tempg->y1;
	  dc->z-=tempg->z1;
	}
	break;
    }
    if (just_one) break;
    tempg(U8 *)+=GrSpriteElemSize(tempg);
  }
  MemCpy(&dc->sym,&old_sym,sizeof(GrSymStruct));
  dc->color=old_color;
  dc->pen_width=old_pen_width;
  dc->flags=dc->flags&~(DCF_SYMMETRY|DCF_TRANSFORMATION) |
	    old_flags&(DCF_SYMMETRY|DCF_TRANSFORMATION);
}

public U0 GrSpriteElemPlot3(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems)
//**!!!WARNING!!!** This will plot just
//one element.  You probably do not
//want this routine.
//
//If you do not enter a color or width into
//a GrSprite, you can pass a color and width
//when you plot it.
{
  GrSpritePlot3(dc,x,y,z,elems,TRUE);
}


public U0 GrSpritePlot3b(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems)
{
  I64 old_x=dc->x,old_y=dc->y,old_z=dc->z;
  dc->x=x; dc->y=y; dc->z=z;
  GrSpritePlot3(dc,0,0,0,elems);
  dc->x=old_x; dc->y=old_y; dc->z=old_z;
}

public U0 GrSpritePlotRotMat3b(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems,I64 *m)
{
  I64 *old_r=dc->r,old_m[16];
  U64 old_flags=dc->flags&DCF_TRANSFORMATION;
  MemCpy(old_m,m,16*sizeof(I64));
  dc->flags|=DCF_TRANSFORMATION;
  GrAddTranslation(m,x,y,z);
  dc->r=GrMulMat(old_r,m);
  GrSpritePlot3(dc,0,0,0,elems);
  Free(dc->r);
  dc->r=old_r;
  MemCpy(m,old_m,16*sizeof(I64));
  dc->flags=dc->flags&~DCF_TRANSFORMATION|old_flags;
}

public U0 GrSpritePlotRotX3b(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems,F64 phi=0)
{
  I64 *r=GrRotX(phi);
  GrSpritePlotRotMat3b(dc,x,y,z,elems,r);
  Free(r);
}

public U0 GrSpritePlotRotY3b(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems,F64 omega=0)
{
  I64 *r=GrRotY(omega);
  GrSpritePlotRotMat3b(dc,x,y,z,elems,r);
  Free(r);
}

public U0 GrSpritePlotRotZ3b(GrDC *dc,I64 x,I64 y,I64 z,U8 *elems,F64 theta=0)
{
  I64 *r=GrRotZ(theta);
  GrSpritePlotRotMat3b(dc,x,y,z,elems,r);
  Free(r);
}

public U0 GrSpriteExtents(U8 *elems,I64 *min_x=NULL,I64 *max_x=NULL,
				   I64 *min_y=NULL,I64 *max_y=NULL)
{ //Ignores flood fills.
  GrDC *dc=GrDCNew(DCT_COLOR8,MAX_I32,MAX_I32,Fs,TRUE);
  GrDCExtentsInit(dc);
  GrSpritePlot3(dc,MAX_I32/2,MAX_I32/2,MAX_I32/2,elems);
  if (dc->min_x<=dc->max_x) {
    dc->min_x-=MAX_I32/2;
    dc->max_x-=MAX_I32/2;
  }
  if (dc->min_y<=dc->max_y) {
    dc->min_y-=MAX_I32/2;
    dc->max_y-=MAX_I32/2;
  }
  if (min_x) *min_x=dc->min_x;
  if (max_x) *max_x=dc->max_x;
  if (min_y) *min_y=dc->min_y;
  if (max_y) *max_y=dc->max_y;
  GrDCDel(dc);
}

public GrDC *GrSprite2DC(U8 *elems,I64 type=DCT_COLOR8)
{
  GrDC *result;
  I64 min_x,max_x,min_y,max_y;
  GrSpriteExtents(elems,&min_x,&max_x,&min_y,&max_y);
  result=GrDCNew(type,max_x-min_x+1,max_y-min_y+1);
  GrSpritePlot3(result,-min_x,-min_y,0,elems);
  return result;
}

public U8 *GrSpriteInterpolate(U8 *elems0,U8 *elems1,F64 t)
{
//The two GrSprite should be identical
//except for points shifted around.
//t ranges from 0.0 to 1.0
//
//See [::/LT/Demo/Graphics/Golf.CPZ] ::/LT/Demo/Graphics/Golf.CPZ for an example.
  I64 i=GrSpriteSize(elems0),
      t1=gr_scale*t,t0=gr_scale-t1;
  I32 *ptr0,*ptr1,*ptrr;
  P3I32 *p0,*p1,*pr;
  U8 *result=MAlloc(i);
  GrSprite *tempg0=elems0-offset(GrSprite.start),
      *tempg1=elems1-offset(GrSprite.start),
      *tempgr=result-offset(GrSprite.start);
  MemCpy(result,elems0,i);
  while (tempg0->type) {
    if (tempg0->type!=tempg1->type)
      throw(EXCEPT_GRAPHICS,4);
    switch (tempg0->type) {
      case GRST_ROTATED_RECT:
	tempgr->rot_angle=(tempg0->rot_angle*t0+tempg1->rot_angle*t1)/gr_scale;
      case GRST_RECT:
      case GRST_LINE:
      case GRST_ARROW:
      case GRST_PLANAR_SYMMETRY:
	tempgr->x2=(tempg0->x2*t0+tempg1->x2*t1)>>32;
	tempgr->y2=(tempg0->y2*t0+tempg1->y2*t1)>>32;
      case GRST_TEXT:
      case GRST_TEXT_BOX:
      case GRST_TEXT_DIAMOND:
      case GRST_POINT:
      case GRST_FLOOD_FILL:
      case GRST_FLOOD_FILL_NOT:
      case GRST_SHIFT:
	tempgr->x1=(tempg0->x1*t0+tempg1->x1*t1)>>32;
	tempgr->y1=(tempg0->y1*t0+tempg1->y1*t1)>>32;
	break;
      case GRST_CIRCLE:
	tempgr->x2=(tempg0->x2*t0+tempg1->x2*t1)>>32;
	tempgr->x1=(tempg0->x1*t0+tempg1->x1*t1)>>32;
	tempgr->y1=(tempg0->y1*t0+tempg1->y1*t1)>>32;
	break;
      case GRST_ELLIPSE:
      case GRST_REG_POLY:
	tempgr->x2=(tempg0->x2*t0+tempg1->x2*t1)>>32;
	tempgr->y2=(tempg0->y2*t0+tempg1->y2*t1)>>32;
	tempgr->width=(tempg0->width*t0+tempg1->width*t1)>>32;
	tempgr->height=(tempg0->height*t0+tempg1->height*t1)>>32;
	break;
      case GRST_BITMAP8:
	tempgr->x2=(tempg0->x2*t0+tempg1->x2*t1)>>32;
	tempgr->y2=(tempg0->y2*t0+tempg1->y2*t1)>>32;
	break;
      case GRST_SPEEDLINE:
	ptr0=&tempg0->y1;
	ptr1=&tempg1->y1;
	ptrr=&tempgr->y1;
	for (i=0;i<tempg0->num;i++) {
	  ptrr[i<<2]=(ptr0[i<<2]*t0+ptr1[i<<2]*t1)>>32;
	  ptrr[i<<2+1]=(ptr0[i<<2+1]*t0+ptr1[i<<2+1]*t1)>>32;
	}
	break;
      case GRST_POLYLINE:
	ptr0=&tempg0->y1;
	ptr1=&tempg1->y1;
	ptrr=&tempgr->y1;
	for (i=0;i<tempg0->num;i++) {
	  ptrr[i<<1]=(ptr0[i<<1]*t0+ptr1[i<<1]*t1)>>32;
	  ptrr[i<<1+1]=(ptr0[i<<1+1]*t0+ptr1[i<<1+1]*t1)>>32;
	}
	break;
      case GRST_POLYPOINT:
	ptr0=&tempg0->y1;
	ptr1=&tempg1->y1;
	ptrr=&tempgr->y1;
	ptrr[0]=(ptr0[0]*t0+ptr1[0]*t1)>>32;
	ptrr[1]=(ptr0[1]*t0+ptr1[1]*t1)>>32;
	break;
      case GRST_BSPLINE2:
      case GRST_BSPLINE3:
      case GRST_BSPLINE2_CLOSED:
      case GRST_BSPLINE3_CLOSED:
	p0=&tempg0->y1;
	p1=&tempg1->y1;
	pr=&tempgr->y1;
	for (i=0;i<tempg0->num;i++) {
	  pr[i].x=(p0[i].x*t0+p1[i].x*t1)>>32;
	  pr[i].y=(p0[i].y*t0+p1[i].y*t1)>>32;
	  pr[i].z=(p0[i].z*t0+p1[i].z*t1)>>32;
	}
	break;
      case GRST_MESH:
	p0=&tempg0->x2;
	p1=&tempg1->x2;
	pr=&tempgr->x2;
	for (i=0;i<tempg0->vertex_cnt;i++) {
	  pr[i].x=(p0[i].x*t0+p1[i].x*t1)>>32;
	  pr[i].y=(p0[i].y*t0+p1[i].y*t1)>>32;
	  pr[i].z=(p0[i].z*t0+p1[i].z*t1)>>32;
	}
	break;
      case GRST_SHIFTABLE_MESH:
	p0=(&tempg0->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	p1=(&tempg1->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	pr=(&tempgr->shiftable_triangle_cnt)(U8 *)+sizeof(I32);
	for (i=0;i<tempg0->shiftable_vertex_cnt;i++) {
	  pr[i].x=(p0[i].x*t0+p1[i].x*t1)>>32;
	  pr[i].y=(p0[i].y*t0+p1[i].y*t1)>>32;
	  pr[i].z=(p0[i].z*t0+p1[i].z*t1)>>32;
	}
	break;
    }
    i=GrSpriteElemSize(tempg0);
    tempg0(U8 *)+=i;
    tempg1(U8 *)+=i;
    tempgr(U8 *)+=i;
  }
  return result;
}


#help_index "Graphics/GrSprite;Ltf/Output"
public LtfEntry *LtfPutSprite(Ltf *l,U8 *elems,U8 *txt=NULL)
{
  I64 size;
  U8 *st;
  BoolI8 unlock=LtfLock(l);
  LtfEntry *ll;
  LtfBinEntry *tempb;
  size=GrSpriteSize(elems);
  tempb=CAlloc(sizeof(LtfBinEntry),l->mem_task);
  tempb->type=LTFBT_GRSPRITE;
  tempb->size=size;
  tempb->data=MAlloc(size,l->mem_task);
  MemCpy(tempb->data,elems,size);
  tempb->num=l->cur_bin_num;
  tempb->use_cnt=1;
  InsQue(tempb,l->bin_root.last);
  if (txt)
    st=MSPrintF(txt,l->cur_bin_num++);
  else
    st=MSPrintF("$PI,\"\",%d$",l->cur_bin_num++);
  ll=LtfPutS(l,st);
  Free(st);
  ll->bin_data=tempb;
  if (unlock)
    LtfUnlock(l);
  return ll;
}

public LtfEntry *PutSprite(U8 *elems,U8 *txt=NULL)
{
//If you set txt,
//then include dollarsigns ("$PI ...$")
//and leave a %d for the number.

  Ltf *l;
  if (l=LtfCur)
    return LtfPutSprite(l,elems,txt);
  return NULL;
}
