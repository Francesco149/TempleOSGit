#define BOOT_ADDRESS 0x030000
#define HIGH_ADDRESS 0x100000

//SYSTEM_FLAGS
#define SYSF_PREEMPTIVE 	    0
#define SYSF_CTRL_ALT_DEL           1
//unused
//unused
#define SYSF_IDE1_BUSY		    4
#define SYSF_IDE1_DONE		    5
//unused
#define SYSF_RAW_CRT_OUT	    7
#define SYSF_FDC0_BUSY		    8
#define SYSF_FDC0_DONE		    9
//VGA can be disabled by setting this flag and RAW_CRT_OUT
//in SYS_FLAGS prior to boot.
#define SYSF_NON_VGA		    10
#define SYSF_CTRL_ALT_ESC           11
#define SYSF_CTRL_ALT_X             12	
#define SYSF_CTRL_ALT_TAB	    13
#define SYSF_TASK_LINKS_ALTERED     14

//SYS_CURRENT_SCAN_CODE FLAGS
#define SCF_KEY_UP		     7
#define SCF_E0_PREFIX		     8
#define SCF_SHIFT		     9
#define SCF_CTRL		     10
#define SCF_ALT 		     11
#define SCF_CAPS		     12
#define SCF_NUM 		     13
#define SCF_NEW_KEY		     16
#define SCF_PRIOR_E0		     17

#define CH_BACKSPACE		     0x08
#define SC_CURSOR_UP		     0x48
#define SC_CURSOR_DOWN		     0x50
#define SC_CURSOR_LEFT		     0x4B
#define SC_CURSOR_RIGHT 	     0x4D
#define SC_PAGE_UP		     0x49
#define SC_PAGE_DOWN		     0x51
#define SC_INSERT		     0x52
#define SC_DELETE		     0x53
#define SC_F1			     0x3B
#define SC_F2			     0x3C
#define SC_F3			     0x3D
#define SC_F4			     0x3E
#define SC_F5			     0x3F
#define SC_F6			     0x40
#define SC_F7			     0x41
#define SC_F8			     0x42
#define SC_F9			     0x43
#define SC_F10			     0x44
#define SC_F11			     0x57
#define SC_F12			     0x58
#define SC_PRT			     0x137
#define SC_SCROLL		     0x46
#define SC_HOME 		     0x47
#define SC_END			     0x4F
#define SC_L_GUI		     0x15B
#define SC_R_GUI		     0x15C


#define MAXGDT			    1024
#define MAXIDT			    96
#define SYS_DEFAULT_FLAGS	    0x40200
#define SYS_DEFAULT_CR3 	    0

		STRUC 0
SF_EBP: 	 DD	 ?;
SF_EIP: 	 DD	 ?;
SF_PARAM1:	 DD	 ?;
SF_PARAM2:	 DD	 ?;
SF_PARAM3:	 DD	 ?;
SF_PARAM4:	 DD	 ?;
SF_PARAM5:	 DD	 ?;
SF_PARAM6:	 DD	 ?;
SF_PARAM7:	 DD	 ?;
SF_PARAM8:	 DD	 ?;
		ENDS

		STRUC 0
NULL_SEL:		  DD	  ?,?;
SYS_BOOT_DS_SEL:	 DD	 ?,?;
SYS_BOOT_CS_SEL:	 DD	 ?,?;
SYS_DS_SEL:		 DD	 ?,?;
SYS_CS_SEL:		 DD	 ?,?;
ZERO_DS_SEL:		 DD	 ?,?;
CRT_SEL:		 DD	 ?,?;
//ROOT_TSS_SEL:		 DD	 ?,?;
FIRST_FREE_SEL:
	ENDS

//MEMORY RELATED
#define PAGE_SIZE	   0x1000
#define PAGE_BITS	   12
#define DEFAULT_STACK      (PAGE_SIZE*16)

	STRUC 0
