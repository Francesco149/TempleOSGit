#define NULL 0
#define true 1
#define false 0
#define on   1
#define off  0
#define bool DWORD

//SYSFLAGS definitions
#define SYSF_PREEMPTIVE         0
#define SYSF_CTRL_ALT_DEL       1
//Unused
//Unused
#define SYSF_IDE1_BUSY	        4
#define SYSF_IDE1_DONE	        5
//Unused
#define SYSF_RAW_CRT_OUT        7
#define SYSF_FDC0_BUSY	        8
#define SYSF_FDC0_DONE	        9
#define SYSF_NON_VGA	        10
#define SYSF_CTRL_ALT_ESC       11
#define SYSF_CTRL_ALT_X         12
#define SYSF_CTRL_ALT_TAB       13
#define SYSF_TASK_LINKS_ALTERED 14


//Extern calls
#define ext_rblocks		 1
#define ext_wblocks		 2
#define ext_irq0E		 3
#define ext_dump		 4
#define ext_read_file		 5
#define ext_ltf_putchar 	 6
#define ext_blank_crt_text	 7
#define ext_irq03		 8
#define ext_irq04		 9
#define ext_select_default_drive 10
#define ext_irq06		 11
#define ext_window_to_top        12
#define ext_irq0C                13

#define HTT_INVALID		0
#define HTT_ALL 		0xFFFFFFFF
#define HTT_STRING_CONSTANT	1
#define HTT_SYS_SYMBOL		2
#define HTT_KEYWORD		4
#define HTT_LOCAL_VARIABLE	8
#define HTT_GLOBAL_VARIABLE	16
#define HTT_CLASS		32
#define HTT_INTERNAL_TYPE	64
#define HTT_FUNCTION		128
#define HTT_FILE		256
#define HTT_UNRESOLVED_LOCAL	0x40000000
#define HTT_UNRESOLVED_GLOBAL	0x80000000

class sys_hash_entry
{
  sys_hash_entry *next;
  char *string;
  DWORD type;
  DWORD user0,user1,user2;
};

class sys_hash_table
{
  sys_hash_table *next;
  DWORD mask;
  sys_hash_entry **body;
};

#define mlf_default_available 1
#define mlf_function	      2
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
#define it_int	   4
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


#define TSSCF_WAKE  0
class tss_cmd_node
{
  tss_cmd_node *next,*last;
  DWORD flags;
  char *string;
  DWORD result;
  DWORD master_tss;
};

// TSS task flags
#define TSSF_MALLOC_TRACE     0
#define TSSF_LEX_TRACE	      1
#define TSSF_PARSE_TRACE      2
#define TSSF_COMPILE_TRACE    3
#define TSSF_RUN_TRACE	      4
#define TSSF_ECHO	      5
#define TSSF_SUSPENDED	      6
#define TSSF_LOCAL_USER       7
#define TSSF_OPT_TRACE	      8
#define TSSF_PREEMPT 	      9
#define TSSF_FOCUS	      10
#define TSSF_KILL_TASK        11

#define CRTF_CURSOR_ON	        0
#define CRTF_WRAP_AROUND        1
#define CRTF_SHOW	        2
#define CRTF_HAS_MENU           3

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
  DWORD eip      format_str "EIP   :%08X";
  DWORD eflags   format_str "EFLAGS:%08X";  
  DWORD eax      format_str "EAX   :%08X";
  DWORD ecx      format_str "ECX   :%08X";
  DWORD edx      format_str "EDX   :%08X";  
  DWORD ebx      format_str "EBX   :%08X";
  DWORD esp      format_str "ESP   :%08X";
  DWORD ebp      format_str "EBP   :%08X";
  DWORD esi      format_str "ESI   :%08X";
  DWORD edi      format_str "EDI   :%08X";
  DWORD es;
  DWORD cs;
  DWORD ss;
  DWORD ds;
  DWORD fs;
  DWORD gs;
  DWORD ldtr;
  WORD	task_switch_trap;
  WORD	io_bitmap_ptr;
  BYTE	io_bitmap[0x800/8];

  char	task_descriptor[tss_desc_length+1];

  void	update_window();
  DWORD *current_ltf;
  DWORD cursor_offset;
  DWORD crt_size;
  int	crt_width;
  int	crt_height;
  WORD	crt_select;
  BYTE	text_attribute;
  BYTE	border_attribute;
  int	window_left;
  int	window_right;
  int	window_top;
  int	window_bottom;
  int	cursor_hor;
  int	cursor_vert;

  int	window_pixel_left;  //These are derived
  int	window_pixel_right;
  int	window_pixel_top;
  int	window_pixel_bottom;

  WORD	crt_flags;

  WORD	task_flags;
