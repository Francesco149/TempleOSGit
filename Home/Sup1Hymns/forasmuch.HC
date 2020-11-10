//5 has graphics
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
      Play("qR5etD4GGG5C4A5qDEeFGCEqD",
	    " \0For \0as \0much \0as \0you \0give, \0"
	    " \0 \0Go\0d \0giv\0es \0more.\n\0");
      Play("5RetD4GGG5C4A5qDEeFGCEqD",
	    " \0For \0as \0long \0as \0you \0live, \0"
	    " \0 \0love, \0 \0love \0the \0Lord.\n\0");
      Play("4eB5DqRetEDFqR4GR5C4etG5GC",
	    " \0 \0 \0He\0's \0like \0 \0that.\n\0 \0 \0 \0 \0 \0");
      Play("4eB5DqRetEDFqR4GR5C4etG5GC",
	    " \0 \0 \0He \0 \0gives \0 \0back.\n\0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
