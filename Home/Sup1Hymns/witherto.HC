//5 has words
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,79970] Mark,11:12\n"
	  "[C:/Misc/Bible.TXT.Z,76908] Matthew,3:10\n"
	  "[C:/Misc/Bible.TXT.Z,76908] Matthew,25:18\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("4qG5sFEFEqD4B5EetC4BG5qF4sB5D4B5D",
	    " \0Damn \0 \0 \0 \0 \0 \0fig \0 \0 \0 \0tree.\n\0 \0 \0 \0 \0");
      Play("4qG5sFEFEqD4B5EetC4BG5qF4sB5D4B5D",
	    " \0On\0ly \0 \0 \0good \0 \0for \0a \0 \0 \0pee.\n\0 \0 \0 \0 \0");
      Play("5qC4G5eFC4G5E4qA5CD4A",
	    "Has \0no \0fruit.\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("5C4G5eFC4G5E4qA5CD4A",
	    "Kill \0the \0root.\n\0 \0 \0 \0 \0 \0 \0 \0");
      "$GREEN$Be sure to bear 'fruit'.$FG$\n\n";
    }
  } catch
    PutExcept;
  Snd;
}

Song;
