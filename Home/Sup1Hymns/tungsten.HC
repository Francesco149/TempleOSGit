//5 has graphics

/* <1> <1> (image) */



























































/* <2> <2> (image) */









U0 DrawIt(CTask *,CDC *dc)
{
  Sprite3(dc,216,0,0,__BIN_2);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=DKGRAY<<4+YELLOW;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 1.930;
  music.stacatto_factor= 0.681;
  try {
    while (!ScanKey) {
      Play("5eD4A5CC4qG5eD4A5CCqFeFAGA",
	    "Tip \0 \0of \0the \0tongue...\n\0Can't \0 \0"
	    "clear \0my \0head...\n\0Toss \0and \0turn \0 \0");
      Play("5FEsEEGG4qAB5eD4A5CC4qG5eD4A",
	    " \0 \0on \0 \0my \0 \0bed.\n\0So, \0this \0 \0"
	    "ain't \0 \0fun.\n\0My \0 \0");
      Play("5CCqFeFAGAFEsEEGG4qA5eEC",
	    "mind \0is \0dead.\n\0Slow\0ly \0burn \0 \0"
	    "through\n\0 \0 \0 \0all \0of \0them.\n\0 \0 \0");
      Play("5F4sA5EeG4BAsAGA5D4A5DeD4A5F4G5GG4qA",
	    "Try \0 \0hard?\n\0 \0 \0 \0 \0 \0 \0Get \0 \0"
	    "luck\0y.\n\0 \0 \0 \0 \0 \0 \0");
      Play("5eF4sA5EeG4BAsAGA5D4A5DeD4A5F4G5GG4qA",
	    "Try \0 \0hard?\n\0 \0 \0 \0 \0 \0 \0Get \0 \0"
	    "luck\0y.\n\0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