MB_NEXT:	 DD	 ?;
MB_SIZE:	 DD	 ?;
MB_STRUCT_SIZE:
	ENDS

	STRUC 0 	 //AVAILABLE TO MALLOC MEMORY
UUM_NEXT:	 DD	 ?;
UUM_SIZE:	 DD	 ?;
UUM_STRUCT_SIZE:
	ENDS

#define USED_USER_MEMORY_SIZE  -4


	STRUC 0
ST_NULL:		 DB	 ?;
ST_ROOT_STARTUP:	 DB	 ?;
ST_USER_STARTUP:	 DB	 ?;
ST_UNUSED:		 DB	 ?;
ST_INTERRUPT:		 DB	 ?;
ST_TRAP:		 DB	 ?;
ST_INVALID_TASK_ADDRESS: DB	 ?;
ST_UNDEFINED_EXTERN:	 DB	 ?;
ST_UNDEFINED_SYSTEXT:	 DB	 ?;
ST_INVALID_EXTERN_NUMBER: DB	?;
ST_INVALID_SYSTEXT_NUMBER: DB	 ?;
ST_COMMAND_NOT_FOUND:	 DB	 ?;
ST_FILE_NOT_FOUND:	 DB	 ?;
			ENDS

	    STRUC 0
			DB	256 DUP(?);
ST_PARSE_ERROR: 	 DB	 ?;
ST_MISSING_SEMICOLON:	 DB	 ?;
ST_MISSING_EXPRESSION:	 DB	 ?;
ST_MISSING_RIGHT_PAREN:  DB	 ?;
ST_MISSING_RIGHT_BRACKET: DB	 ?;
ST_UNDEFINED_SYMBOL:	 DB	 ?;
ST_INVALID_CLASS:	 DB	 ?;
ST_INVALID_MEMBER:	 DB	 ?;
ST_EXPECTING_TYPE:	 DB	 ?;
ST_EXPECTING_IDENTIFIER: DB	 ?;
ST_EXPECTING_LEFT_PAREN: DB	 ?;
ST_EXPECTING_LEFT_BRACE: DB	 ?;
ST_MISSING_RIGHT_BRACE:  DB	 ?;
ST_INVALID_LVALUE:	 DB	 ?;
ST_EXPECTING_EQUAL:	 DB	 ?;
ST_NOT_POINTER_TYPE:	 DB	 ?;
ST_EXPECTING_COMMA:	 DB	 ?;
ST_EXPECTING_SYS_SYMBOL: DB	 ?;
ST_EXPECTING_STRING:	 DB	 ?;
			ENDS
#define ST_TABLE_SIZE	4096

#define SCREEN_COLS	100
#define SCREEN_LINES	75
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define FONT_HEIGHT	8
#define FONT_WIDTH	8


#define TSSCF_WAKE	0
	STRUC	0
TSS_CMD_NEXT:		DD	?;
TSS_CMD_LAST:		DD	?;
TSS_CMD_FLAGS:		DD	?;
TSS_CMD_STRING:		DD	?;
TSS_CMD_RESULT:		DD	?;
TSS_CMD_MASTER_TSS:	DD	?;
TSS_CMD_STRUCT_SIZE:	DD	?;
	ENDS

#define MAXIO		    0x800
#define HEAP_CACHE_SIZE     512
#define TSS_DESC_LENGTH     31

	     STRUC 0
