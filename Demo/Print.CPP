//TempleOS supports standard [MN:Print] Print()
//codes and extended ones.
//See [C:/Doc/HolyC.TXT,1] Print Codes.

//This file demonstrates [C:/Doc/HolyC.TXT,3] sub_fmt codes.



"%10h?d\n",123456789;
"%10h?d\n",12345;

"%10h3d\n",123456789;
"%10h3d\n",12345;

"\nWith the 'k' flag:\n";
"%10kh?d\n",123456789;
"%10kh?d\n",12345;

"%10kh3d\n",123456789;
"%10kh3d\n",12345;

"\nWith the ',' flag:\n";
"%10,kh3d\n",123456789;
"%10,kh3d\n",12345;

"\nFloating Point:\n";
"%16h?n\n",.00123456789;
"%16h?n\n",12345.0;

"%16.2h-6n\n",.00123456789;
"%16.2h-6n\n",12345.0;


