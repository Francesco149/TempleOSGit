#define TK_EOF			0
#define TK_IDENT		1
#define TK_STRING		2
#define TK_INTEGER		3
#define TK_DOUBLE		4
#define TK_LESS_EQUAL		5
#define TK_EQUAL_EQUAL		6
#define TK_GREATER_EQUAL	7
#define TK_AND_AND		8
#define TK_OR_OR		9
#define TK_ROTATE_LEFT		0x0B
#define TK_ROTATE_RIGHT 	0x0C
#define TK_CR			0x0D
#define TK_DEREFERENCE		0x0E
#define TK_NOT_EQUAL		0x0F
#define TK_PLUS_PLUS		0x10
#define TK_MINUS_MINUS		0x11
#define TK_DOUBLE_COLON 	0x12

#define TK_DOUBLE_QUOTE 	0x22
#define TK_SINGLE_QUOTE 	0x27
#define TK_LEFT_BRACE		0x7B
#define TK_RIGHT_BRACE		0x7D

#define LF_EXPECTING_HEX	1
#define LF_PROMPT		2
#define LF_DONT_FREE_BUFFER	4
#define LF_DOUBLE_SEMICOLON	8
#define LF_NO_DEFINES		16

#define LF_ASM_EXPRESSIONS	0x000200000
#define LF_UNRESOLVED_LOCAL	0x000400000
#define LF_UNRESOLVED_GLOBAL	0x000800000
#define LF_FUNCTION		0x001000000
#define LF_POSTINC		0x002000000
#define LF_POSTDEC		0x004000000
#define LF_PREINC		0x008000000
#define LF_PREDEC		0x010000000
#define LF_ARRAY		0x020000000
#define LF_EAX			0x040000000
#define LF_USE_LAST_CHAR	0x080000000

#define LEX_MAX_IDENT		134

#define LFN_COUNT		8
#define LFN_STACK_COUNT 	8


class lex_file_node
{
  char *buffer;
  char *buf_ptr;
  int  line_num;
  char *name;
  char *cur_line_start;
};

class lex_undef_entry
{
  lex_undef_entry *next;
  sys_hash_entry *hash;
};

class lex_node
{
  int token;
  DWORD flags;
  int cur_i;
  DWORD cur_d[3];
  char *ident;
  int	ident_len;
  int	last_char;
  int	instruction_pointer;  //For asm
  sys_hash_table   *define_hash_table;
  sys_hash_table   *local_hash_table;
  sys_hash_table   *global_hash_table;
  sys_hash_table   *hash_table_list;
  sys_hash_entry   *hash_entry;
  lex_undef_entry  *undef_hash_entry;
  class_node	   *local_var_list;
  member_list_node *local_var_entry;
  char *cur_buf_ptr;
  lex_file_node *cur_lfn;
  lex_file_node *stack_ptr;

  DWORD cb_out_first;
  DWORD cb_out_last;
  DWORD cb_out_end;
  DWORD cb_out_end_ptr;
  DWORD cb_last_out_ptr;
  DWORD cb_in_first;
  DWORD cb_in_end;
  DWORD cb_in_ptr;
  DWORD cb_label_list;
  DWORD cb_stack;

  lex_file_node lfns[LFN_COUNT];
  lex_file_node stack[LFN_STACK_COUNT];
};

DWORD sysfun CMD_LEX   lex(lex_node *l);
void sysfun CMD_LEX_SKIP_EOL lex_skip_eol(lex_node *l);
void sysfun CMD_LEX_PUSH lex_push(lex_node *l);
void sysfun CMD_LEX_POP lex_pop(lex_node *l);
void sysfun CMD_LEX_SUB_STACK_PTR lex_sub_stack_ptr(lex_node *l); //grows up
void sysfun CMD_LEX_PUT_TOKEN lex_put_token(lex_node *l);
lex_node * sysfun CMD_NEW_LEX lex_new(char *buf,DWORD flags=LF_DONT_FREE_BUFFER+LF_ASM_EXPRESSIONS);
void sysfun CMD_DELETE_LEX lex_delete(lex_node *l);

void sysfun CMD_DELETE_EXPRESSION delete_expression(BYTE *e);
bool sysfun CMD_GET_EXPRESSION get_expression(lex_node *l,BYTE *e); //false=no err
DWORD sysfun CMD_RUN_STATEMENT run_statement(BYTE *s);

void lex_display_line(char *start)
{
  char temp,*ptr;
  ptr=start;
  while (*ptr && *ptr!=10 && *ptr!=13)
    ptr++;
  temp=*ptr;
  *ptr=0;
  ? start,"\r\n";
  *ptr=temp;
}

int lex_line_num(lex_node *l)
{
  return l->cur_lfn->line_num;
}

void display_lex_error(lex_node *l,char *msg)
{
  ? systext(st_error),msg;
  lex_put_token(l);
  printf(" %s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num);
  lex_display_line(l->cur_lfn->cur_line_start);
  lex(l); //skip error
}

void display_systext_lex_error(lex_node *l,DWORD num)
{
  ? systext(st_error),systext(num);
  lex_put_token(l);
  printf(" %s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num);
  lex_display_line(l->cur_lfn->cur_line_start);
  lex(l); //skip error
}

void display_lex_warning(lex_node *l,char *msg)
{
  ? systext(st_warning),msg;
  lex_put_token(l);
  printf(" %s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num);
  lex_display_line(l->cur_lfn->cur_line_start);
  lex(l); //skip error
}

void display_systext_lex_warning(lex_node *l,DWORD num)
{
  ? systext(st_warning),systext(num);
  lex_put_token(l);
  printf(" %s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num);
  lex_display_line(l->cur_lfn->cur_line_start);
  lex(l); //skip error
}

