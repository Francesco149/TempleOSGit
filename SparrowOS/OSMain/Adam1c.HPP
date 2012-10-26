#help_index "Time/CDate"
#help_file "::/SparrowOS/Doc/TimeCDate"
public extern CDate local_time_offset;

#help_index "Time/Jiffies"
//18.33333*65536Hz (SYS_TIMER_FREQ)
public extern U64 SysTimerRead();

#help_index "Task"
extern U8 *sys_acct;
extern CTask *sys_winmgr_task,*sys_task_being_screen_updated;
public extern CTask *adam_task;
public extern I64 sys_num_spawned_users;
extern U0 TaskFinishOffDying();
public _extern SYS_SAVE_CONTEXT_YIELD U0 Yield();
public extern U0 TSSBusy(I64 tr,BoolI8 val=OFF);
_extern SYS_IDLE_START U8 sys_idle_start;
_extern SYS_IDLE_END	U8 sys_idle_end;
_extern SYS_TASK_END_NOW U0 TaskEndNow();


#help_index "Time/CPU Cycles"
#help_file "::/SparrowOS/Doc/TimeCycles"
public extern U64 time_stamp_freq,time_stamp_kHz_freq;
public extern U64 TaskTotalTime();

//This is sampled once at boot time
public extern U64 time_stamp_freq_initial;

#help_index "Time/HPET"
#help_file "::/SparrowOS/Doc/TimeHPET"
public extern U64 HPET_freq,HPET_kHz_freq;

#help_index "Time/Seconds"
public extern F64 tNP(); //High freq.  No accounting for task Pause.
public extern F64 tJ(); //From Jiffies.
public extern F64 tS(); //From SysTimerFreq.
public extern F64 tP(CTask *task=NULL); //High freq.  Accounts for task pause.
public extern BoolI64 Blink(F64 Hz=2.5,F64 t=0);

#help_index "Snd"
#help_file "::/SparrowOS/Doc/Snd"
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
extern U8 *sys_fault_handlers_code;
public extern U0 *GetIDTEntry(U64 irq,I64 cpu_num=-1);
public extern U0 *SetIDTEntry(U64 irq,U0 (*fp_new_handler)(),I64 cpu_num=-1,U64 type=IDTET_IRQ,U64 dpl=0);
extern CPCIDev sys_pci_dev_root;
extern CMemRange sys_mem32_dev_root;

#help_index "Task/Delay;Time/CPU Cycles"
public extern U64 TimeStampFreqCal();
public extern U0 BusyWait(U64 uS); //Loosely timed
public extern U0 SleepUntil(U64 wake_jiffy);
public extern U0 Sleep(U64 mS);
public extern U0 PortNop(); //On the order of 1uS
#help_index "Task"
public extern BoolI64 IsPreempt();

#help_index "Debugging"
#help_file "::/SparrowOS/Doc/Dbg"

extern CTask *sys_dbg_focus_task;
public extern U0 D(U0 *buf,U64 cnt=0x80,BoolI8 show_offset=TRUE);
public extern U0 Dm(U0 *buf,U64 cnt=0x80);
public extern U0 Da(U0 **b,U64 cnt=0x10);
extern U64 *TaskRegAdd(CTask *task,I64 reg_num);
public extern U0 Dr(CTask *task=NULL);
public extern BoolI64 IsDbgTask();
public extern BoolI64 IsSysDbg();
public extern BoolI64 IsSysDeterministic();
public extern BoolI64 SysDbg(BoolI8 val);
public extern BoolI64 SysDeterministic(BoolI8 val);
public extern BoolI64 ChkPtr(U0 *ptr);
public extern BoolI64 ChkOnStk(U0 *ptr,CTask *task=NULL);
public extern I64 UnusedStk(CTask *task=NULL);
public extern U0 StkRep(CTask *task=NULL);
public extern U0 CallerRep(U0 **rbp=NULL,CTask *task=NULL);
public extern U0 *Caller(U64 num=1);
public extern U0 *TaskCaller(CTask *task=NULL,I64 num=0,BoolI8 saved_context=FALSE);
public extern U0 Dbg(U8 *msg=NULL,U64 num=0,BoolI8 fault=FALSE);
public extern U0 Go1();
public extern U64 sys_dbg_fault_num;
public extern U0 *sys_dbg_fault_rip;
#help_index "Debugging/Raw Output"
public extern U0 RawDrawChar(U64 ch);
public extern U0 VGAFlush();
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
public extern U8 *Color2Text(U8 *buf,CColorROPU32 c);

