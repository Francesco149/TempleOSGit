#help_index "Compiler/Directive"
public extern I8i Option(I64i num,I8i val);
Option(0,0); //(0,0)=EchoOff   (0,1)=EchoOn
#include "/Kernel/KernelA.HH"
#include "/Compiler/CompilerA.HH"
#include "/Kernel/KernelB.HH"
#include "/Kernel/KernelC.HH"
#include "/Compiler/CompilerB.HH"

Option(OPTf_WARN_PAREN,ON);
Option(OPTf_WARN_DUP_TYPES,ON);
HashTablePurge(adam_task->hash_table);
#include "Adam/MakeAdam"

Dbg("Welcome to the Debug Distro");