//  DWORD selector;
  tss_struct *absolute_address;
  tss_struct *parent_tss;
  tss_struct *next_tss;
  tss_struct *last_tss;
  DWORD memory_block_list;
  BYTE	heap_cache[512];
  DWORD malloc_free_list;
  DWORD trap_list;
  sys_hash_table *hash_table;
  char **string_table;
  DWORD optimization;
  tss_cmd_node *next_slave_cmd,*last_slave_cmd;
  tss_cmd_node *next_master_cmd,*last_master_cmd;
  DWORD current_partition;
  char *current_directory;
  DWORD current_dir_cluster;

  DWORD user_aux0;
  DWORD user_aux1;
  DWORD user_aux2;
  DWORD user_aux3;
}

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
#define SC_ESC			     0x01
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


BYTE *internal 0x63 ds();  //Data segment
BYTE *mz=-ds;	//Zero_based memory array
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
void port_nop() {outp(0xEB,0);}
void sysfun DEBUG_SPIN_WAIT debug_spin_wait();
void sysfun CMD_DEFINE_EXTERN defext(DWORD i, DWORD VAL);
BYTE * sysfun CMD_MALLOC malloc(DWORD size);
void sysfun CMD_FREE	 free(DWORD *add);
BYTE *sysfun CMD_RMALLOC rmalloc(DWORD size);
void sysfun CMD_RFREE	 rfree(DWORD *add);
DWORD sysfun CALCULATE_FREE_SYS_MEMORY free_sys_memory();
char *sysfun CMD_NEW_STRING  new_string(char *buf);
char *sysfun CMD_RNEW_STRING rnew_string(char *buf);
DWORD sysfun CMD_STRLEN  strlen(char *st);
int sysfun CMD_STRCMP	 strcmp(char *st1,char *st2);
char *sysfun CMD_STRSTR  strstr(char *st,char *pattern);
char *sysfun CMD_STRISTR stristr(char *st,char *pattern);
void sysfun CMD_STRCPY	 strcpy(char *dest,char *src);
void sysfun CMD_STRCAT	 strcat(char *dest,char *src);
char toupper(char ch) {if (ch>='a' && ch<='z')
  return ch+'A'-'a'; else return ch;}
void sysfun CMD_SPACES	 spaces(DWORD cnt);
int sysfun CMD_ATOI atoi(char *st);
void sysfun CMD_PUT_CHAR putchar(char ch);
void sysfun CMD_PUT_STRING puts(char *st);
char sysfun CMD_GET_CHAR getchar();
char sysfun GET_KEY	 getkey(); //no echo

bts(&fs->task_flags,TSSF_FOCUS);  //Temporary

bool y_or_n()
{
  char ch;
  ? "(y or n)? ";
  while (true) {
    ch=toupper(getkey);
    if (ch=='Y') {
      ? "YES\r\n";
      return true;
    } else if (ch=='N') {
      ? "NO\r\n";
      return false;
    }
  }
}

bool assign_bit(DWORD *field,DWORD bit,bool val)
{
  bool result;
  if (val)
    result=bts(field,bit);
  else
    result=btr(field,bit);
  return result;
}

bool mtrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_MALLOC_TRACE,val);
}
bool ltrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_LEX_TRACE,val);
}
bool ptrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_PARSE_TRACE,val);
}
bool otrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_OPT_TRACE,val);
}
bool ctrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_COMPILE_TRACE,val);
}
bool rtrace(bool val) {
  return assign_bit(&fs->task_flags,TSSF_RUN_TRACE,val);
}
bool echo(bool val) {
  return assign_bit(&fs->task_flags,TSSF_ECHO,val);
}
bool preempt(bool val) {
  return assign_bit(SYS_FLAGS,SYSF_PREEMPTIVE,val);
}

void sysfun SWAP_IN_NEXT swap_in_next();
void sysfun CMD_ADD_SYS_HASH add_sys_hash(sys_hash_entry *e);
sys_hash_entry * sysfun CMD_FIND_HASH_ENTRY
  find_hash_entry(char *st,sys_hash_table *table,DWORD mask);
sys_hash_entry ** sysfun CMD_FIND_HASH_BUCKET
  find_hash_bucket(char *st,sys_hash_table *table);
sys_hash_table * sysfun CMD_NEW_HASH_TABLE
  new_hash_table(DWORD size);
