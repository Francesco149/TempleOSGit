//8 has graphics



    /* <1> <1> (image) */








    /* <2> <2> (image) */






 


    /* <3> <3> (image) */

 



F64 t0=tMBeat;

#define WOOD_X	220
#define WOOD_Y	45

U0 DrawIt(CTask *,CDC *dc)
{
  U8 *img;
  F64 dt=tMBeat-t0;
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
  SpritePlot3(dc,WOOD_X,WOOD_Y,0,img);
}



U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  MusicSettingsRst;
  Fs->task_end_cb=&SndTaskEndCB;
  Fs->draw_it=&DrawIt;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,29345] 1 Kings,18:35\n"
    );
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  while (!ScanKey) {
	t0=tMBeat;
	DocClear;
	Play("W12qGeG!3BsD2G3D2G3qBeFCqDeAFqA",
		"Soak, \0 \0 \0soak \0 \0that \0 \0wood.\n\0 \0 \0 \0 \0 \0 \0");
	Play("2GeG!3BsD2G3D2G3qBeFCqDeAFqA",
		"Once, \0 \0 \0once \0 \0for \0 \0good...\n\0 \0 \0 \0 \0 \0 \0");
	DocClear;
	Play("!etB!D!EeD!FqEB!sCFCFeC!!FqDsFD!!F!!D",
		"See \0 \0 \0the \0em\0bers?  \0 \0 \0Where \0 \0is \0 \0your \0God?\n\0 \0 \0 \0 \0");
	Play("!etB!D!EeD!FqEB!sCFCFeC!!FqDsFD!!F!!D",
		"Tell, \0 \0 \0tell \0me, \0sirs, \0 \0 \0where \0 \0is \0 \0your \0God?\n\0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