#help_index "Char"
#help_file "::/SparrowOS/Doc/Char"
extern U0 (**fp_ctrl_alt_let_cbs)(U64 sc);
extern U64 ctrl_alt_let_in_irq_flags;
extern U8 **ctrl_alt_let_descs;
extern U64 **ctrl_alt_let_ret_addr; //addr of ret add on stack in kbd irq
extern CTaskCmd sys_macro_root;
extern CKeyDev sys_put_key_root;
extern CTask *sys_macro_util_task;
extern U64 sys_raw_dc_col,sys_raw_dc_flags;
extern U8 *sys_dc_screen_image;
public extern BoolI8 text_mode;

//Constants are defined for these to
//optimize.  See [::/SparrowOS/Adam/Gr/GrCfg.CPP.Z,1] ::/SparrowOS/Adam/Gr/GrCfg.CPP.Z.
public extern I64 text_rows,text_cols,gr_width,gr_height;
public extern U8 *gr_font;

public extern CKeyDev *KeyDevAdd(BoolI64 (*fp_put_key)(U64 ch,U64 sc),BoolI64 (*fp_puts)(U8 *st),U64 priority,BoolI8 key_descs=FALSE);
public extern U0 KeyDevRem(CKeyDev *tempp);
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
public extern U64 KbdMouseEvtTime();

//Gets [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1461] Flags
public extern U8 *MGetS(U64 flags=0);
extern U8  *(*fp_mgets)(U64 flags=0);
extern U8 *GetFJoin(U8 *src,U8 *fmt,I64 argc,I64 *argv);
public extern U8 *GetF(U8 *src,U8 *fmt,...);

#help_index "Char/Lists"
public extern U8 *ListEntryPt(I64 entry_num, U8 *lst);
public extern I64 ListEntryMatch(U8 *src, U8 *lst,I64 flags=0,I64 *_ambiguity_overkill=NULL);

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
#help_file "::/SparrowOS/Doc/Define"
public extern CHashDefineStr *DefineLoad(U8 *name,U8 *st,BoolI8 replace=FALSE);
public extern CHashDefineStr *DefineListLoad(U8 *name,U8 *st,BoolI8 replace=FALSE);
public extern U0 DefinePrintF(U8 *name,U8 *src,...);
public extern U8 *Define(U8 *name);
public extern U8 *DefineSub(U64 sub,U8 *name);
public extern I64 DefineSubCnt(U8 *name);
public extern U0 PutDefine(U8 *name);
#help_index "Char/Define;Char/Lists"
public extern I64 DefineMatch(U8 *src,U8 *lst_name,U64 flags=0,I64 *_ambiguity_overkill=NULL);

#help_index "Memory"
#help_file "::/SparrowOS/Doc/Mem"
public _extern _MEMCMP I64 MemCmp(U8 *ptr1,U8 *ptr2,U64 cnt);
public extern U64 MemPageSize(U0 *a)
public extern BoolI64 MemPagePresentMark(U0 *a,BoolI8 val);
public extern BoolI64 MemLowPagesProtect(BoolI8 val);
public extern U0 *MemBlks2MegAlloc(U64 *_pages2Meg,CBlkPool *bp=NULL);
public extern U0 *MemBlksUncachedAlloc(U64 *_pages2Meg,CBlkPool *bp=NULL);
public extern U0 *MemBlksWriteThroughAlloc(U64 *_pages2Meg,CBlkPool *bp=NULL);
public extern U0 MemBlks2MegFree(CMemBlk *m,U64 pages2Meg,CBlkPool *bp=NULL);
public extern U0 *MemBlksNonTaskAlloc(U64 *_pages512,CBlkPool *bp=NULL);
public extern U64 *MemPtAtPageTableEntry(U0 *a);
public extern U0 MemBlksNonTaskFree(CMemBlk *m,U64 pages512,CBlkPool *bp=NULL);
public _extern _MALLOC U0 *MAlloc(U64 size,CTask *mem_task=NULL);
public _extern _FREE U0 Free(U0 *add);
public _extern _MSIZE U64 MSize(U0 *src);	//size of heap object
public _extern _MSIZE2 U64 MSize2(U0 *src); //Internal size
public _extern _MHEAP_CTRL CHeapCtrl *MHeapCtrl(U0 *src);
public extern U0 *CAlloc(U64 size,CTask *mem_task=NULL);
public extern U0 *MAllocIdentical(U0 *src,CTask *mem_task=NULL);
public extern U0 *MAllocAligned(U64 size,U64 alignment,CTask *mem_task=NULL,U64 misalignment=0);
public extern U0 *CAllocAligned(U64 size,U64 alignment,CTask *mem_task=NULL,U64 misalignment=0);
public extern U0 LinkedListDel(U0 **_list);
public extern U0 *LinkedListCopy(U0 **_list,CTask *mem_task=NULL);
public extern U64 LinkedListCnt(U0 **_list);
public extern U64 LinkedListSize(U0 **_list);
public extern I64 Scale2Mem(I64 min,I64 max,I64 limit=2*1024*1024*1024);
#help_index "Memory;Circular Queue"
public extern U0 QueDel(CQue *root,BoolI8 querem=FALSE);
public extern CQue *QueCopy(CQue *root,CTask *mem_task=NULL);
public extern U64 QueCnt(CQue *root);
public extern U64 QueSize(CQue *root);
#help_index "Memory/Heap Ctrl"
public extern U0 BlkPoolAdd(CBlkPool *bp,CMemBlk *m,U64 pages512);
public extern U0 BlkPoolInit(CBlkPool *bp,U64 pages512);
extern CHeapCtrl *TaskHeapCtrlNew(CTask *task,CBlkPool *bp);
extern U0 TaskHeapCtrlDel(CHeapCtrl *hc);
public extern CHeapCtrl *HeapCtrlIndependentInit(CBlkPool *bp,U64 pages512);
#help_index "Memory/Adam Heap"
public extern U0 *AMAlloc(U64 size);
public extern U0 *AMAllocIdentical(U0 *src);
public extern U0 *ACAlloc(U64 size);

