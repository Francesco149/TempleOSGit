#define FBO1_NOT_CONST	0
#define FBO1_INT	1
#define FBO1_F64	2


BoolI64 OptFixupBinaryOp1(IntermediateCode *tempi,
     IntermediateCode *tempi1,IntermediateCode *tempi2,
     BoolI8 *is_unsigned)
{
  ClassStruct *tempc=tempi->ic_class,*tempc1,*tempc2;

  if (tempi1->ic_flags&ICF_R_TO_INT)
    tempc1=sys_internal_types[IT_I64];
  else if (tempi1->ic_flags&ICF_R_TO_F64)
    tempc1=sys_internal_types[IT_F64];
  else {
    tempc1=tempi1->ic_class;
    while (tempc1->fwd_class)
      tempc1=tempc1->fwd_class;
  }

  if (tempi2->ic_flags&ICF_R_TO_INT)
    tempc2=sys_internal_types[IT_I64];
  else if (tempi2->ic_flags&ICF_R_TO_F64)
    tempc2=sys_internal_types[IT_F64];
  else {
    tempc2=tempi2->ic_class;
    while (tempc2->fwd_class)
      tempc2=tempc2->fwd_class;
  }

  if (tempc1->sub_type>tempc2->sub_type)
    tempc=tempi->ic_class=tempc1;
  else
    tempc=tempi->ic_class=tempc2;

  if (tempc->sub_type==IT_F64) {
    if (tempi1->ic_opcode==EC_IMM) {
      tempi1->ic_data(F64)=tempi1->ic_data;
      tempi1->ic_class=sys_internal_types[IT_F64];
      tempi1->ic_opcode=EC_F64;
      tempi1->ic_flags&=~ICF_R_TO_F64;
    } else
      if (tempc1->sub_type!=IT_F64)
      tempi1->ic_flags|=ICF_R_TO_F64;
    if (tempi2->ic_opcode==EC_IMM) {
      tempi2->ic_data(F64)=tempi2->ic_data;
      tempi2->ic_class=sys_internal_types[IT_F64];
      tempi2->ic_opcode=EC_F64;
      tempi2->ic_flags&=~ICF_R_TO_F64;
    } else
      if (tempc2->sub_type!=IT_F64)
      tempi2->ic_flags|=ICF_R_TO_F64;
    if (tempi1->ic_opcode==EC_F64 &&
	tempi2->ic_opcode==EC_F64 &&
	!(tempi->ic_flags&(ICF_PUSH_CMP|ICF_POP_CMP))) {
      tempi->ic_flags|=tempi1->ic_flags|tempi2->ic_flags;
      tempi1->ic_opcode=EC_NOP;
      tempi1->ic_flags=0;
      tempi2->ic_opcode=EC_NOP;
      tempi2->ic_flags=0;
      return FBO1_F64;
    } else
      return FBO1_NOT_CONST;
  }
  *is_unsigned=tempc1->sub_type&1 || tempc2->sub_type&1;
  if (tempi1->ic_opcode==EC_IMM &&
      tempi2->ic_opcode==EC_IMM &&
      !(tempi->ic_flags&(ICF_PUSH_CMP|ICF_POP_CMP))) {
    tempi->ic_flags|=tempi1->ic_flags|tempi2->ic_flags;
    tempi1->ic_opcode=EC_NOP;
    tempi1->ic_flags=0;
    tempi2->ic_opcode=EC_NOP;
    tempi2->ic_flags=0;
    return FBO1_INT;
  } else
    return FBO1_NOT_CONST;
}

BoolI64 OptFixupBinaryOp2(IntermediateCode **tempi1,IntermediateCode **tempi2)
{
  IntermediateCode *tempii1=*tempi1,
		   *tempii2=*tempi2;
  if (tempii1->ic_opcode==EC_IMM &&
      !(tempii1->ic_flags & ICF_R_TO_F64))
    return TRUE;
  if (tempii2->ic_opcode==EC_IMM &&
      !(tempii2->ic_flags & ICF_R_TO_F64)) {
    *tempi1=tempii2;
    *tempi2=tempii1;
    return TRUE;
  }
  return FALSE;
}

BoolI64 OptFixupUnaryOp(IntermediateCode *tempi,
     IntermediateCode *tempi1,
     BoolI8 *is_unsigned)
{
  ClassStruct *tempc,*tempc1;
  tempc1=tempi1->ic_class;
  while (tempc1->fwd_class)
    tempc1=tempc1->fwd_class;
  tempi->ic_class=tempc1;
  tempc=tempi->ic_class;
  if (tempc->sub_type==IT_F64) {
    if (tempi1->ic_opcode==EC_IMM) {
      tempi1->ic_data(F64)=tempi1->ic_data;
      tempi1->ic_class=sys_internal_types[IT_F64];
      tempi1->ic_opcode=EC_F64;
      tempi1->ic_flags&=~ICF_R_TO_F64;
    } else
      if (tempc1->sub_type!=IT_F64)
      tempi1->ic_flags|=ICF_R_TO_F64;
    if (tempi1->ic_opcode==EC_F64) {
      tempi->ic_flags|=tempi1->ic_flags;
      tempi1->ic_opcode=EC_NOP;
      tempi1->ic_flags=0;
      return FBO1_F64;
    } else
      return FBO1_NOT_CONST;
  }
  *is_unsigned=tempc1->sub_type&1;
  if (tempi1->ic_opcode==EC_IMM) {
    tempi->ic_flags|=tempi1->ic_flags;
    tempi1->ic_opcode=EC_NOP;
    tempi1->ic_flags=0;
    return FBO1_INT;
  } else
    return FBO1_NOT_CONST;
}

I64 CmpOffset2Reg(I64 offset,OptRegStruct *register_offsets)
{
  I64 i;
  for (i=0;i<NUM_REGS;i++)
    if (offset==register_offsets[i].offset)
      return i;
  return -1;
}

U0 OptMissing(LexStruct *lx,IntermediateCode *tempi)
{
  "Pass:%d Missing IC handler\n",lx->pass;
  CbPutIc(tempi);
  PrsExcept(lx,"ST_OPT_INTERNAL_ERROR");
}

IntermediateCode opt_nop;


PrsStk *OptStart(LexStruct *lx,IntermediateCode **temp_lag=NULL)
{
  I64 i;
  PrsStk *ps=MAlloc(sizeof(PrsStk));
  CbInPtrsInit(&lx->cbh);
  ps->ptr=0;
  ps->ptr2=0;
  if (temp_lag)
    for (i=0;i<4;i++)
      temp_lag[i]=&opt_nop;
  return ps;
}

U0 OptEnd(LexStruct *lx,PrsStk *ps)
{
  if (ps->ptr>2) {
    "Pass:%d Stk:%08X\n",lx->pass,ps->ptr;
    PrsExcept(lx,"ST_OPT_INTERNAL_ERROR");
  }
  Free(ps);
}


IntermediateCode *OptLag(IntermediateCode *tempi)
{
  do {
    if (tempi->ic_opcode==EC_START)
      return NULL;
    else
      tempi--;
  } while (tempi->ic_opcode<=EC_END_EXP);
  return tempi;
}

IntermediateCode *OptLag1(IntermediateCode *tempi)
{
  do {
    if (tempi->ic_opcode==EC_START)
      return NULL;
    else
      tempi--;
  } while (tempi->ic_opcode==EC_NOP||tempi->ic_opcode==EC_NOP1);
  return tempi;
}

IntermediateCode *OptLead1(LexStruct *lx,IntermediateCode *tempi)
{
  nounusedwarn lx;
  do {
    tempi++;
    if (tempi->ic_opcode==EC_END)
      return NULL;
  } while (tempi->ic_opcode==EC_NOP||tempi->ic_opcode==EC_NOP1);
  return tempi;
}

extern U0 OptBrNotZero(LexStruct *lx,IntermediateCode *tempi);

