U0 StreamPutS(U8 *st)
//Inss text into the stream of
//characters being compiled.  Called
//from withing #exe{} block.
{
  LexStruct *lx=Fs->last_lex;
  StreamBlkStruct *tempe=lx->last_stream_blk;
  if (tempe!=&lx->next_stream_blk)
    tempe->body=StrAdd(tempe->body,st);
  else
    PrintFErr("No exe{} blk\n");
}

U0 StreamPrintF(U8 *src,...)
//Inss text into the stream of
//characters being compiled.  Called
//from withing #exe{} block.
{
  U8 *buf=SPrintFJoin(NULL,src,argc,argv);
  StreamPutS(buf);
  Free(buf);
}

U8 *CmpStatement(LexStruct *lx,U64 *type)
{
  U64 *result=INVALID_PTR;
  U64 size,i,j,k;
  IntermediateCode *tempi;
  if (type) *type=IT_I64;
  Btr(&lx->flags,Lf_OPT_TRACE_PRESENT);
  if (lx->a_depth==2)
    CbPush(lx);
  CbOutPtrsInit(lx);
  CbIcAdd(lx,EC_START,0,0); //So we can scan back an instruction
  tempi=lx->cbh.cb_last_out_ptr;
  if (!PrsStatement(lx,NULL)) {
    if (tempi!=lx->cbh.cb_last_out_ptr) {
      CbIcAdd(lx,EC_RETURN_VAL2,0,NULL);
      CbIcAdd(lx,EC_RET,0,0);
      CbIcAdd(lx,EC_END,0,0);
      if (result=CbCompile(lx,NULL,&size,NULL,NULL,type)) {
	if (lx->flags&LF_STATIC_COMPILE) {
	  j=lx->a->ip;
	  k=(size+7)>>3;
	  for (i=0;i<k;i++)
	    AsmStoreCodeU64(lx,result[i]);
	  Free(result);
	  result=j;
	}
      }
    } else
      LinkedListDel(lx->cbh.cb_out_first);
  } else
    LinkedListDel(lx->cbh.cb_out_first);
  if (lx->a_depth==2)
    CbPop(lx);
  return result;
}


