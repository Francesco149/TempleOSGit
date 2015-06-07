//TempleOS supports standard [C:/Kernel/StrPrint.CPP.Z,898] Print()
//codes and extended ones.
//See [C:/Doc/Print.TXT.Z,1] Print("") Fmt Strings.

//Demonstrates [C:/Doc/Print.TXT.Z,2] aux_fmt_num codes.

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

'\n';

"%h25c\n",'x'; //Print 25 x's

"%h*c\n",25,'y'; //Print 25 y's

'zzzz\n'; //[C:/Kernel/KeyDev.CPP.Z,20] PutChars() up to 8 characters.

'\n';

