        P486
        LOCALS

;SYSTEM_FLAGS
SYSF_PREEMPTIVE         EQU     0       ;1=PREEMPTIVE 0=NONPREEMPTIVE
SYSF_MEMORY_IN_USE      EQU     1
SYSF_ROOT_BUSY          EQU     2
SYSF_ROOT_DONE          EQU     3
SYSF_IDE1_BUSY          EQU     4
SYSF_IDE1_DONE          EQU     5
SYSF_ROOT_QUEUE_IN_USE  EQU     6
SYSF_RAW_CRT_OUT        EQU     7

;SCANCODE FLAGS
SCF_NEW_KEY             EQU     16
SCF_SHIFT               EQU     8
SCF_CTRL                EQU     9
SCF_ALT                 EQU     10
SCF_WIN                 EQU     11

MAXGDT                  EQU     32
MAXIDT                  EQU     96
SYS_DEFAULT_FLAGS       EQU     040200H
SYS_DEFAULT_CR3         EQU     0       ;PAGING REGISTER

STK_FRAME       STRUC
SF_EBP          DD      ?
SF_EIP          DD      ?
SF_PARAM1       DD      ?
SF_PARAM2       DD      ?
SF_PARAM3       DD      ?
SF_PARAM4       DD      ?
SF_PARAM5       DD      ?
SF_PARAM6       DD      ?
SF_PARAM7       DD      ?
SF_PARAM8       DD      ?
                ENDS

GDT_STRUCT      STRUC
NIL_SEL                 DD      ?,?
SYS_BOOT_DS_SEL         DD      ?,?
SYS_BOOT_CS_SEL         DD      ?,?
SYS_FILES_SEL           DD      ?,?
SYS_DS_SEL              DD      ?,?
SYS_CS_SEL              DD      ?,?
ZERO_DS_SEL             DD      ?,?
CRT_SEL                 DD      ?,?
ROOT_TSS_SEL            DD      ?,?
FIRST_FREE_SEL          EQU     $
ROOT_TSS_MEM_SEL        EQU     FIRST_FREE_SEL
        ENDS

;SCAN CODES
SC_CURSOR_UP            EQU     048h
SC_CURSOR_DOWN          EQU     050h
SC_CURSOR_LEFT          EQU     04Bh
SC_CURSOR_RIGHT         EQU     04Dh
SC_PAGE_UP              EQU     049h
SC_PAGE_DOWN            EQU     051h

DISK_BLOCK_SIZE         EQU     512
DISK_BLOCK_BITS         EQU     9

DIRECTORY_ENTRY STRUC
DE_NAME         DB      56 DUP(?)
DE_BLOCK        DD      ?
DE_SIZE         DD      ?
DE_STRUCT_SIZE  EQU     $
                ENDS

MAX_DIRECTORY_ENTRIES   EQU     1024


FILE_BUFFER_BLOCK_COUNT EQU     127
;DOS CALL CODES
DC_HALT                 EQU     0
DC_READ_BLOCKS          EQU     1
DC_WRITE_BLOCKS         EQU     2
DC_GRAPHICS_MODE        EQU     3


;MEMORY RELATED
PAGE_SIZE       EQU     01000H
PAGE_BITS       EQU     12


MEMORY_BLOCK    STRUC
MB_NEXT         DD      ?
MB_SIZE         DD      ?
MB_STRUCT_SIZE  EQU     $
        ENDS

UNUSED_USER_MEMORY      STRUC           ;AVAILABLE TO MALLOC MEMORY
UUM_NEXT        DD      ?
UUM_SIZE        DD      ?
UUM_STRUCT_SIZE EQU     $
        ENDS

USED_USER_MEMORY_SIZE   EQU     -4      ;OFFSET OFF MALLOC MEMORY


