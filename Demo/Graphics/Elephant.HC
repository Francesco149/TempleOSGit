//Scroll Down



























































/* <1> <1> (image) */



























































/* <2> <2> (image) */





    /* <3> <3> (image) */



    /* <4> <4> (image) */







    /* <5> <5> (image) */




















    /* <6> <6> (image) */


U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *tmps;
  Sprite3(dc,100,task->pix_height-60,0,__BIN_3);
  Sprite3(dc,194,task->pix_height-140,0,__BIN_4);
  Sprite3(dc,400,task->pix_height-10,0,__BIN_5);
  Sprite3(dc,50,task->pix_height-160,0,__BIN_6);
  tmps=SpriteInterpolate(Tri(tS,2.0),__BIN_1,__BIN_2);
  Sprite3(dc,0,task->pix_height,0,tmps);
  Free(tmps);
}

U0 SongTask(I64)
{//Randomly generate (by God :-)
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("4qG5eCGqE4A5FCeDF4qB");
    Play("4G5eCGqE4A5FCeDF4qB");
    Play("4B5DeF4G5etE4BAqBAetA5EDeE4G");
    Play("4qB5DeF4G5etE4BAqBAetA5EDeE4G");
  }
}

U0 Main()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  AutoComplete;
  WinBorder;
  WinMax;
  DocClear;
  Fs->text_attr=YELLOW<<4+BLACK;
  "$BG,LTCYAN$%h31c",'\n';
  View;
  SettingsPop;
}

Main;
