//TempleOS supports standard [C:/Kernel/StrPrint.CPP.Z,946] Print()
//codes and extended ones.
//See [C:/Doc/HolyC.TXT.Z,1] Print Extended Codes.

//This file demonstrates [C:/Doc/HolyC.TXT.Z,3] sub_fmt codes.



"%10h?d\n",123456789;
"%10h?d\n",12345;

"%10h3d\n",123456789;
"%10h3d\n",12345;

"\nWith the ',' flag:\n";
"%10,h3d\n",123456789;
"%10,h3d\n",12345;

"\nFloating Point:\n";
"%16h?n\n",.00123456789;
"%16h?n\n",12345.0;

"%16.2h-6n\n",.00123456789;
"%16.2h-6n\n",12345.0;

"\n";
