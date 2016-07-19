//5 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3eCsAAeCsAAeDABRCqEsBBeEDFE",
	    "Bill \0 \0 \0Gates... \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0Life's \0not \0fair.\n\0");
      Play("CAqCsDDAAeBRCDsCCBBeEAFsEE",
	    "Heavy \0 \0weights... \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0Done \0my \0share!\n\0 \0");
      Play("G2G3G2G3qGeBBBFsD2G3eDsB2G3B2G3eBAsGF2eG",
	    "God's \0 \0 \0 \0World?  \0 \0 \0 \0"
	    "Per\0fect\0 \0ly \0 \0 \0 \0 \0 \0 \0just.\n\0 \0 \0");
      Play("3sG2G3G2G3qGeBBBFsD2G3eDsB2G3B2G3eBAsGF2eG",
	    "Thank's \0 \0 \0 \0Bill, \0 \0 \0 \0"
	    "but \0God \0 \0we \0 \0 \0 \0 \0 \0 \0trust.\n\0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
