//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.500;
  music_stacatto_factor= 0.900;
  try {
  while (!ScanKey) {
	Play("qRW1!3etD!!2GGG!3CAqD!EeF!G!C!EqD",
		" \0For \0as \0much \0as \0you \0give, \0 \0 \0Go\0d \0giv\0es \0more.\n\0");
	Play("R!etD!!2GGG!3CAqD!EeF!G!C!EqD",
		" \0For \0as \0long \0as \0you \0live, \0 \0 \0love, \0 \0love \0the \0Lord.\n\0");
	Play("!eB!DqRetEDFqR2GR3C!!2etG!3GC",
		" \0 \0 \0He\0's \0like \0 \0that.\n\0 \0 \0 \0 \0 \0");
	Play("!eB!DqRetEDFqR2GR3C!!2etG!3GC",
		" \0 \0 \0He \0 \0gives \0 \0back.\n\0 \0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