#help_index "Char/Operations;Memory"
public extern U8 *StrNew(U8 *buf,CTask *mem_task=NULL);
#help_index "Char/Operations;Memory/Adam Heap"
public extern U8 *AStrNew(U8 *buf);

#help_index "Device"
public extern U0 *Mem32DevAlloc(U64 size,U64 alignment);
public extern U0 *Mem64DevAlloc(U64 *_pages2Meg);
public extern U0 Mem32DevFree(U8 *base);
public extern U0 Mem64DevFree(U8 *base,U64 pages2Meg);
public extern CHeapCtrl *sys_uncached_heap;

#help_index "Task"
public extern U0 Exit();
public extern U0 Kill(CTask *task,BoolI8 wait=TRUE);
public extern BoolI64 TaskValidate(CTask *task);
public extern CTask *Spawn(U0 (*fp_add)(U0 *data),U0 *data=NULL,
	    U8 *task_name=NULL,
	    CTask *parent=NULL, //NULL means adam
	    U64 stk_size=0,U64 flags=SPAWNF_ADD_TO_QUE);
public extern U0 TaskPause(CTask *task,BoolI8 state);
public extern U0 TaskQueIns(CTask *task,CTask *pred=NULL);
public extern U0 TaskQueRem(CTask *task);
public extern U0 TaskWait(CTask *task=NULL);
public extern U0 TaskInitExt();
public extern U0 UserCmdLine();
public extern U0 WinDerivedValsUpdate(CTask *task=NULL);
public extern U0 TaskDerivedValsUpdate(CTask *task=NULL,BoolI8 update_z_buf=TRUE);
extern U0 (*fp_update_ctrls)(CTask *task);
public extern CTask *SpawnUser(U8 *fmt=NULL,...);
extern U0 TaskFinishOff(CTask *task);
public extern U0 OverRideSingleUserSwap();

#help_index "Task/Requests"
public extern U0 TaskRstAwaitingMsg(CTask *task=NULL);
public extern CTaskCmd *
  TaskRqstQue(CTask *servant,CTask *master,
		     U8 *data,U64 flags);
public extern U64 ServantCmdsHandler(CTask *task=NULL);
public extern I64 ServantUser(U8 *fmt,...);
extern U0 ServantUserCmdLine();
public extern BoolI64 GetRqstResult(CTaskCmd *rqst=NULL,U64 *result=NULL);
public extern U0 ServantTaskCont();
public extern I64 Adam(U8 *fmt,...);
public extern U0 AdamLogExcept(I64 argc,I64 *argv);
public extern U0 AdamLog(U8 *fmt,...);
public extern U0 AdamErr(U8 *fmt,...);
#help_index "Task/Text Input"
public extern CTaskCmd *
  TextInputQue(CTask *servant,CTask *master,
		     U8 *data,U64 flags);
#help_index "Task/Text Input;Messages"
public extern CTaskCmd *QueMsg(CTask *servant,CTask *master,
		     I64 msg_code,I64 p1,I64 p2,U64 flags);
public extern U0 PostMsg(CTask *task,I64 msg_code,I64 p1,I64 p2,U64 flags=0);
public extern U0 SendMsg(CTask *task,I64 msg_code,I64 p1,I64 p2,U64 flags=0);
public extern U0 Msg(I64 msg_code,I64 p1,I64 p2,U64 flags=0);

