//This is the main LoseThos header file
//for data structures and vals.
//There is no code in this file.

//The internal types are U16i,I32i, etc.
//They are defined [C:/LT/Compiler/CmpInit.CPZ,8] internal_types_table.
//LoseThos C+ has a feat where a
//union structure is treated as a
//whole if no member is specified.
//See [::/LT/Demo/SubIntAccess.CPZ] ::/LT/Demo/SubIntAccess.CPZ.
public U16i union U16
{
  I8i i8[2];
  U8i u8[2];
};

public I16i union I16
{
  I8i i8[2];
  U8i u8[2];
};

public U32i union U32
{
  I8i i8[4];
  U8i u8[4];
  I16 i16[2];
  U16 u16[2];
};

public I32i union I32
{
  I8i i8[4];
  U8i u8[4];
  I16 i16[2];
  U16 u16[2];
};

public U64i union U64
{
  I8i i8[8];
  U8i u8[8];
  I16 i16[4];
  U16 u16[4];
  I32 i32[2];
  U32 u32[2];
};

public I64i union I64
{
  I8i i8[8];
  U8i u8[8];
  I16 i16[4];
  U16 u16[4];
  I32 i32[2];
  U32 u32[2];
};

public class P2I32  //two dimensional pt
{
  I32 x,y;
};

public class P3I32  //three dimensional pt
{
  I32 x,y,z;
};

public class P2I64  //two dimensional pt
{
  I64 x,y;
};

public class P3I64  //three dimensional pt
{
  I64 x,y,z;
};

public class P2d  //two dimensional pt
{
  F64 x,y;
};

public class P3d  //three dimensional pt
{
  F64 x,y,z;
};

public class Complex
{
  F64 x,y;
};

public class Que
{
  Que *next,*last;
};

#define NULL	0
#define TRUE	1
#define FALSE	0
#define ON	1
#define OFF	0
#define BoolI8	I8
#define BoolI16	I16
#define BoolI32	I32
#define BoolI64	I64
#define MAX_U8 0xFF
#define MIN_U8 0
#define MAX_I8 0x7F
#define MIN_I8 (-0x80)
#define MAX_U16 0xFFFF
#define MIN_U16 0
#define MAX_I16 0x7FFF
#define MIN_I16 (-0x8000)
#define MAX_U32 0xFFFFFFFF
#define MIN_U32 0
#define MAX_I32 0x7FFFFFFF
#define MIN_I32 (-0x80000000)
#define MAX_U64 0xFFFFFFFFFFFFFFFF

//(Int to F64 conversion is signed)
#define MAX_U64_F64 (0x43F0000000000000(F64))

#define MIN_U64 0
#define MAX_I64 0x7FFFFFFFFFFFFFFF
#define MIN_I64 (-0x8000000000000000)

#define MAX_F64 (0x7FEFFFFFFFFFFFFF(F64))
#define MIN_F64 (0xFFEFFFFFFFFFFFFF(F64))
#define inf	   (0x7FF0000000000000(F64))
#define inf	   (0x7FF0000000000000(F64))
#define pi	   (0x400921FB54442D18(F64))
#define pi	   (0x400921FB54442D18(F64))
#define exp_1	   (0x4005BF0A8B145769(F64)) //The number "e"
#define log2_10    (0x400A934F0979A371(F64))
#define log2_e	   (0x3FF71547652B82FE(F64))
#define log10_2    (0x3FD34413509F79FF(F64))
#define loge_2	   (0x3FE62E42FEFA39EF(F64))
#define sqrt2	   (0x3FF6A09E667F3BCD(F64))

#define eps	   (0x3CB0000000000000(F64))

#define INVALID_PTR	MAX_I64
#define INVALID_CLUSTER -1
#define ALL_MASK	-1

#define OC_OP_SIZE_PREFIX	0x66
#define OC_ADDR_SIZE_PREFIX	0x67
#define OC_LOCK_PREFIX		0xF0
#define OC_NOP			0x90
#define OC_BPT			0xCC
#define OC_CALL			0xE8

#define SF_RBP	0x00
#define SF_RIP	0x08
#define SF_ARG1	0x10
#define SF_ARG2	0x18
#define SF_ARG3	0x20
#define SF_ARG4	0x28
#define SF_ARG5	0x30
#define SF_ARG6	0x38
#define SF_ARG7	0x40
#define SF_ARG8	0x48

extern class TaskStruct;
extern class Ltf;
extern class CPUStruct;
extern class ClassStruct;
extern class FunStruct;
extern class ExeImportExportStruct;
extern class HeapCtrl;
extern class ExeHeapGlblStruct;
extern class GrDC;
extern class MPCmdCtrl;
extern class IntermediateCode;

#define U8_QUE_VECT_CNT		512
class U8QueVect
{
  U8QueVect *next,*last;
  I64 total_cnt,node_cnt,min_index;
  U8 body[U8_QUE_VECT_CNT];
};

public class U8Fifo
{
  U8 *buf;
  U64 mask,in_ptr,out_ptr;
};

public class U64Fifo
{
  U64 *buf;
  U64 mask,in_ptr,out_ptr;
};

#define NUM_DATE_FMTS	57
#define NUM_TIME_FMTS	23

#define LT_YEAR_DAYS		365.24225
#define LT_YEAR_DAYS_INT	36524225
#define LT_BASE_DAY_OF_WEEK	0
public I64 class LTDate
{
  U32 time;
  I32 date;
};

public class D3
{
  F64 x,y,z;
};

public class Order2D3
{
  F64 x,y,z,DxDt,DyDt,DzDt;
};

#define MSF_INACTIVE		1
#define MSF_FIXED		2
public class MassStruct
{
  MassStruct *next,*last;
  Order2D3 *state,*DstateDt;

  U0 start_saved_area;
  U32 flags,num;
  F64 mass,drag_profile_factor;
  U0 saved_state;
  F64 x,y,z,DxDt,DyDt,DzDt;
  I64 user_data;
  U0 end_saved_area;
};

#define SSF_INACTIVE		1
#define SSF_NO_COMPRESSION	2
#define SSF_NO_TENSION		4
public class SpringStruct
{
  SpringStruct *next,*last;
  MassStruct *end1,*end2;
  F64 f,displacement; //set for you to check

  U0 start_saved_area;
  U32 flags,num,
     end1_num,end2_num;
  F64 constant,rest_len;
  I64 user_data;
  U0 end_saved_area;
};

#define ODEF_HAS_MASSES	1
#define ODEF_PAUSED	2
#define ODEF_STARTED	4

#define ODE_SIGNATURE_VAL	'OdeS'
public class Ode
{
  Ode *next,*last;
  U32 ode_signature,pad;
  U64 flags;
  I64 n,n_internal;
  MassStruct   *next_mass,*last_mass;
  SpringStruct *next_spring,*last_spring;
  F64 drag_v;  //drag proportional to velocity
  F64 drag_v2; //drag proportional to velocity squared
  F64 drag_v3; //drag proportional to velocity cubed
  F64 acceleration_limit; //This clips acceleration
  F64 base_t;
  F64 t,t_scale;
  F64 h,h_min,h_max;

  //This is not precise, just a ballpark
  //LoseThos Ode's are for video games
  //not science.  It bails if it takes
  //too long.
  F64 min_tolerance,max_tolerance;

  F64 tolerance_internal;
  F64 *array_base;
  F64 *state;
  F64 *state_internal;
  F64 *DstateDt;
  F64 *state_scale;
  F64 *initial_state;
  F64 *temp0,*temp1,*temp2,*temp3,
	 *temp4,*temp5,*temp6,*temp7;
  TaskStruct *mem_task,*win_task;
  U0 (*derivative)(Ode *o,F64 t,F64 *state,F64 *DstateDt);
  I64 user_data;
};

class BinFileHeaderStruct
{
/* See [::/LT/Compiler/CmpMain.CPZ,LT_XSUM] Bin File Header Generation by compiler.

Bin files have this 16-byte header,
followed by code,
followed by loader patch table.

See [::/LT/Compiler/CmpMain.CPZ,EIE_MODULE_ALIGNMENT] Loader Patch Table Generation and [C:/LT/Adam/BinRep.APZ,22] BinRep().
*/
  U16 jmp;
  U16 type;
  U32 loader_patch_table_offset;
  U32 file_size;
  U32 xsum;
};

//IDT entry types
#define IDTET_TASK		0x05
#define IDTET_IRQ		0x0E
#define IDTET_TRAP		0x0F //Same as IRQ but doesnt do CLI.

//0x00-0x1F are reserved by Intel
#define I_DIVIDE_BY_ZERO	0x00
#define I_SINGLE_STEP		0x01
#define I_NMI			0x02
#define I_BPT			0x03
#define I_PAGE_FAULT		0x0E
//0x20-0x2F are used for hardware
#define I_TIMER			0x20
//Software Interrupts
#define I_MP_CRASH		0x30
#define I_WAKE			0x31
//See [] ST_INT_NAMES

//You might want to start backward from
//0xFF for your own interrupts.
#define I_USER			0x40


#define MP_MAX_PROCESSORS	64

#define SYS_START_CR0		0x0031

#define RFLAGS_SINGLE_STEP	8
#define RFLAGS_INTERRUPTS	9

#define SYS_START_RFLAGS	0x0000
#define SYS_NORMAL_RFLAGS	0x0200

#define NUM_MEM_E820_ENTRIES	48
#define MEM_E820_ENTRY_SIZE	20
class MemE820Entry
{
  U8 *base;
  U64 len;
  U8 type,pad[3];
};

#define GDT_NULL_SEL		0x00
#define GDT_BOOT_DS_SEL 	0x10
#define GDT_BOOT_CS_SEL		0x20
#define GDT_CS_SEL		0x30
#define GDT_CS64_SEL		0x40	//The [::/LT/Doc/Constitution.TXZ] Constitution says just ring0.
#define GDT_CS64_RING3_SEL	0x50 //[::/LT/Demo/Lectures/Ring3.CPZ] Ring3, in case you want to play around.
#define GDT_ZERO_DS_SEL		0x60
#define GDT_ZERO_DS_RING3_SEL	0x70
#define GDT_END1		0x80
#define GDT_CPU_TR_SELS		0x80
#define GDT_CPU_TR_RING3_SELS	(GDT_CPU_TR_SELS      +MP_MAX_PROCESSORS*16)
#define GDT_SIZE		(GDT_CPU_TR_RING3_SELS+MP_MAX_PROCESSORS*16)

class OSMainStruct
{ //Must match [::/LT/OSMain/OSStart.CPZ,SYS_BOOT_BASE] OSStartUp
  BinFileHeaderStruct h; //16 bytes, 1 paragraph
  U32 jmp;
  U32 boot_base;
  U32 boot_patch_table_base;
  LTDate compile_time;
  U32 os_32start;
  U32 boot_code;
  U16 sys_mem_E801[2];
  MemE820Entry sys_mem_E820[NUM_MEM_E820_ENTRIES];
  U8  sys_gdttab[GDT_SIZE];
  U16 sys_gr_mode;
  U16 sys_pci_busses;
};

#define RLf_16BIT		0
#define RLf_32BIT		1
#define RLf_PATCHED		2
#define RLf_64BIT		3
#define RLf_BOOT_HEAP		4
#define RLf_FULL_HEAP		5
#define RLf_RAW			6
#define RLf_INTERRUPTS		7
#define RLf_MP			8
#define RLf_DSK			9
#define RLf_COMPILER		10
#define RLf_LTF			11
#define RLf_WINMGR		12
#define RLf_LOGIN		13
#define RLf_REGISTRY		14
#define RLf_ADAM2		15
#define RLf_ADAM_SERVER		16

#define RLF_16BIT		0x000001
#define RLF_32BIT		0x000002
#define RLF_PATCHED		0x000004
#define RLF_64BIT		0x000008
#define RLF_BOOT_HEAP		0x000010
#define RLF_FULL_HEAP		0x000020
#define RLF_RAW			0x000040
#define RLF_INTERRUPTS		0x000080
#define RLF_MP			0x000100
#define RLF_DSK			0x000200
#define RLF_COMPILER		0x000400
#define RLF_LTF			0x000800
#define RLF_WINMGR		0x001000
#define RLF_LOGIN		0x002000
#define RLF_REGISTRY		0x004000
#define RLF_ADAM2		0x008000
#define RLF_ADAM_SERVER		0x010000

#define LDF_NO_ABSS		1
#define LDF_JUST_LOAD		2
#define LDF_SILENT		4

#define HPET_GCAP_ID	(0xFED00000+0x00)
#define HPET_GEN_CONF	(0xFED00000+0x10)
#define HPET_MAIN_CNT	(0xFED00000+0xF0)

#define LAPIC_BASE		0xFEE00000

#define LAPIC_APIC_ID		(LAPIC_BASE+0x020)
#define LAPIC_APIC_VERSION	(LAPIC_BASE+0x030)
#define LAPIC_TASK_PRIORITY	(LAPIC_BASE+0x080)
#define LAPIC_ARIBITRATION_PRIORITY (LAPIC_BASE+0x090)
#define LAPIC_PROCESSOR_PRIORITY (LAPIC_BASE+0x0A0)
#define LAPIC_EOI		(LAPIC_BASE+0x0B0)
#define LAPIC_LOG_DST		(LAPIC_BASE+0x0D0)
#define LAPIC_DFR		(LAPIC_BASE+0x0E0)
#define LAPIC_LDR		(LAPIC_BASE+0x0D0)

#define LAPICF_APIC_ENABLED		0x100
#define LAPIC_SVR		(LAPIC_BASE+0x0F0)

#define LAPIC_ISR		(LAPIC_BASE+0x100)
#define LAPIC_TMR		(LAPIC_BASE+0x180)
#define LAPIC_IRR		(LAPIC_BASE+0x200)
#define LAPIC_ICR_LOW		(LAPIC_BASE+0x300)
#define LAPIC_ICR_HIGH		(LAPIC_BASE+0x310)

#define LAPIC_LVT_TIMER		(LAPIC_BASE+0x320)
#define LAPIC_LVT_THERMAL	(LAPIC_BASE+0x330)
#define LAPIC_LVT_PERF		(LAPIC_BASE+0x340)
#define LAPIC_LVT_LINT0		(LAPIC_BASE+0x350)
#define LAPIC_LVT_LINT1		(LAPIC_BASE+0x360)
#define LAPIC_LVT_ERR		(LAPIC_BASE+0x370)

#define MPN_VECT		0x7F
#define MP_VECT_ADDR	(MPN_VECT*0x1000)

//I/O APIC Memory mapped window
#define IOAPIC_REG		0xFEC00000 //U8
#define IOAPIC_DATA		0xFEC00010 //U32
//I/O APIC Registers
#define IOAPICID		0x00
#define IOAPICVER		0x01
#define IOAPICARB		0x02
#define IOREDTAB		0x10

//Model specific registers.
#define IA32F_SCE	0x001
#define IA32F_LME	0x100
#define IA32_LAPIC_BASE	0x01B
#define IA32_EFER	0xC0000080
#define IA32_FS_BASE	0xC0000100
#define IA32_GS_BASE	0xC0000101

class MPMainStruct
{
  U32 jmp;
  U64 sys_temp_ptr;
};

class MPCrashStruct
{
  TaskStruct *task;
  I64 cpu_num;
  U8 *msg;
  I64 msg_num;
};


#define JIFFY_FREQ		1000 // Hz
#define LTDATE_FREQ		49710 // Hz
#define SYS_TIMER_FREQ		(18333*65536/1000) //Hz

#define SYS_TIMER0_PERIOD	(65536*182/10/JIFFY_FREQ)

#define DFT_CACHE_LINE_WIDTH	64
#define SEMA_STRUCT_SIZE	DFT_CACHE_LINE_WIDTH

class Sema
{//I read you're supposed to use one cache line.	I don't know why.  Might be silly.
  BoolI64 val;
  U8 pad[SEMA_STRUCT_SIZE-8];
};

#define SYS_SEMA_DEBUG			0
#define SYS_SEMA_DETERMINISTIC		1
#define SYS_SEMA_RECORD_MACRO		2
#define SYS_SEMA_NO_IO_SND		3
#define SYS_SEMA_SYS_TIMER		4
#define SYS_SEMA_SYS_DATE		5
#define SYS_SEMA_IN_DEBUGGER		6
#define SYS_SEMA_SND			7
#define SYS_SEMA_HEAPLOG_ACTIVE		8
#define SYS_SEMA_HEAPLOG_LOCK		9
#define SYS_SEMA_REFRESH_IN_PROGRESS	10
#define SYS_SEMA_FLUSH_VGA_IMAGE	11
#define SYS_SEMA_SINGLE_USER		12
#define SYS_SEMA_DSK_CACHE		13
#define SYS_SEMA_FAR_CALL32		14
#define SYS_SEMA_DEV_MEM		15
#define SYS_SEMA_VGA			16
#define SYS_SEMA_UPDATE_WIN_Z_BUF	17
#define SYS_SEMA_TNP			18
#define SYS_SEMA_MUTE			19
#define NUM_SYS_SEMAS			20

