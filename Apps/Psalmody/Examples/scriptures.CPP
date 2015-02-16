//8 has graphics

/*  <1> (image) */





U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
  "$WW+H,1$";
  while (!ScanKey) {
    Play("3qECGeECqBBAA",
	"Stor\0ies \0old \0are \0 \0my \0de\0light.\n\0 \0");
    Sprite(__BIN_1);
    '\n\n\n\n\n';
    Play("ECGeECqBBAA",
	"No, \0I \0won't \0share \0 \0they're \0p\0light.\n\0 \0");
    Sprite(__BIN_1);
    '\n\n\n\n\n';
    Play("DGCEBGCA",
	"I \0 \0love \0 \0God.  \0 \0Yes, \0sir.\n\0");
    Play("DGCEBGCA",
	    "I \0 \0love \0 \0God.  \0 \0Oh, \0sir!\n\0");
  }
  } catch
    PutExcept;
  Snd(0);
}

Song;
