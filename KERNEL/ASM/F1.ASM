        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_OS_MAP
FIL_OS_MAP:

        DB 13,10,' Start  Stop   Length Name              '
        DB ' Class',13,10,13,10,' 00000H 0874FH 08750H S'
        DB 'YSCODE            ',13,10,' 08750H 1A0B2H 1196'
        DB '3H SYSFILES           ',13,10,' 1A0C0H 1A4BFH '
        DB '00400H DOSSTK             ',13,10,' 1A4C0H 1A5'
        DB '9EH 000DFH BOOTCODE           ',13,10,' 1A5A0H'
        DB ' 1A71FH 00180H INITCODE           ',13,10,13,10
        DB '  Address         Publics by Name',13,10,13,10
        DB ' 0000:03844       ACTIVATE_NEXT_USER',13,10,' '
        DB '0000:07854       ADDS',13,10,' 0000:04BBC     '
        DB '  ADD_INTERMEDIATE_CODE',13,10,' 0000:0684B   '
        DB '    ADD_MAP_SYMBOLS',13,10,' 0000:03638       '
        DB 'ADD_TASK_TO_QUEUE',13,10,' 0000:00CDB       AD'
        DB 'D_USER_HASH_ENTRIES',13,10,' 0000:03350       '
        DB 'ALLOCATE_MEMORY_BLOCKS',13,10,' 0000:02C49    '
        DB '   BINARY_TO_FIXED_HEX',13,10,' 0000:02C44    '
        DB '   BINARY_TO_HEX',13,10,' 1A4C:0005A       BOO'
        DB 'T_CODE',13,10,' 1A4C:000D7       BOOT_IMG_BLOC'
        DB 'K_LO',13,10,' 0000:01AC0       CLEAR_CRT',13,10
        DB ' 0000:0390C       CMD_ACTIVATE_NEXT_USER',13,10
        DB ' 0000:05B78       CMD_ADD_SYS_HASH',13,10,' 00'
        DB '00:07814       CMD_ATOI',13,10,' 0000:05B58   '
        DB '    CMD_CRT_LINE_OUT',13,10,' 0000:07A24      '
        DB ' CMD_DEFINE_EXTERN',13,10,' 0000:05B68       C'
        DB 'MD_DELETE_LINKED_LIST',13,10,' 0000:05B18     '
        DB '  CMD_FIND_HASH_ENTRY',13,10,' 0000:05B08     '
        DB '  CMD_FREE',13,10,' 0000:05C1C       CMD_GET_C'
        DB 'HAR',13,10,' 0000:05C30       CMD_GET_STRING',13
        DB 10,' 0000:0297C       CMD_GR_CLEAR',13,10,' 00'
        DB '00:02AA4       CMD_GR_LINE',13,10,' 0000:02994'
        DB '       CMD_GR_PLOT',13,10,' 0000:05B34       C'
        DB 'MD_KILL',13,10,' 0000:07A04       CMD_LOADSTR',13
        DB 10,' 0000:05BC4       CMD_LOAD_TEXT_FILE',13,10
        DB ' 0000:05AF4       CMD_MALLOC',13,10,' 0000:05B'
        DB 'D8       CMD_NEW_STRING',13,10,' 0000:05C10   '
        DB '    CMD_PUT_CHAR',13,10,' 0000:05C20       CMD'
        DB '_PUT_STRING',13,10,' 0000:02E28       CMD_QSYS'
        DB 'TEM',13,10,' 0000:030D0       CMD_QUEUE_ROOT',13
        DB 10,' 0000:02D08       CMD_RAND',13,10,' 0000:0'
        DB '0FC0       CMD_REBOOT',13,10,' 0000:05AE4     '
        DB '  CMD_RFREE',13,10,' 0000:05AD0       CMD_RMAL'
        DB 'LOC',13,10,' 0000:05BEC       CMD_RNEW_STRING',13
        DB 10,' 0000:03084       CMD_ROOT',13,10,' 0000:0'
        DB '5B98       CMD_SLEEP',13,10,' 0000:079F4      '
        DB ' CMD_SPACES',13,10,' 0000:02DE8       CMD_SPAW'
        DB 'N',13,10,' 0000:02CF0       CMD_SRAND',13,10
        DB ' 0000:077A0       CMD_STRCAT',13,10,' 0000:077'
        DB 'D8       CMD_STRCMP',13,10,' 0000:077C0       '
        DB 'CMD_STRCPY',13,10,' 0000:05C00       CMD_STRLE'
        DB 'N',13,10,' 0000:02E70       CMD_SYSTEM',13,10
        DB ' 0000:05B44       CMD_TO_LINKED_TEXT_FILE',13,10
        DB ' 0000:03C84       CODE_TEXT_TABLE',13,10,' 000'
        DB '0:02F34       COMMAND_LINE',13,10,' 0000:049BF'
        DB '       COMPILE_CODE',13,10,' 0000:08698       '
        DB 'COMPILE_EXPRESSION',13,10,' 0000:067B4       C'
        DB 'OMPILE_STATEMENT',13,10,' 0000:07E74       CON'
        DB 'T_PARSE_EXPRESSION',13,10,' 0000:00ED0       C'
        DB 'REATE_CODE_SELECTOR',13,10,' 0000:00E88       '
        DB 'CREATE_MEMORY_SELECTOR',13,10,' 0000:00F18    '
        DB '   CREATE_TSS_SELECTOR',13,10,' 0000:079BC    '
        DB '   CRLF',13,10,' 0000:01C94       CRT_LINE_OUT'
        DB 13,10,' 0000:01BBC       CRT_OUT',13,10,' 000'
        DB '0:01958       CURSOR_HOME',13,10,' 0000:0198C '
        DB '      CURSOR_OFF',13,10,' 0000:019CC       CUR'
        DB 'SOR_ON',13,10,' 0000:03878       DEACTIVATE_US'
        DB 'ER',13,10,' 0000:02BF0       DEBUG_SPIN_WAIT',13
        DB 10,' 0000:07E6C       DELETE_EXPRESSION',13,10
        DB ' 0000:06DE8       DELETE_LEX',13,10,' 0000:02B'
        DB 'FC       DELETE_LINKED_LIST',13,10,' 0000:0263'
        DB '4       DRAW_BORDER',13,10,' 0000:02598       '
        DB 'DRAW_CHAR',13,10,' 1A4C:000DF       END_BOOT_C'
        DB 'ODE',13,10,' 0000:03738       END_TASK',13,10
        DB ' 0000:02CC0       END_TRAP',13,10,' 1A5A:00165'
        DB '       ENTER_GRAPHICS_MODE',13,10,' 1A5A:00014'
        DB '       ENTER_PMODE',13,10,' 0000:0870C       E'
        DB 'VAL_EXPRESSION',13,10,' 0000:02ECC       EXECU'
        DB 'TE_COMMAND_LINE',13,10,' 0000:01A24       FILL'
        DB '_CRT',13,10,' 0875:0890C       FIL_FAT32_CPP',13
        DB 10,' 0875:05888       FIL_KLUDGE_CPP',13,10,' '
        DB '0875:00000       FIL_OS_MAP',13,10,' 0875:0348'
        DB '8       FIL_ROOT_CPP',13,10,' 0875:0FD24      '
        DB ' FIL_STRINGS_CPP',13,10,' 0875:11084       FIL'
        DB '_SYSTEXT_CPP',13,10,' 0000:07934       FIND_HA'
        DB 'SH_BUCKET',13,10,' 0000:078E4       FIND_HASH_'
        DB 'ENTRY',13,10,' 0000:05C68       FIND_MEMBER',13
        DB 10,' 0000:0192C       FIXED_PUT_HEX',13,10,' 0'
        DB '000:018D1       FIXED_PUT_STRING',13,10,' 0000'
        DB ':04D00       FIXUP_CODE',13,10,' 0000:0351C   '
        DB '    FREE',13,10,' 0000:033B8       FREE_BLOCK_'
        DB 'LIST',13,10,' 0000:00000       GDTTAB',13,10
        DB ' 0000:0187C       GET_CHAR',13,10,' 0000:08724'
        DB '       GET_EXPRESSION',13,10,' 0000:04C4C     '
        DB '  GET_INTERMEDIATE_CODE',13,10,' 0000:04C80   '
        DB '    GET_INTERMEDIATE_CODE2',13,10,' 0000:06AE0'
        DB '       GET_KEY',13,10,' 0000:01884       GET_S'
        DB 'TRING',13,10,' 0000:026F4       GR_UPDATE_SCRE'
        DB 'EN',13,10,' 0000:07894       HASH_STRING',13,10
        DB ' 0000:02C70       HEX_TO_BINARY',13,10,' 0000:'
        DB '00518       IDTTAB',13,10,' 0000:00F60       I'
        DB 'NIT_8259_AND_LIDT',13,10,' 0000:04C94       IN'
        DB 'IT_CB_IN_PTRS',13,10,' 0000:04C1C       INIT_C'
        DB 'B_OUT_PTRS',13,10,' 0000:01D20       INIT_CRT',13
        DB 10,' 0000:03318       INIT_MEMORY',13,10,' 000'
        DB '0:07944       INIT_SYSTEXT_AND_EXTERNS',13,10,' '
        DB '0000:02C14       INSQUE',13,10,' 0000:02C24   '
        DB '    INSQUE_REV',13,10,' 0000:01018       IRQ_V'
        DB 'ECTORS',13,10,' 0000:01198       JIFFIES',13,10
        DB ' 0000:038BC       KILL_ACTIVE_USER',13,10,' 00'
        DB '00:03774       KILL_TASK',13,10,' 0000:07160  '
        DB '     LEX',13,10,' 0000:076BC       LEX_CB_APPE'
        DB 'ND',13,10,' 0000:0767C       LEX_CB_POP',13,10
        DB ' 0000:076A0       LEX_CB_POP_NO_FREE',13,10,' '
        DB '0000:07650       LEX_CB_PUSH',13,10,' 0000:06E'
        DB '78       LEX_GET_CHAR',13,10,' 0000:070A4     '
        DB '  LEX_INCLUDE',13,10,' 0000:0712C       LEX_PO'
        DB 'P',13,10,' 0000:070EC       LEX_PUSH',13,10,' '
        DB '0000:07158       LEX_SUB_STACK_PTR',13,10,' 00'
        DB '00:07734       LOAD_TEXT_FILE',13,10,' 0000:03'
        DB '3F4       MALLOC',13,10,' 0000:03314       MEM'
        DB 'ORY_FREE_LIST',13,10,' 0000:05C9C       NEW_CL'
        DB 'ASS',13,10,' 0000:078B8       NEW_HASH_TABLE',13
        DB 10,' 0000:06D80       NEW_LEX',13,10,' 0000:07'
        DB '838       NEW_STRING',13,10,' 0000:06818      '
        DB ' NEW_SYSFILES_STRING',13,10,' 0000:05678      '
        DB ' OPTIMIZE_CODE',13,10,' 0000:07DE8       PARSE'
        DB '_ERROR',13,10,' 0000:0655C       PARSE_STATEME'
        DB 'NT',13,10,' 0000:025F0       PLOT_CHAR',13,10
        DB ' 0000:04B82       PUT_ALL_IC',13,10,' 0000:018'
        DB 'BC       PUT_CHAR',13,10,' 0000:04AF8       PU'
        DB 'T_CLASS',13,10,' 0000:018F8       PUT_DEC',13
        DB 10,' 0000:01905       PUT_HEX',13,10,' 0000:01'
        DB '91F       PUT_HEX_BYTE',13,10,' 0000:01912    '
        DB '   PUT_HEX_WORD',13,10,' 0000:04B3C       PUT_'
        DB 'IC',13,10,' 0000:018C4       PUT_STRING',13,10
        DB ' 0000:07988       PUT_SYSTEXT',13,10,' 0000:06'
        DB 'E2C       PUT_TOKEN',13,10,' 0000:02FC0       '
        DB 'QSYSTEM_COMMAND_LINE',13,10,' 0000:02C9C      '
        DB ' REGISTER_TRAP',13,10,' 0000:02C34       REMQU'
        DB 'E',13,10,' 0000:037A8       REMQUE_TASK',13,10
        DB ' 0000:036D4       RESTORE_CONTEXT',13,10,' 000'
        DB '0:035E0       RFREE',13,10,' 0000:035B8       '
        DB 'RMALLOC',13,10,' 0000:035CC       RNEW_STRING',13
        DB 10,' 0000:03124       ROOT_COMMAND_LINE',13,10
        DB ' 0000:00100       ROOT_TSS',13,10,' 0000:08718'
        DB '       RUN_STATEMENT',13,10,' 0000:03664      '
        DB ' SAVE_CONTEXT',13,10,' 0000:079D0       SPACE',13
        DB 10,' 0000:079DC       SPACES',13,10,' 0000:00D'
        DB '50       SPAWN_TASK',13,10,' 0000:03814       '
        DB 'SPAWN_USER_AND_ACTIVATE',13,10,' 1A5A:00000   '
        DB '    START',13,10,' 1A4C:00000       START_BOOT'
        DB '_CODE',13,10,' 0000:07790       STRLEN',13,10
        DB ' 0000:037C8       SWAP_IN_NEXT',13,10,' 0000:0'
        DB '37F1       SWAP_IN_NEXT_PART2',13,10,' 0000:03'
        DB '018       SYSTEM_COMMAND_LINE',13,10,' 0000:07'
        DB '710       SYSTEM_EXTERN_TABLE',13,10,' 0000:03'
        DB '310       SYSTEM_MEMORY_HIGH',13,10,' 0000:033'
        DB '0C       SYSTEM_MEMORY_LOW',13,10,' 0000:0770C'
        DB '       SYSTEM_STRING_TABLE',13,10,' 0000:0081C'
        DB '       SYS_BOOT_CODE',13,10,' 0000:00820      '
        DB ' SYS_BOOT_IMAGE_END',13,10,' 0000:00818       '
        DB 'SYS_BOOT_IMAGE_START',13,10,' 0000:06AFC      '
        DB ' SYS_BPT_LIST',13,10,' 0000:0086C       SYS_CL'
        DB 'IPBOARD_LTF',13,10,' 0000:08750       SYS_CS_S'
        DB 'IZE',13,10,' 0000:06930       SYS_CURRENT_SCAN'
        DB '_CODE',13,10,' 0000:0082C       SYS_FLAGS',13
        DB 10,' 0000:01D94       SYS_FONT',13,10,' 0000:0'
        DB '0864       SYS_GRAPHICS_BASE',13,10,' 0000:008'
        DB '68       SYS_GRAPHICS_HIDDEN',13,10,' 0000:008'
        DB '3C       SYS_HASH_TABLE',13,10,' 0000:00840   '
        DB '    SYS_INTERNAL_TYPES',13,10,' 0000:00838    '
        DB '   SYS_MEMORY_BASE',13,10,' 0000:00824       S'
        DB 'YS_TEMP_PTR',13,10,' 0000:00860       SYS_TEXT'
        DB '_BASE',13,10,' 0000:03250       TO_LINKED_TEXT'
        DB '_FILE',13,10,' 0000:06A84       TRANSLATE_SCAN'
        DB '_CODE',13,10,' 0000:02CD8       TRAP',13,10,' '
        DB '0000:035F4       TSK_FREEMEM',13,10,' 0000:029'
        DB '70       TSK_GR_UPDATE_SCREEN',13,10,' 0000:00'
        DB 'FF0       TSK_POLL_KEYBOARD',13,10,' 0000:0770'
        DB '4       ZEROS',13,10,13,10,'  Address       '
        DB '  Publics by Value',13,10,13,10,' 0000:08750'
        DB '       SYS_CS_SIZE',13,10,' 0000:08724       G'
        DB 'ET_EXPRESSION',13,10,' 0000:08718       RUN_ST'
        DB 'ATEMENT',13,10,' 0000:0870C       EVAL_EXPRESS'
        DB 'ION',13,10,' 0000:08698       COMPILE_EXPRESSI'
        DB 'ON',13,10,' 0000:07E74       CONT_PARSE_EXPRES'
        DB 'SION',13,10,' 0000:07E6C       DELETE_EXPRESSI'
        DB 'ON',13,10,' 0000:07DE8       PARSE_ERROR',13,10
        DB ' 0000:07A24       CMD_DEFINE_EXTERN',13,10,' 0'
        DB '000:07A04       CMD_LOADSTR',13,10,' 0000:079F'
        DB '4       CMD_SPACES',13,10,' 0000:079DC       S'
        DB 'PACES',13,10,' 0000:079D0       SPACE',13,10
        DB ' 0000:079BC       CRLF',13,10,' 0000:07988    '
        DB '   PUT_SYSTEXT',13,10,' 0000:07944       INIT_'
        DB 'SYSTEXT_AND_EXTERNS',13,10,' 0000:07934       '
        DB 'FIND_HASH_BUCKET',13,10,' 0000:078E4       FIN'
        DB 'D_HASH_ENTRY',13,10,' 0000:078B8       NEW_HAS'
        DB 'H_TABLE',13,10,' 0000:07894       HASH_STRING',13
        DB 10,' 0000:07854       ADDS',13,10,' 0000:07838'
        DB '       NEW_STRING',13,10,' 0000:07814       CM'
        DB 'D_ATOI',13,10,' 0000:077D8       CMD_STRCMP',13
        DB 10,' 0000:077C0       CMD_STRCPY',13,10,' 0000'
        DB ':077A0       CMD_STRCAT',13,10,' 0000:07790   '
        DB '    STRLEN',13,10,' 0000:07734       LOAD_TEXT'
        DB '_FILE',13,10,' 0000:07710       SYSTEM_EXTERN_'
        DB 'TABLE',13,10,' 0000:0770C       SYSTEM_STRING_'
        DB 'TABLE',13,10,' 0000:07704       ZEROS',13,10
        DB ' 0000:076BC       LEX_CB_APPEND',13,10,' 0000:'
        DB '076A0       LEX_CB_POP_NO_FREE',13,10,' 0000:0'
        DB '767C       LEX_CB_POP',13,10,' 0000:07650     '
        DB '  LEX_CB_PUSH',13,10,' 0000:07160       LEX',13
        DB 10,' 0000:07158       LEX_SUB_STACK_PTR',13,10
        DB ' 0000:0712C       LEX_POP',13,10,' 0000:070EC '
        DB '      LEX_PUSH',13,10,' 0000:070A4       LEX_I'
        DB 'NCLUDE',13,10,' 0000:06E78       LEX_GET_CHAR',13
        DB 10,' 0000:06E2C       PUT_TOKEN',13,10,' 0000:'
        DB '06DE8       DELETE_LEX',13,10,' 0000:06D80    '
        DB '   NEW_LEX',13,10,' 0000:06AFC       SYS_BPT_L'
        DB 'IST',13,10,' 0000:06AE0       GET_KEY',13,10
        DB ' 0000:06A84       TRANSLATE_SCAN_CODE',13,10,' '
        DB '0000:06930       SYS_CURRENT_SCAN_CODE',13,10,' '
        DB '0000:0684B       ADD_MAP_SYMBOLS',13,10,' 0000'
        DB ':06818       NEW_SYSFILES_STRING',13,10,' 0000'
        DB ':067B4       COMPILE_STATEMENT',13,10,' 0000:0'
        DB '655C       PARSE_STATEMENT',13,10,' 0000:05C9C'
        DB '       NEW_CLASS',13,10,' 0000:05C68       FIN'
        DB 'D_MEMBER',13,10,' 0000:05C30       CMD_GET_STR'
        DB 'ING',13,10,' 0000:05C20       CMD_PUT_STRING',13
        DB 10,' 0000:05C1C       CMD_GET_CHAR',13,10,' 00'
        DB '00:05C10       CMD_PUT_CHAR',13,10,' 0000:05C0'
        DB '0       CMD_STRLEN',13,10,' 0000:05BEC       C'
        DB 'MD_RNEW_STRING',13,10,' 0000:05BD8       CMD_N'
        DB 'EW_STRING',13,10,' 0000:05BC4       CMD_LOAD_T'
        DB 'EXT_FILE',13,10,' 0000:05B98       CMD_SLEEP',13
        DB 10,' 0000:05B78       CMD_ADD_SYS_HASH',13,10,' '
        DB '0000:05B68       CMD_DELETE_LINKED_LIST',13,10
        DB ' 0000:05B58       CMD_CRT_LINE_OUT',13,10,' 00'
        DB '00:05B44       CMD_TO_LINKED_TEXT_FILE',13,10,' '
        DB '0000:05B34       CMD_KILL',13,10,' 0000:05B18 '
        DB '      CMD_FIND_HASH_ENTRY',13,10,' 0000:05B08 '
        DB '      CMD_FREE',13,10,' 0000:05AF4       CMD_M'
        DB 'ALLOC',13,10,' 0000:05AE4       CMD_RFREE',13
        DB 10,' 0000:05AD0       CMD_RMALLOC',13,10,' 000'
        DB '0:05678       OPTIMIZE_CODE',13,10,' 0000:04D0'
        DB '0       FIXUP_CODE',13,10,' 0000:04C94       I'
        DB 'NIT_CB_IN_PTRS',13,10,' 0000:04C80       GET_I'
        DB 'NTERMEDIATE_CODE2',13,10,' 0000:04C4C       GE'
        DB 'T_INTERMEDIATE_CODE',13,10,' 0000:04C1C       '
        DB 'INIT_CB_OUT_PTRS',13,10,' 0000:04BBC       ADD'
        DB '_INTERMEDIATE_CODE',13,10,' 0000:04B82       P'
        DB 'UT_ALL_IC',13,10,' 0000:04B3C       PUT_IC',13
        DB 10,' 0000:04AF8       PUT_CLASS',13,10,' 0000:'
        DB '049BF       COMPILE_CODE',13,10,' 0000:03C84  '
        DB '     CODE_TEXT_TABLE',13,10,' 0000:0390C      '
        DB ' CMD_ACTIVATE_NEXT_USER',13,10,' 0000:038BC   '
        DB '    KILL_ACTIVE_USER',13,10,' 0000:03878      '
        DB ' DEACTIVATE_USER',13,10,' 0000:03844       ACT'
        DB 'IVATE_NEXT_USER',13,10,' 0000:03814       SPAW'
        DB 'N_USER_AND_ACTIVATE',13,10,' 0000:037F1       '
        DB 'SWAP_IN_NEXT_PART2',13,10,' 0000:037C8       S'
        DB 'WAP_IN_NEXT',13,10,' 0000:037A8       REMQUE_T'
        DB 'ASK',13,10,' 0000:03774       KILL_TASK',13,10
        DB ' 0000:03738       END_TASK',13,10,' 0000:036D4'
        DB '       RESTORE_CONTEXT',13,10,' 0000:03664    '
        DB '   SAVE_CONTEXT',13,10,' 0000:03638       ADD_'
        DB 'TASK_TO_QUEUE',13,10,' 0000:035F4       TSK_FR'
        DB 'EEMEM',13,10,' 0000:035E0       RFREE',13,10
        DB ' 0000:035CC       RNEW_STRING',13,10,' 0000:03'
        DB '5B8       RMALLOC',13,10,' 0000:0351C       FR'
        DB 'EE',13,10,' 0000:033F4       MALLOC',13,10,' '
        DB '0000:033B8       FREE_BLOCK_LIST',13,10,' 0000'
        DB ':03350       ALLOCATE_MEMORY_BLOCKS',13,10,' 0'
        DB '000:03318       INIT_MEMORY',13,10,' 0000:0331'
        DB '4       MEMORY_FREE_LIST',13,10,' 0000:03310  '
        DB '     SYSTEM_MEMORY_HIGH',13,10,' 0000:0330C   '
        DB '    SYSTEM_MEMORY_LOW',13,10,' 0000:03250     '
        DB '  TO_LINKED_TEXT_FILE',13,10,' 0000:03124     '
        DB '  ROOT_COMMAND_LINE',13,10,' 0000:030D0       '
        DB 'CMD_QUEUE_ROOT',13,10,' 0000:03084       CMD_R'
        DB 'OOT',13,10,' 0000:03018       SYSTEM_COMMAND_L'
        DB 'INE',13,10,' 0000:02FC0       QSYSTEM_COMMAND_'
        DB 'LINE',13,10,' 0000:02F34       COMMAND_LINE',13
        DB 10,' 0000:02ECC       EXECUTE_COMMAND_LINE',13,10
        DB ' 0000:02E70       CMD_SYSTEM',13,10,' 0000:02E'
        DB '28       CMD_QSYSTEM',13,10,' 0000:02DE8      '
        DB ' CMD_SPAWN',13,10,' 0000:02D08       CMD_RAND',13
        DB 10,' 0000:02CF0       CMD_SRAND',13,10,' 0000:'
        DB '02CD8       TRAP',13,10,' 0000:02CC0       END'
        DB '_TRAP',13,10,' 0000:02C9C       REGISTER_TRAP',13
        DB 10,' 0000:02C70       HEX_TO_BINARY',13,10,' 0'
        DB '000:02C49       BINARY_TO_FIXED_HEX',13,10,' 0'
        DB '000:02C44       BINARY_TO_HEX',13,10,' 0000:02'
        DB 'C34       REMQUE',13,10,' 0000:02C24       INS'
        DB 'QUE_REV',13,10,' 0000:02C14       INSQUE',13,10
        DB ' 0000:02BFC       DELETE_LINKED_LIST',13,10,' '
        DB '0000:02BF0       DEBUG_SPIN_WAIT',13,10,' 0000'
        DB ':02AA4       CMD_GR_LINE',13,10,' 0000:02994  '
        DB '     CMD_GR_PLOT',13,10,' 0000:0297C       CMD'
        DB '_GR_CLEAR',13,10,' 0000:02970       TSK_GR_UPD'
        DB 'ATE_SCREEN',13,10,' 0000:026F4       GR_UPDATE'
        DB '_SCREEN',13,10,' 0000:02634       DRAW_BORDER',13
        DB 10,' 0000:025F0       PLOT_CHAR',13,10,' 0000:'
        DB '02598       DRAW_CHAR',13,10,' 0000:01D94     '
        DB '  SYS_FONT',13,10,' 0000:01D20       INIT_CRT',13
        DB 10,' 0000:01C94       CRT_LINE_OUT',13,10,' 00'
        DB '00:01BBC       CRT_OUT',13,10,' 0000:01AC0    '
        DB '   CLEAR_CRT',13,10,' 0000:01A24       FILL_CR'
        DB 'T',13,10,' 0000:019CC       CURSOR_ON',13,10
        DB ' 0000:0198C       CURSOR_OFF',13,10,' 0000:019'
        DB '58       CURSOR_HOME',13,10,' 0000:0192C      '
        DB ' FIXED_PUT_HEX',13,10,' 0000:0191F       PUT_H'
        DB 'EX_BYTE',13,10,' 0000:01912       PUT_HEX_WORD'
        DB 13,10,' 0000:01905       PUT_HEX',13,10,' 000'
        DB '0:018F8       PUT_DEC',13,10,' 0000:018D1     '
        DB '  FIXED_PUT_STRING',13,10,' 0000:018C4       P'
        DB 'UT_STRING',13,10,' 0000:018BC       PUT_CHAR',13
        DB 10,' 0000:01884       GET_STRING',13,10,' 0000'
        DB ':0187C       GET_CHAR',13,10,' 0000:01198     '
        DB '  JIFFIES',13,10,' 0000:01018       IRQ_VECTOR'
        DB 'S',13,10,' 0000:00FF0       TSK_POLL_KEYBOARD',13
        DB 10,' 0000:00FC0       CMD_REBOOT',13,10,' 0000'
        DB ':00F60       INIT_8259_AND_LIDT',13,10,' 0000:'
        DB '00F18       CREATE_TSS_SELECTOR',13,10,' 0000:'
        DB '00ED0       CREATE_CODE_SELECTOR',13,10,' 0000'
        DB ':00E88       CREATE_MEMORY_SELECTOR',13,10,' 0'
        DB '000:00D50       SPAWN_TASK',13,10,' 0000:00CDB'
        DB '       ADD_USER_HASH_ENTRIES',13,10,' 0000:008'
        DB '6C       SYS_CLIPBOARD_LTF',13,10,' 0000:00868'
        DB '       SYS_GRAPHICS_HIDDEN',13,10,' 0000:00864'
        DB '       SYS_GRAPHICS_BASE',13,10,' 0000:00860  '
        DB '     SYS_TEXT_BASE',13,10,' 0000:00840       S'
        DB 'YS_INTERNAL_TYPES',13,10,' 0000:0083C       SY'
        DB 'S_HASH_TABLE',13,10,' 0000:00838       SYS_MEM'
        DB 'ORY_BASE',13,10,' 0000:0082C       SYS_FLAGS',13
        DB 10,' 0000:00824       SYS_TEMP_PTR',13,10,' 00'
        DB '00:00820       SYS_BOOT_IMAGE_END',13,10,' 000'
        DB '0:0081C       SYS_BOOT_CODE',13,10,' 0000:0081'
        DB '8       SYS_BOOT_IMAGE_START',13,10,' 0000:005'
        DB '18       IDTTAB',13,10,' 0000:00100       ROOT'
        DB '_TSS',13,10,' 0000:00000       GDTTAB',13,10
        DB ' 0875:11084       FIL_SYSTEXT_CPP',13,10,' 087'
        DB '5:0FD24       FIL_STRINGS_CPP',13,10,' 0875:08'
        DB '90C       FIL_FAT32_CPP',13,10,' 0875:05888   '
        DB '    FIL_KLUDGE_CPP',13,10,' 0875:03488       F'
        DB 'IL_ROOT_CPP',13,10,' 0875:00000       FIL_OS_M'
        DB 'AP',13,10,' 1A4C:000DF       END_BOOT_CODE',13
        DB 10,' 1A4C:000D7       BOOT_IMG_BLOCK_LO',13,10
        DB ' 1A4C:0005A       BOOT_CODE',13,10,' 1A4C:0000'
        DB '0       START_BOOT_CODE',13,10,' 1A5A:00165   '
        DB '    ENTER_GRAPHICS_MODE',13,10,' 1A5A:00014   '
        DB '    ENTER_PMODE',13,10,' 1A5A:00000       STAR'
        DB 'T',13,10,13,10,'Program entry point at 1A5A:'
        DB '0000',13,10,'Warning: No stack',13,10,13,10
        DB 0
        ENDS
        END
