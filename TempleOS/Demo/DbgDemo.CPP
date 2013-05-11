/*
Press <CTRL-ALT-C> to break.

Rerun Program

Press <CTRL-ALT-D> to enter Dbg.
> [C:/TempleOS/Kernel/Dbg1a.CPP.Z,378] Dr;	//Dump Registers
> [C:/TempleOS/Kernel/Dbg1a.CPP.Z,527] G2;	//Cont program


Press <CTRL-ALT-D> to enter Dbg.
> [C:/TempleOS/Adam/Dbg2a.CPP.Z,236] Uf("DbgDemo");
> [C:/TempleOS/Compiler/Unassembler.CPP.Z,866] U(_RIP);

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

