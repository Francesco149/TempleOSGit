sys_hash_table *asm_hash=NULL;

//Assembler Hash Types
#define AHT_STRING_CONSTANT   1
#define AHT_SYS_SYMBOL	      2
#define AHT_KEYWORD	      4
#define AHT_OPCODE	      8
#define AHT_R8		      16
#define AHT_R16 	      32
#define AHT_R32 	      64
#define AHT_SEG 	      128
#define AHT_EXPORT	      0x08000000
#define AHT_IMPORT	      0x10000000
#define AHT_LITERAL	      0x20000000
#define AHT_UNRESOLVED_LOCAL  0x40000000
#define AHT_UNRESOLVED_GLOBAL 0x80000000

class instruction_entry
{
  BYTE ins_entry_num; //This entry num in opcode hash entry
  BYTE opcode_count;
  BYTE opcode[4],operand_size16,operand_size32,dont_switch_modes;
  BYTE plus_opcode,slash_value,opcode_modifier,default;
  BYTE arg1,arg2;
};

class opcode_hash_entry
{
  sys_hash_entry *next;
  char *string;
  DWORD type;  //AHT_OPCODE
  BYTE instruction_entry_count;
  BYTE reserved[3];
  instruction_entry ins[1];
};


#define OM_NO 0
#define OM_R  1 /*Not used*/
#define OM_CB 2
#define OM_CW 3
#define OM_CD 4
#define OM_CP 5
#define OM_IB 6
#define OM_IW 7
#define OM_ID 8
#define OM_RB 9
#define OM_RW 10
#define OM_RD 11

#define ARGT_NO      0
#define ARGT_REL8    1
#define ARGT_REL16   2
#define ARGT_REL32   3

#define ARGT_UIMM8   4
#define ARGT_UIMM16  5
#define ARGT_UIMM32  6
#define ARGT_R8      7

#define ARGT_R16     8
#define ARGT_R32     9
#define ARGT_IMM8    10
#define ARGT_IMM16   11

#define ARGT_IMM32   12
#define ARGT_RM8     13
#define ARGT_RM16    14
#define ARGT_RM32    15

#define ARGT_M8      16 /*Not used, needed for LEA, etc */
#define ARGT_M16     17 /*Not used*/
#define ARGT_M32     18 /*Not used*/
#define ARGT_M1616   19 /*Not used*/

#define ARGT_M1632   20 /*Not used*/
#define ARGT_M16N32  21 /*Not used*/
#define ARGT_M16N16  22 /*Not used*/
#define ARGT_M32N32  23 /*Not used*/

#define ARGT_MOFFS8  24
#define ARGT_MOFFS16 25
#define ARGT_MOFFS32 26
#define ARGT_CL      27

#define ARGT_AL      28
#define ARGT_AX      29
#define ARGT_EAX     30
#define ARGT_DX      31

#define ARGT_SS      32
#define ARGT_DS      33
#define ARGT_ES      34
#define ARGT_FS      35

#define ARGT_GS      36
#define ARGT_SREG    37
#define ARGT_CS      38


#define AKW_INCLUDE   0
#define AKW_DEFINE    1
#define AKW_EXTRN     16
#define AKW_ALIGN     17
#define AKW_END       18
#define AKW_BYTE   19
#define AKW_WORD   20
#define AKW_DWORD  21
#define AKW_DB	   22
#define AKW_DW	   23
#define AKW_DD	   24
#define AKW_DUP    25
#define AKW_ORG    26
#define AKW_STRUC  27
#define AKW_ENDS   28
#define AKW_USE16  29
#define AKW_USE32  30
#define AKW_IMPORT 31
#define AKW_EXPORT 32
#define AKW_INSERT 33
#define AKW_LIST   34
#define AKW_NOLIST 35

#define ASM_CODE_BLOCK_SIZE 0x10000

class asm_code_block
{
  asm_code_block *next;
  BYTE body[ASM_CODE_BLOCK_SIZE];
};

class asm_arg_node
{
  int seg,size,reg1,reg2;
  int imm;
  int scale;
  bool indirect,imm_or_off_present,just_seg,reserved;
  lex_undef_entry *undef_local,*undef_global;
  BYTE *exp;
};

class asm_unresolved_ref
{
  asm_unresolved_ref *next;
  int type,line_num;
  BYTE *exp;
  DWORD ip,rel_ip;
  bool byte_avail;
  char *string;  //Only for import globals
  bool literal;  //Only for import globals
  lex_undef_entry *undef_hash;
};

#define eie_rel_export 0
#define eie_lit_export 1
#define eie_rel_BYTE   2
#define eie_lit_BYTE   3
#define eie_rel_WORD   4
#define eie_lit_WORD   5
#define eie_rel_DWORD  6
#define eie_lit_DWORD  7
#define eie_end        0xFF

class exe_import_export_node
{
  exe_import_export_node *next;
  BYTE type;
  DWORD ip;
  char *string;
};

class exe_node
{
  BYTE *code;
  DWORD code_bytes;
  exe_import_export_node *ie;
};


class asm_ctrl_node
{
  sys_hash_table *local_hash,*global_hash;
  DWORD ip;  //instruction ptr
  asm_arg_node arg1,arg2;
  asm_code_block *code;
  DWORD num_code_bytes;
  asm_unresolved_ref *local_unresolved,*global_unresolved;
  bool use16,has_locals,list;
  int display_col,cur_line_num,last_ip;
  char *last_line,*last_label;
  lex_file_node *last_lfn;
};


void display_asm_error(lex_node *l,DWORD num)
{
  lex_push(l);
  display_systext_lex_error(l,num);
  lex_pop(l);
  lex_skip_eol(l);
  lex(l);
}

void display_asm_warning(lex_node *l,DWORD num)
{
  lex_push(l);
  display_systext_lex_warning(l,num);
  lex_pop(l);
  lex_skip_eol(l);
  lex(l);
}

