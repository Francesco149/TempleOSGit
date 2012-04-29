BoolI64 OptEqu(U64 num,BoolI8 val) {
  return BEqu(&Fs->last_lex->opts,num,val);
}

//Turn on compiler option
BoolI64 OptOn(U64 num) {
  return OptEqu(num,ON);
}

//Turn off compiler option
BoolI64 OptOff(U64 num) {
  return OptEqu(num,OFF);
}

//Get state of compiler option
BoolI64 GetOpt(U64 num) {
  return Bt(&Fs->last_lex->opts,num);
}

BoolI64 Externs2Imports(BoolI8 val) {
  return OptEqu(OPTf_EXTERNS_TO_IMPORTS,val);
}

//Private syms are accessible but
//don't show-up in reports
BoolI64 KeepPrivate(BoolI8 val) {
  return OptEqu(OPTf_KEEP_PRIVATE,val);
}

//Displays intermediate code as it is being added
BoolI64 PTrace(BoolI8 val) {
  return OptEqu(OPTf_PARSE_TRACE,val);
}

//Displays intermediate code after passes through optimization
BoolI64 OTrace(BoolI8 val) {
  return OptEqu(OPTf_OPT_TRACE,val);
}

//Ctrls which optimizer passes are displayed
U64 OPassTraceMask(U64 i=0b1001111101) {
  U64 old=Fs->last_lex->pass_trace_mask;
  Fs->last_lex->pass_trace_mask=i;
  return old;
}


//Displays assembly code output
BoolI64 CTrace(BoolI8 val) {
  return OptEqu(OPTf_COMPILE_TRACE,val);
}

//Displays lexical analyser's output
BoolI64 LTrace(BoolI8 val) {
  return OptEqu(OPTf_LEX_TRACE,val);
}

BoolI64 NoRegVar(BoolI8 val) {
  return OptEqu(OPTf_NO_REG_VAR,val);
}

//Displays text as it is being compiled
BoolI64 Echo(BoolI8 val) {
  return OptEqu(OPTf_ECHO,val);
}

BoolI64 IsReplaceSyms() {
  return GetOpt(OPTf_REPLACE_SYMS);
}

BoolI64 SysEcho(BoolI8 val) {
  return OptEqu(OPTf_ECHO,val);
}

U0 ImportHashTable(SysHashTable *src)
{ //called from #exe {} in static modules
// See [::/LT/OSMain/StaticAdam.HPZ] ::/LT/OSMain/StaticAdam.HPZ.
  LexStruct *lx=Fs->last_lex;
  if (lx==&Fs->next_lex) {
    PutErr;
    "Not Compiling\n";
  }
  if (!(lx->flags&LF_EXE_BLK))
    PrsExcept(lx,"ST_IMPORT_HASH_TABLE_NOT_ALLOWED");
  while (src) {
    if (lx->stream_parent_glbl_hash_table)
      HashTableImport(src,lx->stream_parent_glbl_hash_table);
    else
      HashTableImport(src,lx->glbl_hash_table);
    src=src->next;
  }
}

I64 IncludeDepth()
{
  LexStruct *lx=Fs->last_lex;
  return lx->cur_lfn-&lx->lfns;
}

BoolI64 CmdLineExit()
{
  LexStruct *lx=Fs->last_lex;
  return Btr(&lx->flags,Lf_PROMPT);
}

