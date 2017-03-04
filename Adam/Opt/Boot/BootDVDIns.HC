//See [C:/Doc/Install.DD.Z,1] Install Documentation or [C:/Doc/Upgrade.DD.Z,1] Upgrading Documentation.
//Study my account examples: [C:/Demo/AcctExample/TOS/TOSCfg.HC,1] Cfg Strs, [C:/Demo/AcctExample/TOS/TOSDistro.HC,1] Update Funs

#include "BootDVD"
#include "DskISORedSea"
#include "DskISO9660"

#help_index "Install"

#define KERNEL_BIN_C	"Kernel.BIN.C"
#define BOOT_DIR	"/0000Boot"
#define BOOT_DIR_KERNEL_BIN_C	BOOT_DIR "/0000" KERNEL_BIN_C

U0 MakeAll()
{
  if (Cmp("/Compiler/Compiler","Compiler",,':'))
    throw;
  if (Cmp("/Kernel/Kernel","Kernel",,':'))
    throw;
}

public U0 BootDVDIns(U8 drv_let=0)
{//See [C:/Misc/DoDistro.HC.Z,1].
  try {
    if (!Drv(drv_let))
      throw;
    MakeAll;
    Move("/Kernel/Kernel.BIN.Z",BOOT_DIR_KERNEL_BIN_C);
  } catch {
    PutExcept;
    Beep;
  }
}