void load_asm_hash()
{
  char *buf;
  lex_node *l;
  int i,size,size1;
  sys_hash_entry *temph,**temph1;
  opcode_hash_entry *tempo,*tempo1;
  instruction_entry *tempi;
  asm_hash=new_hash_table(2048);
  size1=sizeof(opcode_hash_entry)+sizeof(instruction_entry)*31;
  tempo=malloc(size1);

  buf=load_text_file("/HOPPY/UTILS/ASM.TXT");
  l=lex_new(buf);
  lex(l);
  while (l->token) {
    if (l->token!=TK_IDENT)
      display_asm_error(l,st_expecting_identifier);
    else {
      i=match_list_entry(l->ident,"OPCODE\0KEYWORD\0R8\0R16\0R32\0SEG\0\0",mle_ignore_case);
      lex(l); //skip keyword
      if (i==0) { //OPCODE
	memset(tempo,0,size1);
	tempo->type=AHT_OPCODE;
	tempo->instruction_entry_count=0;
	tempo->string=l->ident;
	l->ident=0;
	lex(l);  //Skip OPCODE
	while (l->token && l->token!=';') {
	  tempi=&tempo->ins[tempo->instruction_entry_count];
	  tempi->ins_entry_num=tempo->instruction_entry_count++;
	  tempi->opcode_count=0;
	  while (l->token==TK_INTEGER) {
	    tempi->opcode[tempi->opcode_count++]=l->cur_i;
	    lex(l);
	  }
	  if (l->token=='+') {
	    tempi->plus_opcode=1;
	    lex(l);
	  } else
	    tempi->plus_opcode=0;
	  if (l->token=='/') {
	    lex(l);
	    if (l->token==TK_INTEGER) {
	      tempi->slash_value=l->cur_i;
	      lex(l);
	    } else if (l->token==TK_IDENT) {
	      tempi->slash_value=8;
	      lex(l);
	    }
	  } else
	    tempi->slash_value=9;
	  if (l->token=='*') {
	    tempi->operand_size16=1;
	    lex(l);
	  } else
	    tempi->operand_size16=0;
	  if (l->token=='%') {
	    tempi->operand_size32=1;
	    lex(l);
	  } else
	    tempi->operand_size32=0;
	  if (l->token=='!') {
	    tempi->dont_switch_modes=1;
	    lex(l);
	  } else
	    tempi->dont_switch_modes=0;
	  if (l->token=='&') {
	    tempi->default=1;
	    lex(l);
	  } else
	    tempi->default=0;
	  if (l->token!=TK_IDENT)
	    display_asm_error(l,st_expecting_identifier);
	  else {
	    tempi->opcode_modifier=match_list_entry(l->ident,
		systext(st_opcode_modifiers),mle_ignore_case);
	    tempi->arg1=0;
	    tempi->arg2=0;
	    lex(l);
	    if (l->token==TK_IDENT) {
	      tempi->arg1=match_list_entry(l->ident,
                  systext(st_argument_types),mle_ignore_case);
	      lex(l);
	      if (l->token==TK_IDENT) {
		tempi->arg2=match_list_entry(l->ident,
                  systext(st_argument_types),mle_ignore_case);
		lex(l);
	      }
	    }
	  }
	}
	if (l->token==';') {
	  lex(l); //Skip ';'
	  size=sizeof(opcode_hash_entry)+
	     sizeof(instruction_entry)*(tempo->instruction_entry_count-1);
	  tempo1=malloc(size);
	  memcpy(tempo1,tempo,size);
	  temph1=find_hash_bucket(tempo1->string,asm_hash);
	  tempo1->next=*temph1;
	  *temph1=tempo1;
	} else
	  display_asm_error(l,st_missing_semicolon);
      } else if (i>=1 && i<=5) { //KEYWORD,R8,R16,R32,SEG
	if (l->token!=TK_IDENT)
	  display_asm_error(l,st_expecting_identifier);
	else {
	  temph=malloc(sizeof(sys_hash_entry));
	  temph->string=l->ident;
	  l->ident=0;
	  lex(l); //skip keyword name
	  if (i==1)
	    temph->type=AHT_KEYWORD;
	  else if (i==2)
	    temph->type=AHT_R8;
	  else if (i==3)
	    temph->type=AHT_R16;
	  else if (i==4)
	    temph->type=AHT_R32;
	  else if (i==5)
	    temph->type=AHT_SEG;
	  if (l->token!=TK_INTEGER)
	    display_asm_error(l,st_expecting_integer);
	  else {
	    temph->user0=l->cur_i;
	    lex(l); //skip keyword number
	    if (l->token!=';')
	      display_asm_error(l,st_missing_semicolon);
	    else {
	      lex(l);  //skip ';'
	      temph1=find_hash_bucket(temph->string,asm_hash);
	      temph->next=*temph1;
	      *temph1=temph;
	    }
	  }
	}
      } else
	display_asm_error(l,"Unknown Statement");
    }
  }
  free(tempo);
  lex_delete(l);
  free(buf);
}

void add_unresolved(asm_ctrl_node *a,BYTE *exp,int type,DWORD ip,DWORD rel_ip,
		    lex_undef_entry *undef_hash,int line_num,bool byte_avail,bool global)
{
  asm_unresolved_ref *tempu=malloc(sizeof(asm_unresolved_ref));
  tempu->exp=exp;
  tempu->type=type;
  tempu->ip=ip;
  tempu->rel_ip=rel_ip;
  tempu->undef_hash=undef_hash;
  tempu->literal=false;
  tempu->line_num=line_num;
  tempu->byte_avail=byte_avail;
  tempu->string=NULL;
  if (global) {
    tempu->next=a->global_unresolved;
    a->global_unresolved=tempu;
    if (undef_hash->hash->type & AHT_IMPORT) {
      tempu->string=new_string(undef_hash->hash->string);
      if (undef_hash->hash->type & AHT_LITERAL)
	tempu->literal=true;
    }
  } else {
    tempu->next=a->local_unresolved;
    a->local_unresolved=tempu;
    a->has_locals=true;
  }
}

void asm_store_code_byte(asm_ctrl_node *a,BYTE b)
{
  int i=a->ip/ASM_CODE_BLOCK_SIZE+1;
  asm_code_block *tempc=&a->code;
  while (i--) {
    if (!*tempc) {
      *tempc=malloc(sizeof(asm_code_block));
      memset(*tempc,0,sizeof(asm_code_block));
    }
    tempc=tempc->next;
  }
  tempc->body[a->ip++ & (ASM_CODE_BLOCK_SIZE-1)]=b;
  if (a->list) {
    printf("%02X ",b);
    a->display_col++;
    if (a->display_col>=8) {
      a->display_col=0;
      printf("\r\n");
      a->cur_line_num++;
      if (a->cur_line_num>(fs->window_bottom-fs->window_top-10)) {
	a->cur_line_num=0;
	press_a_key;
      }
    }
  }
  if (a->ip>a->num_code_bytes)
    a->num_code_bytes=a->ip;
}

void asm_store_code_byte_at(asm_ctrl_node *a,BYTE b,int ip)
{
  int i=ip/ASM_CODE_BLOCK_SIZE+1;
  asm_code_block *tempc=&a->code;
  while (i--) {
    if (!*tempc) {
      *tempc=malloc(sizeof(asm_code_block));
      **tempc=NULL;
    }
    tempc=tempc->next;
  }
  tempc->body[ip & (ASM_CODE_BLOCK_SIZE-1)]=b;
  if (ip+1>a->num_code_bytes)
    a->num_code_bytes=ip+1;
}


DWORD asm_eval_expression(lex_node *l)
{
  BYTE *exp;
  DWORD result=0;
  if (get_expression(l,&exp))
    lex_skip_eol(l);
  else {
    if (!(l->flags & (LF_UNRESOLVED_LOCAL+LF_UNRESOLVED_GLOBAL))) {
      result=run_statement(exp);
      delete_expression(exp);
    } else
      display_asm_error(l,st_undefined_symbol);
  }
  return result;
}

void resolve_locals(lex_node *l,asm_ctrl_node *a)
{
  asm_unresolved_ref *tempu=a->local_unresolved,*tempu1;
  int result=0;
  lex_undef_entry *templu;
  bool undefined;
  char *label=NULL;

  if (!a->has_locals) return;
  while (tempu) {
    undefined=false;
    tempu1=tempu->next;
    templu=tempu->undef_hash;
    while (templu) {
      if (templu->hash->type & (AHT_UNRESOLVED_LOCAL + AHT_UNRESOLVED_GLOBAL)) {
	? systext(st_error),systext(st_undefined_symbol),systext(st_line);
	printf("%04X ",tempu->line_num);
	if (a->last_label) ? a->last_label," ";
	? templu->hash->string,"\r\n";
	undefined=true;
      } else
	label=templu->hash->string;
      templu=templu->next;
    }
    if (!undefined) {
      result=run_statement(tempu->exp);
      if (!(tempu->type & 1)) {
	result=result-tempu->rel_ip;
	if ((tempu->type==eie_rel_BYTE && (result<-128 || result>127)) ||
	   (tempu->type==eie_rel_WORD && (result<-0x8000 || result>0x7FFF)) ) {
	  ? systext(st_error),systext(st_branch_out_of_range),systext(st_line);
	  printf("%04X ",tempu->line_num);
	  if (a->last_label)
	    printf("%s ",a->last_label);
	  if (label) ? label;
	  ? "\r\n";
	}
	if (tempu->byte_avail && tempu->type>eie_lit_BYTE && (result>=-124 && result<=123)) {
	  ? systext(st_warning),systext(st_could_use_BYTE_disp),systext(st_line);
	  printf("%04X ",tempu->line_num);
	  if (a->last_label)
	    printf("%s ",a->last_label);
	  if (label) ? label;
	  ? "\r\n";
	}
      }
      asm_store_code_byte_at(a,result & 255,tempu->ip);
      if (tempu->type>=eie_rel_WORD)
	asm_store_code_byte_at(a,(result>>8) & 255,tempu->ip+1);
      if (tempu->type>=eie_rel_DWORD) {
	asm_store_code_byte_at(a,(result>>16) & 255,tempu->ip+2);
	asm_store_code_byte_at(a,(result>>24) & 255,tempu->ip+3);
      }
    }
    delete_expression(tempu->exp);
    delete_linked_list(tempu->undef_hash);
    free(tempu);
    tempu=tempu1;
  }
  delete_hash_table(a->local_hash);
  a->local_hash=new_hash_table(16);
  a->local_hash->next=a->global_hash;
  l->hash_table_list=a->local_hash;
  l->local_hash_table=a->local_hash;
  a->local_unresolved=NULL;
  a->has_locals=false;
}

