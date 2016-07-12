#help_index "Debugging/IDE"

#define IDET_GO			1
#define IDET_STEP_INTO		2
#define IDET_STEP_OVER		3

class CIDE
{
  I64 type;
  U8 *cur_line,*next_line;
  U8 *call_bpt;
  Bool existing_bpt;
};

U0 IDEWaitParent()
{
  CTask *task=Fs->parent_task;
  while (!IsSuspended(task) &&
	!Bt(&task->task_flags,TASKf_IDLE) &&
	!Bt(&task->rflags,RFLAGf_TRAP))
    Yield;
}

public CHashFun *DbgFunFind(CDoc *doc,I64 line_offset=0)
{
  U8 st[256],*filename=doc->filename.name;
  I64 i,line=doc->cur_entry->y+1+line_offset;
  CHashTable *h=Fs->hash_table;
  CHashSrcSym *temph;
  CDbgInfo *dbg_info;
  while (h) {
    for (i=0;i<=h->mask;i++) {
      temph=h->body[i];
      while (temph) {
	if (temph->type&(HTT_FUN|HTT_EXPORT_SYS_SYM))
	  dbg_info=temph->dbg_info;
	else
	  dbg_info=NULL;
	if (dbg_info) {
	  StrCpy(st,temph->src_link);
	  StrFirstRem(st,":");
	  StrLastRem(st,",");
	  if (!StrCmp(st,filename)) {
	    if (dbg_info->min_line<=line<=dbg_info->max_line)
	      return temph;
	  }
	}
	temph=temph->next;
      }
    }
    h=h->next;
  }
  return NULL;
}

U0 WatchLocals()
{
  CTask *task=Fs->parent_task,*dbg_task;
  CDoc *doc;
  CHashFun *tempf,*tempf1;
  I64 sc,ch,offset,ip;
  F64 timeout;
  U8 **rbp,**ptr;
  while (TRUE) {
    IDEWaitParent;
    if (!TaskValidate(dbg_task=task->dbg_task))
      Exit;
    if (ScanKey(&ch,&sc,TRUE)) {
      timeout=tS+4.0;
      do {
	if (ch==CH_SHIFT_ESC || ch==CH_ESC)
	  Exit;
	if (ScanKey(&ch,&sc,TRUE))
	  timeout=tS+4.0;
	WinMgrSync;
      } while (tS<timeout);
    } else {
      IDEWaitParent;
      if (!TaskValidate(dbg_task=task->dbg_task))
	Exit;
      DocDblBufStart;
      if (doc=DocPut(dbg_task)) {
	if (tempf=DbgFunFind(doc,0)) {
	  "$WW,1$";
	  rbp=task->rbp;
	  ip=task->rip;
	  while (TRUE) {
	    tempf1=FunSegFind(ip,&offset);
	    break; //TODO: load reg vars for higher level subroutines
	    if (tempf==tempf1)
	      break;
	    else {
	      if (rbp>=*rbp)
		break;
	      ptr=rbp+1;
	      rbp=*rbp;
	      ip=*ptr;
	      if (!ChkOnStk(rbp,task))
		break;
	    }
	  }
	  if (tempf==tempf1)
	    FunRep(tempf->str,rbp,1,task);
	}
      }
      DocDblBufEnd;
      WinMgrSync;
    }
  }
}

