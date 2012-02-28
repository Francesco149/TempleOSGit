#help_index "Utils;Cmd Line (Typically)"

#define DS_USE_FILE1	0
#define DS_USE_FILE2	1
#define DS_REMAINDER_1	2
#define DS_REMAINDER_2	3
#define DS_ABORT_FILE	4

I64 PopUpDiffOptions()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$CM+LX,2,4$$FG$$BT,\"USE FILE1\",DS_USE_FILE1$"
	"$CM+LX,24,0$$FG,CYAN$$BT,\"USE FILE2\",DS_USE_FILE2$"
	"$CM+LX,2,4$$FG$$BT,\"REMAINDER ALL FILE1\",DS_REMAINDER_1$"
	"$CM+LX,24,0$$FG,CYAN$$BT,\"REMAINDER ALL FILE2\",DS_REMAINDER_2$"
	"$CM+LX,2,4$$FG$$BT,\"ABORT FILE\",DS_ABORT_FILE$"
	"$CM+LX,24,0$$FG$$BT,\"ABORT ALL FILES\",LTFM_CANCEL$\n");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

I64 DiffEntriesCompare(LtfEntry *ll1,LtfEntry *ll2)
{
  return StrCmp(ll1->tag,ll2->tag);
}

#define DF_MODIFIED			1
#define DF_DONT_MODIFIED		2
#define DF_REMAINDER_ALL_FILE1		4
#define DF_REMAINDER_ALL_FILE2		8
#define DF_ABORT_FILE			16
#define DF_ABORT_ALL_FILES		32
#define DF_NO_MORE_PROMPTS_THIS_FILE	64

U0 DiffSelect(Ltf *l,U64 *_df_flags,I64 j1_lo,I64 j1_hi,
		 I64 j2_lo,I64 j2_hi,LtfEntry **usl1,LtfEntry **usl2)
{
  I64 i;
  LtfEntry *cl,*cl1,*cl2;
  BoolI8 use_file1;
  U64 old_flags;
  Ltf *cur_l;
  if (!(*_df_flags & (DF_ABORT_FILE|DF_ABORT_ALL_FILES))) {
    "$FG,RED$%d,%d---------------------$FG$\n",j1_lo,j2_lo;
    i= j1_lo<=0 ? 0:j1_lo-1;
    while (i<j1_hi) {
      if (cur_l=LtfCur) {
	old_flags=cur_l->flags&LTFF_PLAIN_TEXT;
	cur_l->flags|=LTFF_PLAIN_TEXT;
      }
      "" usl1[i++]->tag;
      if (cur_l)
	cur_l->flags= cur_l->flags&~LTFF_PLAIN_TEXT |old_flags;
      NewLine;
    }
    "$FG,CYAN$";
    i= j2_lo<=0 ? 0:j2_lo-1;
    while (i<j2_hi) {
      if (cur_l=LtfCur) {
	old_flags=cur_l->flags&LTFF_PLAIN_TEXT;
	cur_l->flags|=LTFF_PLAIN_TEXT;
      }
      "" usl2[i++]->tag;
      if (cur_l)
	cur_l->flags= cur_l->flags&~LTFF_PLAIN_TEXT |old_flags;
      NewLine;
    }
    "$FG$";

    use_file1=TRUE;
    if (!(*_df_flags & DF_NO_MORE_PROMPTS_THIS_FILE)) {
      switch (PopUpDiffOptions) {
	case DS_USE_FILE1:
	  break;
	case DS_USE_FILE2:
	  use_file1=FALSE;
	  break;
	case DS_REMAINDER_1:
	  *_df_flags=*_df_flags&~DF_REMAINDER_ALL_FILE2|DF_REMAINDER_ALL_FILE1|DF_NO_MORE_PROMPTS_THIS_FILE;
	  break;
	case DS_REMAINDER_2:
	  *_df_flags=*_df_flags&~DF_REMAINDER_ALL_FILE1|DF_REMAINDER_ALL_FILE2|DF_NO_MORE_PROMPTS_THIS_FILE;
	  break;
	case DS_ABORT_FILE:
	  *_df_flags|=DF_DONT_MODIFIED|DF_ABORT_FILE|DF_NO_MORE_PROMPTS_THIS_FILE;
	  break;
	default:
	  *_df_flags|=DF_DONT_MODIFIED|DF_ABORT_ALL_FILES|DF_NO_MORE_PROMPTS_THIS_FILE;
      }
    }
    if (*_df_flags & DF_REMAINDER_ALL_FILE2 &&
	!(*_df_flags & (DF_DONT_MODIFIED|DF_REMAINDER_ALL_FILE1)))
      use_file1=FALSE;
    if (!use_file1) {
      *_df_flags|=DF_MODIFIED;
      cl1=usl1[j1_lo]->last;
      if (j1_lo<j1_hi) {
	cl=usl1[j1_lo];
	while (cl!=usl1[j1_hi]) {
	  cl2=cl->next;
	  LtfEntryDel(l,cl);
	  cl=cl2;
	}
      }
      if (j2_lo<j2_hi) {
	cl=usl2[j2_lo];
	while (cl!=usl2[j2_hi]) {
	  cl2=LtfEntryCopy(l,cl);
	  InsQue(cl2,cl1);
	  cl1=cl2;
	  cl=cl->next;
	}
      }
    }
  }
}

