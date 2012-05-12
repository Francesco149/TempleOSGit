U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  nounusedwarn dc;
  if (gr_screen_updates&1)
    PlotWinStr(task,"test",RED+WHITE*16,10,3);
  else
    PlotWinStr(task,"test",PURPLE+WHITE*16,10,3);
  PlotWinStr(task,"test",RED+WHITE*16,10,2);
  PlotWinStr(task,"test",PURPLE+WHITE*16,10,4);
}

U0 Main()
{
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  LtfClear;
  Fs->draw_it=&DrawIt;

  GetChar;
  SettingsPop;
}

Main;