#define SYSf_CTRL_ALT_DEL		0
#define SYSf_CTRL_ALT_C			1
#define SYSf_CTRL_ALT_X 		2
#define SYSf_CTRL_ALT_TAB		3

//Extern calls
//I'm the only one allowed to create these.
//They work with [C:/LT/OSMain/Adam1b.HPZ,294] CallExtNum().
//I guess you could reallocate
//and extend [C:/LT/OSMain/Adam1b.HPZ,293] ext[] and copy it.
#define EXT_WIN_TO_TOP		0
#define EXT_DEBUGGER		1
#define EXT_EXIT_DEBUGGER	2
#define EXT_HEAPLOG_MALLOC	3
#define EXT_HEAPLOG_FREE	4
#define EXT_RESUME_DBG		5

#define EXT_TABLE_SIZE		6

//Hash table types
//[::/LT/OSMain/Define1a.CPZ,ST_HTT_TYPES] ST_HTT_TYPES
#define HTt_EXPORT_SYS_SYM	0
#define HTt_IMPORT_SYS_SYM	1
#define HTt_DEFINE_STR		2
#define HTt_GLBL_VAR		3
#define HTt_TYPEDEF		4
#define HTt_CLASS		5
#define HTt_INTERNAL_TYPE	6
#define HTt_FUN			7
#define HTt_WORD		8
#define HTt_DICT_WORD		9
#define HTt_KEYWORD		10
#define HTt_ASM_KEYWORD		11
#define HTt_OPCODE		12
#define HTt_R8			13
#define HTt_R16 		14
#define HTt_R32 		15
#define HTt_R64 		16
#define HTt_SEG 		17
#define HTt_FSTK		18
#define HTt_MM			19
#define HTt_XMM			20
#define HTt_FILE		21
#define HTt_MODULE		22
#define HTt_HELP_FILE		23
#define HTt_NAMED_FRAME_PTR	24
#define HTt_NUM_TYPES		25

#define HTf_PRIVATE		25
#define HTf_PUBLIC		26
#define HTf_EXPORT		27
#define HTf_IMPORT		28
#define HTf_LIT			29
#define HTf_UNRESOLVED		30
#define HTf_LOCAL		31

#define HTT_INVALID		0
#define HTT_EXPORT_SYS_SYM	0x001 //ExportSysSymHashEntry
#define HTT_IMPORT_SYS_SYM	0x002 //ImportSysSymHashEntry
#define HTT_DEFINE_STR		0x004 //DefineStrHashEntry
#define HTT_GLBL_VAR		0x008 //GlblVarStruct
#define HTT_TYPEDEF		0x010 //GlblVarStruct
#define HTT_CLASS		0x020 //ClassStruct
#define HTT_INTERNAL_TYPE	0x040 //ClassStruct
#define HTT_FUN			0x080 //FunStruct
#define HTT_WORD		0x100 //WsHashEntry -- only in WS table
#define HTT_DICT_WORD		0x200 //GenericHashEntry -- only in WS table
#define HTT_KEYWORD		0x400 //GenericHashEntry
#define HTT_ASM_KEYWORD		0x800 //GenericHashEntry
#define HTT_OPCODE		0x1000 //OpcodeHashEntry
#define HTT_R8			0x2000 //GenericHashEntry
#define HTT_R16 		0x4000 //GenericHashEntry
#define HTT_R32 		0x8000 //GenericHashEntry
#define HTT_R64 		0x10000 //GenericHashEntry
#define HTT_SEG 		0x20000 //GenericHashEntry
#define HTT_FSTK		0x40000 //GenericHashEntry
#define HTT_MM			0x80000 //GenericHashEntry
#define HTT_XMM			0x100000 //GenericHashEntry
#define HTT_FILE		0x200000 //GenericHashEntry
#define HTT_MODULE		0x400000 //GenericHashEntry
#define HTT_HELP_FILE		0x800000 //SrcSymHashEntry
#define HTT_NAMED_FRAME_PTR	0x1000000 //GenericHashEntry
#define HTT_TYPE_MASK		0x1FFFFFF

#define HTF_PRIVATE		0x02000000
#define HTF_PUBLIC		0x04000000
#define HTF_EXPORT		0x08000000
#define HTF_IMPORT		0x10000000
#define HTF_LIT			0x20000000
#define HTF_UNRESOLVED		0x40000000
#define HTF_LOCAL		0x80000000
#define HTF_FLAGS_MASK		0xFE000000

#define HTT_SRC_SYM		(HTT_DEFINE_STR|HTT_GLBL_VAR|HTT_FUN|HTT_TYPEDEF|HTT_CLASS|HTT_EXPORT_SYS_SYM|HTT_HELP_FILE)
#define HTT_ALL 		-1

#define OSMAIN_MODULE_NAME	"/LT/OSMain/OSMain"

class DbgInfo
{
  U32 min_line,max_line;
  U8 *body[1];
};

class IndentInfo
{
  U32 min_line,max_line;
  I8 body[1];
};

public class SysHashEntry
{
//Hash entries are not normally ever removed.  Normally, a new
//entry is added to eclipse the old by being in front of it
//in future searches.  Old function code and stuff is not disposed
//in case a reference uses it and old hash entries are kept, too.
  SysHashEntry *next;
//Once in a while, the whole hash table is traverse -- [C:/LT/OSMain/PSect.CPZ,10] PSectFind(), for example.
//To speed traversal of all symbols, the following links
//allow skipping all the empty slots in a hash table.
  SysHashEntry *next_added,*last_added;
  U8 *str;
  U32 type,use_cnt;
//use_cnt is never decremented.  It is incremented
//each time a node is found in a hash search.
//One place it is used is to tell if a externed function was unused
//before being defined.  It is also used for popularity measures
//in wordstat.	You can do a [C:/LT/Adam/Hash2a.APZ,151] HashRep() to see
//roughly how many times a symbol has been
//referenced.	Beware of instances where
//a hash entry is continuously searched for and
//racks-up huge use_cnt numbers.
};

public class SrcSymHashEntry:SysHashEntry
{
  U8 *src_link;
  U8 *index;
  DbgInfo *debug;
  IndentInfo *indent;
  U8 *import_name;
  ExeImportExportStruct *list;
};

public class GenericHashEntry:SysHashEntry
{
  I64 user_data0,user_data1,user_data2;
};

public class SysHashTable
{
  SysHashTable *next;
  SysHashEntry *next_added,*last_added;
  U64 mask,locked_flags;
  SysHashEntry **body;
};

public class WsHashEntry:SysHashEntry
{
  I32 num;
  U32 hits;
  U16 last_vowel,last_consonant; //Not implemented
};

public class ExportSysSymHashEntry:SrcSymHashEntry
{
  I64 val;
};

public class ImportSysSymHashEntry:SrcSymHashEntry
{
  U8 *module_base;
  U8 *module_header_entry;
};

public class DefineStrHashEntry:SrcSymHashEntry
{
  U8 *data;
  I64 cnt;
  U0 *caller;
};

#define MLF_DFT_AVAILABLE	1
#define MLF_STR_DFT_AVAILABLE	2
#define MLF_FUN			4
#define MLF_DOT_DOT_DOT		8
#define MLF_NO_UNUSED_WARN	16
#define MLF_STATIC		32
#define MLF_BY_ADDR		64
#define MLf_BY_ADDR		6

public class ArrayDimStruct
{
  ArrayDimStruct *next;
  I64 cnt,total_cnt;
};

#define MLMF_IS_STR	1
public class MemberListMeta
{
  MemberListMeta *next;
  U8 *str;
  I64 flags;
  I64 user_data;
};

public class MemberListStruct
{
  MemberListStruct *next,*left,*right;
  U8 *str;
  ClassStruct *member_class;
  MemberListMeta *meta;
  U32 use_cnt;
  U16 flags;
  I8 register,pad;
  I64 offset;
  ArrayDimStruct dim;
  U8 *static_data;
  union {
    I64 static_data_ip;
    I64 dft_val;
  }
  FunStruct *fun_ptr;
};

#define Cf_EXTERN		0
#define Cf_INTERNAL_TYPE	1

public class ClassStruct:SrcSymHashEntry
{ //See [C:/LT/Compiler/StmtParser.CPZ,1] PrsClassNew().	baseclass of [C:/LT/OSMain/Adam1a.HPZ,783] FunStruct
  I64 size; //must be at same offset as [C:/LT/OSMain/Adam1a.HPZ,826] GlblVarStruct.size
  U32 member_cnt;
  U8 ptr_cnt,sub_type,flags[2];
  MemberListStruct *member_list;
  MemberListStruct *last_in_member_list;
  ClassStruct *base_class;
  //fun return or ptr to
  //union-no-member-specified class
  union {
    ClassStruct *return_class;	//for funs
    ClassStruct *fwd_class; //for classes
  }
};

class ExternUsage
{
  ExternUsage *next;
  I64 ip;
};

#define Ff_INTERNAL		8
#define Ff_INTERRUPT		9
#define Ff_DOT_DOT_DOT		10
#define Ff_RET1			11
#define Ff_ARGPOP		12
#define Ff_NOARGPOP		13

public class FunStruct:ClassStruct
{ //See [C:/LT/Compiler/StmtParser.CPZ,45] PrsFunNew().
  U32 arg_cnt,pad;
  U32 used_reg_mask,clobbered_reg_mask;
  U8 *executable_addr;
  ExternUsage *ext_list;
};

//Internal type definitions
#define IT_I0		0
#define IT_U0		1
#define IT_I8		2
#define IT_U8		3
#define IT_I16		4
#define IT_U16		5
#define IT_I32		6
#define IT_U32		7
#define IT_I64		8
#define IT_PTR		8
#define IT_U64		9
#define IT_F64		10
#define IT_UF64		11 //fictitious placeholder
#define IT_NUM_IT	12

#define IT_MASK		0x0F

#define TY_NULL			0x00
#define TY_IMM			0x10
#define TY_REG			0x20
#define TY_DISP			0x30
#define TY_SIB			0x40
#define TY_RIP_DISP32		0x50
#define TY_RIP_DISP32_IMPORT	0x60 //not implemented--requires storage for glblvar struct in intermediate code
#define TY_STK			0x70

#define TY_MASK		0xF0

#define GVAF_FUN	1
#define GVAF_IMPORT	2
#define GVAF_EXTERN	4
#define GVAF_DATA_HEAP	8
#define GVAF_ALIAS	16

public class GlblVarStruct:SrcSymHashEntry
{//also used for typedef's
  I64 size; //must be at same offset as [C:/LT/OSMain/Adam1a.HPZ,754] ClassStruct.size
  I64 flags;
  ClassStruct *var_class;
  FunStruct *fun_ptr;
  ArrayDimStruct dim;
  U8 *data_addr;
  union {
    I64 data_addr_ip;
    ExeHeapGlblStruct *heap_glbl;
  }
};

//Registry sys_msg_flags
#define ARf_FLOODFILL			0
//obsolete
#define ARf_GRSPRITE_INS_CLIPBOARD	2
#define ARf_FILE_MGR_HELP		3
#define ARf_GRSPRITE_PICK_GRAPHICALLY	4
#define ARf_PLANAR_SYMMETRY		5
#define ARf_MUSIC_ORGAN_JUKEBOX		6
#define ARf_MESH_ED			7
//obsolete
#define ARf_GRSPRITE_PTS_RECTANGLES	9
#define ARf_MANAGE_SLIDER		10

#define TASKCf_WAKE_MASTER	0
#define TASKCf_FOCUS_MASTER	1
#define TASKCf_EXIT_ON_COMPLETE	2
#define TASKCf_DONT_FILTER	3
#define TASKCf_HIGHEST_PRIORITY	4

#define TASKCT_TEXT_INPUT	0
#define TASKCT_MSG		1
#define TASKCT_EXECUTE_STR	2
#define TASKCT_EXECUTE_FILE	3

class TaskCmdStruct
{
  TaskCmdStruct *next,*last;
  U64 cmd_code;
  I64 msg_code;
  I64 p1,p2;
  U64 flags;
  U8 *data;
  I64 result;
  TaskStruct *master_task;
};


#define MPCf_DONE		0
#define MPCf_DISPATCHED		1
#define MPCf_FREE_ON_COMPLETE	2

#define MPCT_CALL		1
#define MPCT_SPAWN_TASK		2

class MPCmdStruct
{
  MPCmdStruct *next,*last;
  MPCmdCtrl *ctrl;
  U64 cmd_code;
  U64 flags;
  I64 (*add)(U0 *data);
  U8 *data;
  U8 *desc;
  U64 target_cpu_mask;
  TaskStruct *task; //spawned task
  I64 handler_cpu; //CPU accepting job
  I64 result;
};

#define MPCCf_LOCKED		0
class MPCmdCtrl
{
  MPCmdStruct *next_waiting,*last_waiting;
  MPCmdStruct *next_done,*last_done;
  U64 flags;
};

class SysBpt
{
  SysBpt *next;
  U8 *addr;
  U8 val,pad[7];
};

//See [::/LT/Adam/Gr/GrScreen.APZ,ExtScreenCodes] ExtScreenCodes
#define ATTRF_BLINK	0x10000000
#define ATTRF_INVERT	0x20000000
#define ATTRF_SELECTED	0x40000000
#define ATTRF_UNDERLINE	0x80000000

#define ATTRf_BLINK		28
#define ATTRf_INVERT		29
#define ATTRf_SELECTED		30
#define ATTRf_UNDERLINE		31

//LtfEntry.type codes (Low 8 bits)
#define LTFT_TEXT		0
#define LTFT_NEW_LINE		1
#define LTFT_SOFT_NEW_LINE	2
#define LTFT_CURSOR		3
#define LTFT_TAB		4
#define LTFT_CLEAR		5
#define LTFT_PAGE_BREAK		6
#define LTFT_DATA		7
#define LTFT_PAGE_LEN		8
#define LTFT_LEFT_MARGIN	9
#define LTFT_RIGHT_MARGIN	10
#define LTFT_HEADER		11
#define LTFT_FOOTER		12
#define LTFT_INDENT		13
#define LTFT_FOREGROUND		14
#define LTFT_BACKGROUND		15
#define LTFT_DFT_FOREGROUND	16
#define LTFT_DFT_BACKGROUND	17
#define LTFT_ALT_FOREGROUND	18
#define LTFT_LINK_FOREGROUND	19
#define LTFT_MACRO_FOREGROUND	20
#define LTFT_ANCHOR_FOREGROUND	21
#define LTFT_HIDDEN_FOREGROUND	22
#define LTFT_TREE_FOREGROUND	23
#define LTFT_USER_FOREGROUND	24
#define LTFT_BINARY_FOREGROUND	25
#define LTFT_KEYWORD_FOREGROUND	26
#define LTFT_KEYWORD2_FOREGROUND 27
#define LTFT_COMMENT_FOREGROUND	28
#define LTFT_QUOTE_FOREGROUND	29
#define LTFT_WORD_WRAP		30
#define LTFT_UNDERLINE		31
#define LTFT_INVERT		32
#define LTFT_BLINK		33
#define LTFT_SHIFTED_X		34
#define LTFT_SHIFTED_Y		35
#define LTFT_CURSOR_MOVEMENT	36
#define LTFT_ANCHOR		37
#define LTFT_LINK		38
#define LTFT_BUTTON		39
#define LTFT_CHECK_BOX		40
#define LTFT_MACRO		41
#define LTFT_MENU_VAL		42
#define LTFT_HEX_ED		43
#define LTFT_HIDE_START		44
#define LTFT_HIDE_END		45
#define LTFT_TREE		46
#define LTFT_PICTURE		47
#define LTFT_INS_BINARY	48
#define LTFT_INS_BINARY_TYPE	49
#define LTFT_INS_BINARY_SIZE	50
#define LTFT_BPT		51
#define LTFT_SONG		52
#define LTFT_HIGHLIGHT		53
#define LTFT_LOCK_TO_PAGE	54
#define LTFT_LOCK_TO_CENTER	55
#define LTFT_HTML_IMG		56
#define LTFT_HTML_VIDEO		57
#define LTFT_HTML_CODE		58
#define LTFT_ERROR		59

#define LTFT_NUM_TYPES		60

