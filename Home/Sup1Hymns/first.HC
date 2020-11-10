//5 has graphics
/* <1> <1> (image) */





U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "$WW+H,1$[C:/Misc/Bible.TXT.Z,76908]\n\n"
	  "How many people hear \"Love God with all heart "
	  "mind and soul; and neighbor as self,\" but walk "
	  "away thinking \"Love neighbor with all heart "
	  "mind and soul?\"\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    DocRecalc(DocPut(task)); //Reformat word-wrap with new window size.
    while (!ScanKey) {
      Play("5qF4eAGqB5eG4B5qEetDC4B5qEeFF",
	    "First, \0lo\0ve \0God \0with \0 \0ALL \0$RED$heart$FG$, \0"
	    "mind \0and \0soul.\n\0 \0 \0");
      Sprite(__BIN_1);
      "$CM,0,6$";
      Play("5qF4eAGqB5eG4B5qEetDC4B5qEeFF",
	    "First, \0lo\0ve \0God \0with \0 \0ALL \0$RED$heart$FG$, \0"
	    "mind \0and \0soul.\n\0 \0 \0");
      Sprite(__BIN_1);
      "$CM,0,6$";
      Play("5G4BsA5D4A5DqE4G5eCDqG4sGBGBetB5D4B",
	    "What's \0 \0 \0 \0 \0 \0left?  \0 \0I \0don't \0know.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0");
      Play("5eG4BsA5D4A5DqE4G5eCDqG4sGBGBetB5D4B",
	    "What's \0 \0 \0 \0 \0 \0left?  \0 \0I \0don't \0know.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
