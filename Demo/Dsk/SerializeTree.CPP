/*You probably don't need this.  It makes a directory
tree into a flat contiguous memory chunk, suitable for
serial communication and rebuilds it into a tree on the
other end.
*/

U0 SerializeTree()
{
  CDirEntry *tempde=FilesFind("/Demo/*.TXT*",FUF_RECURSE);
  I64 size;
  U8 *ptr;
  CArcCompress *ac;

  LineRep1(tempde); //We have [C:/Kernel/Adam1a.HPP.Z,2450] CDirEntry.fullname
  LineRep2(DocPut,tempde);
  "$GREEN$***************$FG$\n";
  ptr=DirTreeSerialize(tempde,&size);
  DirTreeDel(tempde);
  D(ptr,size);
  "$GREEN$***************:%7d$FG$\n",size;
  ac=CompressBuf(ptr,size);
  Free(ptr);
  D(ac,ac->compressed_size);
  "$GREEN$***************:%7d$FG$\n",size;
  ptr=ExpandBuf(ac);
  size=ac->expanded_size;
  Free(ac);
  D(ptr,size);
  "$GREEN$***************:%7d$FG$\n",size;
  tempde=DirTreeUnserialize(ptr);
  LineRep2(DocPut,tempde); //We don't have [C:/Kernel/Adam1a.HPP.Z,2450] CDirEntry.fullname
  DirTreeDel(tempde);
}

SerializeTree;