//LtfEntry.type flags upper bits
#define LTFLT_BLINK		ATTRF_BLINK
#define LTFLT_INVERT		ATTRF_INVERT
#define LTFLT_SELECTED		ATTRF_SELECTED
#define LTFLT_UNDERLINE		ATTRF_UNDERLINE
#define LTF_BL_IV_UL		0xB0000000
#define LTFLt_BLINK		ATTRf_BLINK
#define LTFLt_INVERT		ATTRf_INVERT
#define LTFLt_SELECTED		ATTRf_SELECTED
#define LTFLt_UNDERLINE		ATTRf_UNDERLINE

#define LTF_DBL_BUF_FLAGS	(LTFF_OVERSTRIKE|LTFF_PLAIN_TEXT|LTFF_PLAIN_TEXT_WITH_TABS| \
				LTFF_MIN_SIZE|LTFF_NO_CURSOR|LTFF_FORM|LTFF_ATTR_BY_PRT| \
				LTFF_ATTR_BY_FILENAME|LTFF_DBL_DOLLARS|LTFF_DONT_SWAP_OUT| \
				LTFF_DO_FULL_REFRESH|LTFF_AUX_LTF|LTFF_HIDE_CURSOR|LTFF_CARRIAGE_RETURN)



//LtfEntry.flag flags
#define LTFLF_AUX_STR		1
#define LTFLF_LINK		2
#define LTFLF_LEFT_CB		4
#define LTFLF_LEFT_EXP		8
#define LTFLF_LEFT_MACRO	0x10
#define LTFLF_LEFT_AUTO		0x20
#define LTFLF_RIGHT_CB		0x40
#define LTFLF_RIGHT_EXP		0x80
#define LTFLF_RIGHT_MACRO	0x100
#define LTFLF_RIGHT_AUTO	0x200
#define LTFLF_TAG_CB		0x400
#define LTFLF_HAS_BIN		0x800
#define LTFLF_ESC		0x1000 //Send <ESC> (Exit and Save)
#define LTFLF_QUIT		0x2000 //Send <SHIFT-ESC> (Abort)
#define LTFLF_LEFT_X		0x4000
#define LTFLF_CENTER_X		0x8000
#define LTFLF_RIGHT_X		0x10000
#define LTFLF_TOP_Y		0x20000
#define LTFLF_CENTER_Y		0x40000
#define LTFLF_BOTTOM_Y		0x80000
#define LTFLF_SHIFTED_X		0x100000
#define LTFLF_SHIFTED_Y		0x200000
#define LTFLF_SCROLLING_X	0x400000
#define LTFLF_SCROLLING_Y	0x800000

#define LTFLF_HAS_BORDER	0x100000000
#define LTFLF_SOLID_BORDER	0x200000000
#define LTFLF_BORDER_PLOT	0x400000000
#define LTFLF_CHECKED_COLLAPSED	0x800000000 //checked or collapsed
#define LTFLF_CHECK_COLLAPSABLE	0x1000000000
#define LTFLF_REFRESH_DATA	0x2000000000
#define LTFLF_DATA_IS_PTR	0x4000000000
#define LTFLF_HAS_TERMINATOR	0x8000000000
#define LTFLF_ZERO_BASED	0x10000000000
#define LTFLF_HOLD		0x20000000000
#define LTFLF_TREE		0x40000000000
#define LTFLF_SKIP		0x80000000000
#define LTFLF_ALIAS		0x100000000000
#define LTFLF_POPUP		0x200000000000
#define LTFLF_PAGE_REL_Y	0x400000000000
#define LTFLF_MARGIN_REL_X	0x800000000000
#define LTFLF_FROM_START	0x1000000000000
#define LTFLF_SKIP_IN_FORM	0x2000000000000
#define LTFLF_WIDTH		0x4000000000000
#define LTFLF_HEIGHT		0x8000000000000
#define LTFLF_HARD_SKIP		0x10000000000000
#define LTFLF_FILTER_SKIP	0x20000000000000
#define LTFLF_NO_CLICK_ON	0x40000000000000
#define LTFLF_DONT_DRAW		0x80000000000000 //only works on pictures
#define LTFLF_UPDATE_DATA	0x100000000000000
#define LTFLF_U8		0x200000000000000
#define LTFLF_U16		0x400000000000000
#define LTFLF_U32		0x800000000000000
#define LTFLF_U64		0x1000000000000000
#define LTFLF_DEFINE		0x2000000000000000
#define LTFLF_WIN_REL		0x4000000000000000
#define LTFLF_HTML_LINK		0x8000000000000000

//These are identical to Ltf flags
#define LTFLF_HIGHLIGHT 	0x1000000
#define LTFLF_LOCK_TO_PAGE	0x2000000
#define LTFLF_LOCK_TO_CENTER	0x4000000
#define LTFLF_WORD_WRAP		0x8000000
#define LTFLF_BLINK		ATTRF_BLINK
#define LTFLF_INVERT		ATTRF_INVERT
#define LTFLF_SELECTED		ATTRF_SELECTED
#define LTFLF_UNDERLINE		ATTRF_UNDERLINE
#define LTFLf_HIGHLIGHT		24
#define LTFLf_LOCK_TO_PAGE	25
#define LTFLf_LOCK_TO_CENTER	26
#define LTFLf_WORD_WRAP		27
#define LTFLf_BLINK		ATTRf_BLINK
#define LTFLf_INVERT		ATTRf_INVERT
#define LTFLf_SELECTED		ATTRf_SELECTED
#define LTFLf_UNDERLINE		ATTRf_UNDERLINE

#define LTFLf_AUX_STR		0
#define LTFLf_LINK		1
#define LTFLf_LEFT_CB		2
#define LTFLf_LEFT_EXP		3
#define LTFLf_LEFT_MACRO	4
#define LTFLf_LEFT_AUTO		5
#define LTFLf_RIGHT_CB		6
#define LTFLf_RIGHT_EXP		7
#define LTFLf_RIGHT_MACRO	8
#define LTFLf_RIGHT_AUTO	9
#define LTFLf_TAG_CB		10
#define LTFLf_HAS_BIN		11
#define LTFLf_ESC		12 //Send <ESC> (Exit and Save)
#define LTFLf_QUIT		13 //Send <SHIFT-ESC> (Abort)
#define LTFLf_LEFT_X		14
#define LTFLf_CENTER_X		15
#define LTFLf_RIGHT_X		16
#define LTFLf_TOP_Y		17
#define LTFLf_CENTER_Y		18
#define LTFLf_BOTTOM_Y		19
#define LTFLf_SHIFTED_X		20
#define LTFLf_SHIFTED_Y		21
#define LTFLf_SCROLLING_X	22
#define LTFLf_SCROLLING_Y	23

#define LTFLf_HAS_BORDER	32
#define LTFLf_SOLID_BORDER	33
#define LTFLf_BORDER_PLOT	34
#define LTFLf_CHECKED_COLLAPSED	35
#define LTFLf_CHECK_COLLAPSABLE	36 //checked or collapsed
#define LTFLf_REFRESH_DATA	37
#define LTFLf_DATA_IS_PTR	38
#define LTFLf_HAS_TERMINATOR	39
#define LTFLf_ZERO_BASED	40
#define LTFLf_HOLD		41
#define LTFLf_TREE		42
#define LTFLf_SKIP		43
#define LTFLf_ALIAS		44
#define LTFLf_POPUP		45
#define LTFLf_PAGE_REL_Y	46
#define LTFLf_MARGIN_REL_X	47
#define LTFLf_FROM_START	48
#define LTFLf_SKIP_IN_FORM	49
#define LTFLf_WIDTH		50
#define LTFLf_HEIGHT		51
#define LTFLf_HARD_SKIP		52
#define LTFLf_FILTER_SKIP	53
#define LTFLf_NO_CLICK_ON	54
#define LTFLf_DONT_DRAW		55 //only works on pictures
#define LTFLf_UPDATE_DATA	56
#define LTFLf_U8		57
#define LTFLf_U16		58
#define LTFLf_U32		59
#define LTFLf_U64		60
#define LTFLf_DEFINE		61
#define LTFLf_WIN_REL		62
#define LTFLf_HTML_LINK		63

#define LTFLf_NUM_FLAGS		64

#define LTFBT_GENERIC_DATA	1
#define LTFBT_GRSPRITE		2
#define LTFBT_DEBUG_LINES	3
#define LTFBT_INDENTS		4

public class LtfBinEntry
{
  LtfBinEntry *next,*last;
  I32 temp_use_cnt,renum_num;
  U0 start;
  U32 type,num,flags,size,use_cnt;
  U8 *data;
};

#define LTF_SCROLL_SPEED	8

#define LTFSS_NORMAL		0
#define LTFSS_SINGLE_QUOTE	1
#define LTFSS_DBL_QUOTE		2
#define LTFSS_COMMENT		3
#define LTFSS_CPP_COMMENT	4

#define LTFSSF_BPT		8

public class LtfSettingsStruct
{
  LtfSettingsStruct *next,*last;
  U0 start;
  I16 left_margin,right_margin,indent;
  U16 page_len,header,footer;
  I8 shifted_x,shifted_y,pad[2];
  U8 state,comment_depth,paren_depth,brace_depth;
  U8 cur_text_attr,dft_text_attr,alt_text_attr,link_attr,
     macro_attr,anchor_attr,hidden_attr,tree_attr,
     user_attr,bin_attr,keyword_attr,keyword2_attr,
     comment_attr,quote_attr,pad[2];
};

#define LTF_DFT 		MIN_I32

public class LtfEntryBase
{
//This is a shortened structure for
//cmds like the text cmd which
//don't require the full LtfEntry structure.
  LtfEntryBase *next,*last;
  union {
    U8 type_u8; //this stores the code
    U32 type; //these store attr flags
  };
  I32 page_line_num;
  U64 flags;
  I32 x,y;
  U32 min_col,max_col;

  //These are here to allow recalculating the Ltf
  //from a pt other than the beginning.  Without
  //these, you do not know the current state.
  //Margins will change to allow multiple columns
  //of text on the same page.  (cursor move back up the page.)

  LtfSettingsStruct *settings;

  I64 user_data;
};

public class LtfEntry:LtfEntryBase
{
  union {
    U8 *tag;
    I64 hex_ed_width;
  };
 
  union {
    I64 attr;
    I64 cursor_x_offset;
    I64 (*left_cb)(Ltf *l,LtfEntry *ll);
    I64 left_exp;
  };
  U8 *left_macro;

  union {
    I64 cursor_y_offset;
    I64 (*right_cb)(Ltf *l,LtfEntry *ll);
    I64 right_exp;
  };
  U8 *right_macro;

  U8 *(*tag_cb)(Ltf *l,LtfEntry *ll,
		   TaskStruct *mem_task);
  U8 *define_str;
  U8 *aux_str;
  U8 *html_link;
  U8 *my_fmt_data;
  union {
    I32 scroll_len;
    I32 width;
  }
  union {
    I32 len;
    I32 height;
  }
  I32 bin_num;
  I32 pad;
  LtfBinEntry *bin_data;

  U8 *data;
};

//[C:/LT/Adam/Ltf/LtfPopUp.APZ,312] LtfFormDo() [C:/LT/Adam/Ltf/LtfPopUp.APZ,365] LtfMenuDo() [C:/LT/Adam/Ltf/LtfPopUp.APZ,185] LtfDo() [C:/LT/Adam/Ltf/LtfPopUp.APZ,452] PopUpMenu()
#define DOF_HELP_IS_FILE	1
#define	DOF_MIN_SIZE		2
#define	DOF_INTERCEPT_TASK_END	4
#define DOF_DONT_HOME		8

class EdFindTextStruct
{
  U8	find_text[136]		fmtstr "$DA-P,135,\"Find        :%s\"$\n";
  U8	replace_text[136]	fmtstr "$DA-P,135,\"Replace     :%s\"$\n";
  BoolI8 replace		fmtstr "$CB,\"Replace\"$\n";
  BoolI8 scan_fwd		fmtstr "$CB,\"Fwd\"$\n";
  BoolI8 scan_selected_text	fmtstr "$CB,\"Selection\"$\n";
  BoolI8 match_case		fmtstr "$CB,\"Match Case\"$\n";
  BoolI8 whole_labels		fmtstr "$CB,\"Whole Labels\"$\n";
  BoolI8 local_var		fmtstr "$CB,\"Rename Local Var\"$\n";
  BoolI8 pmt;
  U8	 pad[1];
  I64	 filter_lines		fmtstr "$DA,131,  \"Filter Lines:%d\"$\n";
};

class EdFileNameStruct
{
  U8 name[256] fmtstr "$DA-P,255,\"File Name:%s\"$";
};


// LTF header flags
#define LTFF_OVERSTRIKE 	0x1
#define LTFF_PLAIN_TEXT 	0x2
#define LTFF_PLAIN_TEXT_WITH_TABS 0x4 //has CH_TAB, not LTFT_TAB
#define LTFF_MIN_SIZE		0x8
#define LTFF_NO_CURSOR		0x10 // Makes LtfInsStr remove
#define LTFF_IN_DOLLAR		0x20
#define LTFF_MORE		0x40
#define LTFF_FORM		0x80
#define LTFF_ATTR_BY_PRT	0x100
#define LTFF_ATTR_BY_FILENAME	0x200
#define LTFF_DBL_DOLLARS	0x400
#define LTFF_NULL_GRAB_SCROLL	0x800
#define LTFF_BACKWARD_MOVEMENT	0x1000
#define LTFF_HAS_SONG		0x2000
#define LTFF_UNDO_DIRTY		0x4000
#define LTFF_ALLOW_UNDO		0x8000
#define LTFF_SUPERSCRIPT_MODE	0x10000
#define LTFF_SUBSCRIPT_MODE	0x20000
#define LTFF_DONT_SWAP_OUT	0x40000
#define LTFF_DONT_SHOW		0x80000
#define LTFF_DO_FULL_REFRESH	0x100000
#define LTFF_BREAK_UNLOCKED	0x200000
#define LTFF_AUX_LTF		0x400000
#define LTFF_HIDE_CURSOR	0x800000 //use [C:/LT/Adam/Ltf/LtfRecalc.APZ,1712] LtfCursor

#define LTFF_HIGHLIGHT		LTFLF_HIGHLIGHT
#define LTFF_LOCK_TO_PAGE	LTFLF_LOCK_TO_PAGE
#define LTFF_LOCK_TO_CENTER	LTFLF_LOCK_TO_CENTER
#define LTFF_WORD_WRAP		LTFLF_WORD_WRAP
#define LTFF_BLINK		LTFLF_BLINK
#define LTFF_INVERT		LTFLF_INVERT
#define LTFF_SELECTED		LTFLF_SELECTED
#define LTFF_UNDERLINE		LTFLF_UNDERLINE

#define LTFF_CARRIAGE_RETURN	0x100000000
#define LTFF_NO_SCROLL_BARS	0x200000000 //use [C:/LT/Adam/Ltf/LtfRecalc.APZ,1722] LtfScroll

#define LTFf_OVERSTRIKE 	0
#define LTFf_PLAIN_TEXT 	1
#define LTFf_PLAIN_TEXT_WITH_TABS 2 //has CH_TAB, not LTFT_TAB
#define LTFf_MIN_SIZE		3
#define LTFf_NO_CURSOR		4 // Makes LtfInsStr remove
#define LTFf_IN_DOLLAR		5
#define LTFf_MORE		6
#define LTFf_FORM		7
#define LTFf_ATTR_BY_PRT	8
#define LTFf_ATTR_BY_FILENAME	9
#define LTFf_DBL_DOLLARS	10
#define LTFf_NULL_GRAB_SCROLL	11
#define LTFf_BACKWARD_MOVEMENT	12
#define LTFf_HAS_SONG		13
#define LTFf_UNDO_DIRTY		14
#define LTFf_ALLOW_UNDO		15
#define LTFf_SUPERSCRIPT_MODE	16
#define LTFf_SUBSCRIPT_MODE	17
#define LTFf_DONT_SWAP_OUT	18
#define LTFf_DONT_SHOW		19
#define LTFf_DO_FULL_REFRESH	20
#define LTFf_BREAK_UNLOCKED	21
#define LTFf_AUX_LTF		22
#define LTFf_HIDE_CURSOR	23  //use [C:/LT/Adam/Ltf/LtfRecalc.APZ,1712] LtfCursor

#define LTFf_HIGHLIGHT		LTFLf_HIGHLIGHT
#define LTFf_LOCK_TO_PAGE	LTFLf_LOCK_TO_PAGE
#define LTFf_LOCK_TO_CENTER	LTFLf_LOCK_TO_CENTER
#define LTFf_WORD_WRAP		LTFLf_WORD_WRAP
#define LTFf_BLINK		LTFLf_BLINK
#define LTFf_INVERT		LTFLf_INVERT
#define LTFf_SELECTED		LTFLf_SELECTED
#define LTFf_UNDERLINE		LTFLf_UNDERLINE

#define LTFf_CARRIAGE_RETURN	32
#define LTFf_NO_SCROLL_BARS	33

