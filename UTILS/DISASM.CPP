instruction_entry **disasm_table=NULL;
int disasm_table_entries=0;

int compare_instruction_entries(instruction_entry **tempi1,instruction_entry **tempi2)
{
  int i1,i2,j=0,result=0;
  while (true) {
    if (j<(*tempi1)->opcode_count && j<(*tempi2)->opcode_count) {
      i1=(*tempi1)->opcode[j];
      i2=(*tempi2)->opcode[j];
      result=i1-i2;
      if (result) return result;
      j++;
    } else {
      i1=(*tempi1)->slash_value;
      i2=(*tempi2)->slash_value;
      result=i1-i2;
      if (result) return result;

      i1=(*tempi1)->operand_size32;
      i2=(*tempi2)->operand_size32;
      result=i1-i2;
      if (result) return result;

      return result;
    }
  }
}

void load_disasm_table()
{
  sys_hash_table *h=asm_hash;
  opcode_hash_entry *temph;
  instruction_entry *tempi;
  DWORD count=0;
  int i,j,k;

  for (i=0;i<=h->mask;i++) {
    temph=&h->body+4*i;
    while (*temph) {
      if (temph->type==AHT_OPCODE)
        count=count+temph->instruction_entry_count;
      temph=temph->next;
    }
  }
  disasm_table_entries=count;

  j=0;
  disasm_table=malloc(count*sizeof(DWORD));
  for (i=0;i<=h->mask;i++) {
    temph=&h->body+4*i;
    while (*temph) {
      if (temph->type==AHT_OPCODE) {
        tempi=&temph->ins;
	for (k=0;k<temph->instruction_entry_count;k++) {
          disasm_table[j++]=tempi;
          tempi=tempi+sizeof(instruction_entry);
        }
      }
      temph=temph->next;
    }
  }

  qsort(disasm_table,count,sizeof(DWORD),&compare_instruction_entries);
}

if (!asm_hash)
  load_asm_hash;
if (!disasm_table)
  load_disasm_table;

sys_hash_entry *locate_program_section(int add,int *offset)
{
  sys_hash_entry *result=NULL,*temph;
  sys_hash_table *h=fs->hash_table;
  class_node *tempc;
  int i,best=0x7FFFFFFF,j;

  while (h) {
    for (i=0;i<=h->mask;i++) {
      temph=&h->body+4*i;
      while (*temph) {
        if (temph->type==HTT_FUNCTION) {
          tempc=temph;
          j=tempc->executable_address;
        } else
          j=temph->user0;
        if (temph->type==HTT_SYS_SYMBOL ||
            temph->type==HTT_FUNCTION) {
          if (add-j>=0 && add-j<best) {
            best=add-j;
            result=temph;
          }
        }
        temph=temph->next;
      }
    }
    h=h->next;
  }
  *offset=best;
  return result;
}


