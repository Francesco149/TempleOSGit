//This is executed by Adam at boot.
//See [::/SparrowOS/OSMain/OSEnd.CPP.Z,2] Adam1 Start-up
extern U64i SysEcho(U64i val);
SysEcho(0);  //0=echo OFF   1=echo ON
#include "/SparrowOS/OSMain/BootDefs.HPP.Z"
#include "/SparrowOS/OSMain/Adam1a.HPP.Z"
#include "/SparrowOS/Compiler/CmpDefs.HPP.Z"
#include "/SparrowOS/OSMain/Adam1b.HPP.Z"
#include "/SparrowOS/OSMain/Adam1c.HPP.Z"
#include "/SparrowOS/Compiler/Compiler.HPP.Z"
Cd("/SparrowOS/Adam");;
#include "Adam2.CPP.Z"