public U0 IDE(U8 *filename)
//Spawns a child process which has
//access to parents syms. Can be used
//to debug parent.
{
  CTask *task;
  U8 *fn1=DftExt(filename,"CPP.Z"),
	*fn2=FileNameAbs(fn1);

  if (PopUpCancelOk("$BK,1$$RED$TempleOS IDE Debugger$FG$$BK,0$\n"
	"Do not use this on files that run when included.\n"
	"\nTo use:\n"
	"  1) Set a break-point in the editor window.\n"
	"  2) Run program from cmd line.\n\n"
	"$GREEN$F5$FG$\t\tGo\n"
	"$GREEN$F10$FG$\t\tStep Over\n"
	"$GREEN$F11$FG$\t\tStep Into\n"
	"$GREEN$F9$FG$\t\tToggle Break Point\n"
	"$GREEN$SHIFT-F9$FG$\tClear All Break Points\n"
	"\n\n\nNote: I don't use this debugger because\n"
	"I'm always doing multitasking stuff, \n"
	"kernel stuff and multicore stuff.  Also,\n"
	"you can't debug code in the window\n"
	"mgr callbacks.  It works on really\n"
	"simple stuff, sort-of.\n\n"
	"I recommend making use of:\n"
	"[C:/Kernel/KernelB.HPP.Z,205] progress1-[C:/Kernel/KernelB.HPP.Z,217] progress4,\t\t-- glbl I64 vars shown on wallpaper\n"
	"[C:/Kernel/KDbg.CPP.Z,217] RawPrint(), [C:/Kernel/KDbg.CPP.Z,234] RawD(), [C:/Kernel/KDbg.CPP.Z,250] RawDm()\t-- raw text output for a time\n"
	"[C:/Kernel/KMisc.CPP.Z,185] Beep(), [C:/Kernel/KMisc.CPP.Z,158] Snd()\t\t\t-- sound\n"
	"[C:/Kernel/SrvCmd.CPP.Z,424] AdamLog(), [C:/Kernel/SrvCmd.CPP.Z,434] AdamErr()\t\t-- print to adam task's window\n"
	"[C:/Kernel/KMisc.CPP.Z,228] SysDbg(), [C:/Kernel/KMisc.CPP.Z,233] IsSysDbg()\t\t-- just a flag\n\n")) {
    WinHorz(1,TEXT_COLS>>1-1);
    WinVert(2,TEXT_ROWS-2);
    LBts(&Fs->display_flags,DISPLAYf_CHILDREN_NOT_ON_TOP);

    task=Spawn(&UserCmdLine,NULL,"Debug",,Fs);
    TaskWait(task);
    WinHorz(TEXT_COLS>>1+1,TEXT_COLS-2,task);
    WinVert(12,TEXT_ROWS-2,task);
    XTalk(task,"Ed(\"%s\");\n",fn2);

    Fs->dbg_task=task;

    task=Spawn(&UserCmdLine,NULL,"Watch",,Fs);
    TaskWait(task);
    WinHorz(TEXT_COLS>>1+1,TEXT_COLS-2,task);
    WinVert(2,10,task);
    XTalk(task,"WatchLocals;\n");
    ExeFile(fn2);
    Free(fn1);
    Free(fn2);
  }
}

I64 IDELineAdd(CDbgInfo *dbg_info,I64 line)
{
  I64 max=dbg_info->max_line-dbg_info->min_line;
  line-=dbg_info->min_line;
  if (line<0)
    return IDELineAdd(dbg_info,dbg_info->min_line);
  while (!dbg_info->body[line] && line<=max)
    line++;
  return dbg_info->body[line];
}

I64 IDERIPLineNum(CDoc *doc,U8 *rip)
{
  U8 st[256],*filename=doc->filename.name;
  I64 i,line;
  CHashTable *h=Fs->hash_table;
  CHashFun *tempf;
  CDbgInfo *dbg_info;
  while (h) {
    for (i=0;i<=h->mask;i++) {
      tempf=h->body[i];
      while (tempf) {
	if (tempf->type&HTT_FUN) {
	  if (dbg_info=tempf->dbg_info) {
	    StrCpy(st,tempf->src_link);
	    StrFirstRem(st,":");
	    StrLastRem(st,",");
	    if (!StrCmp(st,filename)) {
	      for (line=dbg_info->min_line;line<=dbg_info->max_line;line++)
		if (rip>=IDELineAdd(dbg_info,line) &&
		      rip<IDELineAdd(dbg_info,line+1))
		  return line;
	    }
	  }
	}
	tempf=tempf->next;
      }
    }
    h=h->next;
  }
  return -1;
}

U0 IDESetStepRange(CDoc *doc)
{
  U8 st[256],*filename=doc->filename.name;
  I64 i,line=doc->cur_entry->y+1;
  CHashTable *h=Fs->hash_table;
  CHashFun *tempf;
  CIDE *tempi=FramePtr("IDEFrame");
  CDbgInfo *dbg_info;
  while (h) {
    for (i=0;i<=h->mask;i++) {
      tempf=h->body[i];
      while (tempf) {
	if (tempf->type&HTT_FUN) {
	  if (dbg_info=tempf->dbg_info) {
	    StrCpy(st,tempf->src_link);
	    StrFirstRem(st,":");
	    StrLastRem(st,",");
	    if (!StrCmp(st,filename)) {
	      if (dbg_info->min_line<=line<=dbg_info->max_line) {
		tempi->cur_line =IDELineAdd(dbg_info,line);
		tempi->next_line=IDELineAdd(dbg_info,line+1);
		return;
	      }
	    }
	  }
	}
	tempf=tempf->next;
      }
    }
    h=h->next;
  }
}