ST_OS_ENUMS             STRUC
ST_NULL                 DB      ?
ST_ROOT_STARTUP         DB      ?
ST_USER_STARTUP         DB      ?
ST_COMMAND_PROMPT       DB      ?
ST_INTERRUPT            DB      ?
ST_TRAP                 DB      ?
ST_INVALID_TASK_ADDRESS DB      ?
ST_UNDEFINED_EXTERN     DB      ?
ST_UNDEFINED_SYSTEXT    DB      ?
ST_INVALID_EXTERN_NUMBER DB    ?
ST_INVALID_SYSTEXT_NUMBER DB    ?
ST_COMMAND_NOT_FOUND    DB      ?
ST_FILE_NOT_FOUND       DB      ?
                        ENDS

ST_COMPILER_ENUMS       STRUC
                        DB      256 DUP(?)
ST_PARSE_ERROR          DB      ?
ST_MISSING_SEMICOLON    DB      ?
ST_MISSING_EXPRESSION   DB      ?
ST_MISSING_RIGHT_PAREN  DB      ?
ST_MISSING_RIGHT_BRACKET DB     ?
ST_UNDEFINED_SYMBOL     DB      ?
ST_INVALID_CLASS        DB      ?
ST_INVALID_MEMBER       DB      ?
ST_EXPECTING_TYPE       DB      ?
ST_EXPECTING_IDENTIFIER DB      ?
ST_EXPECTING_LEFT_PAREN DB      ?
ST_EXPECTING_LEFT_BRACE DB      ?
ST_MISSING_RIGHT_BRACE  DB      ?
ST_INVALID_LVALUE       DB      ?
ST_EXPECTING_EQUAL      DB      ?
ST_NOT_POINTER_TYPE     DB      ?
ST_EXPECTING_COMMA      DB      ?
ST_EXPECTING_SYS_SYMBOL DB      ?
                        ENDS

SCREEN_COLS     EQU 100
SCREEN_LINES    EQU 75
SCREEN_WIDTH    EQU 800
SCREEN_HEIGHT   EQU 600
FONT_HEIGHT     EQU 8
FONT_WIDTH      EQU 8

ST_TABLE_SIZE           EQU     4096


MAXIO           EQU     0800H
HEAP_CACHE_SIZE EQU     512
TSS_DESC_LENGTH EQU     31

TSS_STRUCT      STRUC
TSS_BACK_LINK           DD      ?
TSS_ESP0                DD      ?
TSS_SS0                 DD      ?
TSS_ESP1                DD      ?
TSS_SS1                 DD      ?
TSS_ESP2                DD      ?
TSS_SS2                 DD      ?
TSS_CR3                 DD      ?
TSS_EIP                 DD      ?
TSS_EFLAGS              DD      ?
TSS_EAX                 DD      ?
TSS_ECX                 DD      ?
TSS_EDX                 DD      ?
TSS_EBX                 DD      ?
TSS_ESP                 DD      ?
TSS_EBP                 DD      ?
TSS_ESI                 DD      ?
TSS_EDI                 DD      ?
TSS_ES                  DD      ?
TSS_CS                  DD      ?
TSS_SS                  DD      ?
TSS_DS                  DD      ?
TSS_FS                  DD      ?
TSS_GS                  DD      ?
TSS_LDTR                DD      ?
TSS_TASK_SWITCH_TRAP    DW      ?
TSS_IO_BITMAP_PTR       DW      ?
TSS_IO_BITMAP           DB      (MAXIO/8) DUP (?)

TSS_TASK_DESCRIPTOR     DB      (TSS_DESC_LENGTH+1) DUP (?)

