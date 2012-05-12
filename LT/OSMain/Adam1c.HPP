#help_index "Time/LTDate"
#help_file "::/LT/Doc/TimeLTDate"
public extern LTDate local_time_offset;
//49,710Hz since boot (LTDATE_FREQ)
public extern LTDate BootTime();

#help_index "Time/Jiffies"
//18.33333*65536Hz (SYS_TIMER_FREQ)
public extern U64 SysTimerRead();

#help_index "Task"
extern SysAcctStruct sys_accts,*sys_acct;
extern U8 *sys_acct_name;
extern TaskStruct *sys_winmgr_task,*sys_task_being_screen_updated;
public extern TaskStruct *adam_task;
public extern I64 sys_num_spawned_users;
extern U0 TaskFinishOffDying();
public LTextern SYS_SAVE_CONTEXT_YIELD U0 Yield();
public extern U0 TssBusy(I64 tr,BoolI8 val=OFF);
LTextern SYS_IDLE_START U8 sys_idle_start;
LTextern SYS_IDLE_END	U8 sys_idle_end;
LTextern SYS_TASK_END_NOW U0 TaskEndNow();


#help_index "Time/CPU Cycles"
#help_file "::/LT/Doc/TimeCycles"
public extern U64 time_stamp_freq,time_stamp_kHz_freq;
public extern U64 TaskTotalTime();

//This is sampled once at boot time
public extern U64 time_stamp_freq_initial;

#help_index "Time/HPET"
#help_file "::/LT/Doc/TimeHPET"
public extern U64 HPET_freq,HPET_kHz_freq;

#help_index "Time/Seconds"
public extern F64 tNP(); //High freq.  No accounting for task Pause.
public extern F64 tJ(); //From Jiffies.
public extern F64 tS(); //From SysTimerFreq.
public extern F64 tP(TaskStruct *task=NULL); //High freq.  Accounts for task pause.
public extern BoolI64 Blink(F64 Hz=2.5,F64 t=0);

#help_index "Snd"
#help_file "::/LT/Doc/Snd"
public extern I64 snd_dev;
public extern F64 snd_vol,snd_reverb_intensity,snd_reverb_delay;
public extern U0 Snd(F64 freq,U64 waveform=WF_SQUARE,F64 amp=1.0);
public extern BoolI64 Mute(BoolI8 val);
public extern BoolI64 IsMute();
public extern F64 snd_freq;
public extern U0 Beep(U64 freq=500,BoolI8 busy=FALSE);
public extern BoolI64 IOSnd(BoolI8 val);
#help_index "Snd/Snd Files"
public extern BoolI8 snd_record;
extern U0 (*fp_snd_record)(F64 freq,U64 waveform,F64 amp);
#help_index "Snd/HDAudio"
public extern U0 (*fp_snd)(F64 freq,U64 waveform,F64 amp);
public extern U0 (*fp_snd_fill_buf)(SND_OUT_CONTAINER *buf,I64 buf_num);
public extern U0 (*fp_snd_copy_buf)(SND_IN_CONTAINER *buf,I64 buf_num);

#help_index "Processor"
public extern U0 *GetIDTEntry(U64 irq,U64 cpu_num=0);
public extern U0 *SetIDTEntry(U64 irq,U0 (*fp_new_handler)(),U64 cpu_num=0,U64 type=IDTET_IRQ,U64 dpl=0);
extern PCIDevStruct sys_pci_dev_root;
extern MemRange sys_mem32_dev_root;

#help_index "Task/Delay;Time/CPU Cycles"
public extern U64 TimeStampFreqCal();
public extern U0 BusyWait(U64 uS); //Loosely timed
public extern U0 SleepUntil(U64 wake_jiffy);
public extern U0 Sleep(U64 mS);
public extern U0 PortNop(); //On the order of 1uS
#help_index "Task"
public extern BoolI64 IsPreempt();

#help_index "Debugging"
#help_file "::/LT/Doc/Dbg"

public extern U0 D(U0 *buf,U64 cnt=0x80,BoolI8 show_offset=TRUE);
public extern U0 Dm(U0 *buf,U64 cnt=0x80);
public extern U0 Da(U0 **b,U64 cnt=0x10);
extern U64 *TaskRegAdd(TaskStruct *task,I64 reg_num);
public extern U0 Dr(TaskStruct *task=NULL);
public extern BoolI64 IsDbgTask();
public extern BoolI64 IsSysDbg();
public extern BoolI64 IsSysDeterministic();
public extern BoolI64 SysDbg(BoolI8 val);
public extern BoolI64 SysDeterministic(BoolI8 val);
public extern BoolI64 ChkPtr(U0 *ptr);
public extern BoolI64 ChkOnStk(U0 *ptr,TaskStruct *task=NULL);
public extern I64 UnusedStk(TaskStruct *task=NULL);
public extern U0 StkRep(TaskStruct *task=NULL);
public extern U0 CallerRep(U0 **rbp=NULL,TaskStruct *task=NULL);
public extern U0 *Caller(U64 num=1);
public extern U0 *TaskCaller(TaskStruct *task=NULL,I64 num=0,BoolI8 saved_context=FALSE);
public extern U0 Dbg(U8 *msg=NULL,U64 num=0,BoolI8 fault=FALSE);
public extern U0 DbgExit();
public extern U64 sys_dbg_fault_num;
public extern U0 *sys_dbg_fault_rip;
#help_index "Debugging/Raw Output"
public extern U0 RawDrawChar(U64 ch);
public extern BoolI64 Raw(BoolI8 val);
public extern BoolI64 IsRaw();
public extern BoolI64 SingleUser(BoolI8 val);
public extern BoolI64 IsSingleUser();
public extern BoolI64 InDbg(BoolI8 val);
public extern BoolI64 IsInDbg();
public extern U0 DbgStr(U8 *st,U64 mS=100);
public extern U0 DbgNum(U64 num,U64 mS=100);
public extern U0 DbgF(F64 d,U64 mS=100);
public extern U0 DbgAdd(U0 *add,U64 mS=100);
public extern U0 DbgD(U0 *add,U64 cnt=0x80,U64 mS=100);
public extern U0 DbgDm(U0 *add,U64 cnt=0x80,U64 mS=100);