U0 IDEToggleBpt(CDoc *doc)
{ //F9
  U8 st[512],*filename=doc->filename.name;
  I64 i,a,line=doc->cur_entry->y+1;
  Bool unlock;
  CHashTable *h=Fs->hash_table;
  CHashFun *tempf;
  CDbgInfo *dbg_info;
  CDocEntry *doc_e;
  while (h) {
    for (i=0;i<=h->mask;h++) {
      tempf=h->body[i];
      while (tempf) {
	if (tempf->type&HTT_FUN) {
	  if (dbg_info=tempf->dbg_info) {
	    StrCpy(st,tempf->src_link);
	    StrFirstRem(st,":");
	    StrLastRem(st,",");
	    if (!StrCmp(st,filename)) {
	      if (dbg_info->min_line<=line<=dbg_info->max_line) {
		a=IDELineAdd(dbg_info,line);
		unlock=DocLock(doc);
		doc_e=doc->cur_entry;
		while (doc_e->last->type_u8!=DOCT_NEW_LINE &&
		      doc_e->last->type_u8!=DOCT_SOFT_NEW_LINE &&
		      doc_e->last!=doc)
		  doc_e=doc_e->last;
		doc->cur_entry=doc_e;
		doc->cur_col=doc_e->min_col;
		if (!BptR(a,Fs->parent_task)) {
		  BptS(a,Fs->parent_task);
		  DocPrint(doc,"$BP$");
		} else {
		  if (doc_e->type_u8==DOCT_BPT) {
		    doc->cur_entry=doc_e->next;
		    doc->cur_col=doc->cur_entry->min_col;
		    DocEntryDel(doc,doc_e);
		  }
		}
		if (unlock)
		  DocUnlock(doc);
		DocLineNumGoTo(doc,line);
		return;
	      }
	    }
	  }
	}
	tempf=tempf->next;
      }
    }
    h=h->next;
  }
}

U0 IDEClearAllBpts(CDoc *doc)
{ //SHIFT-F9
  CDocEntry *doc_e,*doc_e2;
  Bool unlock;
  IDEWaitParent;
  B2(Fs->parent_task);
  unlock=DocLock(doc);
  doc_e=doc->head.next;
  while (doc_e!=doc) {
    doc_e2=doc_e->next;
    if (doc_e->type_u8==DOCT_BPT) {
      if (doc->cur_entry==doc_e) {
	doc->cur_entry=doc_e->next;
	doc->cur_col=doc->cur_entry->min_col;
      }
      DocEntryDel(doc,doc_e);
    }
    doc_e=doc_e2;
  }
  if (unlock)
    DocUnlock(doc);
}

U0 IDEGo()  //Continue after breakpoint
{ //F5
  CTask *task=Fs->parent_task;
  CIDE *tempi=FramePtr("IDEFrame");
  if (!tempi) {
    tempi=CAlloc(sizeof(CIDE));
    FramePtrAdd("IDEFrame",tempi);
  }
  IDEWaitParent;
  LBts(&task->task_flags,TASKf_DISABLE_BPTS);
  tempi->cur_line=task->rip;
  tempi->type=IDET_GO;
  LBts(&task->rflags,RFLAGf_TRAP);
  Suspend(task,FALSE);
}

U0 IDEStepOver(CDoc *doc)
{ //F10
  CTask *task=Fs->parent_task;
  I64 line;
  U8 *opcode;

  CIDE *tempi=FramePtr("IDEFrame");
  if (!tempi) {
    tempi=CAlloc(sizeof(CIDE));
    FramePtrAdd("IDEFrame",tempi);
  }

  IDEWaitParent;
  line=IDERIPLineNum(doc,task->rip);
  LBts(&task->task_flags,TASKf_DISABLE_BPTS);
  if (line>0) {
    DocLineNumGoTo(doc,line);
    IDESetStepRange(doc);
  } else {
    tempi->cur_line=NULL;
    tempi->next_line=NULL;
  }
  tempi->type=IDET_STEP_OVER;
  tempi->call_bpt=NULL;

  LBts(&task->rflags,RFLAGf_TRAP);
  Suspend(task,FALSE);

  opcode=task->rip;
  if (*opcode==OC_CALL) {
    if (BptS(opcode+5,task)) {
      if (tempi->call_bpt!=opcode+5)
	tempi->existing_bpt=TRUE;
    } else
      tempi->existing_bpt=FALSE;
    tempi->call_bpt=opcode+5;
    LBtr(&task->task_flags,TASKf_DISABLE_BPTS);
    LBtr(&task->rflags,RFLAGf_TRAP);
  }
}

U0 IDEStepInto(CDoc *doc)
{ //F11
  CTask *task=Fs->parent_task;
  I64 line;

  CIDE *tempi=FramePtr("IDEFrame");
  if (!tempi) {
    tempi=CAlloc(sizeof(CIDE));
    FramePtrAdd("IDEFrame",tempi);
  }

  IDEWaitParent;
  line=IDERIPLineNum(doc,task->rip);
  LBts(&task->task_flags,TASKf_DISABLE_BPTS);
  if (line>0) {
    DocLineNumGoTo(doc,line);
    IDESetStepRange(doc);
  } else {
    tempi->cur_line=NULL;
    tempi->next_line=NULL;
  }
  tempi->type=IDET_STEP_INTO;
  LBts(&task->rflags,RFLAGf_TRAP);
  Suspend(task,FALSE);
}

