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
  CArcCompress *arc;

  LineRep1(tempde); //We have [C:/Kernel/KernelA.HPP.Z,2461] CDirEntry.fullname
  LineRep2(DocPut,tempde);
  "$GREEN$***************$FG$\n";
  ptr=DirTreeSerialize(tempde,&size);
  DirTreeDel(tempde);
  D(ptr,size);
  "$GREEN$***************:%7d$FG$\n",size;
  arc=CompressBuf(ptr,size);
  Free(ptr);
  D(arc,arc->compressed_size);
  "$GREEN$***************:%7d$FG$\n",size;
  ptr=ExpandBuf(arc);
  size=arc->expanded_size;
  Free(arc);
  D(ptr,size);
  "$GREEN$***************:%7d$FG$\n",size;
  tempde=DirTreeUnserialize(ptr);
  LineRep2(DocPut,tempde); //We don't have [C:/Kernel/KernelA.HPP.Z,2461] CDirEntry.fullname
  DirTreeDel(tempde);
}

SerializeTree;