#help_index "Graphics/Color"
extern U0 (*fp_set_std_palette)();
public extern U8 *Color2Text(U8 *buf,ColorROPU32 c);

#help_index "Char"
#help_file "::/LT/Doc/Char"
extern U0 (**fp_ctrl_alt_let_cbs)(U64 sc);
extern U64 ctrl_alt_let_in_irq_flags;
extern U8 **ctrl_alt_let_descs;
extern TaskCmdStruct sys_macro_root;
extern KeyDevStruct sys_put_key_root;
extern TaskStruct *sys_macro_util_task;
extern U64 sys_raw_dc_col,sys_raw_dc_flags;
extern U8 *sys_dc_screen_image;
public extern BoolI8 text_mode;

//Constants are defined for these to
//optimize.  See [::/LT/Adam/Gr/GrCfg.APZ] ::/LT/Adam/Gr/GrCfg.APZ.
public extern I64 text_rows,text_cols,gr_width,gr_height;
public extern U8 *gr_font;

public extern KeyDevStruct *KeyDevAdd(BoolI64 (*fp_put_key)(U64 ch,U64 sc),BoolI64 (*fp_puts)(U8 *st),U64 priority,BoolI8 key_descs=FALSE);
public extern U0 KeyDevRem(KeyDevStruct *tempp);
public extern TaskKeyDevStruct *TaskKeyDevAdd(BoolI64 (*fp_put_key)(TaskStruct *task,U0 *data,U64 ch,U64 sc),
  BoolI64 (*fp_puts)(TaskStruct *task,U0 *data,U8 *st),U64 priority,BoolI8 key_descs,TaskStruct *task,U0 *data);
public extern U0 TaskKeyDevRem(TaskKeyDevStruct *tempk);
public extern U0 SetKeyDesc(U8 *fmt,...);
public extern U0 SetCtrlAltLetCB(U8 ch,U0 (*fp_handler)(U64 sc),U8 *desc,BoolI8 in_irq=FALSE);
extern U8 key_desc[128];
extern U0 *key_handler;

#help_index "Char/Output;Char/Define"
public extern U0 PutDefineErr(U8 *st);
public extern U0 PutDefineWarn(U8 *st);
#help_index "Char/Output"
public extern U0 PrintF(U8 *fmt,...);
public extern U0 PrintFErr(U8 *fmt,...);
public extern U0 PrintFWarn(U8 *fmt,...);
public extern U0 NewLine(I64 cnt=1);
public extern U0 Spaces(I64 cnt=1);
public extern U0 PutErr(U8 *st=NULL);
public extern U0 PutWarn(U8 *st=NULL);
public extern U0 PutKey(U64 ch,U64 scancode);
public extern U0 PutChar(U64 ch);
public extern U0 PutS(U8 *st);
public extern U8 *Bin2Hex(U8 *dst,U64 n;U64 width);
public extern U0 PutHex(U64 n,U64 width);
public extern BoolI64 Silent(BoolI8 val);
public extern BoolI64 IsSilent();
public extern U0 GetOutOfDollar();
#help_index "Char/Conversion"
public extern I64 A2I(U8 *st,I64 radix=10,U8 **_end_ptr=NULL);
#help_index "Input Pointer/Mouse"
public extern U64 mouse_evt_time;
extern U64 mouse_pkt_size;
#help_index "Char/Input"
public extern U64 kbd_evt_time;
extern BoolI8 kbd_irqs_working;
public extern BoolI64 PressAKey();
public extern BoolI64 ExitAllApplications();
public extern BoolI64 AreYouSure();
public extern U64 GetKey(U64 *_scancode=NULL,BoolI8 echo=FALSE,BoolI8 raw_cursor=FALSE);
public extern U64 GetChar(U64 *_scancode=NULL,BoolI8 echo=TRUE,BoolI8 raw_cursor=FALSE);
public extern BoolI64 ScanKey(U64 *_ch=NULL,U64 *_scancode=NULL,BoolI8 echo=FALSE);
public extern BoolI64 YorN();
public extern U64 GetS(U8 *buf,U64 size,BoolI8 allow_ext=TRUE);

//Gets [C:/LT/OSMain/Adam1a.HPZ,1419] Flags
public extern U8 *MGetS(U64 flags=0);
extern U8  *(*fp_mgets)(U64 flags=0);
extern U8 *GetFJoin(U8 *src,U8 *fmt,I64 argc,I64 *argv);
public extern U8 *GetF(U8 *src,U8 *fmt,...);

#help_index "Char/Lists"
public extern U8 *ListEntryPoint(I64 entry_num, U8 *lst);
public extern I64 ListEntryMatch(U8 *src, U8 *lst,I64 flags=0,I64 *ambiguity_overkill_ptr=NULL);

#help_index "Char/Operations"
public extern I64 StrOcc(U8 *src, U8 ch);
public extern U8 *StrFirstOcc(U8 *src,U8 *marker);
public extern U8 *StrLastOcc(U8 *src,U8 *marker);
public extern I64 Spaces2Tabs(U8 *dst,U8 *src);
public extern U8 *Tabs2Spaces(U8 *src);
public extern U8 *ScaleIndent(U8 *src,F64 indent_scale_factor);
public extern U8 *StrUtil(U8 *_src,I64 flags);
public extern U8 *MStrUtil(U8 *src,U64 flags,F64 indent_scale_factor=0);
public extern U8 *StrFirstRem(U8 *src,U8 *marker,U8 *dst=NULL);
public extern U8 *StrLastRem(U8 *src,U8 *marker,U8 *dst=NULL);
public extern BoolI64 WildCardMatch(U8 *wild_st,U8 *src_st);
public extern U8 *StrAdd(U8 *base,U8 *addition); //frees base arg
public extern U8 *SearchStr(U8 *src,U8 *pattern,U64 flags=0);
extern U8 *SPrintFJoin(U8 *dst,U8 *fmt,I64 argc,I64 *argv);
public extern U8 *SPrintF(U8 *dst,U8 *fmt,...);
public extern U8 *MSPrintF(U8 *fmt,...);

