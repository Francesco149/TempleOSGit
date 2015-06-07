//7 has graphics





/* <1> <1> (image) */


U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.814;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,46190] Psalms,18:26 Try other translations.\n"
	  "[C:/Misc/Bible.TXT.Z,12005] Numbers,11:1 Read the whole chapter.\n"
	  "[C:/Misc/Bible.TXT.Z,29345] 1 Kings,22:1 Read the whole chapter.\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("3sCACAeABqAeFAetDF2GeG3FqG2G",
	    "$CL$Oh, \0 \0my \0 \0 \0 \0Gosh!\n\0 \0 \0 \0 \0"
	    "You \0 \0 \0lied!\n\0$GREEN$God talking...\n$FG$\0");
      Play("3sCACAeABqAeFAetDF2GeG3FqG2G",
	    "$RED$Just \0 \0 \0 \0 \0 \0joshed.\n\0 \0 \0"
	    "Shhh. \0 \0 \0 \0Con\0fide.\n\0 \0");
      "%17c",CH_SPACE;
      Sprite(__BIN_1);
      Play("3eDCqD2G3etE2G3GqDDeGAqD",
	    "\nI \0am \0God.\n\0 \0I \0 \0can \0lie.\n\0 \0 \0 \0 \0");
      Play("eDCqD2G3etE2G3GqDDeGAqD",
	    "I \0am \0God.\n\0 \0I \0 \0can \0lie.$FG$\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
