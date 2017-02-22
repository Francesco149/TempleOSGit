//5 has graphics
U0 DrawIt(CTask *,CDC *dc)
{
  static F64 last_t=Beat,tt=0;
  tt+=0.4*(Beat-last_t)*Abs(Sin(Beat));
  last_t=Beat;
  tt%=3.5;
  Sprite3ZB(dc,180+tt,tt*30,0,__BIN_1,0.25*Sin(tt)+0.1);
}


/* <1> <1> (image) */

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      "$GREEN$God talking...\n$FG$";
      Play("5qGDF4A5F4AA5F",
	    "Pre\0ci\0ous \0 \0one,\n\0 \0 \0My \0");
      Play("5GDF4A5F4AA5F",
	    "pre\0ci\0ous \0 \0one,\n\0 \0 \0He\0");
      Play("5DE4A5F4GG5E4B",
	    "ar \0my \0 \0word,\n\0 \0 \0For \0your \0");
      Play("5DE4A5F4GG5E4B",
	    "own \0 \0 \0good.\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
