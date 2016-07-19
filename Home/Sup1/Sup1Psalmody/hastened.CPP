//5 has words
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,57332] Isaiah,5:18\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("2e.FsF3etCFCqB2Ge.FsF3etCFCqB2eGG",
	    " \0 \0 \0$RED$Hur\0ry\0-up!\n\0 \0 \0 \0 \0"
	    "Hur\0ry\0-up!$FG$\n\0 \0 \0");
      Play("3qC2eG3F2qGetGG3FeDDetDA2G3qAG",
	    "No, \0 \0can't.\n\0 \0 \0 \0 \0Fast \0as \0"
	    "I \0 \0 \0 \0go.\n\0");
      Play("C2eG3F2qGetGG3FeDDetDA2G3qAeGG",
	    "I \0 \0won't \0 \0 \0 \0 \0rape \0or \0 \0 \0 \0 \0hoe.\n\0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
