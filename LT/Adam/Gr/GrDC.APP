#help_index "Graphics/DCs"

public U0 GrDCRst(GrDC *dc)
{
  dc->color=BLACK;
  dc->color2=BLACK;
  dc->bkcolor=BLACK;
  dc->collision_cnt=0;
  dc->pen_width=1;
  dc->ls.x=37837; //1<<16/Sqrt(3)
  dc->ls.y=37837;
  dc->ls.z=37837;
  dc->x=0;
  dc->y=0;
  dc->z=0;
  dc->transform=&GrTransform;
  dc->lighting =&GrLighting;
  dc->speedline_scale=0.04;
  GrIdentEqu(dc->r);
  dc->r_norm=gr_scale;
  dc->flags&=~(DCF_SYMMETRY|DCF_TRANSFORMATION|DCF_JUST_MIRROR);
}

public GrDC *GrDCAlias(GrDC *dc=NULL,TaskStruct *task=NULL)
{
  GrDC *result;
  if (!dc)   dc=gr_dc2;
  if (!task) task=Fs;
  if (dc->dc_signature!=DCS_SIGNATURE_VAL)
    Dbg;
  result=MAlloc(sizeof(GrDC),task);
  MemCpy(result,dc,sizeof(GrDC));
  result->win_task=task;
  result->mem_task=task;
  result->r=MAlloc(16*sizeof(I64),task);
  GrDCRst(result);
  result->flags|=DCF_ALIAS;
  result->alias=dc;
  return result;
}


public GrDC *GrDCNew(U64 type,U64 width,U64 height,TaskStruct *task=NULL,BoolI8 null_bitmap=FALSE)
{  //This internally only allows widths which are divisible by 8
//Don't forget these sizeof(GrDC)
`Grep("sizeof(GrDC)","/LT/"TEXT_FILE_MASK);View;`
.
  GrDC *result;
  if (!task) task=Fs;
  result=CAlloc(sizeof(GrDC),task);
  result->win_task=task;
  result->mem_task=task;
  result->type=type;
  result->width=width;
  result->width_internal=(width+7)&~7;
  result->height=height;
  switch (type) {
    case DCT_COLOR4:
      result->plane_size=(result->width_internal*result->height)>>3;
      if (!null_bitmap)
	result->body=CAlloc(result->plane_size<<2,task);
      break;
    case DCT_COLOR8:
      result->plane_size=result->width_internal*result->height;
      if (!null_bitmap)
	result->body=CAlloc(result->plane_size,task);
      break;
    default:
      throw(EXCEPT_GRAPHICS,1);
  }
  if (null_bitmap)
    result->flags|=DCF_DONT_DRAW;
  result->r=MAlloc(16*sizeof(I64),task);
  GrDCRst(result);
  result->dc_signature=DCS_SIGNATURE_VAL;
  return result;
}

public U0 GrDCDel(GrDC *dc)
{
  if (!dc) return;
  if (dc->dc_signature!=DCS_SIGNATURE_VAL)
    Dbg;
  dc->dc_signature=0;
  Free(dc->r);
  if (!(dc->flags & DCF_ALIAS))
    Free(dc->body);
  Free(dc->depth_buf);
  Free(dc);
}

public I64 GrDCSize(GrDC *b)
{
  if (b)
    return MSize2(b)+MSize2(b->body)+MSize2(b->depth_buf);
  else
    return 0;
}

public U0 GrDCDepthBufRst(GrDC *dc)
{
  if (dc->depth_buf)
    MemSetU32(dc->depth_buf,MAX_I32,dc->width_internal*dc->height);
}

public U0 GrDCAllocDepthBuf(GrDC *dc)
{
  Free(dc->depth_buf);
  dc->depth_buf=MAlloc(dc->width_internal*dc->height*sizeof(I32),dc->mem_task);
  GrDCDepthBufRst(dc);
}

public GrDC *GrDCCopy(GrDC *dc,TaskStruct *task=NULL)
{
  GrDC *result;
  if (!dc) return NULL;
  if (dc->dc_signature!=DCS_SIGNATURE_VAL)
    Dbg;
  result=MAllocIdentical(dc,task);
  GrSetRotMat(result,MAllocIdentical(dc->r,task));
  result->mem_task=task;
  result->body=MAllocIdentical(dc->body,task);
  return result;
}

public U0 GrDCMono(GrDC *dc,
  I64 quest=COLOR_TRANSPARENT,I64 true_color=0,I64 false_color=COLOR_MONO)
{
  I64 i;
  U8 *dst;
  if (dc->type==DCT_COLOR8) {
    dst=dc->body;
    i=dc->width_internal*dc->height;
    while (i--)
      if (*dst==quest)
	*dst++=true_color;
      else
	*dst++=false_color;
  }
}

public U0 GrDCColorChg(GrDC *dc,I64 src_color,I64 dst_color=COLOR_TRANSPARENT)
{
  I64 i;
  U8 *dst;
  if (dc->type==DCT_COLOR8) {
    dst=dc->body;
    i=dc->width_internal*dc->height;
    while (i--)
      if (*dst==src_color)
	*dst++=dst_color;
      else
	dst++;
  }
}

