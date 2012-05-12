extern BoolI8 win_show_menu;
extern BoolI64 WinToTop(TaskStruct *task=NULL,BoolI8 update_z_buf=TRUE);
extern U0 GrPlot0(GrDC *dc,I64 x,I64 y);
extern U0 DrawTaskBorder(TaskStruct *task);
extern U0 DrawCtrls(TaskStruct *task);
extern U0 MgdSliderCtrl(Ltf *l);
extern U0 WinScrollsInit(TaskStruct *task);

extern U64 ltf_tag_types[],ltf_nontag_invisible_types[],
	  ltf_form_types[],ltf_data_types[];
extern U64 ltf_dft_flags[],ltf_dft_type_flags[];

extern I64 PopUp(U8 *msg,TaskStruct *parent=NULL,TaskStruct **pu_task=NULL);
extern LtfEntry *LtfPutS(Ltf *l,U8 *st);
extern BoolI64 PopUpNoYes(U8 *header=NULL,U8 *footer=NULL);
extern I64 PopUpMenu(Ltf *l,U64 do_flags=0,U8 *help=NULL);
extern I64 PopUpRangeI64(I64 lo,I64 hi,I64 step=1,U8 *header=NULL,U8 *footer=NULL);
extern I64 PopUpColor(U8 *header=NULL,BoolI8 allow_transpant=TRUE,BoolI8 allow_dft=TRUE);
extern I64 PopUpLightingColor(U8 *header=NULL);
extern I64 PopUpDitherColor(U8 *header=NULL);
extern U8 *PopUpMGetS(U8 *header=NULL);
extern BoolI64 PopUpTransform(I64 *r);
extern U0 PopUpSnap(SnapStruct *g);
extern Ltf *LtfNew(TaskStruct *task=NULL);
extern Ltf *LtfRead(U8 *name,U64 flags=0);
extern U0 EdCodeTools(Ltf *l);
extern U0 LtfDel(Ltf *l);
extern U0 LtfMax(U64 i=MAX_I32);
extern LtfEntry *LtfPrintF(Ltf *l,U8 *src,...);
extern U8 *EdPicture();
extern BoolI64 View();
extern BoolI64 LtfLock(Ltf *l);
extern BoolI64 LtfUnlock(Ltf *l);
extern U0 LtfInsert(Ltf *l,LtfEntry *ll);
extern U0 LtfDataFmt(LtfEntry *cl);
extern U0 LtfLineNumGoTo(Ltf *l,I64 linenum); //one based
extern U0 LtfDump(Ltf *l,U64 uS_delay=0);
extern LtfEntry *LtfEntryCopy(Ltf *l,LtfEntry *cl);
extern U0 LtfInsertLtf(Ltf *l,Ltf *b);
extern U0 LtfD(U8 *buf,U64 cnt=0x80);
extern U0 LtfRecalc(Ltf *l,BoolI8 has_cursor=FALSE,I64 recalc=LTF_RECALC_ALL);
extern BoolI64 LtfFormDo(U8 *D,U8 *class_name,U64 do_flags=0,
    U8 *help=NULL,U8 *header=NULL,U8 *footer=NULL);
extern Ltf *LtfCur(TaskStruct *task=NULL);
extern U0 LtfClear(Ltf *l=NULL);
extern BoolI64 LtfCursor(BoolI8 show=OFF,Ltf *l=NULL);
extern U0 LtfBottom(Ltf *l=NULL);
extern U0 LtfBinsValidate(Ltf *l);
extern U0 LtfRemQue(Ltf *l);
extern U0 LtfHome(Ltf *l=NULL);
extern U0 LtfCenter(Ltf *l=NULL,I64 recalc=LTF_RECALC_ALL);
extern U0 LtfRst(Ltf *l,BoolI8 is_old);
extern U0 LtfInsQue(Ltf *l,Ltf *pred);
extern U0 LtfUpdateTaskLtfs(TaskStruct *task);
extern I64 PmtI64(U8 *msg,I64 dft,I64 lo=MIN_I64,I64 hi=MAX_I64);
extern F64 PmtF64(U8 *msg,F64 dft,F64 lo=MIN_F64,F64 hi=MAX_F64);
extern U0 ClassRep(U8 *class_name,U8 *d,I64 max_depth=2,BoolI8 fun=FALSE,I64 offset=0);
extern BoolI64 Ed(U8 *filename);
extern U0 Plain(U8 *filename);

extern U0 WsFillIn(I64 n);
extern U0 WsMan(I64 n,TaskStruct *parent=NULL);
extern U0 WsdFillin(I64 n);
extern U0 WsdDef(I64 n,TaskStruct *parent=NULL);
extern BoolI64 WordStat(BoolI64 val=ON);
extern U0 WsdDisplayDefs(Ltf *l,U8 *st,I64 num=-1);
extern U0 WsFindMisspelled(Ltf *l);

extern U0 LtfHelpIndex(Ltf *l,U8 *index);
extern U0 AcctOneTimePopUp(I64 flag_num,U8 *msg);
extern BoolI64 PopUpOk(U8 *header=NULL,U8 *footer=NULL);
extern BoolI64 PopUpCancelOk(U8 *header=NULL,U8 *footer=NULL);
extern I64 PopUpFile(U8 *filename,BoolI8 warn_ext=TRUE,TaskStruct *parent=NULL,TaskStruct **pu_task=NULL);
extern I64 AdamFile(U8 *filename,BoolI8 warn_ext=TRUE);

extern TaskSettings *SettingsPush(TaskStruct *task=NULL,U64 flags=0);
extern U0 SettingsPop(TaskStruct *task=NULL,U64 flags=0);

extern U0 WinScrollNull(TaskStruct *task,P3I64 *s);
extern U0 WinScrollRestore(TaskStruct *task,P3I64 *s);
extern F64 win_desired_refresh,win_actual_refresh;
extern BoolI64 WinBorder(BoolI8 val,TaskStruct *task=NULL);
extern U0 WinSync(BoolI8 full_refresh=FALSE);
extern U0 WinMax(TaskStruct *task=NULL);
extern BoolI64 SetCursorPos(TaskStruct *task,I64 iptx,I64 ipty,BoolI8 set_cursor=TRUE);
extern F64 refresh_ode_time,last_refresh_ode_time;

extern MenuEntry *MenuEntryFind(Menu *m,U8 *full_name);
extern Menu *MenuPush(U8 *st);
extern U0 MenuPop();
extern Menu *MenuFilePush(U8 *filename);

extern U0 Noise(I64 mS,I64 min_freq,I64 max_freq,I64 waveform=WF_NOISE);

extern Ctrl *CtrlFindUnique(TaskStruct *task,I64 type);
extern BoolI64 CtrlInside(Ctrl *c,I64 x,I64 y);
extern Ctrl *ViewAnglesNew(TaskStruct *task=NULL);
extern U0 ViewAnglesDel(TaskStruct *task=NULL);

#define FM_NORMAL	0
#define FM_PICK_FILE	1
#define FM_PICK_DIR	2
extern U64 GrepWiz();

#define CTRL_L_HELP "::/LT/Doc/CtrlL.TXZ"
