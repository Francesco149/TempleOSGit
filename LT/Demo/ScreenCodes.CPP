//See [::/LT/Adam/Gr/GrScreen.APZ,ExtScreenCodes] ExtScreenCodes for the format
//of the U32 you pass to PlotWinU8()

//See [::/LT/Demo/ExtendedChars.CPZ] ::/LT/Demo/ExtendedChars.CPZ
//and [::/LT/Demo/Graphics/FontEd.CPZ] ::/LT/Demo/Graphics/FontEd.CPZ.
//Hold ALT and press a 3 digit decimal number
//or press <CTRL-ALT-A>.

U0 DrawIt(TaskStruct *task,GrDC *)
{
  I64 i;
  for (i=0;i<256;i++)
    PlotWinU8(task,i+BLACK<<12+WHITE<<8,(i&15)*2,(i/16)*2);
}

U0 ScreenCodes()
{
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  LtfClear;
  Fs->draw_it=&DrawIt;
  GetChar(NULL,FALSE);
  SettingsPop;
}

ScreenCodes;
