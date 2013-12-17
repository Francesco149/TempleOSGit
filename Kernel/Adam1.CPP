//This is executed by Adam at boot.
//See [C:/Kernel/KEnd.CPP.Z,2] Adam1 Start-up
extern I8i OptEqu(I64i num,I8i val);
OptEqu(0,0); //(0,0)=EchoOff   (0,1)=EchoOn
#include "/Kernel/BootDefs.HPP"
#include "/Kernel/Adam1a.HPP"
#include "/Compiler/CmpDefs.HPP"
#include "/Kernel/Adam1b.HPP"
#include "/Kernel/Adam1c.HPP"
#include "/Compiler/Cmp.HPP"
Cd("/Adam");;
#include "Adam2"