#help_index "Exceptions"
//Exceptions are thrown through the following funcation call.
//by convention, the first arg should be a type code.
public extern U0 PutExcept();
public extern U0 throw(...);
public extern U0 throw_no_log(...);
public extern U64 ExceptDepth(CTask *task=NULL);
//The following are used internally and should not be called directly
extern U0 SysTry(U0 *start_label,U0 *skip_label);
extern U0 SysUntry();
public extern U0 ThrowBreak(CTask *task=NULL);
public extern BoolI64 BreakLock(CTask *task=NULL);
public extern BoolI64 BreakUnlock(CTask *task=NULL);

#help_index "Compression/Piece by Piece"
public extern U0 ArcCompressBuf(CArcCtrl *c);
public extern U0 ArcFinishCompression(CArcCtrl *c);
public extern U0 ArcExpandBuf(CArcCtrl *c);
public extern CArcCtrl *ArcCtrlNew(BoolI8 expand,BoolI8 text_only);
public extern U0 ArcCtrlDel(CArcCtrl *c);
#help_index "Compression"
public extern CArcCompress *
  CompressBuf(U0 *src,I64 size,U64 flags=0,CTask *mem_task=NULL);
public extern U0 *ExpandBuf(CArcCompress *r,CTask *mem_task=NULL);
//This is just an XOR checksum.
extern U64 ArcXSum(U32 *buf,U64 size);

#help_index "AutoFile"
#help_file "::/SparrowOS/Doc/AutoFile"
#help_index "Task/Text Input;AutoFile"
public extern U0 Auto(U8 *fmt,...);
public extern U0 AutoStr(U8 *fmt,...);
public extern U0 AutoFile(U8 *filename);
public extern U0 XTalk(CTask *task,U8 *fmt,...);
public extern U0 XTalkWait(CTask *task,U8 *fmt,...);
public extern U0 XTalkStr(CTask *task,U8 *fmt,...);
public extern U0 XTalkStrWait(CTask *task,U8 *fmt,...);

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

public extern U16 EndianU16(U16 d);
public extern U32 EndianU32(U32 d);
public extern U64 EndianU64(U64 d);
extern U64 sys_rnd_seed;

#help_index "Bit"
public extern I64 BCnt(U64 d);
public extern U8 *reverse_bits_table,*set_bits_table;

#help_index "Doc/Clipboard"
public extern CDoc *sys_clipboard_doc;

#help_index "Time/CDate"
public extern U16 mon_start_days1[12];
public extern U16 mon_start_days2[12];
public extern I64 YearStartDate(I64 year);
public extern CDate CDateStruct2CDate(CDateStruct *_ds);
public extern U0 CDate2CDateStruct(CDateStruct *_ds,CDate ltdt);
public extern I64 FirstDayOfMon(I64 i);
public extern I64 LastDayOfMon(I64 i);
public extern I64 FirstDayOfYear(I64 i);
public extern I64 LastDayOfYear(I64 i);
public extern I64 DayOfWeek(I64 i);
public extern CDate GetCurTimeCDate();
public extern U0 CDate2ISO1(CISO1Date *dst,CDate ltdt);

#help_index "File/System"
extern U8 sys_temp_filename[136];
extern U64 sys_dsk_reads,sys_dsk_writes;

#help_index "File/System/Cache"
extern CCacheBlk *sys_dsk_cache_base,*sys_dsk_cache_ctrl;
extern CCacheBlk **sys_dsk_cache_hash_table;
extern U64 sys_dsk_cache_size;

extern U0 DskCacheInit(U64 size_in_U8s);
public extern U0 DskCacheInvalidate(CPrt *p);

#help_index "File/System/BlkDev"
public extern I64 num_blkdevs;
public extern CBlkDev *sys_blkdevs;

#help_index "File/System/Partitions"
public extern CPrt *sys_prts,*dft_prt;
extern CPrt *drv_to_prt[32];

#help_index "File/System/Hardware"
extern I64 ATAProbe(I64 base0,I64 base1,I64 unit);
extern U0 BlkDevAdd(CBlkDev *bdev,BoolI8 one_prt);
extern U0 ATAReadBlks(CBlkDev *bdev,U0 *buf, U64 blk, U64 cnt);
extern U0 ATAWriteBlks(CBlkDev *bdev,U0 *buf, U64 blk, U64 cnt);

