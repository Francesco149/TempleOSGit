/* ::/LT/Demo/OldSchool/Demo4/ObjDemo.SPZ */

// imports all syms from the Adam Task's hash table.
#include "::/LT/OSMain/StaticAdam.HPZ"

import U0 Routine1(U8 *st);
import U0 Routine2(U8 *st);
LTimport _MY_BEEP U0 MyBeep();

U0 Routine3(U8 *st)
{
  PrintF("Routine3:%s\n",st);
}


//Any routine with var parameters
//automatically has vars argc and argv[]
//whether it is main or not.
I64 ObjDemo(...)
{
  I64 i;
  MyBeep();
  if (argc>0) {
    for (i=0;i<argc;i++)
      PrintF("Arg %d:%08X\n",i,argv[i]);
    Routine1(argv[0]);
    if (argc>1) {
      Routine2(argv[1]);
      if (argc>2)
	Routine3(argv[2]);
    }
  } else
    PutS("Pass string parameters for args 1 and 2.\n");
  return argc;
}

