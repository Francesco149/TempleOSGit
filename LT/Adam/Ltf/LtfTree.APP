#help_index "Ltf"

public BoolI64 LtfTreeFind(Ltf *l,U8 *path,
  LtfEntry **tree_entry=NULL,
  LtfEntry **start_indent=NULL, LtfEntry **end_indent=NULL)
{
  BoolI8 result=FALSE;
  I64 i=0,k=0;
  U8 *st1=StrNew(path),*st2=MAlloc(StrLen(path)+1);
  BoolI8 unlock_l=LtfLock(l);
  LtfEntry *ll=l->root.next;
  if (tree_entry) *tree_entry=l;
  if (start_indent) *start_indent=l;
  if (end_indent) *end_indent=l;
  while (*st1 && ll!=l) {
    StrFirstRem(st1,"/",st2);
    if (*st2) {
      while (ll!=l) {
	if (ll->type_u8==LTFT_INDENT)
	  i+=ll->attr;
	else if (i==k && ll->flags&LTFLF_TREE && !StrCmp(ll->tag+3,st2)) {
	   if (*st1)
	     break;
	   else {
	     if (tree_entry) *tree_entry=ll;
	     i=0;
	     while (ll!=l && ll->type_u8!=LTFT_INDENT)
	       ll=ll->next;
	     if (ll!=l) {
	       i=ll->attr;
	       if (start_indent) *start_indent=ll;
	       ll=ll->next;
	       while (ll!=l && i>0) {
		 if (ll->type_u8==LTFT_INDENT) {
		   i+=ll->attr;
		   if (i<=0) {
		     if (end_indent) *end_indent=ll;
		     result=TRUE;
		     break;
		   }
		 }
		 ll=ll->next;
	       }
	     }
	     goto ft_done;
	   }
	}
	ll=ll->next;
      }
      k+=2;
    }
  }
ft_done:
  if (unlock_l)
    LtfUnlock(l);
  Free(st1);
  Free(st2);
  return result;
}


public BoolI64 LtfTreeMake(Ltf *l,U8 *path)
{
  BoolI8 result=TRUE;
  I64 i=0,j=MIN_I64,k=0;
  U8 *st1=StrNew(path),
     *st2=MAlloc(StrLen(path)+1),
     *st3=StrNew(path);
  BoolI8 unlock_l=LtfLock(l);
  LtfEntry *ll=l->root.next;
  l->cur_entry=l;
  l->cur_data_col=0;
  while (*st1 && ll!=l) {
    StrFirstRem(st1,"/",st2);
    if (*st2) {
      while (ll!=l) {
	if (ll->type_u8==LTFT_INDENT) {
	  i+=ll->attr;
	  if (i==j) {
	    l->cur_entry=ll;
	    l->cur_data_col=0;
	    goto mt_done;
	  }
	} else if (i==k && ll->flags&LTFLF_TREE && !StrCmp(ll->tag+3,st2)) {
	   Free(st3);
	   st3=StrNew(st1);
	   j=i;
	   if (!*st1)
	     result=FALSE;
	   else
	     break;
	}
	ll=ll->next;
      }
      k+=2;
    }
  }
mt_done:
  if (result) {
    while (*st3) {
      StrFirstRem(st3,"/",st2);
      if (*st2) {
	LtfPrintF(l,"$TR+C,\"%s\"$\n$ID,2$",st2);
	l->cur_entry=LtfPutS(l,"$ID,-2$");
	l->cur_data_col=0;
      }
    }
  }
  if (unlock_l)
    LtfUnlock(l);
  Free(st1);
  Free(st2);
  Free(st3);
  return result;
}

#help_index "Ltf/Compiler;Compiler"
public I64 ExeLtf(Ltf *l,BoolI8 just_load=FALSE)
{
  I64 result;
  BoolI8 okay=TRUE;
  LexStruct *lx;
  BoolI8 unlock_l=LtfLock(l);
  lx=LexNew("",LF_DONT_FREE_BUF,FileNameAbs(sys_temp_filename));
  if (just_load)
    lx->flags|=LF_JUST_LOAD;
  if (Fs->last_lex!=&Fs->next_lex)
    lx->opts=Fs->last_lex->opts;
  InsQue(lx,Fs->last_lex);
  LexIncludeLtf(lx,NULL,l);
  try {
    Lex(lx);
    result=ExeCmdLine(lx);
  } catch {
    if (Fs->except_argc>0) {
      if (Fs->except_argv[0]==EXCEPT_COMPILER ||
      Fs->except_argv[0]==EXCEPT_BREAK) {
	Fs->catch_except=TRUE;
	okay=FALSE;
	result=0;
      }
    }
  }
  RemQue(lx);
  if (okay)
    LexDel(lx); //TODO: can crash
  if (unlock_l)
    LtfUnlock(l);
  return result;
}


public I64 LtfTreeBranchExe(Ltf *l,U8 *path)
{
  I64 result=0;
  Ltf *b;
  BoolI8 unlock_l=LtfLock(l);
  LtfEntry *tree_branch,*start_indent,*end_indent;
  if (LtfTreeFind(l,path,&tree_branch,&start_indent,&end_indent)) {
    b=LtfCopy(l,tree_branch,end_indent);
    result=ExeLtf(b);
    LtfDel(b);
  }
  if (unlock_l)
    LtfUnlock(l);
  return result;
}

public I64 ExeTreeBranch(U8 *registry_file,U8 *path)
{
  I64 result=0;
  Ltf *l=LtfRead(registry_file);
  result=LtfTreeBranchExe(l,path);
  LtfDel(l);
  return result;
}

#help_index "Ltf"