void sysfun CMD_DELETE_HASH_TABLE
  delete_hash_table(sys_hash_table *table);

sys_hash_entry *find_sys_hash(char *st,DWORD mask)
{
  return find_hash_entry(st,*SYS_HASH_TABLE,mask);
}

sys_hash_entry *find_local_hash(char *st,DWORD mask)
{
  return find_hash_entry(st,fs->hash_table,mask);
}


//MEMORY RELATED
#define PAGE_SIZE	   0x1000
#define PAGE_BITS	   12
#define DEFAULT_STACK      (PAGE_SIZE*16)
void sysfun SPAWN_USER_AND_ACTIVATE spawn_user_and_activate();
tss_struct *sysfun CMD_SPAWN spawn(BYTE *start_add,char *desc="GENERIC",DWORD stk=DEFAULT_STACK); //Spawn Task
void sysfun CMD_KILL  kill(tss_struct *task); //Kill Task
//Note: "void exit()" is a keyword

tss_cmd_node *sysfun CMD_QUEUE_TASK_REQUEST 
  queue_task_request(tss_struct *slave,char *request,DWORD flags);
bool sysfun CMD_GET_REQUEST_RESULT 
  get_request_result(tss_cmd_node *rqst=NULL,DWORD *result=NULL);
//Returns true if request found and result!=NULL.  If result=NULL, 
//  returns tesult.  Scans for rqst if rqst!=NULL, else takes first in queue 

DWORD root(char *cmd) {
  queue_task_request(root_tss,cmd,(1<<TSSCF_WAKE));
  return get_request_result; 
}
DWORD system(char *cmd) {
  DWORD result=0;
  tss_struct *tss=spawn(SYSTEM_COMMAND_LINE,"SYSTEM TASK");
  queue_task_request(tss,cmd,(1<<TSSCF_WAKE));
  result=get_request_result;
  kill(tss);
  return result; 
}
void sysfun CMD_INTERRUPT interrupt(BYTE num);
char sysfun TRANSLATE_SCAN_CODE translate_scan_code(); //no echo
bool kbhit() {
  if (bt(&fs->task_flags,TSSF_FOCUS))
    return btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
  else
    return false;
}
void sysfun CMD_GET_STRING gets(char *buf,DWORD size);
char *sysfun CMD_LOAD_TEXT_FILE load_text_file(char *filename);
void sysfun CMD_SLEEP	 sleep(DWORD mS);
void sysfun CMD_REBOOT	 reboot();
void sysfun CRLF	 crlf();
void sysfun SPACE	 space();
void sysfun INIT_CRT	 init_crt();
void sysfun CLEAR_CRT	 clear_crt();
void sysfun CMD_CRT_LINE_OUT crt_line_out(char *st);
void sysfun CURSOR_HOME  cursor_home();
void sysfun CURSOR_ON	 cursor_on();
void sysfun CURSOR_OFF	 cursor_off();
void sysfun CMD_DELETE_LINKED_LIST delete_linked_list(DWORD *list);
void sysfun CMD_LOADSTR  loadstr(DWORD i, char *st);
void sysfun CMD_LOADLIST loadlist(DWORD i, char *st);
void sysfun CMD_DEACTIVATE_ALL deactivate_all();
tss_struct *sysfun CMD_DEACTIVATE_USER deactivate_user();
void sysfun CMD_ACTIVATE_NEXT_USER activate_next_user();

#define BLACK	   0
#define BLUE	   1
#define GREEN	   2
#define CYAN	   3
#define RED	   4
#define PURPLE	   5
#define BROWN	   6
#define LTGRAY	   7
#define DKGRAY	   8
#define LTBLUE	   9
#define LTGREEN    10
#define LTCYAN	   11
#define LTRED	   12
#define LTPURPLE   13
#define YELLOW	   14
#define WHITE	   15

#define screen_width 800
#define screen_height 600
#define font_width  8
#define font_height 8
void sysfun GR_UPDATE_SCREEN gr_update_screen();
void sysfun CMD_GR_CLEAR gr_clear2();
bool gr_just_cleared=true; //For cursor
void gr_clear()
{
  gr_clear2;
  gr_just_cleared=true;
}
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

void unrecoverable_system_error(int msg)
{
  ? systext(msg);
  while (true);
}

#include "FIL_STRINGS_CPP"
#include "FIL_DISK1_CPP"
#include "FIL_FAT_CPP"
#include "FIL_DISK2_CPP"
;
read_partition_info;
select_default_drive;
;
#include "ROOT2.CPP"
