/*
Press <CTRL-ALT-C> to break.

Rerun Program

Press <CTRL-ALT-D> to enter Dbg.
> [C:/Kernel/Dbg1a.CPP.Z,337] Dr;	//Dump Regs
> [C:/Kernel/Dbg1a.CPP.Z,486] G2;	//Cont program


Press <CTRL-ALT-D> to enter Dbg.
> [C:/Adam/Dbg2a.CPP.Z,236] Uf("DbgDemo");
> [C:/Compiler/Unassembler.CPP.Z,647] U(_RIP);

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

