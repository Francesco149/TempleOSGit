//This makes a file of N random double values.
//[D:/Demo/SortFileExample/UnsortedF64.TXT.Z,1]

Cd(__DIR__);;

U0 F64FileGenTXT(U8 *filename,I64 n)
{
  I64 i;
  CDoc *doc=DocNew(filename);
  DocPrint(doc,"//This is unsorted.\n");
  for (i=0;i<n;i++)
    DocPrint(doc,"%12.6f\n",Rand);
  DocWrite(doc);
  DocDel(doc);
}
F64FileGenTXT("UnsortedF64.TXT.Z",10);

U0 F64FileGenDAT(U8 *filename,I64 n)
{//[File/CFile,1] File/CFile, [C:/Demo/Dsk/DataBase.CPP.Z,1]
  I64 i;
  F64 *a=MAlloc(n*sizeof(F64));
  for (i=0;i<n;i++)
    a[i]=Rand;
  FileWrite(filename,a,n*sizeof(F64));
  Free(a);
}
F64FileGenDAT("UnsortedF64.DAT.Z",10);
