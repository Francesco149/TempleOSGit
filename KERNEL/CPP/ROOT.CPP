#define NULL 0
#define true 1
#define false 0
#define on   1
#define off  0
#define bool DWORD

//SYSFLAGS definitions
#define SYSF_PREEMPTIVE        0
#define SYSF_MEMORY_IN_USE     1
#define SYSF_ROOT_BUSY         2
#define SYSF_ROOT_DONE         3
#define SYSF_IDE1_BUSY         4
#define SYSF_IDE1_DONE         5
#define SYSF_ROOT_QUEUE_IN_USE 6
#define SYSF_RAW_CRT_OUT       7 

//Extern calls
#define ext_rblocks              1 
#define ext_wblocks              2
#define ext_irq0E                3
#define ext_dump                 4
#define ext_read_file            5
#define ext_ltf_putchar          6
#define ext_blank_crt_text       7
#define ext_irq03                8
#define ext_irq04                9
#define ext_select_default_drive 10

#define HTT_INVALID             0
#define HTT_ALL                 0xFFFFFFFF
#define HTT_FILE                1
#define HTT_SYS_SYMBOL          2
#define HTT_LOCAL_VARIABLE      4
#define HTT_GLOBAL_VARIABLE     8
#define HTT_CLASS               16
#define HTT_KEYWORD             32
#define HTT_INTERNAL_TYPE       64
#define HTT_FUNCTION            128
#define HTT_STRING_CONSTANT     256

class sys_hash_entry 
{
  sys_hash_entry *next;
  char *string;
  DWORD type;
  DWORD user0,user1,user2;
};

#define mlf_default_available 1
#define mlf_function          2
class member_list_node
{
  member_list_node *next;
  char *string;
  DWORD *member_class;
  DWORD count; //for arrays
  DWORD offset;
  WORD parameter_count;
  WORD flags;
  DWORD default_value;
  char *format_str;
  char *format_data;
  void output_fun(char *buf,BYTE *d);
  void input_fun(char *buf,BYTE *d);
};

#define cf_internal 0
class class_node
{
  sys_hash_entry *next;
  char *string;
  DWORD type;
  WORD member_count;
  WORD parameter_count;
  DWORD size;
  WORD dereference_code;
  WORD assign_code;
  member_list_node *member_list;
  member_list_node *last_in_member_list;
  DWORD *executable_address;
  class_node *return_class;
  BYTE pointer_count;
  BYTE flags[3];
};

#define it_char    0
#define it_BYTE    1
#define it_short   2
#define it_WORD    3
#define it_int     4
#define it_DWORD   5
#define it_double  6
#define it_udouble 7

class global_variable_node
{
  sys_hash_entry *next;
  char *string;
  DWORD type;
  class_node *variable_class;
  WORD flags;
  WORD parameter_count;
  DWORD data;
};

// TSS task flags
#define TSSF_MALLOC_TRACE     0
#define TSSF_HEAP_IN_USE      1
#define TSSF_LEX_TRACE        2
#define TSSF_PARSE_TRACE      3
#define TSSF_COMPILE_TRACE    4
#define TSSF_RUN_TRACE        5
#define TSSF_ECHO             6
#define TSSF_SUSPENDED        7
#define TSSF_LOCAL_USER       8
#define TSSF_OPT_TRACE        9

#define CRTF_CURSOR_ON        0
#define CRTF_WRAP_AROUND      1
#define CRTF_SHOW             2

#define tss_desc_length 31
class tss_struct 
{
  DWORD backlink;
  DWORD esp0;
  DWORD ss0;
  DWORD esp1;
  DWORD ss1;
  DWORD esp2;
  DWORD ss2;
  DWORD cr3;
  DWORD eip;
  DWORD eflags;
  DWORD eax;
  DWORD ecx;
  DWORD edx;
  DWORD ebx;
  DWORD esp;
  DWORD ebp;
  DWORD esi;
  DWORD edi;
  DWORD es;
  DWORD cs;
  DWORD ss;
  DWORD ds;
  DWORD fs;
  DWORD gs;
  DWORD ldtr;
  WORD  task_switch_trap;
  WORD  io_bitmap_ptr;
  BYTE  io_bitmap[0x800/8];

  char  task_descriptor[tss_desc_length+1];

  void  update_window();
  DWORD *current_ltf;
  DWORD *menu_ltf;
  DWORD menu_selection;
  DWORD cursor_offset;
  DWORD crt_size;
  int   crt_width;
  int   crt_height;
  WORD  crt_select;
  BYTE  text_attribute;
  BYTE  border_attribute;
  BYTE  menu_attribute;
  BYTE  reserved_attributes[3];
  int   window_left;
  int   window_right;
  int   window_top;
  int   window_bottom;
  int   cursor_hor;
  int   cursor_vert;