#help_index "Char/Define"
#help_file "::/LT/Doc/Define"
public extern DefineStrHashEntry *DefineLoad(U8 *name,U8 *st,BoolI8 replace=FALSE);
public extern DefineStrHashEntry *DefineListLoad(U8 *name,U8 *st,BoolI8 replace=FALSE);
public extern U0 DefinePrintF(U8 *name,U8 *src,...);
public extern U8 *Define(U8 *name);
public extern U8 *DefineSub(U64 sub,U8 *name);
public extern I64 DefineSubCnt(U8 *name);
public extern U0 PutDefine(U8 *name);
#help_index "Char/Define;Char/Lists"
public extern I64 DefineMatch(U8 *src,U8 *lst_name,U64 flags=0,I64 *ambiguity_overkill_ptr=NULL);

#help_index "Memory"
#help_file "::/LT/Doc/Mem"
public LTextern _MEMCMP I64 MemCmp(U8 *ptr1,U8 *ptr2,U64 cnt);
public extern U64 MemPageSize(U0 *a)
public extern BoolI64 MemMarkPresent(U0 *a,BoolI8 val);
public extern BoolI64 MemProtectLowPages(BoolI8 val);
public extern U0 *Alloc2MegMemBlks(U64 *_pages2Meg,BlkPool *bp=NULL);
public extern U0 *AllocUncachedMemBlks(U64 *_pages2Meg,BlkPool *bp=NULL);
public extern U0 *AllocWriteThroughMemBlks(U64 *_pages2Meg,BlkPool *bp=NULL);
public extern U0 Free2MegMemBlks(MemBlk *m,U64 pages2Meg,BlkPool *bp=NULL);
public extern U0 *AllocNonTaskMemBlks(U64 *_pages512,BlkPool *bp=NULL);
public extern U64 *MemPointAtPageTableEntry(U0 *a);
public extern U0 FreeNonTaskMemBlks(MemBlk *m,U64 pages512,BlkPool *bp=NULL);
public LTextern _MALLOC U0 *MAlloc(U64 size,TaskStruct *mem_task=NULL);
public LTextern _FREE U0 Free(U0 *add);
public LTextern _MSIZE U64 MSize(U0 *src);	//size of heap object
public LTextern _MSIZE2 U64 MSize2(U0 *src); //Internal size
public LTextern _MHEAP_CTRL HeapCtrl *MHeapCtrl(U0 *src);
public extern U0 *CAlloc(U64 size,TaskStruct *mem_task=NULL);
public extern U0 *MAllocIdentical(U0 *src,TaskStruct *mem_task=NULL);
public extern U0 *MAllocAligned(U64 size,U64 alignment,TaskStruct *mem_task=NULL,U64 misalignment=0);
public extern U0 *CAllocAligned(U64 size,U64 alignment,TaskStruct *mem_task=NULL,U64 misalignment=0);
public extern U0 LinkedListDel(U0 **list);
public extern U0 *LinkedListCopy(U0 **list,TaskStruct *mem_task=NULL);
public extern U64 LinkedListSize(U0 **list);
public extern I64 Scale2Mem(I64 min,I64 max,I64 limit=2*1024*1024*1024);
#help_index "Memory/Heap Ctrl"
public extern U0 BlkPoolAdd(BlkPool *bp,MemBlk *m,U64 pages512);
public extern U0 BlkPoolInit(BlkPool *bp,U64 pages512);
extern HeapCtrl *TaskHeapCtrlNew(TaskStruct *task,BlkPool *bp);
extern U0 TaskHeapCtrlDel(HeapCtrl *hc);
public extern HeapCtrl *IndependentHeapCtrlInit(BlkPool *bp,U64 pages512);
#help_index "Memory/Adam Heap"
public extern U0 *AMAlloc(U64 size);
public extern U0 *AMAllocIdentical(U0 *src);
public extern U0 *ACAlloc(U64 size);

#help_index "Char/Operations;Memory"
public extern U8 *StrNew(U8 *buf,TaskStruct *mem_task=NULL);
#help_index "Char/Operations;Memory/Adam Heap"
public extern U8 *AStrNew(U8 *buf);

#help_index "Device"
public extern U0 *AllocMemMap32BitDev(U64 size,U64 alignment);
public extern U0 *AllocMemMap64BitDev(U64 *_pages2Meg);
public extern U0 FreeMemMap32BitDev(U8 *base);
public extern U0 FreeMemMap64BitDev(U8 *base,U64 pages2Meg);
public extern HeapCtrl *sys_uncached_heap;

#help_index "Task"
public extern U0 Exit();
public extern U0 Kill(TaskStruct *task,BoolI8 wait=TRUE);
public extern BoolI64 TaskValidate(TaskStruct *task);
public extern TaskStruct *Spawn(U0 (*fp_add)(U0 *data),U0 *data=NULL,
	    U8 *task_name=NULL,
	    TaskStruct *parent=NULL, //NULL means adam
	    U64 stk_size=0,U64 flags=SPAWNF_ADD_TO_QUEUE);
public extern U0 TaskPause(TaskStruct *task,BoolI8 state);
public extern U0 TaskInsQue(TaskStruct *task,TaskStruct *pred=NULL);
public extern U0 TaskRemQue(TaskStruct *task);
public extern U0 TaskWaitIdle(TaskStruct *task);
public extern U0 UserTaskInit();
public extern U0 TaskLocalInit();
public extern U0 UserCmdLine();
public extern U0 WinDerivedValsUpdate(TaskStruct *task=NULL);
public extern U0 TaskDerivedValsUpdate(TaskStruct *task=NULL,BoolI8 update_z_buf=TRUE);
extern U0 (*fp_update_ctrls)(TaskStruct *task);
public extern TaskStruct *SpawnUser(U8 *st=NULL);
extern U0 TaskFinishOff(TaskStruct *task);
public extern U0 OverRideSingleUserSwap();

