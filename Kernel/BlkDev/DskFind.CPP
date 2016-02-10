CDirEntry *FilesFind2(U8 *files_find_mask,I64 fuf_flags)
{
  CDrv *dv=Fs->cur_dv;
  CDirEntry *result=NULL;
  DrvChk(dv);
  switch (dv->fs_type) {
    case FSt_REDSEA:
      result=RedSeaFilesFind(files_find_mask,fuf_flags);
      break;
    case FSt_FAT32:
      result=FAT32FilesFind(files_find_mask,fuf_flags);
      break;
    case FSt_ISO9660:
      result=ISO1FilesFind(files_find_mask,fuf_flags);
      break;
    default:
      PrintErr("File System Not Supported\n");
      result=NULL;
  }
  if (result) {
    DirFilesSort(&result,SK_NAME);
    if (fuf_flags&(FUF_FLATTEN_TREE|FUF_JUST_FILES))
      DirFilesFlatten(result,&result,fuf_flags);
    if (fuf_flags&FUF_CLUSTER_ORDER)
      DirFilesSort(&result,SK_CLUSTER);
  }
  return result;
}

CDirEntry *FilesFind(U8 *files_find_mask,I64 fuf_flags=0)
{/* See [C:/Doc/FileUtils.TXT.Z,1].

Find files and make a directory tree in memory.

When done, you free with [C:/Kernel/BlkDev/DskDirA.CPP.Z,17] DirEntryDel(),[C:/Kernel/BlkDev/DskDirA.CPP.Z,26] DirEntryDel2(),
[C:/Kernel/BlkDev/DskDirA.CPP.Z,36] DirTreeDel() or [C:/Kernel/BlkDev/DskDirA.CPP.Z,49] DirTreeDel2().

*/
  CDirEntry *result;
  CDirContext *dirc;
  if (fuf_flags&~FUG_FILES_FIND)
    throw('FUF');
  if (fuf_flags&FUF_SINGLE) {
    result=MAlloc(sizeof(CDirEntry));
    if (!FileFind(files_find_mask,result)) {
      Free(result);
      return NULL;
    }
  } else if (dirc=DirContextNew(files_find_mask,TRUE)) {
    result=FilesFind2(dirc->mask,fuf_flags);
    DirContextDel(dirc);
  } else
    return NULL;
  return result;
}

Bool FileFind(U8 *filename,CDirEntry *_de=NULL,I64 fuf_flags=0)
{//[C:/Kernel/KernelA.HPP.Z,2583] FUF_JUST_DIRS, [C:/Kernel/KernelA.HPP.Z,2584] FUF_JUST_FILES, [C:/Kernel/KernelA.HPP.Z,2590] FUF_Z_OR_NOT_Z, [C:/Kernel/KernelA.HPP.Z,2592] FUF_SCAN_PARENTS
//If you pass _de, you must Free(_de->full_name);
  I64 i,j,cur_dir_cluster;
  U8 *altname,*curname,*full_name=NULL;
  CDirEntry de;
  CDirContext *dirc;
  Bool result=FALSE,old_silent;
  if (fuf_flags&~FUG_FILE_FIND)
    throw('FUF');
  if (!filename || *filename && filename[1]==':' && !Let2Drv(*filename,FALSE))
    return FALSE;
  altname=ToggleZorNotZ(filename);
  if (fuf_flags&FUF_Z_OR_NOT_Z)
    j=2;
  else
    j=1;
  for (i=0;i<j && !result;i++) {
    if (!i)
      curname=filename;
    else
      curname=altname;
    old_silent=Silent;
    if (!(dirc=DirContextNew(curname)))
      Silent(old_silent);
    else {
      Silent(old_silent);
      cur_dir_cluster=Name2DirCluster(dirc->dv,Fs->cur_dir);
      switch (dirc->dv->fs_type) {
	case FSt_REDSEA:
	  result=RedSeaFileFind(dirc->dv,cur_dir_cluster,dirc->mask,
		&de,fuf_flags);
	  break;
	case FSt_FAT32:
	  result=FAT32FileFind(dirc->dv,cur_dir_cluster,dirc->mask,
		&de,fuf_flags);
	  break;
	case FSt_ISO9660:
	  result=ISO1FileFind(dirc->dv,cur_dir_cluster,dirc->mask,
		&de,fuf_flags);
	  break;
	default:
	  PrintErr("File System Not Supported\n");
      }
      if (result && _de) {
	if (StrCmp(Fs->cur_dir,"/"))
	  full_name=MStrPrint("%C:%s/%s",
		Drv2Let(Fs->cur_dv),Fs->cur_dir,de.name);
	else
	  full_name=MStrPrint("%C:/%s",Drv2Let(Fs->cur_dv),de.name);
      }
      DirContextDel(dirc);
    }
  }
  for (i=0;i<j && !result && fuf_flags&FUF_SCAN_PARENTS;i++) {
    if (!i)
      curname=filename;
    else
      curname=altname;
    old_silent=Silent;
    if (!(dirc=DirContextNew(curname)))
      Silent(old_silent);
    else {
      Silent(old_silent);
      cur_dir_cluster=Name2DirCluster(dirc->dv,Fs->cur_dir);
      while (!result && StrCmp(Fs->cur_dir,"/")) {
	Cd("..");
	cur_dir_cluster=Name2DirCluster(dirc->dv,Fs->cur_dir);
	switch (dirc->dv->fs_type) {
	  case FSt_REDSEA:
	    result=RedSeaFileFind(dirc->dv,cur_dir_cluster,
		  dirc->mask,&de,fuf_flags);
	    break;
	  case FSt_FAT32:
	    result=FAT32FileFind(dirc->dv,cur_dir_cluster,
		  dirc->mask,&de,fuf_flags);
	    break;
	  case FSt_ISO9660:
	    result=ISO1FileFind(dirc->dv,cur_dir_cluster,
		  dirc->mask,&de,fuf_flags);
	    break;
	  default:
	    PrintErr("File System Not Supported\n");
	}
      }
      if (result && _de) {
	if (StrCmp(Fs->cur_dir,"/"))
	  full_name=MStrPrint("%C:%s/%s",
		Drv2Let(Fs->cur_dv),Fs->cur_dir,de.name);
	else
	  full_name=MStrPrint("%C:/%s",
		Drv2Let(Fs->cur_dv),de.name);
      }
      DirContextDel(dirc);
    }
  }
  if (_de) {
    if (result) {
      MemCpy(_de,&de,sizeof(CDirEntry));
      _de->full_name=full_name;
    } else
      MemSet(_de,0,sizeof(CDirEntry));
  }
  Free(altname);
  return result;
}
