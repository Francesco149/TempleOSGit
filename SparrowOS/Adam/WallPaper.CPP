#help_index "Windows"

I64 last_swap_cnter2[MP_MAX_PROCESSORS];
MemSet(last_swap_cnter2,0,sizeof(last_swap_cnter2));

I64 last_unused_sysmem=0,
   last_calc_refresh_cnt=0;
F64 last_refresh_delta_time=1.0;

#define KEYNAME_BUF_SIZE	18
#define WALLPAPER_ATTR		(BLUE<<4+WHITE)
#define WALLPAPER_CPU_TEXT_COL	45
Bool wallpaper_record_on=FALSE;

class CTaskWallPaperData
{
  I64 allocated_u8s,used_u8s;
  U8 caller_stk[TASK_WALLPAPER_DATA_SIZE-16];
};

U0 WallPaper(CTask *_task)
{
  I64 i,j,l=GR_HEIGHT/FONT_HEIGHT-1;
  CTask *task;
  CTaskWallPaperData *wpd;
  CHashTable *old_hash=Fs->hash_table;
  CCPU *c;
  Bool old_preempt=Preempt(OFF);
  CDate ltdt;
  U8 buf[256],buf2[256],*st;
  CDC *dc=DCAlias(gr_dc,_task);
  static I64 code_heap_unused=0,code_heap_used=0,
	     data_heap_unused=0,data_heap_used=0;

  _task->text_attr=WALLPAPER_ATTR;

  ltdt=GetCurTimeCDate;
  if (!(win_updates&63))
    last_unused_sysmem=UnusedSysMem+data_heap_unused;
  SPrintF(buf,"%h2D %h4T Ref:%2tf Mem:%010X CPU",
    ltdt,ltdt,win_actual_refresh,last_unused_sysmem);
  PlotScreenStr(buf,WALLPAPER_ATTR,0,0);
 
  if (wallpaper_record_on)
    PlotScreenStr("*",WALLPAPER_ATTR&0xF0+RED,WALLPAPER_CPU_TEXT_COL-4,0);

  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    SPrintF(buf,"%2tf",100.0*(1.0-c->idle_factor));
    if (i&1)
      PlotScreenStr(buf,WALLPAPER_ATTR,WALLPAPER_CPU_TEXT_COL+i*2,0);
    else
      PlotScreenStr(buf,WALLPAPER_ATTR&0xF0+YELLOW,WALLPAPER_CPU_TEXT_COL+i*2,0);
  }

  st=ScanCode2KeyName(sys_last_down_scan_code);
  PlotScreenStr(st,WALLPAPER_ATTR&0xF0+YELLOW,80-KEYNAME_BUF_SIZE,0);
  Free(st);

  SPrintF(buf,"Reads:%d Writes:%d lines=%d funs=%d bytes=%010X",
    sys_dsk_reads,sys_dsk_writes,
    compiled_lines,compiled_funs,compiled_bytes);
  buf[TEXT_COLS]=0;
  PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);

  SPrintF(buf,"progress1=%016X progress2=%016X",
    progress1,progress2);
  buf[TEXT_COLS]=0;
  PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);

  SPrintF(buf,"progress3=%016X progress4=%016X",
    progress3,progress4);
  buf[TEXT_COLS]=0;
  PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);

  l--;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    task=c->seth_task;
    do {
      wpd=&task->wallpaper_data;
      if (!TaskValidate(task)) break;
      if (win_updates&31==15) {
	if (c==Gs)
	  Fs->hash_table=task->hash_table;
	else //precaution
	  Fs->hash_table=old_hash;
	SPrintF(wpd->caller_stk,"%19tP %19tP %19tP %19tP",
	  TaskCaller(task,0),TaskCaller(task,1),
	  TaskCaller(task,2),TaskCaller(task,3));
      }
      PlotScreenStr(wpd->caller_stk,WALLPAPER_ATTR&0xF0|YELLOW,0,l--);
      if (!l) goto wp_done;

      j=UnusedStk(task);
      if (j<0)
	StkOverflow(task);
      if (task->user_num)
	SPrintF(buf2,"#%02td",task->user_num);
      else
	StrCpy(buf2,"   ");
      if (!(win_updates&63)) {
	wpd->allocated_u8s=TaskAllocatedMem(task);
	wpd->used_u8s	  =TaskUsedMem(task);
      }
      SPrintF(buf,"%s %08X %010X %010X %010X %-16ts %05X:%02tX %6.2fm",buf2,task,j,
	wpd->allocated_u8s,wpd->used_u8s,task->task_title,
	task->task_flags,task->display_flags,
	task->total_time/60.0/time_stamp_freq_initial);
      buf[TEXT_COLS]=0;
      PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
      if (!l) goto wp_done;
      if (!TaskValidate(task)) break;
      task=task->next_task;
    } while (task!=c->seth_task);
  }
  if (l) {
    PlotScreenStr("____Task____ UnusedStk_ _AllocMem_ _UsedMem__ __Description___ _Flags__ _Time__",
      WALLPAPER_ATTR,0,l--);
    if (!l) goto wp_done;

    if (!--l) goto wp_done;

    if (win_updates&63==32) {
      code_heap_unused=UnusedBPMem(sys_code_bp);
      code_heap_used  =sys_code_bp->allocated_u8s;
      if (sys_data_bp) {
	data_heap_unused=UnusedBPMem(sys_data_bp);
	data_heap_used	=sys_data_bp->allocated_u8s;
      }
    }

    if (sys_data_bp) {
      SPrintF(buf,"Data Heap Unused:%010X Data Heap Used:%010X",data_heap_unused,data_heap_used);
      PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
      if (!l) goto wp_done;
      SPrintF(buf,"Code Heap Unused:%010X Code Heap Used:%010X",code_heap_unused,code_heap_used);
    } else
      SPrintF(buf,"Code/Data Heap Unused:%010X Used:%010X",code_heap_unused,code_heap_used);
    PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
    if (!l) goto wp_done;

    if (!--l) goto wp_done;

    for (i=0;i<mp_cnt;i++) {
      c=&cpu_structs[i];

      SPrintF(buf,"%03d %9,d",i,
	ToI64((last_swap_cnter[i]-
	last_swap_cnter2[i])/last_refresh_delta_time));
      if (win_calc_refresh_cnt!=last_calc_refresh_cnt) {
	last_refresh_delta_time=refresh_delta_time;
	last_swap_cnter2[i]=last_swap_cnter[i];
	last_swap_cnter[i]=c->swap_cnter;
      }
      buf[TEXT_COLS]=0;
      PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
      if (!l) goto wp_done;
    }

    PlotScreenStr("CPU   Swaps/s",
	WALLPAPER_ATTR,0,l--);
    if (!l) goto wp_done;
    PlotScreenStr("      Context",
	WALLPAPER_ATTR,0,l--);
    if (!l) goto wp_done;
  }