TSS_BACK_LINK:		 DD	 ?;
TSS_ESP0:		 DD	 ?;
TSS_SS0:		 DD	 ?;
TSS_ESP1:		 DD	 ?;
TSS_SS1:		 DD	 ?;
TSS_ESP2:		 DD	 ?;
TSS_SS2:		 DD	 ?;
TSS_CR3:		 DD	 ?;
TSS_EIP:		 DD	 ?;
TSS_EFLAGS:		 DD	 ?;
TSS_EAX:		 DD	 ?;
TSS_ECX:		 DD	 ?;
TSS_EDX:		 DD	 ?;
TSS_EBX:		 DD	 ?;
TSS_ESP:		 DD	 ?;
TSS_EBP:		 DD	 ?;
TSS_ESI:		 DD	 ?;
TSS_EDI:		 DD	 ?;
TSS_ES: 		 DD	 ?;
TSS_CS: 		 DD	 ?;
TSS_SS: 		 DD	 ?;
TSS_DS: 		 DD	 ?;
TSS_FS: 		 DD	 ?;
TSS_GS: 		 DD	 ?;
TSS_LDTR:		 DD	 ?;
TSS_TASK_SWITCH_TRAP:	 DW	 ?;
TSS_IO_BITMAP_PTR:	 DW	 ?;
TSS_IO_BITMAP:		 DB	 (MAXIO/8) DUP (?);

TSS_TASK_DESCRIPTOR:	 DB	 (TSS_DESC_LENGTH+1) DUP (?);

TSS_UPDATE_WINDOW:	 DD	 ?;
TSS_CURRENT_LTF:	 DD	 ?;
TSS_CURSOR_OFFSET:	 DD	 ?;
TSS_CRT_SIZE:		 DD	 ?;
TSS_CRT_WIDTH:		 DD	 ?;
TSS_CRT_HEIGHT: 	 DD	 ?;
TSS_CRT_SELECT: 	 DW	 ?;
TSS_TEXT_ATTRIBUTE:	 DB	 ?;
TSS_BORDER_ATTRIBUTE:	 DB	 ?;
TSS_WINDOW_LEFT:	 DD	 ?;
TSS_WINDOW_RIGHT:	 DD	 ?;
TSS_WINDOW_TOP: 	 DD	 ?;
TSS_WINDOW_BOTTOM:	 DD	 ?;
TSS_CURSOR_HOR: 	 DD	 ?;
TSS_CURSOR_VERT:	 DD	 ?;

TSS_WINDOW_PIXEL_LEFT:	 DD	 ?; //These are derived
TSS_WINDOW_PIXEL_RIGHT:  DD	 ?;
TSS_WINDOW_PIXEL_TOP:	 DD	 ?;
TSS_WINDOW_PIXEL_BOTTOM: DD	 ?;

#define CRTF_CURSOR_ON		    0
#define CRTF_WRAP_AROUND	    1
#define CRTF_SHOW		    2
#define CRTF_HAS_MENU		    3

TSS_CRT_FLAGS:		 DW	 ?;

#define TSSF_MALLOC_TRACE	    0
#define TSSF_LEX_TRACE		    1
#define TSSF_PARSE_TRACE	    2
#define TSSF_COMPILE_TRACE	    3
#define TSSF_RUN_TRACE		    4
#define TSSF_ECHO		    5
#define TSSF_SUSPENDED		    6
#define TSSF_LOCAL_USER 	    7
#define TSSF_OPT_TRACE		    8
#define TSSF_PREEMPT		    9
#define TSSF_FOCUS		    10
#define TSSF_KILL_TASK		    11

TSS_TASK_FLAGS: 	 DW	 ?;

//TSS_SELECTOR:		 DD	 ?;	 //SELECTOR OF THIS TSS
TSS_ABSOLUTE_ADDRESS:	 DD	 ?;	 //ABSOLUTE ADDRESS OF THIS TSS
TSS_PARENT_TSS: 	 DD	 ?;
TSS_NEXT_TSS:		 DD	 ?;	  //ABSOLUTE ADDRESS OF NEXT TSS
TSS_LAST_TSS:		 DD	 ?;	  //ABSOLUTE ADDRESS OF LAST TSS
TSS_MEMORY_BLOCK_LIST:	 DD	 ?;
TSS_HEAP_CACHE: 	 DB	 HEAP_CACHE_SIZE DUP (?);
TSS_MALLOC_FREE_LIST:	 DD	 ?;
TSS_TRAP_LIST:		 DD	 ?;
TSS_HASH_TABLE: 	 DD	 ?;
TSS_STRING_TABLE:	 DD	 ?;
TSS_OPTIMIZATION:	 DD	 ?;
TSS_NEXT_SLAVE_CMD:	 DD	 ?;
TSS_LAST_SLAVE_CMD:	 DD	 ?;
TSS_NEXT_MASTER_CMD:	 DD	 ?;
TSS_LAST_MASTER_CMD:	 DD	 ?;
TSS_CURRENT_PARTITION:	 DD	 ?;
TSS_CURRENT_DIRECTORY:	 DD	 ?;
TSS_CURRENT_DIR_CLUSTER: DD	 ?;

