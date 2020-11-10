//5 has graphics





/* <1> <1> (image) */


F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  if (dt>16)
    Sprite3XB(dc,170,45,0,__BIN_1,0.5*Sin(pi*dt));
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.814;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,46190] Psalms,18:26 Try other translations.\n"
	  "[C:/Misc/Bible.TXT.Z,12005] Numbers,11:1 Read the whole chapter.\n"
	  "[C:/Misc/Bible.TXT.Z,29345] 1 Kings,22:1 Read the whole chapter.\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      t0=Beat;
      Play("5sC4A5C4AeABqA5eF4A5etDF4GeG5FqG4G",
	    "$CL$Oh, \0 \0my \0 \0 \0 \0Gosh!\n\0 \0 \0 \0 \0"
	    "You \0 \0 \0lied!\n\0$GREEN$God talking...\n$FG$\0");
      Play("5sC4A5C4AeABqA5eF4A5etDF4GeG5FqG4G",
	    "$RED$Just \0 \0 \0 \0 \0 \0joshed.\n\0 \0 \0"
	    "Shhh. \0 \0 \0 \0Con\0fide.\n\0 \0");
      Play("5eDCqD4G5etE4G5GqDDeG4A5qD",
	    "\nI \0am \0God.\n\0 \0I \0 \0can \0lie.\n\0 \0 \0 \0 \0");
      Play("5eDCqD4G5etE4G5GqDDeG4A5qD",
	    "I \0am \0God.\n\0 \0I \0 \0can \0lie.$FG$\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