void resolve_globals(lex_node *l,asm_ctrl_node *a,exe_node *e)
{
  asm_unresolved_ref *tempu=a->global_unresolved,*tempu1;
  int result=0;
  exe_import_export_node *tempe,*tempe1;
  lex_undef_entry *templu;
  int i;
  sys_hash_entry *temph;
  char *label;
  bool undefined;
  while (tempu) {
    undefined=false;
    tempu1=tempu->next;
    templu=tempu->undef_hash;
    while (templu) {
      if (templu->hash->type & (AHT_UNRESOLVED_LOCAL + AHT_UNRESOLVED_GLOBAL)) {
	? systext(st_error),systext(st_undefined_symbol),systext(st_line);
	printf("%04X ",tempu->line_num);
	? templu->hash->string,"\r\n";
	undefined=true;
      } else
	label=templu->hash->string;
      templu=templu->next;
    }
    if (undefined) {
      templu=tempu->undef_hash;
      if (templu->hash->type & AHT_IMPORT) {
	tempe=malloc(sizeof(exe_import_export_node));
	tempe->next=templu->hash->user1;
	tempe->string=NULL;
	templu->hash->user1=tempe;
	tempe->ip=tempu->ip;
	tempe->type=tempu->type;
      }
    } else {
      result=run_statement(tempu->exp);
      if (!(tempu->type & 1)) {
	result=result-tempu->rel_ip;
	if ((tempu->type==eie_rel_BYTE && (result<-128 || result>127)) ||
	    (tempu->type==eie_rel_WORD && (result<-0x8000 || result>0x7FFF))) {
	  ? systext(st_error),systext(st_branch_out_of_range),systext(st_line);
	  printf("%04X ",tempu->line_num);
	  if (label) ? label;
	  ? "\r\n";
	}
	if (tempu->byte_avail && tempu->type>eie_lit_BYTE && (result>=-124 && result<=123)) {
	  ? systext(st_warning),systext(st_could_use_BYTE_disp),systext(st_line);
	  printf("%04X ",tempu->line_num);
	  if (a->last_label)
	    printf("%s ",a->last_label);
	  if (label) ? label;
	  ? "\r\n";
	}
      } 
      asm_store_code_byte_at(a,result & 255,tempu->ip);
      if (tempu->type>=eie_rel_WORD)
	asm_store_code_byte_at(a,(result>>8) & 255,tempu->ip+1);
      if (tempu->type>=eie_rel_DWORD) {
	asm_store_code_byte_at(a,(result>>16) & 255,tempu->ip+2);
	asm_store_code_byte_at(a,(result>>24) & 255,tempu->ip+3);
      }
    }
    delete_expression(tempu->exp);
    delete_linked_list(tempu->undef_hash);
    free(tempu);
    tempu=tempu1;
  }

  tempe1=&e->ie;
  for (i=0;i<=a->global_hash->mask;i++) {
    temph=*(&a->global_hash->body+i*4);
    while (temph) {
      if (temph->type & AHT_IMPORT) {
	tempe=temph->user1;
	if (tempe) {
	  tempe1->next=tempe;
	  tempe->string=temph->string;
	  temph->string=NULL;
	  while (*tempe1)
	    tempe1=tempe1->next;
	}
      } else if (temph->type & AHT_EXPORT) {
	if (temph->type & AHT_UNRESOLVED_GLOBAL)
	  ? systext(st_error),systext(st_undefined_symbol),temph->string,"\r\n";
	else {
	  tempe=malloc(sizeof(exe_import_export_node));
	  tempe->type=eie_rel_export;
	  tempe->ip=temph->user0;
	  if (temph->type & AHT_LITERAL)
	    tempe->type++;
	  tempe1->next=tempe;
	  tempe->next=NULL;
	  while (*tempe1)
	    tempe1=tempe1->next;
	  tempe->string=temph->string;
	  temph->string=NULL;
	}
      }
      temph=temph->next;
    }
  }
}

bool parse_asm_imm(lex_node *l,asm_ctrl_node *aa,asm_arg_node *a,DWORD ip)
{
  if (a->imm_or_off_present) {
    display_asm_error(l,st_already_one_immediate);
    return false;
  } else {
    a->imm_or_off_present=true;
    a->undef_local=NULL;
    a->undef_global=NULL;
    l->instruction_pointer=ip;
    if (get_expression(l,&a->exp))
      lex_skip_eol(l);
    else {
      if (!(l->flags & (LF_UNRESOLVED_LOCAL+LF_UNRESOLVED_GLOBAL))) {
	a->imm=run_statement(a->exp);
	delete_expression(a->exp);
	a->exp=NULL;
      } else {
	if (l->flags & LF_UNRESOLVED_LOCAL) {
	  a->undef_local=l->undef_hash_entry;
	  l->undef_hash_entry=NULL;
	  aa->has_locals=true;
	} else if (l->flags & LF_UNRESOLVED_GLOBAL) {
	  a->undef_global=l->undef_hash_entry;
	  l->undef_hash_entry=NULL;
	}
      }
    }
  }
  return true;
}

void parse_asm_arg(lex_node *l,asm_ctrl_node *aa,asm_arg_node *a,DWORD ip)
{
  int i,temp;
  a->size=0;
  a->seg=8;
  a->reg1=8;
  a->reg2=8;
  a->imm=0;
  a->scale=1;
  a->indirect=false;
  a->imm_or_off_present=false;
  a->undef_local=NULL;
  a->undef_global=NULL;
  a->just_seg=false;
  a->exp=NULL;
  while (true) {
    if (l->token==TK_IDENT) {
      if (l->hash_entry) {
	if (l->hash_entry->type==AHT_R8) {
	  a->size=1;
	  a->reg1=l->hash_entry->user0;
	  lex(l);
	  return;
	} else if (l->hash_entry->type==AHT_R16) {
	  a->size=2;
	  a->reg1=l->hash_entry->user0;
	  lex(l);
	  return;
	} else if (l->hash_entry->type==AHT_R32) {
	  a->size=4;
	  a->reg1=l->hash_entry->user0;
	  lex(l);
	  return;
	} else if (l->hash_entry->type==AHT_SEG) {
	  a->seg=l->hash_entry->user0;
	  lex(l);
	  if (l->token!=':') {
	    a->just_seg=true;
	    return;
	  } else
	    lex(l); //skip ":"
	} else if (l->hash_entry->type==AHT_KEYWORD) {
	  i=l->hash_entry->user0;
	  if (i==AKW_BYTE) {
	    lex(l); //skip keyword
	    a->size=1;
	  } else if (i==AKW_WORD) {
	    lex(l); //skip keyword
	    a->size=2;
	  } else if (i==AKW_DWORD) {
	    lex(l); //skip keyword
	    a->size=4;
	  } else {
	    display_asm_error(l,st_syntax_error);
	    return;
	  }
	} else {
	  parse_asm_imm(l,aa,a,ip);
	  if (l->token!='[')
	    return;
	}
      } else {
	parse_asm_imm(l,aa,a,ip);
	if (l->token!='[')
	  return;
      }
    } else if (l->token=='[') {
      a->indirect=true;
      lex(l); // skip [
      while (l->token && l->token!=']') {
	if (l->token==TK_IDENT) {
	  if (l->hash_entry) {
	    if (l->hash_entry->type==AHT_R16 || l->hash_entry->type==AHT_R32) {
	      if (a->reg1==8)
		a->reg1=l->hash_entry->user0;
	      else
		a->reg2=l->hash_entry->user0;
	      lex(l);
	    } else
	      parse_asm_imm(l,aa,a,ip);
	  } else
	    parse_asm_imm(l,aa,a,ip);
	} else if (l->token=='*') {
	  lex(l);
	  if (l->token!=TK_INTEGER)
	    display_asm_error(l,st_expecting_scale_factor);
	  else {
	    a->scale=l->cur_i;
	    lex(l); //skip scale
	    if (a->reg2!=8) {
	      temp=a->reg1;
	      a->reg1=a->reg2;
	      a->reg2=temp;
	    }
	  }
	} else if (l->token=='+') {
	  lex(l); //skip '+'
	} else
	  parse_asm_imm(l,aa,a,ip);
      }
      if (l->token==']') {
	lex(l); //skip ]
	return;
      } else
	display_asm_error(l,st_missing_right_bracket);
    } else {
      parse_asm_imm(l,aa,a,ip);
      if (l->token!='[')
	return;
    }
  }
}