ExeStruct *CmpJoin(LexStruct *lx,FunStruct *locals,BoolI8 first_lex,BoolI8 asm_blk=FALSE,
  U8 *map_name=NULL,U8 mapfile_drv=0)
{
  AsmCtrlStruct *a,*old_a=lx->a;
  U64 i,j,l;
  ExeStruct *result;
  AsmCodeBlk *tempc;
  ExeImportExportStruct *tempe;
  BoolI8 okay=TRUE;
  SysHashTable *ht1=lx->hash_table_list,
	       *ht2=lx->define_hash_table,
	       *ht3=lx->glbl_hash_table,
	       *ht4=lx->local_hash_table,
	       *asm_hash_alias=MAllocIdentical(asm_hash);
  FunStruct    *old_var_list=lx->local_var_list;
  result=CAlloc(sizeof(ExeStruct));
  result->parent_exe=lx->exe_struct;
  lx->exe_struct=result;

  result->next_ie=result->last_ie=&result->next_ie;
  lx->a=a=CAlloc(sizeof(AsmCtrlStruct));
  lx->a_depth++;

  a->code=CAlloc(sizeof(AsmCodeBlk));
  a->max_align=1;

  lx->local_hash_table=HashTableNew(16);
  lx->glbl_hash_table =HashTableNew(2048);

  asm_hash_alias->next=lx->local_hash_table;
  lx->local_hash_table->next=lx->glbl_hash_table;

  if (lx->flags&LF_STATIC_COMPILE)
    lx->define_hash_table=lx->glbl_hash_table;
  else
    lx->glbl_hash_table->next=Fs->hash_table;
  lx->hash_table_list =asm_hash_alias;

  lx->local_var_list=locals; //C+ local vars
  try {
    if (first_lex)
      Lex(lx);
    if (lx->flags&LF_STATIC_COMPILE) {
      while (lx->token!=TK_EOF) {
	i=CmpStatement(lx,NULL);
	if (i!=INVALID_PTR) {
	  tempe=CAlloc(sizeof(ExeImportExportStruct));
	  tempe->type=EIE_MAIN;
	  tempe->ip=i;
	  QueIns(tempe,result->last_ie);
	}
	if (asm_blk)
	  break;
      }
    } else
      PrsStatement(lx,NULL);
    AsmGlblsResolve(lx,result);
  } catch {
    if (Fs->except_argc>0 && Fs->except_argv[0]==EXCEPT_COMPILER &&
      !asm_blk) {
      if (Fs->except_argc>1 && Fs->except_argv[1]) {
	"Compiler exception #%d\n",Fs->except_argv[1];
	LexPutLink(lx);
	LexPutLine(lx,lx->cur_lfn->line_start);
      }
      Fs->catch_except=TRUE;
    }
    okay=FALSE;
  }
  if (!okay) {
    if (lx->error_cnt<1)
      lx->error_cnt=1;
    lx->exe_struct=result->parent_exe;
    Free(result);
    LinkedListDel(a->code);
    result=NULL;
  } else {
    if (map_name && !cmp_do_not_write_to_dsk)
      HashRep(lx->glbl_hash_table,HTT_ALL,map_name,"-r-p",mapfile_drv);
    HashTableDel(lx->local_hash_table);
    HashTableDel(lx->glbl_hash_table);

    if (!a->num_code_U8s)
      result->code=NULL;
    else {
      if (lx->flags&LF_STATIC_COMPILE)
	result->code=MAlloc(a->num_code_U8s);
      else
	result->code=MAlloc(a->num_code_U8s,Fs->code_heap);
      result->code_U8s=a->num_code_U8s;
      tempc=a->code;
      j=0;
      l=a->num_code_U8s;
      while (tempc) {
	i=l;
	if (i>ASM_CODE_BLK_SIZE)
	  i=ASM_CODE_BLK_SIZE;
	MemCpy(result->code+j,tempc->body,i);
	j+=i;
	l-=i;
	tempc=tempc->next;
      }
    }
    LinkedListDel(a->code);
    result->abss=a->abss;
    result->heap_glbls=a->heap_glbls;
    result->max_align=a->max_align;
    lx->exe_struct=result->parent_exe;
  }
  lx->hash_table_list  =ht1,
  lx->define_hash_table=ht2;
  lx->glbl_hash_table  =ht3;
  lx->local_hash_table =ht4;
  lx->local_var_list=old_var_list;
  Free(asm_hash_alias);
  Free(a);
  lx->a=old_a;
  lx->a_depth--;
  return result;
}

ExeStruct *CmpBuf(U8 *buf,U8 *map_name=NULL,
  I64 *error_cnt=NULL, I64 *warning_cnt=NULL,U8 mapfile_drv=0)
{
  LexStruct *lx;
  ExeStruct *result=NULL;
 
  if (!is_first_compiler)
    "$FG,PURPLE$**** Not First Compiler ****$FG$\n";
 
  lx=LexNew(buf,LF_DONT_FREE_BUF,NULL);
  lx->flags|=LF_STATIC_COMPILE;
  QueIns(lx,Fs->last_lex);
  result=CmpJoin(lx,NULL,TRUE,FALSE,map_name,mapfile_drv);
  if (error_cnt) *error_cnt=lx->error_cnt;
  if (warning_cnt) *warning_cnt=lx->warning_cnt;
  QueRem(lx);
  if (result)
    LexDel(lx);
  return result;
}

