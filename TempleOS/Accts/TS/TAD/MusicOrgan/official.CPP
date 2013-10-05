//2 has words



      /* <1> <1> (image) */





      /* <2> <2> (image) */


U0 DrawIt(CTask *task,CDC *dc)
{
  I64 w=task->win_pixel_width,h=task->win_pixel_height;
  F64 t=tM,
      x=w*Saw(t,2.0),
      y=h*Sqr((x-w>>1)/w>>1),
      dx=w/2,
      dy=h*2*((x-w>>1)/w>>1),
      theta=Arg(dx,dy);
  SpritePlotRotZ3B(dc,x,y,0,__BIN_1,0.3*Sin(30*t)+theta);
  SpritePlotRotZ3B(dc,x,y+20*FullSaw(30*t,0.5),0,__BIN_2,0.3*Sin(t)+theta);
}


U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  task=PopUpViewStr(
    "[C:/TempleOS/Misc/Bible.TXT.Z,81941] Luke,18:11\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  while (!ScanKey) {
	Play("2eGGqG3eCCADqFeFBqGeCB",
		"Hey! \0Hey! \0Hey! \0That's \0 \0 \0a \0foul!\n\0 \0 \0 \0 \0 \0");
	Play("2GGqG3eCCADqFeFBqGeCB",
		"$FG,RED$Hey!\n\0 \0 \0 \0 \0 \0Don't \0 \0you \0 \0scowl!\n$FG$\0 \0 \0");
  }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