void asm_make_arg_mask(asm_arg_node *a,DWORD *mask)
{
  if (a->just_seg) {
    mask[0]=0;
    if (a->seg==0)
      mask[1]=0x24;
    else if (a->seg==1)
      mask[1]=0x60;
    else if (a->seg==2)
      mask[1]=0x21;
    else if (a->seg==3)
      mask[1]=0x22;
    else if (a->seg==4)
      mask[1]=0x28;
    else
      mask[1]=0x30;
    return;
  }
  mask[1]=0;

  if (a->size==0)
    mask[0]=0xFF07FFFE;
  else if (a->size==1)
    mask[0]=0x19012492;
  else if (a->size==2)
    mask[0]=0xA2024924;
  else if (a->size==4)
    mask[0]=0x44049248;

  if (a->reg1!=8 && a->imm_or_off_present && a->imm==0) {
    if (!a->undef_global && !a->undef_local)
      a->imm_or_off_present=false;  //Zero displacement
  }

  if (a->reg2!=8 || a->scale!=1) {
    mask[0]=mask[0] & 0x0000E000;
    return;
  }

  if (a->indirect) {
    if (a->imm_or_off_present) {
      mask[0]=mask[0] & 0x07FFE000;
    } else
      mask[0]=mask[0] & 0x00FFE000;
  } else {
    if (a->imm_or_off_present)
      mask[0]=mask[0] & 0x00F81C7E;
    else
      mask[0]=mask[0] & 0xF8F8E380;
  }
  if (a->seg!=8)
    mask[0]=mask[0] & 0x07FFE000;
  if (a->reg1==8) {
    if (a->indirect)
      mask[0]=mask[0] & 0x07FFE000;
    else if (a->imm>=-128 && a->imm<=127)
      mask[0]=mask[0] & 0x00001C7E;
    else if (a->imm>=-128 && a->imm<=255)
      mask[0]=mask[0] & 0x0000187E;
    else if (a->imm>=-0x8000 && a->imm<=0x7FFF)
      mask[0]=mask[0] & 0x0000186E;
    else if (a->imm>=-0x8000 && a->imm<=0xFFFF)
      mask[0]=mask[0] & 0x0000106E;
    else
      mask[0]=mask[0] & 0x0000104E;
  } else
    mask[0]=mask[0] & 0xF807E380;
  if (a->reg1!=0)
    mask[0]=mask[0] & 0x8FFFFFFF;
  if (a->reg1!=1)
    mask[0]=mask[0] & 0xF7FFFFFF;
  if (a->reg1!=2)
    mask[0]=mask[0] & 0x7FFFFFFF;
//  ? "MASK:",mask[0],"\r\n";
}

bool asm_store_num(lex_node *l,asm_ctrl_node *a,int num,bool is_question,int num_bytes,int count,
		   int num_ip_rel,int num_lit,
		   BYTE *num_exp,lex_undef_entry *num_local_hash,lex_undef_entry *num_global_hash,
		   bool byte_avail)
{
  int i;
  if (!num_lit)
    num=num-num_ip_rel;
  for (i=0;i<count;i++) {
    if (num_bytes==1) {
      if (num_local_hash)
	add_unresolved(a,num_exp,eie_rel_BYTE+num_lit,a->ip,num_ip_rel,
	  num_local_hash,lex_line_num(l),byte_avail,false);
      else if (num_global_hash)
	add_unresolved(a,num_exp,eie_rel_BYTE+num_lit,a->ip,num_ip_rel,
	  num_global_hash,lex_line_num(l),byte_avail,true);
      else {
	if (!num_lit && (num<-128 || num>127)) {
	  lex_pop(l);
	  display_asm_error(l,st_branch_out_of_range);
	  a->ip++;
	  return false;
	}
      }
      if (is_question)
	a->ip++;
      else
	asm_store_code_byte(a,num & 255);

    } else if (num_bytes==2) {
      if (num_local_hash)
	add_unresolved(a,num_exp,eie_rel_WORD+num_lit,a->ip,num_ip_rel,
	  num_local_hash,lex_line_num(l),byte_avail,false);
      else if (num_global_hash)
	add_unresolved(a,num_exp,eie_rel_WORD+num_lit,a->ip,num_ip_rel,
	  num_global_hash,lex_line_num(l),byte_avail,true);
      else {
	if (!num_lit && (num <-0x8000 || num>0x7FFF)) {
	  lex_pop(l);
	  display_asm_error(l,st_branch_out_of_range);
	  a->ip=a->ip+2;
	  return false;
	}
      }
      if (is_question)
	a->ip=a->ip+2;
      else {
	asm_store_code_byte(a,num & 255);
	asm_store_code_byte(a,(num>>8) & 255);
      }
      if (byte_avail && !num_local_hash && !num_global_hash &&
	  !num_lit && (num>=-124 && num<=123)) {
	lex_pop(l);
	display_asm_warning(l,st_could_use_BYTE_disp);
	return false;
      }
    } else if (num_bytes==4) {
      if (num_local_hash)
	add_unresolved(a,num_exp,eie_rel_DWORD+num_lit,a->ip,num_ip_rel,
	  num_local_hash,lex_line_num(l),byte_avail,false);
      else if (num_global_hash)
	add_unresolved(a,num_exp,eie_rel_DWORD+num_lit,a->ip,num_ip_rel,
	  num_global_hash,lex_line_num(l),byte_avail,true);
      if (is_question)
	a->ip=a->ip+4;
      else {
	asm_store_code_byte(a,num & 255);
	asm_store_code_byte(a,(num>>8) & 255);
	asm_store_code_byte(a,(num>>16) & 255);
	asm_store_code_byte(a,(num>>24) & 255);
      }
      if (byte_avail && !num_local_hash && !num_global_hash &&
	  !num_lit && (num>=-124 && num<=123)) {
	lex_pop(l);
	display_asm_warning(l,st_could_use_BYTE_disp);
	return false;
      }
    }
  }
  return true;
}


