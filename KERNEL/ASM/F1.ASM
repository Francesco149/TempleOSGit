        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_OS_MAP
FIL_OS_MAP:

        DB 13,10,' Start  Stop   Length Name              '
        DB ' Class',13,10,13,10,' 00000H 08BABH 08BACH S'
        DB 'YSCODE            ',13,10,' 08BACH 1AC5AH 120A'
        DB 'FH SYSFILES           ',13,10,' 1AC60H 1B05FH '
        DB '00400H DOSSTK             ',13,10,' 1B060H 1B1'
        DB '3EH 000DFH BOOTCODE           ',13,10,' 1B140H'
        DB ' 1B2BFH 00180H INITCODE           ',13,10,13,10
        DB '  Address         Publics by Name',13,10,13,10
        DB ' 0000:0390C       ACTIVATE_NEXT_USER',13,10,' '
        DB '0000:07AFC       ADDS',13,10,' 0000:04C90     '
        DB '  ADD_INTERMEDIATE_CODE',13,10,' 0000:06ABB   '
        DB '    ADD_MAP_SYMBOLS',13,10,' 0000:03700       '
        DB 'ADD_TASK_TO_QUEUE',13,10,' 0000:00D73       AD'
        DB 'D_USER_HASH_ENTRIES',13,10,' 0000:03418       '
        DB 'ALLOCATE_MEMORY_BLOCKS',13,10,' 0000:02D09    '
        DB '   BINARY_TO_FIXED_HEX',13,10,' 0000:02D04    '
        DB '   BINARY_TO_HEX',13,10,' 1B06:0005A       BOO'
        DB 'T_CODE',13,10,' 1B06:000D7       BOOT_IMG_BLOC'
        DB 'K_LO',13,10,' 0000:01B60       CLEAR_CRT',13,10
        DB ' 0000:039D4       CMD_ACTIVATE_NEXT_USER',13,10
        DB ' 0000:05C4C       CMD_ADD_SYS_HASH',13,10,' 00'
        DB '00:07ABC       CMD_ATOI',13,10,' 0000:05C2C   '
        DB '    CMD_CRT_LINE_OUT',13,10,' 0000:07CCC      '
        DB ' CMD_DEFINE_EXTERN',13,10,' 0000:05C3C       C'
        DB 'MD_DELETE_LINKED_LIST',13,10,' 0000:05BEC     '
        DB '  CMD_FIND_HASH_ENTRY',13,10,' 0000:05BDC     '
        DB '  CMD_FREE',13,10,' 0000:05CF0       CMD_GET_C'
        DB 'HAR',13,10,' 0000:05D04       CMD_GET_STRING',13
        DB 10,' 0000:02A1C       CMD_GR_CLEAR',13,10,' 00'
        DB '00:02B64       CMD_GR_LINE',13,10,' 0000:02A34'
        DB '       CMD_GR_PLOT',13,10,' 0000:05C08       C'
        DB 'MD_KILL',13,10,' 0000:07CAC       CMD_LOADSTR',13
        DB 10,' 0000:05C98       CMD_LOAD_TEXT_FILE',13,10
        DB ' 0000:05BC8       CMD_MALLOC',13,10,' 0000:05C'
        DB 'AC       CMD_NEW_STRING',13,10,' 0000:05CE4   '
        DB '    CMD_PUT_CHAR',13,10,' 0000:05CF4       CMD'
        DB '_PUT_STRING',13,10,' 0000:02EE8       CMD_QSYS'
        DB 'TEM',13,10,' 0000:03190       CMD_QUEUE_ROOT',13
        DB 10,' 0000:02DC8       CMD_RAND',13,10,' 0000:0'
        DB '1058       CMD_REBOOT',13,10,' 0000:05BB8     '
        DB '  CMD_RFREE',13,10,' 0000:05BA4       CMD_RMAL'
        DB 'LOC',13,10,' 0000:05CC0       CMD_RNEW_STRING',13
        DB 10,' 0000:03144       CMD_ROOT',13,10,' 0000:0'
        DB '5C6C       CMD_SLEEP',13,10,' 0000:07C9C      '
        DB ' CMD_SPACES',13,10,' 0000:02EA8       CMD_SPAW'
        DB 'N',13,10,' 0000:02DB0       CMD_SRAND',13,10
        DB ' 0000:07A48       CMD_STRCAT',13,10,' 0000:07A'
        DB '80       CMD_STRCMP',13,10,' 0000:07A68       '
        DB 'CMD_STRCPY',13,10,' 0000:05CD4       CMD_STRLE'
        DB 'N',13,10,' 0000:02F30       CMD_SYSTEM',13,10
        DB ' 0000:05C18       CMD_TO_LINKED_TEXT_FILE',13,10
        DB ' 0000:03D4C       CODE_TEXT_TABLE',13,10,' 000'
        DB '0:02FF4       COMMAND_LINE',13,10,' 0000:04A93'
        DB '       COMPILE_CODE',13,10,' 0000:08AF4       '
        DB 'COMPILE_EXPRESSION',13,10,' 0000:06A24       C'
        DB 'OMPILE_STATEMENT',13,10,' 0000:0811C       CON'
        DB 'T_PARSE_EXPRESSION',13,10,' 0000:00F68       C'
        DB 'REATE_CODE_SELECTOR',13,10,' 0000:00F20       '
        DB 'CREATE_MEMORY_SELECTOR',13,10,' 0000:00FB0    '
        DB '   CREATE_TSS_SELECTOR',13,10,' 0000:07C64    '
        DB '   CRLF',13,10,' 0000:01D34       CRT_LINE_OUT'
        DB 13,10,' 0000:01C5C       CRT_OUT',13,10,' 000'
        DB '0:019F8       CURSOR_HOME',13,10,' 0000:01A2C '
        DB '      CURSOR_OFF',13,10,' 0000:01A6C       CUR'
        DB 'SOR_ON',13,10,' 0000:03940       DEACTIVATE_US'
        DB 'ER',13,10,' 0000:02CB0       DEBUG_SPIN_WAIT',13
        DB 10,' 0000:08114       DELETE_EXPRESSION',13,10
        DB ' 0000:07058       DELETE_LEX',13,10,' 0000:02C'
        DB 'BC       DELETE_LINKED_LIST',13,10,' 0000:026D'
        DB 'C       DRAW_BORDER',13,10,' 0000:02640       '
        DB 'DRAW_CHAR',13,10,' 1B06:000DF       END_BOOT_C'
        DB 'ODE',13,10,' 0000:03800       END_TASK',13,10
        DB ' 0000:02D80       END_TRAP',13,10,' 1B14:00165'
        DB '       ENTER_GRAPHICS_MODE',13,10,' 1B14:00014'
        DB '       ENTER_PMODE',13,10,' 0000:08B68       E'
        DB 'VAL_EXPRESSION',13,10,' 0000:02F8C       EXECU'
        DB 'TE_COMMAND_LINE',13,10,' 0000:01AC4       FILL'
        DB '_CRT',13,10,' 08BA:09034       FIL_FAT32_CPP',13
        DB 10,' 08BA:05FB0       FIL_KLUDGE_CPP',13,10,' '
        DB '08BA:0000C       FIL_OS_MAP',13,10,' 08BA:0344'
        DB '4       FIL_ROOT_CPP',13,10,' 08BA:1044C      '
        DB ' FIL_STRINGS_CPP',13,10,' 08BA:117B4       FIL'
        DB '_SYSTEXT_CPP',13,10,' 0000:07BDC       FIND_HA'
        DB 'SH_BUCKET',13,10,' 0000:07B8C       FIND_HASH_'
        DB 'ENTRY',13,10,' 0000:05D3C       FIND_MEMBER',13
        DB 10,' 0000:019CC       FIXED_PUT_HEX',13,10,' 0'
        DB '000:01971       FIXED_PUT_STRING',13,10,' 0000'
        DB ':04DD4       FIXUP_CODE',13,10,' 0000:035E4   '
        DB '    FREE',13,10,' 0000:03480       FREE_BLOCK_'
        DB 'LIST',13,10,' 0000:00000       GDTTAB',13,10
        DB ' 0000:0191C       GET_CHAR',13,10,' 0000:08B80'
        DB '       GET_EXPRESSION',13,10,' 0000:04D20     '
        DB '  GET_INTERMEDIATE_CODE',13,10,' 0000:04D54   '
        DB '    GET_INTERMEDIATE_CODE2',13,10,' 0000:06D50'
        DB '       GET_KEY',13,10,' 0000:01924       GET_S'
        DB 'TRING',13,10,' 0000:0279C       GR_UPDATE_SCRE'
        DB 'EN',13,10,' 0000:07B3C       HASH_STRING',13,10
        DB ' 0000:02D30       HEX_TO_BINARY',13,10,' 0000:'
        DB '0052C       IDTTAB',13,10,' 0000:00FF8       I'
        DB 'NIT_8259_AND_LIDT',13,10,' 0000:04D68       IN'
        DB 'IT_CB_IN_PTRS',13,10,' 0000:04CF0       INIT_C'
        DB 'B_OUT_PTRS',13,10,' 0000:01DC0       INIT_CRT',13
        DB 10,' 0000:033E0       INIT_MEMORY',13,10,' 000'
        DB '0:07BEC       INIT_SYSTEXT_AND_EXTERNS',13,10,' '
        DB '0000:02CD4       INSQUE',13,10,' 0000:02CE4   '
        DB '    INSQUE_REV',13,10,' 0000:010B8       IRQ_V'
        DB 'ECTORS',13,10,' 0000:01238       JIFFIES',13,10
        DB ' 0000:03984       KILL_ACTIVE_USER',13,10,' 00'
        DB '00:0383C       KILL_TASK',13,10,' 0000:073D0  '
        DB '     LEX',13,10,' 0000:07964       LEX_CB_APPE'
        DB 'ND',13,10,' 0000:07924       LEX_CB_POP',13,10
        DB ' 0000:07948       LEX_CB_POP_NO_FREE',13,10,' '
        DB '0000:078F8       LEX_CB_PUSH',13,10,' 0000:070'
        DB 'E8       LEX_GET_CHAR',13,10,' 0000:07314     '
        DB '  LEX_INCLUDE',13,10,' 0000:0739C       LEX_PO'
        DB 'P',13,10,' 0000:0735C       LEX_PUSH',13,10,' '
        DB '0000:073C8       LEX_SUB_STACK_PTR',13,10,' 00'
        DB '00:079DC       LOAD_TEXT_FILE',13,10,' 0000:03'
        DB '4BC       MALLOC',13,10,' 0000:033DC       MEM'
        DB 'ORY_FREE_LIST',13,10,' 0000:05D70       NEW_CL'
        DB 'ASS',13,10,' 0000:07B60       NEW_HASH_TABLE',13
        DB 10,' 0000:06FF0       NEW_LEX',13,10,' 0000:07'
        DB 'AE0       NEW_STRING',13,10,' 0000:06A88      '
        DB ' NEW_SYSFILES_STRING',13,10,' 0000:0574C      '
        DB ' OPTIMIZE_CODE',13,10,' 0000:08090       PARSE'
        DB '_ERROR',13,10,' 0000:067CC       PARSE_STATEME'
        DB 'NT',13,10,' 0000:02698       PLOT_CHAR',13,10
        DB ' 0000:04C56       PUT_ALL_IC',13,10,' 0000:019'
        DB '5C       PUT_CHAR',13,10,' 0000:04BCC       PU'
        DB 'T_CLASS',13,10,' 0000:01998       PUT_DEC',13
        DB 10,' 0000:019A5       PUT_HEX',13,10,' 0000:01'
        DB '9BF       PUT_HEX_BYTE',13,10,' 0000:019B2    '
        DB '   PUT_HEX_WORD',13,10,' 0000:04C10       PUT_'
        DB 'IC',13,10,' 0000:01964       PUT_STRING',13,10
        DB ' 0000:07C30       PUT_SYSTEXT',13,10,' 0000:07'
        DB '09C       PUT_TOKEN',13,10,' 0000:03080       '
        DB 'QSYSTEM_COMMAND_LINE',13,10,' 0000:02D5C      '
        DB ' REGISTER_TRAP',13,10,' 0000:02CF4       REMQU'
        DB 'E',13,10,' 0000:03870       REMQUE_TASK',13,10
        DB ' 0000:0379C       RESTORE_CONTEXT',13,10,' 000'
        DB '0:036A8       RFREE',13,10,' 0000:03680       '
        DB 'RMALLOC',13,10,' 0000:03694       RNEW_STRING',13
        DB 10,' 0000:031E4       ROOT_COMMAND_LINE',13,10
        DB ' 0000:00100       ROOT_TSS',13,10,' 0000:08B74'
        DB '       RUN_STATEMENT',13,10,' 0000:0372C      '
        DB ' SAVE_CONTEXT',13,10,' 0000:07C78       SPACE',13
        DB 10,' 0000:07C84       SPACES',13,10,' 0000:00D'
        DB 'E8       SPAWN_TASK',13,10,' 0000:038DC       '
        DB 'SPAWN_USER_AND_ACTIVATE',13,10,' 1B14:00000   '
        DB '    START',13,10,' 1B06:00000       START_BOOT'
        DB '_CODE',13,10,' 0000:07A38       STRLEN',13,10
        DB ' 0000:03890       SWAP_IN_NEXT',13,10,' 0000:0'
        DB '38B9       SWAP_IN_NEXT_PART2',13,10,' 0000:03'
        DB '0D8       SYSTEM_COMMAND_LINE',13,10,' 0000:07'
        DB '9B8       SYSTEM_EXTERN_TABLE',13,10,' 0000:03'
        DB '3D8       SYSTEM_MEMORY_HIGH',13,10,' 0000:033'
        DB 'D4       SYSTEM_MEMORY_LOW',13,10,' 0000:079B4'
        DB '       SYSTEM_STRING_TABLE',13,10,' 0000:00830'
        DB '       SYS_BOOT_CODE',13,10,' 0000:00834      '
        DB ' SYS_BOOT_IMAGE_END',13,10,' 0000:0082C       '
        DB 'SYS_BOOT_IMAGE_START',13,10,' 0000:06D6C      '
        DB ' SYS_BPT_LIST',13,10,' 0000:00880       SYS_CL'
        DB 'IPBOARD_LTF',13,10,' 0000:08BAC       SYS_CS_S'
        DB 'IZE',13,10,' 0000:06BA0       SYS_CURRENT_SCAN'
        DB '_CODE',13,10,' 0000:00840       SYS_FLAGS',13
        DB 10,' 0000:01E3C       SYS_FONT',13,10,' 0000:0'
        DB '0878       SYS_GRAPHICS_BASE',13,10,' 0000:008'
        DB '7C       SYS_GRAPHICS_HIDDEN',13,10,' 0000:008'
        DB '50       SYS_HASH_TABLE',13,10,' 0000:00854   '
        DB '    SYS_INTERNAL_TYPES',13,10,' 0000:0084C    '
        DB '   SYS_MEMORY_BASE',13,10,' 0000:00838       S'
        DB 'YS_TEMP_PTR',13,10,' 0000:00874       SYS_TEXT'
        DB '_BASE',13,10,' 0000:03318       TO_LINKED_TEXT'
        DB '_FILE',13,10,' 0000:06CF4       TRANSLATE_SCAN'
        DB '_CODE',13,10,' 0000:02D98       TRAP',13,10,' '
        DB '0000:036BC       TSK_FREEMEM',13,10,' 0000:010'
        DB '90       TSK_POLL_KEYBOARD',13,10,' 0000:079AC'
        DB '       ZEROS',13,10,13,10,'  Address        '
        DB ' Publics by Value',13,10,13,10,' 0000:08BAC '
        DB '      SYS_CS_SIZE',13,10,' 0000:08B80       GE'
        DB 'T_EXPRESSION',13,10,' 0000:08B74       RUN_STA'
        DB 'TEMENT',13,10,' 0000:08B68       EVAL_EXPRESSI'
        DB 'ON',13,10,' 0000:08AF4       COMPILE_EXPRESSIO'
        DB 'N',13,10,' 0000:0811C       CONT_PARSE_EXPRESS'
        DB 'ION',13,10,' 0000:08114       DELETE_EXPRESSIO'
        DB 'N',13,10,' 0000:08090       PARSE_ERROR',13,10
        DB ' 0000:07CCC       CMD_DEFINE_EXTERN',13,10,' 0'
        DB '000:07CAC       CMD_LOADSTR',13,10,' 0000:07C9'
        DB 'C       CMD_SPACES',13,10,' 0000:07C84       S'
        DB 'PACES',13,10,' 0000:07C78       SPACE',13,10
        DB ' 0000:07C64       CRLF',13,10,' 0000:07C30    '
        DB '   PUT_SYSTEXT',13,10,' 0000:07BEC       INIT_'
        DB 'SYSTEXT_AND_EXTERNS',13,10,' 0000:07BDC       '
        DB 'FIND_HASH_BUCKET',13,10,' 0000:07B8C       FIN'
        DB 'D_HASH_ENTRY',13,10,' 0000:07B60       NEW_HAS'
        DB 'H_TABLE',13,10,' 0000:07B3C       HASH_STRING',13
        DB 10,' 0000:07AFC       ADDS',13,10,' 0000:07AE0'
        DB '       NEW_STRING',13,10,' 0000:07ABC       CM'
        DB 'D_ATOI',13,10,' 0000:07A80       CMD_STRCMP',13
        DB 10,' 0000:07A68       CMD_STRCPY',13,10,' 0000'
        DB ':07A48       CMD_STRCAT',13,10,' 0000:07A38   '
        DB '    STRLEN',13,10,' 0000:079DC       LOAD_TEXT'
        DB '_FILE',13,10,' 0000:079B8       SYSTEM_EXTERN_'
        DB 'TABLE',13,10,' 0000:079B4       SYSTEM_STRING_'
        DB 'TABLE',13,10,' 0000:079AC       ZEROS',13,10
        DB ' 0000:07964       LEX_CB_APPEND',13,10,' 0000:'
        DB '07948       LEX_CB_POP_NO_FREE',13,10,' 0000:0'
        DB '7924       LEX_CB_POP',13,10,' 0000:078F8     '
        DB '  LEX_CB_PUSH',13,10,' 0000:073D0       LEX',13
        DB 10,' 0000:073C8       LEX_SUB_STACK_PTR',13,10
        DB ' 0000:0739C       LEX_POP',13,10,' 0000:0735C '
        DB '      LEX_PUSH',13,10,' 0000:07314       LEX_I'
        DB 'NCLUDE',13,10,' 0000:070E8       LEX_GET_CHAR',13
        DB 10,' 0000:0709C       PUT_TOKEN',13,10,' 0000:'
        DB '07058       DELETE_LEX',13,10,' 0000:06FF0    '
        DB '   NEW_LEX',13,10,' 0000:06D6C       SYS_BPT_L'
        DB 'IST',13,10,' 0000:06D50       GET_KEY',13,10
        DB ' 0000:06CF4       TRANSLATE_SCAN_CODE',13,10,' '
        DB '0000:06BA0       SYS_CURRENT_SCAN_CODE',13,10,' '
        DB '0000:06ABB       ADD_MAP_SYMBOLS',13,10,' 0000'
        DB ':06A88       NEW_SYSFILES_STRING',13,10,' 0000'
        DB ':06A24       COMPILE_STATEMENT',13,10,' 0000:0'
        DB '67CC       PARSE_STATEMENT',13,10,' 0000:05D70'
        DB '       NEW_CLASS',13,10,' 0000:05D3C       FIN'
        DB 'D_MEMBER',13,10,' 0000:05D04       CMD_GET_STR'
        DB 'ING',13,10,' 0000:05CF4       CMD_PUT_STRING',13
        DB 10,' 0000:05CF0       CMD_GET_CHAR',13,10,' 00'
        DB '00:05CE4       CMD_PUT_CHAR',13,10,' 0000:05CD'
        DB '4       CMD_STRLEN',13,10,' 0000:05CC0       C'
        DB 'MD_RNEW_STRING',13,10,' 0000:05CAC       CMD_N'
        DB 'EW_STRING',13,10,' 0000:05C98       CMD_LOAD_T'
        DB 'EXT_FILE',13,10,' 0000:05C6C       CMD_SLEEP',13
        DB 10,' 0000:05C4C       CMD_ADD_SYS_HASH',13,10,' '
        DB '0000:05C3C       CMD_DELETE_LINKED_LIST',13,10
        DB ' 0000:05C2C       CMD_CRT_LINE_OUT',13,10,' 00'
        DB '00:05C18       CMD_TO_LINKED_TEXT_FILE',13,10,' '
        DB '0000:05C08       CMD_KILL',13,10,' 0000:05BEC '
        DB '      CMD_FIND_HASH_ENTRY',13,10,' 0000:05BDC '
        DB '      CMD_FREE',13,10,' 0000:05BC8       CMD_M'
        DB 'ALLOC',13,10,' 0000:05BB8       CMD_RFREE',13
        DB 10,' 0000:05BA4       CMD_RMALLOC',13,10,' 000'
        DB '0:0574C       OPTIMIZE_CODE',13,10,' 0000:04DD'
        DB '4       FIXUP_CODE',13,10,' 0000:04D68       I'
        DB 'NIT_CB_IN_PTRS',13,10,' 0000:04D54       GET_I'
        DB 'NTERMEDIATE_CODE2',13,10,' 0000:04D20       GE'
        DB 'T_INTERMEDIATE_CODE',13,10,' 0000:04CF0       '
        DB 'INIT_CB_OUT_PTRS',13,10,' 0000:04C90       ADD'
        DB '_INTERMEDIATE_CODE',13,10,' 0000:04C56       P'
        DB 'UT_ALL_IC',13,10,' 0000:04C10       PUT_IC',13
        DB 10,' 0000:04BCC       PUT_CLASS',13,10,' 0000:'
        DB '04A93       COMPILE_CODE',13,10,' 0000:03D4C  '
        DB '     CODE_TEXT_TABLE',13,10,' 0000:039D4      '
        DB ' CMD_ACTIVATE_NEXT_USER',13,10,' 0000:03984   '
        DB '    KILL_ACTIVE_USER',13,10,' 0000:03940      '
        DB ' DEACTIVATE_USER',13,10,' 0000:0390C       ACT'
        DB 'IVATE_NEXT_USER',13,10,' 0000:038DC       SPAW'
        DB 'N_USER_AND_ACTIVATE',13,10,' 0000:038B9       '
        DB 'SWAP_IN_NEXT_PART2',13,10,' 0000:03890       S'
        DB 'WAP_IN_NEXT',13,10,' 0000:03870       REMQUE_T'
        DB 'ASK',13,10,' 0000:0383C       KILL_TASK',13,10
        DB ' 0000:03800       END_TASK',13,10,' 0000:0379C'
        DB '       RESTORE_CONTEXT',13,10,' 0000:0372C    '
        DB '   SAVE_CONTEXT',13,10,' 0000:03700       ADD_'
        DB 'TASK_TO_QUEUE',13,10,' 0000:036BC       TSK_FR'
        DB 'EEMEM',13,10,' 0000:036A8       RFREE',13,10
        DB ' 0000:03694       RNEW_STRING',13,10,' 0000:03'
        DB '680       RMALLOC',13,10,' 0000:035E4       FR'
        DB 'EE',13,10,' 0000:034BC       MALLOC',13,10,' '
        DB '0000:03480       FREE_BLOCK_LIST',13,10,' 0000'
        DB ':03418       ALLOCATE_MEMORY_BLOCKS',13,10,' 0'
        DB '000:033E0       INIT_MEMORY',13,10,' 0000:033D'
        DB 'C       MEMORY_FREE_LIST',13,10,' 0000:033D8  '
        DB '     SYSTEM_MEMORY_HIGH',13,10,' 0000:033D4   '
        DB '    SYSTEM_MEMORY_LOW',13,10,' 0000:03318     '
        DB '  TO_LINKED_TEXT_FILE',13,10,' 0000:031E4     '
        DB '  ROOT_COMMAND_LINE',13,10,' 0000:03190       '
        DB 'CMD_QUEUE_ROOT',13,10,' 0000:03144       CMD_R'
        DB 'OOT',13,10,' 0000:030D8       SYSTEM_COMMAND_L'
        DB 'INE',13,10,' 0000:03080       QSYSTEM_COMMAND_'
        DB 'LINE',13,10,' 0000:02FF4       COMMAND_LINE',13
        DB 10,' 0000:02F8C       EXECUTE_COMMAND_LINE',13,10
        DB ' 0000:02F30       CMD_SYSTEM',13,10,' 0000:02E'
        DB 'E8       CMD_QSYSTEM',13,10,' 0000:02EA8      '
        DB ' CMD_SPAWN',13,10,' 0000:02DC8       CMD_RAND',13
        DB 10,' 0000:02DB0       CMD_SRAND',13,10,' 0000:'
        DB '02D98       TRAP',13,10,' 0000:02D80       END'
        DB '_TRAP',13,10,' 0000:02D5C       REGISTER_TRAP',13
        DB 10,' 0000:02D30       HEX_TO_BINARY',13,10,' 0'
        DB '000:02D09       BINARY_TO_FIXED_HEX',13,10,' 0'
        DB '000:02D04       BINARY_TO_HEX',13,10,' 0000:02'
        DB 'CF4       REMQUE',13,10,' 0000:02CE4       INS'
        DB 'QUE_REV',13,10,' 0000:02CD4       INSQUE',13,10
        DB ' 0000:02CBC       DELETE_LINKED_LIST',13,10,' '
        DB '0000:02CB0       DEBUG_SPIN_WAIT',13,10,' 0000'
        DB ':02B64       CMD_GR_LINE',13,10,' 0000:02A34  '
        DB '     CMD_GR_PLOT',13,10,' 0000:02A1C       CMD'
        DB '_GR_CLEAR',13,10,' 0000:0279C       GR_UPDATE_'
        DB 'SCREEN',13,10,' 0000:026DC       DRAW_BORDER',13
        DB 10,' 0000:02698       PLOT_CHAR',13,10,' 0000:'
        DB '02640       DRAW_CHAR',13,10,' 0000:01E3C     '
        DB '  SYS_FONT',13,10,' 0000:01DC0       INIT_CRT',13
        DB 10,' 0000:01D34       CRT_LINE_OUT',13,10,' 00'
        DB '00:01C5C       CRT_OUT',13,10,' 0000:01B60    '
        DB '   CLEAR_CRT',13,10,' 0000:01AC4       FILL_CR'
        DB 'T',13,10,' 0000:01A6C       CURSOR_ON',13,10
        DB ' 0000:01A2C       CURSOR_OFF',13,10,' 0000:019'
        DB 'F8       CURSOR_HOME',13,10,' 0000:019CC      '
        DB ' FIXED_PUT_HEX',13,10,' 0000:019BF       PUT_H'
        DB 'EX_BYTE',13,10,' 0000:019B2       PUT_HEX_WORD'
        DB 13,10,' 0000:019A5       PUT_HEX',13,10,' 000'
        DB '0:01998       PUT_DEC',13,10,' 0000:01971     '
        DB '  FIXED_PUT_STRING',13,10,' 0000:01964       P'
        DB 'UT_STRING',13,10,' 0000:0195C       PUT_CHAR',13
        DB 10,' 0000:01924       GET_STRING',13,10,' 0000'
        DB ':0191C       GET_CHAR',13,10,' 0000:01238     '
        DB '  JIFFIES',13,10,' 0000:010B8       IRQ_VECTOR'
        DB 'S',13,10,' 0000:01090       TSK_POLL_KEYBOARD',13
        DB 10,' 0000:01058       CMD_REBOOT',13,10,' 0000'
        DB ':00FF8       INIT_8259_AND_LIDT',13,10,' 0000:'
        DB '00FB0       CREATE_TSS_SELECTOR',13,10,' 0000:'
        DB '00F68       CREATE_CODE_SELECTOR',13,10,' 0000'
        DB ':00F20       CREATE_MEMORY_SELECTOR',13,10,' 0'
        DB '000:00DE8       SPAWN_TASK',13,10,' 0000:00D73'
        DB '       ADD_USER_HASH_ENTRIES',13,10,' 0000:008'
        DB '80       SYS_CLIPBOARD_LTF',13,10,' 0000:0087C'
        DB '       SYS_GRAPHICS_HIDDEN',13,10,' 0000:00878'
        DB '       SYS_GRAPHICS_BASE',13,10,' 0000:00874  '
        DB '     SYS_TEXT_BASE',13,10,' 0000:00854       S'
        DB 'YS_INTERNAL_TYPES',13,10,' 0000:00850       SY'
        DB 'S_HASH_TABLE',13,10,' 0000:0084C       SYS_MEM'
        DB 'ORY_BASE',13,10,' 0000:00840       SYS_FLAGS',13
        DB 10,' 0000:00838       SYS_TEMP_PTR',13,10,' 00'
        DB '00:00834       SYS_BOOT_IMAGE_END',13,10,' 000'
        DB '0:00830       SYS_BOOT_CODE',13,10,' 0000:0082'
        DB 'C       SYS_BOOT_IMAGE_START',13,10,' 0000:005'
        DB '2C       IDTTAB',13,10,' 0000:00100       ROOT'
        DB '_TSS',13,10,' 0000:00000       GDTTAB',13,10
        DB ' 08BA:117B4       FIL_SYSTEXT_CPP',13,10,' 08B'
        DB 'A:1044C       FIL_STRINGS_CPP',13,10,' 08BA:09'
        DB '034       FIL_FAT32_CPP',13,10,' 08BA:05FB0   '
        DB '    FIL_KLUDGE_CPP',13,10,' 08BA:03444       F'
        DB 'IL_ROOT_CPP',13,10,' 08BA:0000C       FIL_OS_M'
        DB 'AP',13,10,' 1B06:000DF       END_BOOT_CODE',13
        DB 10,' 1B06:000D7       BOOT_IMG_BLOCK_LO',13,10
        DB ' 1B06:0005A       BOOT_CODE',13,10,' 1B06:0000'
        DB '0       START_BOOT_CODE',13,10,' 1B14:00165   '
        DB '    ENTER_GRAPHICS_MODE',13,10,' 1B14:00014   '
        DB '    ENTER_PMODE',13,10,' 1B14:00000       STAR'
        DB 'T',13,10,13,10,'Program entry point at 1B14:'
        DB '0000',13,10,'Warning: No stack',13,10,13,10
        DB 0
        ENDS
        END
