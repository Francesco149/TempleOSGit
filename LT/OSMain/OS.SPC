//You should compile this by calling
//[C:/LT/Adam/Boot/InsBoot.APZ,185] InstallBoot(), not doing it directly.
//See [::/LT/Adam/Boot/InsBoot.APZ,Cmp("OS"] Cmp("OS").

#exe {
OptOn(OPTf_PAREN_WARN);
#include "OSCfg"
OSMainCfg *osmain_cfg;
osmain_cfg=OSMainCfgNew;
};

#include "BootDefs.HPZ"
#include "Adam1a.HPZ"
asm {
#include "OSAsmDefs.HPZ"
}
#include "OSStartUp"
#include "SysUtils"
#include "/LT/Compiler/CmpDefs.HPZ"
#include "Adam1b.HPZ"
#include "OSExts"
#include "OSGlbls"
#include "Scheduler"
#include "Font8x8"
#include "Memory"
#include "Interrupts"
#include "FIFO"
#include "Compress"
#include "Hash1a"
#include "Str1a"
#include "Define1a"
#include "Display"
#include "Misc1a"
#include "Dbg1a"
#include "Except"
#include "KbdMouse/InputPointer"
#include "KbdMouse/Keyboard"
#include "KbdMouse/Mouse"
#include "KbdMouse/Message"

#include "QSort"
#include "Task1a"
#include "TaskCmd"
#include "Date"
#include "USB1a"

#exe {Cd("Dsk");};
#include "DskStr"
#include "DskCache"
#include "ATADef.HPZ"
#include "DskATA"
#include "Dsk1a"
#include "DskAddDev"
#include "DskDir"
#include "FileSysISO1"
#include "FileSysISO3"
#include "FileSysLT"
#include "FileSysFAT"
#include "Dsk1b"
#include "DskLTFile"
#include "Dsk1c"

#exe {Cd("..");};
#include "MultiProc"
#include "BIOS32"
#include "OSMain"

#exe {OSMainCfgDel(osmain_cfg);};