IntermediateCode *OptBrZero(LexStruct *lx,IntermediateCode *tempi)
{
  CbMiscStruct *lb_true,*lb_false;
  IntermediateCode *tempii=OptLag(tempi),*tempii2;
  switch (tempii->ic_opcode) {
    case EC_NOT:
      tempi->ic_opcode=EC_BR_NOT_ZERO;
      tempi->ic_class=tempii->ic_class;
      tempi->ic_flags|=tempii->ic_flags;
      tempi->t.p1c=tempii->t.p1c;
      tempi->t.p1t=tempii->t.p1t;
      tempii->ic_opcode=EC_NOP;
      tempii->ic_flags=0;
      return OptBrNotZero(lx,tempi);
    case EC_EQUAL_EQUAL:
      tempi->ic_opcode=EC_BR_NOT_EQUAL;
      break;
    case EC_NOT_EQUAL:
      tempi->ic_opcode=EC_BR_EQUAL_EQUAL;
      break;
    case EC_LESS:
      tempi->ic_opcode=EC_BR_GREATER_EQUAL;
      break;
    case EC_GREATER:
      tempi->ic_opcode=EC_BR_LESS_EQUAL;
      break;
    case EC_LESS_EQUAL:
      tempi->ic_opcode=EC_BR_GREATER;
      break;
    case EC_GREATER_EQUAL:
      tempi->ic_opcode=EC_BR_LESS;
      break;
    case EC_OR_OR:
      tempi->ic_opcode=EC_BR_OR_OR_ZERO;
      break;
    case EC_AND_AND:
      tempi->ic_opcode=EC_BR_AND_AND_ZERO;
      break;
    case EC_AND:
      tempi->ic_opcode=EC_BR_AND_ZERO;
      break;
    case EC_MM_:
      if (lx->pass==2 && !(tempii->ic_flags&ICF_R_TO_F64) &&
	  tempii->ic_class->sub_type!=IT_F64)
	tempi->ic_opcode=EC_BR_MM_ZERO;
      break;
    case EC_CALL_END:
      tempii2=OptLag(tempii);
      switch (tempii2->ic_opcode) {
	case EC_BT:
	  tempii2->ic_opcode=EC_BR_NOT_BT;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTS:
	  tempii2->ic_opcode=EC_BR_NOT_BTS;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTR:
	  tempii2->ic_opcode=EC_BR_NOT_BTR;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTC:
	  tempii2->ic_opcode=EC_BR_NOT_BTC;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTS:
	  tempii2->ic_opcode=EC_BR_NOT_LBTS;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTR:
	  tempii2->ic_opcode=EC_BR_NOT_LBTR;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTC:
	  tempii2->ic_opcode=EC_BR_NOT_LBTC;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
      }
      break;
  }
  if (tempi->ic_opcode!=EC_BR_ZERO) {
    tempi->ic_class=tempii->ic_class;
    tempi->ic_flags|=tempii->ic_flags;
    tempi->t.p1c=tempii->t.p1c;
    tempi->t.p2c=tempii->t.p2c;
    tempi->t.p1t=tempii->t.p1t;
    tempi->t.p2t=tempii->t.p2t;
    tempii->ic_opcode=EC_NOP;
    tempii->ic_flags=0;

    if (tempi->ic_flags&ICF_PUSH_CMP &&
	EC_BR_NOT_EQUAL<=tempi->ic_opcode<=EC_BR_LESS &&
	!(tempi->ic_flags&ICF_USE_F64)) {
      tempi->ic_opcode+=EC_BR_NOT_EQUAL2-EC_BR_NOT_EQUAL;
      tempi->ic_flags&=~ICF_PUSH_CMP;
      tempii =tempi+1; //EC_PUSH_CMP instruction
      while (tempii->ic_opcode!=EC_PUSH_CMP)
	tempii++;
      tempii->ic_flags=0;
      tempii->ic_opcode=EC_NOP;
      tempii->t.p1t=tempi;
    }

    lb_true=tempi->ic_data;
    if (tempi->ic_opcode==EC_BR_AND_AND_ZERO) {
      tempi->ic_opcode=EC_NOP;
      tempi->ic_flags=0;
      tempii=tempi->t.p1t+1;
      tempii->ic_data=lb_true;
      tempii->t.p1t=tempi->t.p1t;
      tempii->t.p1c=tempi->t.p1c;
      tempii->ic_opcode=EC_BR_ZERO;
      OptBrZero(lx,tempii);
      tempii=tempi->t.p2t+1;
      tempii->t.p1t=tempi->t.p2t;
      tempii->t.p1c=tempi->t.p2c;
      tempii->ic_data=lb_true;
      tempii->ic_opcode=EC_BR_ZERO;
      tempii=OptBrZero(lx,tempii);
    } else if (tempi->ic_opcode==EC_BR_OR_OR_ZERO) {
      lb_false=CbMiscNew(lx,CBMT_LABEL);
      tempi->ic_opcode=EC_LABEL;
      tempi->ic_flags=0;
      tempi->ic_data=lb_false;
      tempii=tempi->t.p1t+1;
      tempii->t.p1t=tempi->t.p1t;
      tempii->t.p1c=tempi->t.p1c;
      tempii->ic_data=lb_false;
      tempii->ic_opcode=EC_BR_NOT_ZERO;
      OptBrNotZero(lx,tempii);
      tempii=tempi->t.p2t+1;
      tempii->t.p1t=tempi->t.p2t;
      tempii->t.p1c=tempi->t.p2c;
      tempii->ic_data=lb_true;
      tempii->ic_opcode=EC_BR_ZERO;
      tempii=OptBrZero(lx,tempii);
    } else
      tempii=tempi;
    if (tempi->ic_flags&ICF_POP_CMP && tempi->t.p1t->ic_opcode==EC_NOP) {
      tempi->t.p1t=tempi->t.p1t->t.p1t;
      tempi->ic_flags&=~ICF_POP_CMP;
    }
    return tempii;
  }
  return tempi;
}


IntermediateCode *OptBrNotZero(LexStruct *lx,IntermediateCode *tempi)
{
  CbMiscStruct *lb_true,*lb_false;
  IntermediateCode *tempii=OptLag(tempi),*tempii2;
  switch (tempii->ic_opcode) {
    case EC_NOT:
      tempi->ic_opcode=EC_BR_ZERO;
      tempi->ic_class=tempii->ic_class;
      tempi->ic_flags|=tempii->ic_flags;
      tempi->t.p1c=tempii->t.p1c;
      tempi->t.p1t=tempii->t.p1t;
      tempii->ic_opcode=EC_NOP;
      tempii->ic_flags=0;
      return OptBrZero(lx,tempi);
    case EC_EQUAL_EQUAL:
      tempi->ic_opcode=EC_BR_EQUAL_EQUAL;
      break;
    case EC_NOT_EQUAL:
      tempi->ic_opcode=EC_BR_NOT_EQUAL;
      break;
    case EC_LESS:
      tempi->ic_opcode=EC_BR_LESS;
      break;
    case EC_GREATER:
      tempi->ic_opcode=EC_BR_GREATER;
      break;
    case EC_LESS_EQUAL:
      tempi->ic_opcode=EC_BR_LESS_EQUAL;
      break;
    case EC_GREATER_EQUAL:
      tempi->ic_opcode=EC_BR_GREATER_EQUAL;
      break;
    case EC_OR_OR:
      tempi->ic_opcode=EC_BR_OR_OR_NOT_ZERO;
      break;
    case EC_AND_AND:
      tempi->ic_opcode=EC_BR_AND_AND_NOT_ZERO;
      break;
    case EC_AND:
      tempi->ic_opcode=EC_BR_AND_NOT_ZERO;
      break;
    case EC_MM_:
      if (lx->pass==2 && !(tempii->ic_flags&ICF_R_TO_F64) &&
	  tempii->ic_class->sub_type!=IT_F64)
	tempi->ic_opcode=EC_BR_MM_NOT_ZERO;
      break;
    case EC_CALL_END:
      tempii2=OptLag(tempii);
      switch (tempii2->ic_opcode) {
	case EC_BT:
	  tempii2->ic_opcode=EC_BR_BT;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTS:
	  tempii2->ic_opcode=EC_BR_BTS;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTR:
	  tempii2->ic_opcode=EC_BR_BTR;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_BTC:
	  tempii2->ic_opcode=EC_BR_BTC;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTS:
	  tempii2->ic_opcode=EC_BR_LBTS;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTR:
	  tempii2->ic_opcode=EC_BR_LBTR;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
	case EC_LBTC:
	  tempii2->ic_opcode=EC_BR_LBTC;
	  tempii2->ic_flags|=ICF_LOCK;
	  tempii2->ic_data=tempi->ic_data;
	  tempii->ic_opcode=EC_CALL_END2;
	  tempi->ic_opcode=EC_NOP;
	  return tempii;
      }
      break;
  }
  if (tempi->ic_opcode!=EC_BR_NOT_ZERO) {
    tempi->ic_class=tempii->ic_class;
    tempi->ic_flags|=tempii->ic_flags;
    tempi->t.p1c=tempii->t.p1c;
    tempi->t.p2c=tempii->t.p2c;
    tempi->t.p1t=tempii->t.p1t;
    tempi->t.p2t=tempii->t.p2t;
    tempii->ic_opcode=EC_NOP;
    tempii->ic_flags=0;

    if (tempi->ic_flags&ICF_PUSH_CMP &&
	EC_BR_NOT_EQUAL<=tempi->ic_opcode<=EC_BR_LESS &&
	!(tempi->ic_flags&ICF_USE_F64)) {
      tempi->ic_opcode+=EC_BR_NOT_EQUAL2-EC_BR_NOT_EQUAL;
      tempi->ic_flags&=~ICF_PUSH_CMP;
      tempii =tempi+1; //EC_PUSH_CMP instruction
      while (tempii->ic_opcode!=EC_PUSH_CMP)
	tempii++;
      tempii->ic_flags=0;
      tempii->ic_opcode=EC_NOP;
      tempii->t.p1t=tempi;
    }

    lb_true=tempi->ic_data;
    if (tempi->ic_opcode==EC_BR_OR_OR_NOT_ZERO) {
      tempi->ic_opcode=EC_NOP;
      tempi->ic_flags=0;
      tempii=tempi->t.p1t+1;
      tempii->t.p1t=tempi->t.p1t;
      tempii->t.p1c=tempi->t.p1c;
      tempii->ic_data=lb_true;
      tempii->ic_opcode=EC_BR_NOT_ZERO;
      OptBrNotZero(lx,tempii);
      tempii=tempi->t.p2t+1;
      tempii->t.p1t=tempi->t.p2t;
      tempii->t.p1c=tempi->t.p2c;
      tempii->ic_data=lb_true;
      tempii->ic_opcode=EC_BR_NOT_ZERO;
      tempii=OptBrNotZero(lx,tempii);
    } else if (tempi->ic_opcode==EC_BR_AND_AND_NOT_ZERO) {
      lb_false=CbMiscNew(lx,CBMT_LABEL);
      tempi->ic_opcode=EC_LABEL;
      tempi->ic_flags=0;
      tempi->ic_data=lb_false;
      tempii=tempi->t.p1t+1;
      tempii->t.p1t=tempi->t.p1t;
      tempii->t.p1c=tempi->t.p1c;
      tempii->ic_data=lb_false;
      tempii->ic_opcode=EC_BR_ZERO;
      OptBrZero(lx,tempii);
      tempii=tempi->t.p2t+1;
      tempii->t.p1t=tempi->t.p2t;
      tempii->t.p1c=tempi->t.p2c;
      tempii->ic_data=lb_true;
      tempii->ic_opcode=EC_BR_NOT_ZERO;
      tempii=OptBrNotZero(lx,tempii);
    } else
      tempii=tempi;
    if (tempi->ic_flags&ICF_POP_CMP && tempi->t.p1t->ic_opcode==EC_NOP) {
      tempi->t.p1t=tempi->t.p1t->t.p1t;
      tempi->ic_flags&=~ICF_POP_CMP;
    }
    return tempii;
  }
  return tempi;
}