#help_index "File/Cmd Line (Typically);Cmd Line (Typically)"
#help_file "::/SparrowOS/Doc/CmdLine"
public extern U0 ChgDsk(U8 drv_let=0);
public extern BoolI64 Drv(U8 drv_let);
public extern BoolI64 Cd(U8 *dirname,BoolI8 make_dirs=FALSE);
public extern BoolI64 MapDrv(U8 drv_let,CPrt *p);
public extern U0 Dir(U8 *wild="*",BoolI8 full=FALSE);
public extern U0 Del(U8 *wild,BoolI8 print_msg=TRUE);
public extern BoolI64 MkDir(U8 *filename);

#help_index "Info;File/Cmd Line (Typically);Cmd Line (Typically);File/Install Related;Install"
public extern U0 PrtRep();

#help_index "File/Program Routines/Low Level"
extern CDirEntry *SysFilesFind(U8 *files_find_mask,U64 fuf_flags);
extern U0 SysDel(U8 *wild,BoolI8 make_mask);	//this is dangerous
extern BoolI64 CopySingle(U8 *f1,U8 *f2); //Just one file
extern U0 CleanMAPBuf(CPrt *p);
extern U0 FAT32FreeClusters(CPrt *p,I64 c);
extern U0 NativeFSFreeClusters(CPrt *p,I64 c,I64 cnt);
public extern BoolI64 BlkDevLock(CBlkDev *bdev);
public extern BoolI64 PrtLock(CPrt *p);
public extern BoolI64 BlkDevUnlock(CBlkDev *bdev,BoolI8 rst=FALSE);
public extern BoolI64 PrtUnlock(CPrt *p,BoolI8 rst=FALSE);
public extern U0 BlkDevsRelease();
public extern U0 PrtsRelease();
public extern I64 ClusterNumNext(CPrt *p,I64 c,I64 cnt=1);
public extern I64 Cluster2Blk(CPrt *p,I64 c);
public extern I64 RClustersBlks(CPrt *p,U0 *buf,I64 c,I64 blks);
public extern I64 RClusters(CPrt *p,U0 *buf,I64 c,I64 cnt);
public extern I64 WClustersBlks(CPrt *p,U0 *buf,I64 c,I64 blks);
public extern I64 WClusters(CPrt *p,U0 *buf,I64 c,I64 cnt);
public extern I64 ClusersAlloc(CPrt *p,I64 c=0,I64 cnt=1,BoolI8 contiguous=FALSE);
public extern BoolI64 RBlks(CPrt *p,U0 *buf, I64 blk, I64 cnt);
public extern BoolI64 WBlks(CPrt *p,U0 *buf, I64 blk, I64 cnt);
public extern U0 WZeroBlks(CPrt *p,I64 blk,I64 cnt);
public extern U0 PrtChk(CPrt *p);
public extern U0 BlkDevChk(CBlkDev *bdev);
public extern U8 Prt2Drv(CPrt *p=NULL);
public extern CPrt *Drv2Prt(U8 drv_let=0);
public extern CBlkDev *Drv2BlkDev(U8 drv_let=0);
public extern CBlkDev *Drv2BlkDevFwded(U8 drv_let=0);
extern I64 Name2DirCluster(CPrt *p,U8 *dirname);
extern I64 Name2ParentDirCluster(CPrt *p,U8 *dirname);

#help_index "File/Program Routines/CFile"
public extern CFile *FOpen(U8 *filename,U8 *flags,I64 cnt=0);
public extern I64 FSize(CFile *f);
public extern U0 FSync(CFile *f);
public extern U0 FClose(CFile *f);
public extern BoolI64 FRBlks(CFile *f,U0 *buf,I64 blk=FFB_NEXT_BLK,I64 cnt=1);
public extern BoolI64 FWBlks(CFile *f,U0 *buf,I64 blk=FFB_NEXT_BLK,I64 cnt=1);

#help_index "File/CD DVD"
public extern U0 CDImageRead(U8 cd_drv_let,U8 *out_name);
public extern U0 CDImageWrite(U8 cd_drv_let,U8 *in_name=NULL,I64 media_type=MT_CD);
public extern U8 *sys_dft_iso_name;

#help_index "File/FileNames"
public extern BoolI64 FilesFindMatch(U8 *files_find_mask,U8 *filename,U64 fuf_flags=0);
public extern BoolI64 IsDotZ(U8 *filename);
public extern BoolI64 IsDotC(U8 *filename);
public extern BoolI64 IsTextFile(U8 *filename);
public extern BoolI64 IsSrcFile(U8 *filename);
public extern U8 *FileExtDot(U8 *src);
public extern U8 *FileExtRem(U8 *src,U8 *dst=NULL);
public extern U0 PutFileLink(U8 *filename,I64 line=0,BoolI8 plain_text=FALSE);
public extern U0 PutDirLink(U8 *dirname);
public extern U8 *CurDir(CTask *task=NULL,CTask *mem_task=NULL);
public extern U8 *DirNameAbs(U8 *dirname);
public extern U8 *DirFile(U8 *dirname,U8 *name=NULL,U8 *_extension=NULL);
public extern U8 *ZorNotZ(U8 *name);
public extern U8 *FileNameAbs(U8 *filename);
public extern BoolI64 FileNameChk(U8 *filename);
public extern U8 *ChgExt(U8 *filename,U8 *extension);
public extern U8 *DftExt(U8 *filename,U8 *extension);
public extern BoolI64 IsDir(U8 *dir_name);
public extern BoolI64 IsDrvWritable(U8 drv_let=0);
extern CDirEntry *CdDir(CDirEntry *tempm,U8 *abs_name);

