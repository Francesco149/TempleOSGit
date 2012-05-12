/*
This converts [C:/LT/Adam/Gr/Gr.APZ,55] GrSprite to the
new [C:/LT/OSMain/Adam1a.HPZ,3463] DCT_COLOR8 format.
*/

public class OldGrSprite
{
  GrSprite *next,*last;

  U0 start;
  U8 type;
  union {
    U8 color8;
    U16 dither_color8;
    I32 x1;
    I32 num;
    I32 width;
    I32 vertex_cnt; //Mesh
  }
  union {
    I32 y1;
    I32 height;
    I32 triangle_cnt; //Mesh
  }
  union {
    I32 z1;
    I32 x2;
    U8 st[4];
  }
  union {
    I32 y2;
    I32 shiftable_vertex_cnt //Shiftable Mesh
  }
  union {
    F64 linespeed;
    F64 rot_angle;
    ColorROPU32 bkcolor;
    I32 shiftable_triangle_cnt //Shiftable Mesh
  }
  I32 sides;
};

BoolI64 Cvt803GrSprite(LtfBinEntry *b)
{
  U8 *result=MAlloc(b->size*2),*dst=result;
  GrSprite *tempg=b->data-offset(GrSprite.start);
  I64 s,s1,i,i1,bkcolor;
  while (TRUE) {
    if (tempg->type>=GRST_NUM_TYPES)
      return FALSE;
    if (tempg->type==GRST_BITMAP8||tempg->type==GRST_BITMAP8+1) {
      i =((tempg->width+7)&~7)*tempg->height*4/8;
      s =gr_sprite_base_size[GRST_BITMAP8]+8+i;
      if (s>0x1000000)
	return FALSE;
      i1=((tempg->width+7)&~7)*tempg->height;
      s1=gr_sprite_base_size[GRST_BITMAP8]+i1;

      if (tempg->type==GRST_BITMAP8+1) { //GRST_BITMAP4_TRANSPARENT
	bkcolor=tempg(OldGrSprite *)->bkcolor&0xF;
	tempg->type=GRST_BITMAP8;
      } else
	bkcolor=COLOR_INVALID;

      MemCpy(dst,&tempg->start,gr_sprite_base_size[GRST_BITMAP8]);
      dst+=gr_sprite_base_size[GRST_BITMAP8];
      dst=GrBitMap4ToBitMap8(dst,&tempg->sides,i,bkcolor);
    } else if (tempg->type==GRST_DITHER_COLOR8) {
      s=GrSpriteElemSize(tempg)-1;
      if (s>0x1000000)
	return FALSE;
      tempg->dither_color8=tempg->dither_color8&15+tempg->dither_color8&0xF0<<4;
      MemCpy(dst,&tempg->start,s+1);
      dst+=s+1;
    } else {
      s=GrSpriteElemSize(tempg);
      if (s>0x1000000)
	return FALSE;
      MemCpy(dst,&tempg->start,s);
      dst+=s;
    }
    if (!tempg->type) {
      tempg(U8 *)+=s;
      break;
    }
    tempg(U8 *)+=s;
  }
  if (tempg(U8 *)-(b->data-offset(GrSprite.start))!=b->size)
    return FALSE;
  Free(b->data);
  b->data=result;
  b->size=dst-result;
  return TRUE;
}

BoolI64 Cvt803LtfFile(U8 *name,LTDate datetime=0)
{
  Ltf *l=LtfRead(name);
  LtfBinEntry *b=l->bin_root.next;
  BoolI8 modified=FALSE;
  while (b!=&l->bin_root) {
    if (b->type==LTFBT_GRSPRITE) {
      "%D:%s\n",datetime,name;
      if (b->size>0x1000000||!Cvt803GrSprite(b)) {
	"Error in:%s\n",name;
	return FALSE;
      }
      modified=TRUE;
    }
    b=b->next;
  }
  if (modified) {
    LtfWrite(l);
    Touch(l->filename.name,"","",datetime);
  }
  LtfDel(l);
  return modified;
}

public BoolI64 Cvt803LGRFile(U8 *name)
{
  BoolI8 result=FALSE;
  GrDC *dc=LGRRead(name);
  U8 *dst;
  if (dc&&dc->type==DCT_COLOR4) {
    dc->type=DCT_COLOR8;
    dc->plane_size*=8;
    dst=MAlloc(dc->plane_size);
    GrBitMap4ToBitMap8(dst,dc->body,dc->plane_size/2,COLOR_INVALID);
    Free(dc->body);
    dc->body=dst;
    LGRWrite(name,dc);
    result=TRUE;
  } else if (dc&&dc->type==2) { //DCT_MONO
    dc->type=DCT_COLOR8;
    dc->plane_size*=8;
    dst=MAlloc(dc->plane_size);
    GrBitMap1ToBitMap8(dst,dc->body,dc->plane_size/8,COLOR_INVALID);
    Free(dc->body);
    dc->body=dst;
    LGRWrite(name,dc);
    result=TRUE;
  }
  GrDCDel(dc);
  return result;
}

public U0 Cvt803(U8 *mask)
{
  LTDirEntry *tempm=FilesFind(mask,FUF_RECURSE|FUF_FLATTEN_TREE|FUF_JUST_TEXT_FILES|FUF_JUST_FILES),
	     *tempm1;
  while (tempm) {
    tempm1=tempm->next;
    Cvt803LtfFile(tempm->full_name,tempm->datetime);
    LTDirEntryDel(tempm);
    tempm=tempm1;
  }
}

//Perhaps: Cvt803("HOME/*");
//Perhaps: Cvt803LGRFile("::/LT/Apps/Pilgrims/NewEngland.LGZ");