TSS_CONSOLE_LTF         DD      ?
TSS_MENU_LTF            DD      ?
TSS_MENU_SELECTION      DD      ?
TSS_CURSOR_OFFSET       DD      ?
TSS_CRT_SIZE            DD      ?
TSS_CRT_WIDTH           DD      ?
TSS_CRT_HEIGHT          DD      ?
TSS_CRT_SELECT          DW      ?
TSS_TEXT_ATTRIBUTE      DB      ?
TSS_BORDER_ATTRIBUTE    DB      ?
TSS_MENU_ATTRIBUTE      DB      ?
TSS_RESERVED_ATTRIBUTES DB      ?,?,?
TSS_WINDOW_RIGHT        DD      ?
TSS_WINDOW_LEFT         DD      ?
TSS_WINDOW_TOP          DD      ?
TSS_WINDOW_BOTTOM       DD      ?
TSS_CURSOR_HOR          DD      ?
TSS_CURSOR_VERT         DD      ?

CRTF_CURSOR_ON          EQU     0
CRTF_WRAP_AROUND        EQU     1        
TSS_CRT_FLAGS           DW      ?

TSSF_MALLOC_TRACE       EQU     0
TSSF_HEAP_IN_USE        EQU     1
TSSF_LEX_TRACE          EQU     2
TSSF_PARSE_TRACE        EQU     3
TSSF_COMPILE_TRACE      EQU     4
TSSF_RUN_TRACE          EQU     5
TSSF_ECHO               EQU     6
TSSF_SUSPENDED          EQU     7
TSSF_LOCAL_USER         EQU     8
TSSF_OPT_TRACE          EQU     9
TSS_TASK_FLAGS          DW      ?

TSS_SELECTOR            DD      ?      ;SELECTOR OF THIS TSS
TSS_ABSOLUTE_ADDRESS    DD      ?       ;ABSOLUTE ADDRESS OF THIS TSS
TSS_PARENT_TSS          DD      ?
TSS_NEXT_TSS            DD      ?       ;ABSOLUTE ADDRESS OF NEXT TSS
TSS_LAST_TSS            DD      ?       ;ABSOLUTE ADDRESS OF LAST TSS
TSS_MEMORY_BLOCK_LIST   DD      ?
TSS_HEAP_CACHE          DB      HEAP_CACHE_SIZE DUP (?)
TSS_MALLOC_FREE_LIST    DD      ?
TSS_TRAP_LIST           DD      ?
TSS_HASH_TABLE          DD      ?
TSS_STRING_TABLE        DD      ?
TSS_OPTIMIZATION        DD      ?
TSS_STARTUP_CMD         DD      ?
TSS_SYSTEM_RESULT       DD      ?
TSS_CURRENT_PARTITION   DD      ?
TSS_CURRENT_DIRECTORY   DD      ?
TSS_CURRENT_DIR_CLUSTER DD      ?

TSS_USER_AUX0           DD      ?
TSS_USER_AUX1           DD      ?
TSS_USER_AUX2           DD      ?
TSS_USER_AUX3           DD      ?
TSS_STRUCT_SIZE         EQU     $
ENDS


LINKED_TEXT_FILE STRUC
LTF_NEXT                DD ?
LTF_LAST                DD ?
LTF_FILENAME            DD ?
LTF_FLAGS               DD ?
LTF_LINE_START_COL      DD ?
LTF_LINE_END_COL        DD ?
LTF_TO_TEXT_CB          DD ?
LTF_CUR_DATA_COL_CB     DD ?
LTF_CUR_COL             DD ?
LTF_CUR_DATA_COL        DD ?
LTF_CUR_LINE            DD ?
LTF_CUR_TOP_LINE_NUM    DD ?
LTF_INSERT_POINT        DD ?
LTF_CUR_TOP_LINE        DD ?
LTF_NUM_LINES           DD ?
LTF_MAX_LINES           DD ?
LTF_STRUCT_SIZE         DD ?
        ENDS

LTFLF_SELECTED          EQU     0
LINKED_TEXT_FILE_LINE STRUC
LTFL_NEXT               DD ?
LTFL_LAST               DD ?
LTFL_FLAGS              DD ?
LTFL_LINE_DATA          DD ?
LTFL_STRUCT_SIZE        EQU     $
        ENDS