#help_index "Task/Requests"
public extern U0 TaskRstAwaitingMsg(TaskStruct *task=NULL);
public extern TaskCmdStruct *
  TaskRequestQueue(TaskStruct *servant,TaskStruct *master,
		     U8 *data,U64 flags);
public extern U64 ServantCmdsHandler(TaskStruct *task=NULL);
public extern I64 ServantUser(U8 *fmt,...);
extern U0 ServantUserCmdLine();
public extern BoolI64 GetRequestResult(TaskCmdStruct *rqst=NULL,U64 *result=NULL);
public extern U0 ServantTaskCont();
public extern I64 Adam(U8 *fmt,...);
public extern U0 AdamLogExcept(I64 argc,I64 *argv);
public extern U0 AdamLog(U8 *fmt,...);
public extern U0 AdamErr(U8 *fmt,...);
#help_index "Task/Text Input"
public extern TaskCmdStruct *
  TextInputQueue(TaskStruct *servant,TaskStruct *master,
		     U8 *data,U64 flags);
#help_index "Task/Text Input;Messages"
public extern TaskCmdStruct *QueueMsg(TaskStruct *servant,TaskStruct *master,
		     I64 msg_code,I64 p1,I64 p2,U64 flags);
public extern U0 PostMsg(TaskStruct *task,I64 msg_code,I64 p1,I64 p2,U64 flags=0);
public extern U0 SendMsg(TaskStruct *task,I64 msg_code,I64 p1,I64 p2,U64 flags=0);
public extern U0 Msg(I64 msg_code,I64 p1,I64 p2,U64 flags=0);

#help_index "Exceptions"
//Exceptions are thrown through the following funcation call.
//by convention, the first arg should be a type code.
public extern U0 PutExcept();
public extern U0 throw(...);
public extern U0 throw_no_log(...);
public extern U64 ExceptDepth(TaskStruct *task=NULL);
//The following are used internally and should not be called directly
extern U0 SysTry(U0 *start_label,U0 *skip_label);
extern U0 SysUntry();
public extern U0 ThrowBreak(TaskStruct *task=NULL);
public extern BoolI64 BreakLock(TaskStruct *task=NULL);
public extern BoolI64 BreakUnlock(TaskStruct *task=NULL);

#help_index "Compression/Piece by Piece"
public extern U0 ArcCompressBuf(ArcCs *c);
public extern U0 ArcFinishCompression(ArcCs *c);
public extern U0 ArcExpandBuf(ArcCs *c);
public extern ArcCs *ArcCsNew(BoolI8 expand,BoolI8 text_only);
public extern U0 ArcCsDel(ArcCs *c);
#help_index "Compression"
public extern ArcCompressStruct *
  CompressBuf(U0 *src,I64 size,U64 flags=0,TaskStruct *mem_task=NULL);
public extern U0 *ExpandBuf(ArcCompressStruct *r,TaskStruct *mem_task=NULL);
//This is just an XOR checksum.
extern U64 ArcXSum(U32 *buf,U64 size);
LTextern _ARC_GET_TABLE_ENTRY
	U0 ArcGetTableEntry(ArcCs *c);

#help_index "AutoFile"
#help_file "::/LT/Doc/AutoFile"
#help_index "Task/Text Input;AutoFile"
public extern U0 Auto(U8 *fmt,...);
public extern U0 AutoStr(U8 *fmt,...);
public extern U0 AutoFile(U8 *filename);
public extern U0 XTalk(TaskStruct *task,U8 *fmt,...);
public extern U0 XTalkWithWait(TaskStruct *task,U8 *fmt,...);
public extern U0 XTalkStr(TaskStruct *task,U8 *fmt,...);
public extern U0 XTalkStrWithWait(TaskStruct *task,U8 *fmt,...);

#help_index "Misc/Progress Bars"
public extern U0 ProgressBarsRst();

#help_index "Math"
public extern U16 RandU16(); //With timestamp
public extern U32 RandU32();
public extern U64 RandU64();
public extern I16 RandI16();
public extern I32 RandI32();
public extern I64 RandI64();
public extern F64 Rand();
public extern U16 Rand2U16(); //Without timestamp
public extern U32 Rand2U32();
public extern U64 Rand2U64();
public extern I16 Rand2I16();
public extern I32 Rand2I32();
public extern I64 Rand2I64();
public extern F64 Rand2();
public extern U0  Randomize(U64 seed=0);
public extern U64 FloorU64(U64 num,U64 to);
public extern U64 CeilU64(U64 num,U64 to);
public extern I64 RoundI64(I64 num,I64 to);
public extern I64 FloorI64(I64 num,I64 to);
public extern I64 CeilI64(I64 num,I64 to);

#help_index "Misc"
public extern U0 QSortU64(U64 *base,U64 num, I64 (*fp_compare)(U64 e1,U64 e2));
public extern U0 QSort(U0 *base,U64 num, U64 width,I64 (*fp_compare)(U0 *e1,U0 *e2));

public extern U16 BigEndianU16(U16 d);
public extern U32 BigEndianU32(U32 d);
public extern U64 BigEndianU64(U64 d);
extern U64 sys_rnd_seed;

#help_index "Bit"
public extern I64 BCnt(U64 d);
public extern U8 *reverse_bits_table,*set_bits_table;

#help_index "Ltf/Clipboard"
public extern Ltf *sys_clipboard_ltf;

#help_index "Time/LTDate"
public extern U16 mon_start_days1[12];
public extern U16 mon_start_days2[12];
public extern I64 YearStartDate(I64 year);
public extern LTDate LTDateStruct2LTDate(LTDateStruct *d);
public extern U0 LTDate2LTDateStruct(LTDateStruct *d,LTDate ltdt);
public extern I64 FirstDayOfMon(I64 i);
public extern I64 LastDayOfMon(I64 i);
public extern I64 FirstDayOfYear(I64 i);
public extern I64 LastDayOfYear(I64 i);
public extern I64 DayOfWeek(I64 i);
public extern LTDate GetCurTimeLTDate();
public extern U0 LTDate2ISO1(ISO1Date *dst,LTDate ltdate);