IntermediateCode *OptPass012(LexStruct *lx,FunStruct *fun_class,BoolI8 combined)
{
/*
Simplify CONST arithmetic.
Sets the class throughout Expression trees.
Returns the type of an Expression for use
in int<-->F64 conversions of fun
args.
*/
  I64 code,i;
  BoolI8 is_unsigned;
  ClassStruct *tempc,*tempc1,*tempc2,*tempc3,*tempcc;
  IntermediateCode *tempi,*tempi1,*tempi2,*tempi3,*tempi_push,
      *temp_lag[4],*last_with_class=NULL,*tempii;
  PrsStk *ps=OptStart(lx,temp_lag);
  CbMiscStruct *lb,*lb1,*lb2;
  if (combined)
    tempi=lx->cbh.cb_in_ptr;
  else
    tempi=CbGetIc1(&lx->cbh);
  if (tempi->ic_opcode==EC_START) {
    if (combined)
      tempi++;
    else
      tempi=CbGetIc1(&lx->cbh);
  }
    #assert !EC_END
  while (code=tempi->ic_opcode) {
    OptNext(tempi,temp_lag);
    tempc=tempi->ic_class;
    tempi->ic_class2=tempc;
    tempi_push=tempi;
    MemSet(&tempi->p1,0,sizeof(IcArg)*4);
    nobound_switch (cmp_arg_cnt[code]) {
      case 9:
	ps->ptr-=tempi->ic_data>>2;
	break;
      case 3:
	tempi3=PrsPop(ps);
	tempc3=tempi3->ic_class;
	tempi->t.p3t=tempi3;
	tempi->t.p3c=PrsPop(ps);
      case 2:
	tempi2=PrsPop(ps);
	tempc2=tempi2->ic_class;
	tempi->t.p2t=tempi2;
	tempi->t.p2c=PrsPop(ps);
      case 1:
	tempi1=PrsPop(ps);
	tempc1=tempi1->ic_class;
	tempi->t.p1t=tempi1;
	tempi->t.p1c=PrsPop(ps);
	break;
      case 0: //nobound_switch
	break;
    }
    if (Bt(not_constant_bitmap,code))
      lx->flags|=LF_NOT_CONSTANT;
    nobound_switch (code) {
      case EC_F64:
	tempi->ic_flags&=~ICF_R_TO_F64;
	if (lx->pass==2 && tempi->ic_flags&ICF_R_TO_INT) {
	  tempi->ic_data=ToI64(tempi->ic_data(F64));
	  tempi->ic_flags&=~ICF_R_TO_INT;
	  tempi->ic_opcode=EC_IMM;
	  tempi->ic_class=sys_internal_types[IT_I64];
	}
	break;
      case EC_IMM:
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (lx->pass==2 && tempi->ic_flags&ICF_R_TO_F64) {
	  tempi->ic_data(F64)=ToF64(tempi->ic_data);
	  tempi->ic_flags&=~ICF_R_TO_F64;
	  tempi->ic_opcode=EC_F64;
	  tempi->ic_class=sys_internal_types[IT_F64];
	}
	break;
      case EC_LT_TYPECAST:
	if (tempi1->ic_opcode==EC_IMM || tempi1->ic_opcode==EC_F64) {
	  if (tempi->ic_class->sub_type==IT_F64)
	    tempi1->ic_opcode=EC_F64;
	  else
	    tempi1->ic_opcode=EC_IMM;
	  tempi1->ic_class=tempi->ic_class;
	  tempi1->ic_flags|=tempi->ic_flags;
	  tempi->ic_opcode=EC_NOP;
	  tempi->ic_flags=0;
	  tempi_push=tempi1;
	} else {
	  if (tempi->ic_data) { //was paren
	    if (!tempi_push->ic_class->ptr_cnt) {
	      if (tempi_push->ic_class->sub_type==IT_F64)
		tempi_push->ic_class2=sys_internal_types[IT_F64];
	      else
		tempi_push->ic_class2=sys_internal_types[IT_I64];
	    }
	  } else {
	    tempi1->ic_class=tempi->ic_class;
	    tempi1->ic_flags|=tempi->ic_flags;
	    tempi->ic_opcode=EC_NOP;
	    tempi->ic_flags=0;
	    tempi_push=tempi1;
	  }
	}
	break;
      case EC_FS:
      case EC_GS:
	//CALL,FS/GS,CALL_END,IMM,ADD,DEREF-->MOV_FS/GS
	if (combined) {
	  tempi1=tempi+2; //IMM
	  tempi2=tempi+3; //ADD
	  tempi3=tempi+4; //DEREF
	  if (tempi1->ic_opcode==EC_IMM && tempi2->ic_opcode==EC_ADD &&
	      tempi3->ic_opcode==EC_DEREF && !(tempi3->ic_flags&~ICF_NO_CVT_MASK)) {
	    tempi1->ic_opcode=EC_NOP;
	    tempi2->ic_opcode=EC_NOP;
	    tempi3->ic_opcode=EC_NOP;
	    if (tempi->ic_opcode==EC_FS)
	      tempi->ic_opcode=EC_MOV_FS;
	    else
	      tempi->ic_opcode=EC_MOV_GS;
	    tempi->ic_data=tempi1->ic_data;
	    tempi->ic_flags|=tempi1->ic_flags|tempi2->ic_flags;
	    tempi1->ic_flags=0;
	    tempi2->ic_flags=0;
	    tempi->ic_class =tempi3->ic_class;
	    tempi->ic_class2=tempi3->ic_class2;

	    tempi1=tempi-1; //CALL
	    tempi2=tempi+1; //CALL_END
	    tempi1->ic_opcode=EC_NOP;
	    tempi2->ic_opcode=EC_NOP;
	    tempi->ic_flags|=tempi1->ic_flags|tempi2->ic_flags;
	    tempi1->ic_flags=0;
	    tempi2->ic_flags=0;
	  }
	}
	break;
      case EC_STR_CONST:
      case EC_RBP:
      case EC_MOV_FS:
      case EC_MOV_GS:
      case EC_RIP:
      case EC_SIZEOF:
	break;
      case EC_PUSH_CMP:
	tempi1=temp_lag[1];
	if (tempi1->ic_opcode==EC_AND_AND)
	  tempi1=temp_lag[2];
	tempi->ic_class=tempi1->ic_class;
	tempi->ic_class2=tempi->ic_class;
	tempi->ic_data=0;
	if (tempi->ic_class->sub_type==IT_F64)
	  tempi->ic_flags|=ICF_USE_F64;
	break;
      case EC_COM:
	if (tempc->sub_type==IT_F64) {
	  if (tempi1->ic_opcode==EC_F64) {
	    tempi->ic_data(F64)=~tempi1->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	    tempi->ic_flags|=tempi1->ic_flags;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	  }
	  break;
	}
	if (tempi1->ic_opcode==EC_IMM) {
	  tempi->ic_data=~tempi1->ic_data;
	  tempi->ic_opcode=EC_IMM;
	  tempi->ic_flags|=tempi1->ic_flags;
	  tempi1->ic_opcode=EC_NOP;
	  tempi1->ic_flags=0;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_NOT:
	if (tempc->sub_type==IT_F64) {
	  if (tempi1->ic_opcode==EC_F64) {
	    tempi->ic_data(F64)=!tempi1->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	    tempi->ic_flags|=tempi1->ic_flags;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	  }
	  break;
	}
	if (tempi1->ic_opcode==EC_IMM) {
	  tempi->ic_data=!tempi1->ic_data;
	  tempi->ic_opcode=EC_IMM;
	  tempi->ic_flags|=tempi1->ic_flags;
	  tempi1->ic_opcode=EC_NOP;
	  tempi1->ic_flags=0;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_UNARY_MINUS:
	if (i=OptFixupUnaryOp(tempi,tempi1,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    tempi->ic_data=-tempi1->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=-tempi1->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	}
	if (tempc1->type&HTT_INTERNAL_TYPE && tempc1->sub_type& 1)
	  tempi->ic_class=sys_internal_types[tempc1->sub_type-1];
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_DEREF:
	if (lx->pass==2) {
	  if (!tempc->size)
	    PutLexFunWarn(lx,fun_class,"ST_DEREF_U0");
	  if (tempi1->ic_class->sub_type!=IT_F64) {
	    if (tempi1->ic_opcode==EC__PP) {
	      tempi->ic_opcode=EC_DEREF_PP;
	      tempi1->ic_opcode=EC_NOP;
	      tempi->ic_flags|=tempi1->ic_flags;
	      tempi1->ic_flags=0;
	    } else if (tempi1->ic_opcode==EC__MM) {
	      tempi->ic_opcode=EC_DEREF_MM;
	      tempi1->ic_opcode=EC_NOP;
	      tempi->ic_flags|=tempi1->ic_flags;
	      tempi1->ic_flags=0;
	    }
	  }
	}
	break;
      case EC__PP:
      case EC__MM:
      case EC_PP_:
      case EC_MM_:
	if (lx->pass==2 && !tempc->size)
	  PutLexFunWarn(lx,fun_class,"ST_DEREF_U0");
	break;
      case EC_SHL_CONST:
	if (i=OptFixupUnaryOp(tempi,tempi1,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)<<tempi->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)<<tempi->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)<<tempi->ic_data;
	    tempi->ic_opcode=EC_F64;
	  }
	} else if (tempi1->ic_opcode==EC_SHL_CONST) {
	  tempi->ic_flags|=tempi1->ic_flags;
	  tempi1->ic_flags=0;
	  tempi->ic_data+=tempi1->ic_data;
	  tempi1->ic_opcode=EC_NOP;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_SHR_CONST:
	if (i=OptFixupUnaryOp(tempi,tempi1,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)>>tempi->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)>>tempi->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)>>tempi->ic_data;
	    tempi->ic_opcode=EC_F64;
	  }
	} else if (tempi1->ic_opcode==EC_SHR_CONST) {
	  tempi->ic_flags|=tempi1->ic_flags;
	  tempi1->ic_flags=0;
	  tempi->ic_data+=tempi1->ic_data;
	  tempi1->ic_opcode=EC_NOP;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;

      case EC_POWER:
	if (tempc1->sub_type>tempc2->sub_type)
	  tempi->ic_class=tempc1;
	else
	  tempi->ic_class=tempc2;
	tempc=tempi->ic_class;
	if (tempc1->sub_type!=IT_F64)
	  tempi1->ic_flags|=ICF_R_TO_F64;
	if (tempc2->sub_type!=IT_F64)
	  tempi2->ic_flags|=ICF_R_TO_F64;
	if (tempc->sub_type!=IT_F64)
	  tempi->ic_flags|=ICF_R_TO_INT|ICF_USE_F64;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_SHL:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)<<tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)<<tempi2->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64) << tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else if (tempi2->ic_opcode==EC_IMM) {
	  tempi->ic_flags|=tempi2->ic_flags;
	  tempi2->ic_opcode=EC_NOP;
	  tempi2->ic_flags=0;
	  tempi->ic_data=tempi2->ic_data;
	  tempi->ic_opcode=EC_SHL_CONST;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_SHR:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)>>tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)>>tempi2->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64) >> tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else if (tempi2->ic_opcode==EC_IMM) {
	  tempi->ic_flags|=tempi2->ic_flags;
	  tempi2->ic_opcode=EC_NOP;
	  tempi2->ic_flags=0;
	  tempi->ic_data=tempi2->ic_data;
	  tempi->ic_opcode=EC_SHR_CONST;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_MUL:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)*tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)*tempi2->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)*
		tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else {
	  if (tempi1->ic_opcode==EC_IMM && lx->pass==2) {
	    switch (i=tempi1->ic_data) {
	      case 0:
		break;
	      case 1:
		tempi2->ic_flags|=tempi->ic_flags|tempi1->ic_flags;
		tempi1->ic_opcode=EC_NOP;
		tempi1->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi2->ic_class2=tempi->ic_class2;
		tempi_push=tempi2;
		break;
	      default:
		if (Bsf(i)==Bsr(i)) {
		  tempi->ic_flags|=tempi1->ic_flags;
		  tempi->t.p1c=tempi->t.p2c;
		  tempi1->ic_opcode=EC_NOP;
		  tempi1->ic_flags=0;
		  tempi->ic_data=Bsf(i);
		  tempi->ic_opcode=EC_SHL_CONST;
		}
	    }
	  } else if (tempi2->ic_opcode==EC_IMM) {
	    switch (i=tempi2->ic_data) {
	      case 0:
		break;
	      case 1:
		tempi1->ic_flags|=tempi->ic_flags|tempi2->ic_flags;
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
		break;
	      default:
		if (Bsf(i)==Bsr(i)) {
		  tempi->ic_flags|=tempi2->ic_flags;
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_data=Bsf(i);
		  tempi->ic_opcode=EC_SHL_CONST;
		} else if (tempi1->ic_opcode==EC_MUL && lx->pass==2) {
		  if (tempi1->t.p1t->ic_opcode==EC_IMM) {
		    tempi1->ic_flags|=tempi->ic_flags;
		    tempi1->t.p1t->ic_data*=tempi2->ic_data;
		    tempi2->ic_opcode=EC_NOP;
		    tempi2->ic_flags=0;
		    tempi->ic_opcode=EC_NOP;
		    tempi->ic_flags=0;
		    tempi1->ic_class2=tempi->ic_class2;
		    tempi_push=tempi1;
		  } else if (tempi1->t.p2t->ic_opcode==EC_IMM) {
		    tempi1->ic_flags|=tempi->ic_flags;
		    tempi1->t.p2t->ic_data*=tempi2->ic_data;
		    tempi2->ic_opcode=EC_NOP;
		    tempi2->ic_flags=0;
		    tempi->ic_opcode=EC_NOP;
		    tempi->ic_flags=0;
		    tempi1->ic_class2=tempi->ic_class2;
		    tempi_push=tempi1;
		  }
		}
	    }
	  } else if (tempi2->ic_opcode==EC_F64 && lx->pass==2) {
	    if (tempi2->ic_data(F64)==1.0) {
	      tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
	      tempi2->ic_opcode=EC_NOP;
	      tempi2->ic_flags=0;
	      tempi->ic_opcode=EC_NOP;
	      tempi->ic_flags=0;
	      tempi1->ic_class2=tempi->ic_class2;
	      tempi_push=tempi1;
	    } else if (tempi1->ic_opcode==EC_MUL) {
	      if (tempi1->t.p1t->ic_opcode==EC_F64) {
		tempi1->ic_flags|=tempi->ic_flags|tempi2->ic_flags;
		tempi1->t.p1t->ic_data(F64)*=tempi2->ic_data(F64);
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      } else if (tempi1->t.p2t->ic_opcode==EC_F64) {
		tempi1->ic_flags|=tempi->ic_flags|tempi2->ic_flags;
		tempi1->t.p2t->ic_data(F64)*=tempi2->ic_data(F64);
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      }
	    }
	  }
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_DIV:
	if ((tempi2->ic_data || tempi2->ic_opcode!=EC_IMM && tempi2->ic_opcode!=EC_F64) &&
	    (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned))) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)/tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)/tempi2->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)/
		tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else {
	  if (tempi2->ic_opcode==EC_IMM && (i=tempi2->ic_data)) {
	    if (i==1) {
	      tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
	      tempi2->ic_opcode=EC_NOP;
	      tempi2->ic_flags=0;
	      tempi->ic_opcode=EC_NOP;
	      tempi->ic_flags=0;
	      tempi1->ic_class2=tempi->ic_class2;
	      tempi_push=tempi1;
	    } else if (Bsf(i)==Bsr(i)) {
	      tempi->ic_flags|=tempi2->ic_flags;
	      tempi2->ic_opcode=EC_NOP;
	      tempi2->ic_flags=0;
	      tempi->ic_data=Bsf(i);
	      tempi->ic_opcode=EC_SHR_CONST;
	    }
	  }
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_MOD:
	if ((tempi2->ic_data || tempi2->ic_opcode!=EC_IMM && tempi2->ic_opcode!=EC_F64) &&
	    (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned))) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)%tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)%tempi2->ic_data(I64);
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)%
		tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else if (lx->pass==2 && tempi2->ic_opcode==EC_IMM &&
	    (i=tempi2->ic_data) && Bsf(i)==Bsr(i) && tempi_push->ic_class->sub_type!=IT_F64 &&
	    tempi_push->ic_class->sub_type&1) { //do only unsigned
	  tempi2->ic_data=i-1;
	  tempi->ic_opcode=EC_AND;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_AND:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data&tempi2->ic_data;
	  if (i==FBO1_INT)
	    tempi->ic_opcode=EC_IMM;
	  else
	    tempi->ic_opcode=EC_F64;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_OR:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data|tempi2->ic_data;
	  if (i==FBO1_INT)
	    tempi->ic_opcode=EC_IMM;
	  else
	    tempi->ic_opcode=EC_F64;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_XOR:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data^tempi2->ic_data;
	  if (i==FBO1_INT)
	    tempi->ic_opcode=EC_IMM;
	  else
	    tempi->ic_opcode=EC_F64;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_ADD:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    tempi->ic_data=tempi1->ic_data+tempi2->ic_data;
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)+
		tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else {
	  if (tempi1->ic_opcode==EC_ABS_ADDR&&tempi2->ic_opcode==EC_IMM) {
	    tempi->ic_flags|=tempi1->ic_flags|tempi2->ic_flags;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi2->ic_opcode=EC_NOP;
	    tempi2->ic_flags=0;
	    tempi->ic_data=tempi1->ic_data+tempi2->ic_data;
	    tempi->ic_opcode=EC_ABS_ADDR;
	  } else if (lx->pass==2) {
	    if (tempi1->ic_opcode==EC_IMM) {
	      if (!tempi1->ic_data) {
		tempi2->ic_flags|=tempi1->ic_flags|tempi->ic_flags;
		tempi1->ic_opcode=EC_NOP;
		tempi1->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi2->ic_class2=tempi->ic_class2;
		tempi_push=tempi2;
	      } else if (tempi2->ic_opcode==EC_ADD||
		  tempi2->ic_opcode==EC_SUB) {
		if (tempi2->t.p1t->ic_opcode==EC_IMM) {
		  tempi2->ic_flags|=tempi->ic_flags;
		  tempi2->t.p1t->ic_data+=tempi1->ic_data;
		  tempi1->ic_opcode=EC_NOP;
		  tempi1->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi2->ic_class2=tempi->ic_class2;
		  tempi_push=tempi2;
		} else if (tempi2->t.p2t->ic_opcode==EC_IMM) {
		  tempi2->ic_flags|=tempi->ic_flags;
		  if (tempi2->ic_opcode==EC_ADD)
		    tempi2->t.p2t->ic_data+=tempi1->ic_data;
		  else
		    tempi2->t.p2t->ic_data-=tempi1->ic_data;
		  tempi1->ic_opcode=EC_NOP;
		  tempi1->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi2->ic_class2=tempi->ic_class2;
		  tempi_push=tempi2;
		}
	      }
	    } else if (tempi2->ic_opcode==EC_IMM) {
	      if (!tempi2->ic_data) {
		tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      } else if (tempi1->ic_opcode==EC_ADD ||
		  tempi1->ic_opcode==EC_SUB) {
		if (tempi1->t.p1t->ic_opcode==EC_IMM) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  tempi1->t.p1t->ic_data+=tempi2->ic_data;
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		} else if (tempi1->t.p2t->ic_opcode==EC_IMM) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  if (tempi1->ic_opcode==EC_ADD)
		    tempi1->t.p2t->ic_data+=tempi2->ic_data;
		  else
		    tempi1->t.p2t->ic_data-=tempi2->ic_data;
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		}
	      }
	    } else if (tempi1->ic_opcode==EC_F64) {
	      if (!tempi1->ic_data) {
		tempi2->ic_flags|=tempi1->ic_flags|tempi->ic_flags;
		tempi1->ic_opcode=EC_NOP;
		tempi1->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi2->ic_class2=tempi->ic_class2;
		tempi_push=tempi2;
	      } else if (tempi2->ic_opcode==EC_ADD||
		  tempi2->ic_opcode==EC_SUB) {
		if (tempi2->t.p1t->ic_opcode==EC_F64) {
		  tempi2->ic_flags|=tempi->ic_flags;
		  tempi2->t.p1t->ic_data(F64)+=tempi1->ic_data(F64);
		  tempi1->ic_opcode=EC_NOP;
		  tempi1->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi2->ic_class2=tempi->ic_class2;
		  tempi_push=tempi2;
		} else if (tempi2->t.p2t->ic_opcode==EC_F64) {
		  tempi2->ic_flags|=tempi->ic_flags;
		  if (tempi2->ic_opcode==EC_ADD)
		    tempi2->t.p2t->ic_data(F64)+=tempi1->ic_data(F64);
		  else
		    tempi2->t.p2t->ic_data(F64)-=tempi1->ic_data(F64);
		  tempi1->ic_opcode=EC_NOP;
		  tempi1->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi2->ic_class2=tempi->ic_class2;
		  tempi_push=tempi2;
		}
	      }
	    } else if (tempi2->ic_opcode==EC_F64) {
	      if (!tempi2->ic_data) {
		tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      } else if (tempi1->ic_opcode==EC_ADD ||
		  tempi1->ic_opcode==EC_SUB) {
		if (tempi1->t.p1t->ic_opcode==EC_F64) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  tempi1->t.p1t->ic_data(F64)+=tempi2->ic_data(F64);
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		} else if (tempi1->t.p2t->ic_opcode==EC_F64) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  if (tempi1->ic_opcode==EC_ADD)
		    tempi1->t.p2t->ic_data(F64)+=tempi2->ic_data(F64);
		  else
		    tempi1->t.p2t->ic_data(F64)-=tempi2->ic_data(F64);
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		}
	      }
	    }
	  }
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_SUB:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    tempi->ic_data=tempi1->ic_data-tempi2->ic_data;
	    tempi->ic_opcode=EC_IMM;
	  } else {
	    tempi->ic_data(F64)=tempi1->ic_data(F64)-
		tempi2->ic_data(F64);
	    tempi->ic_opcode=EC_F64;
	  }
	} else {
	  if (lx->pass==2) {
	    if (tempi2->ic_opcode==EC_IMM) {
	      if (!tempi2->ic_data) {
		tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      } else
	      if (tempi1->ic_opcode==EC_ADD ||
		  tempi1->ic_opcode==EC_SUB) {
		if (tempi1->t.p1t->ic_opcode==EC_IMM) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  tempi1->t.p1t->ic_data-=tempi2->ic_data;
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		} else if (tempi1->t.p2t->ic_opcode==EC_IMM) {
		  tempi1->ic_flags|=tempi->ic_flags;
		  if (tempi1->ic_opcode==EC_ADD)
		    tempi1->t.p2t->ic_data-=tempi2->ic_data;
		  else
		    tempi1->t.p2t->ic_data+=tempi2->ic_data;
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		}
	      }
	    } else
	    if (tempi2->ic_opcode==EC_F64) {
	      if (!tempi2->ic_data) {
		tempi1->ic_flags|=tempi2->ic_flags|tempi->ic_flags;
		tempi2->ic_opcode=EC_NOP;
		tempi2->ic_flags=0;
		tempi->ic_opcode=EC_NOP;
		tempi->ic_flags=0;
		tempi1->ic_class2=tempi->ic_class2;
		tempi_push=tempi1;
	      } else
	      if (tempi1->ic_opcode==EC_ADD ||
		  tempi1->ic_opcode==EC_SUB) {
		if (tempi1->t.p1t->ic_opcode==EC_F64) {
		  tempi1->ic_flags|=tempi->ic_flags|tempi2->ic_flags;
		  tempi1->t.p1t->ic_data(F64)-=tempi2->ic_data(F64);
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		} else if (tempi1->t.p2t->ic_opcode==EC_F64) {
		  tempi1->ic_flags|=tempi->ic_flags|tempi2->ic_flags;
		  if (tempi1->ic_opcode==EC_ADD)
		    tempi1->t.p2t->ic_data(F64)-=tempi2->ic_data(F64);
		  else
		    tempi1->t.p2t->ic_data(F64)+=tempi2->ic_data(F64);
		  tempi2->ic_opcode=EC_NOP;
		  tempi2->ic_flags=0;
		  tempi->ic_opcode=EC_NOP;
		  tempi->ic_flags=0;
		  tempi1->ic_class2=tempi->ic_class2;
		  tempi_push=tempi1;
		}
	      }
	    }
	  }

	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_EQUAL_EQUAL:
	if (OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data==tempi2->ic_data;
	  tempi->ic_opcode=EC_IMM;
	} else
	if (tempi->ic_class->sub_type==IT_F64)
	  tempi->ic_flags|=ICF_USE_F64;
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_NOT_EQUAL:
	if (OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data!=tempi2->ic_data;
	  tempi->ic_opcode=EC_IMM;
	} else
	if (tempi->ic_class->sub_type==IT_F64)
	  tempi->ic_flags|=ICF_USE_F64;
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_LESS:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)<tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)<tempi2->ic_data(I64);
	  } else {
	    tempi->ic_data=tempi1->ic_data(F64)<
		tempi2->ic_data(F64);
	  }
	  tempi->ic_opcode=EC_IMM;
	} else {
	  if (is_unsigned)
	    tempi->ic_flags|=ICF_USE_UNSIGNED;
	  if (tempi->ic_class->sub_type==IT_F64)
	    tempi->ic_flags|=ICF_USE_F64;
	}
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_GREATER:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)>tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)>tempi2->ic_data(I64);
	  } else {
	    tempi->ic_data=tempi1->ic_data(F64)>
		tempi2->ic_data(F64);
	  }
	  tempi->ic_opcode=EC_IMM;
	} else {
	  if (is_unsigned)
	    tempi->ic_flags|=ICF_USE_UNSIGNED;
	  if (tempi->ic_class->sub_type==IT_F64)
	    tempi->ic_flags|=ICF_USE_F64;
	}
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_LESS_EQUAL:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)<=tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)<=tempi2->ic_data(I64);
	  } else {
	    tempi->ic_data=tempi1->ic_data(F64)<=
		tempi2->ic_data(F64);
	  }
	  tempi->ic_opcode=EC_IMM;
	} else {
	  if (is_unsigned)
	    tempi->ic_flags|=ICF_USE_UNSIGNED;
	  if (tempi->ic_class->sub_type==IT_F64)
	    tempi->ic_flags|=ICF_USE_F64;
	}
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_GREATER_EQUAL:
	if (i=OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  if (i==FBO1_INT) {
	    if (is_unsigned)
	      tempi->ic_data=tempi1->ic_data(U64)>=tempi2->ic_data(U64);
	    else
	      tempi->ic_data=tempi1->ic_data(I64)>=tempi2->ic_data(I64);
	  } else {
	    tempi->ic_data=tempi1->ic_data(F64)>=
		tempi2->ic_data(F64);
	  }
	  tempi->ic_opcode=EC_IMM;
	} else {
	  if (is_unsigned)
	    tempi->ic_flags|=ICF_USE_UNSIGNED;
	  if (tempi->ic_class->sub_type==IT_F64)
	    tempi->ic_flags|=ICF_USE_F64;
	}
	tempi->ic_flags&=~ICF_R_TO_INT;
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	if (tempi_push->ic_flags & ICF_PUSH_CMP)
	  tempi->ic_class=tempi->ic_class2;
	else
	  tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_AND_AND:
	if (OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data&&tempi2->ic_data;
	  tempi->ic_opcode=EC_IMM;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_OR_OR:
	if (OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data||tempi2->ic_data;
	  tempi->ic_opcode=EC_IMM;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_XOR_XOR:
	if (OptFixupBinaryOp1(tempi,tempi1,tempi2,&is_unsigned)) {
	  tempi->ic_data=tempi1->ic_data^^tempi2->ic_data;
	  tempi->ic_opcode=EC_IMM;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else if (is_unsigned)
	    tempi_push->ic_class2=sys_internal_types[IT_U64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	tempi->ic_class=sys_internal_types[IT_I64];
	break;
      case EC_COLON:
	if (tempc2->sub_type>tempc3->sub_type)
	  tempi->ic_class=tempc2;
	else
	  tempi->ic_class=tempc3;
	tempc=tempi->ic_class;
	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	  if (tempc3->sub_type!=IT_F64)
	    tempi3->ic_flags|=ICF_R_TO_F64;
	}
	if (!tempi_push->ic_class->ptr_cnt) {
	  if (tempi_push->ic_class->sub_type==IT_F64)
	    tempi_push->ic_class2=sys_internal_types[IT_F64];
	  else
	    tempi_push->ic_class2=sys_internal_types[IT_I64];
	}
	break;
      case EC_MUL_EQUAL:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempi2->ic_opcode==EC_IMM &&
	    tempc->sub_type!=IT_F64 &&
	    tempc2->sub_type!=IT_F64) {
	  if (i=tempi2->ic_data) {
	    if (Bsf(i)==Bsr(i)) {
	      tempi2->ic_data=Bsf(i);
	      tempi->ic_opcode=EC_SHL_EQUAL;
	    }
	  }
	}
	if (tempi2->ic_class->sub_type==IT_F64)
	  tempi->ic_flags=tempi->ic_flags|ICF_USE_F64;
	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	}
	break;
      case EC_DIV_EQUAL:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempi2->ic_opcode==EC_IMM &&
	    tempc->sub_type!=IT_F64 &&
	    tempc2->sub_type!=IT_F64 &&
	    (i=tempi2->ic_data) && Bsf(i)==Bsr(i)) {
	  tempi2->ic_data=Bsf(i);
	  tempi->ic_opcode=EC_SHR_EQUAL;
	}
	if (tempi2->ic_class->sub_type==IT_F64)
	  tempi->ic_flags=tempi->ic_flags|ICF_USE_F64;
	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	}
	break;
      case EC_MOD_EQUAL:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempi2->ic_opcode==EC_IMM &&
	    tempc->sub_type!=IT_F64 &&
	    tempc2->sub_type!=IT_F64 &&
	    (i=tempi2->ic_data) && Bsf(i)==Bsr(i)) {
	  tempi2->ic_data=i-1;
	  tempi->ic_opcode=EC_AND_EQUAL;
	}
	if (tempi2->ic_class->sub_type==IT_F64)
	  tempi->ic_flags=tempi->ic_flags|ICF_USE_F64;
	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	}
	break;
      case EC_ADD_EQUAL:
      case EC_SUB_EQUAL:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempi2->ic_class->sub_type==IT_F64)
	  tempi->ic_flags=tempi->ic_flags|ICF_USE_F64;
	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	}
	break;
      case EC_ASSIGN:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempc->sub_type==IT_F64) {
	  if (tempc2->sub_type!=IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_F64;
	} else {
	  if (tempc2->sub_type==IT_F64)
	    tempi2->ic_flags|=ICF_R_TO_INT;
	}
	if (lx->pass==2 && tempi1->ic_class->sub_type!=IT_F64) {
	  if (tempi1->ic_opcode==EC__PP) {
	    tempi->ic_opcode=EC_ASSIGN_PP;
	    tempi1->ic_opcode=EC_NOP;
	    tempi->ic_flags|=tempi1->ic_flags;
	    tempi1->ic_flags=0;
	    tempi->t.p3c=tempi1->ic_class;
	  } else if (tempi1->ic_opcode==EC__MM) {
	    tempi->ic_opcode=EC_ASSIGN_MM;
	    tempi1->ic_opcode=EC_NOP;
	    tempi->ic_flags|=tempi1->ic_flags;
	    tempi1->ic_flags=0;
	    tempi->t.p3c=tempi1->ic_class;
	  }
	}
	break;
      case EC_SHL_EQUAL:
      case EC_SHR_EQUAL:
      case EC_AND_EQUAL:
      case EC_OR_EQUAL:
      case EC_XOR_EQUAL:
	if (lx->pass==2 && (!tempc->size||!tempc2->size)) PutLexFunWarn(lx,fun_class,"ST_ASSIGN_U0");

	if (tempc2->sub_type==IT_F64)
	  tempi2->ic_flags|=ICF_R_TO_INT;
	break;
      case EC_ENTER:
      case EC_LEAVE:
	tempi->ic_data=-fun_class->size;
	break;
      case EC_ADD_RSP:
	if (temp_lag[1]->ic_opcode==EC_ADD_RSP) {
	  tempi->ic_data+=temp_lag[1]->ic_data;
	  tempi->ic_flags|=temp_lag[1]->ic_flags;
	  temp_lag[1]->ic_opcode=EC_NOP;
	  temp_lag[1]->ic_flags=0;
	}
      case EC_ADD_RSP1:
	break;

      case EC_CALL:
      case EC_CALL_INDIRECT:
      case EC_CALL_INDIRECT2:
      case EC_CALL_EXTERN:
      case EC_CALL_IMPORT:
      case EC_CLI:
      case EC_STI:
      case EC_HLT:
      case EC_PUSHFD:
      case EC_POPFD:
      case EC_PUSH:
      case EC_POP:
      case EC_CLD:
      case EC_STD:
      case EC_WBINVD:
      case EC_INVD:
      case EC_INVLPG:
      case EC_CLFLUSH:
      case EC_PAUSE:
      case EC_GET_FLAGS:
      case EC_GET_BASE_PTR:
      case EC_GET_STK_PTR:
      case EC_GET_RAX:
      case EC_RETURN_VAL:
      case EC_RETURN_VAL2:
      case EC_ABS_ADDR:
      case EC_HEAP_GLBL:
      case EC_ADDR_IMPORT:
      case EC_GET_LABEL:
      case EC_TYPE:
      case EC_RDTSC:
      case EC_HPET:
      case EC_SET_FLAGS:
      case EC_SET_BASE_PTR:
      case EC_SET_STK_PTR:
      case EC_SET_RAX:
      case EC_BSF:
      case EC_BSR:
      case EC_PREEMPT:
      case EC_SIGN_I64:
      case EC_TOUPPER:
      case EC_ABS_I64:
      case EC_LIMIT_I64:
      case EC_LIMIT_U64:
      case EC_MIN_I64:
      case EC_MAX_I64:
      case EC_MIN_U64:
      case EC_MAX_U64:
      case EC_MOD_U64:
      case EC_SQR_I64:
      case EC_SQR_U64:
      case EC_SWAP_U8:
      case EC_SWAP_U16:
      case EC_SWAP_U32:
      case EC_SWAP_U64:
      case EC_INU32:
      case EC_INU16:
      case EC_INU8:
      case EC_STRLEN:
      case EC_BT:
      case EC_BTS:
      case EC_BTR:
      case EC_BTC:
      case EC_BEQU:
      case EC_REMQUE:
      case EC_INSQUE:
      case EC_INSQUE_REV:
      case EC_OUTU32:
      case EC_OUTU16:
      case EC_OUTU8:
      case EC_NOBOUND_SWITCH:
      case EC_SWITCH:
      case EC_MEMCPY:
      case EC_MEMSET:
      case EC_MEMSET_U16:
      case EC_MEMSET_U32:
      case EC_MEMSET_U64:
      case EC_ASM:
	break;
      case EC_LBEQU:
      case EC_LBTS:
      case EC_LBTR:
      case EC_LBTC:
	tempi->ic_flags|=ICF_LOCK;
	break;
      case EC_TO_I64:
	if (combined) {
	  if (tempi1->ic_opcode==EC_F64) {
	    tempi->ic_flags|=tempi1->ic_flags&~ICF_R_TO_INT;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi->ic_opcode=EC_IMM;
	    tempi->ic_data=ToI64(tempi1->ic_data(F64));
	    tempi->ic_class=sys_internal_types[IT_I64];
	    tempi->ic_class2=sys_internal_types[IT_I64];

	    tempii=tempi1-1;
	    while (tempii->ic_opcode!=EC_CALL_START)
	      tempii--;
	    tempii->ic_opcode=EC_NOP;

	    tempii=tempi+1;
	    while (tempii->ic_opcode!=EC_CALL_END)
	      tempii++;
	    tempii->ic_opcode=EC_NOP;

	  } else if (tempi1->ic_opcode==EC_IMM) {
	    tempi->ic_flags|=tempi1->ic_flags&~ICF_R_TO_F64;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi->ic_opcode=EC_IMM;
	    tempi->ic_data=tempi1->ic_data;
	    tempi->ic_class=sys_internal_types[IT_I64];
	    tempi->ic_class2=sys_internal_types[IT_I64];

	    tempii=tempi1-1;
	    while (tempii->ic_opcode!=EC_CALL_START)
	      tempii--;
	    tempii->ic_opcode=EC_NOP;

	    tempii=tempi+1;
	    while (tempii->ic_opcode!=EC_CALL_END)
	      tempii++;
	    tempii->ic_opcode=EC_NOP;

	  }
	  if (tempi1->ic_flags&ICF_R_TO_F64) {
	    tempi1->ic_flags=tempi->ic_flags|tempi1->ic_flags&~(ICF_R_TO_F64|ICF_PUSH_RESULT);
	    tempi->ic_flags=0;
	    tempi->ic_opcode=EC_NOP;
	    tempi1->ic_class=sys_internal_types[IT_I64];
	    tempi1->ic_class2=sys_internal_types[IT_I64];
	    tempi_push=tempi1;

	    i=0;
	    tempii=tempi1-1;
	    while (TRUE) {
	      if (tempii->ic_opcode==EC_CALL_START) {
		if (!i) break;
		i--;
	      } else if (tempii->ic_opcode==EC_CALL_END)
		i++;
	      tempii--;
	    }
	    tempii->ic_opcode=EC_NOP;

	    i=0;
	    tempii=tempi1+1;
	    while (TRUE) {
	      if (tempii->ic_opcode==EC_CALL_END) {
		if (!i) break;
		i--;
	      } else if (tempii->ic_opcode==EC_CALL_START)
		i++;
	      tempii++;
	    }
	    tempii->ic_opcode=EC_NOP;
	  }
	}
	break;
      case EC_TO_F64:
	if (combined) {
	  if (tempi1->ic_opcode==EC_IMM) {
	    tempi->ic_flags|=tempi1->ic_flags&~ICF_R_TO_F64;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi->ic_opcode=EC_F64;
	    tempi->ic_data(F64)=ToF64(tempi1->ic_data);
	    tempi->ic_class=sys_internal_types[IT_F64];
	    tempi->ic_class2=sys_internal_types[IT_F64];

	    tempii=tempi1-1;
	    while (tempii->ic_opcode!=EC_CALL_START)
	      tempii--;
	    tempii->ic_opcode=EC_NOP;

	    tempii=tempi+1;
	    while (tempii->ic_opcode!=EC_CALL_END)
	      tempii++;
	    tempii->ic_opcode=EC_NOP;
	  } else if (tempi1->ic_opcode==EC_F64) {
	    tempi->ic_flags|=tempi1->ic_flags&~ICF_R_TO_INT;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi->ic_opcode=EC_F64;
	    tempi->ic_data=tempi1->ic_data;
	    tempi->ic_class=sys_internal_types[IT_F64];
	    tempi->ic_class2=sys_internal_types[IT_F64];

	    tempii=tempi1-1;
	    while (tempii->ic_opcode!=EC_CALL_START)
	      tempii--;
	    tempii->ic_opcode=EC_NOP;

	    tempii=tempi+1;
	    while (tempii->ic_opcode!=EC_CALL_END)
	      tempii++;
	    tempii->ic_opcode=EC_NOP;
	  }
	  if (tempi1->ic_flags&ICF_R_TO_INT) {
	    tempi1->ic_flags=tempi->ic_flags|tempi1->ic_flags&~(ICF_R_TO_INT|ICF_PUSH_RESULT);
	    tempi->ic_flags=0;
	    tempi->ic_opcode=EC_NOP;
	    tempi1->ic_class=sys_internal_types[IT_F64];
	    tempi1->ic_class2=sys_internal_types[IT_F64];
	    tempi_push=tempi1;

	    i=0;
	    tempii=tempi1-1;
	    while (TRUE) {
	      if (tempii->ic_opcode==EC_CALL_START) {
		if (!i) break;
		i--;
	      } else if (tempii->ic_opcode==EC_CALL_END)
		i++;
	      tempii--;
	    }
	    tempii->ic_opcode=EC_NOP;

	    i=0;
	    tempii=tempi1+1;
	    while (TRUE) {
	      if (tempii->ic_opcode==EC_CALL_END) {
		if (!i) break;
		i--;
	      } else if (tempii->ic_opcode==EC_CALL_START)
		i++;
	      tempii++;
	    }
	    tempii->ic_opcode=EC_NOP;
	  }
	}
	break;
      case EC_TO_BOOL:
	if (combined) {
	  if (tempi1->ic_opcode==EC_IMM || tempi1->ic_opcode==EC_F64) {
	    tempi->ic_flags|=tempi1->ic_flags&~ICF_R_TO_F64;
	    tempi1->ic_opcode=EC_NOP;
	    tempi1->ic_flags=0;
	    tempi->ic_opcode=EC_IMM;
	    tempi->ic_data=ToBool(tempi1->ic_data);
	    tempi->ic_class=sys_internal_types[IT_I64];
	    tempi->ic_class2=sys_internal_types[IT_I64];

	    tempii=tempi1-1;
	    while (tempii->ic_opcode!=EC_CALL_START)
	      tempii--;
	    tempii->ic_opcode=EC_NOP;

	    tempii=tempi+1;
	    while (tempii->ic_opcode!=EC_CALL_END)
	      tempii++;
	    tempii->ic_opcode=EC_NOP;

	  }
	}
	break;
      case EC_BR_ZERO:
	if (combined)
	  tempi_push=OptBrZero(lx,tempi);
	break;
      case EC_BR_NOT_ZERO:
	if (combined)
	  tempi_push=OptBrNotZero(lx,tempi);
	break;
      case EC_BR_NOT_EQUAL:
      case EC_BR_EQUAL_EQUAL:
      case EC_BR_GREATER_EQUAL:
      case EC_BR_LESS_EQUAL:
      case EC_BR_GREATER:
      case EC_BR_LESS:
      case EC_BR_NOT_EQUAL2:
      case EC_BR_EQUAL_EQUAL2:
      case EC_BR_GREATER_EQUAL2:
      case EC_BR_LESS_EQUAL2:
      case EC_BR_GREATER2:
      case EC_BR_LESS2:
      case EC_BR_OR_OR_NOT_ZERO:
      case EC_BR_OR_OR_ZERO:
      case EC_BR_AND_AND_NOT_ZERO:
      case EC_BR_AND_AND_ZERO:
      case EC_BR_AND_NOT_ZERO:
      case EC_BR_AND_ZERO:
      case EC_BR_MM_NOT_ZERO:
      case EC_BR_MM_ZERO:
      case EC_BR_BT:
      case EC_BR_BTS:
      case EC_BR_BTR:
      case EC_BR_BTC:
      case EC_BR_NOT_BT:
      case EC_BR_NOT_BTS:
      case EC_BR_NOT_BTR:
      case EC_BR_NOT_BTC:
	break;
      case EC_BR_LBTS:
      case EC_BR_LBTR:
      case EC_BR_LBTC:
      case EC_BR_NOT_LBTS:
      case EC_BR_NOT_LBTR:
      case EC_BR_NOT_LBTC:
	tempi->ic_flags|=ICF_LOCK;
	break;

      case EC_NOP:
	if (combined && tempi->ic_flags&ICF_PUSH_RESULT) {
	  tempii=tempi;
	  do tempii--;
	  while (tempii->ic_opcode==EC_NOP);
	  tempii->ic_flags|=ICF_PUSH_RESULT;
	  tempi->ic_flags&=~ICF_PUSH_RESULT;
	}
	break;
      case EC_NOP1:
	ps->ptr+=tempi->ic_data<<1;
	break;
      case EC_LABEL:
	lb=tempi->ic_data;
	while (lb->fwd)
	  lb=lb->fwd;
	lb1=tempi->ic_data;
	while (lb2=lb1->fwd) {
	  lb1->fwd=lb;
	  lb1=lb2;
	}
	tempii=temp_lag[1];
	if (tempii->ic_opcode==EC_JMP) {
	  lb1=tempii->ic_data;
	  while (lb1->fwd)
	    lb1=lb1->fwd;
	  if (lb1==lb) {
	    tempii->ic_opcode=EC_NOP;
	    tempi->ic_flags|=tempii->ic_flags;
	  }
	} else if (tempii->ic_opcode==EC_LABEL) {
	  lb1=tempii->ic_data;
	  if (!lb1->fwd)
	    lb1->fwd=lb;
	  tempii=temp_lag[2];
	  if (tempii->ic_opcode==EC_JMP) {
	    lb1=tempii->ic_data;
	    while (lb1->fwd)
	      lb1=lb1->fwd;
	    if (lb1==lb) {
	      tempii->ic_opcode=EC_NOP;
	      tempi->ic_flags|=tempii->ic_flags;
	    }
	  }
	}
	break;
      case EC_END:
      case EC_ADDR:
      case EC_QUESTION:
      case EC_RET:
      case EC_END_EXP:
      case EC_CALL_START:
      case EC_CALL_END:
      case EC_CALL_END2:
      case EC_PUSH_REGS:
      case EC_POP_REGS:
      case EC_IRQ:
	break;
      case EC_JMP:
	lb=tempi->ic_data;
	if (temp_lag[1]->ic_opcode==EC_LABEL) {
	  while (lb->fwd)
	    lb=lb->fwd;
	  lb1=temp_lag[1]->ic_data;
	  while (lb1->fwd)
	    lb1=lb1->fwd;
	  if (lb!=lb1)
	    lb1->fwd=lb;
	}
	break;
      default:
	OptMissing(lx,tempi);
    }
    if (cmp_arg_cnt[code]==2) {
      if (tempi_push->ic_precedence.u8[0]&~ASSOC_MASK==PREC_ASSIGN) {
	if (tempi2->ic_opcode==EC_MUL &&
	    tempi2->t.p2t->ic_opcode==EC_SIZEOF) {
	  tempcc=tempi1->ic_class-1;
	  goto here1;
	}
      } else {
	if (tempi1->ic_opcode==EC_MUL &&
	    tempi1->t.p2t->ic_opcode==EC_SIZEOF) {
	  tempcc=tempi2->ic_class;
	  tempi1->t.p2t->ic_opcode=EC_IMM;
	  tempi1->t.p2t->ic_class=tempcc;
	  tempi_push->ic_class=tempcc;
	  if (tempcc->ptr_cnt) {
	    tempcc--;
	    if (tempcc->size==1)
	      goto here2;
	    tempi1->t.p2t->ic_data=tempcc->size;
	  } else {
	    here2:
	    if (tempi_push==tempi1)
	      tempi1->t.p2t->ic_data=1;
	    else {
	      tempi1->ic_opcode=EC_NOP;
	      tempi1->ic_flags=0;
	      tempi1->t.p2t->ic_opcode=EC_NOP;
	      tempi1->t.p2t->ic_flags=0;
	    }
	  }
	}
	if (tempi2->ic_opcode==EC_MUL &&
	    tempi2->t.p2t->ic_opcode==EC_SIZEOF) {
	  tempcc=tempi1->ic_class;
here1:
	  tempi2->t.p2t->ic_opcode=EC_IMM;
	  tempi2->t.p2t->ic_class=tempcc;
	  tempi_push->ic_class=tempcc;
	  if (tempcc->ptr_cnt) {
	    tempcc--;
	    if (tempcc->size==1)
	      goto here3;
	    tempi2->t.p2t->ic_data=tempcc->size;
	  } else {
	    here3:
	    if (tempi_push==tempi2)
	      tempi2->t.p2t->ic_data=1;
	    else {
	      tempi2->ic_opcode=EC_NOP;
	      tempi2->ic_flags=0;
	      tempi2->t.p2t->ic_opcode=EC_NOP;
	      tempi2->t.p2t->ic_flags=0;
	    }
	  }
	}
      }
    }
    if (cmp_result_cnt[tempi_push->ic_opcode]) {
      PrsPush(ps,tempi->ic_class2);
      PrsPush(ps,tempi_push);
    }
    if (tempi->ic_class) {
      if (tempi->ic_class->sub_type==IT_F64)
	tempi->ic_flags&=~ICF_R_TO_F64;
      else
	tempi->ic_flags&=~ICF_R_TO_INT;
      if (code>EC_END_EXP)
	last_with_class=tempi;
    }
    if (combined)
      tempi++;
    else
      tempi=CbGetIc1(&lx->cbh);
  }
  OptEnd(lx,ps);

  /*
  This is for determining type conversions
  for passing args to funs.
  */
  return last_with_class;
}