//locked flags
#define LTFlf_LOCKED		0

class LtfUndo
{
  LtfUndo *next,*last;
  U64 size;
  U64 ltf_flags;
  U64 time_stamp;
  U8 *body;
};

//See [C:/LT/Adam/Ltf/LtfPopUp.APZ,365] LtfMenuDo()
#define LTFM_CANCEL		(-1)

#define LTF_SIGNATURE_VAL		'LtfS'
#define LTF_IN_QUE_SIGNATURE_VAL	'LtfQ'

#define LTF_RECALC_ALL		0
#define LTF_RECALC_PARTIAL	1
#define LTF_RECALC_FIND_CURSOR	2
#define LTF_RECALC_TO_SCREEN	3

public class Ltf //Linked Text File header
{  //See [Ltf] Ltf for documentation.
  LtfEntryBase root;
  U8 *dummy_display;
  U64 flags,locked_flags;
  LtfEntry *cur_entry,*old_cur_entry;
  I32 cur_data_col,old_cur_data_col,
     line_start_col,cur_top_line_num;
  U64 swap_out_time;
  I32 ext_ascii_val,ext_ascii_cnt;
  I32 dollar_buf_size,dollar_buf_ptr;
  U8 *dollar_buf;
  TaskStruct *win_task,*mem_task;
  LtfEntry *recalc_start;
  U32 start_text_attr;
  I32 page_line_num;
  I32 x,y,min_x,max_x,min_y,max_y;
  I64 undo_cnt;
  I64 line,col,best_d;
  I64 old_win_top,old_win_bottom,
     old_win_left,old_win_right;

  I64 cmd_U8;
  U32 ltf_signature,ltf_in_que_signature;

  I32 max_entries,ww_lines_back,cur_bin_num,pad;
  I64 refresh_cnt;
  TaskStruct *owning_task;
  EdFindTextStruct *find_replace;
  Ltf *menu_ltf;
  I32 start_x,start_y;

  U8 *cur_sprite;

//not document num, but num in a GrSprite collection
  I64 cur_sprite_elem_num;

  EdFileNameStruct filename;
  U64 file_attr;
  I64 (*left_click_link)(Ltf *l,LtfEntry *cl);
  I64 (*right_click_link)(Ltf *l,LtfEntry *cl);

  U0 *user_put_data;
  BoolI64 (*user_put_key)(Ltf *l,U0 *data,U64 ch,U64 sc);
  BoolI64 (*user_put_s)(Ltf *l,U0 *data,U8 *st);

  Ltf *next_ltf,*last_ltf;
  Ltf *parent_ltf;
  U8 desc[8]; //Be careful -- only 7 characters and terminator

  LtfBinEntry  bin_root;
  LtfSettingsStruct settings_root;
  LtfUndo undo_root;

  I64 user_data;
};

//MGetS flags
#define GETSF_SHIFT_ESC_EXIT	1 //This kills task on <SHIFT-ESC>
#define GETSF_WITH_NEW_LINE	2

//Compiler Options
//  Use [C:/LT/Compiler/Directives.CPZ,6] OptOn() [C:/LT/Compiler/Directives.CPZ,11] OptOff()

//You might need to do #exe {OptOn();}
//Note: The #exe statement is lexed-ahead, so it takes effect earlier than you might expect.

#define OPTf_WARN_UNUSED_VAR	0 //Applied to funs, not statements
#define OPTf_WARN_PAREN		1 //warn unnecessary parens
#define OPTf_PARSE_TRACE	2
#define OPTf_COMPILE_TRACE	3
#define OPTf_ECHO		4
#define OPTf_OPT_TRACE		5
#define OPTf_LEX_TRACE		6
#define OPTf_EXTERNS_TO_IMPORTS	7
#define OPTf_KEEP_PRIVATE	8
#define OPTf_NO_REG_VAR		9  //Applied to funs, not statements
#define OPTf_REPLACE_SYMS	10
#define OPTf_GLBLS_ON_DATA_HEAP	11
//Disable 10-byte float consts for pi,log2_10,log10_2,loge_2
#define OPTf_NO_BUILTIN_CONST	12 //Applied to funs, not statements
#define OPTf_USE_LIT64		13 //Not completely implemented

#define OPTF_ECHO			(1<<OPTf_ECHO)
#define OPTF_PARSE_TRACE		(1<<OPTf_PARSE_TRACE)

#define LFSF_IS_INCLUDED	1
#define LFSF_IS_LTF		2
#define LFSF_IS_DEFINE		4

#define LFSf_IS_INCLUDED	0
#define LFSf_IS_LTF		1
#define LFSf_IS_DEFINE		2

class LexFileStruct
{
  U8 *buf;
  U8 *buf_ptr;
  I64 line_num,flags;
  U8 *name;
  U8 *line_start;
  Ltf *l;
  LtfEntry *cur_entry;
  U8 last_U16,pad[7];
};

class LexUndefEntry
{
  LexUndefEntry *next;
  ExportSysSymHashEntry *hash;
};

#define ICF_R_TO_F64			0x0000000001
#define ICF_R_TO_INT			0x0000000002
#define ICF_P1_TO_F64			0x0000000004
#define ICF_P1_TO_INT			0x0000000008
#define ICF_P2_TO_F64			0x0000000010
#define ICF_P2_TO_INT			0x0000000020
#define ICF_P3_TO_F64			0x0000000040
#define ICF_P3_TO_INT			0x0000000080
#define ICF_USE_F64			0x0000000100
#define ICF_USE_UNSIGNED		0x0000000200
#define ICF_USE_INT			0x0000000400 //highest priority
#define ICF_NO_DEPEND_RESULT		0x0000000800
#define ICF_CODE_FINAL			0x0000001000
#define ICF_NOT_ADDR			0x0000002000
#define ICF_SHORT_JMP			0x0000004000
#define ICF_P1_FIRST			0x0000008000
#define ICF_PUSH_RESULT			0x0000010000
#define ICF_TRACE			0x0000020000
#define ICF_R_WAS_STK			0x0000040000
#define ICF_P1_WAS_STK			0x0000080000
#define ICF_P2_WAS_STK			0x0000100000
#define ICF_P3_WAS_STK			0x0000200000
#define ICF_PUSH_CMP			0x0000400000 //for 50<i<j<=100 expressions
#define ICF_POP_CMP			0x0000800000 //for 50<i<j<=100 expressions
#define ICF_DEPEND_RESULT		0x0001000000
#define ICF_SWAP			0x0002000000
#define ICf_DONT_PUSH_FLOAT0		26 // 4 bits
#define ICf_DONT_POP_FLOAT0		30 // 4 bits
#define ICF_ALTERNATE_TEMPLATE		0x0400000000
#define ICF_LOCK			0x0800000000
#define ICf_LOCK			35
#define ICF_NO_RIP			0x1000000000
#define ICF_DEL_PREVIOUS_INS		0x2000000000
#define ICF_PREVIOUS_DELETED		0x4000000000
#define ICF_DONT_RESTORE		0x8000000000
#define ICF_NO_CVT_MASK			0xFFFFFFFF00

#define IC_BODY_SIZE			48

#define ECF_HAS_PUSH_CMP		0x01 //for 50<i<j<=100 expressions


class IcArg
{
  U8 type,ptype;
  U16 reg; //low is reg, high is index_reg+scale<<6
  I64 disp;
};

class IcTreeLinks
{
  ClassStruct *p1c,*p2c,*p3c;
  IntermediateCode *p1t,*p2t,*p3t;
};

class IntermediateCode
{
  U16 ic_opcode;
  U16 ic_precedence;
  I16 ic_cnt;
  I16 ic_last_cnt;

  U64 ic_flags;
  I64 ic_data;
 

  ClassStruct *ic_class;
  ClassStruct *ic_class2;

  U32 ic_line;
  I8 ic_indent_level,pad[3];
  IcArg p1,p2,p3,r;

  U8 *ic_ext_body;
  union {
    U8 ic_body[IC_BODY_SIZE];
    IcTreeLinks t;
  };
};

class PrsStk
{
  U64 ptr;
  I64 stk[255];
  U64 ptr2;
  I64 stk2[255];
};

#define CB_IC_CNT 511
class CodeBlk
{
  CodeBlk *next;
  IntermediateCode data[CB_IC_CNT];
};

#define CBMT_LABEL		0
#define CBMT_GOTO_LABEL		1
#define CBMT_STR_CONST		2
#define CBMT_JMP_TABLE		3
#define CBMT_FLOAT_CONSTS	4
#define CBMT_ARRAY_DIM		5
#define CBMT_HASH_ENTRY		6

#define CBMF_USED		1
#define CBMF_POP_CMP		2
#define CBMF_DEFINED		4

#define CBM_MAX_CONSTS		16

class CbMiscStruct
{
  CbMiscStruct *next,*fwd,*dft;
  U8 *str;
  U32 type,flags;
  U8 *addr;
  union {
    U64 st_len;
    U64 num_consts;
    U64 range;
  }
  union {
    U8 **jmp_table;
    F64 *float_consts;
    ArrayDimStruct *dim;
    SysHashEntry *h;
  };
};

#define IEF_OP_SIZE16		0x001
#define IEF_OP_SIZE32		0x002
#define IEF_DONT_SWITCH_MODES	0x004
#define IEF_PLUS_OPCODE		0x008
#define IEF_DFT			0x010
#define IEF_NO_REX		0x020
#define IEF_40_REX		0x040
#define IEF_48_REX		0x080
#define IEF_REX_ONLY_R8_R15	0x100
#define IEF_REX_XOR_LIKE	0x200
#define IEF_STI			0x400
#define IEF_ENDING_ZERO		0x800

#define SV_R_REGISTER	8
#define SV_I_REGISTER	9
#define SV_NONE		10
class InstructionEntry
{
  U8 ins_entry_num; //This entry num in opcode hash entry
  U8 opcode_cnt;
  U8 opcode[4];
  U16 flags;
  U8 slash_val,opcode_modifier;
  U8 arg1,arg2;
  U32 pad;
};

class OpcodeHashEntry:SysHashEntry
{
  U8 instruction_entry_cnt;
  U8 pad[7];
  InstructionEntry ins[1];
};

#define ASM_CODE_BLK_BITS 16
#define ASM_CODE_BLK_SIZE (1<<ASM_CODE_BLK_BITS)

class AsmCodeBlk
{
  AsmCodeBlk *next;
  U8 body[ASM_CODE_BLK_SIZE];
};

U64 class AbsCntsU64
{
  U16 abs_addres;
  U16 c_addres;
  U16 externs,pad;
};

class AsmNum
{
  I64 i;
  U8 *exp;
  LexUndefEntry *undef_local,*undef_glbl;
  AbsCntsU64 abs_cnts;
};

class AsmNum2
{
  AsmNum num;
  I64 U8_cnt,rel,lit;
};

class AsmIns
{
  InstructionEntry *tempi;
  AsmNum2 imm,disp;
  U64 U8_cnt,last_opcode_U8;
  U64 REX,ModrM,SIB;
  BoolI8 has_REX,has_ModrM,has_SIB;
  BoolI8 has_addr_prefix;
  BoolI8 has_operand_prefix;
  BoolI8 is_dft,pad[2];
};

class AsmArgStruct
{
  AsmNum num;
  I64 seg,size,reg1,reg2,fstk;
  I64 scale;
  BoolI8 indirect,imm_or_off_present,just_seg,pad[5];
};

class AsmUnresolvedRef
{
  AsmUnresolvedRef *next;
  I64 type,line_num;
  U8 *exp;
  U64 ip,rel_ip;
  U8 *str;  //Only for import glbls
  LexUndefEntry *undef_hash;
  BoolI8 U8_avail;
  BoolI8 lit;  //Only for import glbls
};

#define EIE_REL_EXPORT		0
#define EIE_LIT_EXPORT		1
#define EIE_REL_U8		2
#define EIE_LIT_U8		3
#define EIE_REL_U16		4
#define EIE_LIT_U16		5
#define EIE_REL_U32		6
#define EIE_LIT_U32		7
#define EIE_REL_U64		8
#define EIE_LIT_U64		9
//reserved
#define EIE_MAIN		16
#define EIE_MODULE_ALIGNMENT	17
//reserved
#define EIE_ABS_ADDR		32
#define EIE_HEAP_GLBL		33
#define EIE_END			0xFF
//See [::/LT/Adam/BinRep.APZ] ::/LT/Adam/BinRep.APZ

class ExeImportExportStruct
{
  ExeImportExportStruct *next,*last;
  I64 ip;
  U8 *str;
  U8 *src_link;
  U8 type;
};

#define EAA_ADD_U8		0
#define EAA_SUB_U8		1
#define EAA_ADD_U16		2
#define EAA_SUB_U16		3
#define EAA_ADD_U32		4
#define EAA_SUB_U32		5
#define EAA_ADD_U64		6
#define EAA_SUB_U64		7
class ExeAbsAddrStruct
{
  ExeAbsAddrStruct *next;
  I64 ip;
  U8 type,pad[7];
};

class ExeHeapGlblRefStruct
{
  ExeHeapGlblRefStruct *next;
  I64 ip;
};

class ExeHeapGlblStruct
{
  ExeHeapGlblStruct *next;
  U8 *str;
  U64 size;
  ExeHeapGlblRefStruct *references;
};

class ExeStruct
{
  U8 *code;
  I64 code_U8s;
  I64 max_align;
  ExeStruct *parent_exe;
  ExeImportExportStruct *next_ie,*last_ie;
  ExeAbsAddrStruct *abss;
  ExeHeapGlblStruct *heap_glbls;
};


#define OM_NO 0
#define OM_CB 1
#define OM_CW 2
#define OM_CD 3
#define OM_CP 4
#define OM_IB 5
#define OM_IW 6
#define OM_ID 7

#define ARGT_NO		0
#define ARGT_REL8	1
#define ARGT_REL16	2
#define ARGT_REL32	3

#define ARGT_UIMM8	4
#define ARGT_UIMM16	5
#define ARGT_UIMM32	6
#define ARGT_UIMM64	7

#define ARGT_R8		8
#define ARGT_R16	9
#define ARGT_R32	10
#define ARGT_R64	11

#define ARGT_IMM8	12
#define ARGT_IMM16	13
#define ARGT_IMM32	14
#define ARGT_IMM64	15

#define ARGT_RM8	16
#define ARGT_RM16	17
#define ARGT_RM32	18
#define ARGT_RM64	19

#define ARGT_M8		20
#define ARGT_M16	21
#define ARGT_M32	22
#define ARGT_M64	23

#define ARGT_M1632	24 // Not implemented
#define ARGT_M16N32	25 // Not implemented
#define ARGT_M16N16	26 // Not implemented
#define ARGT_M32N32	27 // Not implemented

#define ARGT_MOFFS8	28
#define ARGT_MOFFS16	29
#define ARGT_MOFFS32	30
#define ARGT_MOFFS64	31

#define ARGT_AL		32
#define ARGT_AX		33
#define ARGT_EAX	34
#define ARGT_RAX	35

#define ARGT_CL		36
#define ARGT_DX		37
#define ARGT_SREG	39

#define ARGT_SS		40
#define ARGT_DS		41
#define ARGT_ES		42
#define ARGT_FS		43

#define ARGT_GS		44
#define ARGT_CS		45
#define ARGT_ST0	46
#define ARGT_STI	47

#define ARGT_MM		48 // Not implemented
#define ARGT_MM32	49 // Not implemented
#define ARGT_MM64	50 // Not implemented
#define ARGT_XMM	51 // Not implemented

#define ARGT_XMM32	52 // Not implemented
#define ARGT_XMM64	53 // Not implemented
#define ARGT_XMM128	54 // Not implemented
#define ARGT_XMM0	55 // Not implemented

#define TK_EOF			0

#define TK_SUPERSCRIPT		0x001
#define TK_SUBSCRIPT		0x002
#define TK_NORMALSCRIPT		0x003

