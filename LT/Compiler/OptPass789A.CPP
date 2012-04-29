U64 OptPass789A(LexStruct *lx,FunStruct *fun_class,
   OptRegStruct *register_offsets,U8 *buf,DbgInfo **_dbg,IndentInfo **_indent,
   I64 *min_line,I64 *max_line)
{
  IntermediateCode *tempi;
  I64 code,i,cnt,num_lines,ip=0,ip2;
  I64 min_line2=-1,last_line=0;
  CbMiscStruct *lb,*lb1;
  ExeStruct *tempex;
  ExeAbsAddrStruct *tempa;
  ExeImportExportStruct *tempe;
  ExeHeapGlblStruct *temphg;
  ExeHeapGlblRefStruct *temphgr;
  DbgInfo *dbg=NULL;
  IndentInfo *indent=NULL;
  AsmCtrlStruct *a=lx->a;
  BoolI8 short_jmp;
  I64 *qq;
  IcArg saved_p123[3];
  ClassStruct *tempc;
  FunStruct *tempf;
  GlblVarStruct *tempg;
  ExternUsage *tempeu;

  if (buf) {
    if (*max_line>=*min_line && *max_line-*min_line<10000) {
      num_lines=*max_line-*min_line+1;
      dbg=CAlloc(offset(DbgInfo.body)+sizeof(U0 *)*(num_lines+1));
      dbg->min_line=*min_line;
      dbg->max_line=*max_line;
      indent=MAlloc(offset(IndentInfo.body)+sizeof(U8)*(num_lines+1));
      MemSet(indent->body,0xFF,num_lines+1);
      indent->min_line=*min_line;
      indent->max_line=*max_line;
    }
    if (_dbg)
      *_dbg=dbg;
    if (_indent)
      *_indent=indent;
  }
  if (Bt(&lx->flags,Lf_OPT_TRACE_PRESENT))
    if (Bt(&lx->pass_trace_mask,lx->pass))
      "$BK,1$$FG,LTRED$$IV,1$This code gets merged together and patched.\n$FG$$IV,0$$BK,0$";

  lx->last_float_op_ic=NULL;

  tempi=lx->cbh.cb_in_ptr;
  if (tempi->ic_opcode==EC_START) {
    tempi->ic_cnt=0;
    tempi->ic_last_cnt=-1;
    tempi++;
  }
  while (code=tempi->ic_opcode) {
    if (code>EC_END_EXP) {
      if (tempi->ic_flags&ICF_TRACE)
	if (Bt(&lx->pass_trace_mask,lx->pass))
	  CbPutIc(tempi);
      ip2=ip;
      if (lx->flags&LF_STATIC_COMPILE)
	ip2+=a->ip;
      else
	ip2+=buf;
      lx->cur_ic_float_op_num=0;
      if (!(tempi->ic_flags &ICF_CODE_FINAL)) {
	tempi->ic_flags=tempi->ic_flags&~(ICF_PREVIOUS_DELETED|ICF_DONT_RESTORE)|ICF_CODE_FINAL;
	if (lx->pass==7) {
	  tempi->ic_ext_body=NULL;
	  lx->dont_push_float=Btr(&tempi->ic_flags,ICf_DONT_PUSH_FLOAT0);
	}
	MemCpy(saved_p123,&tempi->p1,3*sizeof(IcArg));
	if (cmp_arg_cnt[code]==2) {
	  if (tempi->p1.type&TY_MASK==TY_REG &&
	      tempi->p1.reg==CREG_RAX)
	    tempi->ic_flags|=ICF_P1_FIRST;
	}
	tempi->ic_cnt=0;
	tempi->ic_last_cnt=-1;
	if (tempi->ic_flags & ICF_P1_FIRST) {
	  if (tempi->p1.type&TY_MASK) {
	    if (tempi->ic_flags & ICF_P1_TO_F64) {
	      IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,FALSE,CN_P1,ip2);
	      tempi->p1.type=TY_REG+IT_I64;
	      tempi->p1.reg=CREG_RDX;
	      tempi->p1.disp=0;
	      tempi->ic_flags&=~ICF_P1_FIRST;
	    } else if (tempi->ic_flags & ICF_P1_TO_INT) {
	      IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,TRUE,CN_P1,ip2);
	      tempi->p1.type=TY_REG+IT_I64;
	      tempi->p1.reg=CREG_RDX;
	      tempi->p1.disp=0;
	      tempi->ic_flags&=~ICF_P1_FIRST;
	    }
	  }
	  if (tempi->p2.type&TY_MASK) {
	    if (tempi->ic_flags & ICF_P2_TO_F64) {
	      if (tempi->ic_flags&ICF_P1_FIRST) {
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
		tempi->p1.type=TY_REG+IT_I64;
		tempi->p1.reg=CREG_RDX;
		tempi->p1.disp=0;
		tempi->ic_flags&=~ICF_P1_FIRST;
	      }
	      IcFCvt(lx,tempi,CREG_RAX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,FALSE,CN_P2,ip2);
	      tempi->p2.type=TY_REG+IT_I64;
	      tempi->p2.reg=CREG_RAX;
	      tempi->p2.disp=0;
	    } else if (tempi->ic_flags & ICF_P2_TO_INT) {
	      if (tempi->ic_flags&ICF_P1_FIRST) {
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
		tempi->p1.type=TY_REG+IT_I64;
		tempi->p1.reg=CREG_RDX;
		tempi->p1.disp=0;
		tempi->ic_flags&=~ICF_P1_FIRST;
	      }
	      IcFCvt(lx,tempi,CREG_RAX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,TRUE,CN_P2,ip2);
	      tempi->p2.type=TY_REG+IT_I64;
	      tempi->p2.reg=CREG_RAX;
	      tempi->p2.disp=0;
	    }
	  }
	} else {
	  if (tempi->p3.type&TY_MASK) {
	    if (tempi->ic_flags & ICF_P3_TO_F64) {
	      IcFCvt(lx,tempi,CREG_RAX,tempi->p3.type,tempi->p3.reg,tempi->p3.disp,FALSE,CN_P3,ip2);
	      tempi->p3.type=TY_REG+IT_I64;
	      tempi->p3.reg=CREG_RAX;
	      tempi->p3.disp=0;
	    } else if (tempi->ic_flags & ICF_P3_TO_INT) {
	      IcFCvt(lx,tempi,CREG_RAX,tempi->p3.type,tempi->p3.reg,tempi->p3.disp,TRUE,CN_P3,ip2);
	      tempi->p3.type=TY_REG+IT_I64;
	      tempi->p3.reg=CREG_RAX;
	      tempi->p3.disp=0;
	    }
	    if (tempi->ic_flags & (ICF_P3_TO_F64 | ICF_P3_TO_INT)) {
	      if (tempi->p2.type&TY_MASK) {
		if (tempi->ic_flags & ICF_P2_TO_F64) {
		  IcFCvt(lx,tempi,CREG_RDX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,FALSE,CN_P2,ip2);
		  tempi->p2.type=TY_REG+IT_I64;
		  tempi->p2.reg=CREG_RDX;
		  tempi->p2.disp=0;
		} else if (tempi->ic_flags & ICF_P2_TO_INT) {
		  IcFCvt(lx,tempi,CREG_RDX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,TRUE,CN_P2,ip2);
		  tempi->p2.type=TY_REG+IT_I64;
		  tempi->p2.reg=CREG_RDX;
		  tempi->p2.disp=0;
		}
	      }
	      if (tempi->ic_flags & (ICF_P2_TO_F64 | ICF_P2_TO_INT)) {
		if (tempi->p1.type&TY_MASK) { //TODO: RCX might cause problems
		  if (tempi->ic_flags & ICF_P1_TO_F64) {
		    IcFCvt(lx,tempi,CREG_RCX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,FALSE,CN_P1,ip2);
		    tempi->p1.type=TY_REG+IT_I64;
		    tempi->p1.reg=CREG_RCX;
		    tempi->p1.disp=0;
		  } else if (tempi->ic_flags & ICF_P1_TO_INT) {
		    IcFCvt(lx,tempi,CREG_RCX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,TRUE,CN_P1,ip2);
		    tempi->p1.type=TY_REG+IT_I64;
		    tempi->p1.reg=CREG_RCX;
		    tempi->p1.disp=0;
		  }
		}
	      } else {
		if (tempi->p1.type&TY_MASK) {
		  if (tempi->ic_flags & ICF_P1_TO_F64) {
		    IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,FALSE,CN_P1,ip2);
		    tempi->p1.type=TY_REG+IT_I64;
		    tempi->p1.reg=CREG_RDX;
		    tempi->p1.disp=0;
		  } else if (tempi->ic_flags & ICF_P1_TO_INT) {
		    IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,TRUE,CN_P1,ip2);
		    tempi->p1.type=TY_REG+IT_I64;
		    tempi->p1.reg=CREG_RDX;
		    tempi->p1.disp=0;
		  }
		}
	      }
	    } else
	      goto p2_RAX_p1_RDX;
	  } else {
	    p2_RAX_p1_RDX:
	    if (tempi->p2.type&TY_MASK) {
	      if (tempi->ic_flags & ICF_P2_TO_F64) {
		IcFCvt(lx,tempi,CREG_RAX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,FALSE,CN_P2,ip2);
		tempi->p2.type=TY_REG+IT_I64;
		tempi->p2.reg=CREG_RAX;
		tempi->p2.disp=0;
	      } else if (tempi->ic_flags & ICF_P2_TO_INT) {
		IcFCvt(lx,tempi,CREG_RAX,tempi->p2.type,tempi->p2.reg,tempi->p2.disp,TRUE,CN_P2,ip2);
		tempi->p2.type=TY_REG+IT_I64;
		tempi->p2.reg=CREG_RAX;
		tempi->p2.disp=0;
	      }
	    }
	    if (tempi->p1.type&TY_MASK) {
	      if (tempi->ic_flags & ICF_P1_TO_F64) {
		IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,FALSE,CN_P1,ip2);
		tempi->p1.type=TY_REG+IT_I64;
		tempi->p1.reg=CREG_RDX;
		tempi->p1.disp=0;
	      } else if (tempi->ic_flags & ICF_P1_TO_INT) {
		IcFCvt(lx,tempi,CREG_RDX,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,TRUE,CN_P1,ip2);
		tempi->p1.type=TY_REG+IT_I64;
		tempi->p1.reg=CREG_RDX;
		tempi->p1.disp=0;
	      }
	    }
	  }
	}
	nobound_switch (code) {
	  case EC_ABS_ADDR:
	    IcU16(tempi,0xB848);
	    IcU64(tempi,tempi->ic_data);
	    if (buf && lx->flags&LF_STATIC_COMPILE &&
	      !(lx->flags&(LF_NO_ABSS|LF_ASM_EXPRESSIONS))) {
	      tempa=CAlloc(sizeof(ExeAbsAddrStruct));
	      tempa->next=a->abss;
	      tempa->type=EAA_ADD_U64;
	      a->abss=tempa;
	      tempa->ip=ip2+tempi->ic_cnt-8;
	    }
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_REG+IT_I64,CREG_RAX,0,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_HEAP_GLBL:
	    IcU16(tempi,0xB848);
	    IcU64(tempi,0);
	    temphg=tempi->ic_data;
	    if (buf && lx->flags&LF_STATIC_COMPILE &&
	      !(lx->flags&(LF_NO_ABSS|LF_ASM_EXPRESSIONS))) { //is this necessary--flags?
	      temphgr=CAlloc(sizeof(ExeHeapGlblRefStruct));
	      temphgr->next=temphg->referrences;
	      temphg->referrences=temphgr;
	      temphgr->ip=ip2+tempi->ic_cnt-8;
	    }
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_REG+IT_I64,CREG_RAX,0,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_ADDR_IMPORT:
	    IcU8(tempi,0xB8);
	    IcU32(tempi,0);
	    if (buf && !(lx->flags&LF_NO_ABSS)) {
	      tempg=tempi->ic_data;
	      tempe=CAlloc(sizeof(ExeImportExportStruct));
	      tempe->type=EIE_LIT_U32;
	      tempe->ip=ip2+tempi->ic_cnt-4;
	      tempe->next=tempg->list;
	      tempg->list=tempe;
	    }
	    IcU24(tempi,0xC06348);
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_REG+IT_I64,CREG_RAX,0,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_STR_CONST:
	  case EC_GET_LABEL:
	    lb=tempi->ic_data;
	    if (lx->flags&LF_STATIC_COMPILE)
	      i=lb->addr+a->ip;
	    else
	      i=lb->addr+buf;
	    IcLea(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_RIP_DISP32+IT_PTR,0,i,lx,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_ASM:
	    tempex=tempi->ic_data;
	    tempi->ic_cnt+=tempex->code_U8s;
	    if (buf) {
	      MemCpy(buf+ip,tempex->code,tempex->code_U8s);
	      Free(tempex->code);
	      tempex->code=buf+ip;
	      if (lx->flags&LF_STATIC_COMPILE)
		CmpFixUpStaticAsm(lx,tempex,ip2);
	      else
		CmpFixUpJITAsm(lx,tempex,ip2);
	      cnt=tempi->ic_cnt;
	      goto skip_copy;
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_FS:
	    IcZero(tempi,CREG_RAX);
	    IcU32(tempi,0x8B4864);
	    break;
	  case EC_GS:
	    IcZero(tempi,CREG_RAX);
	    IcU32(tempi,0x8B4865);
	    break;
	  case EC_MOV_FS:
	    IcZero(tempi,CREG_RAX);
	    IcU8(tempi,0x64);
	    //It's ugly to use ic_class here
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_DISP+CmpType(tempi->ic_class),CREG_RAX,tempi->ic_data,ip2);
	    break;
	  case EC_MOV_GS:
	    IcZero(tempi,CREG_RAX);
	    IcU8(tempi,0x65);
	    //It's ugly to use ic_class here
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_DISP+CmpType(tempi->ic_class),CREG_RAX,tempi->ic_data,ip2);
	    break;
	  case EC_RIP:
	    IcU16(tempi,0xB848);
	    IcU64(tempi,ip2+tempi->ic_cnt-2);
	    if (lx->flags&LF_STATIC_COMPILE && buf && !(lx->flags&LF_NO_ABSS)) {
	      tempa=CAlloc(sizeof(ExeAbsAddrStruct));
	      tempa->next=a->abss;
	      tempa->type=EAA_ADD_U64;
	      a->abss=tempa;
	      tempa->ip=ip2+tempi->ic_cnt-8;
	    }
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_REG+IT_I64,CREG_RAX,0,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_LT_TYPECAST:
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_COM:
	    IcUnaries(tempi,SLASH_OP_NOT,ip2);
	    break;
	  case EC_NOT:
	    IcNot(tempi,ip2);
	    break;
	  case EC_UNARY_MINUS:
	    if (tempi->r.type&IT_MASK==IT_F64)
	      IcFUnaryMinus(lx,tempi,buf,ip2);
	    else
	      IcUnaries(tempi,SLASH_OP_NEG,ip2);
	    break;
	  case EC_ADDR:
	  case EC_MOV:
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_DEREF:
	    IcDeref(tempi,ip2);
	    break;
	  case EC_DEREF_PP:
	    IcDerefPostIncDec(tempi,SLASH_OP_INC,ip2);
	    break;
	  case EC_DEREF_MM:
	    IcDerefPostIncDec(tempi,SLASH_OP_DEC,ip2);
	    break;
	  case EC__PP:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcPostIncDec(tempi,SLASH_OP_INC,ip2);
	    else
	      IcFPostIncDec(lx,tempi,CMP_TEMPLATE_INC,ip2);
	    break;
	  case EC__MM:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcPostIncDec(tempi,SLASH_OP_DEC,ip2);
	    else
	      IcFPostIncDec(lx,tempi,CMP_TEMPLATE_DEC,ip2);
	    break;
	  case EC_PP_:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcPreIncDec(tempi,SLASH_OP_INC,ip2);
	    else
	      IcFPreIncDec(lx,tempi,CMP_TEMPLATE_INC,ip2);
	    break;
	  case EC_MM_:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcPreIncDec(tempi,SLASH_OP_DEC,ip2);
	    else
	      IcFPreIncDec(lx,tempi,CMP_TEMPLATE_DEC,ip2);
	    break;
	  case EC_LEA:
	    IcLea(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,lx,buf,ip2);
	    break;
	  case EC_POWER:
	    IcFPow(lx,tempi,buf,ip2);
	    break;
	  case EC_SHL:
	    IcShift(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0xE0D1E0D3E0C1,0xE0D1E0D3E0C1,ip2);
	    break;
	  case EC_SHR:
	    IcShift(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0xE8D1E8D3E8C1,0xF8D1F8D3F8C1,ip2);
	    break;
	  case EC_MUL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcMul(tempi,ip2);
	    else
	      IcFMul(lx,tempi,buf,ip2);
	    break;
	  case EC_DIV:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcDiv(tempi,ip2);
	    else
	      IcFDiv(lx,tempi,buf,ip2);
	    break;
	  case EC_MOD:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcMod(tempi,ip2);
	    else
	      IcFMod(lx,tempi,ip2);
	    break;
	  case EC_AND:
	    IcAddEct(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x23,ip2);
	    break;
	  case EC_OR:
	    IcAddEct(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x0B,ip2);
	    break;
	  case EC_XOR:
	    IcAddEct(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x33,ip2);
	    break;
	  case EC_ADD:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcAddEct(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		  tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x03,ip2);
	    else
	      IcFAdd(lx,tempi,buf,ip2);
	    break;
	  case EC_SUB:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcSub(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		  tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
	    else
	      IcFSub(lx,tempi,buf,ip2);
	    break;
	  case EC_EQUAL_EQUAL:
	    IcCmp(tempi,0x75,0x75,ip2);
	    break;
	  case EC_NOT_EQUAL:
	    IcCmp(tempi,0x74,0x74,ip2);
	    break;
	  case EC_LESS:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcCmp(tempi,0x73,0x7D,ip2);
	    else
	      IcFCmp(lx,tempi,CMP_TEMPLATE_LESS,ip2);
	    break;
	  case EC_GREATER:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcCmp(tempi,0x76,0x7E,ip2);
	    else
	      IcFCmp(lx,tempi,CMP_TEMPLATE_GREATER,ip2);
	    break;
	  case EC_LESS_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcCmp(tempi,0x77,0x7F,ip2);
	    else
	      IcFCmp(lx,tempi,CMP_TEMPLATE_LESS_EQUAL,ip2);
	    break;
	  case EC_GREATER_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcCmp(tempi,0x72,0x7C,ip2);
	    else
	      IcFCmp(lx,tempi,CMP_TEMPLATE_GREATER_EQUAL,ip2);
	    break;
	  case EC_AND_AND:
	    IcAndAnd(tempi,ip2);
	    break;
	  case EC_OR_OR:
	    IcOrOr(tempi,ip2);
	    break;
	  case EC_XOR_XOR:
	    IcXorXor(tempi,ip2);
	    break;
	  case EC_COLON:
	    IcColon(tempi,ip2);
	    break;
	  case EC_ASSIGN:
	    IcAssign(tempi,ip2);
	    break;
	  case EC_ASSIGN_PP:
	    IcAssignPostIncDec(tempi,SLASH_OP_INC,ip2);
	    break;
	  case EC_ASSIGN_MM:
	    IcAssignPostIncDec(tempi,SLASH_OP_DEC,ip2);
	    break;
	  case EC_SHL_EQUAL:
	    IcShiftEqual(tempi,tempi->p1.ptype,
		tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0xE0D1E0D3E0C1,0xE0D1E0D3E0C1,ip2);
	    break;
	  case EC_SHR_EQUAL:
	    IcShiftEqual(tempi,tempi->p1.ptype,
		tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0xE8D1E8D3E8C1,0xF8D1F8D3F8C1,ip2);
	    break;
	  case EC_MUL_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcMulEqual(tempi,ip2);
	    else
	      IcFOpEqual(lx,tempi,SLASH_OP_FMUL,buf,ip2);
	    break;
	  case EC_DIV_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcDivEqual(tempi,FALSE,ip2);
	    else
	      IcFOpEqual(lx,tempi,SLASH_OP_FDIV,buf,ip2);
	    break;
	  case EC_MOD_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcDivEqual(tempi,TRUE,ip2);
	    else
	      IcFModEqual(lx,tempi,ip2);
	    break;
	  case EC_AND_EQUAL:
	    IcAndEqual(tempi,ip2);
	    break;
	  case EC_OR_EQUAL:
	    IcOrEqual(tempi,ip2);
	    break;
	  case EC_XOR_EQUAL:
	    IcXorEqual(tempi,ip2);
	    break;
	  case EC_ADD_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcAddSubEctEqual(tempi,tempi->p1.ptype,
		  tempi->r.type,tempi->r.reg,tempi->r.disp,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		  tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x010000000003,ip2);
	    else
	      IcFOpEqual(lx,tempi,SLASH_OP_FADD,buf,ip2);
	    break;
	  case EC_SUB_EQUAL:
	    if (tempi->ic_flags&ICF_USE_INT)
	      IcAddSubEctEqual(tempi,tempi->p1.ptype,
		  tempi->r.type,tempi->r.reg,tempi->r.disp,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		  tempi->p2.type,tempi->p2.reg,tempi->p2.disp,0x29000000052B,ip2);
	    else
	      IcFOpEqual(lx,tempi,SLASH_OP_FSUB,buf,ip2);
	    break;
	  case EC_SHL_CONST:
	    IcShift(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		TY_IMM+IT_I64,0,tempi->ic_data,0xE0D1E0D3E0C1,0xE0D1E0D3E0C1,ip2);
	    break;
	  case EC_SHR_CONST:
	    IcShift(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
	    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,
		TY_IMM+IT_I64,0,tempi->ic_data,0xE8D1E8D3E8C1,0xF8D1F8D3F8C1,ip2);
	    break;
	  case EC_ADD_CONST:
	    IcAddConst(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,tempi->ic_data,0x0003,ip2);
	    break;
	  case EC_SUB_CONST:
	    IcAddConst(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,tempi->ic_data,0x052B,ip2);
	    break;
	  case EC_ENTER:
	    if (tempi->ic_data<=MAX_I16)
	      IcU32(tempi,tempi->ic_data<<8+0xC8);
	    else {
	      IcPush(tempi,TY_REG+IT_I64,CREG_RBP,0,ip2);
	      IcMov(tempi,TY_REG+IT_I64,CREG_RBP,0,
		  TY_REG+IT_I64,CREG_RSP,0,ip2);
	      IcAddRSP(tempi,-tempi->ic_data);
	    }
	    if (fun_class) {
	      if (Bt(fun_class->flags,Ff_INTERRUPT))
		IcPushRegs(tempi,CALWAYS_CLOBBERED_MASK&~(1<<CREG_RBP|1<<CREG_RSP)|fun_class->used_reg_mask&(CREG_VARS_MASK|CREG_NON_PTR_MASK|CSTK_TEMPS_MASK));
	      else {
		if (sys_var_init_flag&1 && i)
		  IcLocalVarInit(tempi);
		IcPushRegs(tempi,fun_class->used_reg_mask&(CREG_VARS_MASK|CREG_NON_PTR_MASK));
	      }
	      for (i=0;i<NUM_REGS;i++)
		if (register_offsets[i]>0 && register_offsets[i].offset!=MAX_I64) {
		  tempc=register_offsets[i].m->member_class;
		  while (tempc->fwd_class)
		    tempc=tempc->fwd_class;
		  IcMov(tempi,TY_REG+IT_I64,i,0,
		      TY_DISP+tempc->sub_type,CREG_RBP,register_offsets[i].offset,ip2);
		}
	    }
	    break;
	  case EC_ADD_RSP:
	    IcAddRSP(tempi,tempi->ic_data);
	  case EC_ADD_RSP1:
	    break;
	  case EC_CALL:
	    i=tempi->ic_data-(ip2+5);
	    if (!(MIN_I32<=i<=MAX_I32) && !(lx->flags&Lf_STATIC_COMPILE)) {
	      IcU16(tempi,0xBB48);
	      IcU64(tempi,tempi->ic_data);
	      IcU16(tempi,0xD3FF);
	    } else {
	      IcU8(tempi,0xE8);
	      IcU32(tempi,i);
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_CALL_EXTERN: //Only for static modules
	    IcU8(tempi,0xE8);
	    IcU32(tempi,0);
	    if (buf) {
	      tempf=tempi->ic_data;
	      tempeu=CAlloc(sizeof(ExternUsage));
	      tempeu->next=tempf->ext_list;
	      tempf->ext_list=tempeu;
	      tempeu->ip=ip2+1;
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_CALL_INDIRECT:
	    if (MIN_I8<=tempi->ic_data<=MAX_I8) {
	      IcU24(tempi,0x2454FF);  //CALL disp[RSP]
	      IcU8(tempi,tempi->ic_data);
	    } else {
	      IcU24(tempi,0x2494FF);  //CALL disp[RSP]
	      IcU32(tempi,tempi->ic_data);
	    }
	    break;
	  case EC_CALL_INDIRECT2:
	    IcU16(tempi,0xBB48);
	    if (lx->flags&LF_STATIC_COMPILE) i=ip2+tempi->ic_cnt;
	    IcU64(tempi,tempi->ic_data);
	    IcU16(tempi,0x13FF);
	    if (buf && lx->flags&LF_STATIC_COMPILE&& !(lx->flags&LF_NO_ABSS)) {
	      tempa=CAlloc(sizeof(ExeAbsAddrStruct));
	      tempa->next=a->abss;
	      tempa->type=EAA_ADD_U64;
	      a->abss=tempa;
	      tempa->ip=i;
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_CALL_IMPORT:
	    if (GetOpt(OPTf_USE_LIT64)) {
	      IcU16(tempi,0xBB48);
	      IcU64(tempi,0);
	      if (buf) {
		tempf=tempi->ic_data;
		tempe=CAlloc(sizeof(ExeImportExportStruct));
		tempe->type=EIE_LIT_U64;
		tempe->ip=ip2+tempi->ic_cnt-8;
		tempe->next=tempf->list;
		tempf->list=tempe;
	      }
	      IcU16(tempi,0xD3FF);
	    } else {
	      IcU8(tempi,0xE8);
	      IcU32(tempi,-(ip2+5));
	      if (buf) {
		tempf=tempi->ic_data;
		tempe=CAlloc(sizeof(ExeImportExportStruct));
		tempe->type=EIE_REL_U32;
		tempe->ip=ip2+tempi->ic_cnt-4;
		tempe->next=tempf->list;
		tempf->list=tempe;
	      }
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_CLI:
	    IcU8(tempi,0xFA);
	    break;
	  case EC_STI:
	    IcU8(tempi,0xFB);
	    break;
	  case EC_PUSHFD:
	    IcU8(tempi,0x9C);
	    break;
	  case EC_POPFD:
	    IcU8(tempi,0x9D);
	    break;
	  case EC_PUSH:
	    IcPush(tempi,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_POP:
	    IcU8(tempi,0x58);
	    break;
	  case EC_CLD:
	    IcU8(tempi,0xFC);
	    break;
	  case EC_STD:
	    IcU8(tempi,0xFD);
	    break;
	  case EC_HLT:
	    IcU8(tempi,0xF4);
	    break;
	  case EC_WBINVD:
	    IcU16(tempi,0x090F);
	    break;
	  case EC_INVD:
	    IcU16(tempi,0x080F);
	    break;
	  case EC_INVLPG:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU24(tempi,0x38010F);
	    break;
	  case EC_CLFLUSH:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU24(tempi,0x38AE0F);
	    break;
	  case EC_PAUSE:
	    IcU16(tempi,0x90F3);
	    break;
	  case EC_GET_FLAGS:
	    IcU8(tempi,0x9C);
	    IcPop(tempi,TY_REG+IT_I64,CREG_RAX,0,ip2);
	    break;
	  case EC_RDTSC:
	    IcCopyTemplate(lx,tempi,CMP_TEMPLATE_RDTSC,TRUE,FALSE,FALSE,CN_MAIN);
	    break;
	  case EC_HPET:
	    IcCopyTemplate(lx,tempi,CMP_TEMPLATE_HPET,TRUE,FALSE,FALSE,CN_MAIN);
	    break;
	  case EC_SET_FLAGS:
	    IcPush(tempi,tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU8(tempi,0x9D);
	    break;
	  case EC_GET_BASE_PTR:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		TY_REG+IT_I64,CREG_RBP,0,ip2);
	    break;
	  case EC_SET_BASE_PTR:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RBP,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_GET_STK_PTR:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		TY_REG+IT_I64,CREG_RSP,0,ip2);
	    break;
	  case EC_SET_STK_PTR:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RSP,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_RETURN_VAL2:
	    break;
	  case EC_RETURN_VAL:
	  case EC_SET_RAX:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    break;
	  case EC_GET_RAX:
	    break;
	  case EC_BT:
	    IcBitOps(tempi,&tempi->p1,&tempi->p2,tempi+1,
		0xA30F,0x20BA0F,ip2);
	    break;
	  case EC_BTS:
	  case EC_LBTS:
	    IcBitOps(tempi,&tempi->p1,&tempi->p2,tempi+1,
		0xAB0F,0x28BA0F,ip2);
	    break;
	  case EC_BTR:
	  case EC_LBTR:
	    IcBitOps(tempi,&tempi->p1,&tempi->p2,tempi+1,
		0xB30F,0x30BA0F,ip2);
	    break;
	  case EC_BTC:
	  case EC_LBTC:
	    IcBitOps(tempi,&tempi->p1,&tempi->p2,tempi+1,
		0xBB0F,0x38BA0F,ip2);
	    break;
	  case EC_BEQU:
	  case EC_LBEQU:
	    IcBEqu(tempi,tempi+1,ip2);
	    break;
	  case EC_BSF:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU32(tempi,0xC0BC0F48);
	    IcU16(tempi,0x0375);
	    IcU24(tempi,0xD0F748);
	    break;
	  case EC_BSR:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU32(tempi,0xC0BD0F48);
	    IcU16(tempi,0x0375);
	    IcU24(tempi,0xD0F748);
	    break;
	  case EC_SIGN_I64:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcCopyTemplate(lx,tempi,CMP_TEMPLATE_SIGN_I64,TRUE,FALSE,FALSE,CN_MAIN);
	    break;
	  case EC_PREEMPT:
	    IcPreempt(tempi,ip2);
	    break;
	  case EC_TOUPPER:
	    IcToUpper(tempi,ip2);
	    break;
	  case EC_TO_I64:
	    IcToI64(lx,tempi,ip2);
	    break;
	  case EC_TO_F64:
	    IcToF64(lx,tempi,ip2);
	    break;
	  case EC_TO_BOOL:
	    IcToBool(lx,tempi,ip2);
	    break;
	  case EC_ABS_I64:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcU24(tempi,0xC00B48);
	    IcU16(tempi,0x0379);
	    IcU24(tempi,0xD8F748);
	    break;
	  case EC_LIMIT_I64:
	    IcLimit(tempi,0x4F4C,ip2);
	    break;
	  case EC_LIMIT_U64:
	    IcLimit(tempi,0x4742,ip2);
	    break;
	  case EC_MIN_I64:
	    IcMinMax(tempi,0x4F,ip2);
	    break;
	  case EC_MAX_I64:
	    IcMinMax(tempi,0x4C,ip2);
	    break;
	  case EC_MIN_U64:
	    IcMinMax(tempi,0x47,ip2);
	    break;
	  case EC_MAX_U64:
	    IcMinMax(tempi,0x42,ip2);
	    break;
	  case EC_MOD_U64:
	    IcModU64(tempi,ip2);
	    break;
	  case EC_SQR_I64:
	    IcSqr(tempi,SLASH_OP_IMUL,ip2);
	    break;
	  case EC_SQR_U64:
	    IcSqr(tempi,SLASH_OP_MUL,ip2);
	    break;
	  case EC_SWAP_U8:
	  case EC_SWAP_U16:
	  case EC_SWAP_U32:
	  case EC_SWAP_U64:
	    IcSwap(tempi,ip2);
	    break;
	  case EC_INSQUE:
	    IcInsQue(tempi,ip2);
	    break;
	  case EC_INSQUE_REV:
	    IcInsQueRev(tempi,ip2);
	    break;
	  case EC_REMQUE:
	    IcRemQue(tempi,ip2);
	    break;
	  case EC_MEMCPY:
	    IcMemCpy(tempi,ip2);
	    break;
	  case EC_MEMSET:
	  case EC_MEMSET_U16:
	  case EC_MEMSET_U32:
	  case EC_MEMSET_U64:
	    IcMemSet(tempi,code-EC_MEMSET,ip2);
	    break;
	  case EC_STRLEN:
	    IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	    IcCopyTemplate(lx,tempi,CMP_TEMPLATE_STRLEN,TRUE,FALSE,FALSE,CN_MAIN);
	    break;
	  case EC_INU32:
	    if (tempi->p1.type&TY_MASK==TY_IMM) {
	      IcU16(tempi,0xC033);
	      if (tempi->p1.disp<=MAX_U8)
		IcU16(tempi,0xE5+tempi->p1.disp<<8);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p1.disp<<16);
		IcU8(tempi,0xED);
	      }
	    } else {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      IcU16(tempi,0xC033);
	      IcU8(tempi,0xED);
	    }
	    break;
	  case EC_INU16:
	    if (tempi->p1.type&TY_MASK==TY_IMM) {
	      IcU16(tempi,0xC033);
	      if (tempi->p1.disp<=MAX_U8)
		IcU24(tempi,0xE500+OC_OP_SIZE_PREFIX+tempi->p1.disp<<16);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p1.disp<<16);
		IcU16(tempi,0xED00+OC_OP_SIZE_PREFIX);
	      }
	    } else {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      IcU16(tempi,0xC033);
	      IcU16(tempi,0xED00+OC_OP_SIZE_PREFIX);
	    }
	    break;
	  case EC_INU8:
	    if (tempi->p1.type&TY_MASK==TY_IMM) {
	      IcU16(tempi,0xC033);
	      if (tempi->p1.disp<=MAX_U8)
		IcU16(tempi,0xE4+tempi->p1.disp<<8);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p1.disp<<16);
		IcU8(tempi,0xEC);
	      }
	    } else {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      IcU16(tempi,0xC033);
	      IcU8(tempi,0xEC);
	    }
	    break;
	  case EC_OUTU32:
	    if (tempi->p2.type&TY_MASK==TY_IMM) {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      if (tempi->p2.disp<=MAX_U8)
		IcU16(tempi,0xE7+tempi->p2.disp<<8);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p2.disp<<16);
		IcU8(tempi,0xEF);
	      }
	    } else {
	      if (tempi->ic_flags & ICF_P1_FIRST) {
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
	      } else {
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      }
	      IcU8(tempi,0xEF);
	    }
	    break;
	  case EC_OUTU16:
	    if (tempi->p2.type&TY_MASK==TY_IMM) {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      if (tempi->p2.disp<=MAX_U8)
		IcU24(tempi,0xE700+OC_OP_SIZE_PREFIX+tempi->p2.disp<<16);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p2.disp<<16);
		IcU16(tempi,0xEF00+OC_OP_SIZE_PREFIX);
	      }
	    } else {
	      if (tempi->ic_flags & ICF_P1_FIRST) {
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
	      } else {
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      }
	      IcU16(tempi,0xEF00+OC_OP_SIZE_PREFIX);
	    }
	    break;
	  case EC_OUTU8:
	    if (tempi->p2.type&TY_MASK==TY_IMM) {
	      IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		  tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      if (tempi->p2.disp<=MAX_U8)
		IcU16(tempi,0xE6+tempi->p2.disp<<8);
	      else {
		IcU32(tempi,0xBA00+OC_OP_SIZE_PREFIX+tempi->p2.disp<<16);
		IcU8(tempi,0xEE);
	      }
	    } else {
	      if (tempi->ic_flags & ICF_P1_FIRST) {
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
	      } else {
		IcMov(tempi,TY_REG+IT_I64,CREG_RDX,0,
		    tempi->p2.type,tempi->p2.reg,tempi->p2.disp,ip2);
		IcMov(tempi,TY_REG+IT_I64,CREG_RAX,0,
		    tempi->p1.type,tempi->p1.reg,tempi->p1.disp,ip2);
	      }
	      IcU8(tempi,0xEE);
	    }
	    break;
	  case EC_NOBOUND_SWITCH:
	    IcSwitch(tempi,ip,TRUE,lx,buf,ip2);
	    break;
	  case EC_SWITCH:
	    IcSwitch(tempi,ip,FALSE,lx,buf,ip2);
	    break;
	  case EC_BR_ZERO:
	    IcTestAndBranch(tempi,ip,0x74840F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_ZERO:
	    IcTestAndBranch(tempi,ip,0x75850F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_MM_ZERO:
	    IcPreIncDec(tempi,SLASH_OP_DEC,ip2);
	    IcZFlagBranch(tempi,ip,0x74840F,buf);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_MM_NOT_ZERO:
	    IcPreIncDec(tempi,SLASH_OP_DEC,ip2);
	    IcZFlagBranch(tempi,ip,0x75850F,buf);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_EQUAL_EQUAL:
	    IcCmpAndBranch(tempi,FALSE,ip,0x74840F,0x74840F,0x74840F,0x74840F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_EQUAL_EQUAL2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x74840F,0x74840F,0x74840F,0x74840F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_EQUAL:
	    IcCmpAndBranch(tempi,FALSE,ip,0x75850F,0x75850F,0x75850F,0x75850F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_EQUAL2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x75850F,0x75850F,0x75850F,0x75850F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_GREATER_EQUAL:
	    if (tempi->ic_flags&ICF_USE_F64)
	      IcFCmpAndBranch(lx,tempi,  ip,0x73830F,0x76860F,buf,ip2);
	    else
	      IcCmpAndBranch(tempi,FALSE,ip,0x73830F,0x7D8D0F,0x76860F,0x7E8E0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_GREATER_EQUAL2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x73830F,0x7D8D0F,0x76860F,0x7E8E0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_LESS_EQUAL:
	    if (tempi->ic_flags&ICF_USE_F64)
	      IcFCmpAndBranch(lx,tempi,  ip,0x76860F,0x73830F,buf,ip2);
	    else
	      IcCmpAndBranch(tempi,FALSE,ip,0x76860F,0x7E8E0F,0x73830F,0x7D8D0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_LESS_EQUAL2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x76860F,0x7E8E0F,0x73830F,0x7D8D0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_GREATER:
	    if (tempi->ic_flags&ICF_USE_F64)
	      IcFCmpAndBranch(lx,tempi,  ip,0x77870F,0x72820F,buf,ip2);
	    else
	      IcCmpAndBranch(tempi,FALSE,ip,0x77870F,0x7F8F0F,0x72820F,0x7C8C0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_GREATER2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x77870F,0x7F8F0F,0x72820F,0x7C8C0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_LESS:
	    if (tempi->ic_flags&ICF_USE_F64)
	      IcFCmpAndBranch(lx,tempi,  ip,0x72820F,0x77870F,buf,ip2);
	    else
	      IcCmpAndBranch(tempi,FALSE,ip,0x72820F,0x7C8C0F,0x77870F,0x7F8F0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_LESS2:
	    IcCmpAndBranch(tempi,TRUE,ip,0x72820F,0x7C8C0F,0x77870F,0x7F8F0F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_BT:
	    IcBrBitOps(tempi,ip,0xA30F,0x20BA0F,0x72820F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_BTS:
	  case EC_BR_LBTS:
	    IcBrBitOps(tempi,ip,0xAB0F,0x28BA0F,0x72820F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_BTR:
	  case EC_BR_LBTR:
	    IcBrBitOps(tempi,ip,0xB30F,0x30BA0F,0x72820F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_BTC:
	  case EC_BR_LBTC:
	    IcBrBitOps(tempi,ip,0xBB0F,0x38BA0F,0x72820F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_BT:
	    IcBrBitOps(tempi,ip,0xA30F,0x20BA0F,0x73830F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_BTS:
	  case EC_BR_NOT_LBTS:
	    IcBrBitOps(tempi,ip,0xAB0F,0x28BA0F,0x73830F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_BTR:
	  case EC_BR_NOT_LBTR:
	    IcBrBitOps(tempi,ip,0xB30F,0x30BA0F,0x73830F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_NOT_BTC:
	  case EC_BR_NOT_LBTC:
	    IcBrBitOps(tempi,ip,0xBB0F,0x38BA0F,0x73830F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_AND_ZERO:
	    IcAndBranch(tempi,ip,0x74840F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_BR_AND_NOT_ZERO:
	    IcAndBranch(tempi,ip,0x75850F,buf,ip2);
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_LEAVE:
	    if (fun_class) {
	      if (Bt(fun_class->flags,Ff_INTERRUPT))
		IcPopRegs(tempi,CALWAYS_CLOBBERED_MASK&~(1<<CREG_RBP|1<<CREG_RSP)|fun_class->used_reg_mask&(CREG_VARS_MASK|CREG_NON_PTR_MASK|CSTK_TEMPS_MASK));
	      else
		IcPopRegs(tempi,fun_class->used_reg_mask&(CREG_VARS_MASK|CREG_NON_PTR_MASK));
	    }
	    if (tempi->ic_data<=MAX_I16)
	      IcU8(tempi,0xC9);
	    else {
	      IcAddRSP(tempi,tempi->ic_data);
	      IcPop(tempi,TY_REG+IT_I64,CREG_RBP,0,ip2);
	    }
	    if (fun_class && Bt(fun_class->flags,Ff_INTERRUPT))
	      IcU16(tempi,0xCF48);
	    else if (fun_class && fun_class->arg_cnt &&
		  (Bt(fun_class->flags,Ff_RET1) || Bt(fun_class->flags,Ff_ARGPOP)) &&
		  !Bt(fun_class->flags,Ff_NOARGPOP)) {
	      IcU8(tempi,0xC2);
	      IcU16(tempi,fun_class->arg_cnt<<3);
	    } else
	      IcU8(tempi,0xC3);
	    break;
	  case EC_RET:
	    IcU8(tempi,0xC3);
	    break;
	  case EC_JMP:
	    short_jmp=ToBool(tempi->ic_flags&ICF_SHORT_JMP);
	    lb=tempi->ic_data;
	    while (lb1=lb->fwd)
	      lb=lb1;
	    if (!buf) {
	      if (lb->addr!=INVALID_PTR) {
		i=lb->addr-ip;
		    if (MIN_I8+5<i<MAX_I8-5)
		  short_jmp=TRUE;
	      }
	    }
	    if (short_jmp) {
	      tempi->ic_flags|=ICF_SHORT_JMP;
	      i=lb->addr-(ip+2);
	      IcU16(tempi,i<<8+0xEB);
	    } else {
	      i=lb->addr-(ip+5);
	      IcU8(tempi,0xE9);
	      IcU32(tempi,i);
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_END:
	    break;
	  case EC_LABEL:
	    lb=tempi->ic_data;
	    lb->addr=ip;
	    if (lb->flags&CBMF_POP_CMP) {
	      IcAddRSP(tempi,-8,FALSE);
	      IcAddRSP(tempi,8,FALSE);
	    }
	    tempi->ic_flags&=~ICF_CODE_FINAL;
	    break;
	  case EC_QUESTION:
	    break;
	  case EC_IRQ:
	    IcU8(tempi,0xCD);
	    IcU8(tempi,tempi->ic_data);
	    break;
	  case EC_NOP:
	  case EC_NOP1:
	  case EC_PUSH_CMP:
	    break;
	  case EC_CALL_START:
	  case EC_PUSH_REGS:
	    IcPushRegs(tempi,tempi->ic_data);
	    break;
	  case EC_CALL_END2:
	    break;
	  case EC_CALL_END:
	    IcPopRegs(tempi,tempi->ic_data);
	    if (tempi->r.type&TY_MASK)
	      IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		  TY_REG+IT_I64,CREG_RAX,0,ip2);
	    break;
	  case EC_POP_REGS:
	    IcPopRegs(tempi,tempi->ic_data);
	    break;
	  case EC_END_EXP:
	    break;

	  default:
	    OptMissing(lx,tempi);
	}
	if (tempi->r.type&TY_MASK) {
	  if (tempi->ic_flags & ICF_R_TO_F64) {
	    if (tempi->ic_opcode==EC_PUSH_CMP) {
	      IcU24(tempi,0xDC8B48);
	      IcU24(tempi,0x2BDF48);
	      IcU24(tempi,0x1BDD48);
	      IcU8(tempi,0x9B);
	    } else {
	      IcFCvt(lx,tempi,CREG_RAX,tempi->r.type,tempi->r.reg,tempi->r.disp,FALSE,CN_R,ip2);
	      IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		  TY_REG+IT_I64,CREG_RAX,0,ip2);
	    }
	  } else if (tempi->ic_flags & ICF_R_TO_INT) {
	    IcFCvt(lx,tempi,CREG_RAX,tempi->r.type,tempi->r.reg,tempi->r.disp,TRUE,CN_R,ip2);
	    IcMov(tempi,tempi->r.type,tempi->r.reg,tempi->r.disp,
		TY_REG+IT_I64,CREG_RAX,0,ip2);
	  }
	}
      }
      if (tempi->ic_flags&ICF_DEL_PREVIOUS_INS) {
	cnt=tempi->ic_last_cnt;
	tempi->ic_flags&=~ICF_DEL_PREVIOUS_INS;
      } else
	cnt=tempi->ic_cnt;

      if (cnt && buf) {
	if (tempi->ic_ext_body) {
	  MemCpy(buf+ip,tempi->ic_ext_body,cnt);
	  Free(tempi->ic_ext_body);
	  tempi->ic_ext_body=NULL;
	} else
	  MemCpy(buf+ip,tempi->ic_body,cnt);
      }

skip_copy:
      if (cnt) {
	if (lx->pass==9) {
	  i=tempi->ic_line;
	  if (i>=last_line) {
	    last_line=i;
	    if (i<*min_line)
	      *min_line=i;
	    if (*min_line<i<min_line2)
	      min_line2=i;
	    if (i>*max_line)
	      *max_line=i;
	  }
	}
	if (buf && dbg) {
	  if (*min_line<=tempi->ic_line<=*max_line) {
	    i=tempi->ic_line-*min_line;
	    if (!dbg->body[i])
	    dbg->body[i]=ip2;
	  }
	}
      }

      if (tempi->ic_flags&ICF_TRACE) {
	if (Bt(&lx->pass_trace_mask,lx->pass)) {
	  if (cnt) {
	    "$FG,RED$";
	    if (buf)
	      Un(buf+ip,cnt,64);
	    else if (tempi->ic_ext_body)
	      Un(tempi->ic_ext_body,cnt,64);
	    else
	      Un(tempi->ic_body,cnt,64);
	    "$FG$";
	  }
	}
      }
      if (!(tempi->ic_flags&(ICF_CODE_FINAL|ICF_DONT_RESTORE)))
	MemCpy(&tempi->p1,saved_p123,3*sizeof(IcArg));
      ip+=cnt;
    } else {
      tempi->ic_cnt=0;
      tempi->ic_last_cnt=-1;
    }
    if (buf && indent) {
      i=tempi->ic_line+tempi->ic_start_line_offset;
      if (*min_line<=i<=*max_line) {
	i-=*min_line;
	if (indent->body[i]==-1)
	  indent->body[i]=tempi->ic_indent_level;
      }
    }
    tempi++;
  }

  lb=lx->cbh.cb_misc_list;
  while (lb) {
    switch (lb->type) {
      case CBMT_STR_CONST:
	lb->addr=ip;
	if (buf)
	  MemCpy(buf+ip,lb->str,lb->st_len);
	ip+=lb->st_len;
	break;
      case CBMT_JMP_TABLE:
	lb->addr=ip;
	if (buf) {
	  for (i=0;i<lb->range;i++) {
	    lb1=lb->jmp_table[i];
	    qq(U8 *)=buf+lb->addr+i<<3;
	    if (lx->flags&LF_STATIC_COMPILE && !(lx->flags&LF_NO_ABSS)) {
	      tempa=CAlloc(sizeof(ExeAbsAddrStruct));
	      tempa->next=a->abss;
	      tempa->type=EAA_ADD_U64;
	      a->abss=tempa;
	      tempa->ip=a->ip+lb->addr+i<<3;
	      *qq=lb1->addr+a->ip;
	    } else
	      *qq=lb1->addr+buf;
	  }
	}
	ip+=lb->range*sizeof(U0 *);
	break;
      case CBMT_FLOAT_CONSTS:
	lb->addr=ip;
	if (buf)
	  MemCpy(buf+lb->addr,lb->float_consts,lb->num_consts*sizeof(F64));
	ip+=lb->num_consts*sizeof(F64);
	break;
    }
    lb=lb->next;
  }
  if (lx->pass==9) {
    if (min_line2!=-1 &&
	min_line2-*min_line>10) //Defines screw things up
      *min_line=min_line2;
  } else if (buf) {
    if (dbg) {
      if (lx->flags&LF_STATIC_COMPILE)
	dbg->body[num_lines]=ip+a->ip;
      else
	dbg->body[num_lines]=ip+buf;
    }
    if (indent) {
      if (indent->body[0]==-1)
	indent->body[0]=0;
      for (i=1;i<=num_lines;i++)
	if (indent->body[i]==-1)
	  indent->body[i]=indent->body[i-1];
      }
  }

  return ip;
}