U0 CmpFixUpJITAsm(LexStruct *lx,ExeStruct *ex,I64 ip2)
{
  I64 i;
  U0 *ptr;
  ExeAbsAddrStruct *tempa,*tempa1;
  ExeImportExportStruct *tempe,*tempe1;
  ExportSysSymHashEntry *tempess;
  FunStruct *tempf;
  GlblVarStruct *tempg;
  I64 *str=NULL;

  tempa=ex->abss;
  while (tempa) {
    tempa1=tempa->next;
    nobound_switch (tempa->type) {
      case EAA_ADD_U8:
	*(ex->code(U8 *)+tempa->ip)(U8 *)+=ip2;
	break;
      case EAA_SUB_U8:
	*(ex->code(U8 *)+tempa->ip)(U8 *)-=ip2;
	break;
      case EAA_ADD_U16:
	*(ex->code(U8 *)+tempa->ip)(U16 *)+=ip2;
	break;
      case EAA_SUB_U16:
	*(ex->code(U8 *)+tempa->ip)(U16 *)-=ip2;
	break;
      case EAA_ADD_U32:
	*(ex->code(U8 *)+tempa->ip)(U32 *)+=ip2;
	break;
      case EAA_SUB_U32:
	*(ex->code(U8 *)+tempa->ip)(U32 *)-=ip2;
	break;
      case EAA_ADD_U64:
	*(ex->code(U8 *)+tempa->ip)(U64 *)+=ip2;
	break;
      case EAA_SUB_U64:
	*(ex->code(U8 *)+tempa->ip)(U64 *)-=ip2;
	break;
    }
    Free(tempa);
    tempa=tempa1;
  }
  tempe=ex->next_ie;
  while (tempe!=&ex->next_ie) {
    tempe1=tempe->next;
    if (tempe->str) {
      Free(str);
      str=tempe->str;
    }
    if (tempe->type<=EIE_LIT_EXPORT) {
      tempess=CAlloc(sizeof(ExportSysSymHashEntry));
      tempess->str=str;
      str=NULL;
      tempess->type=HTT_EXPORT_SYS_SYM|HTF_LIT;
      if (tempe->type==EIE_LIT_EXPORT)
	tempess->val=tempe->ip;
      else
	tempess->val=tempe->ip+ip2;
      tempess->src_link=tempe->src_link;
      tempe->src_link=NULL;
      if (IsReplaceSyms)
	HashReplace(tempess,Fs->hash_table);
      else
	HashAdd(tempess,Fs->hash_table);
      SysSymImportsResolve(tempess->str);
    } else if (tempe->type<EIE_MAIN) {
      if (tempe->str && !(tempess=HashFind(str,Fs->hash_table,HTT_ALL-HTT_IMPORT_SYS_SYM)))
	"%S%s\n","ST_UNRESOLVED_REFERENCE",str;
      else {
	ptr=tempe->ip+ip2;
	if (tempess->type & HTT_FUN) {
	  tempf=tempess;
	  if (tempe->type & 1)
	    i=tempf->executable_addr;
	  else
	    i=tempf->executable_addr-ex->code;
	} else if (tempess->type & HTT_GLBL_VAR) {
	  tempg=tempess;
	  if (tempe->type & 1)
	    i=tempg->data_addr;
	  else
	    i=tempg->data_addr-ex->code;
	} else {
	  if (tempe->type & 1)
	    i=tempess->val;
	  else
	    i=tempess->val(U8 *)-ex->code;
	}
	if (tempe->type<=EIE_LIT_U8)
	  *ptr(U8 *)+=i;
	else if (tempe->type<=EIE_LIT_U16)
	  *ptr(U16 *)+=i;
	else if (tempe->type<=EIE_LIT_U32)
	  *ptr(U32 *)+=i;
	else if (tempe->type<=EIE_LIT_U64)
	  *ptr(U64 *)+=i;
      }
    }
    Free(tempe->src_link);
    Free(tempe);
    tempe=tempe1;
  }
  Free(str);
  if (!lx->a_depth && Bt(&lx->opts,OPTf_COMPILE_TRACE))
    Un(ex->code,ex->code_U8s,64);
  Free(ex);
}

