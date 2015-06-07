//Scroll Down



























































/* <1> <1> (image) */



























































/* <2> <2> (image) */





    /* <3> <3> (image) */



    /* <4> <4> (image) */







    /* <5> <5> (image) */




















    /* <6> <6> (image) */

U0 DrawIt(CTask *task,CDC *dc)
{
  CSprite *temps;
  Sprite3(dc,100,task->pix_height-60,0,__BIN_3);
  Sprite3(dc,194,task->pix_height-140,0,__BIN_4);
  Sprite3(dc,400,task->pix_height-10,0,__BIN_5);
  Sprite3(dc,50,task->pix_height-160,0,__BIN_6);
  temps=SpriteInterpolate(__BIN_1,__BIN_2,Tri(tS,2.0));
  Sprite3(dc,0,task->pix_height,0,temps);
  Free(temps);
}

U0 SongTask(I64)
{//Randomly generate (by God :-)
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
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  WinMax;
  WinBorder;
  AutoComplete;
  DocClear;
  Fs->text_attr=YELLOW<<4+BLACK;
  "$BG,LTCYAN$%h31c",'\n';
  View;
  SettingsPop;
}

Main;
