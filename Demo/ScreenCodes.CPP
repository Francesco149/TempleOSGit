/*See [TextBase Layer,1] TextBase Layer for the format
of the U32 you pass to TextChar()

See [C:/Doc/CharOverview.TXT.Z,1], [C:/Demo/ExtendedChars.CPP.Z,1],
and [C:/Demo/Graphics/FontEd.CPP.Z,1].

Hold <ALT> and press a 3 digit decimal number
or press <CTRL-ALT-a> to enter extended characters.
*/

U0 DrawIt(CTask *task,CDC *)
{
  I64 i;
  for (i=0;i<256;i++)
    TextChar(task,,(i&15)*2,(i/16)*2,i+BLACK<<12+WHITE<<8);
}

U0 ScreenCodes()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar(,FALSE);
  SettingsPop;
}

ScreenCodes;
