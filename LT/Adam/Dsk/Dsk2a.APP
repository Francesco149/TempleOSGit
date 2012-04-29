#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"

public U8 *DBlk(U64 blk,BoolI8 write=FALSE)
{//Dump Disk Blk.
//If you set write to TRUE, it will be written when you press <ESC>.
//See [::/LT/Demo/Dsk/Raw.CPZ] ::/LT/Demo/Dsk/Raw.CPZ.
  U8 *buf=MAlloc(BLK_SIZE);

  RBlks(Fs->cur_prt,buf,blk,1);
  LtfD(buf,BLK_SIZE);
  if (write) {
    "Edit and press <ESC> to write or <SHIFT-ESC>\n";
    if (View) {
      "Write\n";
      WBlks(Fs->cur_prt,buf,blk,1);
    }
  }
  return buf;
}

public U8 *DCluster(U64 c,BoolI8 write=FALSE,I64 num=0)
{//Dump Disk Cluster.
//If you set write to TRUE, it will be written when you press <ESC>.
//See [::/LT/Demo/Dsk/Raw.CPZ] ::/LT/Demo/Dsk/Raw.CPZ.
//Do [C:/LT/OSMain/Dsk/Dsk1b.CPZ,236] Dir("*",TRUE); to get cluster numbers of files.
  U8 *buf=MAlloc(Fs->cur_prt->spc<<BLK_SIZE_BITS);
  c=ClusterNumNext(Fs->cur_prt,c,num);
  RClusters(Fs->cur_prt,buf,c,1);
  "Cluster:%X\n",c;
  LtfD(buf,Fs->cur_prt->spc<<BLK_SIZE_BITS);
  if (write) {
    "Edit and press <ESC> to write or <SHIFT-ESC>\n";
    if (View) {
      "Write\n";
      WClusters(Fs->cur_prt,buf,c,1);
    }
  }
  return buf;
}

#help_index "File/Program Routines"

U0 OFFlatten(LTDirEntry *tempm,LTDirEntry **a,I64 *i)
{
  LTDirEntry *tempm1;
  while (tempm) {
    tempm1=tempm->next;
    if (tempm->attr&LT_ATTR_DIR) {
      OFFlatten(tempm->sub,a,i);
      LTDirEntryDel(tempm);
    } else {
      a[*i]=tempm;
      *i=*i+1;
    }
    tempm=tempm1;
  }
}


public I64 LTFileCnt(LTDirEntry *tempm)
{
  I64 cnt=0;
  while (tempm) {
    if (tempm->attr&LT_ATTR_DIR)
      cnt+=LTFileCnt(tempm->sub);
    else
      cnt++;
    tempm=tempm->next;
  }
  return cnt;
}

I64 OFCompare(LTDirEntry *e1,LTDirEntry *e2)
{
  return e1->cluster-e2->cluster;
}

public LTDirEntry **FilesOrder(LTDirEntry *tempm,I64 *_cnt=NULL)
{ //See [::/LT/Demo/Dsk/OrderedIO.CPZ] ::/LT/Demo/Dsk/OrderedIO.CPZ.
  I64 cnt=LTFileCnt(tempm),i=0;
  LTDirEntry **a=MAlloc(sizeof(LTDirEntry *)*cnt);
  OFFlatten(tempm,a,&i);
  QSortU64(a,cnt,&OFCompare);
  if (_cnt) *_cnt=cnt;
  return a;
}


#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"

public BoolI64 Copy(U8 *src_files_find_mask,U8 *dst_files_find_mask=".")
{
  BoolI8 result=TRUE;
  LTFileAccess *fa;
  LTDirEntry *tempm,*tempm1;
  U8 *st;
  if (!(tempm=FilesFind(src_files_find_mask)))
    return FALSE;
  if (IsDir(dst_files_find_mask)) {
    fa=LTFileAccessNew(dst_files_find_mask,TRUE);
    if (fa) {
      while (tempm) {
	tempm1=tempm->next;
	if (!(tempm->attr & LT_ATTR_DIR)) {
	  st=FileNameAbs(tempm->name);
	  if (!CopySingle(tempm->full_name,st))
	    result=FALSE;
	  Free(st);
	}
	LTDirEntryDel(tempm);
	tempm=tempm1;
      }
      LTFileAccessDel(fa);
    }
    return result;
  } else
    return CopySingle(src_files_find_mask,dst_files_find_mask);
}

