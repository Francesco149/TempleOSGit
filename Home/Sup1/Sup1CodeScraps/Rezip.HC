extern U8 *FAT32FileRead(CDrv *dv,U8 *cur_dir,U8 *filename,
	I64 *_size,I64 *_attr);
extern I64 FAT32FileWrite(CDrv *dv,U8 *cur_dir,U8 *name,U8 *buf,I64 size,
	CDate cdt,I64 attr);

#define CT_NONE 	0
#define CT_7_BIT	1
#define CT_8_BIT	2

public class CArcCompress
{
  I64	compressed_size,expanded_size;
  U8	compression_type,pad[3];
  U0	body;
};

U8 *ExpandBuf(CArcCompress *arc,CTask *mem_task=NULL)
{//See [C:/Demo/Dsk/SerializeTree.HC.Z,1].
  CArcCtrl *c;
  U8 *res=MAlloc(arc->expanded_size+1,mem_task);
  res[arc->expanded_size]=0; //terminate
  if (arc->compression_type==CT_NONE)
    MemCpy(res,&arc->body,arc->expanded_size);
  else {
    c=ArcCtrlNew(TRUE,arc->compression_type+1);
    c->src_size=arc->compressed_size<<3;
    c->src_pos=sizeof(CArcCompress)<<3;
    c->src_buf=arc;
    c->dst_size=arc->expanded_size;
    c->dst_buf=res;
    c->dst_pos=0;
    ArcExpandBuf(c);
    ArcCtrlDel(c);
  }
  return res;
}

U8 *FileRead(U8 *filename,I64 *_size=NULL,I64 *_attr=NULL)
{//Read whole file from disk.
  U8 *absname,*curname,*res=NULL;
  I64 size=0,attr=0;
  CDirContext *dirc;
  CArcCompress *arc;
  absname=FileNameAbs(filename);
  curname=absname;
  if (dirc=DirContextNew(curname)) {
    switch (dirc->dv->fs_type) {
      case FSt_FAT32:
	res=FAT32FileRead(dirc->dv,Fs->cur_dir,dirc->mask,&size,&attr);
	break;
      default:
	PrintErr("File System Not Supported\n");
    }
    DirContextDel(dirc);
  }
  if (!res)
    PrintErr("File not found: \"%s\".\n",filename);
  if (res && attr & RS_ATTR_COMPRESSED) {
    arc=res;
    size=arc->expanded_size;
    res=ExpandBuf(arc);
    Free(arc);
  }
  if (_attr) *_attr=attr;
  if (_size) *_size=size;
  Free(absname);
  return res;
}

#define CT_NONE 	1
#define CT_7_BIT	2
#define CT_8_BIT	3

public class CArcCompress
{
  I64	compressed_size,expanded_size;
  U8	compression_type;
  U0	body;
};

I64 ArcDetermineCompressionType(U8 *src,I64 size)
{
  while (size--)
    if (*src++&0x80)
      return CT_8_BIT;
  return CT_7_BIT;
}

CArcCompress *CompressBuf(U8 *src,I64 size,CTask *mem_task=NULL)
{//See [C:/Demo/Dsk/SerializeTree.HC.Z,1].
  CArcCompress *arc;
  I64 size_out,compression_type=ArcDetermineCompressionType(src,size);
  CArcCtrl *c=ArcCtrlNew(FALSE,compression_type);
  c->src_size=size;
  c->src_buf=src;
  c->dst_size=(size+sizeof(CArcCompress))<<3;
  c->dst_buf=CAlloc(c->dst_size>>3);
  c->dst_pos=sizeof(CArcCompress)<<3;
  ArcCompressBuf(c);
  if (ArcFinishCompression(c) && c->src_pos==c->src_size) {
    size_out=(c->dst_pos+7)>>3;
    arc=MAlloc(size_out,mem_task);
    MemCpy(arc,c->dst_buf,size_out);
    arc->compression_type=compression_type;
    arc->compressed_size=size_out;
  } else {
    arc=MAlloc(size+sizeof(CArcCompress),mem_task);
    MemCpy(&arc->body,src,size);
    arc->compression_type=CT_NONE;
    arc->compressed_size=size+sizeof(CArcCompress);
  }
  arc->expanded_size=size;
  Free(c->dst_buf);
  ArcCtrlDel(c);
  return arc;
}

I64 FileWrite(U8 *filename,U8 *fbuf,
   I64 size,CDate cdt=0,I64 attr=0)
{//Write whole file to disk.
  I64 c=0;
  CHashGeneric *tmph;
  CDirContext *dirc;
  U8 *fbuf2,*absname=FileNameAbs(filename);
  if (dirc=DirContextNew(filename,FALSE,TRUE)) {
    attr=FileAttr(dirc->mask,attr);
    if (attr&RS_ATTR_COMPRESSED) {
      fbuf=CompressBuf(fbuf,size);
      size=fbuf(CArcCompress *)->compressed_size;
      fbuf2=fbuf;
    } else
      fbuf2=NULL;
    if (!cdt) cdt=Now;
    switch (dirc->dv->fs_type) {
      case FSt_FAT32:
	c=FAT32FileWrite(dirc->dv,Fs->cur_dir,dirc->mask,
	      fbuf,size,cdt,attr);
	break;
      default:
	PrintErr("File System Not Supported\n");
    }
    if (tmph=HashFind(absname,adam_task->hash_table,HTT_FILE)) {
      if (attr & RS_ATTR_RESIDENT) {
	Free(tmph->user_data0);
	tmph->user_data0=AMAllocIdent(fbuf);
	tmph->user_data1=size;
      } else
	HashRemDel(tmph,adam_task->hash_table);
    } else if (attr & RS_ATTR_RESIDENT)
      HashGenericAdd(absname,HTT_FILE,AMAllocIdent(fbuf),size,0,adam_task);
    Free(fbuf2);
    DirContextDel(dirc);
  }
  Free(absname);
  return c;
}

Bool CopySingleZ(U8 *f1,U8 *f2) //Just one file
{
  U8 *file_buf=NULL;
  I64 size,attr=0,c;
  CDirEntry de;
  if (FileFind(f1,&de,FUF_JUST_FILES)) {
    Free(de.full_name);
    file_buf=FileRead(f1,&size,&attr);
    attr=FileAttr(f2,attr);
    if (file_buf) {
      "Copying %s to %s\n",f1,f2;
      c=FileWrite(f2,file_buf,size,de.datetime,attr);
      Free(file_buf);
      return ToBool(c);
    } else
      PrintErr("File not found: \"%s\".\n",f1);
  } else
    PrintErr("File not found: \"%s\".\n",f1);
  return FALSE;
}

public I64 Rezip(U8 *files_find_mask="*.Z",U8 *fu_flags=NULL)
{
  U8 *st;
  CDirEntry *tmpde,*tmpde1;
  I64 res=0,fuf_flags=0;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+f+F+O");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tmpde=tmpde1=FilesFind(files_find_mask,fuf_flags);
  while (tmpde) {
    if (IsDotZ(tmpde->full_name)) {
      st=StrNew(tmpde->full_name);
      StrLastRem(st,".");
      res+=CopySingleZ(tmpde->full_name,st);
      CopySingleZ(st,tmpde->full_name);
      Del(st);
      Free(st);
    }
    tmpde=tmpde->next;
  }
  DirTreeDel(tmpde1);
  return res;
}
