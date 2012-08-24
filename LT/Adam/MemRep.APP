#help_index "Info;Utils;Memory/Info;Cmd Line (Typically)"

I64 TssSize(TssStruct *tss)
{
  return MSize2(tss)+
      MSize2(tss->st0)+
      MSize2(tss->st1)+
      MSize2(tss->st2);
}

I64 PenBrushesSize()
{
  I64 result=0,i;
  for (i=0;i<GR_NUM_PEN_BRUSHES;i++)
    result+=
      GrDCSize(gr_pen_brushes[i])+
      GrDCSize(gr_collision_pen_brushes[i])+
      GrDCSize(gr_even_pen_brushes[i])+
      GrDCSize(gr_odd_pen_brushes[i] );
  return result;
}

I64 ScreenScaleTablesSize()
{
  I64 result=0,i;
  for (i=1;i<=GR_MAX_SCREEN_SCALE;i++)
    result+=MSize2(gr_screen_scale_tables[i]);
  return result;
}

I64 TaskStkSize(TaskStruct *task)
{
  I64 result=0;
  TaskStk *temps=task->stk;
  while (temps) {
    result+=MSize2(temps);
    temps=temps->next_stk;
  }
  return result;
}

I64 TaskQueSize(TaskStruct *task)
{
  TaskCmdStruct *tempc,*tempc1;
  I64 result=0;
  PushFD;
  Cli;
  while (LBts(&task->locked_flags,TASKLf_TASK_LOCK))
    Pause;
  tempc1=&task->next_servant_cmd;
  tempc=tempc1->next;
  while (tempc!=tempc1) {
    result+=MSize2(tempc);
    if (tempc->cmd_code==TASKCT_EXECUTE_STR ||
	tempc->cmd_code==TASKCT_EXECUTE_FILE ||
	tempc->cmd_code==TASKCT_TEXT_INPUT)
      result+=MSize2(tempc->data);
    tempc=tempc->next;
  }
  LBtr(&task->locked_flags,TASKLf_TASK_LOCK);
  PopFD;
  return result;
}

I64 BlkDevsSize()
{
  I64 i,result=MSize2(local_blkdevs);
  LTBlkDev *bd;
  for (i=0;i<num_blkdevs;i++) {
    bd=&local_blkdevs[i];
    if (bd->type==LTBDT_RAM)
      "RAMDsk %C\t:%08X Unused:%08X\n",
	  bd->drv_let_base,MSize2(bd->RAM_dsk),
	  UnusedDrvSpace(bd->drv_let_base);
    result+=MSize2(bd->dev_id_record);
  }
  return result;
}

I64 PrtsSize()
{
  I64 i,result=MSize2(local_prts);
  for (i=0;i<NUM_LTPRTS;i++)
    result+=MSize2(local_prts[i].buffered_map_dirty_bits)+
	    MSize2(local_prts[i].fis);
  return result;
}

