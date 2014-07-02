/*
Press <CTRL-ALT-c> to break.

Rerun Program

Press <CTRL-ALT-d> to enter Dbg.
> [C:/Kernel/Dbg1a.CPP.Z,326] Dr;	//Dump Regs
> [C:/Kernel/Dbg1a.CPP.Z,474] G2;	//Cont program

Press <CTRL-ALT-d> to enter Dbg.
> [C:/Adam/Dbg2a.CPP.Z,249] Uf("DbgDemo");
> [C:/Compiler/Unassembler.CPP.Z,643] U(_RIP);

See [Debugging,1] Debugging
*/

U0 DbgDemo()
{
  I64 i=0;
  while (TRUE)
    if (!(i++%2000000))
      ".";
}

DbgDemo;

