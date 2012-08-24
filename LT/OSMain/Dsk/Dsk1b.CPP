BoolI64 MapDrv(U8 drv_let,LTPrt *p)
{
  drv_let=ToUpper(drv_let);
  if ('A'<=drv_let<='Z') {
    drv_to_prt[drv_let-'A']=p;
    p->drv_let=drv_let;
    return TRUE;
  } else
    return FALSE;
}

BoolI64 Drv(U8 drv_let)
{
  LTPrt *p=Drv2Prt(drv_let);
  LTBlkDev *bdev=p->bdev;
  BlkDevChk(bdev);
  if (p!=Fs->cur_prt) {
    if ( bdev->flags & LTBDF_REMOVABLE &&
       !(bdev->flags & LTBDF_INITIALIZED) )
      ChgDsk(Prt2Drv(p));
    if (bdev->type==LTBDT_RAM ||
	bdev->type==LTBDT_NON_LT_FILE)
      BlkDevInit(bdev);
  }
  Fs->cur_prt=p;
  Free(Fs->cur_dir);
  Fs->cur_dir=StrNew("/");
  switch (p->type) {
    case PT_LT:
    case PT_FAT32:
    case PT_ISO9660:
      return TRUE;
    default:
      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
      return FALSE;
  }
}

BoolI64 Cd(U8 *dirname,BoolI8 make_dirs=FALSE)
{//Optionally, will create the directories if they don't exist.
  I64 maxlen;
  BoolI8 result=TRUE;
  U8 *chg_to_buf,*new_cur_dir,*buf;
  LTPrt *p;
  U64 cur_dir_cluster=0;
  if (!*dirname) return TRUE;
  if (dirname[1]==':') {
    if (*dirname==':') {
      if (Fs->cur_prt!=dft_prt)
	if (!Drv(*dirname)) return FALSE;
    } else {
      if (Fs->cur_prt!=
	Drv2Prt(*dirname))
	  if (!Drv(*dirname)) return FALSE;
    }
    dirname+=2;
  }
  if (*dirname=='/' || !*dirname || !Fs->cur_dir) {
    Free(Fs->cur_dir);
    Fs->cur_dir=StrNew("/");
    if (*dirname=='/')
      dirname++;
  }
  chg_to_buf=MStrUtil(dirname,
    SUF_REM_LEADING|SUF_REM_TRAILING|SUF_REM_CTRL_CHARS);
  maxlen=StrLen(Fs->cur_dir)+1+StrLen(chg_to_buf)+1;
  new_cur_dir=MAlloc(maxlen);
  buf=MAlloc(maxlen);
  StrCpy(new_cur_dir,Fs->cur_dir);
  while (*chg_to_buf && result) {
    StrFirstRem(chg_to_buf,"/",buf);
    if (!*buf)
      StrCpy(new_cur_dir,"/");
    else if (!StrCmp(buf,"..")) {
      StrLastRem(new_cur_dir,"/");
      if (!*new_cur_dir)
	StrCpy(new_cur_dir,"/");
    } else if (!StrCmp(buf,".")) {
      ;
    } else if (*buf) {
      if (!StrCmp(buf,"HOME")) {
	result=Cd(sys_acct);
	Free(new_cur_dir);
	new_cur_dir=MAlloc(maxlen+StrLen(sys_acct));
	StrCpy(new_cur_dir,sys_acct+2);
      } else {
	p=Fs->cur_prt;
	cur_dir_cluster=Name2DirCluster(p,new_cur_dir);
	switch (p->type) {
	  case PT_LT:
	    result=LTFSCd(buf,cur_dir_cluster);
	    break;
	  case PT_FAT32:
	    result=FAT32Cd(buf,cur_dir_cluster);
	    break;
	  case PT_ISO9660:
	    result=ISO1Cd(buf,cur_dir_cluster);
	    break;
	  default:
	    PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	    result=FALSE;
	}
	if (!result && make_dirs) {
	  Free(Fs->cur_dir);
	  Fs->cur_dir=StrNew(new_cur_dir);
	  result=MkDir(buf);
	}
	if (result) {
	  if (StrCmp(new_cur_dir,"/"))
	    StrCat(new_cur_dir,"/");
	  StrCat(new_cur_dir,buf);
	}
      }
    }
  }
  Free(Fs->cur_dir);
  Fs->cur_dir=StrNew(new_cur_dir);
  Free(buf);
  Free(chg_to_buf);
  Free(new_cur_dir);
  return result;
}