TSS_USER_AUX0:		 DD	 ?;
TSS_USER_AUX1:		 DD	 ?;
TSS_USER_AUX2:		 DD	 ?;
TSS_USER_AUX3:		 DD	 ?;
TSS_STRUCT_SIZE:
	ENDS


#define LTF_FILENAME_LEN 256
	 STRUC 0
LTF_NEXT:		 DD ?;
LTF_LAST:		 DD ?;
LTF_FLAGS:		 DD ?;
LTF_LINE_START_COL:	 DD ?;
LTF_LINE_END_COL:	 DD ?;
LTF_TO_TEXT_CB: 	 DD ?;
LTF_CUR_DATA_COL_CB:	 DD ?;
LTF_CUR_COL_CB:		 DD ?;
LTF_CUR_COL:		 DD ?;
LTF_CUR_DATA_COL:	 DD ?;
LTF_CUR_LINE:		 DD ?;
LTF_CUR_TOP_LINE_NUM:	 DD ?;
LTF_INSERT_POINT:	 DD ?;
LTF_CUR_TOP_LINE:	 DD ?;
LTF_NUM_LINES:		 DD ?;
LTF_MAX_LINES:		 DD ?;
LTF_FIND_REPLACE:	 DD ?;
LTF_BASE_SELECT:	 DD ?;
LTF_LIMIT_SELECT:	 DD ?;
LTF_MENU_LTF:		 DD ?;
LTF_TEXT_ATTRIBUTE:	 DB ?;
LTF_RESERVED1:		 DB ?,?,?;
LTF_FILENAME:		 DB LTF_FILENAME_LEN DUP(?);
LTF_STRUCT_SIZE:	 DD ?;
	ENDS

#define LTFLF_SELECTED		    0
	STRUC 0
LTFL_NEXT:		 DD ?;
LTFL_LAST:		 DD ?;
LTFL_FLAGS:		 DD ?;
LTFL_LINE_DATA: 	 DD ?;
LTFL_STRUCT_SIZE:
	ENDS

//LEX.ASM
#define TK_EOF			    0
#define TK_IDENT		    1
#define TK_STRING		    2
#define TK_INTEGER		    3
#define TK_DOUBLE		    4
#define TK_LESS_EQUAL		    5
#define TK_EQUAL_EQUAL		    6
#define TK_GREATER_EQUAL	    7
#define TK_AND_AND		    8
#define TK_OR_OR		    9
#define TK_ROTATE_LEFT		    0xB
#define TK_ROTATE_RIGHT 	    0xC
#define TK_CR			    0xD
#define TK_DEREFERENCE		    0xE
#define TK_NOT_EQUAL		    0xF
#define TK_PLUS_PLUS		    0x10
#define TK_MINUS_MINUS		    0x11
#define TK_DOUBLE_COLON 	    0x12

#define TK_DOUBLE_QUOTE 	    0x22
#define TK_SINGLE_QUOTE 	    0x27
#define TK_LEFT_BRACE		    0x7B
#define TK_RIGHT_BRACE		    0x7D

#define LF_EXPECTING_HEX	    1
#define LF_PROMPT		    2
#define LF_DONT_FREE_BUFFER	    4
#define LF_DOUBLE_SEMICOLON	    8
#define LF_NO_DEFINES		    16