public BoolI64 Move(U8 *f1,U8 *f2)
{
  if (Copy(f1,f2)) {
    Del(f1);
    return TRUE;
  }
  return FALSE;
}

U64 PutCopyTree(LTDirEntry *tempm,U64 src_dir_len,U64 dst_dir_len,U8 *dst_dir)
{
  U64 result=1;
  U8 *st;
  while (tempm) {
    st=MAlloc(StrLen(tempm->full_name)+dst_dir_len+2);
    MemCpy(st,dst_dir,dst_dir_len);
    StrCpy(st+dst_dir_len,tempm->full_name+src_dir_len);
    if (tempm->attr & LT_ATTR_DIR) {
      MkDir(st);
      result+=PutCopyTree(tempm->sub,src_dir_len,dst_dir_len,dst_dir);
    } else
      if (CopySingle(tempm->full_name,st))
	result++;
    Free(st);
    tempm=tempm->next;
  }
  return result;
}

public U64 CopyTree(U8 *src_files_find_mask,U8 *dst_files_find_mask)
{  //returns count of copied files (not dirs)
  LTFileAccess *fa;
  LTDirEntry *tempm=NULL;
  U64 result=0,i1,i2;
  U8 *st1,*st2;

  st1=DirNameAbs(src_files_find_mask);
  st2=DirNameAbs(dst_files_find_mask);
  i1=StrLen(st1);
  if (!StrNCmp(st1,st2,i1) && (st2[i1]=='/' || !st2[i1]) ) {
    Free(st1);
    Free(st2);
    return 0;
  }
  Free(st1);
  Free(st2);

  fa=LTFileAccessNew(src_files_find_mask,TRUE);
  if (fa) {
    tempm=SysFilesFind(fa->mask,1<<FUf_RECURSE);
    st1=CurDir;
    LTFileAccessDel(fa);
    i1=StrLen(st1);
    if (i1==3) i1--;

    fa=LTFileAccessNew(dst_files_find_mask,TRUE,TRUE);
    if (fa) {
      st2=CurDir;
      i2=StrLen(st2);
      if (i2==3) i2--;
      result=PutCopyTree(tempm,i1,i2,st2);
      LTFileAccessDel(fa);
      Free(st2);
    }
    LTDirListDel(tempm);
    Free(st1);
  }
  return result;
}

U0 DelTreeDirs(LTDirEntry *tempm1)
{
  LTDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & LT_ATTR_DIR) {
      if (tempm1->sub)
	DelTreeDirs(tempm1->sub);
      SysDel(tempm1->full_name,TRUE);
    }
    LTDirEntryDel(tempm1);
    tempm1=tempm2;
  }
}

U0 DelTreeFiles(LTDirEntry *tempm1)
{
  LTDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & LT_ATTR_DIR) {
      if (tempm1->sub)
	DelTreeFiles(tempm1->sub);
    } else
      SysDel(tempm1->full_name,FALSE);
    LTDirEntryDel(tempm1);
    tempm1=tempm2;
  }
}

public U0 DelTree(U8 *files_find_mask,U8 *fu_flags=NULL)
{
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (IsDir(files_find_mask)) {
    DelTreeDirs(FilesFind(files_find_mask,fuf_flags));
    SysDel(files_find_mask,TRUE);
    SysDel(files_find_mask,FALSE);
  } else
    DelTreeFiles(FilesFind(files_find_mask,fuf_flags));
}

U0 TouchFile(U8 *filename,U8 *attr,LTDate dt=0) //Just one file
{
  U8 *cur_dir=StrNew(filename);
  LTPrt *p=Drv2Prt(*filename);
  LTDirEntry de;
  U8 buf[136];
  if (FileFind(filename,&de,FALSE,TRUE)) {
    if (!StrCmp(attr,"+?"))
      "%-32ts %s\n",filename,SPrintFFlags(buf,Define("ST_FILE_ATTRS"),de.attr);
    else {
      StrFirstRem(cur_dir,":");
      StrLastRem(cur_dir,"/");
      if (!*cur_dir)
	StrCpy(cur_dir,"/");
      GetFFlags(&de.attr,Define("ST_FILE_ATTRS"),attr);
      if (dt)
	de.datetime=dt;
      else
	de.datetime=GetCurTimeLTDate;
      LTDirEntryNew(p,cur_dir,&de,FALSE);
    }
  } else PutDefineErr("ST_FILE_NOT_FOUND");
}

