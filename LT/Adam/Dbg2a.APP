#help_index "Debugging"

BoolI64 ClassRep2(Ltf *l,U8 *class_name,U8 *d,I64 depth,U64 max_depth,BoolI8 dynamic,U64 types=HTT_CLASS,I64 offset=0)
{
  U64 i,j;
  MemberListStruct *ml;
  U64 *ptr;
  LtfEntry *ll;
  BoolI8 unlock;
  ClassStruct *tempc,*tempc2;
  if (depth>=max_depth) return TRUE;
  if (!(tempc=HashFind(class_name,Fs->hash_table,types))) {
    LtfPutS(l,"Class Not Found.\n");
    return FALSE;
  }
  if (!ChkPtr(d) || !ChkPtr(d(U8 *)+tempc->size)) {
    LtfPrintF(l,"Bad Ptr:%016X\n",d);
    return FALSE;
  }
  if (tempc->base_class && !ClassRep2(l,tempc->base_class->str,d,depth,max_depth,dynamic,types,offset))
    return FALSE;
  unlock=LtfLock(l);
  LtfPrintF(l,"Class:\"%s\"\n",class_name);
  ml=tempc->member_list;
  while (ml) {
    tempc2=ml->member_class;
    ptr=d(U8 *)+ml->offset;
    LtfPrintF(l,"%08X ",ptr(U8 *)+offset);
    j=tempc2->ptr_cnt;
    while (tempc2->fwd_class)
      tempc2=tempc2->fwd_class;
    tempc2-=tempc2->ptr_cnt;
    if (tempc2->type & HTT_INTERNAL_TYPE) {
      LtfPrintF(l,"$FG,GREEN$%-20ts:$FG$",ml->str);
      if (j==1 && (tempc2->sub_type==IT_I8 || tempc2->sub_type==IT_U8)) {
	ptr=*ptr;
	if (ChkPtr(ptr)) {
	  if (dynamic) {
	    ll=LtfPutS(l,"$DA-T-P+RD,64,\"%40ts\"$\n");
	    ll->data=ptr;
	    LtfDataFmt(ll);
	  } else
	    LtfPrintF(l,"%40ts\n",ptr);
	}
      } else if (!j) {
	j=MinI64(ml->dim.total_cnt,32);
	if (tempc2->sub_type==IT_I8 || tempc2->sub_type==IT_U8) {
	  if (j==1) {
	    if (dynamic) {
	      ll=LtfPutS(l,"$DA-T+RD+UD-U64+U8,32,\"%c\"$\n");
	      ll->data=ptr;
	    } else
	      LtfPrintF(l,"%c\n",*ptr(U8 *));
	  } else {
	    if (dynamic) {
	      ll=LtfPutS(l,"$DA-T-P+RD,64,\"%40ts\"$\n");
	      ll->data=ptr;
	      LtfDataFmt(ll);
	    } else
	      LtfPrintF(l,"%40ts\n",ptr);
	  }
	}
	for (i=0;i<j;i++) {
	  switch (tempc2->sub_type) {
	    case IT_I0:
	    case IT_U0:
	      break;
	    case IT_I8:
	    case IT_U8:
	      if (dynamic) {
		ll=LtfPutS(l,"$DA-T+RD+UD-U64+U8,32,\"%02X\"$ ");
		ll->data=ptr(U8 *)++;
	      } else
		LtfPrintF(l,"%02X ",*ptr(U8 *)++);
	      break;
	    case IT_I16:
	    case IT_U16:
	      if (dynamic) {
		ll=LtfPutS(l,"$DA-T+RD+UD-U64+U16,32,\"%04X\"$ ");
		ll->data=ptr(U16 *)++;
	      } else
		LtfPrintF(l,"%04X ",*ptr(U16 *)++);
	      break;
	    case IT_I32:
	    case IT_U32:
	      if (dynamic) {
		ll=LtfPutS(l,"$DA-T+RD+UD-U64+U32,32,\"%08X\"$ ");
		ll->data=ptr(U32 *)++;
	      } else
		LtfPrintF(l,"%08X ",*ptr(U32 *)++);
	      break;
	    case IT_F64:
	      if (dynamic) {
		ll=LtfPutS(l,"$DA-T+RD+UD,32,\"%16g\"$ ");
		ll->data=ptr(U64 *)++;
	      } else
		LtfPrintF(l,"%16g ",*ptr(U64 *)++);
	      break;
	    default:
	      if (dynamic) {
		ll=LtfPutS(l,"$DA-T+RD+UD,32,\"%016X\"$ ");
		ll->data=ptr(U64 *)++;
	      } else
		LtfPrintF(l,"%016X ",*ptr(U64 *)++);
	  }
	  if (dynamic)
	    LtfDataFmt(ll);
	}
	if (j<ml->dim.total_cnt)
	  LtfPutS(l,"...");
      } else
	LtfPrintF(l,"%016X",*ptr);
      LtfPutS(l,"\n");
    } else {
      if (depth<2) {
	if (j==1 && !ChkPtr(*ptr))
	  LtfPrintF(l,"%-20ts:%016X\n",ml->str,*ptr);
	else {
	  LtfPrintF(l,"$TR,\"%s\"$\n",ml->str);
	  LtfPutS(l,"$ID,2$");
	  if (!j)
	    ClassRep2(l,tempc2->str,ptr,depth+1,max_depth,dynamic);
	  else if (j==1)
	    ClassRep2(l,tempc2->str,*ptr,depth+1,max_depth,dynamic);
	  LtfPutS(l,"$ID,-2$");
	}
      } else
	LtfPrintF(l,"%-20ts\n",ml->str);
    }
    ml=ml->next;
  }
  if (unlock)
    LtfUnlock(l);
  return TRUE;
}

