extern U8 *RedSeaFileRead(CDrv *dv,U8 *cur_dir,U8 *filename,
	I64 *_size,I64 *_attr);
extern U8 *FAT32FileRead(CDrv *dv,U8 *cur_dir,U8 *filename,
	I64 *_size,I64 *_attr);
extern U8 *ISO1FileRead(CDrv *dv,U8 *cur_dir,U8 *filename,
	I64 *_size,I64 *_attr);

U8 *FileReadRaw(U8 *filename,I64 *_size=NULL,I64 *_attr=NULL)
{//Read whole file from disk. No uncompress.
  CHashGeneric *temph;
  U8 *absname,*altname,*curname,*res=NULL;
  I64 i,size=0,attr=0;
  CDirContext *dirc;
  absname=FileNameAbs(filename);
  altname=ToggleZorNotZ(absname);
  if ((temph=HashFind(absname,adam_task->hash_table,HTT_FILE))||
	(temph=HashFind(altname,adam_task->hash_table,HTT_FILE))) {
    size=temph->user_data1;
    res=MAlloc(size+1);
    MemCpy(res,temph->user_data0,size);
    res[size]=0; //Terminate
    attr=FileAttr(temph->str,attr);
  } else {
    for (i=0;i<2 && !res;i++) {//Try name, then altname
      if (!i)
	curname=absname;
      else
	curname=altname;
      if (dirc=DirContextNew(curname)) {
	switch (dirc->dv->fs_type) {
	  case FSt_REDSEA:
	    res=RedSeaFileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		  &size,&attr);
	    break;
	  case FSt_FAT32:
	    res=FAT32FileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		  &size,&attr);
	    break;
	  case FSt_ISO9660:
	    res=ISO1FileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		  &size,&attr);
	    break;
	  default:
	    PrintErr("File System Not Supported\n");
	}
	DirContextDel(dirc);
      }
    }

    //Search parent directories.
    for (i=0;i<2 && !res;i++) {//Try name, then altname
      if (!i)
	curname=absname;
      else
	curname=altname;
      if (dirc=DirContextNew(curname)) {
	while (!res && StrCmp(Fs->cur_dir,"/")) {
	  Cd("..");
	  switch (Fs->cur_dv->fs_type) {
	    case FSt_REDSEA:
	      res=RedSeaFileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		    &size,&attr);
	      break;
	    case FSt_FAT32:
	      res=FAT32FileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		    &size,&attr);
	      break;
	    case FSt_ISO9660:
	      res=ISO1FileRead(dirc->dv,Fs->cur_dir,dirc->mask,
		    &size,&attr);
	      break;
	    default:
	      PrintErr("File System Not Supported\n");
	  }
	}
	DirContextDel(dirc);
      }
    }
    if (!res) {
      "%s ",filename;
      PrintErr("File not found.\n");
    }
    if (res && attr & RS_ATTR_RESIDENT)
      HashGenericAdd(curname,HTT_FILE,
	    AMAllocIdent(res),size,0,adam_task);
  }
  if (_attr) *_attr=attr;
  if (_size) *_size=size;
  Free(absname);
  Free(altname);
  return res;
}

extern I64 RedSeaFileWrite(CDrv *dv,U8 *cur_dir,U8 *name,U8 *buf,I64 size,
	CDate cdt,I64 attr);
extern I64 FAT32FileWrite(CDrv *dv,U8 *cur_dir,U8 *name,U8 *buf,I64 size,
	CDate cdt,I64 attr);

I64 FileWriteRaw(U8 *filename,U8 *fbuf,I64 size,CDate cdt=0,I64 attr=0)
{//Write whole file to disk. No compress.
  I64 c=0;
  CHashGeneric *temph;
  CDirContext *dirc;
  U8 *absname=FileNameAbs(filename);
  if (dirc=DirContextNew(filename,FALSE,TRUE)) {
    attr=FileAttr(dirc->mask,attr);
    if (!cdt) cdt=Now;
    switch (dirc->dv->fs_type) {
      case FSt_REDSEA:
	c=RedSeaFileWrite(dirc->dv,Fs->cur_dir,dirc->mask,fbuf,
	      size,cdt,attr);
	break;
      case FSt_FAT32:
	c=FAT32FileWrite(dirc->dv,Fs->cur_dir,dirc->mask,fbuf,
	      size,cdt,attr);
	break;
      case FSt_ISO9660:
	PrintErr("Not Writable\n");
	break;
      default:
	PrintErr("File System Not Supported\n");
    }
    if (temph=HashFind(absname,adam_task->hash_table,HTT_FILE)) {
      if (attr & RS_ATTR_RESIDENT) {
	Free(temph->user_data0);
	temph->user_data0=AMAllocIdent(fbuf);
	temph->user_data1=size;
      } else
	HashRemDel(temph,adam_task->hash_table);
    } else if (attr & RS_ATTR_RESIDENT)
      HashGenericAdd(absname,HTT_FILE,AMAllocIdent(fbuf),size,0,adam_task);
    DirContextDel(dirc);
  }
  Free(absname);
  return c;
}