U0 CmpFixUpStaticAsm(LexStruct *lx,ExeStruct *ex,I64 ip2)
{
  AsmCtrlStruct *a=lx->a;
  U0 *ptr;
  ExeAbsAddrStruct *tempa,*tempa1;
  ExeImportExportStruct *tempe,*tempe1;

  tempa=ex->abss;
  while (tempa) {
    tempa1=tempa->next;
    tempa->next=a->abss;
    nobound_switch (tempa->type) {
      case EAA_ADD_U8:
	*(ex->code(U8 *)+tempa->ip)(U8 *)+=ip2;
	break;
      case EAA_SUB_U8:
	*(ex->code(U8 *)+tempa->ip)(U8 *)-=ip2;
	break;
      case EAA_ADD_U16:
	*(ex->code(U8 *)+tempa->ip)(U16 *)+=ip2;
	break;
      case EAA_SUB_U16:
	*(ex->code(U8 *)+tempa->ip)(U16 *)-=ip2;
	break;
      case EAA_ADD_U32:
	*(ex->code(U8 *)+tempa->ip)(U32 *)+=ip2;
	break;
      case EAA_SUB_U32:
	*(ex->code(U8 *)+tempa->ip)(U32 *)-=ip2;
	break;
      case EAA_ADD_U64:
	*(ex->code(U8 *)+tempa->ip)(U64 *)+=ip2;
	break;
      case EAA_SUB_U64:
	*(ex->code(U8 *)+tempa->ip)(U64 *)-=ip2;
	break;
    }
    a->abss=tempa;
    tempa->ip+=ip2;
    tempa=tempa1;
  }

  tempe=ex->next_ie;
  while (tempe!=&ex->next_ie) {
    tempe1=tempe->next;
    QueRem(tempe);
    if (tempe->type<=EIE_LIT_EXPORT)
      tempe->ip+=ip2;
    else if (tempe->type<EIE_MAIN) {
      ptr=ex->code(U8 *)+tempe->ip;
      if (tempe->type<=EIE_LIT_U8)
	*ptr(U8 *)-=ip2;
      else if (tempe->type<=EIE_LIT_U16)
	*ptr(U16 *)-=ip2;
      else if (tempe->type<=EIE_LIT_U32)
	*ptr(U32 *)-=ip2;
      else if (tempe->type<=EIE_LIT_U64)
	*ptr(U64 *)-=ip2;
      tempe->ip+=ip2;
    }
    QueIns(tempe,ex->parent_exe->last_ie);
    tempe=tempe1;
  }
  Free(ex);
}