#define TK_IDENT		0x100
#define TK_STR			0x101
#define TK_INTEGER		0x102
#define TK_U8			0x103 // character(s) in single quotes
#define TK_F64			0x104
#define TK_PLUS_PLUS		0x105
#define TK_MINUS_MINUS		0x106
#define TK_DEREFERENCE		0x107
#define TK_DBL_COLON		0x108
#define TK_SHL			0x109
#define TK_SHR			0x10A
#define TK_EQUAL_EQUAL		0x10B
#define TK_NOT_EQUAL		0x10C
#define TK_LESS_EQUAL		0x10D
#define TK_GREATER_EQUAL	0x10E
#define TK_AND_AND		0x10F
#define TK_OR_OR		0x110
#define TK_XOR_XOR		0x111
#define TK_SHL_EQUAL		0x112
#define TK_SHR_EQUAL		0x113
#define TK_MUL_EQUAL		0x114
#define TK_DIV_EQUAL		0x115
#define TK_AND_EQUAL		0x116
#define TK_OR_EQUAL		0x117
#define TK_XOR_EQUAL		0x118
#define TK_ADD_EQUAL		0x119
#define TK_SUB_EQUAL		0x11A
#define TK_IF			0x11B
#define TK_IFDEF		0x11C
#define TK_IFNDEF		0x11D
#define TK_IFSTATIC		0x11E
#define TK_IFNSTATIC		0x11F
#define TK_ENDIF		0x120
#define TK_ELSE			0x121
#define TK_MOD_EQUAL		0x122
#define TK_DOT_DOT		0x123
#define TK_DOT_DOT_DOT		0x124
#define TK_INS_BINARY	0x125
#define TK_INS_BINARY_TYPE	0x126
#define TK_INS_BINARY_SIZE	0x127
#define TK_NUM_TK		0x128

//Lex flags
#define LF_PROMPT		1
#define Lf_PROMPT		0
#define LF_DONT_FREE_BUF	2
#define LF_NO_DEFINES		4
#define LF_IN_IF		8
#define LF_JUST_LOAD		16
#define LF_KEEP_NEW_LINES	32
#define LF_KEEP_DOT		64
#define LF_KEEP_NUM_SIGN	128
#define LF_KEEP_AT_SIGN		256
#define Lf_OPT_TRACE_PRESENT	9
#define LF_NOT_CONSTANT		0x00000010000
#define LF_NO_REG_OPT		0x00000020000
#define LF_IN_QUOTES		0x00000040000
#define LF_EXE_BLK		0x00000080000
#define LF_HAS_MISC_DATA	0x00000100000
#define LF_HAS_RETURN		0x00000200000
#define LF_ASM_EXPRESSIONS	0x00000400000
#define LF_UNRESOLVED		0x00000800000
#define LF_LOCAL		0x00001000000
#define LF_FUN_EXP		0x00002000000
#define Lf_FUN_EXP		25
#define LF_POSTINC		0x00004000000
#define LF_POSTDEC		0x00008000000
#define LF_PREINC		0x00010000000
#define LF_PREDEC		0x00020000000
#define LF_ARRAY		0x00040000000
#define LF_RAX			0x00080000000
#define LF_USE_LAST_U16		0x00100000000
#define LF_LAST_WAS_DOT		0x00200000000
#define LF_INSIDE_EXPRESSION	0x00400000000
#define LF_STATIC_COMPILE	0x00800000000
#define Lf_STATIC_COMPILE	35
#define LF_BY_ADDR		0x01000000000
#define Lf_BY_ADDR		36
#define LF_NO_ABSS		0x02000000000
#define Lf_RAW_ERRORS		38
#define LF_PAREN		0x08000000000
#define Lf_PAREN		39
#define LF_CLASS_IP		0x10000000000
#define LEX_MAX_IDENT	134
#define LFN_CNT		8
#define LFN_STK_CNT	32

class AsmCtrlStruct
{
  I64 ip;	//instruction ptr
  AsmArgStruct arg1,arg2;
  AsmCodeBlk *code;
  I64 num_code_U8s;
  I64 max_align;
  AsmUnresolvedRef *local_unresolved,*glbl_unresolved;
  ExeAbsAddrStruct *abss;
  ExeHeapGlblStruct *heap_glbls;
  I64 display_col,last_ip;
  U8 *last_label,*last_line_listed;
  LexFileStruct *last_lfn;
  I64 seg_size;
  BoolI8 has_locals,list;
};

class StreamBlkStruct
{
  StreamBlkStruct *next,*last;
  U8 *body;
};

class LexCbHeader
{
  LexCbHeader		*cb_next;
  CodeBlk		*cb_out_first;
  CodeBlk		*cb_out_last;
  IntermediateCode	*cb_out_end;
  IntermediateCode	*cb_out_ptr;
  IntermediateCode	*cb_last_out_ptr;
  CodeBlk		*cb_in_first;
  IntermediateCode	*cb_in_end;
  IntermediateCode	*cb_in_ptr;
  CbMiscStruct		*cb_misc_list,*cb_misc_list_end;
};

#define NUM_REGS	16

public class LexStruct
{
  LexStruct *next,*last;
  I64	token;
  U64	flags;
  I64	cur_i;
  F64	cur_f;
  U8	*ident;
  I64	ident_len;
  I64	class_ip;
  U8	*dollar;
  I64	dollar_cnt;
  U8	*cur_index;
  I64	last_U16;
  I64	min_line,max_line,last_line_num;
  I64	expression_start_line_num;
  I64	indent_level,temp_indent_level;
  I64	braces_cnt;
  I64	statement_cnt;
  I64	lock_cnt;
  U32	*alpha_numeric_bitmap;
  U8QueVect *indent_que_vect;
  SysHashTable	*define_hash_table;
  SysHashTable	*local_hash_table;
  SysHashTable	*glbl_hash_table;
  SysHashTable	*stream_parent_glbl_hash_table;
  U64 hash_mask;
  SysHashTable	 *hash_table_list;
  GenericHashEntry *hash_entry;
  AbsCntsU64 abs_cnts;
  LexUndefEntry  *undef_hash_entry;
  FunStruct	   *local_var_list;
  MemberListStruct *local_var_entry;
  CbMiscStruct	*lb_leave;
  U8 *cur_buf_ptr;
  LexFileStruct *cur_lfn,*fun_lfn;
  I64 stk_ptr;
  StreamBlkStruct *next_stream_blk,*last_stream_blk;
  ExeStruct *exe_struct;

  U64 pass;
  U64 opts;
  U64 pass_trace_mask;
  U64 error_cnt,warning_cnt;

  //For intermediate codes with multiple float ops (int<->float conversions)
  I64 cur_ic_float_op_num,last_ic_float_op_num;
  IntermediateCode *last_float_op_ic;
  BoolI8 last_dont_pushable,last_dont_popable,last_float_op_pos,
	 dont_push_float,pad[4];

  LexCbHeader cbh;
  AsmCtrlStruct *a;
  I64 a_depth;

  U0 end_of_zeroed_members;
  LexFileStruct lfns[LFN_CNT];
  LexFileStruct stk[LFN_STK_CNT];
};


class PCIDevStruct
{
  PCIDevStruct *next,*last;
  U16 vendor,dev_id;
  U8 bus,dev,fun,pad;
  U8 sub_code,base_code,pad[6];
  U8 *vendor_str,*dev_id_str;
};

#define LT_XSUM			0xA5CF3796
#define SRC_FILE_MASK		"*.CPZ;*.CPP;*.SPZ;*.SPP;*.HPZ;*.HPP;*.APZ;*.APP;*.AUZ;*.AUT"
#define TEXT_FILE_MASK		SRC_FILE_MASK ";*.TXZ;*.TXT"

public class MbrPrtEntry
{
  U8 active;	//0x80=active  0x00=inactive
  U8 start_head;
  U16 start_cyl;
  U8 type;
  U8 end_head;
  U16 end_cyl;
  U32 offset;  //Sects between MBR and first sect
  U32 size;	 //Sects in partition
};

public class MasterBootStruct
{
  U8 boot_code[440];
  U32 media_id;
  U16 zero;
  MbrPrtEntry p[4];
  U16 signature;  //AA55
};

public class LTBootStruct
{
  U8 jump_and_nop[3];
  U8 signature;		//MBR_PT_LT=0x88
  U16 U8s_per_sect;
  U16 sects_per_cluster;
  U64 sects;
  U64 root_cluster;
  U32 bitmap_sects;
  U32 unique_id;
  U8 code[478];
  U16 signature2; //0xAA55
};

class FAT32BootStruct
{
  U8 jump_and_nop[3];
  U8 oem_name[8];
  U16 bytes_per_sect;
  U8 sects_per_cluster;
  U16 reserved_sects;
  U8 copies_of_fat;
  U16 max_root_dir_entries;	  //Not used
  U16 old_sects_in_prt;	//Not used
  U8 media_desc;	  //F64 for hard disk
  U16 old_sects_per_fat;	  //Not used
  U16 sects_per_track;
  U16 num_heads;
  U32 hidden_sects;
  U32 sects;
  U32 sects_per_fat;
  U16 flags;
  U16 version;
  U32 root_cluster;
  U16 file_system_info_sect;
  U16 backup_boot_sect;
  U8 reserved[12];
  U8 log_drv_num;
  U8 unused;
  U8 ext_signature; //0x29
  U32 serial_num;
  U8 vol_name[11];
  U8 fat_name[8];
  U8 code[420];
  U16 signature;
};

class FAT32FileInfoSect
{
  U32 signature1;
  U8 unknown[480];
  U32 signature2;
  U32 free_clusters;
  U32 most_recently_allocated;
  U8 reserved[12];
  U32 signature3;
};

class FAT32DirEntry
{
  U8 name[11];
  U8 attr;
  U8 NTres;
  U8 CrtTimeTenth;
  U16 CrtTime;
  U16 CrtDate;
  U16 LstAccDate;
  U16 cluster_hi;
  U16 WrtTime;
  U16 WrtDate;
  U16 cluster_lo;
  U32 size;
};

class FAT32LongDirEntry
{
  U8 ord;
  U16 name1[5];
  U8 attr;
  U8 type;
  U8 xsum;
  U16 name2[6];
  U16 zero;
  U16 name3[2];
};

#define FAT32_ENTRIES_PER_BLK	(BLK_SIZE/sizeof(FAT32DirEntry))

public class U16Palindrome
{
  U16 little,big;
};

public class U32Palindrome
{
  U32 little,big;
};

class ATAPITrackInfo
{
  U16 track_info_len;
  U8 track_lo;
  U8 session_lo;
  U8 reserved;
  U16 misc1;
  U8 misc2;
  U32 track_start;
  U32 next_writable;
  U32 free_blks;
  union {
    U32 fixed_packet_size;
    U32 blocking_factor;
  };
  union {
    U32 track_size;
    U32 rzone_end;
  };
  U32 last_recorded;
  U8 track_hi;
  U8 session_hi;
  U16 reserved;
  U32 read_compatibility;
  U32 next_layer_jmp;
  U32 last_layer_jmp;
};

//ISO1 is ISO9660
//ISO2 is ISO13490
//ISO3 is ISO13346

class ISO1Date
{
  U8 year,mon,day,hour,min,sec,sec100;
};

class ISO1DirEntry
{
  U8 len;
  U8 ext_attr_len;
  U32Palindrome loc;
  U32Palindrome size;
  ISO1Date date;
  U8 flags;
  U8 file_unit_size;
  U8 interleave;
  U16Palindrome vol_seq_num;
  U8 name_len;
  U8 name;
};

class ISO1PathTableEntry
{
  U8 name_len;
  U8 zero;
  U32 blk;
  U16 parent_entry_num;
  U16 name[1];	//Aligned to U16 boundries
};

//9660
#define ISO1T_BOOT_RECORD		0
#define ISO1T_PRI_VOL_DESC		1
#define ISO1T_SUPPLEMENTARY_DESC	2
#define ISO1T_VOL_PRT_DESC		3
#define ISO1T_TERMINATOR		255

class ISO1PriDesc
{
  U8 type;
  U8 id[5];
  U8 version;
  U8 unused1;
  U8 system_id[32];
  U8 vol_id[32];
  U8 unused2[8];
  U32Palindrome vol_space_size;
  U8 unused3[32];
  U16Palindrome vol_set_size;
  U16Palindrome vol_seq_num;
  U16Palindrome log_block_size;
  U32Palindrome path_table_size;
  U32 type_l_path_table;
  U32 opt_type_l_path_table;
  U32 type_m_path_table;
  U32 opt_type_m_path_table;
  ISO1DirEntry root_dir_record;
  U8 vol_set_id[128];
  U8 publisher_id[128];
  U8 preparer_id[128];
  U8 application_id[128];
  U8 copyright_file_id[37];
  U8 abstract_file_id[37];
  U8 bibliographic_file_id[37];
  U8 creation_date[17];
  U8 modification_date[17];
  U8 expiration_date[17];
  U8 effective_date[17];
  U8 file_structure_version;
  U8 unused4;
  U8 application_data[512];
  U8 unused5[653];
};

//ISO13346 tag id types
#define ISO3T_PRI_VOL_DESC		1
#define ISO3T_ANCHOR_VOL_DESC_PTR	2
#define ISO3T_VOL_DESC_PTR		3
#define ISO3T_IMPLEMENTATION_VOL_DESC	4
#define ISO3T_PRT_DESC			5
#define ISO3T_LOG_VOL_DESC		6
#define ISO3T_UNALLOCATED_SPACE_DESC	7
#define ISO3T_TERMINATOR_DESC		8
#define ISO3T_LOG_VOL_INTEGRITY_DESC	9
#define ISO3T_FILE_SET_DESC		256
#define ISO3T_FILE_ID_DESC		257
#define ISO3T_ALLOC_EXTENT_DESC		258
#define ISO3T_INDIRECT_ENTRY		259
#define ISO3T_TERMINAL_ENTRY		260
#define ISO3T_FILE_ENTRY		261
#define ISO3T_EXT_ATTR_HEADER_DESC	262
#define ISO3T_UNALLOC_SPACE_ENTRY	263
#define ISO3T_SPACE_BITMAP_DESC		264
#define ISO3T_PRT_INTEGRITY_ENTRY	265
#define ISO3T_EXT_FILE_ENTRY		266

class ISO3Tag
{
  U16 id;
  U16 version;
  U8 xsum;
  U8 reserved;
  U16 serial_num;
  U16 desc_crc;
  U16 desc_crc_len;
  U32 loc;
};

class ISO3Date
{
  U16 time_zone;
  I16 year;
  U8 mon;
  U8 day;
  U8 hour;
  U8 min;
  U8 sec;
  U8 sec100;
  U8 sec10000;
  U8 sec1000000;
};


class ISO3DString32
{
  U8 body[31];
  U8 len;
};

class ISO3DString128
{
  U8 body[127];
  U8 len;
};

class ISO3CharSpec
{
  U8 type;
  U8 body[63];
};

class ISO3RegId
{
  U8 flags;
  U8 id[23];
  U8 id_suffix[8];
};

class ISO3ExtentAD
{
  U32 len;
  U32 loc;
};

class ISO3LBAdd
{
  U32 log_blk_num;
  U16 prt_ref_num;
};

class ISO3LongAD
{
  U32 len;
  ISO3LBAdd loc;
  U8 implementation_use[6];
};

class ISO3ICBTag
{
  U32 prior_record_direct_entries;
  U16 strategy_type;
  U16 strategy_param;
  U16 max_entries;
  U8 reserved;
  U8 file_type;
  ISO3LBAdd parent_icb_loc;
  U16 flags;
};

class ISO3PriDesc
{
  ISO3Tag tag;
  U32 vol_desc_seq_num;
  U32 pri_vol_desc_num;
  ISO3DString32 vol_id;
  U16 vol_seq_num;
  U16 max_vol_seq_num;
  U16 interchg_level;
  U16 max_interchg_level;
  U32 char_set_list;
  U32 max_char_set_list;
  ISO3DString128 vol_set_id;
  ISO3CharSpec desc_char_set;
  ISO3CharSpec explanatory_char_set;
  ISO3ExtentAD vol_abstract;
  ISO3ExtentAD vol_copyright_notice;
  ISO3RegId app_id;
  ISO3Date recording_date_time;
  ISO3RegId implementation_id[32];
  U8 implementation_use[64];
  U32 pred_vol_desc_seq_loc;
  U16 flags;
  U8 reserved[22];
};

class ISO3PrtMap
{
  U8 type;
  U8 len;
  U0 mapping;
};

class ISO3PrtMap1
{
  U8 type;
  U8 len;
  U16 vol_seq_num;
  U16 prt_num;
};

class ISO3PrtMap2
{
  U8 type;
  U8 len;
  U8 prt_id[62];
};

class ISO3LogVolDesc
{
  ISO3Tag tag;
  U32 vol_desc_seq_num;
  ISO3CharSpec desc_char_set;
  ISO3DString128 log_vol_id;
  U32 blk_size;
  ISO3RegId domain_id;
  U8 log_vol_contents_use[16];
  U32 map_table_len;
  U32 num_prt_maps;
  ISO3RegId implementation_id;
  U8 implementation_use[128];
  ISO3ExtentAD integrity_seq_extent;
  U0 prt_maps;
};

class ISO3Prt
{
  ISO3Tag tag;
  U32 vol_desc_seq_num;
  U16 flags;
  U16 num;
  ISO3RegId contents;
  U8 contents_use[128];
  U32 access_type;
  U32 starting_loc;
  U32 len;
  ISO3RegId implementation_id;
  U8 implementation_use[128];
  U8 reserved[156];
};