#help_index "File/Program Routines/Utils"
public extern U0 GetFFlags(U0 *f,U8 *list,U8 *st);
public extern U8 *SPrintFFlags(U8 *dst,U8 *list,U64 flags);
public extern U0 FileAccessDel(CFileAccess *fa);
public extern CFileAccess *FileAccessNew(U8 *mask,BoolI8 make_mask=FALSE,BoolI8 make_dirs=FALSE);
extern I64 FileAttr(U8 *name,I64 old_attr=0);
public extern U0 CDirDel(CDirEntry *tempm);
public extern U0 CDirDel2(CDirEntry *tempm);
public extern U0 CDirListDel(CDirEntry *tempm);
public extern U0 CDirListDel2(CDirEntry *tempm);
public extern BoolI64 CDirNew(CPrt *p,U8 *cur_dir,CDirEntry *tempm,BoolI8 free_old_chain=TRUE);

#help_index "File/Program Routines"
public extern CDirEntry *FilesFind(U8 *files_find_mask,U64 fuf_flags=0);
public extern U64 FileSize(U8 *filename);
public extern U64 FileSizeExpanded(U8 *filename);
public extern BoolI64 FileFind(U8 *filename,CDirEntry *de=NULL,
      BoolI8 dir_only=FALSE,BoolI8 file_only=FALSE,BoolI8 scan_parents=FALSE);
public extern U0 *FileRead(U8 *filename,I64 *_size=NULL,I64 *_attr=NULL,BoolI8 raw=FALSE);
public extern U8 *TextFileRead(U8 *filename,I64 *_attr=NULL);
public extern I64 FileWrite(U8 *filename,U0 *fbuf,
   I64 size,CDate ltdt=0,I64 attr=0,BoolI8 raw=FALSE);
public extern U0 TextFileWrite(U8 *filename,U8 *st,I64 attr=0);

#help_index "Install;File/Install Related;File/Cmd Line (Typically);Cmd Line (Typically);"
public extern U0 Mount(BoolI8 repartition=FALSE);

#help_index "Install;File/Install Related"
extern CATARep *ATARepFind(CATARep *root,I64 num);
public extern BoolI64 DrvEnable(U8 drv_let,BoolI8 val);
public extern U0 Fmt(U8 drv_let,BoolI8 quick=TRUE,BoolI8 confirm=TRUE,I64 type=PT_FAT32);
extern U0 DrvDftSelect();
public extern U8 dft_drv;
extern BoolI8 boot_drv_pmt;

#help_index "Hash"
#help_file "::/SparrowOS/Doc/Hash"
public _extern _HASH_STR U64 HashStr(U8 *st);
public _extern _HASH_FIND CHash *
  HashFind(U8 *st,CHashTable *table,U64 mask,U64 instance=1);
public _extern _HASH_SINGLE_TABLE_FIND CHash *
  HashSingleTableFind(U8 *st,CHashTable *table,U64 mask,U64 instance=1);
public _extern _HASH_BUCKET_FIND CHash **
  HashBucketFind(U8 *st,CHashTable *table);
public _extern _HASH_ADD U0 HashAdd(CHash *temph,CHashTable *table);
public _extern _HASH_ADD_AFTER U0 HashAddAfter(CHash *temph,CHash *pred,CHashTable *table);
public _extern _HASH_REPLACE U0 HashReplace(CHash *temph,CHashTable *table,
					U64 instance=1);//instance must match temph's
public _extern _HASH_REM_DEL BoolI64 HashRemDel(CHash *temph,CHashTable *table,
					U64 instance=1);//instance must match temph's
public _extern _EXT_FIND U0 *ExtFind(U8 *st,U64 instance=1);
public extern CHashGeneric *HashGenericAdd(U8 *name,I64 type,I64 u0=0,I64 u8=0,I64 u16=0,CTask *task=NULL);
public extern I64 HashTypeNum(CHash *temph);
public extern CHashTable *HashTableNew(U64 size,CTask *mem_task=NULL);
public extern U0 HashDel(CHashSrcSym *temph);
public extern U0 HashTableDel(CHashTable *table);
extern U0 SysSymImportsResolve(U8 *sptr,I64 ld_flags=0);