I64 Cmp(U8 *filename,U8 *map_name=NULL,U8 *out_name=NULL,U8 mapfile_drv=0)
{ //Compile file
  U8 *fbuf=NULL,*fbuf2=NULL,*fbuf3=NULL;
  ExeStruct *ex;
  U64 size=0;
  U8 *patch_table=MAlloc(0x20000),*ptr;
  BinFileHeaderStruct *bfh;
  U64 i,cnt,error_cnt=0,warning_cnt=0,code_U8s=0;
  ExeImportExportStruct *tempe,*tempe1;
  ExeAbsAddrStruct *tempa,*tempa1;
  ExeHeapGlblRefStruct *temphgr,*temphgr1;
  ExeHeapGlblStruct *temphg,*temphg1;

  fbuf=DftExt(filename,"SPZ");
  fbuf2=MSPrintF("#include \"%s\"",fbuf);
  if (map_name)
    fbuf3=DftExt(map_name,"MPZ");
  ex=CmpBuf(fbuf2,fbuf3,&error_cnt,&warning_cnt,mapfile_drv);

  if (ex) {
    code_U8s=ex->code_U8s;
    ptr=patch_table;
    *ptr++=EIE_MODULE_ALIGNMENT; //See [C:/LT/OSMain/Hash1a.CPZ,729] Load() and [C:/LT/Adam/BinRep.APZ,22] BinRep()
    *ptr(U32 *)++=ex->max_align;
    *ptr++=0;
    cnt=0;
    tempa=ex->abss;
    while (tempa) {
      if (!(tempa->type&1))
	cnt++;
      tempa=tempa->next;
    }
    if (cnt) {
      *ptr++=EIE_ABS_ADDR;
      *ptr(U32 *)++=cnt;
      *ptr++=0;
      tempa=ex->abss;
      while (tempa) {
	tempa1=tempa->next;
	if (!(tempa->type&1))
	  *ptr(U32 *)++ =tempa->ip;
	Free(tempa);
	tempa=tempa1;
      }
    }
    temphg=ex->heap_glbls;
    while (temphg) {
      temphg1=temphg->next;
      cnt=0;
      temphgr=temphg->references;
      while (temphgr) {
	cnt++;
	temphgr=temphgr->next;
      }
      if (cnt) {
	*ptr++=EIE_HEAP_GLBL;
	*ptr(U32 *)++=cnt;
	if (temphg->str) {
	  i=StrLen(temphg->str);
	  MemCpy(ptr,temphg->str,i+1);
	  Free(temphg->str);
	  ptr+=i+1;
	} else
	  *ptr++=0;
	*ptr(U64 *)++=temphg->size;
	temphgr=temphg->references;
	while (temphgr) {
	  temphgr1=temphgr->next;
	  *ptr(U32 *)++=temphgr->ip;
	  Free(temphgr);
	  temphgr=temphgr1;
	}
      }
      Free(temphg);
      temphg=temphg1;
    }

//Do exports first
    tempe=ex->next_ie;
    while (tempe!=&ex->next_ie) {
      tempe1=tempe->next;
      if (tempe->type<=EIE_LIT_EXPORT) {
	QueRem(tempe);
	*ptr++=tempe->type;
	*ptr(U32 *)++=tempe->ip;
	if (tempe->str) {
	  i=StrLen(tempe->str);
	  MemCpy(ptr,tempe->str,i+1);
	  Free(tempe->str);
	  ptr+=i+1;
	} else
	  *ptr++=0;
	Free(tempe->src_link);
	Free(tempe);
      }
      tempe=tempe1;
    }

//Do imports second
    tempe=ex->next_ie;
    while (tempe!=&ex->next_ie) {
      tempe1=tempe->next;
      QueRem(tempe);
      *ptr++=tempe->type;
      *ptr(U32 *)++=tempe->ip;
      if (tempe->str) {
	i=StrLen(tempe->str);
	MemCpy(ptr,tempe->str,i+1);
	Free(tempe->str);
	ptr+=i+1;
      } else
	*ptr++=0;
      Free(tempe->src_link);
      Free(tempe);
      tempe=tempe1;
    }

    *ptr++=EIE_END;
    MemSet(ptr,0,16);
    i=ptr-patch_table;
    //[::/LT/OSMain/OSStart.CPZ,LT_XSUM] LT_XSUM needs 16 ALIGN
    size=(sizeof(BinFileHeaderStruct)+code_U8s+i+15)&-16;
    bfh=MAlloc(size);
    bfh->jmp=0xEB+256*(sizeof(BinFileHeaderStruct)-2);
#assert sizeof(BinFileHeaderStruct)-2<=MAX_I8
    bfh->type=0;
    bfh->loader_patch_table_offset=sizeof(BinFileHeaderStruct)+code_U8s;
    bfh->file_size=size;
    bfh->xsum=LT_XSUM;
    MemCpy(bfh(U8 *)+sizeof(BinFileHeaderStruct),ex->code,code_U8s);
    MemCpy(bfh(U8 *)+sizeof(BinFileHeaderStruct)+code_U8s,patch_table,
      size-code_U8s-sizeof(BinFileHeaderStruct));
    bfh->xsum=ArcXSum(bfh,size);
    Free(fbuf2);
    if (out_name)
      fbuf2=DftExt(out_name,"BIZ");
    else
      fbuf2=ChgExt(fbuf,"BIZ");
    if (!cmp_do_not_write_to_dsk)
      FileWrite(fbuf2,bfh,size);
    Free(bfh);
    Free(ex->code);
    Free(ex);
  }
  Free(patch_table);
  Free(fbuf);
  Free(fbuf2);
  Free(fbuf3);
  PrintF(Define("ST_ERRORS_WARNINGS"),error_cnt,warning_cnt,code_U8s,size);
  return error_cnt;
}

I64 ExePutS(U8 *buf,U8 *filename=NULL,BoolI8 just_load=FALSE)
{
  I64 result;
  BoolI8 okay=TRUE;
  LexStruct *lx;
  if (!filename)
    filename=sys_temp_filename;
  lx=LexNew(buf,LF_DONT_FREE_BUF,FileNameAbs(filename));
  if (just_load)
    lx->flags|=LF_JUST_LOAD;
  if (Fs->last_lex!=&Fs->next_lex)
    lx->opts=Fs->last_lex->opts;
  QueIns(lx,Fs->last_lex);
  try {
    Lex(lx);
    result=ExeCmdLine(lx);
  } catch {
    if (Fs->except_argc>0) {
      if (Fs->except_argv[0]==EXCEPT_COMPILER ||
      Fs->except_argv[0]==EXCEPT_BREAK) {
	Fs->catch_except=TRUE;
	okay=FALSE;
	result=0;
      }
    }
  }
  QueRem(lx);
  if (okay)
    LexDel(lx); //TODO: can crash
  return result;
}