#help_index "File/System"
extern U8 sys_temp_filename[136];
extern U64 sys_dsk_reads,sys_dsk_writes;

#help_index "File/System/Cache"
extern LTCacheBlk *sys_ltdc_base,*sys_ltdc_ctrl;
extern LTCacheBlk **sys_ltdc_hash_table;
extern U64 sys_ltdc_size;
extern TaskStruct *sys_ltdc_write_out_task;

extern U0 LTDCInit(U64 size_in_U8s);
public extern BoolI64 CacheWrites(U8 drv_let,BoolI8 val,BoolI8 all_writes=FALSE);
public extern U0 DskCacheWriteOut(U64 time_stamp_delta,BoolI8 lock);
public extern U0 DskCacheInvalidate(LTPrt *p);

#help_index "File/System/BlkDev"
public extern I64 num_blkdevs;
public extern LTBlkDev *local_blkdevs;

#help_index "File/System/Partitions"
public extern LTPrt *local_prts,*dft_prt;
extern LTPrt *drv_to_prt[32];

#help_index "File/System/Hardware"
extern I64 ATAProbe(I64 base0,I64 base1,I64 unit);
extern U0 BlkDevAdd(LTBlkDev *bdev,BoolI8 one_prt);
extern U0 ATAReadBlks(LTBlkDev *bdev,U0 *buf, U64 blk, U64 cnt);
extern U0 ATAWriteBlks(LTBlkDev *bdev,U0 *buf, U64 blk, U64 cnt);

#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"
#help_file "::/LT/Doc/CmdLine"
public extern BoolI64 RiskyCopy(BoolI8 val);
public extern U0 ChgDsk(U8 drv_let=0);
public extern BoolI64 Drv(U8 drv_let);
public extern BoolI64 Cd(U8 *dirname,BoolI8 make_dirs=FALSE);
public extern BoolI64 MapDrv(U8 drv_let,LTPrt *p);
public extern U0 Dir(U8 *wild="*",BoolI8 full=FALSE);
public extern U0 Del(U8 *wild,BoolI8 print_msg=TRUE);
public extern BoolI64 MkDir(U8 *filename);

#help_index "Info;File/Cmd Line (Typically);Cmd Line (Typically);File/Install Related;Install"
public extern U0 PrtRep();

#help_index "File/Program Routines/Low Level"
extern LTDirEntry *SysFilesFind(U8 *files_find_mask,U64 fuf_flags);
extern U0 SysDel(U8 *wild,BoolI8 make_mask);	//this is dangerous
extern BoolI64 CopySingle(U8 *f1,U8 *f2); //LTust one file
extern U0 CleanMAPBuf(LTPrt *p);
extern U0 FAT32FreeClusters(LTPrt *p,I64 c);
extern U0 LTFSFreeClusters(LTPrt *p,I64 c,I64 cnt);
public extern BoolI64 BlkDevLock(LTBlkDev *bdev);
public extern BoolI64 PrtLock(LTPrt *p);
public extern BoolI64 BlkDevUnlock(LTBlkDev *bdev,BoolI8 rst=FALSE);
public extern BoolI64 PrtUnlock(LTPrt *p,BoolI8 rst=FALSE);
public extern U0 BlkDevsRelease();
public extern U0 PrtsRelease();
public extern I64 ClusterNumNext(LTPrt *p,I64 c,I64 cnt=1);
public extern I64 Cluster2Blk(LTPrt *p,I64 c);
public extern I64 RClustersBlks(LTPrt *p,U0 *buf,I64 c,I64 blks);
public extern I64 RClusters(LTPrt *p,U0 *buf,I64 c,I64 cnt);
public extern I64 WClustersBlks(LTPrt *p,U0 *buf,I64 c,I64 blks,BoolI8 write_cache=FALSE);
public extern I64 WClusters(LTPrt *p,U0 *buf,I64 c,I64 cnt,BoolI8 write_cache=FALSE);
public extern I64 AllocClusters(LTPrt *p,I64 c=0,I64 cnt=1,BoolI8 contiguous=FALSE);
public extern BoolI64 RBlks(LTPrt *p,U0 *buf, I64 blk, I64 cnt);
public extern BoolI64 WBlks(LTPrt *p,U0 *buf, I64 blk, I64 cnt,BoolI8 write_cache=FALSE);
public extern U0 WZeroBlks(LTPrt *p,I64 blk,I64 cnt);
public extern U0 PrtChk(LTPrt *p);
public extern U0 BlkDevChk(LTBlkDev *bdev);
public extern U8 Prt2Drv(LTPrt *p=NULL);
public extern LTPrt *Drv2Prt(U8 drv_let=0);
public extern LTBlkDev *Drv2BlkDev(U8 drv_let=0);
public extern LTBlkDev *Drv2BlkDevFwded(U8 drv_let=0);
extern I64 Name2DirCluster(LTPrt *p,U8 *dirname);
extern I64 Name2ParentDirCluster(LTPrt *p,U8 *dirname);

#help_index "File/Program Routines/LTFile"
public extern LTFile *FOpen(U8 *filename,U8 *flags,I64 cnt=0);
public extern I64 FSize(LTFile *f);
public extern U0 FSync(LTFile *f);
public extern U0 FClose(LTFile *f);
public extern BoolI64 FRBlks(LTFile *f,U0 *buf,I64 blk=LTFB_NEXT_BLK,I64 cnt=1);
public extern BoolI64 FWBlks(LTFile *f,U0 *buf,I64 blk=LTFB_NEXT_BLK,I64 cnt=1);

#help_index "File/CD DVD"
public extern U0 CDImageRead(U8 cd_drv_let,U8 *out_name);
public extern U0 CDImageWrite(U8 cd_drv_let,U8 *in_name=NULL,I64 media_type=MT_CD);
public extern U8 *sys_dft_iso_name;