class ISO3FileSetDesc
{
  ISO3Tag tag;
  ISO3Date recording_date_time;
  U16 interchg_level;
  U16 max_interchg_level;
  U32 char_set_list;
  U32 max_char_set_list;
  U32 file_set_num;
  U32 file_set_desc_num;
  ISO3CharSpec log_vol_id_char_set;
  ISO3DString128 log_vol_id;
  ISO3CharSpec	file_set_char_set;
  ISO3DString32 file_set_id;
  ISO3DString32 copyright_file_id;
  ISO3DString32 abstract_file_id;
  ISO3LongAD root_dir_icb;
  ISO3RegId domain_id;
  ISO3LongAD next_extent;
  ISO3LongAD sys_stream_dir_icb;
  U8 reserved[32];
};

class ISO3FileIdDesc
{
  ISO3Tag tag;
  U16 version;
  U8 characteristics;
  U8 len_of_file_id;
  ISO3LongAD icb;
  U16 len_of_implementation_use;
  U0 implementation_use;
  U0 file_id;
};

class ISO3FileEntry
{
  ISO3Tag tag;
  ISO3ICBTag icb_tag;
  U32 uid;
  U32 gid;
  U32 permissions;
  U16 file_link_cnt;
  U8 record_fmt;
  U8 record_display_attr;
  U32 record_len;
  U64 info_len;
  U64 log_blks_recorded;
  ISO3Date access_date_time;
  ISO3Date modification_date_time;
  ISO3Date attr_date_time;
  U32 chk_pt;
  ISO3LongAD ext_attr_icb;
  ISO3RegId implementation_id;
  U64 unique_id;
  U32 ext_attr_len;
  U32 alloc_desc_len;
  U0 ext_attr;
  U0 alloc_desc;
};

class ISO3ExtFileEntry
{
  ISO3Tag tag;
  ISO3ICBTag icb_tag;
  U32 uid;
  U32 gid;
  U32 permissions;
  U16 file_link_cnt;
  U8 record_fmt;
  U8 record_display_attr;
  U32 record_len;
  U64 info_len;
  U64 object_size;
  U64 log_blks_recorded;
  ISO3Date access_date_time;
  ISO3Date modification_date_time;
  ISO3Date creation_date_time;
  ISO3Date attr_date_time;
  U32 chk_pt;
  U32 reserved;
  ISO3LongAD ext_attr_icb;
  ISO3LongAD stream_dir_icb;
  ISO3RegId implementation_id;
  U64 unique_id;
  U32 ext_attr_len;
  U32 alloc_desc_len;
  U0 ext_attr;
  U0 alloc_desc;
};

#define ISO1_BASE_YEAR	1900

//media types
#define MT_CD		1
#define MT_DVD		2

public class LTDateStruct
{
  U8 sec10000,sec100,sec,min,hour;
  U8 day_of_week,day_of_mon,mon;
  I32 year;
};


//LTDirEntry flags (Used by [C:/LT/Adam/Dsk/FileMgr.APZ,577] FileMgr())
#define LTDEF_PROCESSED		1
#define LTDEF_REMOVABLE		2
#define LTDEF_NOT_INITIALIZED	4

#define LT_MAX_FILENAME_LEN	25
public class LTDirEntry
{
  LTDirEntry *next,*parent,*sub;
  U8 *full_name;
  U64 flags;
  I64 user_data;

  U0 start;
  U16 attr;
  U8 name[LT_MAX_FILENAME_LEN+1];
  U32 xsum;
  U64 cluster;
  U64 size;
  U64 expanded_size;
  LTDate datetime;
};

#define LT_DIR_ENTRY_SIZE (sizeof(LTDirEntry)-offset(LTDirEntry.start))

#define LTBDT_NULL		0
#define LTBDT_RAM		1
#define LTBDT_ATA		2
#define LTBDT_ATAPI		3
#define LTBDT_NON_LT_FILE	4

#define LTBDF_REMOVABLE			1
#define LTBDF_INITIALIZED		2
#define LTBDF_READ_ONLY			4
#define LTBDF_READ_ONLY_OVERRIDE	8
#define LTBDF_LAST_WAS_WRITE		16
#define LTBDF_READ_CACHE		32
#define LTBDF_WRITE_CACHE		64
#define LTBDF_FMT			128
#define LTBDF_INIT_IN_PROGRESS		256
#define LTBDF_EXT_SIZE			512

#define LTBDf_REMOVABLE			0
#define LTBDf_INITIALIZED		1
#define LTBDf_READ_ONLY			2
#define LTBDf_READ_ONLY_OVERRIDE	3
#define LTBDf_LAST_WAS_WRITE		4
#define LTBDf_READ_CACHE		5
#define LTBDf_WRITE_CACHE		6
#define LTBDf_FMT			7
#define LTBDf_INIT_IN_PROGRESS		8
#define LTBDf_EXT_SIZE			9

//locked flags
#define LTBDlf_LOCKED	0

#define NUM_LTBLKDEVS	32
#define LTBLKDEV_SIGNATURE_VAL	'BDSV'
public class LTBlkDev
{
  LTBlkDev *lock_fwding;
  U64 locked_flags;
  U32 type,flags;
  U32 bd_signature;
  U8 drv_let_base,unit,pad[2];
  U32 base0,base1;
  U16 read_freq,write_freq;
  U32 blk_size;
  U64 offset;
  U64 min_blk,max_blk;
  U64 most_recent_blk;
  U16 *dev_id_record;
  U8 *RAM_dsk;
  TaskStruct *owning_task;

  //The following are for NON_LT_FILE partitions
  U64 non_lt_drv;
  U64 non_lt_offset;

  U8 *stat;
  U64 last_time;
  U32 max_reads,max_writes;
};


class ATARepStruct
{
  ATARepStruct *next;
  U32 unit,base0,base1,irq,num;
};

#define LTPf_CACHE_WRITES	0
#define LTPf_CACHE_ALL_WRITES	1

//locked flags
#define LTPlf_LOCKED		0

#define PT_NULL		0
#define PT_LT		1 //Supported
#define PT_FAT32	2 //Supported except for short names, to some degree
#define PT_ISO9660	3 //Supported (CD-ROM)
#define PT_ISO13490	4 //Not Supported (CD-ROM)
#define PT_ISO13346	5 //Not Supported (CD-ROM)
#define PT_NTFS		6 //Not Supported
#define PT_UNKNOWN	7

#define PTf_DISABLE	31 //High bit of type is disable

#define MBR_PT_FAT12	0x01
#define MBR_PT_FAT32a	0x0B
#define MBR_PT_FAT32b	0x0C
#define MBR_PT_FAT32c	0x1B
#define MBR_PT_FAT32d	0x1C
#define MBR_PT_FAT32e	0x8B
#define MBR_PT_FAT32f	0x8C
#define MBR_PT_NTFS	0x07
#define MBR_PT_LT	0x88

public class LTFreeList
{
  LTFreeList *next,*last;
  U64 start,size;
};

#define NUM_LTPRTS		32

#define LTPRT_SIGNATURE_VAL	'PTSV'
public class LTPrt
{//LT Partition
//Don't access ->drive directly in case a drive has been remapped.
//Use [C:/LT/OSMain/Dsk/DskStr.CPZ,72] Prt2Drv().
  U64 locked_flags;
  U32 type, //Bit31 is used as a disable, since it will be unknown
     flags;
  U32 pt_signature;
  U8 drv_let,text_attr,pad[2];
  U64 offset;
  U64 size;
  U64 file_system_info_sect;
  U64 map1,map2;
  U64 root_cluster;
  U64 data;
  U64 spc; //sects_per_cluster
  TaskStruct *owning_task;
  LTBlkDev *bdev;

  FAT32FileInfoSect *fis;
  I64 num_buffered_map_blks;
  U64 cur_map_blk_num;
  U32 *cur_map_blk;
  U8 *buffered_map_dirty_bits;
  LTFreeList *next_free,*last_free;

  BoolI64 (*rblks)(LTPrt *p,U8 *buf,U64 blk,U64 cnt);
  BoolI64 (*wblks)(LTPrt *p,U8 *buf,U64 blk,U64 cnt);
};

#define LTDC_TABLE_SIZE	0x2000

#define LTDCf_DIRTY		0

public class LTCacheBlk
{
  LTCacheBlk *next_lru,*last_lru;
  LTCacheBlk *next_hash,*last_hash;
  LTCacheBlk *next_dirty,*last_dirty;
  LTPrt *p;
  U64 time_stamp;
  U64 blk,flags;
  U8 body[BLK_SIZE];
};

#define EXCEPT_LOCAL		0 // good for narrow scope user handling
#define EXCEPT_BREAK		1
#define EXCEPT_OUT_OF_MEM	2
#define EXCEPT_DIVIDE_BY_ZERO	3
#define EXCEPT_FLOATING		4
#define EXCEPT_COMPILER		5
#define EXCEPT_UNDEF_DEFINE	6
#define EXCEPT_UNDEF_EXTERN	7
#define EXCEPT_FILE		8
#define EXCEPT_ARC_XSUM		9
#define EXCEPT_PRT		10
#define EXCEPT_BLKDEV		11
#define EXCEPT_GRAPHICS		12
#define EXCEPT_SPRINTF		13
#define EXCEPT_GETF		14
#define EXCEPT_BIN_MODULE	15
#define EXCEPT_MULTICORE	16
[::/LT/OSMain/Define1a.CPZ,ST_EXCEPT] EXCEPT_DEFINE

class ExceptStruct
{
  ExceptStruct *next,*last;
  U64 handler_catch,handler_untry;
  U64 rsp,rbp,rflags;
  U64 rsi,rdi,r10,r11,
     r12,r13,r14,r15;
};

//MEM RELATED
#define MBS_USED_SIGNATURE_VAL		'MBUs'
#define MBS_UNUSED_SIGNATURE_VAL	'MBUn'
class MemBlk
{
  MemBlk *next,*last;
  U32 mb_signature;
  U32 pages;
};

#define MRT_UNUSED	0
#define MRT_RAM		1
#define MRT_DEV		2

class MemRange
{
  MemRange *next,*last;
  U32 type,flags;
  U8 *base;
  U64 size;
};


#define _CFG_HEAP_DBG FALSE

#if _CFG_HEAP_DBG
class UnusedAllocatedMem
{
  HeapCtrl *hc;
  U8 *caller1,*caller2;
  UnusedAllocatedMem *next;
  I64 size;
};
class UsedAllocatedMem
{
  HeapCtrl *hc;
  U8 *caller1,*caller2;
  UsedAllocatedMem *next,*last;
  I64 size;
  U0 start;
};
#else
class UnusedAllocatedMem
{
  U0 hc;
  U0 caller1,caller2;
  UnusedAllocatedMem *next;
  I64 size;
};
class UsedAllocatedMem
{
  HeapCtrl *hc;
  U0 caller1,caller2;
  U0 next,last;
  I64 size;
  U0 start;
};
#endif


//LoseThos does not mess with page tables
//after [::/LT/OSMain/Memory.CPZ,1] boot init identity maps.
//PAGE_SIZE is arbitrary, not related to CPU hardware.
#define PAGE_SIZE		0x200
#define PAGE_BITS		9

#define HEAP_HASH_SIZE		1024
#define FREE_PAGE_HASH_SIZE	0x100

#define ADAM_STK		(PAGE_SIZE*512) //limit of like 16384*PAGE_SIZE
#define SETH_STK		(PAGE_SIZE*512)
#define INTERRUPT_STK		(PAGE_SIZE*512)
#define DFT_STK			(PAGE_SIZE*512)
#define MP_DFT_STK		(PAGE_SIZE*512)

#define ADAM_HASH_SIZE		(1<<18)
#define STD_HASH_SIZE		(1<<12)
#define MP_STD_HASH_SIZE	(1<<12)

//locked flags
#define BPlf_LOCKED		0
public class BlkPool
//See [::/LT/OSMain/OSStart.CPZ,SYS_BP_START] SYS_BP_START
{
  U64 locked_flags,size;
  MemBlk *mem_free_list;
  MemBlk *mem_free_2meg_list;
  MemBlk *free_page_hash[FREE_PAGE_HASH_SIZE];
  MemBlk *free_page_hash2[64-PAGE_BITS];
};

#define HCf_NON_TASK_QUE	0
//locked flags
#define HClf_LOCKED		0

#define HEAP_CTRL_SIGNATURE_VAL	'HcSV'
public class HeapCtrl
{
  BlkPool *bp;
  U32 hc_signature,pad;
  U64 locked_flags,flags;
  TaskStruct *mem_task;
  MemBlk *next_mem_blk,*last_mem_blk;
  UnusedAllocatedMem *malloc_free_list;
  UsedAllocatedMem *next_um,*last_um;
  UnusedAllocatedMem *heap_hash[HEAP_HASH_SIZE/sizeof(U0 *)];
};

//Raster operations
#define ROP_EQU				0x0000
#define ROP_XOR				0x0100
#define ROP_COLLISION			0x0200
#define ROP_MONO			0x0300

//These are just for [C:/LT/OSMain/Adam1a.HPZ,2903] ROPF_PROBABILITY_DITHER
//See [C:/LT/Adam/Gr/MathGr.APZ,460] GrLighting().
#define ROPF_HALF_RANGE_COLOR		0x1000
#define ROPF_TWO_SIDED			0x2000

//These always go in the c1.rop of a ColorROPU32
#define ROPF_DITHER			0x40000000
#define ROPF_PROBABILITY_DITHER		0x80000000

#define ROPB_EQU			0x00
#define ROPB_XOR			0x01
#define ROPB_COLLISION			0x02
#define ROPB_MONO			0x03
#define ROPBF_HALF_RANGE_COLOR		0x10
#define ROPBF_TWO_SIDED			0x20
#define ROPBF_DITHER			0x40
#define ROPBF_PROBABILITY_DITHER	0x80

public U16 class ColorROPU16 //Don't use this, use ColorROPU32
{
  U8 color,rop;
};

public U32 class ColorROPU32
{
  ColorROPU16 c0,c1;
};
#define COLORROP_COLORS_MASK	0x00FF00FF
#define COLORROP_NO_ROP0_MASK	0xFFFF00FF
#define COLORROP_BITS		16

#define CTRLT_GENERIC		0
#define CTRLT_WIN_HSCROLL	1 //unique
#define CTRLT_WIN_VSCROLL	2 //unique
#define CTRLT_VIEWING_ANGLES	3 //unique

#define CTRLF_SHOW		1
#define CTRLF_BORDER		2
#define CTRLF_CAPTURE_LEFT_IP	4
#define CTRLF_CAPTURE_RIGHT_IP	8
#define CTRLF_CLICKED		16

#define WSSf_SET_TO_POS		0

public class WinScrollState
{
  I64 min,pos,max;
  U32 flags;
  U8 color,pad[3];
};

public class ViewAnglesState
{
  I64 sx,sy,sz;
  F64 ax,ay,az;
  ColorROPU32 cx,cy,cz,cbd,cbg,cfg;
};

public class Ctrl
{
  Ctrl *next,*last;
  TaskStruct *win_task;
  I64 type,flags;

//win pixel coordinates
  I64 left,right,top,bottom;

//screen pixel coordinates (derived)
  I64 screen_left,screen_right,
     screen_top,screen_bottom;

  U8 *state;

//called on resize
  U0 (*update_derived_vals)(Ctrl *c);

  U0 (*draw_it)(GrDC *dc,Ctrl *c);

  BoolI64 (*inside_ctrl)(Ctrl *c,I64 x,I64 y); //For nonbox shapes
  U0 (*left_click)(Ctrl *c,I64 x,I64 y,BoolI8 down);
  U0 (*right_click)(Ctrl *c,I64 x,I64 y,BoolI8 down);
  U0 (*wheel_chg)(Ctrl *c,I64 delta);
};

#define MENU_SIGNATURE_VAL	'MnuS'
class MenuEntry
{
  MenuEntry *next;
  MenuEntry *sub;
  U8 name[32];
  I64 msg_code,p1,p2;
  BoolI8 checked,dir,pad[6];
};

class Menu
{
  Menu *next;
  MenuEntry *sub;
  TaskStruct *task;
  U64 flags;
  U32 mu_signature;
  U8 attr,checked_attr,pad[2];
};

//win_inhibit mask
#define WIF_MENU		0x001
#define WIF_CTRLS		0x002
#define WIF_IP_WHEEL		0x004
#define WIF_IP_L		0x008
#define WIF_IP_R		0x010
#define WIF_BORDER		0x020
#define WIF_DBL_CLICK		0x040
#define WIF_GRAB_SCROLL		0x080
#define WIF_LTF			0x100
#define WIF_ODE			0x200
#define WIF_ALL			(-1-WIF_LTF-WIF_ODE)

