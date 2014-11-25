#help_index "Memory/Blk Pool"
public I64 UnusedBPMem(CBlkPool *bp)
{//Count of unused bytes in bp.
  I64 result=0,i;
  CMemBlk *tempm;
  if (bp) {
    PUSHFD
    CLI
    while (LBts(&bp->locked_flags,BPlf_LOCKED))
	PAUSE
	  tempm=bp->mem_free_lst;
    while (tempm) {
      if (tempm->mb_signature!=MBS_UNUSED_SIGNATURE_VAL)
	Dbg("Corrupt Sys Memory");
      result+=tempm->pages;
      tempm=tempm->next;
    }
    for (i=0;i<FREE_PAGE_HASH_SIZE;i++) {
      tempm=bp->free_page_hash[i];
      while (tempm) {
	if (tempm->mb_signature!=MBS_UNUSED_SIGNATURE_VAL)
	  Dbg("Corrupt Sys Memory");
	result+=tempm->pages;
	tempm=tempm->next;
      }
    }
    for (i=0;i<64-PAGE_BITS;i++) {
      tempm=bp->free_page_hash2[i];
      while (tempm) {
	if (tempm->mb_signature!=MBS_UNUSED_SIGNATURE_VAL)
	  Dbg("Corrupt Sys Memory");
	result+=tempm->pages;
	tempm=tempm->next;
      }
    }
    LBtr(&bp->locked_flags,BPlf_LOCKED);
    POPFD
  }
  return result*PAGE_SIZE;
}

#help_index "Memory/Heap Ctrl"
I64 MemBlkLstSize(CHeapCtrl *hc)
{
  CMemBlk *m;
  I64 result=0;
  m=hc->next_mem_blk;
  while (m!=&hc->next_mem_blk) {
    result+=sizeof(CMemBlk);
    m=m->next;
  }
  return result;
}

public I64 HeapCtrlAllocated(CHeapCtrl *hc)
{//Count of bytes alloced to a heap ctrl, used+unused.
  CMemBlk *m,*m1;
  I64 result=0;
  if (hc) {
    PUSHFD
    CLI
    while (LBts(&hc->locked_flags,HClf_LOCKED))
      PAUSE
    m=hc->next_mem_blk;
    m1=&hc->next_mem_blk;
    while (m!=m1) {
      if (m->mb_signature!=MBS_USED_SIGNATURE_VAL)
	Dbg("Corrupt Task Memory");
      result+=m->pages*PAGE_SIZE;
      m=m->next;
    }
    LBtr(&hc->locked_flags,HClf_LOCKED);
    POPFD
  }
  return result;
}

public I64 HeapCtrlUnused(CHeapCtrl *hc)
{//Count of bytes alloced to a heap ctrl but unused.
  CMemUnused *uum;
  I64 result=0,i;
  if (hc) {
    PUSHFD
    CLI
    while (LBts(&hc->locked_flags,HClf_LOCKED))
      PAUSE
    result+=MemBlkLstSize(hc);
    for (i=0;i<HEAP_HASH_SIZE>>3;i++) {
      uum=hc->heap_hash[i];
      while (uum) {
	result+=uum->size;
	uum=uum->next;
      }
    }
    uum=hc->malloc_free_lst;
    while (uum) {
      result+=uum->size;
      uum=uum->next;
    }
    LBtr(&hc->locked_flags,HClf_LOCKED);
    POPFD
  }
  return result;
}

#help_index "Memory/Task"
public I64 TaskMemAllocated(CTask *task=NULL,Bool override_validate=FALSE)
{//Count of bytes alloced to a task, used+unused.
  I64 result;
  if (!task) task=Fs;
  if (override_validate || TaskValidate(task)) {
    result=HeapCtrlAllocated(task->code_heap);
    if (task->code_heap!=task->data_heap)
      result+=HeapCtrlAllocated(task->data_heap);
    return result;
  } else
    return 0;
}

public I64 TaskMemUnused(CTask *task=NULL,Bool override_validate=FALSE)
{//Count of bytes alloced to a task but unused.
  I64 result;
  if (!task) task=Fs;
  if (override_validate || TaskValidate(task)) {
    result=HeapCtrlUnused(task->code_heap);
    if (task->code_heap!=task->data_heap)
      result+=HeapCtrlUnused(task->data_heap);
    return result;
  } else
    return 0;
}

public I64 TaskMemUsed(CTask *task=NULL,Bool override_validate=FALSE)
{//Count of bytes alloced to a task and in use.
  I64 result;
  if (!task) task=Fs;
  if (override_validate || TaskValidate(task)) {
    result=task->code_heap->used_u8s;
    if (task->data_heap!=task->code_heap)
      result+=task->data_heap->used_u8s;
    return result;
  } else
    return 0;
}

