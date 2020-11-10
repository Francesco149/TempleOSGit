U8 *note_lst="A\0A#\0B\0C\0C#\0D\0D#\0E\0F\0F#\0G\0G#\0";

I64 NextNote(U8 *st)
{
  while (*st && !('0'<=*st<='9') && !('A'<=*st<='G'))
    st++;
  if ('A'<=*st<='G')
    return music.note_map[*st-'A'];
  else
    return I64_MIN;
}

U8 *CvtSong500Str(I64 *_octave_in,U8 *st)
{
  I64 ona,note,octave,octave_out=I64_MIN;
  U8 *src=st,buf[512],*dst=buf;
  note=NextNote(src);
  if (note>=0) {
    ona=(*_octave_in+2)*12+note;
    octave_out=Ona2Octave(ona);
    *dst++='0'+octave_out;
  }
  while (*src) {
    while (*src && '0'<=*src<='9')
      *_octave_in=*src++-'0';
    if ('A'<=*src<='G') {
      ona=(*_octave_in+2)*12+music.note_map[*src++-'A'];
      octave=Ona2Octave(ona);
      note  =Ona2Note  (ona);
      if (octave!=octave_out) {
	octave_out=octave;
	*dst++='0'+octave;
      }
      *dst++=*LstSub(note,note_lst);
    } else {
      note=NextNote(src);
      if (note>=0) {
	ona=(*_octave_in+2)*12+note;
	octave=Ona2Octave(ona);
	if (octave!=octave_out) {
	  octave_out=octave;
	  *dst++='0'+octave;
	}
      }
      *dst++=*src++;
    }
  }
  *dst=0;
  "  IN :%s\n",st;
  "  OUT:%s\n",buf;
  return StrNew(buf);
}

I64 CvtSong500Doc(CDoc *doc)
{
  Bool unlock=DocLock(doc);
  U8 *st,*st2,*src,*src2,*dst,buf[512];
  I64 res=0,octave_in=3;
  Bool	src_file=FilesFindMatch(doc->filename.name,FILEMASK_SRC);
  CDocEntry *doc_e;

  DocRecalc(doc);
  doc_e=doc->head.next;
  while (doc_e!=doc) {
    if (src_file && doc_e->type_u8==DOCT_TEXT &&
	  (src2=StrMatch("Play(\"",doc_e->tag))) {
      src=src2+6;
      dst=buf;
      while (*src && *src!='\"')
	*dst++=*src++;
      *dst++=0;
      if (*src=='\"')  {
	*src++=0;
	st=CvtSong500Str(&octave_in,src2+6);
	src2[5]=0;
	st2=MStrPrint("%s\"%s\"%s",doc_e->tag,st,src);
	Free(st);
	Free(doc_e->tag);
	doc_e->tag=st2;
	res++;
      }
    } else if (doc_e->type_u8==DOCT_SONG) {
      st=CvtSong500Str(&octave_in,doc_e->aux_str);
      Free(doc_e->aux_str);
      doc_e->aux_str=st;
      res++;
    }
    doc_e=doc_e->next;
  }
  DocRecalc(doc);
  if (unlock)
    DocUnlock(doc);
  return res;
}

I64 CvtSong500File(U8 *filename)
{
  I64 res;
  CDoc *doc=DocRead(filename);
  if (res=CvtSong500Doc(doc)) {
    "%8d:%s\n",res,doc->filename.name;
    DocWrite(doc);
  }
  DocDel(doc);
  return res;
}
I64 CvtSong500Lst(CDirEntry *tmpde)
{
  I64 res=0;
  CDirEntry *tmpde1;
  while (tmpde) {
    tmpde1=tmpde->next;
    if (tmpde->attr & RS_ATTR_DIR) {
      if (tmpde->sub) {
	"Processing Directory: %s\n",tmpde->full_name;
	res+=CvtSong500Lst(tmpde->sub);
      }
    } else
      res+=CvtSong500File(tmpde->full_name);
    DirEntryDel(tmpde);
    tmpde=tmpde1;
  }
  return res;
}
I64 CvtSong500(U8 *files_find_mask="*",U8 *fu_flags=NULL)
{
  I64 fuf_flags=0;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+$");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  return CvtSong500Lst(FilesFind(files_find_mask,fuf_flags&FUG_FILES_FIND));
}