wp_done:
  Fs->hash_table=old_hash;
  last_calc_refresh_cnt=win_calc_refresh_cnt;
  DCDel(dc);
  Preempt(old_preempt);
}

fp_wall_paper=&WallPaper;



/* <1> <1> (image) */



U0 DrawTermButton(CDC *dc,CCtrl *c)
{
  if (Blink)
    CSpritePlot3(dc,c->left,c->top,0,__BIN_1);
}

U0 LeftClickTermButton(CCtrl *,I64,I64,Bool down)
{
  if (down)
    SpawnUser;
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

  CSpriteExtents(__BIN_1,&min_x,&max_x,&min_y,&max_y);
  //min must be zero
  c->left=sys_winmgr_task->win_pixel_width-(max_x-min_x+1);
  c->right=c->left+(max_x-min_x+1)-1;
  c->top=sys_winmgr_task->win_pixel_height-(max_y-min_y+1);
  c->bottom=c->top+(max_y-min_y+1)-1;

  old_preempt=Preempt(OFF);
  QueIns(c,sys_winmgr_task->last_ctrl);
  TaskDerivedValsUpdate(sys_winmgr_task);
  Preempt(old_preempt);

  return c;
}

TermButtonNew;


/* <2> <2> (image) */

gr_empty_sprite=__BIN_2;

/* <3> <3> (image) */

/* <4> <4> (image) */

#help_index "Graphics;Input Pointer/Displayed Image"
public U0 DrawGrabInputPtr(CDC *dc,I64 x,I64 y,Bool closed)
{
  dc->flags&=~(DCF_TRANSFORMATION|DCF_SYMMETRY);
  dc->color=ROP_EQU;
  if (closed)
    CSpritePlot3(dc,x,y,0,__BIN_3);
  else
    CSpritePlot3(dc,x,y,0,__BIN_4);
}
fp_draw_grab_input_ptr=&DrawGrabInputPtr;

#help_index ""