bool asm_code_instruction(lex_node *l,asm_ctrl_node *a,
			  opcode_hash_entry *tempo,int argcount)
{
  int i,j,a1,a2,om,seg;
  DWORD arg1mask[2],arg2mask[2];
  asm_arg_node *tempa1,*tempa2;
  bool ModrM_complete,byte_avail=false,found_second_possible=false;

  int byte_count;
  BYTE last_opcode_byte;
  instruction_entry *tempi;
  bool has_ModrM;
  BYTE ModrM;
  bool has_SIB;
  BYTE SIB;
  bool has_address_prefix;
  bool has_operand_prefix;
  bool is_default;
  int disp,disp_bytes,disp_ip_rel,disp_lit;
  int imm, imm_bytes, imm_ip_rel, imm_lit;
  BYTE *disp_exp,*imm_exp;
  lex_undef_entry *disp_local_hash,*disp_global_hash;
  lex_undef_entry *imm_local_hash,*imm_global_hash;

  int best_byte_count=255;
  BYTE blast_opcode_byte;
  instruction_entry *btempi;
  bool bhas_ModrM;
  BYTE bModrM;
  bool bhas_SIB;
  BYTE bSIB;
  bool bhas_address_prefix;
  bool bhas_operand_prefix;
  bool bis_default;
  int bdisp,bdisp_bytes,bdisp_ip_rel,bdisp_lit;
  int bimm, bimm_bytes, bimm_ip_rel, bimm_lit;
  BYTE *bdisp_exp,*bimm_exp;
  lex_undef_entry *bdisp_local_hash,*bdisp_global_hash;
  lex_undef_entry *bimm_local_hash,*bimm_global_hash;


  if (argcount>0)
    asm_make_arg_mask(&a->arg1,arg1mask);
  else {
    arg1mask[0]=1;
    arg1mask[1]=0;
  }
  if (argcount>1)
    asm_make_arg_mask(&a->arg2,arg2mask);
  else {
    arg2mask[0]=1;
    arg2mask[1]=0;
  }

  for (i=0;i<tempo->instruction_entry_count;i++) {
    tempi=&tempo->ins[i];
    if (tempi->arg1==ARGT_REL8 || tempi->arg2==ARGT_REL8)
      byte_avail=true;
    if (bt(arg1mask,tempi->arg1) && bt(arg2mask,tempi->arg2)) {
      ModrM_complete=false;
      last_opcode_byte=0;
      has_ModrM=false;
      ModrM=0;
      has_SIB=false;
      SIB=0;
      has_address_prefix=false;
      has_operand_prefix=false;
      if (tempi->default)
	is_default=true;
      else
	is_default=false;
      disp=0; disp_bytes=0; disp_ip_rel=0; disp_lit=1;
      imm=0; imm_bytes=0; imm_ip_rel=0; imm_lit=1;
      disp_exp=NULL; imm_exp=NULL;
      disp_local_hash=NULL; disp_global_hash=NULL;
      imm_local_hash=NULL; imm_global_hash=NULL;
      om=tempi->opcode_modifier;
      a1=tempi->arg1;
      a2=tempi->arg2;
      tempa1=&a->arg1;
      tempa2=&a->arg2;
      last_opcode_byte=tempi->opcode[tempi->opcode_count-1];

      if (tempi->slash_value<8) {
	ModrM=ModrM+tempi->slash_value<<3;
	has_ModrM=true;
      }

      if ((a->use16 && tempi->operand_size32) ||
	(!a->use16 && tempi->operand_size16))
	has_operand_prefix=true;

      if (om==OM_IB)	  imm_bytes=1;
      else if (om==OM_IW) imm_bytes=2;
      else if (om==OM_ID) imm_bytes=4;

      if (om==OM_CB) {
	imm_bytes=1;
	imm_lit=0;
      } else if (om==OM_CW) {
	imm_bytes=2;
	imm_lit=0;
      } else if (om==OM_CD) {
	imm_bytes=4;
	imm_lit=0;
      }

      if (argcount==1) {
	if (best_byte_count!=255 && !found_second_possible && !bis_default) {
	  found_second_possible=true;
	  if (a->arg1.size==0) {
	    ? systext(st_warning),systext(st_no_size_specified);
	    printf("%s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num-1);
	  }
	}
	if (tempi->plus_opcode)
	  last_opcode_byte=last_opcode_byte+tempa1->reg1;
	if (a1>=ARGT_RM8 && a1<=ARGT_RM32) {
	  if (a->use16)
	    has_address_prefix=true;

	  has_ModrM=true;
	  if (tempa1->imm_or_off_present && tempa1->indirect && tempa1->reg1==8) {
	    ModrM=ModrM+5;
	    disp=tempa1->imm;
	    disp_exp=tempa1->exp;
	    disp_local_hash=tempa1->undef_local;
	    disp_global_hash=tempa1->undef_global;
	    disp_bytes=4;
	  } else {
	    if (tempa1->reg2==8 && tempa1->scale==1)
	      ModrM=ModrM+tempa1->reg1;
	    else {
	      ModrM=ModrM+4;
	      has_SIB=true;
	      if (tempa1->scale==1)
		SIB=0;
	      else if (tempa1->scale==2)
		SIB=0x40;
	      else if (tempa1->scale==4)
		SIB=0x80;
	      else if (tempa1->scale==8)
		SIB=0xC0;
	      if (tempa1->reg2==8) {
		ModrM_complete=true;
		SIB=SIB+(tempa1->reg1<<3)+5;
		disp=tempa1->imm;
		disp_exp=tempa1->exp;
		disp_local_hash=tempa1->undef_local;
		disp_global_hash=tempa1->undef_global;
		disp_bytes=4;
	      } else {
		SIB=SIB+(tempa1->reg1<<3)+tempa1->reg2;
		if (tempa1->reg2==5 && !tempa1->imm_or_off_present && tempa1->indirect) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		  ModrM_complete=true;
		}
	      }
	    }
	    if (!ModrM_complete) {
	      if (tempa1->imm_or_off_present) {
		disp=tempa1->imm;
		disp_exp=tempa1->exp;
		disp_local_hash=tempa1->undef_local;
		disp_global_hash=tempa1->undef_global;
		if (!disp_exp && (disp>=-128) && (disp<=127)) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		} else if (a->use16) {
		  ModrM=ModrM+0x80;
		  disp_bytes=2;
		} else {
		  ModrM=ModrM+0x80;
		  disp_bytes=4;
		}
	      } else if (!tempa1->indirect) {
		has_address_prefix=false;
		ModrM=ModrM+0xC0;
	      } else {
		if (tempa1->reg1==5) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		}
	      }
	    }
	  }
	} else if ((a1>=ARGT_REL8 && a1<=ARGT_REL32) ||
		   (a1>=ARGT_IMM8 && a1<=ARGT_IMM32) ||
		   (a1>=ARGT_UIMM8 && a1<=ARGT_UIMM32)) {
	  imm=tempa1->imm;
	  imm_exp=tempa1->exp;
	  imm_local_hash=tempa1->undef_local;
	  imm_global_hash=tempa1->undef_global;
	}
      } else if (argcount==2) {
	if (best_byte_count!=255 && !found_second_possible && !bis_default) {
	  found_second_possible=true;
	  if (a->arg1.size==0 && a->arg2.size==0) {
	    ? systext(st_warning),systext(st_no_size_specified);
	    printf("%s,%04X\r\n",l->cur_lfn->name,l->cur_lfn->line_num-1);
	  }
	}
	if (tempi->plus_opcode) {
	  if (om>=OM_RB && om<=OM_RD)
	    last_opcode_byte=last_opcode_byte+tempa1->reg1;
	  else if (tempi->slash_value==8) {
	    if (a1>=ARGT_AL && a1<=ARGT_EAX)
	      last_opcode_byte=last_opcode_byte+tempa2->reg1;
	    else if (a2>=ARGT_AL && a2<=ARGT_EAX)
	      last_opcode_byte=last_opcode_byte+tempa1->reg1;
	  }
	}
	if ((a1>=ARGT_RM8 && a1<=ARGT_RM32) ||(a2>=ARGT_RM8 && a2<=ARGT_RM32)) {
	  if (a->use16)
	    has_address_prefix=true;
	  has_ModrM=true;
	  if (a2>=ARGT_RM8 && a2<=ARGT_RM32) {
	    tempa1=&a->arg2;
	    tempa2=&a->arg1;
	  }
	  if (tempi->slash_value==8) {
	    if (tempa2->just_seg)
	      ModrM=ModrM+(tempa2->seg<<3);
	    else {
	      if (tempa2->reg1==8)
		ModrM=ModrM+(tempa1->reg1<<3);
	      else
		ModrM=ModrM+(tempa2->reg1<<3);
	    }
	  }
	  if (tempa1->reg2==8 && tempa1->scale==1) {
	    if (tempa1->reg1!=8)
	      ModrM=ModrM+tempa1->reg1;
	  } else {
	    ModrM=ModrM+4;
	    has_SIB=true;
	    if (tempa1->scale==1)
	      SIB=0;
	    else if (tempa1->scale==2)
	      SIB=0x40;
	    else if (tempa1->scale==4)
	      SIB=0x80;
	    else if (tempa1->scale==8)
	      SIB=0xC0;
	      if (tempa1->reg2==8) {
		ModrM_complete=true;
		SIB=SIB+(tempa1->reg1<<3)+5;
		disp=tempa1->imm;
		disp_exp=tempa1->exp;
		disp_local_hash=tempa1->undef_local;
		disp_global_hash=tempa1->undef_global;
		disp_bytes=4;
	      } else {
		SIB=SIB+(tempa1->reg1<<3)+tempa1->reg2;
		if (tempa1->reg2==5 && !tempa1->imm_or_off_present && tempa1->indirect) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		  ModrM_complete=true;
		}
	      }
	  }
	  if (!ModrM_complete) {
	    if (tempa1->imm_or_off_present && tempa1->indirect && tempa1->reg1==8) {
	      ModrM=(ModrM & 0xF8)+5;
	      disp=tempa1->imm;
	      disp_exp=tempa1->exp;
	      disp_local_hash=tempa1->undef_local;
	      disp_global_hash=tempa1->undef_global;
	      disp_bytes=4;
	    } else {
	      if (tempa1->imm_or_off_present) {
		disp=tempa1->imm;
		disp_exp=tempa1->exp;
		disp_local_hash=tempa1->undef_local;
		disp_global_hash=tempa1->undef_global;
		if (!disp_exp && (disp>=-128) && (disp<=127)) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		} else if (a->use16) {
		  ModrM=ModrM+0x80;
		  disp_bytes=2;
		} else {
		  ModrM=ModrM+0x80;
		  disp_bytes=4;
		}
	      } else if (!tempa1->indirect) {
		has_address_prefix=false;
		ModrM=ModrM+0xC0;
	      } else {
		if (tempa1->reg1==5) {
		  ModrM=ModrM+0x40;
		  disp_bytes=1;
		}
	      }
	    }
	  }
	} else if (a1>=ARGT_MOFFS8 && a1<=ARGT_MOFFS32) {
	  disp=tempa1->imm;
	  disp_exp=tempa1->exp;
	  disp_local_hash=tempa1->undef_local;
	  disp_global_hash=tempa1->undef_global;
	  if (a->use16)
	    disp_bytes=2;
	  else
	    disp_bytes=4;
	  has_address_prefix=false;
	} else if (a2>=ARGT_MOFFS8 && a2<=ARGT_MOFFS32) {
	  disp=tempa2->imm;
	  disp_exp=tempa2->exp;
	  disp_local_hash=tempa2->undef_local;
	  disp_global_hash=tempa2->undef_global;
	  if (a->use16)
	    disp_bytes=2;
	  else
	    disp_bytes=4;
	  has_address_prefix=false;
	} else if ((a1>=ARGT_IMM8 && a1<=ARGT_IMM32) ||
	    (a1>=ARGT_UIMM8 && a1<=ARGT_UIMM32)) {
	  imm=tempa1->imm;
	  imm_exp=tempa1->exp;
	  imm_local_hash=tempa1->undef_local;
	  imm_global_hash=tempa1->undef_global;
	  if (a1==ARGT_IMM8 || a1==ARGT_UIMM8) imm_bytes=1;
	  else if (a1==ARGT_IMM16 || a1==ARGT_UIMM16) imm_bytes=2;
	  else imm_bytes=4;
	}
	if ((a2>=ARGT_IMM8 && a2<=ARGT_IMM32) ||
	    (a2>=ARGT_UIMM8 && a2<=ARGT_UIMM32)) {
	  imm=tempa2->imm;
	  imm_exp=tempa2->exp;
	  imm_local_hash=tempa2->undef_local;
	  imm_global_hash=tempa2->undef_global;
	  if (a2==ARGT_IMM8 || a2==ARGT_UIMM8) imm_bytes=1;
	  else if (a2==ARGT_IMM16 || a2==ARGT_UIMM16) imm_bytes=2;
	  else imm_bytes=4;
	}
      }
      byte_count=tempi->opcode_count+disp_bytes+imm_bytes;
      if (has_ModrM) byte_count++;
      if (has_SIB) byte_count++;

      if (byte_count<best_byte_count &&
	 !(tempi->dont_switch_modes &&
	  (has_address_prefix || has_operand_prefix))) {
	best_byte_count=byte_count;
	blast_opcode_byte=last_opcode_byte;
	btempi=tempi;
	bhas_ModrM=has_ModrM;
	bModrM=ModrM;
	bhas_SIB=has_SIB;
	bSIB=SIB;
	bhas_address_prefix=has_address_prefix;
	bhas_operand_prefix=has_operand_prefix;
	bis_default=is_default;
	bdisp=disp;
	bdisp_bytes=disp_bytes;
	bdisp_ip_rel=disp_ip_rel;
	bdisp_lit=disp_lit;
	bimm=imm;
	bimm_bytes=imm_bytes;
	bimm_ip_rel=imm_ip_rel;
	bimm_lit=imm_lit;
	bdisp_exp=disp_exp;
	bimm_exp=imm_exp;
	bdisp_local_hash=disp_local_hash;
	bdisp_global_hash=disp_global_hash;
	bimm_local_hash=imm_local_hash;
	bimm_global_hash=imm_global_hash;
      }
    }
  }
  if (best_byte_count<255) {
    if (bhas_operand_prefix)
      asm_store_code_byte(a,0x66); //Operand size override
    if (bhas_address_prefix  || (a->use16 && has_SIB))
      asm_store_code_byte(a,0x67); //Operand size override

    seg=8;
    if (argcount>1 && a->arg2.seg!=8 && !a->arg2.just_seg)
      seg=a->arg2.seg;
    else if (argcount>0 && a->arg1.seg!=8 && !a->arg1.just_seg)
      seg=a->arg1.seg;
    if (seg!=8) {
      if (seg==0)
	asm_store_code_byte(a,0x26); //ES:
      else if (seg==1)
	asm_store_code_byte(a,0x2E); //CS:
      else if (seg==2)
	asm_store_code_byte(a,0x36); //SS:
      else if (seg==3)
	asm_store_code_byte(a,0x3E); //DS:
      else if (seg==4)
	asm_store_code_byte(a,0x64); //FS:
      else
	asm_store_code_byte(a,0x65); //GS:
    }

    for (j=0;j<btempi->opcode_count-1;j++)
      asm_store_code_byte(a,btempi->opcode[j]);
    asm_store_code_byte(a,blast_opcode_byte);

    if (bhas_ModrM)
      asm_store_code_byte(a,bModrM);
    if (bhas_SIB)
      asm_store_code_byte(a,bSIB);

    if (bdisp_bytes) {
if (bdisp_lit && bdisp_global_hash) ? "GLBL LIT\r\n";
      bdisp_ip_rel=a->ip+bdisp_bytes+bimm_bytes;
      if (!asm_store_num(l,a,bdisp,false,bdisp_bytes,1,bdisp_ip_rel,bdisp_lit,
	  bdisp_exp,bdisp_local_hash,bdisp_global_hash,byte_avail))
	return false;
    }

    if (bimm_bytes) {
      bimm_ip_rel=a->ip+bimm_bytes;
      if (!asm_store_num(l,a,bimm,false,bimm_bytes,1,bimm_ip_rel,bimm_lit,
	  bimm_exp,bimm_local_hash,bimm_global_hash,byte_avail))
	return false;
    }
    return true;
  }
  lex_pop(l);
  display_asm_error(l,st_invalid_instruction);
  return false;
}