I64 ExePrintF(U8 *fmt,...)
{
  I64 result;
  U8 *buf=SPrintFJoin(NULL,fmt,argc,argv);
  result=ExePutS(buf);
  Free(buf);
  return result;
}

I64 ExeFile(U8 *name,BoolI8 just_load=FALSE)
{
  I64 result;
  U8 *name2=DftExt(name,"CPZ");
  U8 *st=MSPrintF("#include \"%s\";",name2);
  result=ExePutS(st,name,just_load);
  Free(st);
  Free(name2);
  return result;
}

I64 ExePutS2(U8 *buf,U8 *filename=NULL,BoolI8 just_load=FALSE)
{ //with REPLACE_SYMS
  I64 result;
  BoolI8 okay=TRUE;
  LexStruct *lx;
  if (!filename)
    filename=sys_temp_filename;
  lx=LexNew(buf,LF_DONT_FREE_BUF,FileNameAbs(filename));
  if (just_load)
    lx->flags|=LF_JUST_LOAD;
  if (Fs->last_lex!=&Fs->next_lex)
    lx->opts=Fs->last_lex->opts|1<<OPTf_REPLACE_SYMS;
  QueIns(lx,Fs->last_lex);
  try {
    Lex(lx);
    result=ExeCmdLine(lx);
  } catch {
    if (Fs->except_argc>0) {
      if (Fs->except_argv[0]==EXCEPT_COMPILER ||
      Fs->except_argv[0]==EXCEPT_BREAK) {
	Fs->catch_except=TRUE;
	okay=FALSE;
	result=0;
      }
    }
  }
  QueRem(lx);
  if (okay)
    LexDel(lx); //TODO: can crash
  return result;
}

I64 ExePrintF2(U8 *fmt,...)
{ //with REPLACE_SYMS
  I64 result;
  U8 *buf=SPrintFJoin(NULL,fmt,argc,argv);
  result=ExePutS2(buf);
  Free(buf);
  return result;
}

I64 ExeFile2(U8 *name,BoolI8 just_load=FALSE)
{ //with REPLACE_SYMS
  I64 result;
  U8 *name2=DftExt(name,"CPZ");
  U8 *st=MSPrintF("#include \"%s\";",name2);
  result=ExePutS2(st,name,just_load);
  Free(st);
  Free(name2);
  return result;
}

I64 ExePutS3(U8 *buf,U8 *filename=NULL,BoolI8 just_load=FALSE)
{ //throws exceptions
  I64 result;
  LexStruct *lx;
  if (!filename)
    filename=sys_temp_filename;
  lx=LexNew(buf,LF_DONT_FREE_BUF,FileNameAbs(filename));
  if (just_load)
    lx->flags|=LF_JUST_LOAD;
  if (Fs->last_lex!=&Fs->next_lex)
    lx->opts=Fs->last_lex->opts;
  QueIns(lx,Fs->last_lex);
  Lex(lx);
  result=ExeCmdLine(lx);
  QueRem(lx);
  LexDel(lx);
  return result;
}

I64 ExePrintF3(U8 *fmt,...)
{ //throws exceptions
  I64 result;
  U8 *buf=SPrintFJoin(NULL,fmt,argc,argv);
  result=ExePutS3(buf);
  Free(buf);
  return result;
}


I64 ExeFile3(U8 *name,BoolI8 just_load=FALSE)
{ //throws exceptions
  I64 result;
  U8 *name2=DftExt(name,"CPZ");
  U8 *st=MSPrintF("#include \"%s\";",name2);
  result=ExePutS3(st,name,just_load);
  Free(st);
  Free(name2);
  return result;
}

U0 CmpInit()
{
  if (!sys_num_installed_compilers++) {
    is_first_compiler=TRUE;
    fp_prs_stream_blk=&PrsStreamBlk;
  } else
    is_first_compiler=FALSE;
  compiled_lines=0;
  compiled_funs=0;
  compiled_bytes=0;
  cmp_do_not_write_to_dsk=FALSE;
  CmpLoadDefines;
  CmpFillTables;
  MemSet(&opt_nop,0,sizeof(IntermediateCode));
  opt_nop.ic_class=sys_internal_types[IT_I64];

  opt_nop.ic_opcode=EC_NOP;
  AsmHashLoad(is_first_compiler);
  UnasmHashLoad;
}

CmpInit;