  int   window_pixel_left;  //These are derived
  int   window_pixel_right;
  int   window_pixel_top;
  int   window_pixel_bottom;

  WORD  crt_flags;

  WORD  task_flags;
  DWORD selector;
  tss_struct *absolute_address;
  tss_struct *parent_tss;
  tss_struct *next_tss;
  tss_struct *last_tss;
  DWORD memory_block_list;
  BYTE  heap_cache[512];
  DWORD malloc_free_list;
  DWORD trap_list;
  sys_hash_entry **hash_table;
  char **string_table;
  DWORD optimization;
  char *startup_cmd;
  DWORD system_result;
  DWORD current_partition;
  char *current_directory;
  DWORD current_dir_cluster;

  DWORD user_aux0;
  DWORD user_aux1;
  DWORD user_aux2;
  DWORD user_aux3;
}

//SYS_CURRENT_SCAN_CODE FLAGS
#define SCF_NEW_KEY                  16
#define SCF_SHIFT                    8
#define SCF_CTRL                     9
#define SCF_ALT                      10
#define SCF_WIN                      11

#define CH_BACKSPACE                 0x08
#define SC_CURSOR_UP                 0x48
#define SC_CURSOR_DOWN               0x50
#define SC_CURSOR_LEFT               0x4B
#define SC_CURSOR_RIGHT              0x4D
#define SC_PAGE_UP                   0x49
#define SC_PAGE_DOWN                 0x51
#define SC_DELETE                    0x53

BYTE *internal 0x63 ds();  //Data segment
BYTE *mz=-ds;   //Zero_based memory array
tss_struct *internal 0x3F fs();
tss_struct *root_tss=fs;

void internal 0x40 cli();
void internal 0x41 sti();
DWORD internal 0x42 get_flags();
void internal 0x43  set_flags(DWORD d);
DWORD internal 0x44 bt(DWORD *bit_field,DWORD bit);
DWORD internal 0x45 bts(DWORD *bit_field,DWORD bit);
DWORD internal 0x46 btr(DWORD *bit_field,DWORD bit);
void internal 0x47 insque(DWORD *entry,DWORD *pred);
void internal 0x48 insque_rev(DWORD *entry,DWORD *succ);
void internal 0x49 remque(DWORD *entry);
BYTE internal 0x4A inp(WORD port);
void internal 0x4B outp(WORD port, BYTE val);
void internal 0x4C rep_insb(BYTE *buf,DWORD count,WORD port);
void internal 0x4D rep_insw(BYTE *buf,DWORD count,WORD port);
void internal 0x4E rep_outsb(BYTE *buf,DWORD count,WORD port);
void internal 0x4F rep_outsw(BYTE *buf,DWORD count,WORD port);
void internal 0x50 memcpy(BYTE *dst,BYTE *src,DWORD count);
void internal 0x51 memset(BYTE *dst,BYTE val,DWORD count);
WORD internal 0x52 inpw(WORD port);
void internal 0x53 outpw(WORD port, WORD val);
DWORD internal 0x5E inpd(WORD port);
void internal 0x5F outpd(WORD port, DWORD val);
void sysfun CMD_DEFINE_EXTERN defext(DWORD i, DWORD VAL);
BYTE * sysfun CMD_MALLOC malloc(DWORD size);
void sysfun CMD_FREE     free(DWORD *add);
BYTE *sysfun CMD_RMALLOC rmalloc(DWORD size);
void sysfun CMD_RFREE    rfree(DWORD *add);
char *sysfun CMD_NEW_STRING  new_string(char *buf);
char *sysfun CMD_RNEW_STRING rnew_string(char *buf);
DWORD sysfun CMD_STRLEN  strlen(char *st);
int sysfun CMD_STRCMP    strcmp(char *st1,char *st2);
void sysfun CMD_STRCPY   strcpy(char *dest,char *src);
void sysfun CMD_STRCAT   strcat(char *dest,char *src);
void sysfun CMD_SPACES   spaces(DWORD cnt);
int sysfun CMD_ATOI atoi(char *st);
void sysfun CMD_PUT_CHAR putchar(char ch);
void sysfun CMD_PUT_STRING puts(char *st);
char sysfun CMD_GET_CHAR getchar();
char sysfun GET_KEY      getkey(); //no echo

void assign_bit(DWORD *field,DWORD bit,bool val)
{
  if (val)
    bts(field,bit);
  else
    btr(field,bit);
}

void mtrace(bool val)
  assign_bit(&fs->task_flags,TSSF_MALLOC_TRACE,val);
void ltrace(bool val)
  assign_bit(&fs->task_flags,TSSF_LEX_TRACE,val);
void ptrace(bool val)
  assign_bit(&fs->task_flags,TSSF_PARSE_TRACE,val);
