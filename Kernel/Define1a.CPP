CHashDefineStr *DefineLoad(U8 *name,U8 *st)
{
  CHashDefineStr *temph=CAlloc(sizeof(CHashDefineStr));
  temph->type=HTT_DEFINE_STR;
  temph->str=StrNew(name);
  temph->data=StrNew(st);
  temph->cnt=-1;
  temph->src_link=MPrint("AD:0x%X",Caller);
  HashAdd(temph,Fs->hash_table);
  return temph;
}

CHashDefineStr *DefineLstLoad(U8 *name,U8 *st)
{ //Not especially efficient but handy.
  I64 cnt=0;
  U8 *ptr,**lst;
  CHashDefineStr *temph=CAlloc(sizeof(CHashDefineStr));
  temph->type=HTT_DEFINE_STR;
  temph->str=StrNew(name);
  temph->src_link=MPrint("AD:0x%X",Caller);
  ptr=st;
  while (*ptr) {
    if (*ptr!='@')
      cnt++;
    while (*ptr++);
  }
  temph->data=MAlloc(ptr+1-st);
  MemCpy(temph->data,st,ptr+1-st);
  temph->cnt=cnt;

  lst=temph->sub_lst=MAlloc(cnt*sizeof(U8 *));
  ptr=st;
  while (*ptr) {
    if (*ptr!='@')
      *lst++=ptr;
    while (*ptr++);
  }

  HashAdd(temph,Fs->hash_table);
  return temph;
}

U0 UndefinedDefine(U8 *name)
{
  ST_ERR_ST "Undefined Define: '%s'.\n",name;
  throw('UndefDef');
}

U8 *Define(U8 *name)
{
  CHashDefineStr *temph;
  if (temph=HashFind(name,Fs->hash_table,HTT_DEFINE_STR))
    return temph->data;
  else if (name)
    UndefinedDefine(name);
  else
    return NULL;
}

U8 *DefineSub(I64 sub,U8 *name)
{
  CHashDefineStr *temph;
  if (temph=HashFind(name,Fs->hash_table,HTT_DEFINE_STR)) {
    if (0<=sub<temph->cnt)
      return temph->sub_lst[sub];
    else
      return NULL;
  } else if (name)
    UndefinedDefine(name);
  else
    return NULL;
}

I64 DefineCnt(U8 *name)
{
  CHashDefineStr *temph;
  if (temph=HashFind(name,Fs->hash_table,HTT_DEFINE_STR))
    return temph->cnt;
  else if (name)
    UndefinedDefine(name);
  else
    return -1;
}

I64 DefineMatch(U8 *pattern,U8 *lst_name,I64 flags=0)
{
  return LstMatch(pattern,Define(lst_name),flags);
}

U0 DefinePrint(U8 *name,U8 *src,...)
{
  U8 *buf=SPrintJoin(NULL,src,argc,argv);
  DefineLoad(name,buf);
  Free(buf);
}
