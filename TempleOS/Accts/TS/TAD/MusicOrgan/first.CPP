//8 has graphics

/* <heart> <1> (image) */











U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  task=PopUpViewStr(
    "$WW+H,1$[C:/TempleOS/Misc/Bible.TXT.Z,76908]\n\n"
    "How many people hear \"Love God with all heart "
    "mind and soul; and neighbor as self,\" but walk "
    "away thinking \"Love neighbor with all heart "
    "mind and soul?\"\n"
    );
  TaskWait(task);
  SetWinHorz(Fs->win_left,Fs->win_right,task);
  SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
  while (!ScanKey) {
	Play("3qFeA2G3qBeGBqEetDCBqEeFF",
	"First, \0lo\0ve \0God \0with \0 \0ALL \0$FG,RED$heart$FG$, \0mind \0and \0soul.\n\0 \0 \0");
	PutSprite(__BIN_1);
	"$CM,0,6$";
	Play("qFeA2G3qBeGBqEetDCBqEeFF",
	"First, \0lo\0ve \0God \0with \0 \0ALL \0$FG,RED$heart$FG$, \0mind \0and \0soul.\n\0 \0 \0");
	PutSprite(__BIN_1);
	"$CM,0,6$";
	Play("GBsADADqE2G3eCDqG2sG3B2G3BetBDB",
	"What's \0 \0 \0 \0 \0 \0left?  \0 \0I \0don't \0know.\n\0 \0 \0 \0 \0 \0 \0 \0");
	Play("eGBsADADqE2G3eCDqG2sG3B2G3BetBDB",
	"What's \0 \0 \0 \0 \0 \0left?  \0 \0I \0don't \0know.\n\0 \0 \0 \0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