U0 LTFileAccessDel(LTFileAccess *fa)
{
  LTBlkDev *bdev;
  if (!fa) return;
  bdev=fa->old_prt->bdev;
  if (!(bdev->flags & LTBDF_INIT_IN_PROGRESS)) {
    if (fa->old_dir) {
      Drv(Prt2Drv(fa->old_prt));
      Cd(fa->old_dir);
    }
  } else {
    Fs->cur_prt=fa->old_prt;
    Free(Fs->cur_dir);
    Fs->cur_dir=StrNew("/");
  }
  Free(fa->old_dir);
  Free(fa->mask);
  Free(fa);
}

LTFileAccess *LTFileAccessNew(U8 *_mask,BoolI8 make_mask=FALSE,BoolI8 make_dirs=FALSE)
{
  BoolI8 valid=TRUE;
  U8 *buf,*mask,*temp_mask;
  BoolI8 old_silent;
  LTFileAccess *fa=CAlloc(sizeof(LTFileAccess));
  mask=MStrUtil(_mask,
    SUF_REM_LEADING|SUF_REM_TRAILING|SUF_REM_CTRL_CHARS);
  temp_mask=mask;
  fa->old_dir=StrNew(Fs->cur_dir);
  fa->old_prt=Fs->cur_prt;
  if (*mask && mask[1]==':') {
    if (Fs->cur_prt!=Drv2Prt(*mask))
      if (!Drv(*mask)) valid=FALSE;
    mask+=2;
  }
  if (StrStr(mask,"HOME") && Fs->cur_prt!=Drv2Prt(*sys_acct))
    Drv(*sys_acct);
  fa->p=Fs->cur_prt;
  PrtChk(fa->p);
  buf=MAlloc(StrLen(mask)+2);
  StrCpy(buf,mask);
  fa->mask=MAlloc(StrLen(mask)+2);
  StrLastRem(buf,"/",fa->mask);
  if (*mask=='/' && !*buf)
    StrCpy(buf,"/");
  if (*buf)
    if (!Cd(buf,make_dirs))
      valid=FALSE;
  if (valid && make_mask) {
    if (!*fa->mask) {
      Free(fa->mask);
      fa->mask=StrNew("*");
    } else {
      if (!make_dirs || FileNameChk(fa->mask)) {
	old_silent=Silent(ON);
	if (Cd(fa->mask,make_dirs)) {
	  Free(fa->mask);
	  fa->mask=StrNew("*");
	}
	Silent(old_silent);
      }
    }
  }
  Free(buf);
  Free(temp_mask);
  if (!valid) {
    LTFileAccessDel(fa);
    fa=NULL;
//    throw(EXCEPT_FILE);
  }
  return fa;
}

LTDirEntry *FilesFind(U8 *files_find_mask,U64 fuf_flags=0)
{
/*
Flags:
 [C:/LT/OSMain/Adam1a.HPZ,3738] FUF_RECURSE		+r Recurse
 [C:/LT/OSMain/Adam1a.HPZ,3752] FUF_FLATTEN_TREE	+f **be sure to use this along with '+F'**.
 [C:/LT/OSMain/Adam1a.HPZ,3748] FUF_JUST_DIRS		+D just directories
 [C:/LT/OSMain/Adam1a.HPZ,3749] FUF_JUST_FILES		+F just files
 [C:/LT/OSMain/Adam1a.HPZ,3750] FUF_JUST_TEXT_FILES	+t just text files: [C:/LT/OSMain/Adam1a.HPZ,2065] TEXT_FILE_MASK
 [C:/LT/OSMain/Adam1a.HPZ,3751] FUF_JUST_SRC_FILES	+s just src files : [C:/LT/OSMain/Adam1a.HPZ,2064] SRC_FILE_MASK
See [] ST_FILE_UTIL_FLAGS when used in calling program taking text flags.
*/
  LTDirEntry *result=NULL;
  LTFileAccess *fa=LTFileAccessNew(files_find_mask,TRUE);
  if (fa) {
    result=SysFilesFind(fa->mask,fuf_flags);
    LTFileAccessDel(fa);
  }
  return result;
}