#define WIf_MENU		0
#define WIf_CTRLS		1
#define WIf_IP_WHEEL		2
#define WIf_IP_L		3
#define WIf_IP_R		4
#define WIf_BORDER		5
#define WIf_DBL_CLICK		6
#define WIf_GRAB_SCROLL		7
#define WIf_LTF			8
#define WIf_ODE			9

#define WIN_DFT_MAX_REFRESH	60.0

class FPUState
{
  U8 body[512];
};

public U32i class RGBEntry
{
  U8 r,g,b,reserved;
};

public U32i class BGREntry
{
  U8 b,g,r,reserved;
};

public class SnapStruct
{
  I64 x,y,z;
  F64 dx,dy,dz;
};

#define TSF_SAME_SONG	1

class TaskSettings
{
  TaskSettings *next;
  I64 left,right,top,bottom;
  U0 (*draw_it)(TaskStruct *task,GrDC *dc);
  U0 (*task_end_cb)();
  TaskStruct *song_task,*animate_task;
  F64	win_max_refresh;
  I64	scroll_x,scroll_y,scroll_z;
  SnapStruct snap;
  U32	win_inhibit;
  U8	text_attr;
  U8	border_attr;
  U8	ltf_cur_attr;
  U8	ltf_aux_attr;
  BGREntry palette4[16];
  BoolI8 border;
  BoolI8 preempt;
  BoolI8 cursor;
  BoolI8 scroll;
  BoolI8 wordstat;
  U8	pad[3];
};

#define NUM_PROGRESS_BARS	4
#define PROGRESS_DESC_LEN	(64-8-8-1)
class ProgressStruct
{
  U64 val,max;
  U8  desc[PROGRESS_DESC_LEN+1];
};

// TASK task flags
#define TASKf_KILL_TASK		0
#define TASKf_SUSPENDED		1
#define TASKf_LOCAL_USER	2
#define TASKf_PREEMPT		3
#define TASKf_IDLE		4
#define TASKf_INPUT_FILTER_TASK	5
#define TASKf_FILTER_INPUT	6
#define TASKf_HAS_SONG		7
#define TASKf_NOT_RAW		8
#define TASKf_LOCK_TITLE	9
#define TASKf_DISABLE_BPTS	10
#define TASKf_AWAITING_MSG	11
#define TASKf_CURRENTLY_RUNNING	12
#define TASKf_BREAK_LOCKED	13
#define TASKf_PENDING_BREAK	14
#define TASKf_PAUSED		15
#define TASKf_SERVANT_USER	16
#define TASKf_BREAK_TO_SHIFT_ESC 17
#define TASKf_CFG_RECORD_DONE	18
#define TASKf_LOCAL_INITIALIZED	19
#define TASKf_KILL_AFTER_DBG	20

//locked flags
#define TASKLf_TASK_LOCK		0

#define DISPLAYf_SHOW			0
#define DISPLAYf_HAS_MENU		1
#define DISPLAYf_HAS_CLOSE_WIN		2
#define DISPLAYf_HAS_BEEN_RESIZED	3
#define DISPLAYf_SILENT			4
#define DISPLAYf_HAS_BEEN_RESIZED2	5
#define DISPLAYf_NO_BORDER		6
#define DISPLAYf_CHILDREN_NOT_ON_TOP	7
#define DISPLAYf_WIN_ON_TOP		8

#define SPAWNF_ADD_TO_QUE		1

#define TASK_SIGNATURE_VAL		'TskS'
#define TASK_IN_QUE_SIGNATURE_VAL	'TskQ'
#define TASK_COMBINED_SIGNATURE_VAL	(TASK_SIGNATURE_VAL+TASK_IN_QUE_SIGNATURE_VAL<<32)

#define TASK_TITLE_LEN			127
#define TASK_NAME_LEN			31
#define TASK_WALLPAPER_DATA_SIZE	128
class TaskStk
{
  TaskStk *next_stk;
  U64 stk_size;
  U64 stk_ptr;
  U0 stk_base;
};

public class TaskStruct
{
  TaskStruct *addr;
  U32 task_signature,task_in_que_signature;
  U64 task_flags,locked_flags;
  U32 display_flags,win_inhibit;
  U64 wake_jiffy;

  HeapCtrl *code_heap,*data_heap;

  Ltf	*next_ltf,*last_ltf;
  I64	win_left;
  I64	win_right;
  I64	win_top;
  I64	win_bottom;

  LTPrt *cur_prt;
  U8	*cur_dir;

  TaskStruct *parent_task;
  TaskStruct *next_task,*last_task;
  TaskStruct *next_input_filter_task,*last_input_filter_task;
  TaskStruct *next_sibling_task,*last_sibling_task;
  TaskStruct *next_child_task,*last_child_task;

  //These are derived from left,top,right,bottom
  I64	win_width;
  I64	win_height;
  I64	win_pixel_left; //These are in pixels, not characters
  I64	win_pixel_right;
  I64	win_pixel_width;
  I64	win_pixel_top;
  I64	win_pixel_bottom;
  I64	win_pixel_height;
  I64	win_border_pixel_left;
  I64	win_border_pixel_right;
  I64	win_border_pixel_top;
  I64	win_border_pixel_bottom;

  I64	win_scroll_x,win_scroll_y,win_scroll_z;
  F64	win_max_refresh;
  SnapStruct snap;

//These must be in this order
//for [::/LT/OSMain/Scheduler.CPZ,SYS_SAVE_CONTEXT] SYS_SAVE_CONTEXT and [::/LT/OSMain/Scheduler.CPZ,SYS_RESTORE_CONTEXT] SYS_RESTORE_CONTEXT
  U64 rip;
  U64 rflags;
  U64 rsp;
  U64 rsi;
  U64 rax;
  U64 rcx;
  U64 rdx;
  U64 rbx;
  U64 rbp;
  U64 rdi;
  U64 r8;
  U64 r9;
  U64 r10;
  U64 r11;
  U64 r12;
  U64 r13;
  U64 r14;
  U64 r15;

  CPUStruct *gs;

  FPUState *fpu_mmx;
  U64	time_slice_start;
  U64	total_time;  //Cycles
  F64	pause_start,pause_total;  //Seconds
  U64	swap_cnter;

  U0	(*draw_it)(TaskStruct *task,GrDC *dc);

  U8	task_title[TASK_TITLE_LEN+1];
  U8	task_name[TASK_NAME_LEN+1];
  U8	wallpaper_data[TASK_WALLPAPER_DATA_SIZE];
 
  U8	text_attr;
  U8	border_attr;
  U16	win_z_num,pad[2];

  TaskStk *stk;

  ExceptStruct *next_except,*last_except;
  U64	except_rbp;	//Stores throw routine's RBP
  U64	except_argc;
  I64	*except_argv;
  BoolI8 catch_except,ignore_except,pad[6];
  SysBpt *bpt_list;
  Ctrl	*next_ctrl,*last_ctrl;
  Menu	*cur_menu;
  TaskSettings *next_settings;
  Ode	*next_ode,*last_ode;
  F64	last_ode_time;
  SysHashTable *hash_table;

#define MSG_NULL		0
#define MSG_CMD			1
#define MSG_KEY_DOWN		2
#define MSG_KEY_UP		3
#define MSG_IP_MOVE		4
#define MSG_IP_L_DOWN		5
#define MSG_IP_L_UP		6
#define MSG_IP_L_D_DOWN 	7
#define MSG_IP_L_D_UP		8
#define MSG_IP_R_DOWN		9
#define MSG_IP_R_UP		10
#define MSG_IP_R_D_DOWN 	11
#define MSG_IP_R_D_UP		12
#define MSG_FOCUS		13
#define MSG_RESIZE		14
#define MSG_MOVE		15

#define MSG_KEY_DOWN_UP		-2
#define MSG_IP_L_DOWN_UP	-5
#define MSG_IP_L_D_DOWN_UP	-7
#define MSG_IP_R_DOWN_UP	-9
#define MSG_IP_R_D_DOWN_UP	-11

#define MSGF_ALLOW_KEY_DESC	63

  TaskCmdStruct *next_servant_cmd,*last_servant_cmd;
  TaskCmdStruct *next_master_cmd,*last_master_cmd;

  LexStruct *next_lex,*last_lex;

  I64	win_old_left;
  I64	win_old_right;
  I64	win_old_top;
  I64	win_old_bottom;

  U0	(*task_end_cb)();
  TaskStruct *song_task,*animate_task;
  U64	rnd_seed;
  U32	user_num;
  U32	fault_num;
  U64	fault_err_code;
  U64	answers[4];
  U16	answer_types[4];
  U64	answers_displayed;
  TaskStruct *dbg_task;
  TaskStruct *popup_task;
  WinScrollState horz_scroll,vert_scroll;

  I64	user_data0,user_data1,
	user_data2,user_data3;
};

class TssStruct
{
  U32 res1;
  U64 rsp0,rsp1,rsp2;
  U64 res2;
  U64 ist1,ist2,ist3,ist4,ist5,ist6,ist7;
  U64 res3;
  U16 res4,io_map_offset;
  U8 io_map[0x10000/8];
  U64 *st0,*st1,*st2;
  U16 tr,tr_ring3;
};

#define ans  (Fs->answers[0])
#define ans0 (Fs->answers[0])
#define ans1 (Fs->answers[1])
#define ans2 (Fs->answers[2])
#define ans3 (Fs->answers[3])

#define ansf  (Fs->answers[0](F64))
#define ansf0 (Fs->answers[0](F64))
#define ansf1 (Fs->answers[1](F64))
#define ansf2 (Fs->answers[2](F64))
#define ansf3 (Fs->answers[3](F64))

#define KBD_PORT 0x60
#define KBD_CTRL 0x64

#define CH_CTRLA		0x01
#define CH_CTRLB		0x02
#define CH_CTRLC		0x03
#define CH_CTRLD		0x04
#define CH_CTRLE		0x05
#define CH_CTRLF		0x06
#define CH_CTRLG		0x07
#define CH_CTRLH		0x08
#define CH_CTRLI		0x09
#define CH_CTRLJ		0x0A
#define CH_CTRLK		0x0B
#define CH_CTRLL		0x0C
#define CH_CTRLM		0x0D
#define CH_CTRLN		0x0E
#define CH_CTRLO		0x0F
#define CH_CTRLP		0x10
#define CH_CTRLQ		0x11
#define CH_CTRLR		0x12
#define CH_CTRLS		0x13
#define CH_CTRLT		0x14
#define CH_CTRLU		0x15
#define CH_CTRLV		0x16
#define CH_CTRLW		0x17
#define CH_CTRLX		0x18
#define CH_CTRLY		0x19
#define CH_CTRLZ		0x1A

#define CH_CURSOR		0x05
#define CH_BACKSPACE		0x08
#define CH_TAB			0x09
#define CH_NEW_LINE		0x0A
#define CH_FORM_FEED		0x0C
#define CH_CARRIAGE_RETURN	0x0D
#define CH_ESC			0x1B
#define CH_SHIFT_ESC		0x1C
#define CH_SHIFT_SPACE		0x1F
#define CH_SPACE		0x20

#define CH_SINGLE_VERT		0xB3
#define CH_DBL_VERT		0xBA
#define CH_SINGLE_HORZ		0xC4
#define CH_DBL_HORZ		0xCD
#define CH_SINGLE_TL		0xDA
#define CH_DBL_TL		0xC9
#define CH_SINGLE_TR		0xBF
#define CH_DBL_TR		0xBB
#define CH_DBL_BR		0xBC
#define CH_SINGLE_BL		0xC0
#define CH_DBL_BL		0xC8
#define CH_SINGLE_BR		0xD9
#define CH_SOLID		0xDB

//Scan code flags
#define SCf_E0_PREFIX		7
#define SCf_KEY_UP		8
#define SCf_SHIFT		9
#define SCf_CTRL		10
#define SCf_ALT 		11
#define SCf_CAPS		12
#define SCf_NUM 		13
#define SCf_SCROLL		14
#define SCf_NEW_KEY		15
#define SCf_IP_L_DOWN		16
#define SCf_IP_R_DOWN		17
#define SCf_DELETE		18
#define SCf_INS		19
#define SCf_KEY_DESC		31
#define SCF_E0_PREFIX (1<<SCf_E0_PREFIX)
#define SCF_KEY_UP    (1<<SCf_KEY_UP)
#define SCF_SHIFT     (1<<SCf_SHIFT)
#define SCF_CTRL      (1<<SCf_CTRL)
#define SCF_ALT       (1<<SCf_ALT)
#define SCF_CAPS      (1<<SCf_CAPS)
#define SCF_NUM       (1<<SCf_NUM)
#define SCF_SCROLL    (1<<SCf_SCROLL)
#define SCF_NEW_KEY   (1<<SCf_NEW_KEY)
#define SCF_IP_L_DOWN (1<<SCf_IP_L_DOWN)
#define SCF_IP_R_DOWN (1<<SCf_IP_R_DOWN)
#define SCF_DELETE    (1<<SCf_DELETE)
#define SCF_INS    (1<<SCf_INS)
#define SCF_KEY_DESC (1<<SCf_KEY_DESC)

//LoseThos places a 1 in bit 7 for
//keys with an E0 prefix.
//See [::/LT/Doc/Char.TXZ] ::/LT/Doc/Char.TXZ
//and [C:/LT/OSMain/KbdMouse/Keyboard.CPZ,472] KbdHandler().
#define SC_ESC			0x01
#define SC_BACKSPACE		0x0E
#define SC_TAB			0x0F
#define SC_CARRIAGE_RETURN			0x1C
#define SC_SHIFT		0x2A
#define SC_CTRL			0x1D
#define SC_ALT			0x38
#define SC_CAPS			0x3A
#define SC_NUM			0x45
#define SC_SCROLL		0x46
#define SC_CURSOR_UP		0x48
#define SC_CURSOR_DOWN		0x50
#define SC_CURSOR_LEFT		0x4B
#define SC_CURSOR_RIGHT 	0x4D
#define SC_PAGE_UP		0x49
#define SC_PAGE_DOWN		0x51
#define SC_HOME			0x47
#define SC_END			0x4F
#define SC_INS		0x52
#define SC_DELETE		0x53
#define SC_F1			0x3B
#define SC_F2			0x3C
#define SC_F3			0x3D
#define SC_F4			0x3E
#define SC_F5			0x3F
#define SC_F6			0x40
#define SC_F7			0x41
#define SC_F8			0x42
#define SC_F9			0x43
#define SC_F10			0x44
#define SC_F11			0x57
#define SC_F12			0x58
#define SC_PAUSE		0x61
#define SC_GUI			0xDB
#define SC_PRTSCRN1		0xAA
#define SC_PRTSCRN2		0xB7

#define FONT_WIDTH		8
#define FONT_HEIGHT		8

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED			4
#define PURPLE			5
#define BROWN			6
#define LTGRAY			7
#define DKGRAY			8
#define LTBLUE			9
#define LTGREEN			10
#define LTCYAN			11
#define LTRED			12
#define LTPURPLE		13
#define YELLOW			14
#define WHITE			15
#define COLOR_INVALID		16
#define COLOR_TRANSPARENT	0xFF
#define COLOR_MONO		0xFF

//Low 8 bits reserved for flags that go into saved bitmaps
#define DCF_COMPRESSED		1
#define DCF_NO_TRANSPARENTS	2 //Can be used to optimized [C:/LT/Adam/Gr/GrBitMap.APZ,76] GrDCBlot().


#define DCF_TRANSFORMATION	0x100

//See [C:/LT/Adam/Gr/MathGr.APZ,374] GrSetSymmetry() or [C:/LT/Adam/Gr/MathGr.APZ,396] GrSetSymmetry3()
#define DCF_SYMMETRY		0x200

//Must be used with DCF_SYMMETRY set also.
//See [::/LT/Demo/GameStarters/BigGuns.CPZ] ::/LT/Demo/GameStarters/BigGuns.CPZ
#define DCF_JUST_MIRROR		0x400

#define DCF_LOCATE_NEAREST	0x800
#define DCF_DONT_DRAW		0x1000
#define DCF_ALIAS		0x2000
#define DCF_SCREEN_BITMAP	0x4000
#define DCF_FILL_NOT_COLOR	0x8000
#define DCF_RECORD_EXTENTS	0x10000
#define DCF_ON_TOP		0x20000

#define DCT_COLOR4	1
#define DCT_COLOR8	3

#define DCS_SIGNATURE_VAL			'DvCS'

public class GrSymStruct
{
  I32 sx,sy,sz,pad;
  //Normal of symmetry plane
  I64 snx,sny,snz;
};