void define_bytes(lex_node *l,asm_ctrl_node *a,int num_bytes)
{
  bool is_question,is_dup;
  int i,dup_value;
  BYTE *exp;
  char *ptr;
  lex_undef_entry *undef_locals,*undef_globals;

  while (l->token && l->token!=';') {
    undef_locals=NULL;
    undef_globals=NULL;
    if (l->token==TK_STRING) {
      ptr=l->ident;
      i=l->ident_len-1;
      while (i--)
	asm_store_code_byte(a,*ptr++);
      lex(l);  //SKip STRING
    } else {
      if (l->token=='?') {
	is_question=true;
	lex(l);
      } else {
	is_question=false;
	if (get_expression(l,&exp))
	  lex_skip_eol(l);
	else {
	  if (l->flags & (LF_UNRESOLVED_LOCAL + LF_UNRESOLVED_GLOBAL)) {
	    if (l->flags & LF_UNRESOLVED_LOCAL) {
	      undef_locals=l->undef_hash_entry;
	      l->undef_hash_entry=NULL;
	      a->has_locals=true;
	    } else if (l->flags & LF_UNRESOLVED_GLOBAL) {
	      undef_globals=l->undef_hash_entry;
	      l->undef_hash_entry=NULL;
	    }
	  } else {
	    i=run_statement(exp);
	    delete_expression(exp);
	  }
	}
      }
      is_dup=false;
      if (l->token==TK_IDENT && l->hash_entry) {
	if ((l->hash_entry->type & AHT_KEYWORD) && (l->hash_entry->user0==AKW_DUP)) {
	  if (is_question)
	    display_asm_error(l,st_question_not_allowed);
	  else {
	    is_dup=true;
	    lex(l); //skip "DUP"
	    if (l->token!='(')
	      display_asm_error(l,st_expecting_left_paren);
	    else {
	      lex(l); //SKIP "("
	      if (l->token=='?') {
		is_question=true;
		lex(l);  //SKIP "?"
	      } else
		dup_value=asm_eval_expression(l);
	      if (l->token!=')')
		display_asm_error(l,st_expecting_right_paren);
	      else
		lex(l); //SKIP ')'
	    }
	  }
	}
      }
      if (is_dup) {
	if (undef_locals || undef_globals)
	  display_asm_error(l,st_undefined_DUP_count);
	else
	  asm_store_num(l,a,dup_value,is_question,num_bytes,i,0,1,exp,undef_locals,undef_globals,false);
      } else
	asm_store_num(l,a,i,is_question,num_bytes,1,0,1,exp,undef_locals,undef_globals,false);
    }
    if (l->token==',')
      lex(l);
  }
  if (l->token==';')
    lex(l);
  else
    display_asm_error(l,st_missing_semicolon);
}