public U0 ClassRep(U8 *class_name,U8 *d,I64 max_depth=2,BoolI8 fun=FALSE,I64 offset=0)
//Displays members of a record
//by using the compiler's info.
{
  Ltf *l;
  BoolI8 old_preempt;
  if (IsRaw) {
    l=LtfNew;
  } else {
    LtfMax;
    old_preempt=Preempt(OFF);
    l=LtfCur;
  }
  if (fun)
    ClassRep2(l,class_name,d,0,max_depth,FALSE,HTT_FUN,offset);
  else
    ClassRep2(l,class_name,d,0,max_depth,FALSE,HTT_CLASS,offset);
  LtfPutS(l,"\n");
  LtfRecalc(l);
  if (IsRaw) {
    LtfDump(l,150000);
    LtfDel(l);
  } else
    Preempt(old_preempt);
}

public U0 ClassRepD(U8 *class_name,U8 *d,I64 max_depth=2,BoolI8 fun=FALSE,I64 offset=0)
//Dynamic classrep (uses hex_ed widgit for live changes)
{
  Ltf *l;
  BoolI8 old_preempt;
  if (IsRaw) {
    l=LtfNew;
  } else {
    LtfMax;
    old_preempt=Preempt(OFF);
    l=LtfCur;
  }
  if (fun)
    ClassRep2(l,class_name,d,0,max_depth,TRUE,HTT_FUN,offset);
  else
    ClassRep2(l,class_name,d,0,max_depth,TRUE,HTT_CLASS,offset);
  LtfPutS(l,"\n");
  LtfRecalc(l);
  if (IsRaw) {
    LtfDump(l,150000);
    LtfDel(l);
  } else
    Preempt(old_preempt);
}

U0 UpdateRegVarImg(FunStruct *tempf,U8 *d,TaskStruct *task)
{
  MemberListStruct *ml;
  ClassStruct *tempc;
  ml=tempf->member_list;
  while (ml) {
    if (ml->register!=CREG_NONE) {
      tempc=ml->member_class;
      while (tempc->fwd_class)
	tempc=tempc->fwd_class;
      MemCpy(d+ml->offset,TaskRegAdd(task,ml->register),tempc->size);
    }
    ml=ml->next;
  }
}

public U0 FunRep(U8 *st,U8 *rbp=NULL,I64 max_depth=2,TaskStruct *task=NULL)
//Shows names and vals of a fun's
//local vars using info the compiler
//has.
{
  I64 size;
  U8 *img;
  FunStruct *tempf=HashFind(st,Fs->hash_table,HTT_FUN);
  MemberListStruct *tempm;
  if (tempf) {
    if (rbp) {
      if (task) {
	//tempf->size is negative.  It's the bottom
	//of the fun local var space relative to RBP .
	size=tempf->arg_cnt*8-tempf->size+16;

	img=MAlloc(size);
	MemCpy(img,rbp+tempf->size,size);
	UpdateRegVarImg(tempf,img-tempf->size,task);
	ClassRep(st,img-tempf->size,max_depth,TRUE,rbp-img+tempf->size);
	Free(img);
      } else
	ClassRep(st,rbp,max_depth,TRUE);
    } else {
      tempm=tempf->member_list;
      while (tempm) {
	if (0<=tempm->register<NUM_REGS)
	  "%08Z %s\n",tempm->register,"ST_U64_REGS",tempm->str;
	else
	  "%08tX %s\n",tempm->offset,tempm->str;
	tempm=tempm->next;
      }
      "%08tX Stk Size\n",tempf->size;
    }
  }
}