//z-vals less than zero are in front of screen and not drawn.
//we want to shift all Z-vals into a drawable range.
//GR_Z_ALL is set to half of the Z-range which is an I32.
#define GR_Z_ALL	(MAX_I32/2)

public class GrDC
{
  U0 start_saved_area;
  I32 type,width,width_internal,height;
  U64 flags;
  I64 plane_size;
  U0 end_saved_area;

//public--change directly
  ColorROPU32 color,
	bkcolor, //Set for use with [C:/LT/OSMain/Adam1a.HPZ,2893] ROP_COLLISION
	color2; //Internally used for [C:/LT/Adam/Gr/GrPrimatives.APZ,236] GrFloodFill()
  P3I32 ls; //Light source (should be normalized to 65536).

  //dither_probability is basically a U16.
  //It is activated by [C:/LT/OSMain/Adam1a.HPZ,2903] ROPF_PROBABILITY_DITHER.
  //0x0000 =100% color.c0
  //0x8000 =50%  color.c0   50% color.c1
  //0x10000=100% color.c1
  U32 dither_probability, //See [::/LT/Demo/Graphics/Shading.CPZ] ::/LT/Demo/Graphics/Shading.CPZ.
     pad;

  GrDC *brush;

//Set with [C:/LT/Adam/Gr/MathGr.APZ,308] GrSetRotMat().  [C:/LT/OSMain/Memory.CPZ,950] Free() before setting.
  I64 *r;  //rotation matrix of quads decimal in lo
  I64 r_norm; //shifted 32 bits.  Used for scaling pen width

//public--change directly
  I32 x,y,z,pen_width;	//translation

//Can be changed from the default [C:/LT/Adam/Gr/MathGr.APZ,154] GrTransform()
  U0 (*transform)(GrDC *dc,I64 *x,I64 *y,I64 *z);
//Can be changed from the default [C:/LT/Adam/Gr/MathGr.APZ,460] GrLighting()
  U0 (*lighting)(GrDC *dc,P3I32 *p1,P3I32 *p2,P3I32 *p3,ColorROPU32 color);

//Set by [C:/LT/Adam/Gr/MathGr.APZ,374] GrSetSymmetry() or [C:/LT/Adam/Gr/MathGr.APZ,396] GrSetSymmetry3()
  GrSymStruct sym;

//not document num, but num in a GrSprite collection
  I64 nearest_sprite_elem_num;

  U64 nearest_dist;

//not document num, but num in a GrSprite collection
  I64 cur_sprite_elem_num;

  I32 cur_x,cur_y,cur_z,pad;
  F64 speedline_scale;
  U64 collision_cnt;

  //Set by [C:/LT/OSMain/Adam1a.HPZ,3476] DCF_RECORD_EXTENTS
  I64 min_x,max_x,min_y,max_y; //screen coordinates

  U32 dc_signature,pad;
  TaskStruct *mem_task,*win_task;
  GrDC *alias;
  U8 *body;

  //Set by [C:/LT/Adam/Gr/GrDC.APZ,104] GrDCAllocDepthBuf()
  I32 *depth_buf;
  I64 db_z; //private
};

public class GrVectMap
{
  U0 start_saved_area;
  I32 width,height;
  U8 r_bits,poly_line_cnt_bits,pad,pad;
  U32 rect_cnt;
  U0 end_saved_area;
 
  ColorROPU32 bkcolor,pad;

//Array of bit numbers for rectangle starts
//Can be regenerated with [C:/LT/Adam/Gr/GrVectMap.APZ,470] GrVectMapRectStartBits()
  U32 *rect_start_bits;

  U8 *body;
};

class RaxRbxRcxRdx
{
  U64 rax,rbx,rcx,rdx;
};

#define VGA_GRAPHIC_MEM_BASE	0xA0000
#define VGA_TEXT_MEM_BASE	0xB8000

#define VGA_ATTR_INDEX		0x03C0
#define VGA_ATTR_DATA_WRITE	0x03C0 //alias
#define VGA_ATTR_DATA_READ	0x03C1
#define VGA_MISC_OUTPUT 	0x03C2
#define VGA_SC_INDEX		0x03C4
#define VGA_SC_DATA		0x03C5
#define VGA_PALETTE_MASK	0x03C6
#define VGA_REG_READ		0x03C7
#define VGA_REG_WRITE		0x03C8
#define VGA_PALETTE_DATA	0x03C9
#define VGA_CRTC_INDEX		0x03D4
#define VGA_CRTC_DATA		0x03D5
#define VGA_INPUT_STAT		0x03DA
#define VGA_MAP_MASK		0x02

#define VGA_H_TOTAL		0x00
#define VGA_H_BLANK_START	0x02
#define VGA_H_RETRACE_START	0x04
#define VGA_V_TOTAL		0x06
#define VGA_MAX_SCAN_LINE	0x09
#define VGA_V_RETRACE_END	0x11
#define VGA_OFFSET		0x13
#define VGA_V_BLANK_START	0x15
#define VGA_MODE_CTRL		0x17

#define ARC_MAX_BITS 12
#define ARC_MAX_TABLE_ENTRY (1<<ARC_MAX_BITS-1)

#define CT_NONE 	0
#define CT_7_BIT	1
#define CT_8_BIT	2
class ArcTableEntry
{
  ArcTableEntry *next;
  U16 basecode;
  U8 ch,pad;
  U32 pad;
};
public class ArcCs //ctrl structure
{
  U64 src_pos;
  U64 src_size;
  U64 dst_pos;
  U64 dst_size;
  U8 *src_buf;
  U8 *dst_buf;
  U64 min_bits;
  U64 min_table_entry;
  ArcTableEntry *cur_entry;
  ArcTableEntry *next_entry;
  U64 cur_bits_in_use;
  U64 next_bits_in_use;
  U8 *stk_ptr;
  U8 *stk_base;
  U64 free_index;
  U64 free_limit;
  U64 saved_basecode;
  U64 entry_used;
  U64 last_ch;
  ArcTableEntry compress[ARC_MAX_TABLE_ENTRY+1];
  ArcTableEntry *hash[ARC_MAX_TABLE_ENTRY+1];
};

public class ArcCompressStruct
{
  U64 compressed_size,
       expanded_size;
  U16 compression_type,flags;
  U0 body;
};

// Flags for StrUtil and MStrUtil
#define SUF_REM_SPACES		0x001
#define SUF_REM_CTRL_CHARS	0x002
#define SUF_REM_LEADING		0x004
#define SUF_SINGLE_SPACE	0x008
#define SUF_TO_UPPER		0x010
#define SUF_TO_LOWER		0x020
#define SUF_REM_TRAILING	0x040
#define SUF_CAP_UNDERSCORES	0x080
#define SUF_S2T			0x100
#define SUF_T2S			0x200 // Only works with MStrUtil
#define SUF_SAFE_DOLLAR		0x400
#define SUF_SCALE_INDENT	0x800

// Flags for SearchStr
#define SRF_IGNORE_CASE		1
#define SRF_WHOLE_LABELS	2

//Flags for ListEntryMatch
#define LEF_IGNORE_CASE		1
#define LEF_EXACT		2

#define ISO1_ATTR_DIR		2

#define LT_ATTR_READ_ONLY	0x01
#define LT_ATTR_HIDDEN		0x02
#define LT_ATTR_SYSTEM		0x04
#define LT_ATTR_VOL_ID		0x08
#define LT_ATTR_DIR		0x10
#define LT_ATTR_ARCHIVE		0x20
#define LT_ATTR_LONG_NAME	(LT_ATTR_READ_ONLY|LT_ATTR_HIDDEN|LT_ATTR_SYSTEM|LT_ATTR_VOL_ID)
#define LT_ATTR_LONG_NAME_MASK	(LT_ATTR_LONG_NAME|LT_ATTR_DIR|LT_ATTR_ARCHIVE)
#define LT_ATTR_DELETED		0x100
#define LT_ATTR_ENCRYPTED	0x200 // not implemented
#define LT_ATTR_RESIDENT	0x400
#define LT_ATTR_COMPRESSED	0x800
#define LT_ATTR_CONTIGUOUS	0x1000
#define LT_ATTR_FIXED		0x2000

public class LTFileAccess
{
  LTPrt *old_prt,*p;
  U8 *old_dir,*mask;
};

#define LTFB_NEXT_BLK		0xFFFFFFFF

#define LTF_WRITE		1
#define LTF_NEW_FILE		2
#define LTF_BUF_DIRTY		4
#define LTF_NEEDS_WRITE		8
#define LTF_CONTIGUOUS		16
#define LTF_USE_OLD_DATETIME	32

public class LTFile
{
  U64 flags;
  LTDirEntry de;
  LTPrt *p;
  I64 fblk_num,cluster,file_cluster_num;
  I64 max_blk;
  U8 *cluster_buf;
};

//File util flags
//See [] ST_FILE_UTIL_FLAGS
#define FUf_RECURSE		0
#define FUf_DIFF		1
#define FUf_IGNORE		2
#define FUf_WHOLE_LABELS	3
#define FUf_QUESTION		4
#define FUf_ALL			5
#define FUf_CANCEL		6
#define FUf_REPLACE		7
#define FUf_PUBLIC		8
#define FUf_MAP			9
#define FUf_JUST_DIRS		10
#define FUf_JUST_FILES		11
#define FUf_JUST_TEXT_FILES	12
#define FUf_JUST_SRC_FILES	13
#define FUf_FLATTEN_TREE	14

#define FUF_RECURSE		0x0001
#define FUF_DIFF		0x0002
#define FUF_IGNORE		0x0004
#define FUF_WHOLE_LABELS	0x0008
#define FUF_QUESTION		0x0010
#define FUF_ALL			0x0020
#define FUF_CANCEL		0x0040
#define FUF_REPLACE		0x0080
#define FUF_PUBLIC		0x0100
#define FUF_MAP			0x0200
#define FUF_JUST_DIRS		0x0400
#define FUF_JUST_FILES		0x0800
#define FUF_JUST_TEXT_FILES	0x1000
#define FUF_JUST_SRC_FILES	0x2000
#define FUF_FLATTEN_TREE	0x4000

#define CPU_WALLPAPER_DATA_SIZE		128
#define CPUf_HAD_IRQ_OPPORTUNITY	0
#define CPUf_SETH_SLEEP			1

public class CPUStruct
{
  CPUStruct   *addr;
  U64 num,cpu_flags;
  U64 startup_rip;
  U64 idle_pt_hits;
  F64 idle_factor;
  U64 total_jiffies;
  TaskStruct *seth_task,*idle_task;
  MPCmdCtrl cmd_ctrl;
  U64 *idt; //interrupt descriptor table
  U64 tr;   //task register
  U64 swap_cnter;
  U0 (*timer_irq_chain)(TaskStruct *task);
  TaskStruct *dying_task_list;
  TssStruct *tss;
  U8 wallpaper_data[CPU_WALLPAPER_DATA_SIZE]; //doubles as start-up stk
  U64 start_stk[8];
};

#define KDF_HAS_DESCS	1

class KeyDevStruct
{
  KeyDevStruct *next,*last;
  U64 priority,flags;
  BoolI64 (*put_key)(U64 ch,U64 sc);
  BoolI64 (*put_s)(U8 *st);
};

#define CREG_RAX	0
#define CREG_RCX	1
#define CREG_RDX	2
#define CREG_RBX	3
#define CREG_RSP	4
#define CREG_RBP	5
#define CREG_RSI	6
#define CREG_RDI	7
#define CREG_RIP	16	//Used by compiler, not really it's num
//Be careful: RBPu8, RSPu8, RSIu8, RDIu8 are 20-24
#define CREG_NONE	32	//noreg flag sets it to this
#define CREG_ALLOC	33	//reg flag sets it to this
#define CREG_UNDEF	MIN_I8

#define CALWAYS_CLOBBERED_MASK		0x003F /* RAX,RCX,RDX,RBX,RBP,RSP */
#define CSTK_TEMPS_MASK			0x0300 /* R8,R9 */
#define CREG_VARS_MASK			0xCCC0 /* RSI,RDI,R10,R11,R14,R15 */
#define CREG_NON_PTR_MASK		0x3000 /* R12,R13 */

#define PUSH_C_REGS PUSH RAX PUSH RCX PUSH RDX PUSH RBX PUSH R8 PUSH R9
#define POP_C_REGS POP R9 POP R8 POP RBX POP RDX POP RCX POP RAX

#define PUSH_REGS PUSH RAX PUSH RCX PUSH RDX PUSH RBX PUSH RBP PUSH RSI \
PUSH RDI PUSH R8 PUSH R9 PUSH R10 PUSH R11 PUSH R12 PUSH R13 PUSH R14 PUSH R15
#define POP_REGS POP R15 POP R14 POP R13 POP R12 POP R11 POP R10 POP R9 \
POP R8 POP RDI POP RSI POP RBP POP RBX POP RDX POP RCX POP RAX

#define MAPPED_MEM_SPACE 0x2000000000 /*Arbitrarily set to 128 Gig*/
#define PAGE_TABLE_BASE	0x100000
#define NUM_PML1	1 /*First 2 Meg are 4K resolution*/
#define NUM_PML2	(MAPPED_MEM_SPACE>>(9+21))
#define NUM_PML3	1
#define NUM_PML4	1
#define PAGE_TABLE_SIZE (NUM_PML1+NUM_PML2+NUM_PML3+NUM_PML4)*0x1000
#define PROTECTED_LOW_PAGE_LIMIT	0x6FFF

#define ST_ERR_ST   "$FG,LTRED$$BK,1$ERROR:$FG$$BK,0$ "
#define ST_WARN_ST  "$FG,RED$$BK,1$WARNING:$FG$$BK,0$ "

//snd devs
#define SD_PC_SPEAKER		0
#define SD_HD_AUDIO		1

#define SND_SAMPLE_RATE		48000
#define SND_SAMPLE_BITS		24
#define SND_OCHANNELS		2
#define SND_ICHANNELS		2
#define SND_OUT_CONTAINER	I32
#define SND_IN_CONTAINER	I16
#define SND_BUF_LEN		0x400
#define SND_BUF_TIME_mS		(SND_BUF_LEN/SND_OCHANNELS*1000.0/SND_SAMPLE_RATE)
//buffer byte size is SND_BUF_LEN*sizeof(I32).
//It's 24 bit packed in 32 bit containers
//There are 2 channels for stereo, so half as many blocks.

#define WF_NULL			0
#define WF_SQUARE		1
#define WF_SINE			2
#define WF_TRIANGLE		3
#define WF_SAWTOOTH		4
#define WF_NOISE		5

#define WF_NUM_WAVEFORMS	6

//sys_raw_dc_flags flags
#define RDCF_IN_DOLLAR		1
#define RDCF_LAST_DOLLAR	2
#define RDCF_SHOW_DOLLAR	4
#define RDCF_SCROLL		8

//Input ptr types
#define IP_NULL		0
#define IP_MOUSE	1
#define IP_JOYSTICK0	2 //Not supported
#define IP_JOYSTICK1	3 //Not supported

#define USBP_CMD	0x00
#define USBP_STS	0x02
#define USBP_INTR	0x04
#define USBP_FRNUM	0x06
#define USBP_FRBASEADD	0x08
#define USBP_SOFMOD	0x0C
#define USBP_PORTSC0	0x10
#define USBP_PORTSC1	0x12
#define MAX_USB	16

class USBTD
{
  U32 td[4];
};

class LTUSB
{
  U8 num, //LT USB dev num
     bus,dev,fun;
  U16 ports,pad;
  U32 *frame_list;
};

//USB Pkt ID's
#define PID_OUT		0xE1
#define PID_IN		0x69
#define PID_SOF		0xA5
#define PID_SETUP	0x2D
#define PID_DATA0	0xC3
#define PID_DATA1	0x4B
#define PID_DATA2	0x87
#define PID_MDATA	0x0F
#define PID_ACK		0xD2
#define PID_NAK		0x5A
#define PID_STALL	0x1E
#define PID_NYET	0x96
#define PID_PRE		0x3C
#define PID_ERR		0x3C
#define PID_SPLIT	0x78
#define PID_PING	0xB4

//USB Std Rqsts
#define RQ_GET_STAT	0x0
#define RQ_CLR_FEAT	0x1
#define RQ_SET_FEAT	0x3
#define RQ_SET_ADDR	0x5
#define RQ_SET_DESC	0x7
#define RQ_GET_CFG	0x8
#define RQ_SET_CFG	0x9
#define RQ_GET_INTERFACE 0xA
#define RQ_SET_INTERFACE 0xB
#define RQ_SYNC_FRAME	0xC

#define PSECT_CACHE_SIZE	256
#define PSECT_CACHE_STR_LEN	(64-24)
class PSectCacheEntry
{
  I64 base,limit;
  U64 timestamp;
  U8 str[PSECT_CACHE_STR_LEN];
};
