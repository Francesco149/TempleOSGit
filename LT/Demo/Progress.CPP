/*
  [C:/LT/OSMain/Adam1b.HPZ,43] progress1-[C:/LT/OSMain/Adam1b.HPZ,52] progress4 are just global vars
  shown on the wallpaper. I use them in debugging a lot.
  If you set [C:/LT/OSMain/Adam1b.HPZ,44] progress1_max-[C:/LT/OSMain/Adam1b.HPZ,53] progress4_max,
  then a bar will display.
*/


U0 Main()
{
  ProgressBarsRst;
  StrCpy(progress1_desc,"Progress Demo"); //PROGRESS_DESC_LEN is max
  progress1_max=400;
  while (progress1++!=progress1_max)
    Sleep(10);
  progress1_max=0; //hide
}


Main;

"$FG,LTRED$Now, from asm...  It's handy for debugging.$FG$\n"
"$FG,LTRED$Watch the progress on the wallpaper.$FG$\n";
PressAKey;

asm {
_PROGRESS_DEMO::
	MOV	RCX,0x80000000
@@05:	MOV	U64 [&progress1],RCX
	LOOP	@@05
	MOV	U64 [&progress1],RCX
	RET
}

LTextern _PROGRESS_DEMO U0 AsmProgressDemo();
AsmProgressDemo;