U0 TouchList(LTDirEntry *tempm1,U8 *attr,LTDate dt=0)
{
  LTDirEntry *tempm2;
  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & LT_ATTR_DIR) {
      if (tempm1->sub) {
	"%S%s\n","ST_SCANNING_DIR",tempm1->full_name;
	TouchList(tempm1->sub,attr,dt);
      }
    } else
      TouchFile(tempm1->full_name,attr,dt);
    LTDirEntryDel(tempm1);
    tempm1=tempm2;
  }
}

public U0 Touch(U8 *files_find_mask,U8 *attr="+?",U8 *fu_flags=NULL,LTDate dt=0)
{
//Attr "+?" =show
//     "+T" =resident
  U64 fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  TouchList(FilesFind(files_find_mask,fuf_flags),attr,dt);
}

U64 FFList(LTDirEntry *tempm1)
{
  U64 cnt=0;
  LTDirEntry *tempm2;

  while (tempm1) {
    tempm2=tempm1->next;
    if (tempm1->attr & LT_ATTR_DIR) {
      if (tempm1->sub)
	cnt+=FFList(tempm1->sub);
    } else {
      PutFileLink(tempm1->full_name);
      NewLine;
      cnt++;
    }
    LTDirEntryDel(tempm1);
    tempm1=tempm2;
  }
  return cnt;
}

public U64 FF(U8 *files_find_mask,U8 *fu_flags=NULL)
//Files Find
{
  U64 cnt=0,fuf_flags=0;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  cnt+=FFList(FilesFind(files_find_mask,fuf_flags));
  return cnt;
}

public U0 SetDrvTextAttr(U8 drv_let,U8 attr)
{ //This is to color the border text according to the current drive
  LTPrt *p;
  try {
    p=Drv2Prt(drv_let);
    p->text_attr=attr;
  } catch
    Fs->catch_except=TRUE;
}

U0 PrintFDrvTextAttr(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  U8 fg=p->text_attr&0xF,bg=p->text_attr>>4;
  if (fg!=bg)
    "$FG,%d$$BG,%d$",fg,bg;
}

I64 LTFSUnusedDrvSpace(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  U64 result=0,i,l;
  U8 *bitmap;
  try {
    l=(p->size+p->spc-1)/p->spc;
    i=((l+7)>>3+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    for (i=0;i<l;i++)
      if (!Bt(bitmap,i))
	result++;
    Free(bitmap);
  } catch
    PrtUnlock(p);
  return result*BLK_SIZE*p->spc;
}

I64 FAT32UnusedDrvSpace(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  U64 result=0,i,l;
  U32 *bitmap;
  try {
    l=(p->size+p->spc-1)/p->spc;
    i=(l*4+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    for (i=0;i<l;i++)
      if (!bitmap[i])
	result++;
    Free(bitmap);
  } catch
    PrtUnlock(p);
  return result*BLK_SIZE*p->spc;
}

public I64 UnusedDrvSpace(U8 drv_let=0)
{ //returns size in bytes
  I64 result=0;
  LTPrt *p=Drv2Prt(drv_let),*p_old=Fs->cur_prt;
  U8 *old_dir=StrNew(Fs->cur_dir);
  Drv(drv_let);
  switch (p->type) {
    case PT_LT:
      result=LTFSUnusedDrvSpace(drv_let);
      break;
    case PT_FAT32:
      result=FAT32UnusedDrvSpace(drv_let);
      break;
    default:
      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
  }
  Drv(Prt2Drv(p_old));
  Cd(old_dir);
  Free(old_dir);
  return result;
}

#help_index "File/Program Routines"

U64 TreeSizeFiles(LTDirEntry *tempm,I64 round_to)
{
  U64 result=0,i;
  while (tempm) {
    if (round_to)
      i=CeilU64(tempm->size,round_to);
    else
      i=tempm->size;
    if (tempm->attr&LT_ATTR_DIR)
      i+=TreeSizeFiles(tempm->sub,round_to);
    tempm->user_data=i;
    result+=i;
    tempm=tempm->next;
  }
  return result;
}

public U64 TreeSize(U8 *files_find_mask="/LT/*",U8 *fu_flags=NULL,I64 round_to=0)
{
//Does not include directory size of base directory, but
//does include size of sub directories.
  U64 fuf_flags=0,result=0;
  LTDirEntry *tempm1=NULL;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (tempm1=FilesFind(files_find_mask,fuf_flags)) {
    result=TreeSizeFiles(tempm1,round_to);
    LTDirListDel(tempm1);
  }
  return result;
}

#help_index ""
