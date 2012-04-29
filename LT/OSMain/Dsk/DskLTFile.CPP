U0 FAllocFile(LTFile *f,U64 cnt,BoolI8 contiguous)
{
  LTPrt *fp=f->p;
  LTFileAccess *fa=LTFileAccessNew(f->de.full_name);
  f->de.cluster=AllocClusters(fp,0,(cnt+fp->spc-1)/fp->spc,contiguous);
  f->de.size=cnt<<BLK_SIZE_BITS;
  f->de.expanded_size=f->de.size;
  if (fa) {
    LTDirEntryNew(fa->p,Fs->cur_dir,&f->de,TRUE);
    LTFileAccessDel(fa);
  }
  f->flags&=~LTF_NEW_FILE;
}

U64 FSize(LTFile *f)
{
  if (f)
    return f->de.size;
  else
    return 0;
}

U0 FSync(LTFile *f)
{
  if (f->flags & LTF_BUF_DIRTY) {
    WClusters(f->p,f->cluster_buf,f->cluster,1);
    f->flags&=~LTF_BUF_DIRTY;
  }
}


LTFile *FOpen(U8 *filename,U8 *flags,U64 cnt=0)
{  //allows flags "r","w","w+" and "c" (contiguous)
  LTFile *f=CAlloc(sizeof(LTFile));
  LTFileAccess *fa;
  U8 *full_name,*full_name2;
  BoolI8 contiguous=StrOcc(flags,'c');

  f->cluster=INVALID_CLUSTER;
  f->fblk_num=0;
  if (cnt)
    f->max_blk=cnt-1;
  else
    f->max_blk=INVALID_CLUSTER;
  f->file_cluster_num=INVALID_CLUSTER;
  full_name2=FileNameAbs(filename);
  full_name=AStrNew(full_name2);
  Free(full_name2);
  f->p=Drv2Prt(*full_name);
  if (f->p->type==PT_LT)
    contiguous=TRUE;
  if (contiguous) {
    f->flags|=LTF_CONTIGUOUS;
    if (f->p->type!=PT_LT) {
      if (!(FileAttr(filename) & LT_ATTR_CONTIGUOUS))
	throw(EXCEPT_FILE,2);
    }
  }
  f->cluster_buf=CAlloc(f->p->spc<<BLK_SIZE_BITS);
  if (StrOcc(flags,'w')) {
    f->flags=f->flags|LTF_WRITE|LTF_NEEDS_WRITE;
    if (StrOcc(flags,'+')) {
      if (FileFind(full_name,&f->de,FALSE,TRUE)) {
	f->de.full_name=full_name;
	if (contiguous)
	  f->max_blk=(FSize(f)+BLK_SIZE-1)>>BLK_SIZE_BITS-1;
	return f;
      }
    } else
      Del(full_name,FALSE);
    f->de.full_name=full_name;
    f->flags|=LTF_NEW_FILE;
    fa=LTFileAccessNew(full_name);
    if (fa) {
      StrCpy(f->de.name,fa->mask);
      LTFileAccessDel(fa);
      if (cnt)
	FAllocFile(f,cnt,contiguous);
      if (!contiguous || cnt)
	return f;
    }
  } else {
    if (FileFind(full_name,&f->de,FALSE,TRUE)) {
      f->de.full_name=full_name;
      f->max_blk=(FSize(f)+BLK_SIZE-1)>>BLK_SIZE_BITS-1;
      return f;
    }
  }
  Free(f->cluster_buf);
  Free(full_name);
  Free(f);
  return NULL;
}

U0 FClose(LTFile *f)
{
  LTFileAccess *fa;
  if (f) {
    FSync(f);
    if (f->flags & LTF_NEEDS_WRITE) {
      fa=LTFileAccessNew(f->de.full_name);
      if (fa) {
	if (!(f->flags & LTF_USE_OLD_DATETIME))
	  f->de.datetime=GetCurTimeLTDate;
	if (f->flags & LTF_NEW_FILE)
	  LTDirEntryNew(fa->p,Fs->cur_dir,&f->de,TRUE);
	else
	  LTDirEntryNew(fa->p,Fs->cur_dir,&f->de,FALSE);
	LTFileAccessDel(fa);
      } else
	throw(EXCEPT_FILE,1);
    }
    Free(f->cluster_buf);
    Free(f->de.full_name);
    Free(f);
  }
}

U64 FSetCluster(LTFile *f,U64 c,U64 blk,BoolI8 read)
{
  LTPrt *p=f->p;
  I64 i;
  if (f->cluster!=c) {
    if (f->flags & LTF_BUF_DIRTY) {
      i=p->spc;
      if (f->max_blk!=INVALID_CLUSTER) {
	i=f->max_blk+1-f->file_cluster_num*p->spc;
	if (i>p->spc)
	  i=p->spc;
      }
      WClustersBlks(p,f->cluster_buf,f->cluster,i);
      f->flags=f->flags & ~LTF_BUF_DIRTY;
    }
    f->cluster=c;
    f->file_cluster_num=blk/p->spc;
    if (read) {
      i=p->spc;
      if (f->max_blk!=INVALID_CLUSTER) {
	i=f->max_blk+1-f->file_cluster_num*p->spc;
	if (i>p->spc)
	  i=p->spc;
      }
      c=RClustersBlks(p,f->cluster_buf,c,i);
    }
  }
  return c;
}

