#exe {OptOn(OPTf_WARN_PAREN);};
#help_index "Task/Login"

//Use Dbg("Type 'DbgExit'"); to debug before window mgr is running.

public F64 os_version=8.04;

#include "AdamExt.HPZ"

#include "Math2a"
#include "Training"
#include "Utils2a"
#include "MathOde"

Cd("::/LT/Adam/Gr");;
#include "Gr"

Cd("::/LT/Adam/Snd");;
#include "Snd"

Cd("::/LT/Adam/Dsk");;
#include "Dsk2"

Cd("::/LT/Adam");;
#include "Menu"
#include "Window"
#include "Dbg2a"

Cd("::/LT/Adam/Ltf");;
#include "Ltf"
LBts(&sys_run_level,RLf_LTF);

Cd("::/LT/Adam/Ctrls");;
#include "Ctrls"

Cd("::/LT/Adam");;
#include "AutoFile2a"
#include "LogIn"

Cd("::/LT/Adam/WordStat");;
#include "WordStat"

Cd("::/LT/Adam");;
#include "Profiler"
#include "Hash2a"
#include "TaskSettings"
#include "DevInfo"
#include "DocDefines"

//Dbg("Type 'DbgExit'");

UseTermLtf(NULL);

adam_task->win_top=2;
adam_task->win_bottom=10;
sys_winmgr_task=Spawn(&WinMgrTask,NULL,"Window Mgr");
LBts(&Fs->display_flags,DISPLAYf_CHILDREN_NOT_ON_TOP);
LBts(&Fs->display_flags,DISPLAYf_SHOW);
SysLogin;
LBts(&sys_run_level,RLf_LOGIN);
RegistryInit;
LBts(&sys_run_level,RLf_REGISTRY);

Cd("::/LT/Adam");;
#include "WallPaper"
#exe {OptOff(OPTf_WARN_PAREN);};
LBts(&sys_run_level,RLf_ADAM2);

Cd("HOME");;
#include "Adam3"