U0 DiffSub(Ltf *l,U64 *_df_flags,I64 j1_lo,I64 j1_hi,I64 j2_lo,I64 j2_hi,
  I64 cnt1,I64 cnt2,LtfEntry **sl1,LtfEntry **sl2,LtfEntry **usl1,LtfEntry **usl2)
{
  I64 i,i1=0,i2=0,i2b,j1,j2,n;
  I64 best_j1,best_j2,best_score=0,score;

  if (j1_lo>=j1_hi || j2_lo>=j2_hi) {
    if (j1_lo<j1_hi || j2_lo<j2_hi)
      DiffSelect(l,_df_flags,j1_lo,j1_hi,j2_lo,j2_hi,usl1,usl2);
    return;
  }

  //Locate longest matching str in intervals
  while (i1<cnt1 && i2<cnt2) {
    if (sl1[i1]->y<j1_lo || sl1[i1]->y>=j1_hi)
      i1++;
    else if (sl2[i2]->y<j2_lo || sl2[i2]->y>=j2_hi)
      i2++;
    else {
      i=StrCmp(sl1[i1]->tag,sl2[i2]->tag);
      if (i>0)
	i2++;
      else if (i<0)
	i1++;
      else {
	i2b=i2;
	while (!StrCmp(sl1[i1]->tag,sl2[i2]->tag)) {
	  if (sl2[i2]->y>=j2_lo && sl2[i2]->y<j2_hi) {
	    score=0;
	    j1=sl1[i1]->y;
	    j2=sl2[i2]->y;
	    n=j1_hi-j1;
	    if (j2_hi-j2<n)
	      n=j2_hi-j2;
	    while (score<n) {
	      if (!StrCmp(usl1[j1+score]->tag,usl2[j2+score]->tag))
		score++;
	      else
		break;
	    }
	    if (score>best_score) {
	      best_score=score;
	      best_j1=j1;
	      best_j2=j2;
	    }
	  }
	  i2++;
	  if (i2>=cnt2)
	    break;
	}
	i2=i2b;
	i1++;
      }
    }
  }
  if (!best_score)
    DiffSelect(l,_df_flags,j1_lo,j1_hi,j2_lo,j2_hi,usl1,usl2);
  else {
    DiffSub(l,_df_flags,j1_lo,best_j1,j2_lo,best_j2,cnt1,cnt2,sl1,sl2,usl1,usl2);
    DiffSub(l,_df_flags,best_j1+best_score,j1_hi,best_j2+best_score,j2_hi,cnt1,cnt2,
      sl1,sl2,usl1,usl2);
  }
}

BoolI64 DiffBins(Ltf *l1,Ltf *l2)
{
  LtfBinEntry *tempb1=l1->bin_root.next,
	      *tempb2=l2->bin_root.next;

  if (tempb1->last->last->num!=
      tempb2->last->last->num)
    return FALSE;
  while (tempb1!=&l1->bin_root) {
    if (tempb1->type!=tempb2->type ||
	tempb1->size!=tempb2->size ||
	MemCmp(tempb1->data,tempb2->data,tempb1->size))
      return FALSE;
    tempb1=tempb1->next;
    tempb2=tempb2->next;
  }
  return TRUE;
}

public U64 Diff(U8 *dst_file,U8 *src_file,U64 df_flags=0)
{ //The df_flags arg is used when this is called by [C:/LT/Adam/Merge.APZ,40] MergeChk().
  Ltf *l1=LtfRead(dst_file, LTFF_PLAIN_TEXT_WITH_TABS | LTFF_NO_CURSOR);
  Ltf *l2=LtfRead(src_file, LTFF_PLAIN_TEXT_WITH_TABS | LTFF_NO_CURSOR);
  LtfEntry *cl,**sl1,**sl2,**usl1,**usl2;
  I64 i,cnt1=0,cnt2=0;

  df_flags&=DF_ABORT_ALL_FILES;

  cl=l1->root.next;
  while (cl!=l1) {
    if (cl->type_u8==LTFT_TEXT)
      cl->y=cnt1++;
    cl=cl->next;
  }

  cl=l2->root.next;
  while (cl!=l2) {
    if (cl->type_u8==LTFT_TEXT)
      cl->y=cnt2++;
    cl=cl->next;
  }

  sl1=MAlloc(cnt1*sizeof(LtfEntry *));
  usl1=MAlloc((cnt1+1)*sizeof(LtfEntry *));
  i=0;
  cl=l1->root.next;
  while (cl!=l1) {
    if (cl->type_u8==LTFT_TEXT) {
      sl1[i]=cl;
      usl1[i++]=cl;
    }
    cl=cl->next;
  }
  usl1[i]=l1;
  QSortU64(sl1,cnt1,&DiffEntriesCompare);

  sl2=MAlloc(cnt2*sizeof(LtfEntry *));
  usl2=MAlloc((cnt2+1)*sizeof(LtfEntry *));
  i=0;
  cl=l2->root.next;
  while (cl!=l2) {
    if (cl->type_u8==LTFT_TEXT) {
      sl2[i]=cl;
      usl2[i++]=cl;
    }
    cl=cl->next;
  }
  usl2[i]=l2;
  QSortU64(sl2,cnt2,&DiffEntriesCompare);

  DiffSub(l1,&df_flags,0,cnt1,0,cnt2,cnt1,cnt2,sl1,sl2,usl1,usl2);
  if (df_flags&DF_MODIFIED && !(df_flags&DF_DONT_MODIFIED))
    LtfWrite(l1);

  if (!DiffBins(l1,l2))
    "$FG,RED$Bin Data is Different$FG$\n";

  LtfDel(l1);
  LtfDel(l2);
  Free(sl1);
  Free(sl2);
  Free(usl1);
  Free(usl2);
  return df_flags;
}
#help_index ""