#help_index "Hash/Named Frame"
#help_file "::/SparrowOS/Doc/NamedFrame"
public extern I64 NamedFramePtr(U8 *name,CTask *task=NULL);
public extern CHashGeneric *NamedFramePtrAdd(U8 *name,I64 u0,CTask *task=NULL);
public extern I64 NamedFramePtrSet(U8 *name,I64 u0,CTask *task=NULL);
public extern I64 NamedFramePtrDel(U8 *name,CTask *task=NULL);

#help_index "Compiler;Cmd Line (Typically)"
public extern U0 *Load(U8 *filename,U64 ld_flags=0,
  CBinFile *bfh_addr=-1); //-1 means don't care what load addr.

extern U64 sys_num_installed_compilers;
#help_index "Compiler"
extern U0 (*fp_prs_stream_blk)(CLex *lx);

#help_index "Boot"
public extern U0 Reboot();

#help_index "MultiCore"
extern CMPCmdCtrl *mp_ctrl;
extern U8 mp_apic_ids[MP_MAX_PROCESSORS];
extern CMPCrash *mp_crash;
public extern U0 MPInt(U8 num,I64 cpu_num=1);
public extern U0 IntEOI(); //signal end of interrupt
public extern U0 MPIntAll(U8 num);
public extern U0 MPWake(U64 target_cpu_mask);
extern U0 MPSethServerTask();
extern CMPCmd *MPQueJob(I64 (*fp_add)(U0 *data),U0 *data=NULL,
       U8 *desc=NULL,
       U64 flags=1<<MPCf_FREE_ON_COMPLETE,
       BoolI8 spawn=FALSE,U64 target_cpu_mask=ALL_MASK);
public extern CMPCmd *MPJob(I64 (*fp_add)(U0 *data),U0 *data=NULL,
       U64 target_cpu=1,U64 flags=1<<MPCf_FREE_ON_COMPLETE);
public extern I64 MPJobResult(CMPCmd *tempm);
public extern CTask *MPSpawn(U0 (*fp_add)(U0 *data),U0 *data=NULL,U8 *name=NULL,U64 target_cpu=1);
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
public _extern SYS_BIOS32_SERVICE_DIR U32 sys_BIOS32_service_dir;
public _extern SYS_PCI_SERVICES U32 sys_pci_services;

#help_index "PCI;Call"
public _extern C32_EAX U32 c32_eax;
public _extern C32_EBX U32 c32_ebx;
public _extern C32_ECX U32 c32_ecx;
public _extern C32_EDX U32 c32_edx;
public _extern C32_ESI U32 c32_esi;
public _extern C32_EDI U32 c32_edi;
//Not reentrant
public _extern _FAR_CALL32 BoolI64 FarCall32(U0 (*fp_add)());

#help_index "Call"
//fp_add can have any fixed number of arguments.
public argpop extern I64 CallStkGrow(U64 stk_size_threshold,U64 stk_size,/*argpop*/I64 (*fp_add)(...),...);

#help_index "Fifo"
public extern CFifoU8 *FifoU8New(U64 size,CTask *mem_task=NULL);
public extern U0 FifoU8Del(CFifoU8 *f);
public extern BoolI64 FifoU8Ins(CFifoU8 *f,U8 b);
public extern BoolI64 FifoU8Rem(CFifoU8 *f,U8 *_b);
public extern BoolI64 FifoU8Peek(CFifoU8 *f,U8 *_b);
public extern U0 FifoU8Flush(CFifoU8 *f);
public extern U64 FifoU8Cnt(CFifoU8 *f);
public extern CFifoU64 *FifoU64New(U64 size,CTask *mem_task=NULL);
public extern U0 FifoU64Del(CFifoU64 *f);
public extern BoolI64 FifoU64Ins(CFifoU64 *f,U64 q);
public extern BoolI64 FifoU64Rem(CFifoU64 *f,U64 *_q);
public extern BoolI64 FifoU64Peek(CFifoU64 *f,U64 *_q);
public extern U0 FifoU64Flush(CFifoU64 *f);
public extern U64 FifoU64Cnt(CFifoU64 *f);

