import BoolI64 PutSrcLink(U0 *add,U64 cnt=1,U8 *buf=NULL);
import CDoc *DocRead(U8 *name,U64 flags=0);
import U0 DocLoad(CDoc *doc,U8 *src2,U64 size);
import U0 DocWrite(CDoc *doc,BoolI8 prompt=FALSE);
import CDoc *DocNew(CTask *task=NULL);
import U0 DocRecalc(CDoc *doc,BoolI8 has_cursor=FALSE,I64 recalc=DOC_RECALC_ALL);
import U0 DocDel(CDoc *doc);
import U0 DocRst(CDoc *doc,BoolI8 is_old);
import CDocEntry *DocPutLine(CDocEntry *doc_e);
import U64 DocSize(CDoc *doc);
import U8 *Doc2PlainText(CDoc *doc,CDocEntry *doc_e);
import U0 HashRep(CHashTable *h=NULL,
  U64 mask=HTG_ALL,U8 *outname=NULL,U8 *flags=NULL,U8 mapfile_drv=0,U8 *index=NULL,
  CDoc *_doc=NULL);

extern I64 Expression(CLex *lx);
extern BoolI64 GetExpression(CLex *lx,U8 **_ex);
extern U64 PrsUnaryTerm(CLex *lx,CPrsStk *ps,CMemberList **_local_var,CArrayDim **_tempad,
    I64 *max_prec,I64 *unary_pre_prec,I64 *paren_prec);
extern U64 PrsUnaryModifier(CLex *lx,CPrsStk *ps,CMemberList **_local_var,CArrayDim **_tempad,I64 *unary_post_prec);
extern U0 PrsDereference(CLex *lx);
extern U0 LexExcept(CLex *lx,U8 *name=NULL,CHashFun *fun_class=NULL);
extern CExe *CmpJoin(CLex *lx,CHashFun *locals,BoolI8 first_lex,BoolI8 asm_blk=FALSE,U8 *map_name=NULL,U8 mapfile_drv=0);
extern U0 CmpFixUpJITAsm(CLex *lx,CExe *ex,I64 ip2);
extern U0 CmpFixUpStaticAsm(CLex *lx,CExe *ex,I64 ip2);
extern CMemberList *MemberListNew(I64 register);
extern CHashClass *PrsClass(CLex *lx,BoolI8 is_union,U64 cps_flags,BoolI8 is_extern);
extern CHashClass *PrsType(CLex *lx,CHashClass **tempc1,
  I64 *_mode,CMemberList *tempm,U8 **_ident,CHashFun **_fun_ptr,
  CHashExport **_tempess,CArrayDim *tempad,U64 cps_flags);
extern U0 PrsVarInit2(CLex *lx,U8 **_dst,CHashClass *tempc,CArrayDim *tempad,U8 *data_addr_ip,U8 **_base,BoolI8 data_heap,I64 pass);
extern CHashFun *PrsFunJoin(CLex *lx,
	CHashClass *temp_return,U8 *name,U64 cps_flags);
extern BoolI64 PrsStatement(CLex *lx,CHashFun *locals,I64 try_cnt=0,
  CCodeBlkMisc *lb_break=NULL,BoolI8 saved_prs_semicolon=TRUE,BoolI8 if_else=FALSE);
extern CIntermediateCode *OptLag1(CIntermediateCode *tempi);
extern CIntermediateCode *OptPass012(CLex *lx,CHashFun *fun_class,BoolI8 combined);
extern U0 OptPass3(CLex *lx,CHashFun *fun_class,COptReg *register_offsets);
extern U0 OptPass4(CLex *lx,COptReg *register_offsets,U64 *_type);
extern U0 OptPass5(CLex *lx);
extern U0 OptPass6(CLex *lx,CHashFun *fun_class);
extern U64 OptPass789A(CLex *lx,CHashFun *fun_class,COptReg *register_offsets,U8 *buf,CDbgInfo **_dbg);
extern I64 PrsKeyWord(CLex *lx);
extern U64 HashEntrySize2(CHashSrcSym *temph);
extern U0 LexPutWarn(CLex *lx,U8 *name=NULL,CHashFun *fun_class=NULL);
extern CHashSrcSym *HashEntryImportCopy(CHashSrcSym *temph);
extern U0 HashTableImport(CHashTable *src,CHashTable *dst);

_extern CMP_TEMPLATES U32 cmp_templates[1];
_extern CMP_TEMPLATES_DONT_POP U32 cmp_templates_dont_pop[1];
_extern CMP_TEMPLATES_DONT_PUSH U32 cmp_templates_dont_push[1];
_extern CMP_TEMPLATES_DONT_PUSH_POP U32 cmp_templates_dont_push_pop[1];
_extern CMP_TEMPLATES_DONT_PUSH2 U32 cmp_templates_dont_push2[1];


