#include "OSINC.ASM"
#include "COMPINC.ASM"
	ALIGN	16
SYS_CS_START::
#include "OSMAIN.ASM"
#include "IRQHAND.ASM"
#include "MEMORY.ASM"
#include "GRMODE.ASM"
#include "SYSUTILS.ASM"
#include "SCHED.ASM"
#include "CHARIO.ASM"
#include "CRT.ASM"
#include "DEBUG.ASM"
#include "KEYBD.ASM"
#include "STRING.ASM"
#include "CALLS.ASM"
#include "LTF.ASM"
#include "CMD.ASM"
#include "LEX.ASM"
#include "MAP.ASM"
#include "PARSE.ASM"
#include "COMPILE.ASM"
#include "CODE.ASM"
#include "OPT.ASM"
NOLIST

FIL_ROOT_CPP::
INSERT "/HOPPY/KERNEL/CPP/ROOT.CPP"
	DB	0;

FIL_FAT_CPP::
INSERT "/HOPPY/KERNEL/CPP/FAT.CPP"
	DB	0;

FIL_SYSTEXT_CPP::
INSERT "/HOPPY/KERNEL/CPP/SYSTEXT.CPP"
	DB	0;

FIL_DISK1_CPP::
INSERT "/HOPPY/KERNEL/CPP/DISK1.CPP"
	DB	0;

FIL_STRINGS_CPP::
INSERT "/HOPPY/KERNEL/CPP/STRINGS.CPP"
	DB	0;

FIL_DISK2_CPP::
INSERT "/HOPPY/KERNEL/CPP/DISK2.CPP"
	DB	0;

	ALIGN	16,0
SYS_CS_SIZE::
	END
