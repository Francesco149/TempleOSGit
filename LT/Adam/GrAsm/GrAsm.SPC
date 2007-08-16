#include "/LT/OSMain/Static.HPZ"
#exe {OptOn(OPTf_IGNORE_OVERFLOW);}
asm {
	USE64
IMPORT	SYS_TEXTBASE;
GRASM_START::
#include "Gr1"
GRASM_END::
}