;LEX.ASM
TK_EOF                  EQU     0
TK_IDENT                EQU     1
TK_STRING               EQU     2
TK_INTEGER              EQU     3
TK_DOUBLE               EQU     4
TK_LESS_EQUAL           EQU     5
TK_EQUAL_EQUAL          EQU     6
TK_GREATER_EQUAL        EQU     7
TK_AND_AND              EQU     8
TK_OR_OR                EQU     9
TK_ROTATE_LEFT          EQU     0Bh
TK_ROTATE_RIGHT         EQU     0Ch
TK_CR                   EQU     0Dh
TK_DEREFERENCE          EQU     0Eh
TK_NOT_EQUAL            EQU     0Fh
TK_PLUS_PLUS            EQU     10h
TK_MINUS_MINUS          EQU     11h

TK_DOUBLE_QUOTE         EQU     022H
TK_SINGLE_QUOTE         EQU     027H
TK_LEFT_BRACE           EQU     07BH
TK_RIGHT_BRACE          EQU     07DH

LF_EXPECTING_HEX        EQU     1
LF_PROMPT               EQU     2
LF_DONT_FREE_BUFFER     EQU     4
LF_DOUBLE_SEMICOLON     EQU     8
LF_NO_DEFINES           EQU     16

LF_FUNCTION             EQU     001000000h
LF_POSTINC              EQU     002000000h
LF_POSTDEC              EQU     004000000h
LF_PREINC               EQU     008000000h
LF_PREDEC               EQU     010000000h
LF_ARRAY                EQU     020000000h      ;Used by compiler
LF_EAX                  EQU     040000000h      ;Used by compiler
LF_USE_LAST_CHAR        EQU     080000000h      ;Used by lex

LEX_MAX_IDENT           EQU     134            


LEX_FILE_NODE   STRUC
LFN_BUFFER              DD      ?
LFN_BUF_PTR             DD      ?
LFN_LINE_NUM            DD      ?
LFN_NAME                DD      ?
LFN_CUR_LINE_START      DD      ?
LFN_STRUCT_SIZE         EQU     $
                ENDS

LFN_COUNT               EQU     8
LFN_STACK_COUNT         EQU     8

LEX_NODE        STRUC
LEX_TOKEN               DD      ?
LEX_FLAGS               DD      ?
LEX_CUR_I               DD      ?
LEX_CUR_D               DD      ?,?,?
LEX_IDENT               DD      ?
LEX_LAST_CHAR           DD      ?
LEX_HASH_TABLE_LIST     DD      ?
LEX_HASH_ENTRY          DD      ?
LEX_LOCAL_VAR_LIST      DD      ?
LEX_LOCAL_VAR_ENTRY     DD      ?
LEX_CUR_BUF_PTR         DD      ?
LEX_CUR_LFN             DD      ?
LEX_STACK_PTR           DD      ?

LEX_CB_PUSH_START       EQU     $
LEX_CB_OUT_FIRST        DD      ?
LEX_CB_OUT_LAST         DD      ?
LEX_CB_OUT_END          DD      ?
LEX_CB_OUT_PTR          DD      ?
LEX_CB_LAST_OUT_PTR     DD      ?
LEX_CB_IN_FIRST         DD      ?
LEX_CB_IN_END           DD      ?
LEX_CB_IN_PTR           DD      ?
LEX_CB_LABEL_LIST       DD      ?
LEX_CB_STACK            DD      ?
LEX_CB_PUSH_END         EQU     $

LEX_LFNS                DB      LFN_COUNT*LFN_STRUCT_SIZE DUP(?)
LEX_STACK               DB      LFN_STACK_COUNT*LFN_STRUCT_SIZE DUP(?)
LEX_STRUCT_SIZE         EQU     $
                ENDS

HASH_TABLE      STRUC
HT_NEXT                 DD      ?
HT_MASK                 DD      ?
HT_BODY                 EQU     $
                ENDS