#define LF_ASM_EXPRESSIONS	    0x00200000
#define LF_UNRESOLVED_LOCAL	    0x00400000
#define LF_UNRESOLVED_GLOBAL	    0x00800000
#define LF_FUNCTION		    0x01000000
#define LF_POSTINC		    0x02000000
#define LF_POSTDEC		    0x04000000
#define LF_PREINC		    0x08000000
#define LF_PREDEC		    0x10000000
#define LF_ARRAY		    0x20000000
#define LF_EAX			    0x40000000
#define LF_USE_LAST_CHAR	    0x80000000

#define LEX_MAX_IDENT		    134


	STRUC 0
LFN_BUFFER:		 DD	 ?;
LFN_BUF_PTR:		 DD	 ?;
LFN_LINE_NUM:		 DD	 ?;
LFN_NAME:		 DD	 ?;
LFN_CUR_LINE_START:	 DD	 ?;
LFN_STRUCT_SIZE:
	ENDS

#define LFN_COUNT		    8
#define LFN_STACK_COUNT 	    8

	STRUC 0
LEX_TOKEN:		 DD	 ?;
LEX_FLAGS:		 DD	 ?;
LEX_CUR_I:		 DD	 ?;
LEX_CUR_D:		 DD	 ?,?,?;
LEX_IDENT:		 DD	 ?;
LEX_IDENT_LEN:		 DD	 ?;
LEX_LAST_CHAR:		 DD	 ?;
LEX_INSTRUCTION_POINTER: DD	 ?; //For asm
LEX_DEFINE_HASH_TABLE:	 DD	 ?;
LEX_LOCAL_HASH_TABLE:	 DD	 ?;
LEX_GLOBAL_HASH_TABLE:	 DD	 ?;
LEX_HASH_TABLE_LIST:	 DD	 ?;
LEX_HASH_ENTRY: 	 DD	 ?;
LEX_UNDEF_HASH_ENTRY:	 DD	 ?;
LEX_LOCAL_VAR_LIST:	 DD	 ?;
LEX_LOCAL_VAR_ENTRY:	 DD	 ?;
LEX_CUR_BUF_PTR:	 DD	 ?;
LEX_CUR_LFN:		 DD	 ?;
LEX_STACK_PTR:		 DD	 ?;

LEX_CB_PUSH_START:
LEX_CB_OUT_FIRST:	 DD	 ?;
LEX_CB_OUT_LAST:	 DD	 ?;
LEX_CB_OUT_END: 	 DD	 ?;
LEX_CB_OUT_PTR: 	 DD	 ?;
LEX_CB_LAST_OUT_PTR:	 DD	 ?;
LEX_CB_IN_FIRST:	 DD	 ?;
LEX_CB_IN_END:		 DD	 ?;
LEX_CB_IN_PTR:		 DD	 ?;
LEX_CB_LABEL_LIST:	 DD	 ?;
LEX_CB_STACK:		 DD	 ?;
LEX_CB_PUSH_END:

LEX_LFNS:		 DB	 LFN_COUNT*LFN_STRUCT_SIZE DUP(?);
LEX_STACK:		 DB	 LFN_STACK_COUNT*LFN_STRUCT_SIZE DUP(?);
LEX_STRUCT_SIZE:
		ENDS

	STRUC 0
HT_NEXT:		 DD	 ?;
HT_MASK:		 DD	 ?;
HT_BODY:
	ENDS

	STRUC 0
HTE_NEXT:		 DD	 ?;
HTE_STRING:		 DD	 ?;
HTE_TYPE:		 DD	 ?;
HTE_USER0:		 DD	 ?;
HTE_USER1:		 DD	 ?;	 //Must be in same position as C_SIZE
HTE_USER2:		 DD	 ?;	 //Must be in same position as C_DEREFERENCE
HTE_STRUCT_SIZE:
	ENDS

