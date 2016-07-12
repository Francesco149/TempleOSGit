//5 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3sDCDCqADetDGFeD4A3qtEetFeAsCAe.GsE",
	    "Ad\0am \0 \0Eve?\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0");
      Play("DCDCqADetDGFeD4A3qtEetFeAsCAe.GsE",
	    "No \0 \0re\0prieve.\n\0 \0 \0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0");
      Play("e.C2sG3e.BsCeAsFC2FG3eCqEeBDBsCBeEE",
	    "Mon\0 \0key \0 \0 \0 \0 \0want \0some \0fruit?\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0");
      Play("e.C2sG3e.BsCeAsFC2FG3eCqEeBDBsCBeEE",
	    "Thou \0 \0shalt \0 \0 \0 \0 \0wear \0a \0suit.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