BoolI64 MemRepTask(TaskStruct *task,BoolI8 override_validate=FALSE)
{
  I64 i,j,k,m,n;
  U8 *st;
  Ltf *l;
  LexStruct *lx;
  Ode *o;
  CPUStruct *c;
  TaskStruct *task1;

  if (!override_validate && !TaskValidate(task))
    return FALSE;
  if (task==Fs)
    task->rsp=GetStkPtr;
  st=MStrUtil(task->task_title,SUF_REM_CTRL_CHARS|SUF_SAFE_DOLLAR);
  "$FG,GREEN$Task:%08X$FG,CYAN$ %-30ts$FG$\n$ID,2$",task,st;
  Free(st);
  "$FG,PURPLE$Alloced:%08X $FG,RED$Unused:%08X$FG$\n",
      TaskAllocatedMem(task,override_validate),
      TaskUnusedAllocatedMem(task,override_validate);
  "HashTable:%08X\n",HashTableSize(task->hash_table);

  i=UnusedStk(task);
  if (0<=i<=task->stk->stk_size)
    "Stk Size :%08X Unused:%08X\n",TaskStkSize(task),i;
  else
    "Stk Size :%08X $FG,RED$$BK,1$Overflow$BK,0$$FG$\n",TaskStkSize(task);

  l=task->last_ltf;
  while (l!=(&task->next_ltf)(U8 *)-offset(Ltf.next_ltf) &&
      l->ltf_in_que_signature==LTF_IN_QUE_SIGNATURE_VAL) {
    "Ltf      :%08X\n",LtfSize(l);
    l=l->last_ltf;
  }

  lx=task->next_lex;
  while (lx!=&task->next_lex) {
    "Lex      :%08X\n",LexSize(lx);
    lx=lx->next;
  }

  o=task->next_ode;
  while (o!=&task->next_ode) {
    "Ode      :%08X\n",OdeSize(o);
    o=o->next;
  }

  if (task==sys_winmgr_task) {
    "gr_dc2\t\t:%08X\n",GrDCSize(gr_dc2);
    "gr_dc\t\t:%08X\n",GrDCSize(gr_dc);
    "gr_dc_cache\t\t:%08X\n",GrDCSize(gr_dc_cache);
    "gr_screen_image\t:%08X\n",GrDCSize(gr_screen_image);
    "gr_scaled_dc\t:%08X\n",GrDCSize(gr_scaled_dc);
    "gr_pen_brushes\t:%08X\n",PenBrushesSize;
    "gr_1_to_8_bits\t:%08X\n",MSize2(gr_1_to_8_bits);
    "gr_1_to_8_colors\t:%08X\n",MSize2(gr_1_to_8_colors);
    "gr_text_base\t:%08X\n",MSize2(gr_text_base);
    "gr_vga_text_cache\t:%08X\n",MSize2(gr_vga_text_cache);
    "gr_win_z_buf\t:%08X\n",MSize2(gr_win_z_buf)+MSize2(gr_win_uncovered_bitmap);
    "gr_screen_scale_tables:%08X\n",ScreenScaleTablesSize;
  } else if (task==adam_task) {
    j=0;k=0;m=0;n=0;
    for (i=0;i<mp_cnt;i++) {
      c=&cpu_structs[i];
      k+=TssSize(c->tss);
      task1=c->seth_task;
      do {
	if (task1!=adam_task) {//adam task located in OSMain mem
	  j+=MSize2(task1);
	  m+=MSize2(task1->code_heap);
	  if (task1->data_heap!=task1->code_heap)
	    m+=MSize2(task1->code_heap);
	}
	n+=TaskQueSize(task1);
	task1=task1->next_task;
      } while (task1!=c->seth_task);
      task1=c->idle_task;
      j+=MSize2(task1);
      m+=MSize2(task1->code_heap);
      if (task1->data_heap!=task1->code_heap)
	m+=MSize2(task1->code_heap);
    }
    "TaskStructs\t:%08X\n",j;
    "Tsses\t:%08X\n",k;
    "HeapCtrls\t:%08X\n",m;
    "TaskQues\t:%08X\n",n;
    "BlkDevs\t:%08X\n",BlkDevsSize;
    "Prts\t:%08X\n",PrtsSize;
    "Audio\t:%08X\n",CallExtStr("AudioSize");
    if (sys_ltdc_base)
      "DskCache\t:%08X\n",
	  MSize2(sys_ltdc_base)+MSize2(sys_ltdc_hash_table)+MSize2(sys_ltdc_ctrl);
    "WordStat\t:%08X\n",CallExtStr("WordStatSize");
    "Clipboard\t:%08X\n",LtfSize(sys_clipboard_ltf);
  }
  "$ID,-2$";
  return TRUE;
}


public U0 MemRep()
{
  I64 i;
  TaskStruct *task;
  CPUStruct *c;
  U32 *u32_ptr;
  BoolI8 old_preempt=Preempt(OFF);
  BinFileHeaderStruct *bfh=sys_boot_base-sizeof(BinFileHeaderStruct);
  bfh(U64)+=bfh->file_size-1;

  "$FG,BLACK$Low Memory\t:00100000$FG$\n$ID,2$";
  u32_ptr=SYS_BOOT_BASE;
  "OSMain\t:%08X-%08X\n",*u32_ptr-sizeof(BinFileHeaderStruct),bfh;
  "VGA\t\t:000A0000-000BFFFF\n";
  "$ID,-2$$FG,BLACK$High Memory\t:00100000-%08X$FG$\n",
      sys_heap_limit;
  "$ID,2$Page Tables\t:%08X-%08X\n",PAGE_TABLE_BASE,sys_heap_base-1;
  "$ID,-2$";

  if (sys_data_bp) {
    "$FG,BLACK$Code Heap:%010X $FG,RED$Unused:%010X$FG$\n",
	sys_code_bp->size*PAGE_SIZE,UnusedBPMem(sys_code_bp);
    "$FG,BLACK$Data Heap:%010X $FG,RED$Unused:%010X$FG$\n",
	sys_data_bp->size*PAGE_SIZE,UnusedBPMem(sys_data_bp);
  } else
    "$FG,BLACK$Code/Data Heap:%08X $FG,RED$Unused:%08X$FG$\n",
	sys_code_bp->size*PAGE_SIZE,UnusedBPMem(sys_code_bp);

  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    "$FG,GREEN$CPU%d$FG$\n$ID,2$",i;
    task=c->seth_task;
    do {
      if (!MemRepTask(task))
	break;
      task=task->next_task;
    } while (task!=c->seth_task);
    MemRepTask(c->idle_task,TRUE);
    "$ID,-2$";
  }

  "$FG,GREEN$Independent Heaps$FG$\n$ID,2$";
  "$FG,BLACK$sys_uncached_heap:%08X $FG,RED$Unused:%08X$FG$\n",
      sys_uncached_heap->bp->size*PAGE_SIZE,UnusedBPMem(sys_uncached_heap->bp);
  "$ID,-2$\n";
  Preempt(old_preempt);
}

#help_index ""
