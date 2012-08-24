#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x00000037
#define __BIN_1 "\x02\x0E\x11\x00\x00\x00\x00\x00\x00\x00\x00\x28\x00\x00\x00\x18\x00\x00\x00\x02\x09\x11\x04\x00\x00\x00\x04\x00\x00\x00\x24\x00\x00\x00\x14\x00\x00\x00\x02\x0F\x17\x04\x00\x00\x00\x08\x00\x00\x00\x54\x65\x72\x6D\x00\x00"
#define __BIN_4_TYPE 0x00000002
#define __BIN_4_SIZE 0x000000D2
#define __BIN_4 "\x15\x0C\x00\x00\x00\x0C\x00\x00\x00\xFA\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\x00\x00\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\x00\x0F\x00\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\x00\x00\x00\x00\x0F\x00\xFF\x00\xFF\xFF\x00\x00\x00\x00\xFF\xFF\x00\x0F\x0F\x00\x0F\x00\x00\x0F\x00\xFF\x00\x00\x00\x00\xFF\xFF\x00\x00\x0F\x00\x0F\x00\x0F\x00\x00\xFF\x00\x00\x00\x00\xFF\xFF\xFF\x00\x0F\x00\x0F\x00\x0F\x00\xFF\xFF\x00\x00\x00\x00\xFF\x00\x00\x0F\x0F\x00\x0F\x00\x0F\x00\xFF\xFF\x00\x00\x00\x00\x00\x0F\x0F\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x0F\x0F\x0F\x00\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x00\x0F\x0F\x0F\x0F\x00\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\x00\x0F\x0F\x0F\x0F\x00\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define __BIN_3_TYPE 0x00000002
#define __BIN_3_SIZE 0x000000D2
#define __BIN_3 "\x15\x09\x00\x00\x00\x0C\x00\x00\x00\xFB\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\x00\x00\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\x00\x0F\x00\x0F\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x0F\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\x00\x0F\x0F\x0F\x0F\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\x00\x0F\x0F\x0F\x0F\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x0F\x0F\x0F\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x0F\x0F\x0F\x0F\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define __BIN_2_TYPE 0x00000002
#define __BIN_2_SIZE 0x00000026
#define __BIN_2 "\x02\x04\x08\x02\x00\x00\x00\x12\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x05\xF8\xFF\xFF\xFF\x06\x00\x00\x00\x08\x00\x00\x00\xF9\xFF\xFF\xFF\x00"

#help_index "Windows"

U64 last_swap_cnter2[MP_MAX_PROCESSORS];
MemSet(last_swap_cnter2,0,sizeof(last_swap_cnter2));

U64 last_unused_sysmem=0,
   last_calc_refresh_cnt=0;
F64 last_refresh_delta_time=1.0;

#define KEYNAME_BUF_SIZE	18
#define WALLPAPER_ATTR		(BLUE<<4+WHITE)

class TaskWallPaperDataStruct
{
  U64 mem_size;
  U8 caller_stk[TASK_WALLPAPER_DATA_SIZE-8];
};

U0 WallPaper(TaskStruct *_task)
{
  I64 i,j,l=GR_HEIGHT/FONT_HEIGHT-1;
  TaskStruct *task;
  TaskWallPaperDataStruct *wpd;
  SysHashTable *old_hash=Fs->hash_table;
  CPUStruct *c;
  BoolI8 old_preempt=Preempt(OFF);
  LTDate ltdt;
  U8 buf[256],buf2[256],*st;
  GrDC *dc=GrDCAlias(gr_dc,_task);
  static U64 code_mem=0,data_mem=0;

  _task->text_attr=WALLPAPER_ATTR;

  ltdt=GetCurTimeLTDate;
  if (!(win_updates&63))
    last_unused_sysmem=UnusedSysMem+data_mem;
  SPrintF(buf,"%h2D %h4T Ref:%2tf Mem:%8X CPU:",
    ltdt,ltdt,win_actual_refresh,last_unused_sysmem);
  PlotScreenStr(buf,WALLPAPER_ATTR,0,0);

  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    SPrintF(buf,"%2tf",100.0*(1.0-c->idle_factor));
    if (i&1)
      PlotScreenStr(buf,WALLPAPER_ATTR,45+i*2,0);
    else
      PlotScreenStr(buf,WALLPAPER_ATTR&0xF0+YELLOW,45+i*2,0);
  }

  st=ScanCode2KeyName(sys_last_down_scan_code);
  PlotScreenStr(st,WALLPAPER_ATTR&0xF0+YELLOW,80-KEYNAME_BUF_SIZE,0);
  Free(st);

  SPrintF(buf,"Reads:%d Writes:%d lines=%d funs=%d bytes=%08X",
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
      if (!TaskValidate(task)) break;
 
      wpd=&task->wallpaper_data;
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
      if (!(win_updates&63))
	wpd->mem_size=TaskAllocatedMem(task);
      SPrintF(buf,"%s %08X  %08X %08X %-16ts %06X:%04X %5.2fm",buf2,task,j,
	wpd->mem_size,task->task_title,
	task->task_flags,task->display_flags,
	task->total_time/60.0/time_stamp_freq_initial);
      buf[TEXT_COLS]=0;
      PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
      if (!l) goto wp_done;
      task=task->next_task;
    } while (task!=c->seth_task);
  }
  if (l) {
    PlotScreenStr("____Task____ UnusedStk UsedMem_ __Description___ ____Flags__ _Time_",
      WALLPAPER_ATTR,0,l--);
    if (!l) goto wp_done;

    l--;
    if (!l) goto wp_done;

    if (win_updates&63==32) {
      code_mem=UnusedBPMem(sys_code_bp);
      data_mem=UnusedBPMem(sys_data_bp);
    }

    if (sys_data_bp)
      SPrintF(buf,"Code Heap:%010X Data Heap:%010X",code_mem,data_mem);
    else
      SPrintF(buf,"Code/Data Heap:%010X",code_mem);

    PlotScreenStr(buf,WALLPAPER_ATTR,0,l--);
    l--;
    if (!l) goto wp_done;

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
  GrDCDel(dc);
  Preempt(old_preempt);
}

fp_wall_paper=&WallPaper;



/* <1> <1> (image) */



U0 DrawTermButton(GrDC *dc,Ctrl *c)
{
  if (Blink)
    GrSpritePlot3(dc,c->left,c->top,0,__BIN_1);
}

U0 LeftClickTermButton(Ctrl *,I64,I64,BoolI8 down)
{
  if (down)
    SpawnUser;
}

Ctrl *TermButtonNew()
{
  BoolI8 old_preempt;
  I64 min_x,max_x,min_y,max_y;
  Ctrl *c=ACAlloc(sizeof(Ctrl));

  c->win_task=sys_winmgr_task;
  c->flags=CTRLF_SHOW;
  c->type=CTRLT_GENERIC;
  c->draw_it=&DrawTermButton;
  c->left_click=&LeftClickTermButton;

  GrSpriteExtents(__BIN_1,&min_x,&max_x,&min_y,&max_y);
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
public U0 DrawGrabInputPtr(GrDC *dc,I64 x,I64 y,BoolI8 closed)
{
  dc->flags&=~(DCF_TRANSFORMATION|DCF_SYMMETRY);
  dc->color=ROP_EQU;
  if (closed)
    GrSpritePlot3(dc,x,y,0,__BIN_3);
  else
    GrSpritePlot3(dc,x,y,0,__BIN_4);
}
fp_draw_grab_input_ptr=&DrawGrabInputPtr;

#help_index ""