HASH_TABLE_ENTRY STRUC
HTE_NEXT                DD      ?
HTE_STRING              DD      ?
HTE_TYPE                DD      ?
HTE_USER0               DD      ?
HTE_USER1               DD      ?       ;Must be in same position as C_SIZE
HTE_USER2               DD      ?       ;Must be in same position as C_DEREFERENCE
HTE_STRUCT_SIZE         EQU     $
                ENDS

FILE_HASH_TABLE_ENTRY STRUC
FHTE_NEXT               DD      ?
FHTE_STRING             DD      ?
FHTE_TYPE               DD      ?
FHTE_MEMORY_ADDRESS     DD      ?       ;0 means not loaded
FHTE_SIZE               DD      ?
FHTE_DIRECTORY_ENTRY    DD      ?
FHTE_STRUCT_SIZE        EQU     $
                ENDS

;HASH TABLE ENTRY TYPES
HTT_INVALID             EQU     0
HTT_ALL                 EQU     0FFFFFFFFH
HTT_FILE                EQU     1
HTT_SYS_SYMBOL          EQU     2
HTT_LOCAL_VARIABLE      EQU     4
HTT_GLOBAL_VARIABLE     EQU     8
HTT_CLASS               EQU     16
HTT_KEYWORD             EQU     32
HTT_INTERNAL_TYPE       EQU     64
HTT_FUNCTION            EQU     128
HTT_STRING_CONSTANT     EQU     256

CONSTANT_NODE   STRUC
CT_HASH_NEXT    DD      ?
CT_HASH_STRING  DD      ?
CT_HASH_TYPE    DD      ?
CT_VALUE        DD      ?
CT_STRUCT_SIZE  EQU     $
                ENDS

;SOFTWARE INTERRUPTS
I_PUT_SYSTEXT         EQU     030H
I_PUT_CHAR            EQU     031H
I_GET_CHAR            EQU     032H
I_PRINTF              EQU     033H

STACK_BLOCK     STRUC
SP_EDI DD      ?
SP_ESI DD      ?
SP_EBP DD      ?
SP_ESP DD      ?
SP_EBX DD      ?
SP_EDX DD      ?
SP_ECX DD      ?
SP_EAX DD      ?
SP_STRUCT_SIZE EQU      $
                ENDS      

VGA_MISC_OUTPUT         EQU 003c2h
VGA_SC_INDEX            EQU 003c4h
VGA_SC_DATA             EQU 003c5h
VGA_PALETTE_INDEX       EQU 003c8h
VGA_PALETTE_DATA        EQU 003c9h
VGA_CRTC_INDEX          EQU 003d4h
VGA_MAP_MASK            EQU 002h
VGA_H_TOTAL             EQU 000h
VGA_H_BLANK_START       EQU 002h
VGA_H_RETRACE_START     EQU 004h
VGA_V_TOTAL             EQU 006h
VGA_MAX_SCAN_LINE       EQU 009h
VGA_V_RETRACE_END       EQU 011h
VGA_OFFSET              EQU 013h
VGA_V_BLANK_START       EQU 015h
VGA_MODE_CONTROL        EQU 017h

EXTERN_TABLE_DEFINES    STRUC
EXT_NULL                DB  ?
EXT_RBLOCKS             DB  ?
EXT_WBLOCKS             DB  ?
EXT_IRQ0E               DB  ?
EXT_DUMP                DB  ?
EXT_READ_FILE           DB  ?
EXT_LTF_PUTCHAR         DB  ?
EXT_LTF_UPDATE_SCREEN   DB  ?
EXT_BLANK_CRT_TEXT      DB  ?
EXT_IRQ03               DB  ?
EXT_IRQ04               DB  ?
EXT_SELECT_DEFAULT_DRIVE DB ?
                        ENDS
EXT_TABLE_SIZE          EQU     1024
