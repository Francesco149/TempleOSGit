U0 DrawIt(CTask *task,CDC *)
{
  if (win_updates&1)
    PlotWinStr(task,"test",RED+WHITE*16,10,3);
  else
    PlotWinStr(task,"test",PURPLE+WHITE*16,10,3);
  PlotWinStr(task,"test",RED+WHITE*16,10,2);
  PlotWinStr(task,"test",PURPLE+WHITE*16,10,4);
}

U0 Main()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;

  GetChar;
  SettingsPop;
}

Main;
