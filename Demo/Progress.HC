/*[C:/Kernel/KernelB.HH.Z,209] progress1-[C:/Kernel/KernelB.HH.Z,227] progress4 are just global vars
shown on the wallpaper. I use them in debugging a lot.

If you set [C:/Kernel/KernelB.HH.Z,212] progress1_max-[C:/Kernel/KernelB.HH.Z,230] progress4_max,
then a progress bar will display.

If you set [C:/Kernel/KernelB.HH.Z,213] progress1_t0-[C:/Kernel/KernelB.HH.Z,231] progress4_t0
then the elapsed time will display.

If you set [C:/Kernel/KernelB.HH.Z,214] progress1_tf-[C:/Kernel/KernelB.HH.Z,232] progress4_tf
then the estimated time will display.
*/

U0 Main1()
{
  "\nWith description.\n";
  PressAKey;
  progress1_max=200;
  StrCpy(progress1_desc,"Progress Demo"); //PROGRESS_DESC_LEN is max
  while (progress1++!=progress1_max)
    Sleep(20);
  ProgressBarsRst;
}
Main1;

U0 Main2()
{
  "\nWith description and elapsed time.\n";
  PressAKey;
  progress1_max=200;
  progress1_t0=tS;
  StrCpy(progress1_desc,"Progress Demo"); //PROGRESS_DESC_LEN is max
  while (progress1++!=progress1_max)
    Sleep(20);
  ProgressBarsRst;
}
Main2;

U0 Main3()
{
  "\nWith description, elapsed time and registry total time.\n";
  PressAKey;
  progress1_max=200;
  progress1_t0=tS;
  RegExe("DemoCompany/ProgressBars");
  StrCpy(progress1_desc,"Progress Demo"); //PROGRESS_DESC_LEN is max
  while (progress1++!=progress1_max)
    Sleep(20);
  ProgressBarsRst("DemoCompany/ProgressBars");
}
Main3;

U0 Main4()
{
  "\nNo description.\n";
  PressAKey;
  progress1_max=200;
  while (progress1++!=progress1_max)
    Sleep(20);
  ProgressBarsRst;
}
Main4;


"\nNow, from asm...  It's handy for debugging.\n"
"Watch the progress on the wallpaper.\n";
WinBorder(ON);
PressAKey;

asm {
_PROGRESS_DEMO::
	MOV	RCX,12000000
@@05:	MOV	U64 [&progress1],RCX
	CALL	&Yield
	LOOP	@@05
	MOV	U64 [&progress1],RCX
	RET
}
_extern _PROGRESS_DEMO U0 AsmProgressDemo();
AsmProgressDemo;
