//This is executed by the Adam task at boot.
//See [C:/Kernel/KEnd.CPP.Z,2] Adam Start-up.

//You do not place this file in /Home. (It would be ignored.)
//but look at the bottom of this file to see
//files you do customize.

#help_index "Compiler/Directive"
public extern I8i Option(I64i num,I8i val);
Option(0,0); //(0,0)=EchoOff   (0,1)=EchoOn

#include "/Kernel/KernelA.HPP"
#include "/Compiler/CompilerA.HPP"
#include "/Kernel/KernelB.HPP"
#include "/Kernel/KernelC.HPP"
#include "/Compiler/CompilerB.HPP"

Cd("/Adam");;
Option(OPTf_WARN_PAREN,ON);
Option(OPTf_WARN_DUP_TYPES,ON);

HashTablePurge(adam_task->hash_table);

#help_index "Misc"

//Use Dbg("Type 'G;'"); to debug before window mgr is running.

public F64 os_version=3.09;//Operating system version.

#include "AExts.HPP"
#include "AMath"
#include "Training"
#include "AUtils"
#include "AMathODE"
Cd("::/Adam/Gr");;
#include "Gr"

Cd("::/Adam/Snd");;
#include "Snd"

Cd("::/Adam/Dsk");;
#include "ADsk"

Cd("::/Adam");;
#include "Menu"
#include "WinA"
#include "WinB"
#include "ADbg"

#include "::/Adam/God/GodExt"

Cd("::/Adam/DolDoc");;
#include "Doc"
LBts(&sys_run_level,RLf_DOC);

Cd("::/Adam/Ctrls");;
#include "Ctrls"

Cd("::/Adam");;
#include "AutoFile"
#include "ARegistry"

Cd("::/Adam/AutoComplete");;
#include "AutoComplete"

Cd("::/Adam/God");;
#include "God"

Cd("::/Adam");;
#include "AHash"
#include "TaskSettings"
#include "DevInfo"
#include "ADefine"

//Dbg("Type 'G;'");
DocTermNew;
WinVert(2,10);

sys_winmgr_task=Spawn(&WinMgrTask,NULL,"Window Mgr");
Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_BORDER
	-WIF_SELF_GRAB_SCROLL-WIF_SELF_CTRLS;
LBts(&Fs->display_flags,DISPLAYf_CHILDREN_NOT_ON_TOP);
LBts(&Fs->display_flags,DISPLAYf_SHOW);
RegInit;
LBts(&sys_run_level,RLf_REGISTRY);

Cd("::/Adam");;
#include "WallPaper"

if (DrvIsWritable)
  MkDir("/Temp"); //Good to have a Temp

Option(OPTf_WARN_PAREN,OFF);
Option(OPTf_WARN_DUP_TYPES,OFF);
LBts(&sys_run_level,RLf_HOME);

#help_index ""
Cd("::/Home");;

//If these are not present in /Home, it uses the version in the root dir.
//You should make your own version of these files in you /Home directory.
#include "::/Home/AdamLocalize"
#include "::/Home/AdamPkgs"
#include "::/Home/AdamKeyPlugIns"
#include "::/Home/AdamWrappers"
#include "::/Home/AdamSys"

//After this file, the Adam task enters [Task/Srv,1] server mode.