//HASH TABLE ENTRY TYPES
#define HTT_INVALID		    0
#define HTT_ALL 		    0xFFFFFFFF
#define HTT_STRING_CONSTANT	    1
#define HTT_SYS_SYMBOL		    2
#define HTT_KEYWORD		    4
#define HTT_LOCAL_VARIABLE	    8
#define HTT_GLOBAL_VARIABLE	    16
#define HTT_CLASS		    32
#define HTT_INTERNAL_TYPE	    64
#define HTT_FUNCTION		    128
#define HTT_FILE		    256

#define HTT_UNRESOLVED_LOCAL	    0x40000000
#define HTT_UNRESOLVED_GLOBAL	    0x80000000

	STRUC 0
CT_HASH_NEXT:	 DD	 ?;
CT_HASH_STRING:  DD	 ?;
CT_HASH_TYPE:	 DD	 ?;
CT_VALUE:	 DD	 ?;
CT_STRUCT_SIZE:
	ENDS

//SOFTWARE INTERRUPTS
#define I_PUT_SYSTEXT		  0x30
#define I_PUT_CHAR		  0x31
#define I_GET_CHAR		  0x32
#define I_PRINTF		  0x33

	STRUC 0
SP_EDI: DD	?;
SP_ESI: DD	?;
SP_EBP: DD	?;
SP_ESP: DD	?;
SP_EBX: DD	?;
SP_EDX: DD	?;
SP_ECX: DD	?;
SP_EAX: DD	?;
SP_STRUCT_SIZE:
SP_EIP: 	 DD	 ?;
SP_PARAM1:	 DD	 ?;
SP_PARAM2:	 DD	 ?;
SP_PARAM3:	 DD	 ?;
SP_PARAM4:	 DD	 ?;
SP_PARAM5:	 DD	 ?;
SP_PARAM6:	 DD	 ?;
SP_PARAM7:	 DD	 ?;
SP_PARAM8:	 DD	 ?;
	ENDS

#define VGA_MISC_OUTPUT 	0x03c2
#define VGA_SC_INDEX		0x03c4
#define VGA_SC_DATA		0x03c5
#define VGA_PALETTE_INDEX	0x03c8
#define VGA_PALETTE_DATA	0x03c9
#define VGA_CRTC_INDEX		0x03d4
#define VGA_MAP_MASK		0x02
#define VGA_H_TOTAL		0x00
#define VGA_H_BLANK_START	0x02
#define VGA_H_RETRACE_START	0x04
#define VGA_V_TOTAL		0x06
#define VGA_MAX_SCAN_LINE	0x09
#define VGA_V_RETRACE_END	0x11
#define VGA_OFFSET		0x13
#define VGA_V_BLANK_START	0x15
#define VGA_MODE_CONTROL	0x17

	STRUC 0
EXT_NULL:		 DB  ?;
EXT_RBLOCKS:		 DB  ?;
EXT_WBLOCKS:		 DB  ?;
EXT_IRQ0E:		 DB  ?;
EXT_DUMP:		 DB  ?;
EXT_READ_FILE:		 DB  ?;
EXT_LTF_PUTCHAR:	 DB  ?;
EXT_BLANK_CRT_TEXT:	 DB  ?;
EXT_IRQ03:		 DB  ?;
EXT_IRQ04:		 DB  ?;
EXT_SELECT_DEFAULT_DRIVE: DB ?;
EXT_IRQ06:		 DB  ?;
EXT_WINDOW_TO_TOP:	DB   ?;
EXT_IRQ0C:		DB   ?;
	ENDS
#define EXT_TABLE_SIZE		    1024

#define eie_rel_export 0
#define eie_lit_export 1
#define eie_rel_BYTE   2
#define eie_lit_BYTE   3
#define eie_rel_WORD   4
#define eie_lit_WORD   5
#define eie_rel_DWORD  6
#define eie_lit_DWORD  7
#define eie_end        0xFF
	END