#help_index "File/FileNames"
public extern BoolI64 FilesFindMatch(U8 *files_find_mask,U8 *filename,U64 fuf_flags=0);
public extern BoolI64 IsTextFile(U8 *filename);
public extern BoolI64 IsSrcFile(U8 *filename);
public extern U8 *FileExtDot(U8 *src);
public extern U8 *FileExtRem(U8 *src,U8 *dst=NULL);
public extern U0 PutFileLink(U8 *filename,I64 line=0,BoolI8 plain_text=FALSE);
public extern U0 PutDirLink(U8 *dirname);
public extern U8 *CurDir(TaskStruct *task=NULL,TaskStruct *mem_task=NULL);
extern U8 *sys_home_dir;
public extern U0 HomeDirRst(U8 *st=NULL);
public extern U8 *DirNameAbs(U8 *dirname);
public extern U8 *DirFile(U8 *dirname,U8 *name=NULL,U8 *_extension=NULL);
public extern U8 *ZorNotZ(U8 *name,U64 ch);
public extern U8 *FileNameAbs(U8 *filename);
public extern BoolI64 FileNameChk(U8 *filename);
public extern U8 *ChgExt(U8 *filename,U8 *extension);
public extern U8 *DftExt(U8 *filename,U8 *extension);
public extern BoolI64 IsDir(U8 *dir_name);
public extern BoolI64 IsDrvWritable(U8 drv_let=0);
extern LTDirEntry *LTCdDir(LTDirEntry *tempm,U8 *abs_name);

#help_index "File/Program Routines/Utils"
public extern U0 GetFFlags(U0 *f,U8 *list,U8 *st);
public extern U8 *SPrintFFlags(U8 *dst,U8 *list,U64 flags);
public extern U0 LTFileAccessDel(LTFileAccess *fa);
public extern LTFileAccess *LTFileAccessNew(U8 *mask,BoolI8 make_mask=FALSE,BoolI8 make_dirs=FALSE);
extern I64 FileAttr(U8 *name,I64 old_attr=0);
public extern U0 LTDirEntryDel(LTDirEntry *tempm);
public extern U0 LTDirEntryDel2(LTDirEntry *tempm);
public extern U0 LTDirListDel(LTDirEntry *tempm);
public extern U0 LTDirListDel2(LTDirEntry *tempm);
public extern BoolI64 LTDirEntryNew(LTPrt *p,U8 *cur_dir,LTDirEntry *tempm,BoolI8 free_old_chain=TRUE);

#help_index "File/Program Routines"
public extern LTDirEntry *FilesFind(U8 *files_find_mask,U64 fuf_flags=0);
public extern U64 FileSize(U8 *filename);
public extern BoolI64 FileFind(U8 *filename,LTDirEntry *de=NULL,
      BoolI8 dir_only=FALSE,BoolI8 file_only=FALSE,BoolI8 scan_parents=FALSE);
public extern U0 *FileRead(U8 *filename,I64 *size=NULL,I64 *attr=NULL,BoolI8 raw=FALSE);
public extern U8 *TextFileRead(U8 *filename,I64 *attr=NULL);
public extern I64 FileWrite(U8 *filename,U0 *fbuf,
   I64 size,LTDate date=0,I64 attr=0,BoolI8 raw=FALSE);
public extern U0 TextFileWrite(U8 *filename,U8 *st,I64 attr=0);

#help_index "Install;File/Install Related;File/Cmd Line (Typically);Cmd Line (Typically);"
public extern U0 Mount(BoolI8 repartition=FALSE);

#help_index "Install;File/Install Related"
extern ATARepStruct *ATARepFind(ATARepStruct *root,I64 num);
public extern BoolI64 DrvEnable(U8 drv_let,BoolI8 val);
public extern U0 Fmt(U8 drv_let,BoolI8 quick=TRUE,BoolI8 confirm=TRUE,I64 type=PT_FAT32);
extern U0 DrvDftSelect();
public extern U8 dft_drv;
extern BoolI8 boot_drv_pmt;

#help_index "Hash"
#help_file "::/LT/Doc/Hash"
public LTextern _HASH_STR U64 HashStr(U8 *st);
public LTextern _HASH_FIND SysHashEntry *
  HashFind(U8 *st,SysHashTable *table,U64 mask,U64 instance=1);
public LTextern _HASH_SINGLE_TABLE_FIND SysHashEntry *
  HashSingleTableFind(U8 *st,SysHashTable *table,U64 mask,U64 instance=1);
public LTextern _HASH_BUCKET_FIND SysHashEntry **
  HashBucketFind(U8 *st,SysHashTable *table);
public LTextern _HASH_ADD U0 HashAdd(SysHashEntry *temph,SysHashTable *table);
public LTextern _HASH_ADD_AFTER U0 HashAddAfter(SysHashEntry *temph,SysHashEntry *pred,SysHashTable *table);
public LTextern _HASH_REPLACE U0 HashReplace(SysHashEntry *temph,SysHashTable *table,
					U64 instance=1);//instance must match temph's
public LTextern _HASH_REM_DEL BoolI64 HashRemDel(SysHashEntry *temph,SysHashTable *table,
					U64 instance=1);//instance must match temph's
public LTextern _EXT_FIND U0 *ExtFind(U8 *st,U64 instance=1);
public extern GenericHashEntry *HashGenericAdd(U8 *name,I64 type,I64 u0=0,I64 u8=0,I64 u16=0,TaskStruct *task=NULL);
public extern SysHashTable *HashTableNew(U64 size,TaskStruct *mem_task=NULL);
public extern U0 HashDel(SrcSymHashEntry *temph);
public extern U0 HashTableDel(SysHashTable *table);
extern U0 SysSymImportsResolve(U8 *sptr,I64 ld_flags=0);

#help_index "Hash/Named Frame"
#help_file "::/LT/Doc/NamedFrame"
public extern I64 NamedFramePtr(U8 *name,TaskStruct *task=NULL);
public extern GenericHashEntry *NamedFramePtrAdd(U8 *name,I64 u0,TaskStruct *task=NULL);
public extern I64 NamedFramePtrSet(U8 *name,I64 u0,TaskStruct *task=NULL);
public extern I64 NamedFramePtrDel(U8 *name,TaskStruct *task=NULL);

