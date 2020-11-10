//5 has graphics
/* <1> <1> (image) */





U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      Sprite(__BIN_1);
      "\n\n\n\n\n\n$GREEN$To God...\n$FG$";
      Play("5etD4GRsR5GRGqFetEDCCRCRC4BqGsGAGA",
	    "O\0kay, \0 \0 \0what \0 \0is \0it \0"
	    "You \0 \0 \0want \0 \0us \0 \0to \0 \0do?\n\0 \0 \0 \0 \0");
      Play("5etD4GRsR5GRGqFetEDCCRCRC4BqGsGAGA",
	    " \0 \0 \0 \0Let \0 \0me \0guess, \0"
	    " \0 \0 \0we \0 \0are \0 \0to \0 \0choose.\n\0 \0 \0 \0 \0");
      Play("4eB5FqDC4eBB5RGetERFqGE",
	    " \0 \0 \0 \0 \0 \0 \0I'm \0so \0 \0con\0fused.\n\0 \0");
      Play("4eB5FqDC4eBB5RGetERFqGE",
	    " \0 \0 \0 \0 \0 \0 \0I \0must \0 \0have \0snoozed.\n\0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