BoolI64 IsDir(U8 *dir_name)
{
  BoolI8 result=FALSE;
  U8 *mask=MSPrintF("%s/*",dir_name);
  BoolI8 old_silent=Silent(ON);
  LTFileAccess *fa=LTFileAccessNew(mask);
  if (fa) {
    result=TRUE;
    LTFileAccessDel(fa);
  }
  Free(mask);
  Silent(old_silent);
  return result;
}

U0 Dir(U8 *files_find_mask="*",BoolI8 full=FALSE)
{
  LTFileAccess *fa;
  fa=LTFileAccessNew(files_find_mask,TRUE);
  if (fa) {
    DirLT(fa->mask,0,full);
    LTFileAccessDel(fa);
  }
}


U0 Del(U8 *files_find_mask,BoolI8 print_msg=TRUE)
{
  LTFileAccess *fa=LTFileAccessNew(files_find_mask);
  if (fa) {
    switch (fa->p->type) {
      case PT_LT:
	LTFSFilesDel(fa->p,Fs->cur_dir,fa->mask,0,FALSE,print_msg);
	break;
      case PT_FAT32:
	FAT32FilesDel(fa->p,Fs->cur_dir,fa->mask,0,FALSE,print_msg);
	break;
      case PT_ISO9660:
	PutDefineErr("ST_NOT_WRITABLE");
	break;
      default:
	PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
    }
    LTFileAccessDel(fa);
  }
}

U8 *FileRead(U8 *filename,U64 *size=NULL,I64 *attr=NULL,BoolI8 raw=FALSE)
{
  GenericHashEntry *temph;
  U8 *absname;
  U8 *result=NULL;
  I64 size2=0,attr2=0;
  LTFileAccess *fa;
  ArcCompressStruct *ac=NULL;

  if (attr)
    *attr=0;
  absname=FileNameAbs(filename);
  if (!raw && (temph=HashFind(absname,adam_task->hash_table,HTT_FILE))) {
    if (FileAttr(absname) & LT_ATTR_COMPRESSED) {
      ac=temph->user_data0;
      if (size)
	*size=ac->expanded_size;
      if (attr)
	*attr=FileAttr(absname,*attr);
      result=ExpandBuf(ac);
    } else {
      result=MAlloc(temph->user_data1+1);
      MemCpy(result,temph->user_data0,temph->user_data1);
      result[temph->user_data1]=0; //Terminate
      if (size)
	*size=temph->user_data1;
    }
  } else {
    fa=LTFileAccessNew(absname);
    if (fa) {
      switch (fa->p->type) {
	case PT_LT:
	  result=LTFSFileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	  break;
	case PT_FAT32:
	  result=FAT32FileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	  break;
	case PT_ISO9660:
	  result=ISO1FileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	  break;
	default:
	  PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
      }
      while (!result && StrCmp(Fs->cur_dir,"/")) {
	Cd("..");
	switch (Fs->cur_prt->type) {
	  case PT_LT:
	    result=LTFSFileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	    break;
	  case PT_FAT32:
	    result=FAT32FileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	    break;
	  case PT_ISO9660:
	    result=ISO1FileRead(fa->p,Fs->cur_dir,fa->mask,&size2,&attr2,raw,&ac);
	    break;
	  default:
	    PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	}
      }
      if (!result) {
	"%s ",filename;
	PutDefineErr("ST_FILE_NOT_FOUND");
      }
      LTFileAccessDel(fa);
    }
    if (size)
      *size=size2;
    if (attr)
      *attr=attr2;
    if (!raw && attr2 & LT_ATTR_RESIDENT)
      HashGenericAdd(absname,HTT_FILE,AMAllocIdentical(ac),size2,0,adam_task);
    Free(ac);
  }
  Free(absname);
  return result;
}

U8 *TextFileRead(U8 *filename,U64 *attr=NULL)
{
  return FileRead(filename,NULL,attr);
}


