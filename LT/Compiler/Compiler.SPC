#include "/LT/OSMain/Static.HPZ"
#include "CmpDefs.HPZ"
asm {
	USE64
COMPILER_START::
#include "CmpAsm.ASZ"
}
#exe {OptOn(OPTf_IGNORE_OVERFLOW);}
#include "CmpDefs2.HPZ"
#include "CmpGlbls.CPZ"
#include "Lex.CPZ"
#include "CodeGen.CPZ"
#include "SPrintF2.CPZ"
#include "AsmMain.CPZ"
#include "ExpParser.CPZ"
#include "StmtParser.CPZ"
#include "Code.CPZ"
#include "OptPass1.CPZ"
#include "OptPass2.CPZ"
#include "OptPass3.CPZ"
#include "OptPass4.CPZ"
#include "OptPass5.CPZ"
#include "OptPass6.CPZ"
#include "CmpInit.CPZ"
#include "CmpMain.CPZ"
asm {
COMPILER_END::
}
