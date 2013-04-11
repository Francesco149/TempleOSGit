//See [C:/TempleOS/Adam/Gr/GrScreen.CPP.Z,1] ExtScreenCodes for the format
//of the U32 you pass to PlotWinU8()

//See [C:/TempleOS/Demo/ExtendedChars.CPP.Z,1]
//and [C:/TempleOS/Demo/Graphics/FontEd.CPP.Z,1].
//Hold ALT and press a 3 digit decimal number
//or press <CTRL-ALT-A>.

U0 DrawIt(CTask *task,CDC *)
{
  I64 i;
  for (i=0;i<256;i++)
    PlotWinU8(task,i+BLACK<<12+WHITE<<8,(i&15)*2,(i/16)*2);
}

U0 ScreenCodes()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar(NULL,FALSE);
  SettingsPop;
}

ScreenCodes;