#help_index "Compiler;Cmd Line (Typically)"
public extern U0 *Load(U8 *filename,U64 ld_flags=0,
  BinFileHeaderStruct *bfh_addr=-1); //-1 means don't care what load addr.

extern U64 sys_num_installed_compilers;
#help_index "Compiler"
extern U0 (*fp_prs_stream_blk)(LexStruct *lx);

#help_index "Boot"
public extern U0 Reboot();

#help_index "MultiCore"
extern MPCmdCtrl *mp_ctrl;
extern U8 mp_apic_ids[MP_MAX_PROCESSORS];
extern MPCrashStruct *mp_crash;
public extern U0 MPInt(U8 num,I64 cpu_num=1);
public extern U0 IntEOI(); //signal end of interrupt
public extern U0 MPIntAll(U8 num);
public extern U0 MPWake(U64 target_cpu_mask);
extern U0 MPSethServerTask();
extern MPCmdStruct *MPQueueJob(I64 (*fp_add)(U0 *data),U0 *data=NULL,
       U8 *desc=NULL,
       U64 flags=1<<MPCf_FREE_ON_COMPLETE,
       BoolI8 spawn=FALSE,U64 target_cpu_mask=ALL_MASK);
public extern MPCmdStruct *MPJob(I64 (*fp_add)(U0 *data),U0 *data=NULL,
       U64 flags=1<<MPCf_FREE_ON_COMPLETE,
       U64 target_cpu_mask=ALL_MASK);
public extern I64 MPJobResult(MPCmdStruct *tempm);
public extern TaskStruct *MPSpawn(U0 (*fp_add)(U0 *data),U0 *data=NULL,U8 *name=NULL,U64 target_cpu=ALL_MASK);
public extern U0 MPNMInt();
public extern U0 MPStart();
#help_index "MultiCore;Boot"
public extern U0 MPHalt();

#help_index "Math"
public extern F64 Limit(F64 p,F64 lo,F64 hi);
public extern F64 Min(F64 n1,F64 n2);
public extern F64 Max(F64 n1,F64 n2);
public extern F64 Pow10I64(I64 i);

#help_index "PCI"
public extern U8 PCIReadU8(I64 bus,I64 dev,I64 fun,I64 rg);
public extern U16 PCIReadU16(I64 bus,I64 dev,I64 fun,I64 rg);
public extern U32 PCIReadU32(I64 bus,I64 dev,I64 fun,I64 rg);
public extern U0 PCIWriteU8(I64 bus,I64 dev,I64 fun,I64 rg,I64 val);
public extern U0 PCIWriteU16(I64 bus,I64 dev,I64 fun,I64 rg,I64 val);
public extern U0 PCIWriteU32(I64 bus,I64 dev,I64 fun,I64 rg,I64 val);
public extern I64 PCIClassFind(I64 class_code,I64 n);

//These are far call routines in the BIOS
public LTextern SYS_BIOS32_SERVICE_DIR U32 sys_BIOS32_service_dir;
public LTextern SYS_PCI_SERVICES U32 sys_pci_services;

#help_index "PCI;Call"
public LTextern C32_EAX U32 c32_eax;
public LTextern C32_EBX U32 c32_ebx;
public LTextern C32_ECX U32 c32_ecx;
public LTextern C32_EDX U32 c32_edx;
public LTextern C32_ESI U32 c32_esi;
public LTextern C32_EDI U32 c32_edi;
//Not reentrant
public LTextern _FAR_CALL32 BoolI64 FarCall32(U0 (*fp_add)());

#help_index "Call"
//fp_add can have any fixed number of arguments.
public argpop extern I64 CallStkGrow(U64 stk_size_threshold,U64 stk_size,/*argpop*/I64 (*fp_add)(...),...);

#help_index "Fifo"
public extern U8Fifo *U8FifoNew(U64 size,TaskStruct *mem_task=NULL);
public extern U0 U8FifoDel(U8Fifo *f);
public extern BoolI64 U8FifoInsert(U8Fifo *f,U8 ch);
public extern BoolI64 U8FifoRem(U8Fifo *f,U8 *ch);
public extern BoolI64 U8FifoPeek(U8Fifo *f,U8 *ch);
public extern U0 U8FifoFlush(U8Fifo *f);
public extern U64 U8FifoCnt(U8Fifo *f);
public extern U64Fifo *U64FifoNew(U64 size,TaskStruct *mem_task=NULL);
public extern U0 U64FifoDel(U64Fifo *f);
public extern BoolI64 U64FifoInsert(U64Fifo *f,U64 d);
public extern BoolI64 U64FifoRem(U64Fifo *f,U64 *d);
public extern BoolI64 U64FifoPeek(U64Fifo *f,U64 *d);
public extern U0 U64FifoFlush(U64Fifo *f);
public extern U64 U64FifoCnt(U64Fifo *f);

#help_index "Char/Operations"
public LTextern _STRCMP I64 StrCmp(U8 *st1,U8 *st2);
public LTextern _STRICMP I64 StrICmp(U8 *st1,U8 *st2);
public LTextern _STRNCMP I64 StrNCmp(U8 *st1,U8 *st2,U64 n);
public LTextern _STRNICMP I64 StrNICmp(U8 *st1,U8 *st2,U64 n);
public LTextern _STRSTR U8 *StrStr(U8 *st,U8 *pattern);
public LTextern _STRISTR U8 *StrIStr(U8 *st,U8 *pattern);
public LTextern _STRCPY U0 StrCpy(U8 *dest,U8 *src);
public LTextern _STRCAT U0 StrCat(U8 *dest,U8 *src);

#help_index "Char/Input"
public extern U64 sys_key_cnt;
public extern U0 SetTypeMatic(U8 delay);
extern U0 KbdInit();
extern U64 KbdQueueMsgs();
extern U0 KbdMouseVarsInit();
extern BoolI64 MouseDriverInstall();
extern U0 KbdMouseHandler(BoolI8 poll_kbd,BoolI8 poll_mouse);
public extern U32 key_down_bitmap[8],key_down_bitmap2[8];

