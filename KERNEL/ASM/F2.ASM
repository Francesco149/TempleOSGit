        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_ROOT_CPP
FIL_ROOT_CPP:

        DB '#define NULL 0',13,10,'#define true 1',13,10
        DB '#define false 0',13,10,'#define on   1',13,10
        DB '#define off  0',13,10,'#define bool DWORD',13
        DB 10,13,10,'//SYSFLAGS definitions',13,10,'#de'
        DB 'fine SYSF_PREEMPTIVE        0',13,10,'#define '
        DB 'SYSF_MEMORY_IN_USE     1',13,10,'#define SYSF_'
        DB 'ROOT_BUSY         2',13,10,'#define SYSF_ROOT_'
        DB 'DONE         3',13,10,'#define SYSF_IDE1_BUSY '
        DB '        4',13,10,'#define SYSF_IDE1_DONE      '
        DB '   5',13,10,'#define SYSF_ROOT_QUEUE_IN_USE 6',13
        DB 10,'#define SYSF_RAW_CRT_OUT       7 ',13,10,13
        DB 10,'//Extern calls',13,10,'#define ext_rblocks'
        DB '              1 ',13,10,'#define ext_wblocks  '
        DB '            2',13,10,'#define ext_irq0E       '
        DB '         3',13,10,'#define ext_dump           '
        DB '      4',13,10,'#define ext_read_file         '
        DB '   5',13,10,'#define ext_ltf_putchar          '
        DB '6',13,10,'#define ext_ltf_update_screen    7',13
        DB 10,'#define ext_blank_crt_text       8',13,10,'#'
        DB 'define ext_irq03                9',13,10,'#def'
        DB 'ine ext_irq04                10',13,10,'#defin'
        DB 'e ext_select_default_drive 11',13,10,13,10,'#'
        DB 'define HTT_INVALID             0',13,10,'#defi'
        DB 'ne HTT_ALL                 0xFFFFFFFF',13,10,'#'
        DB 'define HTT_FILE                1',13,10,'#defi'
        DB 'ne HTT_SYS_SYMBOL          2',13,10,'#define H'
        DB 'TT_LOCAL_VARIABLE      4',13,10,'#define HTT_G'
        DB 'LOBAL_VARIABLE     8',13,10,'#define HTT_CLASS'
        DB '               16',13,10,'#define HTT_KEYWORD '
        DB '            32',13,10,'#define HTT_INTERNAL_TY'
        DB 'PE       64',13,10,'#define HTT_FUNCTION      '
        DB '      128',13,10,'#define HTT_STRING_CONSTANT '
        DB '    256',13,10,13,10,'class sys_hash_entry ',13
        DB 10,'{',13,10,'  sys_hash_entry *next;',13,10
        DB '  char *string;',13,10,'  DWORD type;',13,10
        DB '  DWORD user0,user1,user2;',13,10,'};',13,10
        DB 13,10,'// TSS task flags',13,10,'#define TSSF'
        DB '_MALLOC_TRACE     0',13,10,'#define TSSF_HEAP_'
        DB 'IN_USE      1',13,10,'#define TSSF_LEX_TRACE  '
        DB '      2',13,10,'#define TSSF_PARSE_TRACE      '
        DB '3',13,10,'#define TSSF_COMPILE_TRACE    4',13
        DB 10,'#define TSSF_RUN_TRACE        5',13,10,'#d'
        DB 'efine TSSF_ECHO             6',13,10,'#define '
        DB 'TSSF_SUSPENDED        7',13,10,'#define TSSF_L'
        DB 'OCAL_USER       8',13,10,'#define TSSF_OPT_TRA'
        DB 'CE        9',13,10,13,10,'#define tss_desc_l'
        DB 'ength 31',13,10,'class tss_struct ',13,10,'{'
        DB 13,10,'  DWORD backlink;',13,10,'  DWORD esp0'
        DB ';',13,10,'  DWORD ss0;',13,10,'  DWORD esp1;'
        DB 13,10,'  DWORD ss1;',13,10,'  DWORD esp2;',13
        DB 10,'  DWORD ss2;',13,10,'  DWORD cr3;',13,10
        DB '  DWORD eip;',13,10,'  DWORD eflags;',13,10,' '
        DB ' DWORD eax;',13,10,'  DWORD ecx;',13,10,'  D'
        DB 'WORD edx;',13,10,'  DWORD ebx;',13,10,'  DWO'
        DB 'RD esp;',13,10,'  DWORD ebp;',13,10,'  DWORD'
        DB ' esi;',13,10,'  DWORD edi;',13,10,'  DWORD e'
        DB 's;',13,10,'  DWORD cs;',13,10,'  DWORD ss;',13
        DB 10,'  DWORD ds;',13,10,'  DWORD fs;',13,10,' '
        DB ' DWORD gs;',13,10,'  DWORD ldtr;',13,10,'  W'
        DB 'ORD  task_switch_trap;',13,10,'  WORD  io_bitm'
        DB 'ap_ptr;',13,10,'  BYTE  io_bitmap[0x800/8];',13
        DB 10,13,10,'  char  task_descriptor[tss_desc_len'
        DB 'gth+1];',13,10,13,10,'  DWORD *console_ltf;',13
        DB 10,'  DWORD *menu_ltf;',13,10,'  DWORD menu_se'
        DB 'lection;',13,10,'  DWORD cursor_offset;',13,10
        DB '  DWORD crt_size;',13,10,'  int   crt_width;',13
        DB 10,'  int   crt_height;',13,10,'  WORD  crt_se'
        DB 'lect;',13,10,'  BYTE  text_attribute;',13,10
        DB '  BYTE  border_attribute;',13,10,'  BYTE  menu'
        DB '_attribute;',13,10,'  BYTE  reserved_attribute'
        DB 's[3];',13,10,'  int   window_right;',13,10,' '
        DB ' int   window_left;',13,10,'  int   window_top'
        DB ';',13,10,'  int   window_bottom;',13,10,'  i'
        DB 'nt   cursor_hor;',13,10,'  int   cursor_vert;',13
        DB 10,'  WORD  crt_flags;',13,10,13,10,'  WORD '
        DB ' task_flags;',13,10,'  DWORD selector;',13,10
        DB '  tss_struct *absolute_address;',13,10,'  tss_'
        DB 'struct *parent_tss;',13,10,'  tss_struct *next'
        DB '_tss;',13,10,'  tss_struct *last_tss;',13,10
        DB '  DWORD memory_block_list;',13,10,'  BYTE  hea'
        DB 'p_cache[512];',13,10,'  DWORD malloc_free_list'
        DB ';',13,10,'  DWORD trap_list;',13,10,'  sys_h'
        DB 'ash_entry **hash_table;',13,10,'  char **strin'
        DB 'g_table;',13,10,'  DWORD optimization;',13,10
        DB '  char *startup_cmd;',13,10,'  DWORD system_re'
        DB 'sult;',13,10,'  DWORD current_partition;',13,10
        DB '  char *current_directory;',13,10,'  DWORD cur'
        DB 'rent_dir_cluster;',13,10,13,10,'  DWORD user'
        DB '_aux0;',13,10,'  DWORD user_aux1;',13,10,'  '
        DB 'DWORD user_aux2;',13,10,'  DWORD user_aux3;',13
        DB 10,'}',13,10,13,10,'//SYS_CURRENT_SCAN_CODE '
        DB 'FLAGS',13,10,'#define SCF_NEW_KEY             '
        DB '     16',13,10,'#define SCF_SHIFT             '
        DB '       8',13,10,'#define SCF_CTRL             '
        DB '        9',13,10,'#define SCF_ALT             '
        DB '         10',13,10,'#define SCF_WIN           '
        DB '           11',13,10,13,10,'#define CH_BACKS'
        DB 'PACE                 0x08',13,10,'#define SC_C'
        DB 'URSOR_UP                 0x48',13,10,'#define '
        DB 'SC_CURSOR_DOWN               0x50',13,10,'#def'
        DB 'ine SC_CURSOR_LEFT               0x4B',13,10,'#'
        DB 'define SC_CURSOR_RIGHT              0x4D',13,10
        DB '#define SC_PAGE_UP                   0x49',13,10
        DB '#define SC_PAGE_DOWN                 0x51',13,10
        DB '#define SC_DELETE                    0x53',13,10
        DB 13,10,'BYTE *internal 0x63 ds();  //Data segmen'
        DB 't',13,10,'BYTE *mz=-ds;   //Zero_based memory '
        DB 'array',13,10,'tss_struct *internal 0x3F fs();',13
        DB 10,'tss_struct *root_tss=fs;',13,10,13,10,'v'
        DB 'oid internal 0x40 cli();',13,10,'void internal'
        DB ' 0x41 sti();',13,10,'DWORD internal 0x42 get_f'
        DB 'lags();',13,10,'void internal 0x43  set_flags('
        DB 'DWORD d);',13,10,'DWORD internal 0x44 bt(DWORD'
        DB ' *bit_field,DWORD bit);',13,10,'DWORD internal'
        DB ' 0x45 bts(DWORD *bit_field,DWORD bit);',13,10,'D'
        DB 'WORD internal 0x46 btr(DWORD *bit_field,DWORD bi'
        DB 't);',13,10,'void internal 0x47 insque(DWORD *e'
        DB 'ntry,DWORD *pred);',13,10,'void internal 0x48 '
        DB 'insque_rev(DWORD *entry,DWORD *succ);',13,10,'v'
        DB 'oid internal 0x49 remque(DWORD *entry);',13,10
        DB 'BYTE internal 0x4A inp(WORD port);',13,10,'voi'
        DB 'd internal 0x4B outp(WORD port, BYTE val);',13,10
        DB 'void internal 0x4C rep_insb(BYTE *buf,DWORD coun'
        DB 't,WORD port);',13,10,'void internal 0x4D rep_i'
        DB 'nsw(BYTE *buf,DWORD count,WORD port);',13,10,'v'
        DB 'oid internal 0x4E rep_outsb(BYTE *buf,DWORD coun'
        DB 't,WORD port);',13,10,'void internal 0x4F rep_o'
        DB 'utsw(BYTE *buf,DWORD count,WORD port);',13,10,'v'
        DB 'oid internal 0x50 memcpy(BYTE *dst,BYTE *src,DWO'
        DB 'RD count);',13,10,'void internal 0x51 memset(B'
        DB 'YTE *dst,BYTE val,DWORD count);',13,10,'WORD i'
        DB 'nternal 0x52 inpw(WORD port);',13,10,'void int'
        DB 'ernal 0x53 outpw(WORD port, WORD val);',13,10,'D'
        DB 'WORD internal 0x5E inpd(WORD port);',13,10,'vo'
        DB 'id internal 0x5F outpd(WORD port, DWORD val);',13
        DB 10,'void sysfun CMD_DEFINE_EXTERN defext(DWORD i'
        DB ', DWORD VAL);',13,10,'BYTE * sysfun CMD_MALLOC'
        DB ' malloc(DWORD size);',13,10,'void sysfun CMD_F'
        DB 'REE     free(DWORD *add);',13,10,'BYTE *sysfun'
        DB ' CMD_RMALLOC rmalloc(DWORD size);',13,10,'void'
        DB ' sysfun CMD_RFREE    rfree(DWORD *add);',13,10
        DB 'char *sysfun CMD_NEW_STRING  new_string(char *bu'
        DB 'f);',13,10,'char *sysfun CMD_RNEW_STRING rnew_'
        DB 'string(char *buf);',13,10,'DWORD sysfun CMD_ST'
        DB 'RLEN  strlen(char *st);',13,10,'int sysfun CMD'
        DB '_STRCMP    strcmp(char *st1,char *st2);',13,10
        DB 'void sysfun CMD_STRCPY   strcpy(char *dest,char '
        DB '*src);',13,10,'void sysfun CMD_STRCAT   strcat'
        DB '(char *dest,char *src);',13,10,'void sysfun CM'
        DB 'D_SPACES   spaces(DWORD cnt);',13,10,'int sysf'
        DB 'un CMD_ATOI atoi(char *st);',13,10,'void sysfu'
        DB 'n CMD_PUT_CHAR putchar(char ch);',13,10,'void '
        DB 'sysfun CMD_PUT_STRING puts(char *st);',13,10,'c'
        DB 'har sysfun CMD_GET_CHAR getchar();',13,10,'cha'
        DB 'r sysfun GET_KEY      getkey(); //no echo',13,10
        DB 13,10,'void assign_bit(DWORD *field,DWORD bit,b'
        DB 'ool val)',13,10,'{',13,10,'  if (val)',13,10
        DB '    bts(field,bit);',13,10,'  else',13,10,' '
        DB '   btr(field,bit);',13,10,'}',13,10,13,10,'v'
        DB 'oid mtrace(bool val)',13,10,'  assign_bit(&fs-'
        DB '>task_flags,TSSF_MALLOC_TRACE,val);',13,10,'vo'
        DB 'id ltrace(bool val)',13,10,'  assign_bit(&fs->'
        DB 'task_flags,TSSF_LEX_TRACE,val);',13,10,'void p'
        DB 'trace(bool val)',13,10,'  assign_bit(&fs->task'
        DB '_flags,TSSF_PARSE_TRACE,val);',13,10,'void otr'
        DB 'ace(bool val)',13,10,'  assign_bit(&fs->task_f'
        DB 'lags,TSSF_OPT_TRACE,val);',13,10,'void ctrace('
        DB 'bool val)',13,10,'  assign_bit(&fs->task_flags'
        DB ',TSSF_COMPILE_TRACE,val);',13,10,'void rtrace('
        DB 'bool val)',13,10,'  assign_bit(&fs->task_flags'
        DB ',TSSF_RUN_TRACE,val);',13,10,'void echo(bool v'
        DB 'al)',13,10,'  assign_bit(&fs->task_flags,TSSF_'
        DB 'ECHO,val);',13,10,'void preempt(bool val)',13
        DB 10,'  assign_bit(SYS_FLAGS,SYSF_PREEMPTIVE,val);'
        DB 13,10,13,10,'void sysfun SWAP_IN_NEXT swap_in'
        DB '_next();',13,10,'void sysfun CMD_ADD_SYS_HASH '
        DB 'add_sys_hash(sys_hash_entry *e);',13,10,'sys_h'
        DB 'ash_entry * sysfun CMD_FIND_HASH_ENTRY ',13,10
        DB '  find_hash_entry(char *st,sys_hash_entry **tabl'
        DB 'e,DWORD mask);',13,10,'sys_hash_entry *find_sy'
        DB 's_hash(char *st,DWORD mask)',13,10,'{',13,10
        DB '  return find_hash_entry(st,*SYS_HASH_TABLE,mask'
        DB ');',13,10,'}',13,10,13,10,'DWORD sysfun CM'
        DB 'D_ROOT    root(char *cmd);          //Run cmd as'
        DB ' root',13,10,'void sysfun CMD_QUEUE_ROOT qroot'
        DB '(char *cmd);       //Queue cmd for root  ',13,10
        DB 'DWORD sysfun CMD_SYSTEM system(char *cmd);      '
        DB '   //system() cmd',13,10,'tss_struct *sysfun C'
        DB 'MD_QSYSTEM qsystem(char *cmd); //Queue system() '
        DB 'cmd',13,10,'char sysfun TRANSLATE_SCAN_CODE tr'
        DB 'anslate_scan_code(); //no echo',13,10,'bool kb'
        DB 'hit()  //Not clears flag',13,10,'  return btr('
        DB 'SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);',13,10,'vo'
        DB 'id sysfun CMD_GET_STRING gets(char *buf,DWORD si'
        DB 'ze);',13,10,'char *sysfun CMD_LOAD_TEXT_FILE l'
        DB 'oad_text_file(char *filename);',13,10,'void sy'
        DB 'sfun CMD_SLEEP    sleep(DWORD mS);',13,10,'voi'
        DB 'd sysfun CMD_REBOOT   reboot();',13,10,'void s'
        DB 'ysfun CRLF         crlf();',13,10,'void sysfun'
        DB ' SPACE        space();',13,10,'void sysfun INI'
        DB 'T_CRT     init_crt();',13,10,'void sysfun CLEA'
        DB 'R_CRT    clear_crt();',13,10,'void sysfun CMD_'
        DB 'CRT_LINE_OUT crt_line_out(char *st);',13,10,'v'
        DB 'oid sysfun CURSOR_HOME  cursor_home();',13,10,'v'
        DB 'oid sysfun CURSOR_ON    cursor_on();',13,10,'v'
        DB 'oid sysfun CURSOR_OFF   cursor_off();',13,10,'v'
        DB 'oid sysfun CMD_DELETE_LINKED_LIST delete_linked_'
        DB 'list(DWORD *list);',13,10,'void sysfun CMD_LOA'
        DB 'DSTR  loadstr(DWORD i, char *st);  ',13,10,'vo'
        DB 'id sysfun DEACTIVATE_USER deactivate_user();',13
        DB 10,'void sysfun CMD_ACTIVATE_NEXT_USER activate_'
        DB 'next_user();',13,10,'void sysfun SPAWN_USER_AN'
        DB 'D_ACTIVATE spawn_user_and_activate();',13,10,13
        DB 10,'tss_struct *sysfun CMD_SPAWN spawn(BYTE *sta'
        DB 'rt_add,char *desc="GENERIC"); //Spawn Task  ',13
        DB 10,'void sysfun CMD_KILL  kill(tss_struct *task)'
        DB '; //Kill Task  ',13,10,'//Note: "void exit()" '
        DB 'is a keyword',13,10,13,10,'#define screen_wi'
        DB 'dth 800',13,10,'#define screen_height 600',13
        DB 10,'#define font_width  8',13,10,'#define font'
        DB '_height 8',13,10,'void sysfun CMD_GR_CLEAR gr_'
        DB 'clear();',13,10,'void sysfun CMD_GR_PLOT  gr_p'
        DB 'lot(DWORD x,DWORD y,DWORD color);',13,10,'void'
        DB ' sysfun CMD_GR_LINE  gr_line(DWORD x1,DWORD y1,D'
        DB 'WORD x2,DWORD y2,DWORD color);',13,10,'#define'
        DB ' MAX_RAND 0x7FFF',13,10,'void sysfun CMD_SRAND'
        DB ' srand(WORD v);',13,10,'void sysfun CMD_RAND r'
        DB 'and();',13,10,'int abs(int i) ',13,10,'{',13
        DB 10,'  if (i<0)',13,10,'    return -i;',13,10
        DB '  else',13,10,'    return i;',13,10,'}',13,10
        DB 13,10,'int sign(int i)',13,10,'{',13,10,'  '
        DB 'if (i<0)',13,10,'    return -1;',13,10,'  el'
        DB 'se if (!i)',13,10,'    return 0;',13,10,'  e'
        DB 'lse',13,10,'    return 1;',13,10,'}',13,10
        DB 13,10,'#include "FIL_SYSTEXT_CPP"',13,10,';',13
        DB 10,'load_sys_strings;',13,10,13,10,'#include'
        DB ' "FIL_STRINGS_CPP"',13,10,'#include "FIL_FAT32'
        DB '_CPP"',13,10,'read_partition_info;',13,10,'s'
        DB 'elect_default_drive;',13,10,13,10,'cd_FAT32('
        DB '"HOPPY");',13,10,'cd_FAT32("KERNEL");',13,10
        DB 'cd_FAT32("CPP");',13,10,'free(fs->current_dire'
        DB 'ctory);',13,10,'fs->current_directory=new_stri'
        DB 'ng("/HOPPY/KERNEL/CPP");',13,10,13,10,13,10
        DB '#include "DISK.CPP"',13,10,'#include "ROOT2.CP'
        DB 'P"',13,10
        DB 0
        ENDS
        END