exe_node *asm_buf(BYTE *buf)
{
  lex_node *l;
  asm_ctrl_node *a;
  int i,j,l,argcount;
  opcode_hash_entry *tempo;
  sys_hash_entry *temph,**temph1;
  DWORD saved_ip;
  exe_node *result;
  asm_code_block *tempc;
  bool is_literal;
  BYTE *file_buf;
  DWORD file_size;
  char *next_last_label;

  result=malloc(sizeof(exe_node));
  result->code=NULL;
  result->ie=NULL;
  l=lex_new(buf,LF_DONT_FREE_BUFFER+LF_ASM_EXPRESSIONS);

  a=malloc(sizeof(asm_ctrl_node));
  a->ip=0;
  a->num_code_bytes=0;
  a->local_hash=new_hash_table(16);
  a->global_hash=new_hash_table(2048);
  a->code=NULL;
  a->use16=false;
  a->local_unresolved=NULL;
  a->global_unresolved=NULL;

  a->global_hash->next=asm_hash;
  a->local_hash->next=a->global_hash;
  a->has_locals=false;

  a->list=false;
  a->display_col=0;
  a->last_line=NULL;
  a->last_lfn=NULL;
  a->last_ip=0;
  a->cur_line_num=0;
  a->last_label=NULL;

  l->hash_table_list=a->local_hash;
  l->define_hash_table=a->global_hash;
  l->local_hash_table=a->local_hash;
  l->global_hash_table=a->global_hash;
  lex(l);
  while (l->token) {
    if (a->list && a->last_line && a->last_lfn==l->cur_lfn) {
      for (i=a->display_col;i<8;i++)
	spaces(3);
      a->display_col=0;
      printf("%08X ",a->last_ip);
      lex_display_line(a->last_line);
      a->cur_line_num++;
      if (a->cur_line_num>(fs->window_bottom-fs->window_top-10)) {
	a->cur_line_num=0;
	press_a_key;
      }
    }
    a->last_ip=a->ip;
    a->last_line=l->cur_lfn->cur_line_start;
    a->last_lfn=l->cur_lfn;
    if (l->token==TK_IDENT && l->hash_entry) {
      if (l->hash_entry->type==AHT_KEYWORD) {
	i=l->hash_entry->user0;
	lex(l); //skip keyword
	if (i==AKW_EXTRN || i==AKW_IMPORT || i==AKW_EXPORT) {
	  while (l->token && (l->token==TK_IDENT || l->token=='#')) {
	    is_literal=false;
	    if (l->token=='#') {
	      is_literal=true;
	      lex(l);
	    }
	    if (l->token==TK_IDENT) {
	      if (temph=l->hash_entry) {
		temph=find_hash_entry(l->ident,a->global_hash,HTT_ALL);
		if (temph)
		  temph->type=AHT_SYS_SYMBOL+AHT_UNRESOLVED_GLOBAL;
	      }
	      if (!temph) {
		temph=malloc(sizeof(sys_hash_entry));
		temph->string=l->ident;
		l->ident=0;
		temph->type=AHT_SYS_SYMBOL+AHT_UNRESOLVED_GLOBAL;
		temph->user0=0;
		temph->user1=NULL;
		temph1=find_hash_bucket(temph->string,a->global_hash);
		temph->next=*temph1;
		*temph1=temph;
	      }
	      if (i==AKW_IMPORT)
		temph->type=temph->type | AHT_IMPORT;
	      else if (i==AKW_EXPORT)
		temph->type=temph->type | AHT_EXPORT;
	      if (is_literal)
		temph->type=temph->type | AHT_LITERAL;
	      lex(l); //skip ident
	      if (l->token==',')
		lex(l); //skip ','
	    }
	  }
	  if (l->token!=';')
	    display_asm_error(l,st_missing_semicolon);
	  else
	    lex(l); //skip ';';
	} else if (i==AKW_ALIGN) {
	  if (l->token!=TK_INTEGER)
	    display_asm_error(l,st_expecting_integer);
	  else {
	    i=(a->ip+l->cur_i-1) & (-l->cur_i);
	    lex(l);
	    if (l->token==',')
	      lex(l);
	    if (l->token==TK_INTEGER) {
	      for (j=a->ip;j<i;j++)
		asm_store_code_byte(a,l->cur_i);
	      lex(l);
	    } else
	      a->ip=i;
	  }
	} else if (i==AKW_END) {
	  ; //UNFINISHED
	} else if (i==AKW_DB)
	  define_bytes(l,a,1);
	else if (i==AKW_DW)
	  define_bytes(l,a,2);
	else if (i==AKW_DD)
	  define_bytes(l,a,4);
	else if (i==AKW_INSERT) {
	  if (l->token!=TK_STRING)
	    display_asm_error(l,st_expecting_string);
	  else {
	    file_buf=read_file(l->ident,&file_size);
	    if (file_buf) {
	      for (i=0;i<file_size;i++)
		asm_store_code_byte(a,file_buf[i]);
	      free(file_buf);
	    }
	    lex(l);
	  }
	} else if (i==AKW_LIST)
	  a->list=true;
	else if (i==AKW_NOLIST)
	  a->list=false;
	else if (i==AKW_ORG)
	  a->ip=asm_eval_expression(l);
	else if (i==AKW_STRUC) {
	  saved_ip=a->ip;
	  a->ip=asm_eval_expression(l);
	} else if (i==AKW_ENDS)
	  a->ip=saved_ip;
	else if (i==AKW_USE16)
	  a->use16=true;
	else if (i==AKW_USE32)
	  a->use16=false;
	else
	  display_asm_error(l,st_syntax_error);
      } else if (l->hash_entry->type & AHT_OPCODE) {
	tempo=l->hash_entry;
	lex_push(l);
	lex(l); //skip opcode
	argcount=0;
	if (tempo->ins[0].arg1) {
	  argcount++;
	  parse_asm_arg(l,a,&a->arg1,a->ip);
	  if (tempo->ins[0].arg2) {
	    argcount++;
	    if (l->token!=',')
	      display_asm_error(l,st_expecting_comma);
	    else {
	      lex(l); //skip ','
	      parse_asm_arg(l,a,&a->arg2,a->ip);
	    }
	  }
	}

	if (asm_code_instruction(l,a,tempo,argcount))
	  lex_sub_stack_ptr(l);
      } else if (l->hash_entry->type & AHT_SYS_SYMBOL) {
	if (l->hash_entry->type & AHT_UNRESOLVED_LOCAL) {
	  l->hash_entry->type=l->hash_entry->type & (-1-AHT_UNRESOLVED_LOCAL);
	  l->hash_entry->user0=a->ip;
	  lex(l); //Skip ident
	  if (l->token!=':')
	    display_asm_error(l,st_expecting_colon);
	  else
	    lex(l);
	} else if (l->hash_entry->type & AHT_UNRESOLVED_GLOBAL) {
	  if (l->hash_entry->type & AHT_IMPORT)
	    display_asm_error(l,st_attempt_define_import);
	  else {
	    l->hash_entry->type=l->hash_entry->type & (-1-AHT_UNRESOLVED_GLOBAL);
	    l->hash_entry->user0=a->ip;
	    next_last_label=l->hash_entry->string;
	    lex(l); //Skip ident
	    if (l->token!=':' && l->token!=TK_DOUBLE_COLON)
	      display_asm_error(l,st_expecting_colon);
	    else {
	      if (l->token==TK_DOUBLE_COLON) {
		l->hash_entry->type=l->hash_entry->type |  AHT_EXPORT;
		resolve_locals(l,a);
		a->last_label=next_last_label;
	      }
	      lex(l);
	      if (l->token=='#') {
		lex(l);
		l->hash_entry->type=l->hash_entry->type | AHT_LITERAL | AHT_EXPORT;
	      }
	    }
	  }
	} else
	  display_asm_error(l,st_redefinition);
      } else
	display_asm_error(l,st_syntax_error);
    } else {
      if (l->token==TK_IDENT) {
	temph=malloc(sizeof(sys_hash_entry));
	temph->string=l->ident;
	l->ident=0;
	temph->type=AHT_SYS_SYMBOL;
	temph->user0=a->ip;
	temph->user1=NULL;
	lex(l);  //Skip ident
	if (l->token==':' || l->token==TK_DOUBLE_COLON) {
	  if (temph->string[0]=='@' && temph->string[1]=='@') {
	    temph1=find_hash_bucket(temph->string,a->local_hash);
	    a->has_locals=true;
	  } else
	    temph1=find_hash_bucket(temph->string,a->global_hash);
	  temph->next=*temph1;
	  *temph1=temph;
	  if (l->token==TK_DOUBLE_COLON) {
	    temph->type=temph->type | AHT_EXPORT; //Export
	    resolve_locals(l,a);
	    a->last_label=temph->string;
	    lex(l);
	  } else {
	    lex(l);
	    if (l->token=='#') {
	      temph->type=temph->type | AHT_EXPORT | AHT_LITERAL; //Export
	      resolve_locals(l,a);
	      a->last_label=temph->string;
	      lex(l);
	    }
	  }
	} else {
	  display_asm_error(l,st_expecting_colon);
	  free(temph->string);
	  free(temph);
	}
      } else
	display_asm_error(l,st_syntax_error);
    }
  }
  resolve_locals(l,a);
  ? "\r\n";
  resolve_globals(l,a,result);
  lex_delete(l);
  delete_hash_table(a->local_hash);
  delete_hash_table(a->global_hash);

  if (!a->num_code_bytes)
    result->code=NULL;
  else {
    result->code=malloc(a->num_code_bytes);
    result->code_bytes=a->num_code_bytes;
    tempc=a->code;
    j=0;
    l=a->num_code_bytes;
    while (tempc) {
      i=l;
      if (i>ASM_CODE_BLOCK_SIZE)
	i=ASM_CODE_BLOCK_SIZE;
      memcpy(result->code+j,tempc->body,i);
      j=j+i;
      l=l-i;
      tempc=tempc->next;
    }
  }
  delete_linked_list(a->code);
  free(a);
  echo(off);
  return result;
}

