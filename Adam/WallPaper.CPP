#help_index "Windows"

class CWallPaperGlbls
{
  I64	last_calc_idle_cnt,last_swap_cnter[MP_MAX_PROCESSORS];
  F64	last_calc_idle_delta_time;
  U8	top_line[128];
  Bool	record;
} *wall=CAlloc(sizeof(CWallPaperGlbls));
wall->last_calc_idle_delta_time=1.0;

class CTaskWallPaperData
{
  I64	alloced_u8s,used_u8s;
  U8	caller_stk[TASK_WALLPAPER_DATA_SIZE-16];
};

U0 WallPaper(CTask *_task)
{
  I64 i,j,l=TEXT_ROWS-1;
  CTask *task;
  CTaskWallPaperData *wpd;
  CHashTable *old_hash=Fs->hash_table;
  CCPU *c;
  Bool old_preempt=Preempt(OFF);
  CDateStruct ds;
  U8 *st;
  _task->text_attr=BLUE<<4+WHITE;
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"Reads:%d Writes:%d lines=%d",
    dsk.read_cnt,dsk.write_cnt,cmp.compiled_lines);
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"progress1=%016X progress2=%016X",progress1,progress2);
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"progress3=%016X progress4=%016X",progress3,progress4);
  l--;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    task=c->seth_task;
    do {
      wpd=&task->wallpaper_data;
      if (!TaskValidate(task)) break;
      if (!(winmgr.updates&31)) {
	if (c==Gs)
	  Fs->hash_table=task->hash_table;
	else //precaution
	  Fs->hash_table=old_hash;
	StrPrint(wpd->caller_stk,"%19tp %19tp %19tp %19tp",
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
      TextPrint(Fs,0,l--,BLUE<<4+WHITE,"%08X %010X %010X %010X %-16ts %05X:%02tX %6.2fm",task,j,
	wpd->alloced_u8s,wpd->used_u8s,task->task_title,
	task->task_flags,task->display_flags,
	task->total_time/60.0/cnts.time_stamp_freq_initial);
      if (!TaskValidate(task)) break;
      task=task->next_task;
    } while (task!=c->seth_task);
  }
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"__Task__ UnusedStk_ _AllocMem_ _UsedMem__ __Description___ _Flags__ _Time__");
  l--;

  if (sys_data_bp) {
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,"Data Heap Alloced:%010X Data Heap Used:%010X",
	sys_data_bp->alloced_u8s,sys_data_bp->used_u8s);
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,"Code Heap Alloced:%010X Code Heap Used:%010X",
	sys_code_bp->alloced_u8s,sys_code_bp->used_u8s);
  } else
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,"Code/Data Heap Alloced:%010X Used:%010X",
	sys_code_bp->alloced_u8s,sys_code_bp->used_u8s);

  l--;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    TextPrint(Fs,0,l--,BLUE<<4+WHITE,"%03d %9,d",i,
      ToI64((winmgr.t->last_swap_cnter[i]-wall->last_swap_cnter[i])/wall->last_calc_idle_delta_time));
    if (winmgr.t->calc_idle_cnt!=wall->last_calc_idle_cnt) {
      wall->last_calc_idle_delta_time=winmgr.t->calc_idle_delta_time;
      wall->last_swap_cnter[i]=winmgr.t->last_swap_cnter[i];
      winmgr.t->last_swap_cnter[i]=c->swap_cnter;
    }
  }

  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"CPU   Swaps/s");
  TextPrint(Fs,0,l--,BLUE<<4+WHITE,"      Context");

  if (!(winmgr.updates&31) || !*wall->top_line) {
    Date2Struct(&ds,Now+local_time_offset);
    i=sys_code_bp->alloced_u8s-sys_code_bp->used_u8s;
    if (sys_data_bp)
      i+=sys_data_bp->alloced_u8s-sys_data_bp->used_u8s;
    StrPrint(wall->top_line,"%3tZ %02d/%02d %02d:%02d:%02d FPS:%2tf Mem:%010X CPU",
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
  Preempt(old_preempt);
}

gr.fp_wall_paper=&WallPaper;

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
  Bool old_preempt;
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

  old_preempt=Preempt(OFF);
  QueIns(c,sys_winmgr_task->last_ctrl);
  TaskDerivedValsUpdate(sys_winmgr_task);
  Preempt(old_preempt);

  return c;
}

TermButtonNew;

/* <2> <2> (image) */

gr.empty_sprite=__BIN_2;

/* <3> <3> (image) */

/* <4> <4> (image) */

#help_index "Graphics/Misc;Input Pointer/Displayed Image"
public U0 DrawGrabInputPtr(CDC *dc,I64 x,I64 y,Bool closed)
{//This is a callback.
  dc->flags&=~(DCF_TRANSFORMATION|DCF_SYMMETRY);
  dc->color=ROP_EQU;
  if (closed)
    Sprite3(dc,x,y,0,__BIN_3);
  else
    Sprite3(dc,x,y,0,__BIN_4);
}
gr.fp_draw_grab_input_ptr=&DrawGrabInputPtr;