public U0 Uf(U8 *st)
{//Unassemble named fun
  I64 i;
  FunStruct *tempf=HashFind(st,Fs->hash_table,HTT_FUN);
  DbgInfo *dbg;
  if (tempf) {
    FunRep(st);
    if (dbg=tempf->debug) {
      i=dbg->body[dbg->max_line-dbg->min_line+1]-tempf->executable_addr;
      Un(tempf->executable_addr,i);
      "Code Size:%04X\n",i;
    } else
      U(tempf->executable_addr);
  }
}

#help_index "Debugging/Breakpoints"
SysBpt *BptFind(TaskStruct *task,U8 *a,BoolI8 rem=FALSE)
{//Rems
  SysBpt *result=NULL,*tempb,*tempb1,*tempb2;
  PushFD;
  Cli;
  tempb1=&task->bpt_list;
  tempb=task->bpt_list;
  while (tempb) {
    tempb2=tempb->next;
    if (tempb->addr==a) {
      result=tempb;
      if (rem)
	tempb1->next=tempb2;
      else
	tempb1=&tempb->next;
    } else
      tempb1=&tempb->next;
    tempb=tempb2;
  }
  PopFD;
  return result;
}

public BoolI64 BptS(U8 *a,TaskStruct *task=NULL,BoolI8 live=TRUE)	//set Bpt
//Sets a breakpoint
//Honestly, I never use this.  I put
//output statements to debug, partly
//because breakpoints don't work well.
{
  BoolI8 result=TRUE;
  SysBpt *tempb;
  if (!task) task=Fs;
  PushFD;
  Cli;
  if (!(tempb=BptFind(task,a,FALSE))) {
    tempb=CAlloc(sizeof(SysBpt),task);
    tempb->addr=a;
    tempb->val=*a;
    result=FALSE;
    tempb->next=task->bpt_list;
    task->bpt_list=tempb;
    if (task==Fs && live)
      *a=OC_BPT;
  }
  PopFD;
  return result;
}

public BoolI64 BptR(U8 *a,TaskStruct *task=NULL,BoolI8 live=TRUE,BoolI8 rem=TRUE)
//Rems one breakpoint
{
  BoolI8 result=FALSE;
  SysBpt *tempb;
  if (!task) task=Fs;
  PushFD;
  Cli;
  if (tempb=BptFind(task,a,rem)) {
    if (task==Fs && live)
      *tempb->addr=tempb->val;
    result=TRUE;
    if (rem)
      Free(tempb);
  }
  PopFD;
  return result;
}

public U0 BptsR(TaskStruct *task=NULL,BoolI8 live=TRUE)	//clear Bpts
//Rems all breakpoints
{
  SysBpt *tempb,*tempb1;
  if (!task) task=Fs;
  PushFD;
  Cli;
  tempb=task->bpt_list;
  task->bpt_list=NULL;
  while (tempb) {
    tempb1=tempb->next;
    if (task==Fs && live)
      *tempb->addr=tempb->val;
    Free(tempb);
    tempb=tempb1;
  }
  PopFD;
}

public U0 Go(TaskStruct *task=NULL)
//Do a [C:/LT/Adam/Dbg2a.APZ,361] Step() first after a Bpt.
{
  if (!task) task=Fs;
  if (BptFind(task,task->rip))
    "!!! Single Step First !!!\n";
  else {
    LBtr(&task->task_flags,TASKf_DISABLE_BPTS);
    LBtr(&task->rflags,RFLAGS_SINGLE_STEP);
    LBtr(&task->task_flags,TASKf_SUSPENDED);
    if (task==Fs)
      DbgExit;
    else
      Exit;
  }
}

public U0 Step(TaskStruct *task=NULL)
//Execute single assembly instruction.
{
  if (!task) task=Fs;
  LBts(&task->task_flags,TASKf_DISABLE_BPTS);
  LBts(&task->rflags,RFLAGS_SINGLE_STEP);
  LBtr(&task->task_flags,TASKf_SUSPENDED);
  if (task==Fs)
    DbgExit;
  else
    Exit;
}

#help_index ""
