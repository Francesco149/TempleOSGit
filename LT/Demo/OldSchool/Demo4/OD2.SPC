/* ::/LT/Demo/OldSchool/Demo4/ObjDemo.SPZ */

// imports all syms from the Adam Task's hash table.
#include "::/LT/OSMain/StaticAdam.HPZ"

asm {
USE64
	IMPORT	Beep;
_MY_BEEP::
	ENTER	0

	PUSH	FALSE
	PUSH	300
	CALL	&Beep

	LEAVE
	RET
}

LTextern _MY_BEEP U0 MyBeep();

U0 Routine2(U8 *st)
{
  PrintF("Routine2:%s\r\n",st);
  MyBeep();
}

PutS("$BK,1$Woo-Hoo!$BK,0$\r\n");
Dir;
PutS("$BK,1$Woo-Hoo!$BK,0$\r\n");

Routine2("Cool");

