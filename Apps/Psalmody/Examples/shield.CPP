//7 has graphics

/* <1> <1> (image) */




U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,94514] Ephesians,6:11\n"
	  "[C:/Misc/Bible.TXT.Z,76908] Matthew,5:10\n"
	  "[C:/Misc/Bible.TXT.Z,93323] 2 Corinthians,6:7\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("3qDCDeEGBDetBBFqDeDB",
	    "God, \0my \0breast \0plate, \0 \0won't \0be \0put \0to \0shame.\n\0 \0 \0 \0");
      '\n';
      Sprite(__BIN_1);
      '\n\n\n\n\n';
      Play("qDCDeEGBDetBBFqDeDB",
	    "God, \0my \0breast \0plate, \0 \0won't \0be \0put \0to \0shame.\n\0 \0 \0 \0");
      '\n\n\n\n\n';
      Sprite(__BIN_1);
      '\n\n\n\n\n';
      Play("FAetAFDeDFqAetDFBeDFetFDEqD",
	    "Mocked \0 \0 \0a \0lit\0tle, \0 \0 \0badge \0 \0of \0 \0 \0cour\0 \0 \0age.\n\0");
      Play("eFAetAFDeDFqAetDFBeDFetFDEqD",
	    "Mocked \0 \0 \0a \0lit\0tle, \0 \0 \0badge \0 \0of \0 \0 \0cour\0 \0 \0age.\n\0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