U0 IDEEdTask(I64)
{
  CTask *task=Fs->parent_task,
	*ed_task=NULL,*dbg_task;
  U8 *rip=task->rip,
	*st,*rip_filename,*bpt_filename;
  CIDE *tempi,*tempi1;

  CDoc *doc;
  CBpt *tempb;
  if (rip_filename=SrcFileName(rip)) {
    ed_task=Spawn(&UserCmdLine,NULL,"Debug",,task);
    TaskWait(ed_task);
    st=MPrint("Ed(\"%s\");\n",rip_filename);
    XTalkWait(ed_task,st);
    Free(st);
    doc=DocPut(ed_task);
    if (TaskValidate(dbg_task=task->dbg_task)) {
      WinHorz(dbg_task->win_left,dbg_task->win_right, ed_task);
      WinVert(dbg_task->win_top, dbg_task->win_bottom,ed_task);
      if (tempi=FramePtr("IDEFrame",dbg_task)) {
	tempi1=MAllocIdent(tempi,ed_task);
	FramePtrAdd("IDEFrame",tempi1);
      }
      Kill(dbg_task);
    }
    task->dbg_task=ed_task;
    tempb=task->bpt_lst;
    while (tempb) {
      if (bpt_filename=SrcFileName(tempb->addr)) {
	if (!StrCmp(rip_filename,bpt_filename)) {
	  DocLineNumGoTo(doc,SrcLineNum(tempb->addr));
	  DocPrint(doc,"$BP$");
	}
	Free(bpt_filename);
      }
      tempb=tempb->next;
    }
    DocLineNumGoTo(doc,SrcLineNum(rip));
    Free(rip_filename);
    StrCpy(ed_task->task_title,"Debug Task");
  }
}

U0 IDEDbgResume()
{
  CTask *dbg_task=Fs->dbg_task;
  CIDE *tempi=FramePtr("IDEFrame",dbg_task);
  U8 *opcode;
  if (!tempi) {
    Spawn(&IDEEdTask,NULL,"Dbg Resume",,Fs);
    Suspend(Fs);
  } else
    switch (tempi->type) {
      case IDET_GO:
	if (tempi->cur_line) {
	  LBtr(&Fs->task_flags,TASKf_DISABLE_BPTS);
	  LBtr(&Fs->rflags,RFLAGf_TRAP);
	  if (Fs->rip==tempi->cur_line) {//Jmp to self
	    tempi->cur_line=NULL;
	    Spawn(&IDEEdTask,NULL,"Dbg Resume",,Fs);
	    Suspend(Fs);
	  } else
	    tempi->cur_line=NULL;
	} else {
	  Spawn(&IDEEdTask,NULL,"Dbg Resume",,Fs);
	  Suspend(Fs);
	}
	break;
      case IDET_STEP_INTO:
	if (!tempi->cur_line ||
	      Fs->rip<tempi->cur_line || Fs->rip>=tempi->next_line) {
	  Spawn(&IDEEdTask,NULL,"Dbg Resume",,Fs);
	  Suspend(Fs);
	} else
	  Suspend(Fs,FALSE);
	break;
      case IDET_STEP_OVER:
	LBts(&Fs->rflags,RFLAGf_TRAP);
	LBts(&Fs->task_flags,TASKf_DISABLE_BPTS);
	if (Fs->rip==tempi->call_bpt) {
	  if (!tempi->existing_bpt)
	    BptR(Fs->rip,Fs,FALSE);
	  tempi->call_bpt=NULL;
	}
	if (!tempi->cur_line ||
	      Fs->rip<tempi->cur_line || Fs->rip>=tempi->next_line) {
	  Spawn(&IDEEdTask,NULL,"Dbg Resume",,Fs);
	  Suspend(Fs);
	} else {
	  opcode=Fs->rip;
	  if (*opcode==OC_CALL) {
	    if (BptS(opcode+5,Fs,FALSE)) {
	      if (tempi->call_bpt!=opcode+5)
		tempi->existing_bpt=TRUE;
	    } else
	      tempi->existing_bpt=FALSE;
	    tempi->call_bpt=opcode+5;
	    LBtr(&Fs->task_flags,TASKf_DISABLE_BPTS);
	    LBtr(&Fs->rflags,RFLAGf_TRAP);
	  }
	  Suspend(Fs,FALSE);
	}
	break;
    }
}

ext[EXT_DBG_RESUME]=&IDEDbgResume;

#help_index ""
