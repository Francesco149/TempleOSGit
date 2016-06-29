#help_index "Windows"

class CWallPaperGlbls
{
  I64	last_calc_idle_cnt,last_swap_cnter[MP_MAX_PROCESSORS];
  F64	last_calc_idle_delta_time;
  U8	top_line[STR_LEN];
  Bool	record;
} *wall=CAlloc(sizeof(CWallPaperGlbls));
wall->last_calc_idle_delta_time=1.0;

class CTaskWallPaperData
{
  I64	alloced_u8s,used_u8s;
  U8	caller_stk[STR_LEN-$];
};

U0 WallPaper(CTask *_task)
{
  I64 i,j,l=TEXT_ROWS-1;
  CTask *task;
  CTaskWallPaperData *wpd;
  CHashTable *old_hash=Fs->hash_table;
  CCPU *c;
  CDateStruct ds;
  U8 *st;
  _task->text_attr=BLUE<<4+WHITE;
  if (sys_data_bp) {
    TextPrint(Fs,0,l--,BLUE<<4+YELLOW,
	  "%010X/%010X %010X/%010X%12td%12td%12td",
	  sys_data_bp->used_u8s,sys_data_bp->alloced_u8s,
	  sys_code_bp->used_u8s,sys_code_bp->alloced_u8s,
	  cmp.compiled_lines,blkdev.read_cnt,blkdev.write_cnt);
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,
	  "______Data_Heap______ ______Code_Heap______ "
	  "___Lines___ ___Reads___ __Writes___");
  } else {
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,
	  "%010X/%010X%12td%12td%12td",
	  sys_code_bp->used_u8s,sys_code_bp->alloced_u8s,
	  cmp.compiled_lines,blkdev.read_cnt,blkdev.write_cnt);
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,
	  "____CodeData_Heap____ "
	  "___Lines___ ___Reads___ ___Writes__");
  }
  TextPrint(Fs,0,l--,BLUE<<4+BROWN,"ProgressBars:%016X %016X %016X %016X",
	progress1,progress2,progress3,progress4);
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    if (winmgr.t->calc_idle_cnt!=wall->last_calc_idle_cnt) {
      wall->last_calc_idle_delta_time=winmgr.t->calc_idle_delta_time;
      wall->last_swap_cnter[i]=winmgr.t->last_swap_cnter[i];
      winmgr.t->last_swap_cnter[i]=c->swap_cnter;
    }

    task=c->seth_task;
    do {
      wpd=&task->wallpaper_data;
      if (!TaskValidate(task)) break;
      if (!(winmgr.updates&31)) {
	if (c==Gs)
	  Fs->hash_table=task->hash_table;
	else //precaution
	  Fs->hash_table=old_hash;
	StrPrint(wpd->caller_stk,"    %-18tp %-18tp %-18tp %-18tp",
	      TaskCaller(task,0),TaskCaller(task,1),
	      TaskCaller(task,2),TaskCaller(task,3));
      }
      TextPrint(Fs,0,l--,BLUE<<4+YELLOW,wpd->caller_stk);
      j=UnusedStk(task);
      if (j<0)
	Dbg("Stk Overflow",task);
      if (!(winmgr.updates&31)) {
	wpd->alloced_u8s=TaskMemAlloced(task);
	wpd->used_u8s	=TaskMemUsed(task);
      }
      TextPrint(Fs,0,l--,BLUE<<4+YELLOW,
	    "  %-16ts %08X %010X %010X/%010X %04X:%02tX:%08X",
	    task->task_title,task,j,wpd->used_u8s,wpd->alloced_u8s,
	    task->task_flags,task->display_flags,task->win_inhibit);
      if (!TaskValidate(task)) break;
      task=task->next_task;
    } while (task!=c->seth_task);
    TextPrint(Fs,0,l--,BLUE<<4+BROWN,"CPU:%02d %2tf%% ContextSwaps/s:%9,d",i,
	  100.0*(1.0-c->idle_factor),ToI64((winmgr.t->last_swap_cnter[i]
	  -wall->last_swap_cnter[i])/wall->last_calc_idle_delta_time));
  }
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,
	"  __Description___ __Task__ UnusedStk_ _UsedMem__/_AllocMem_ "
	"______Flags_____");

  if (!(winmgr.updates&31) || !*wall->top_line) {
    Date2Struct(&ds,Now+local_time_offset);
    i=sys_code_bp->alloced_u8s-sys_code_bp->used_u8s;
    if (sys_data_bp)
      i+=sys_data_bp->alloced_u8s-sys_data_bp->used_u8s;
    StrPrint(wall->top_line,
	  "%3tZ %02d/%02d %02d:%02d:%02d FPS:%2tf Mem:%010X CPU",
	  ds.day_of_week,"ST_DAYS_OF_WEEK",ds.mon,ds.day_of_mon,
	  ds.hour,ds.min,ds.sec,winmgr.fps,i);
  }
  TextPrint(Fs,0,0,BLUE<<4+WHITE,wall->top_line);

  if (wall->record)
    TextPrint(Fs,44-4,0,BLUE<<4+RED,"*");
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    if (i&1)
      TextPrint(Fs,44+i*2,0,BLUE<<4+WHITE,"%2tf",100.0*(1.0-c->idle_factor));
    else
      TextPrint(Fs,44+i*2,0,BLUE<<4+YELLOW,"%2tf",100.0*(1.0-c->idle_factor));
  }

  st=ScanCode2KeyName(kbd.last_down_scan_code);
  TextPrint(Fs,80-18,0,BLUE<<4+YELLOW,"%18ts",st);
  Free(st);

  Fs->hash_table=old_hash;
  wall->last_calc_idle_cnt=winmgr.t->calc_idle_cnt;
}

/* <1> <1> (image) */



U0 DrawTermButton(CDC *dc,CCtrl *c)
{
  if (Blink)
    Sprite3(dc,c->left,c->top,0,__BIN_1);
}

U0 LeftClickTermButton(CCtrl *,I64,I64,Bool down)
{
  if (down)
    User;
}

CCtrl *TermButtonNew()
{
  I64 min_x,max_x,min_y,max_y;
  CCtrl *c=ACAlloc(sizeof(CCtrl));

  c->win_task=sys_winmgr_task;
  c->flags=CTRLF_SHOW;
  c->type=CTRLT_GENERIC;
  c->draw_it=&DrawTermButton;
  c->left_click=&LeftClickTermButton;

  SpriteExtents(__BIN_1,&min_x,&max_x,&min_y,&max_y);
//min must be zero
  c->left=sys_winmgr_task->pix_width-(max_x-min_x+1);
  c->right=c->left+(max_x-min_x+1)-1;
  c->top=sys_winmgr_task->pix_height-(max_y-min_y+1);
  c->bottom=c->top+(max_y-min_y+1)-1;

  QueIns(c,sys_winmgr_task->last_ctrl);
  TaskDerivedValsUpdate(sys_winmgr_task);

  return c;
}

U0 WallPaperInit()
{
  TermButtonNew;
  gr.fp_wall_paper=&WallPaper;
}
