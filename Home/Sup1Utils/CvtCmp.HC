Cd(__DIR__);;
#include "FileRaw"

class CArcCompress1
{
  U32	compressed_size,expanded_size;
  U32	compression_type,xsum,pad[4];
  U0	body;
};

public U0 CvtCmp1(U8 *in_name,U8 *out_name)
{//Compression V1.0 --> V3.0
  I64 size;
  CArcCompress1 *in_buf=FileReadRaw(in_name,&size);
  CArcCompress	*out_buf=MAlloc(size);
  out_buf->compressed_size=in_buf->compressed_size+
	sizeof(CArcCompress)-sizeof(CArcCompress1);
  out_buf->expanded_size=in_buf->expanded_size;
  out_buf->compression_type=in_buf->compression_type+1;
  MemCpy(&out_buf->body,&in_buf->body,
	out_buf->compressed_size-sizeof(CArcCompress));
  FileWriteRaw(out_name,out_buf,out_buf->compressed_size);
  Free(in_buf);
  Free(out_buf);
}

public U0 CvtCmp2(U8 *in_name,U8 *out_name)
{//Compression V2.0 --> V3.0
  I64 size;
  U8 *in_buf=FileReadRaw(in_name,&size),*out_buf=MAlloc(size-3);
  MemCpy(out_buf,in_buf,sizeof(CArcCompress));
  out_buf(CArcCompress *)->compressed_size-=3;
  out_buf(CArcCompress *)->compression_type++;
  MemCpy(&out_buf(CArcCompress *)->body,
	(&in_buf(CArcCompress *)->body)(U8 *)+3,
	size-sizeof(CArcCompress)-3);
  FileWriteRaw(out_name,out_buf,size-3);
  Free(in_buf);
  Free(out_buf);
}

public U0 UnzipCmp150(U8 *files_find_mask="*z",U8 *fu_flags=NULL)
{
  U8 name[STR_LEN],ext[STR_LEN];
  CDirEntry *tmpde,*tmpde1;
  I64 i,fuf_flags=0;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+f+F+O");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tmpde=tmpde1=FilesFind(files_find_mask,fuf_flags);
  while (tmpde) {
    "%s\n",tmpde->full_name;
    StrCpy(name,tmpde->full_name);
    StrLastRem(name,".",ext);
    i=LstMatch(ext,"asz\0cpz\0hpz\0dtz\0biz\0mpz\0muz\0txz\0auz\0glz\0");
    if (i>=0) {
      CatPrint(name,".%s.Z",
	    LstSub(i,"ASM\0CPP\0HPP\0DAT\0BIN\0MAP\0TXT\0TXT\0AUT\0TXT\0"));
      "$BLACK$%s$FG$\n",name;
      if (FileFind(name))
	"$RED$Collision:%s$FG$\n",name;
      else {
	CvtCmp1(tmpde->full_name,name);
	Del(tmpde->full_name);
      }
    } else {
      i=LstMatch(ext,"bic\0");
      if (i>=0) {
	CatPrint(name,".%s",LstSub(i,"BIN\0"));
	"$BLACK$%s$FG$\n",name;
	if (FileFind(name))
	  "$RED$Collision:%s$FG$\n",name;
	else {
	  Move(tmpde->full_name,name);
	  Del(tmpde->full_name);
	}
      } else
	"$RED$%s$FG$\n",tmpde->full_name;
    }
    tmpde=tmpde->next;
  }
  DirTreeDel(tmpde1);
}

public U0 UnzipCmp250(U8 *files_find_mask="*z",U8 *fu_flags=NULL)
{
  U8 name[STR_LEN],ext[STR_LEN];
  CDirEntry *tmpde,*tmpde1;
  I64 i,fuf_flags=0;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+f+F+O");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tmpde=tmpde1=FilesFind(files_find_mask,fuf_flags);
  while (tmpde) {
    "%s\n",tmpde->full_name;
    StrCpy(name,tmpde->full_name);
    StrLastRem(name,".",ext);
    i=LstMatch(ext,"asz\0cpz\0hpz\0dtz\0biz\0mpz\0muz\0txz\0"
	  "auz\0glz\0apz\0spz\0");
    if (i>=0) {
      CatPrint(name,".%s.Z",
	    LstSub(i,"ASM\0CPP\0HPP\0DAT\0BIN\0MAP\0TXT\0TXT\0"
	    "IN\0TXT\0CPP\0CPP\0"));
      "$BLACK$%s$FG$\n",name;
      if (FileFind(name))
	"$RED$Collision:%s$FG$\n",name;
      else {
	CvtCmp2(tmpde->full_name,name);
	Del(tmpde->full_name);
      }
    } else {
      i=LstMatch(ext,"bic\0dat\0");
      if (i>=0) {
	CatPrint(name,".%s",LstSub(i,"BIN\0DAT\0"));
	"$BLACK$%s$FG$\n",name;
	if (FileFind(name))
	  "$RED$Collision:%s$FG$\n",name;
	else {
	  Move(tmpde->full_name,name);
	  Del(tmpde->full_name);
	}
      } else
	"$RED$%s$FG$\n",tmpde->full_name;
    }
    tmpde=tmpde->next;
  }
  DirTreeDel(tmpde1);
}

Bool IsDotz(U8 *filename)
{//Does name end in .Z?
  I64 i=StrLen(filename);
  if (i>6 && filename[i-1]=='z' && filename[i-2]=='.' && filename[i-6]=='.')
    return TRUE;
  else
    return FALSE;
}

public U0 UnzipCmp270(U8 *files_find_mask="*.Z",U8 *fu_flags=NULL)
{
  U8 buf[STR_LEN],name[STR_LEN];
  CDirEntry *tmpde,*tmpde1;
  I64 fuf_flags=0;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+f+F+O");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  tmpde=tmpde1=FilesFind(files_find_mask,fuf_flags);
  while (tmpde) {
    StrCpy(buf,tmpde->full_name);
    StrLastRem(buf,"/",name);
    StrUtil(name,SUF_TO_UPPER);
    CatPrint(buf,"/%s",name);
    "%s\n",tmpde->full_name;
    if (FileFind(buf))
      "$RED$Collision:%s$FG$\n",buf;
    else if (IsDotz(tmpde->full_name)) {
      "$BLACK$%s$FG$\n",buf;
      CvtCmp2(tmpde->full_name,buf);
      Del(tmpde->full_name);
    } else {
      "$BLACK$%s$FG$\n",buf;
      Move(tmpde->full_name,buf);
    }
    tmpde=tmpde->next;
  }
  DirTreeDel(tmpde1);
}
