/*
Press <CTRL-ALT-c> to break.

Rerun Program

Press <CTRL-ALT-d> to enter Dbg.
> [C:/Kernel/KDbg.CPP.Z,328] Dr;	//Dump Regs
> [C:/Kernel/KDbg.CPP.Z,469] G2;	//Cont program

Press <CTRL-ALT-d> to enter Dbg.
> [C:/Adam/ADbg.CPP.Z,254] Uf("DbgDemo");
> [C:/Compiler/UAsm.CPP.Z,645] U(_RIP);

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

