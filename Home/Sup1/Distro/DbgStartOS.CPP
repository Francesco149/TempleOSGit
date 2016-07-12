#help_index "Compiler/Directive"
public extern I8i Option(I64i num,I8i val);
Option(0,0); //(0,0)=EchoOff   (0,1)=EchoOn
#include "/Kernel/KernelA.HPP"
#include "/Compiler/CompilerA.HPP"
#include "/Kernel/KernelB.HPP"
#include "/Kernel/KernelC.HPP"
#include "/Compiler/CompilerB.HPP"

Option(OPTf_WARN_PAREN,ON);
Option(OPTf_WARN_DUP_TYPES,ON);
HashTablePurge(adam_task->hash_table);

#help_index "Misc"
F64 os_version=4.090;//Operating system version.

#include "Adam/MakeAdam"

Dbg("Type 'G;'");