#help_index "Char/BitMaps"
public extern U32 alpha_bitmap[16];
public extern U32 alpha_numeric_bitmap[16];
public extern U32 alpha_numeric_no_at_bitmap[16];
public extern U32 word_bitmap[16];
public extern U32 dec_numeric_bitmap[16];
public extern U32 hex_numeric_bitmap[16];
public extern U32 white_space_bitmap[16];
public extern U32 non_eol_white_space_bitmap[16];
public extern U32 zero_cr_nl_cursor_bitmap[16];
public extern U32 zero_tab_ff_cr_nl_cursor_bitmap[16];
public extern U32 zero_tab_ff_cr_nl_cursor_dollar_bitmap[16];
public extern U32 getkey_chars_bitmap[16];
public extern U32 printable_chars_bitmap[16];
public extern U32 displayable_chars_bitmap[16];
public extern U32 safe_dollar_chars_bitmap[16];
public extern U32 filename_chars_bitmap[16];
public extern U32 non_eol_chars_bitmap[16];

#help_index "Char/Conversion"
public extern U64 A2ScanCode(U64 ch,U64 sc_flags=0);
public extern U8 ScanCode2A(U64 sc);
public extern U8 *A2KeyName(U64 ch,BoolI8 include_ctrl=TRUE);
public extern U8 *ScanCode2KeyName(U64 sc);

#help_index "Input Pointer/Mouse"
public extern F64 mxx_scale,myy_scale,mzz_scale;

//These are the coordinates before
//they get scaled.
public extern I64 mxx_prescale,myy_prescale,mzz_prescale;
public extern I64 mxx,myy,mzz;
public extern F64 mouse_speed;
extern I64 old_mxx,old_myy,old_mzz;
public extern BoolI8 mouse_buttons[5];
extern BoolI8 mouse_installed;
extern U64     mouse_install_attempts,
       mouse_last_evt_time;
extern BoolI8 mouse_has_wheel;
extern BoolI8 mouse_has_ext_buttons;
extern BoolI8 mouse_evt;
extern BoolI8 mouse_install_attempt_in_progress;
extern U64Fifo *scan_code_fifo;
extern U64 sys_last_down_scan_code;
extern BoolI8 kbdmouse_rst;
extern BoolI8 mouse_irqs_working;

#help_index "Input Pointer"
#help_file "::/LT/Doc/InputPointer"
public extern I64 ipx,ipy,ipz,ip_throttle;
public extern I64 ipx_offset,ipy_offset,ipz_offset;
public extern F64 ipx_scale,ipy_scale,ipz_scale;
public extern F64 ip_speed;
public extern I64 ip_dev;
public extern I64 iptx,ipty;
public extern BoolI8 ip_lb,ip_cb,ip_rb;
extern BoolI8 ip_lb_last,ip_cb_last,ip_rb_last;
public extern BoolI8 ip_has_wheel;
extern LTDate ip_last_move_time;
extern SnapStruct ip_snap_last;
public extern U0 SetIP(I64 x=MAX_I64,I64 y=MAX_I64,I64 z=MAX_I64,I64 l=MAX_I64,I64 r=MAX_I64,I64 c=MAX_I64);
#help_index "Input Pointer/Displayed Image"
public extern BoolI8 ip_on;  //Show or hide cursor
extern U0 IPVarsUpdate(I64 x,I64 y,I64 z,BoolI8 l,BoolI8 c,BoolI8 r);
extern U0 IPVarsInit();
#help_index "Input Pointer/Snap"
public extern I64 ipx_presnap,ipy_presnap,ipz_presnap;
extern SnapStruct *SnapCur();
public extern U0 GetSnap(SnapStruct *s);
public extern U0 SetSnap(SnapStruct *s,I64 i=0,F64 d=0);

#help_index "Messages"
extern U0 InputFilterTask();
public extern I64 ScanMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,TaskStruct *task=NULL);
public extern U64 FlushMsgs(TaskStruct *task=NULL);
public extern I64 GetMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,TaskStruct *task=NULL);
public extern U64 ScanChar();

#help_index "Misc;Messages"
public extern U64 sys_cur_scan_code;

#help_index "USB"
extern I64 sys_num_usb;
extern LTUSB sys_usb_devs[MAX_USB];
extern USBTD *USBAllocTD();
extern U0 USBFreeTD(USBTD *temptd);
extern U0 USBEndAll();
extern U0 USBInitAll();
extern U32 *USBAllocFrameList(U64 usb_num,U64 size);

#help_index "Ltf/Task"
extern Ltf *(*fp_ltf_cur)(TaskStruct *task=NULL);

#help_index "Char/Conversion"
public extern F64 A2F64(U8 *src,U8 **_end_ptr=NULL);

#help_index "Time/LTDate"
public extern U8 *date_fmt_map,*time_fmt_map;
public extern U0 DateTimeFmtMapsRst();

#help_index "Char/Conversion;Time/LTDate"
public extern LTDate A2LTDate(U8 *src);

#help_index "Hash"
public extern U64 HashTableSize(SysHashTable *table);

#help_index "Debugging;Hash"
public extern SysHashEntry *PSectFind(U0 *add,I64 *offset);
extern PSectCacheEntry *psect_cache;
extern I64 psect_cache_index;

#help_index "Debugging"
public extern U0 EdLite(U8 *filename);
public extern I64 SrcLineNum(U0 *add,U64 cnt=1);
public extern U8 *SrcFileName(U0 *add,U64 cnt=1,TaskStruct *mem_task=NULL);
public extern U8 *SrcEdLink(U0 *add,U64 cnt=1,TaskStruct *mem_task=NULL);
public extern BoolI64 PutSrcLink(U0 *add,U64 cnt=1,U8 *buf=NULL);
public extern Ltf *EdAddSrc(U0 *add,U64 cnt=1,BoolI8 bail=FALSE);

#help_index "Exceptions"
public extern U0 CatchAll();

#help_index "Task/Requests;Compiler"
public extern U0 UserTaskCont();
public extern I64 ExeCmdLine(LexStruct *lx);

#help_index ""