BoolI64 FRBlks(LTFile *f,U8 *buf,U64 blk=LTFB_NEXT_BLK,U64 cnt=1)
{
  LTPrt *p=f->p;
  U64 spc=p->spc,i,j,c=f->de.cluster;

  if (!f || !p) return FALSE;
  if (blk==LTFB_NEXT_BLK)
    blk=f->fblk_num;
  if (blk+cnt-1>f->max_blk)
    return FALSE;
  if (!cnt) return TRUE;

  if (f->flags & LTF_CONTIGUOUS) {
    RBlks(p,buf,Cluster2Blk(p,c)+blk,cnt);
    blk+=cnt;
  } else {
    i=blk/spc;
    if (i>=f->file_cluster_num) {
      c=f->cluster;
      i-=f->file_cluster_num;
    }
    if (i)
      c=ClusterNumNext(p,c,i);

    i=blk%spc;
    if (i) {
      c=FSetCluster(f,c,blk,TRUE);
      j= cnt<spc-i ? cnt:spc-i;
      MemCpy(buf,f->cluster_buf+i<<BLK_SIZE_BITS,j<<BLK_SIZE_BITS);
      buf+=j<<BLK_SIZE_BITS;
      cnt-=j;
      blk+=j;
    }
    while (cnt>=spc) {
      c=FSetCluster(f,c,blk,TRUE);
      MemCpy(buf,f->cluster_buf,spc<<BLK_SIZE_BITS);
      buf+=spc<<BLK_SIZE_BITS;
      cnt-=spc;
      blk+=spc;
    }
    if (cnt) {
      c=FSetCluster(f,c,blk,TRUE);
      MemCpy(buf,f->cluster_buf,cnt<<BLK_SIZE_BITS);
      buf+=cnt<<BLK_SIZE_BITS;
      blk+=cnt;
    }
  }
  f->fblk_num=blk;
  return TRUE;
}

BoolI64 FWBlks(LTFile *f,U8 *buf,U64 blk=LTFB_NEXT_BLK,U64 cnt=1)
{
  LTPrt *p=f->p;
  U64 spc=p->spc,i,j,c=f->de.cluster,c1;

  if (!f || !p) return FALSE;
  if (blk==LTFB_NEXT_BLK)
    blk=f->fblk_num;
  if (blk+cnt-1>f->max_blk)
    return FALSE;
  if (!(f->flags & LTF_WRITE))
    return FALSE;
  if (!cnt) return TRUE;
  if (f->flags & LTF_CONTIGUOUS) {
    WBlks(p,buf,Cluster2Blk(p,c)+blk,cnt);
    blk+=cnt;
  } else {
    if (!c) {
      c=AllocClusters(p,0,1,FALSE);
      f->file_cluster_num=0;
      f->cluster=c;
      f->de.cluster=c;
      f->flags|=LTF_NEEDS_WRITE|LTF_NEW_FILE;
    }
    i=blk/spc;
    if (i>=f->file_cluster_num) {
      c=f->cluster;
      i-=f->file_cluster_num;
    }
    while (i) {
      c1=c;
      c=ClusterNumNext(p,c1,1);
      if (c==INVALID_CLUSTER) {
	c=AllocClusters(p,c1,i,FALSE);
	if (i>1)
	  c=ClusterNumNext(p,c,i-1);
	break;
      } else
	i--;
    }

    i=blk%spc;
    if (i) {
      FSetCluster(f,c,blk,TRUE);
      j=cnt<spc-i ? cnt:spc-i;
      MemCpy(f->cluster_buf+BLK_SIZE*i,buf,j<<BLK_SIZE_BITS);
      f->flags|=LTF_BUF_DIRTY;
      buf+=j<<BLK_SIZE_BITS;
      cnt-=j;
      blk+=j;
      if (cnt) {
	c1=c;
	c=ClusterNumNext(p,c1,1);
	if (c==INVALID_CLUSTER)
	  c=AllocClusters(p,c1,1,FALSE);
      }
    }
    while (cnt>=spc) {
      FSetCluster(f,c,blk,FALSE);
      MemCpy(f->cluster_buf,buf,spc<<BLK_SIZE_BITS);
      f->flags|=LTF_BUF_DIRTY;
      buf+=spc<<BLK_SIZE_BITS;
      cnt-=spc;
      blk+=spc;
      if (cnt) {
	c1=c;
	c=ClusterNumNext(p,c1,1);
	if (c==INVALID_CLUSTER)
	  c=AllocClusters(p,c1,1,FALSE);
      }
    }
    if (cnt) {
      FSetCluster(f,c,blk,TRUE);
      MemCpy(f->cluster_buf,buf,cnt<<BLK_SIZE_BITS);
      f->flags|=LTF_BUF_DIRTY;
      buf+=cnt<<BLK_SIZE_BITS;
      blk+=cnt;
    }
    if (f->de.size<blk<<BLK_SIZE_BITS)
      f->de.size=blk<<BLK_SIZE_BITS;
    if (f->de.expanded_size<blk<<BLK_SIZE_BITS)
      f->de.expanded_size=blk<<BLK_SIZE_BITS;
  }
  f->fblk_num=blk;
  return TRUE;
}
