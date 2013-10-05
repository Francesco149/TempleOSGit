


























































/*  <1> (image) */



























































/*  <2> (image) */

    /* <3> <3> (image) */




    /* <4> <4> (image) */









    /* <5> <5> (image) */





















    /* <6> <6> (image) */


U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *temps;
  SpritePlot3(dc,100,task->win_pixel_height-60,0,__BIN_3);
  SpritePlot3(dc,194,task->win_pixel_height-140,0,__BIN_4);
  SpritePlot3(dc,400,task->win_pixel_height-10,0,__BIN_5);
  SpritePlot3(dc,50,task->win_pixel_height-160,0,__BIN_6);
  temps=SpriteInterpolate(__BIN_1,__BIN_2,Tri(tT,2.0));
  SpritePlot3(dc,0,task->win_pixel_height,0,temps);
  Free(temps);
}

U0 SongTask(I64)
{ //Randomly generate (by God :-)
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
	Play("2qG3eCGqEAFCeDFqB");
	Play("2G3eCGqEAFCeDFqB");
	Play("BDeF2G3etEBAqBAetAEDeE2G");
	Play("3qBDeF2G3etEBAqBAetAEDeE2G");
  }
}

U0 Main()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  WinMax;
  WinBorder;
  WordStat;
  DocClear;
  Fs->text_attr=YELLOW<<4+BLACK;
  "$BG,LTCYAN$"; NewLine(31);
  View;
  SettingsPop;
}

Main;
