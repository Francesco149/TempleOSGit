U0 CvtBMPFile(U8 *fname)
{
  CDC *dc=BMPFileRead(fname,TRUE);
  CSprite *elems=DC2Sprite(dc);
  NewLine(TEXT_ROWS/2);
  PutSprite(elems);
  NewLine(TEXT_ROWS/2);
  Free(elems);
  DCDel(dc);
}

public U0 CvtBMPDir(U8 *_dir_name=NULL)
{
  U8 *dir_name,*mask,*st;
  CDirEntry *tempm,*tempm1;

  if (_dir_name)
    dir_name=StrNew(_dir_name);
  else
    dir_name=PopUpPickDir;
 
  mask=MSPrintF("%s/*",dir_name);
  Free(dir_name);

  tempm=tempm1=FilesFind(mask,FUF_FLATTEN_TREE+FUF_JUST_FILES);
  Free(mask);

  DocClear;
  while (tempm) {
    st=MStrUtil(tempm->name,SUF_TO_UPPER);
    if (WildCardMatch("*.BMP",st))
      CvtBMPFile(tempm->full_name);
    Free(st);
    tempm=tempm->next;
  }

  CDirListDel(tempm1);
}

/*
Save your files in Windows as 16-color BMP.
Then, convert them to CSprite with this
program.

Cut and paste the converted bmp files
after running this program and selecting
a directory.
*/

//This is so I can use this file stand alone
//or as a component of a program.
#if IncludeDepth<2
CvtBMPDir;
#endif
