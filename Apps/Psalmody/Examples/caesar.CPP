//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 1.765;
  music.stacatto_factor= 0.681;
  try {
    while (!ScanKey) {
      Play("3etCBCDDEqCDetEFGGGGEF4A3qE",
	    "Stab\0 \0bed \0right \0in \0the \0back.\n\0"
	    " \0Right \0in \0the \0back \0 \0 \0by \0 \0my \0friends.\n\0");
      Play("etCBCDDEqCDetEFGGGGEF4A3qE",
	    "Stab\0 \0bed \0right \0in \0the \0back.\n\0"
	    " \0Right \0in \0the \0back \0 \0 \0by \0 \0my \0friends.\n\0");
      Play("eEFetCGCeFCetGBGDEEeCEsGEGEeEG",
	    " \0How \0 \0could \0 \0you?\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("EFetCGCeFCetGBGDEEeCEsGEGEeEG",
	    " \0How \0 \0could \0 \0you?\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