void asm(char *filename)
{
  char fbuf[512],fbuf2[512];
  exe_node *e;
  BYTE *buf,*header=malloc(0x20000),*ptr;
  int i;
  exe_import_export_node *tempe,*tempe1;

  strcpy(fbuf,filename);
  if (!occurrences(fbuf,'.'))
    strcat(fbuf,".ASM");
  sprintf(fbuf2,"#include \"%s\"",fbuf);
  e=asm_buf(fbuf2);

  tempe=e->ie;
  ptr=header+4;
  while (tempe) {
    tempe1=tempe->next;
    *ptr++=tempe->type;
    memcpy(ptr,&tempe->ip,4);
    ptr=ptr+4;
    if (tempe->string) {
/*	if (tempe->type<=eie_lit_export)
	?"Export:",tempe->string,"\r\n";
      else
	?"Import:",tempe->string,"\r\n";
*/	i=strlen(tempe->string);
      memcpy(ptr,tempe->string,i+1);
      free(tempe->string);
      ptr=ptr+i+1;
    } else
      *ptr++=0;
    free(tempe);
    tempe=tempe1;
  }
  *ptr++=eie_end;
  i=(ptr-header+15) & 0xFFFFFFF0;
  memcpy(header,&i,4);
  buf=malloc(i+e->code_bytes);
  memcpy(buf,header,i);
  memcpy(buf+i,e->code,e->code_bytes);
  remove_last_segment(fbuf,'.',fbuf2);
  strcat(fbuf,".BIN");
  save_file(fbuf,buf,i+e->code_bytes);
  free(buf);
  free(e->code);
  free(e);
}


void load(char *filename)
{
  char fbuf[512];
  BYTE *buf,*code,*ptr,etype,*bb;
  char *sptr;
  WORD *ww;
  DWORD *dd,size;
  int i;
  sys_hash_entry *temph,**temph1,*saved_hash_ptr=NULL;

  strcpy(fbuf,filename);
  if (!occurrences(fbuf,'.'))
    strcat(fbuf,".BIN");
  buf=read_file(fbuf,&size); //Unfinished: align to para
  dd=buf;
  code=buf+*dd;
  size=size-*dd;

  ptr=buf+4;
  while ((etype=*ptr++)!=eie_end) {
    dd=ptr;
    ptr=ptr+4;
    i=*dd;
    sptr=ptr;
    ptr=ptr+strlen(sptr)+1;
    if (etype<=eie_lit_export) {
      temph=malloc(sizeof(sys_hash_entry));
      temph->string=new_string(sptr);
      temph->type=HTT_SYS_SYMBOL;
      if (etype==eie_lit_export)
	temph->user0=i;
      else
	temph->user0=i+code;
      temph1=find_hash_bucket(temph->string,fs->hash_table);
      temph->next=*temph1;
      *temph1=temph;
    } else {
      if (*sptr)
	saved_hash_ptr=find_hash_entry(sptr,fs->hash_table,HTT_ALL);
      if (!saved_hash_ptr)
	? "Unresolved Reference:",sptr,"\r\n";
      else {
	bb=code+i;
	ww=code+i;
	dd=code+i;
	if (etype & 1)
	  i=saved_hash_ptr->user0;
	else
	  i=saved_hash_ptr->user0-code;
	if (etype<=eie_lit_BYTE)
	  *bb=i+*bb;
	else if (etype<=eie_lit_WORD)
	  *ww=i+*ww;
	else if (etype<=eie_lit_DWORD)
	  *dd=i+*dd;
      }
    }
  }
}

void gsym(char *name)
{
  BYTE *code=sym(name);
  if (code)
    ? run_statement(code),"\r\n";
  else
    ? "Not Defined\r\n";
}

if (!asm_hash)
  load_asm_hash;