void dasm_ins(char *buf,BYTE **add,bool use16=false)
{
  char buf2[256],seg_overrides[40],arg1_st[256],arg2_st[256],
       *a1_st,*a2_st;
  BYTE *ip=*add;
  BYTE b,*bb;
  WORD *ww;
  DWORD *dd;
  int disp,imm,offset;
  bool cont=true,cont2,opsize=false,opadd=false;
  int i,j,k,l,m,o1,o2,a1,a2,aa1,aa2,arg1_size=0,arg2_size=0,a1_size,a2_size,
      ModrM=-1,SIB=-1,scale,r1,r2,
      Mod=-1,RM1=-1,RM2=-1;
  instruction_entry *tempi;
  opcode_hash_entry *tempo,dummy;
  sys_hash_entry *temph;

  *arg1_st=0;
  *arg2_st=0;
  a1_st=arg1_st;
  a2_st=arg2_st;
  if (temph=locate_program_section(ip,&offset)) 
    sprintf(buf,"%s,%X ",temph->string,offset);
  else
    sprintf(buf,"%08X ",ip);
  *seg_overrides=0;
  while (cont) {
    b=*(ip++);
         if (b==0x2E) strcat(seg_overrides,"CS:");
    else if (b==0x36) strcat(seg_overrides,"SS:");
    else if (b==0x3E) strcat(seg_overrides,"DS:");
    else if (b==0x26) strcat(seg_overrides,"ES:");
    else if (b==0x64) strcat(seg_overrides,"FS:");
    else if (b==0x65) strcat(seg_overrides,"GS:");
    else if (b==0x66) opsize=true;
    else if (b==0x67) opadd=true;
    else cont=false;
  }
  ip--;

  cont=true;
  i=0;
  j=disasm_table_entries-1;
  while (cont) {
    k=(i+j)/2;
    tempi=disasm_table[k];
    m=0;
    l=0;
    cont2=true;
    while (cont2) {
      if (l<tempi->opcode_count) {
        o1=ip[l];
        o2=tempi->opcode[l];
        m=o1-o2;
	l++;
	if (m) cont2=false;
      } else {
        if (tempi->slash_value<8) {
          o1=(ip[l] >> 3) & 7;
          o2=tempi->slash_value;
          m=o1-o2;
          if (!m) {
            if (use16 ^ opsize) {
              if (tempi->operand_size32)
                m=-1;
            } else {
              if (tempi->operand_size16)
                m=1;
            }
          } 
          cont2=false;
        } else {
          m=0;
          if (use16 ^ opsize) {
            if (tempi->operand_size32)
              m=-1;
          } else {
            if (tempi->operand_size16)
              m=1;
          }
	  cont2=false;
        }
      }
    }

    if (m>0) {
      if (k==i) 
        i=j;
      else
        i=k;
    } else if (m<0) {
      if (i==j) {
        k=i-1;
        if (k<0) k=0;
        cont=false;
      } else if (j-i<=2) {
        k=i;
        cont=false;
      } else
        j=k;
    } else 
      cont=false;
  }
  tempi=disasm_table[k];
  ip=ip+tempi->opcode_count;
  tempo=tempi-(tempi->ins_entry_num*sizeof(instruction_entry))+&dummy-&dummy.ins;
  strcat(buf,tempo->string);

  a1=tempi->arg1;
  a2=tempi->arg2;
  aa1=a1;
  aa2=a2;

  strcpy(buf2,point_at_list_entry(a1,systext(st_argument_types)));
  if (strstr(buf2,"8"))
    arg1_size=1;
  else if (strstr(buf2,"16"))
    arg1_size=2;
  else if (strstr(buf2,"32"))
    arg1_size=4;

  strcpy(buf2,point_at_list_entry(a2,systext(st_argument_types)));
  if (strstr(buf2,"8"))
    arg2_size=1;
  else if (strstr(buf2,"16"))
    arg2_size=2;
  else if (strstr(buf2,"32"))
    arg2_size=4;

  a1_size=arg1_size;
  a2_size=arg2_size;  
  if (tempi->plus_opcode && (tempi->slash_value==8 ||
      (tempi->opcode_modifier>=OM_RB && tempi->opcode_modifier<=OM_RD))) {
    ip--;
    RM1=*(ip++) - tempi->opcode[tempi->opcode_count-1];
    *buf2=0;
    if (a1>=ARGT_R8 && a1<=ARGT_R32) {
      if (a1_size==1)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_byte_regs)));
      else if (a1_size==2)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_word_regs)));
      else if (a1_size==4)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_dword_regs)));
      strcat(a1_st,buf2);
    } else {
      if (a2_size==1)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_byte_regs)));
      else if (a2_size==2)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_word_regs)));
      else if (a2_size==4)
        sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_dword_regs)));
      strcat(a2_st,buf2);
    }
  }
  if ((a1>=ARGT_RM8 && a1<=ARGT_RM32) ||
      (a2>=ARGT_RM8 && a2<=ARGT_RM32)) {
    if (a2>=ARGT_RM8 && a2<=ARGT_RM32) {
      a1_st=arg2_st;
      a2_st=arg1_st;
      a1_size=arg2_size;
      a2_size=arg1_size;  
      aa1=a2;
      aa2=a1;
    }
    strcat(a1_st,seg_overrides);
    ModrM=*(ip++);
    Mod=(ModrM>>6) & 3;
    RM1=ModrM & 7;
    RM2=(ModrM>>3) & 7;
    if (Mod<3 && RM1==4) {
      SIB=*(ip++);
    }
    if (Mod==1) {
      bb=ip;
      disp=*bb;
      sprintf(buf2,"%02X",disp);
      strcat(a1_st,buf2);
      ip++;
    }
    if (Mod==2) {
      dd=ip;
      disp=*dd;
      sprintf(buf2,"%08X",disp);
      strcat(a1_st,buf2);
      ip=ip+4;
    }
    if (tempi->slash_value<8)
      RM2=-1;
    else {
      *buf2=0;
      if (aa2==ARGT_SREG)
        sprintf(buf2,"%s",point_at_list_entry(RM2,systext(st_seg_regs)));
      else if (!(aa2>=ARGT_IMM8 && aa2<=ARGT_IMM32) &&
               !(aa2>=ARGT_UIMM8 && aa2<=ARGT_UIMM32)) {
        if (a2_size==1)
          sprintf(buf2,"%s",point_at_list_entry(RM2,systext(st_byte_regs)));
        else if (a2_size==2)
          sprintf(buf2,"%s",point_at_list_entry(RM2,systext(st_word_regs)));
        else if (a2_size==4)
          sprintf(buf2,"%s",point_at_list_entry(RM2,systext(st_dword_regs)));
      }
      strcat(a2_st,buf2);
    }
    if (RM1==5 && Mod==0) {
      dd=ip;
      disp=*dd;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X] ",temph->string,offset);
      else
        sprintf(buf2,"[%08X] ",disp);
      strcat(a1_st,buf2);
      ip=ip+4;
      RM1=-1;
    } else {
      if (Mod<3) {
        if (RM1==4) {
          RM1=-1;
          r1=SIB & 7;
          r2=(SIB >> 3) & 7;
          scale=(SIB >> 6) &3;
          if (scale==3)
	    scale=8;
          else if (scale==2)
	    scale=4;
          else if (scale==1)
	    scale=2;
          else
	    scale=1;
          if (r1==5 && Mod==0) {
            dd=ip;
            disp=*dd;
            sprintf(buf2,"%08X",disp);
            strcat(a1_st,buf2);
            ip=ip+4;
            sprintf(buf2,"[%s*%d]",point_at_list_entry(r2,systext(st_dword_regs)),scale);
            strcat(a1_st,buf2);
          } else {
            sprintf(buf2,"[%s+%s*%d]",point_at_list_entry(r1,systext(st_dword_regs)),
	      point_at_list_entry(r2,systext(st_dword_regs)),scale);
            strcat(a1_st,buf2);
          }
        } else {
          *buf2=0;
          if (use16 ^ opadd)
            sprintf(buf2,"[%s]",point_at_list_entry(RM1,systext(st_word_regs)));
          else 
            sprintf(buf2,"[%s]",point_at_list_entry(RM1,systext(st_dword_regs)));
          strcat(a1_st,buf2);
        }
      } else {
        *buf2=0;
        if (a1_size==1)
          sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_byte_regs)));
        else if (a1_size==2)
          sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_word_regs)));
        else if (a1_size==4)
          sprintf(buf2,"%s",point_at_list_entry(RM1,systext(st_dword_regs)));
        strcat(a1_st,buf2);
      }
    } 
  }
  if (a1>=ARGT_MOFFS8 && a1<=ARGT_MOFFS32) {
    if (arg1_size==1) {
      bb=ip;
      disp=*bb;
      ip++;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%02X]",disp);
      strcat(arg1_st,buf2);
    } else if (use16 ^ opadd) {
      ww=ip;
      disp=*ww;
      ip=ip+2;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%04X]",disp);
      strcat(arg1_st,buf2);
    } else {
      dd=ip;
      disp=*dd;
      ip=ip+4;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%08X]",disp);
      strcat(arg1_st,buf2);
    }
  }

  if (a2>=ARGT_MOFFS8 && a2<=ARGT_MOFFS32) {
    if (arg2_size==1) {
      bb=ip;
      disp=*bb;
      ip++;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%02X]",disp);
      strcat(arg2_st,buf2);
    } else if (use16 ^ opadd) {
      ww=ip;
      disp=*ww;
      ip=ip+2;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%04X]",disp);
      strcat(arg2_st,buf2);
    } else {
      dd=ip;
      disp=*dd;
      ip=ip+4;
      if (temph=locate_program_section(disp,&offset)) 
        sprintf(buf2,"[%s,%X]",temph->string,offset);
      else
        sprintf(buf2,"[%08X]",disp);
      strcat(arg2_st,buf2);
    }
  }

  if (a1==ARGT_IMM8 || a1==ARGT_UIMM8) {
    bb=ip;
    imm=*bb;
    sprintf(buf2,"%02X",imm);
    strcat(arg1_st,buf2);
    ip++;
  } else if (a1==ARGT_IMM16 || a1==ARGT_UIMM16) {
    ww=ip;
    imm=*ww;
    sprintf(buf2,"%04X",imm);
    strcat(arg1_st,buf2);
    ip=ip+2;
  } else if (a1==ARGT_IMM32 || a1==ARGT_UIMM32) {
    dd=ip;
    imm=*dd;
    sprintf(buf2,"%08X",imm);
    strcat(arg1_st,buf2);
    ip=ip+4;
  } else if (a1==ARGT_REL8) {
    bb=ip;
    disp=*bb;
    if (disp>=0x80)
      disp=disp | 0xFFFFFF00;
    ip++;
    disp=disp+ip;
    if (temph=locate_program_section(disp,&offset)) 
      sprintf(buf2,"%s,%X ",temph->string,offset);
    else
      sprintf(buf2,"%08X ",disp);
    strcat(arg1_st,buf2);
  } else if (a1==ARGT_REL16) {
    ww=ip;
    disp=*ww;
    if (disp>=0x8000)
      disp=disp | 0xFFFF0000;
    ip=ip+2;
    disp=disp+ip;
    if (temph=locate_program_section(disp,&offset)) 
      sprintf(buf2,"%s,%X ",temph->string,offset);
    else
      sprintf(buf2,"%08X ",disp);
    strcat(arg1_st,buf2);
  } else if (a1==ARGT_REL32) {
    dd=ip;
    disp=*dd;
    ip=ip+4;
    disp=disp+ip;
    if (temph=locate_program_section(disp,&offset)) 
      sprintf(buf2,"%s,%X ",temph->string,offset);
    else
      sprintf(buf2,"%08X ",disp);
    strcat(arg1_st,buf2);
  } else if (a1==ARGT_CL)
    strcat(arg1_st,"CL");
  else if (a1==ARGT_AL)
    strcat(arg1_st,"AL");
  else if (a1==ARGT_AX)
    strcat(arg1_st,"AX");
  else if (a1==ARGT_EAX)
    strcat(arg1_st,"EAX");
  else if (a1==ARGT_DX)
    strcat(arg1_st,"DX");
  else if (a1==ARGT_SS)
    strcat(arg1_st,"SS");
  else if (a1==ARGT_DS)
    strcat(arg1_st,"DS");
  else if (a1==ARGT_ES)
    strcat(arg1_st,"ES");
  else if (a1==ARGT_FS)
    strcat(arg1_st,"FS");
  else if (a1==ARGT_GS)
    strcat(arg1_st,"GS");
  else if (a1==ARGT_CS)
    strcat(arg1_st,"CS");

  if (a2==ARGT_IMM8 || a2==ARGT_UIMM8) {
    bb=ip;
    imm=*bb;
    sprintf(buf2,"%02X",imm);
    strcat(arg2_st,buf2);
    ip++;
  } else if (a2==ARGT_IMM16 || a2==ARGT_UIMM16) {
    ww=ip;
    imm=*ww;
    sprintf(buf2,"%04X",imm);
    strcat(arg2_st,buf2);
    ip=ip+2;
  } else if (a2==ARGT_IMM32 || a2==ARGT_UIMM32) {
    dd=ip;
    imm=*dd;
    sprintf(buf2,"%08X",imm);
    strcat(arg2_st,buf2);
    ip=ip+4;
  } else if (a2==ARGT_CL)
    strcat(arg2_st,"CL");
  else if (a2==ARGT_AL)
    strcat(arg2_st,"AL");
  else if (a2==ARGT_AX)
    strcat(arg2_st,"AX");
  else if (a2==ARGT_EAX)
    strcat(arg2_st,"EAX");
  else if (a2==ARGT_DX)
    strcat(arg2_st,"DX");
  else if (a2==ARGT_SS)
    strcat(arg2_st,"SS");
  else if (a2==ARGT_DS)
    strcat(arg2_st,"DS");
  else if (a2==ARGT_ES)
    strcat(arg2_st,"ES");
  else if (a2==ARGT_FS)
    strcat(arg2_st,"FS");
  else if (a2==ARGT_GS)
    strcat(arg2_st,"GS");
  else if (a2==ARGT_CS)
    strcat(arg2_st,"CS");

  if (*arg1_st) {
    strcat(buf,"\t");
    strcat(buf,arg1_st);
  }
  if (*arg2_st) {
    strcat(buf,",");
    strcat(buf,arg2_st);
  }
  strcat(buf,"\r\n");
  *add=ip;
}


BYTE *dasm(BYTE *ip,int count=20,bool use16=false)
{
  char buf[512];
  int i;

  for (i=0;i<count;i++) {
    dasm_ins(buf,&ip,use16);
    ? buf;
  }

  return ip;
}



