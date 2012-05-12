/* There is no FPrintF type function and no
way to grow files. Therefore, use mem to
hold the file until you are done.  The [C:/LT/OSMain/Adam1a.HPZ,1361] Ltf
framework is convenient for this.
*/

#include "/LT/Demo/Shared/TreeDemoJoin"

U0 FPrintFDemo(U8 *output_filename=NULL)
{
  U64 fuf_flags=0;
  Ltf *l=LtfNew;
  GetFFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r");
  LtfPutS(l,"$TR-C+AL,\"\"$\n");
  LtfPutS(l,"$ID,+2$");
  TreeSub(l,FilesFind("/LT/*",fuf_flags));
  LtfPutS(l,"$ID,-2$");
  LtfRecalc(l);
  if (output_filename) {
    StrCpy(l->filename.name,output_filename);
    LtfWrite(l,FALSE);
  } else
    LtfWrite(l,TRUE);
  LtfDel(l);
}

FPrintFDemo;