#help_index "Char/Operations"
public _extern _STRCMP I64 StrCmp(U8 *st1,U8 *st2);
public _extern _STRICMP I64 StrICmp(U8 *st1,U8 *st2);
public _extern _STRNCMP I64 StrNCmp(U8 *st1,U8 *st2,U64 n);
public _extern _STRNICMP I64 StrNICmp(U8 *st1,U8 *st2,U64 n);
public _extern _STRSTR U8 *StrStr(U8 *st,U8 *pattern);
public _extern _STRISTR U8 *StrIStr(U8 *st,U8 *pattern);
public _extern _STRCPY U0 StrCpy(U8 *dest,U8 *src);
public _extern _STRCAT U0 StrCat(U8 *dest,U8 *src);

#help_index "Char/Input"
public extern U64 sys_key_cnt;
public extern U0 SetTypeMatic(U8 delay);
extern U0 KbdInit();
extern U64 KbdQueMsgs();
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
extern CFifoU64 *scan_code_fifo;
extern U64 sys_last_down_scan_code;
extern BoolI8 kbdmouse_rst;
extern BoolI8 mouse_irqs_working;

#help_index "Input Pointer"
#help_file "::/SparrowOS/Doc/InputPointer"
public extern I64 ipx,ipy,ipz,ip_throttle;
public extern I64 ipx_offset,ipy_offset,ipz_offset;
public extern F64 ipx_scale,ipy_scale,ipz_scale;
public extern F64 ip_speed;
public extern I64 ip_dev;
public extern I64 iptx,ipty;
public extern BoolI8 ip_lb,ip_cb,ip_rb;
extern BoolI8 ip_lb_last,ip_cb_last,ip_rb_last;
public extern BoolI8 ip_has_wheel;
extern F64 ip_last_move_time;
extern CSnap ip_snap_last;
public extern U0 SetIP(I64 x=MAX_I64,I64 y=MAX_I64,I64 z=MAX_I64,I64 l=MAX_I64,I64 r=MAX_I64,I64 c=MAX_I64);
#help_index "Input Pointer/Displayed Image"
public extern BoolI8 ip_on;  //Show or hide cursor
extern U0 IPVarsUpdate(I64 x,I64 y,I64 z,BoolI8 l,BoolI8 c,BoolI8 r);
extern U0 IPVarsInit();
#help_index "Input Pointer/Snap"
public extern I64 ipx_presnap,ipy_presnap,ipz_presnap;
extern CSnap *SnapCur();
public extern U0 GetSnap(CSnap *s);
public extern U0 SetSnap(CSnap *s,I64 i=0,F64 d=0);

#help_index "Messages"
extern U0 InputFilterTask();
public extern I64 ScanMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,CTask *task=NULL);
public extern U64 FlushMsgs(CTask *task=NULL);
public extern I64 GetMsg(I64 *_p1=NULL,I64 *_p2=NULL,U64 mask=~1,CTask *task=NULL);
public extern U64 ScanChar();

#help_index "Misc;Messages"
public extern U64 sys_cur_scan_code;

#help_index "USB"
extern I64 sys_num_usb;
extern CUSB sys_usb_devs[MAX_USB];
extern CUSBTD *USBAllocTD();
extern U0 USBFreeTD(CUSBTD *temptd);
extern U0 USBEndAll();
extern U0 USBInitAll();
extern U32 *USBAllocFrameList(U64 usb_num,U64 size);

#help_index "Doc/Task"
extern CDoc *(*fp_doc_cur)(CTask *task=NULL);

#help_index "Char/Conversion"
public extern F64 A2F64(U8 *src,U8 **_end_ptr=NULL);

#help_index "Time/CDate"
public extern U8 *date_fmt_map,*time_fmt_map;
public extern U0 DateTimeFmtMapsRst();

#help_index "Char/Conversion;Time/CDate"
public extern CDate A2CDate(U8 *src);

#help_index "Debugging;Hash"
public extern CHash *PSectFind(U0 *add,I64 *offset);
extern CPSectCache *psect_cache;
extern I64 psect_cache_index;

#help_index "Debugging"
public extern U0 EdLite(U8 *filename);
public extern I64 SrcLineNum(U0 *add,U64 cnt=1);
public extern U8 *SrcFileName(U0 *add,U64 cnt=1,CTask *mem_task=NULL);
public extern U8 *SrcEdLink(U0 *add,U64 cnt=1,CTask *mem_task=NULL);
public extern BoolI64 PutSrcLink(U0 *add,U64 cnt=1,U8 *buf=NULL);
public extern U0 EdAddSrc(U0 *add,U64 cnt=1,U64 ed_flags=0);

#help_index "Exceptions"
public extern U0 CatchAll();

#help_index "Task/Requests;Compiler"
public extern U0 UserTaskCont();
public extern I64 ExeCmdLine(CLex *lx);

#help_index "Training"
public extern U0 Help();

#help_index ""