public Bool HeapRep(CTask *task)
{//Report status of task's heap.
  I64 i,cnt;
  CMemUnused *uum;

  if (!task || task==Fs) {
    "Task can't HeapRep on self.\n";
    return FALSE;
  }
  if (!TaskValidate(task)) return FALSE;

  PUSHFD
  CLI
  while (LBts(&task->code_heap->locked_flags,HClf_LOCKED))
    PAUSE
  if (task->data_heap!=task->code_heap)
    while (LBts(&task->data_heap->locked_flags,HClf_LOCKED))
      PAUSE

  for (i=0;i<HEAP_HASH_SIZE>>3;i++) {
    cnt=0;
    uum=task->code_heap->heap_hash[i];
    while (uum) {
      cnt+=uum->size;
      uum=uum->next;
    }
    if (task->data_heap!=task->code_heap) {
      uum=task->data_heap->heap_hash[i];
      while (uum) {
	cnt+=uum->size;
	uum=uum->next;
      }
    }
    if (cnt)
      "%03X:%08X\n",i<<3,cnt;
  }
  '\n';

  uum=task->code_heap->malloc_free_lst;
  while (uum) {
    "%X, ",uum->size;
    uum=uum->next;
  }
  if (task->data_heap!=task->code_heap) {
    uum=task->data_heap->malloc_free_lst;
    while (uum) {
      "%X, ",uum->size;
      uum=uum->next;
    }
  }

  if (task->data_heap!=task->code_heap)
    LBtr(&task->data_heap->locked_flags,HClf_LOCKED);
  LBtr(&task->code_heap->locked_flags,HClf_LOCKED);
  POPFD

  '\n';
}

#help_index "Memory/Heap Ctrl;Debugging/Heap"
public Bool IsInHeapCtrl(U8 *a,CHeapCtrl *hc,Bool lock=TRUE)
{//Check addr if on heap.
  CMemBlk *m;
  PUSHFD
  CLI
  if (lock)
    while (LBts(&hc->locked_flags,HClf_LOCKED))
      PAUSE
  m=hc->next_mem_blk;
  while (m!=&hc->next_mem_blk) {
    if (a>=m && a<m(U8 *)+m->pages*PAGE_SIZE) {
      if (lock)
	LBtr(&hc->locked_flags,HClf_LOCKED);
      POPFD
      return TRUE;
    }
    m=m->next;
  }
  if (lock)
    LBtr(&hc->locked_flags,HClf_LOCKED);
  POPFD
  return FALSE;
}

public Bool HeapCtrlWalk(CHeapCtrl *hc)
{//Check integrity of heap ctrl.
  I64 i;
  CMemUnused *uum;

  PUSHFD
  CLI
  while (LBts(&hc->locked_flags,HClf_LOCKED))
    PAUSE

  for (i=0;i<HEAP_HASH_SIZE>>3;i++) {
    uum=hc->heap_hash[i];
    while (uum) {
      if (!IsInHeapCtrl(uum,hc,FALSE))
	goto hc_false;
      uum=uum->next;
    }
  }
  uum=hc->malloc_free_lst;
  while (uum) {
    if (!IsInHeapCtrl(uum,hc,FALSE))
      goto hc_false;
    uum=uum->next;
  }

#if _CFG_HEAP_DBG
  CMemUsed *um,*um1;
  um1=(&hc->next_um)(U8 *)-offset(CMemUsed.next);
  um=um1->next;
  while (um!=um1) {
    if (!IsInHeapCtrl(um,hc,FALSE))
      goto hc_false;
    um=um->next;
  }
#endif

  LBtr(&hc->locked_flags,HClf_LOCKED);
  POPFD
  return TRUE;

hc_false:
  LBtr(&hc->locked_flags,HClf_LOCKED);
  POPFD
  return FALSE;
}

#help_index "Memory/Task;Debugging/Heap"
public Bool IsInHeap(U8 *a,CTask *task=NULL,Bool lock=TRUE)
{//Check addr if on heap.
  if (!task) task=Fs;
  if (TaskValidate(task) && (IsInHeapCtrl(a,task->code_heap,lock)||
    task->data_heap!=task->code_heap && IsInHeapCtrl(a,task->data_heap,lock)))
    return TRUE;
  else
    return FALSE;
}

public Bool HeapWalk(CTask *task=NULL)
{//Check integrity of task's heaps.
  if (!task) task=Fs;
  if (!TaskValidate(task) || !HeapCtrlWalk(task->code_heap) ||
    task->data_heap!=task->code_heap && !HeapCtrlWalk(task->data_heap))
    return FALSE;
  else
    return TRUE;
}

#help_index "Info;Task"
U0 TaskRepTask(CTask *task,I64 indent)
{
  CTask *task1;
  U8 *st,*desc=MStrUtil(task->task_title,SUF_SAFE_DOLLAR);
  st=MStrPrint("$MA,T=\"%08X\",LM=\"Kill(0x%X);\n\",$",task,task,task);
  "%h*c%s $TX,\"%Q...\",SCX=16$ Mem:%08X\n",indent,CH_SPACE,
    st,desc,TaskMemAllocated(task);
  "%h*cFlags:%04X:%04X Time:%0.2fm\n",indent+2,CH_SPACE,
    task->task_flags,task->display_flags,
    task->total_time/60.0/cnts.time_stamp_freq_initial;
  Free(st);
  Free(desc);
  task1=task->next_child_task;
  while (task1!=(&task->next_child_task)(U8 *)-offset(CTask.next_sibling_task)) {
    TaskRepTask(task1,indent+2);
    task1=task1->next_sibling_task;
  }
}

public U0 TaskRep()
{//Report current tasks on all cores.
  I64 i;
  CCPU *c;
  PUSHFD
  CLI
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    "$RED$CPU:%d$FG$\n",i;
    TaskRepTask(c->seth_task,2);
  }
  POPFD
}
