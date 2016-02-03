CHeapCtrl *HeapCtrlInit(CHeapCtrl *hc=NULL,CTask *task=NULL,CBlkPool *bp)
{//See [C:/Adam/Opt/Utils/HeapLog.CPP.Z,73] HeapLog() for an example.
//Duplicated for [C:/Kernel/KStart.CPP.Z,1] Adam Task.
  if (!hc)
    hc=ACAlloc(sizeof(CHeapCtrl));
  hc->hc_signature=HEAP_CTRL_SIGNATURE_VAL;
  hc->mem_task=task;
  hc->bp=bp;
  QueInit(&hc->next_mem_blk);
  hc->last_mergable=NULL;
  hc->next_um=hc->last_um=(&hc->next_um)(U8 *)-offset(CMemUsed.next);
  return hc;
}

U0 HeapCtrlDel(CHeapCtrl *hc)
{//Free all blks alloced to a HeapCtrl.
  CMemBlk *m,*m1;
  if (hc) {
    PUSHFD
    CLI
    while (LBts(&hc->locked_flags,HClf_LOCKED))
      PAUSE
    m=hc->next_mem_blk;
    while (m!=&hc->next_mem_blk) {
      m1=m->next;
      Mem512TaskFree(m,hc);
      m=m1;
    }
    LBtr(&hc->locked_flags,HClf_LOCKED);
    POPFD
    Free(hc);
  }
}
