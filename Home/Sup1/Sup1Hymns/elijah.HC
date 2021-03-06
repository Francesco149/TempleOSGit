//5 has graphics



    /* <1> <1> (image) */







    /* <2> <2> (image) */






 

    /* <3> <3> (image) */

 

F64 t0=Beat;

#define WOOD_X	220
#define WOOD_Y	45

U0 DrawIt(CTask *,CDC *dc)
{
  U8 *img;
  F64 dt=Beat-t0;
  I64 r;

  if (dt<8)
    r=3;
  else if (dt<18)
    r=dt-5;
  else
    r=18-5;
  dc->color=BLUE;
  GrEllipse  (dc,WOOD_X,WOOD_Y,r,1.5*r,pi/8);
  GrFloodFill(dc,WOOD_X,WOOD_Y);

  if (dt<20)
    img=__BIN_1;
  else if (dt%0.4>.2)
    img=__BIN_2;
  else
    img=__BIN_3;
  Sprite3(dc,WOOD_X,WOOD_Y,0,img);
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  MusicSettingsRst;
  Fs->task_end_cb=&SndTaskEndCB;
  Fs->draw_it=&DrawIt;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    task=PopUpViewPrint("[C:/Misc/Bible.TXT.Z,29345] 1 Kings,18:35\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      t0=Beat;
      DocClear;
      Play("4qGeGB5sD4G5D4GqB5eFCqD4eA5F4qA",
	    "Soak, \0 \0 \0soak \0 \0that \0 \0wood.\n\0 \0 \0 \0 \0 \0 \0");
      Play("4GeGB5sD4G5D4GqB5eFCqD4eA5F4qA",
	    "Once, \0 \0 \0once \0 \0for \0 \0good...\n\0 \0 \0 \0 \0 \0 \0");
      DocClear;
      Play("4etB5DEeDFqE4B5sCFCFeCFqDsFDFD",
	    "See \0 \0 \0the \0em\0bers?  \0 \0 \0"
	    "Where \0 \0is \0 \0your \0God?\n\0 \0 \0 \0 \0");
      Play("4etB5DEeDFqE4B5sCFCFeCFqDsFDFD",
	    "Tell, \0 \0 \0tell \0me, \0sirs, \0 \0 \0"
	    "where \0 \0is \0 \0your \0God?\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
