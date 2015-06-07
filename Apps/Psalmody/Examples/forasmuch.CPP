//7 has graphics
/* <1> <1> (image) */





U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo= 2.500;
  music.stacatto_factor= 0.900;
  try {
    while (!ScanKey) {
      Sprite(__BIN_1);
      "%h6c",'\n';
      Play("qR3etD2GGG3CAqDEeFGCEqD",
	    " \0For \0as \0much \0as \0you \0give, \0"
	    " \0 \0Go\0d \0giv\0es \0more.\n\0");
      Play("RetD2GGG3CAqDEeFGCEqD",
	    " \0For \0as \0long \0as \0you \0live, \0"
	    " \0 \0love, \0 \0love \0the \0Lord.\n\0");
      Play("eBDqRetEDFqR2GR3C2etG3GC",
	    " \0 \0 \0He\0's \0like \0 \0that.\n\0 \0 \0 \0 \0 \0");
      Play("eBDqRetEDFqR2GR3C2etG3GC",
	    " \0 \0 \0He \0 \0gives \0 \0back.\n\0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