public U8 *GrDCSave(GrDC *b,U64 *size=NULL,BoolI8 compressed=TRUE)
{ //stores to mem
  U8 *ptr,*body,*result;
  BoolI8 old_preempt;
  ArcCompressStruct *ac;
  I64 i,j,old_flags=b->flags;

  if (b->type==DCT_COLOR8)
    i=b->plane_size;
  else
    throw(EXCEPT_GRAPHICS,2);
  if (compressed) {
    ac=CompressBuf(b->body,i);
    j=ac->compressed_size;
    body=ac;
  } else {
    ac=NULL;
    j=i;
    body=b->body;
  }

  ptr=result=MAlloc(offset(GrDC.end_saved_area)-
      offset(GrDC.start_saved_area)+j);

  old_preempt=Preempt(OFF); //in case it is a screen bitmap
  if (compressed) b->flags|=DCF_COMPRESSED;
  b->flags&=DCF_COMPRESSED;
  MemCpy(result,&b->start_saved_area,
      offset(GrDC.end_saved_area)-
      offset(GrDC.start_saved_area));
  b->flags=old_flags;
  Preempt(old_preempt);

  ptr+=offset(GrDC.end_saved_area)-
      offset(GrDC.start_saved_area);
  MemCpy(ptr,body,j);
  ptr+=j;

  Free(ac);
  if (size)
    *size=ptr-result;
  return result;
}

public GrDC *GrDCLoad(U8 *src,U64 *size=NULL,TaskStruct *task=NULL)
{ //loads from mem
  I64 i;
  GrDC *result;
  U8 *ptr=src;
  ArcCompressStruct *ac;
  if (!task) task=Fs;
  result=CAlloc(sizeof(GrDC),task);
  result->win_task=task;
  result->mem_task=task;
  MemCpy(&result->start_saved_area,src,
      offset(GrDC.end_saved_area)-
      offset(GrDC.start_saved_area));
  ptr+=offset(GrDC.end_saved_area)-
      offset(GrDC.start_saved_area);
  if (result->type==DCT_COLOR8)
    i=result->plane_size;
  else
    throw(EXCEPT_GRAPHICS,3);
  if (result->flags&DCF_COMPRESSED) {
    result->flags&=~DCF_COMPRESSED;
    ac=ptr;
    result->body=ExpandBuf(ac,task);
    ptr+=ac->compressed_size;
  } else {
    result->body=MAlloc(i,task);
    MemCpy(result->body,ptr,i);
  }

  result->pen_width=1;
  result->r=GrIdent(task);
  result->r_norm.u32[1]=1;
  result->speedline_scale=0.04;
  result->dc_signature=DCS_SIGNATURE_VAL;
  if (size)
    *size=ptr-src;
  return result;
}

public U64 LGRWrite(U8 *filename,GrDC *dc)
{ // LoseThos LGR File
  U64 size;
  U8 *st=DftExt(filename,"LGZ"),
     *src=GrDCSave(dc,&size);
  FileWrite(st,src,size);
  Free(st);
  Free(src);
  return size;
}

public GrDC *LGRRead(U8 *filename,TaskStruct *task=NULL)
{ // LoseThos LGR File
  GrDC *dc=NULL;
  U8 *st=DftExt(filename,"LGZ"),
     *src=FileRead(st);
  if (src)
    dc=GrDCLoad(src,NULL,task);
  Free(src);
  Free(st);
  return dc;
}

#help_index "Graphics/DCs;Graphics/Screen"

public GrDC *GrDCCaptureScreen(TaskStruct *task=NULL)
{
  GrDC *dc;
  U8 *dst;
  WinMgrSync(2);
  dc=GrDCCopy(gr_screen_image,task);
  if (dc->type==DCT_COLOR4) {
    dc->type=DCT_COLOR8;
    dc->plane_size*=8;
    dst=MAlloc(dc->plane_size,task);
    GrBitMap4ToBitMap8(dst,dc->body,dc->plane_size/2,COLOR_INVALID);
    Free(dc->body);
    dc->body=dst;
  }
  return dc;
}

public U64 LGRCaptureScreen(U8 *filename)
{ // LoseThos LGR File
  U64 size;
  GrDC *dc=GrDCCaptureScreen;
  size=LGRWrite(filename,dc);
  GrDCDel(dc);
  return size;
}

#help_index "Graphics;Graphics/DCs"

public U0 GrDCExtentsInit(GrDC *dc)
{//See [::/LT/Demo/Graphics/Extents.CPZ] ::/LT/Demo/Graphics/Extents.CPZ
//You should clear the record flag yourself
  dc->flags|=DCF_RECORD_EXTENTS;
  dc->min_x=MAX_I64;
  dc->max_x=MIN_I64;
  dc->min_y=MAX_I64;
  dc->max_y=MIN_I64;
}
