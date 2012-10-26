/*
Press <CTRL-ALT-C> to break.

Rerun Program

Press <CTRL-ALT-D> to enter Dbg.
> [C:/SparrowOS/OSMain/Dbg1a.CPP.Z,332] Dr;	 //Dump Registers
> [C:/SparrowOS/Adam/Dbg2a.CPP.Z,333] BptsR;  //Clear break-points.
> [C:/SparrowOS/Adam/Dbg2a.CPP.Z,352] Go2;	 //Cont program


Press <CTRL-ALT-D> to enter Dbg.
> [C:/SparrowOS/Adam/Dbg2a.CPP.Z,241] Uf("DbgDemo");
> [C:/SparrowOS/Compiler/Unassembler.CPP.Z,851] U(RIP);

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