BoolI64 FileFind(U8 *filename,LTDirEntry *rde=NULL,
      BoolI8 dir_only=FALSE,BoolI8 file_only=FALSE,BoolI8 scan_parents=FALSE)
{
  U64 cur_dir_cluster;
  BoolI8 result=FALSE;
  LTDirEntry de;
  LTFileAccess *fa=LTFileAccessNew(filename);
  if (fa) {
    cur_dir_cluster=Name2DirCluster(fa->p,Fs->cur_dir);
    switch (fa->p->type) {
      case PT_LT:
	result=LTFSFileFind(fa->p,cur_dir_cluster,fa->mask,
			  &de,dir_only,file_only);
	break;
      case PT_FAT32:
	result=FAT32FileFind(fa->p,cur_dir_cluster,fa->mask,
			  &de,dir_only,file_only);
	break;
      case PT_ISO9660:
	result=ISO1FileFind(fa->p,cur_dir_cluster,fa->mask,
			   &de,dir_only,file_only);
	break;
      default:
	PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
    }
    if (scan_parents) {
      while (!result && StrCmp(Fs->cur_dir,"/")) {
	Cd("..");
	cur_dir_cluster=Name2DirCluster(fa->p,Fs->cur_dir);
	switch (fa->p->type) {
	   case PT_LT:
	     result=LTFSFileFind(fa->p,cur_dir_cluster,fa->mask,
			      &de,dir_only,file_only);
	     break;
	   case PT_FAT32:
	     result=FAT32FileFind(fa->p,cur_dir_cluster,fa->mask,
			      &de,dir_only,file_only);
	     break;
	   case PT_ISO9660:
	     result=ISO1FileFind(fa->p,cur_dir_cluster,fa->mask,
				&de,dir_only,file_only);
	     break;
	   default:
	     PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
	}
      }
    }
    LTFileAccessDel(fa);
  }
  if (result && rde)
    MemCpy(rde,&de,sizeof(LTDirEntry));
  return result;
}

U64 FileSize(U8 *filename)
{
  LTDirEntry de;
  if (FileFind(filename,&de))
    return de.size;
  else
    return 0;
}

U64 FileSizeExpanded(U8 *filename)
{
  U8 buf[BLK_SIZE];
  U8 *full_name;
  LTPrt *p;
  LTDirEntry de;
  if (FileFind(filename,&de)) {
    full_name=FileNameAbs(filename);
    p=Drv2Prt(*full_name);
    Free(full_name);
    if (p->type==PT_LT && de.expanded_size)
      return de.expanded_size;
    else {
      if (de.size && FileAttr(de.name) & LT_ATTR_COMPRESSED) {
	RBlks(p,buf,Cluster2Blk(p,de.cluster),1);
	return (&buf)(ArcCompressStruct *)->expanded_size;
      } else
	return de.size;
    }
  } else
    return 0;
}

U64 FileWrite(U8 *filename,U8 *fbuf,
   U64 size,LTDate date=0,U64 attr=0,BoolI8 raw=FALSE)
{
  U64 c=0;
  GenericHashEntry *temph;
  LTFileAccess *fa=LTFileAccessNew(filename,FALSE,TRUE);
  ArcCompressStruct *ac=NULL;
  U8 *absname=FileNameAbs(filename);
  if (fa) {
    switch (fa->p->type) {
      case PT_LT:
	c=LTFSFileWrite(fa->p,Fs->cur_dir,fa->mask,
	  fbuf,size,date,attr,raw,&ac);
	break;
      case PT_FAT32:
	c=FAT32FileWrite(fa->p,Fs->cur_dir,fa->mask,
	  fbuf,size,date,attr,raw,&ac);
	break;
      case PT_ISO9660:
	PutDefineErr("ST_NOT_WRITABLE");
	break;
      default:
	PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
    }
    if (temph=HashFind(absname,adam_task->hash_table,HTT_FILE)) {
      if (!raw && ac && attr & LT_ATTR_RESIDENT) {
	Free(temph->user_data0);
	temph->user_data0=AMAllocIdentical(ac);
	temph->user_data1=size;
      } else
	HashRemDel(temph,adam_task->hash_table);
    } else if (!raw && ac && attr & LT_ATTR_RESIDENT)
      HashGenericAdd(absname,HTT_FILE,AMAllocIdentical(ac),size,0,adam_task);
    Free(ac);
    LTFileAccessDel(fa);
  }
  Free(absname);
  return c;
}

U0 TextFileWrite(U8 *filename,U8 *st,U64 attr=0)
{
  FileWrite(filename,st,StrLen(st),0,attr);
}
