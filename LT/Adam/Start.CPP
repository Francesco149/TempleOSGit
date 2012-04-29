#if snd_dev==SD_HD_AUDIO
#include "::/LT/Demo/Snd/PolyPhonic.CPZ"
#endif

U0 Start()
{
  TaskStruct *task;
  Type("::/LT/Doc/Splash.TXZ");
  Fs->win_left=TEXT_COLS/4;
  Fs->win_right=3*TEXT_COLS/4;
  Fs->win_top=TEXT_ROWS/4;
  TaskDerivedValsUpdate;
  LBts(&Fs->display_flags,DISPLAYf_WIN_ON_TOP);
  "$FG,GREEN$Tip of day$FG$\n";
  TipOfDay;
  "$FG,GREEN$<SHIFT-ESC>$FG$ to exit.\n";
#if snd_dev==SD_HD_AUDIO
  PopUp("#include \"::/LT/Demo/Snd/SndScope.CPZ\";",NULL,&task);
  TaskWaitIdle(task);
  WinToTop;
  PolyPhonic;
  Kill(task);
#else
  nounusedwarn task;
  View;
#endif
}

Start;
