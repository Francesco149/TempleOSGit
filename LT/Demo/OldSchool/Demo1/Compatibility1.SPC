/*
This is the quick start version to get you
going with conventional programming.

See [::/LT/Demo/OldSchool/Demo2/Compatibility2.SPP] ::/LT/Demo/OldSchool/Demo2/Compatibility2.SPP
for detailed info.

Create a binary (executable) by
right-clicking on this file in a
[C:/LT/OSMain/Dsk/Dsk1b.CPZ,238] Dir() listing and pressing "COMPILE".
Then, right click on the .BIZ file
and press "LOAD".

*/


// imports all syms from the Adam Task's hash table.
#include "::/LT/OSMain/StaticAdam.HPZ"

#exe {OptOn(OPTf_STD_C);};

void main()
{
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