void otrace(bool val)
  assign_bit(&fs->task_flags,TSSF_OPT_TRACE,val);
void ctrace(bool val)
  assign_bit(&fs->task_flags,TSSF_COMPILE_TRACE,val);
void rtrace(bool val)
  assign_bit(&fs->task_flags,TSSF_RUN_TRACE,val);
void echo(bool val)
  assign_bit(&fs->task_flags,TSSF_ECHO,val);
void preempt(bool val)
  assign_bit(SYS_FLAGS,SYSF_PREEMPTIVE,val);

void sysfun SWAP_IN_NEXT swap_in_next();
void sysfun CMD_ADD_SYS_HASH add_sys_hash(sys_hash_entry *e);
sys_hash_entry * sysfun CMD_FIND_HASH_ENTRY 
  find_hash_entry(char *st,sys_hash_entry **table,DWORD mask);
sys_hash_entry *find_sys_hash(char *st,DWORD mask)
{
  return find_hash_entry(st,*SYS_HASH_TABLE,mask);
}

sys_hash_entry *find_local_hash(char *st,DWORD mask)
{
  return find_hash_entry(st,fs->hash_table,mask);
}

DWORD sysfun CMD_ROOT    root(char *cmd);          //Run cmd as root
void sysfun CMD_QUEUE_ROOT qroot(char *cmd);       //Queue cmd for root  
DWORD sysfun CMD_SYSTEM system(char *cmd);         //system() cmd
tss_struct *sysfun CMD_QSYSTEM qsystem(char *cmd); //Queue system() cmd
char sysfun TRANSLATE_SCAN_CODE translate_scan_code(); //no echo
bool kbhit()  //Not clears flag
  return btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
void sysfun CMD_GET_STRING gets(char *buf,DWORD size);
char *sysfun CMD_LOAD_TEXT_FILE load_text_file(char *filename);
void sysfun CMD_SLEEP    sleep(DWORD mS);
void sysfun CMD_REBOOT   reboot();
void sysfun CRLF         crlf();
void sysfun SPACE        space();
void sysfun INIT_CRT     init_crt();
void sysfun CLEAR_CRT    clear_crt();
void sysfun CMD_CRT_LINE_OUT crt_line_out(char *st);
void sysfun CURSOR_HOME  cursor_home();
void sysfun CURSOR_ON    cursor_on();
void sysfun CURSOR_OFF   cursor_off();
void sysfun CMD_DELETE_LINKED_LIST delete_linked_list(DWORD *list);
void sysfun CMD_LOADSTR  loadstr(DWORD i, char *st);  
void sysfun DEACTIVATE_USER deactivate_user();
void sysfun CMD_ACTIVATE_NEXT_USER activate_next_user();
void sysfun SPAWN_USER_AND_ACTIVATE spawn_user_and_activate();

tss_struct *sysfun CMD_SPAWN spawn(BYTE *start_add,char *desc="GENERIC"); //Spawn Task  
void sysfun CMD_KILL  kill(tss_struct *task); //Kill Task  
//Note: "void exit()" is a keyword

#define BLACK      0
#define BLUE       1
#define GREEN      2
#define CYAN       3
#define RED        4
#define PURPLE     5
#define BROWN      6
#define LTGRAY     7
#define DKGRAY     8
#define LTBLUE     9
#define LTGREEN    10
#define LTCYAN     11
#define LTRED      12
#define LTPURPLE   13
#define YELLOW     14
#define WHITE      15

#define screen_width 800
#define screen_height 600
#define font_width  8
#define font_height 8
void sysfun GR_UPDATE_SCREEN gr_update_screen();
void sysfun CMD_GR_CLEAR gr_clear();
void sysfun CMD_GR_PLOT  gr_plot(DWORD x,DWORD y,DWORD color);
void sysfun CMD_GR_LINE  gr_line(DWORD x1,DWORD y1,DWORD x2,DWORD y2,DWORD color);
#define MAX_RAND 0x7FFF
void sysfun CMD_SRAND srand(WORD v);
void sysfun CMD_RAND rand();
int abs(int i) 
{
  if (i<0)
    return -i;
  else
    return i;
}

int sign(int i)
{
  if (i<0)
    return -1;
  else if (!i)
    return 0;
  else
    return 1;
}

#include "FIL_SYSTEXT_CPP"
;
load_sys_strings;

#include "FIL_STRINGS_CPP"
#include "FIL_FAT32_CPP"
read_partition_info;
select_default_drive;

cd_FAT32("HOPPY");
cd_FAT32("KERNEL");
cd_FAT32("CPP");
free(fs->current_directory);
fs->current_directory=new_string("/HOPPY/KERNEL/CPP");


#include "DISK.CPP"
#include "ROOT2.CPP"
