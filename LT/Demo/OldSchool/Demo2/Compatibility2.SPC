/*
This file has a ".SPP" extension because
it is a "statically" compiled program
which you must compile ahead of time
as opposed to just-in-time.

To make it more standard, I chose not
to give it a file name extension
ending in "Z", so that it will not be
stored compressed.  (Use ".SPZ", if
you want a statically compiled,
compressed source file.)

Right-click the directory entry and
select "COMPILE" from the menu.  This
will produce a binary file with a
".BIZ" extension.  This is the
executable.  You can use
[C:/LT/Compiler/CmpMain.CPZ,380] Cmp() in scripts or whatever.

To run this program, right-click the
directory entry for the binary and
select "LOAD".  Or, you can use [C:/LT/OSMain/Hash1a.CPZ,733] Load()
in scripts or whatever.

If you place statements to start
execution, it will run them when loaded.
Otherwise, include a header after loading
and call them, as a library, if you like.

There still are no object files.

*/


#include "::/LT/OSMain/Static.HPZ"


//Syms not in the OSMain modules
//can be declared with import or LTimport.
//There are not currently header files
//for /LT/Adam or /LT/Adam routines.

//See [::/LT/Doc/ImportExports.TXZ] Scoping and Linkages for details on
//extern, import, LTextern, LTimport, etc.


//Note: [C:/LT/Compiler/Directives.CPZ,22] Externs2Imports() is use
//in ::/LT/OSMain/Static.HPZ.

import void LtfClear(Ltf *l=NULL);


#exe {OptOn(OPTf_STD_C);};

void main()
{
//Do not combine "short" or "long"
//with "int".

  char ch;
  unsigned short s; //example of unsigned
  int i;
  long q;
  double d;
  bool b;

  LtfClear();
  PrintF("char\t:%d\r\n",sizeof(ch));
  PrintF("bool\t:%d\r\n",sizeof(b));
  PrintF("short\t:%d\r\n",sizeof(s));
  PrintF("int\t:%d\r\n",sizeof(i));
  PrintF("long\t:%d\r\n",sizeof(q));
  PrintF("double\t:%d\r\n",sizeof(d));
}

main(); //run main() when [C:/LT/OSMain/Hash1a.CPZ,733] Load()ed.
