#define st_user_startup 	  2
#define st_undefined_systext	  8
#define st_invalid_systext_number 10
#define st_file_not_found	  12
#define st_press_a_key		  13
#define st_verify_error 	  14
#define st_not_FAT32		  15
#define st_delete		  16
#define st_invalid_drive	  17
#define st_current_partition	  18
#define st_not_FAT12		  19
#define st_file_system_not_supported 20
#define st_no_matching_entries	  21
#define st_out_of_root_dir_space  22
#define st_out_of_drive_space     23
#define st_invalid_cluster        24
#define st_table_size 4096

#define st_error		  256
#define st_missing_semicolon	  257
#define st_missing_expression	  258
#define st_missing_right_paren	  259
#define st_missing_right_bracket  260
#define st_undefined_symbol	  261
#define st_invalid_class	  262
#define st_invalid_member	  263
#define st_expecting_type	  264
#define st_expecting_identifier   265
#define st_expecting_right_paren  266
#define st_expecting_left_brace   267
#define st_missing_right_brace	  268
#define st_invalid_lvalue	  269
#define st_expecting_equal	  270
#define st_not_pointer_type	  271
#define st_expecting_comma	  272
#define st_expecting_sys_symbol   273
#define st_expecting_string	  274
#define st_expecting_integer	  275
#define st_unknown_statement	  276
#define st_already_one_immediate  277
#define st_syntax_error 	  278
#define st_expecting_scale_factor 279
#define st_question_not_allowed   280
#define st_expecting_left_paren   281
#define st_undefined_DUP_count	  282
#define st_expecting_colon	  283
#define st_expecting_double_colon 284
#define st_redefinition 	  285
#define st_invalid_instruction	  286
#define st_internal_error	  287
#define st_branch_out_of_range	  288
#define st_attempt_define_import  289
#define st_could_use_BYTE_disp	  290
#define st_warning		  291
#define st_line 		  292
#define st_no_size_specified	  293
#define st_byte_regs              294
#define st_word_regs              295
#define st_dword_regs             296
#define st_opcode_modifiers       297
#define st_argument_types         298
#define st_seg_regs               299

void load_sys_strings()
{
  int i;

  i=0x002;  // #0-0xFF reserved for general OS text
  loadstr(i++,"Welcome\r\n");
  loadstr(i++,">");
  loadstr(i++,"Interrupt #");
  loadstr(i++,"Trap\r\n");
  loadstr(i++,"Invalid task address");
  loadstr(i++,"ERROR: Undefined extern\r\n");
  loadstr(i++,"ERROR: Undefined systext\r\n");
  loadstr(i++,"Invalid extern number\r\n");
  loadstr(i++,"Invalid systext number\r\n");
  loadstr(i++,"ERROR: Command not found.\r\n");
  loadstr(i++,"ERROR: File not found.\r\n");
  loadstr(i++,"PRESS A KEY\r\n");
  loadstr(i++,"Verify Error:");
  loadstr(i++,"ERROR: Not FAT32 Partition\r\n");
  loadstr(i++,"Delete ");
  loadstr(i++,"ERROR: Invalid Drive\r\n");
  loadstr(i++,"Current Partition: ");
  loadstr(i++,"ERROR: Not FAT12 Partition\r\n");
  loadstr(i++,"ERROR: File System Not supported\r\n");
  loadstr(i++,"No matching entries\r\n");
  loadstr(i++,"ERROR: Out of root directory space\r\n");
  loadstr(i++,"ERROR: Out of drive space\r\n");
  loadstr(i++,"ERROR: Invalid Cluster\r\n");

  i=0x100;  // #0-0x1FF reserved for system parser/compiler/asm
  loadstr(i++,"ERROR: ");
  loadstr(i++,"missing ';' at");
  loadstr(i++,"missing expression at ");
  loadstr(i++,"missing ')' at ");
  loadstr(i++,"missing ']' at ");
  loadstr(i++,"undefined symbol at ");
  loadstr(i++,"invalid class at ");
  loadstr(i++,"invalid member at ");
  loadstr(i++,"expecting type at ");
  loadstr(i++,"expecting identifier at ");
  loadstr(i++,"expecting ')' at ");
  loadstr(i++,"expecting '{' at ");
  loadstr(i++,"missing '}' at ");
  loadstr(i++,"invalid lvalue at ");
  loadstr(i++,"expecting '=' at ");
  loadstr(i++,"not pointer type at ");
  loadstr(i++,"expecting ',' at ");
  loadstr(i++,"expecting system symbol at ");
  loadstr(i++,"expecting string at ");
  loadstr(i++,"expecting integer at ");
  loadstr(i++,"unknown statement at ");
  loadstr(i++,"already one immediate at ");
  loadstr(i++,"syntax error at ");
  loadstr(i++,"expecting scale factor at ");
  loadstr(i++,"'?' not allowed at ");
  loadstr(i++,"expecting '(' at ");
  loadstr(i++,"undefined DUP count at ");
  loadstr(i++,"expecting ':' at ");
  loadstr(i++,"expecting '::' at ");
  loadstr(i++,"redefinition at ");
  loadstr(i++,"invalid instruction at ");
  loadstr(i++,"internal error at ");
  loadstr(i++,"branch out of range at ");
  loadstr(i++,"attempt to define import at ");
  loadstr(i++,"could use BYTE displacement at ");
  loadstr(i++,"WARNING: ");
  loadstr(i++,"line: ");
  loadstr(i++,"no size specified at ");
  loadlist(i++,"AL\0CL\0DL\0BL\0AH\0CH\0DH\0BH\0\0");
  loadlist(i++,"AX\0CX\0DX\0BX\0SP\0BP\0SI\0DI\0\0");
  loadlist(i++,"EAX\0ECX\0EDX\0EBX\0ESP\0EBP\0ESI\0EDI\0\0");
  loadlist(i++,"NO\0R\0CB\0CW\0CD\0CP\0IB\0IW\0ID\0RB\0RW\0RD\0\0");
  loadlist(i++,"NO\0REL8\0REL16\0REL32\0UIMM8\0UIMM16\0UIMM32\0R8\0R16\0R32\0"
	      "IMM8\0IMM16\0IMM32\0RM8\0RM16\0RM32\0M8\0M16\0M32\0"
	      "M1616\0M1632\0M16N32\0M16N16\0M32N32\0"
	      "MOFFS8\0MOFFS16\0MOFFS32\0CL\0"
	      "AL\0AX\0EAX\0DX\0SS\0DS\0ES\0FS\0GS\0SREG\0CS\0\0");
  loadlist(i++,"ES\0CS\0SS\0DS\0FS\0GS\0\0");


  i=0x200;  //Start of next block of system text

  i=0x400;  //Start of user messages
}


char *systext(DWORD num)
{
  char *result;
  if (!num || num>=4096) {
    ? fs->string_table[st_invalid_systext_number];
    return NULL;
  }
  result=fs->string_table[num];
  if (!result)
    ? fs->string_table[st_undefined_systext];
  return result;
}


