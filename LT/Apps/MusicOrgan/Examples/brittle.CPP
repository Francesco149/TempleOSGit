//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13etCCCqCBAetCCCqCBD",
		"Hearts \0 \0 \0are \0brit\0tle.\n\0Down \0 \0 \0the \0mid\0dle.\n\0");
	Play("etCCCqCBAetEDCqCBE",
		"Not \0 \0 \0a \0rid\0dle.\n\0An \0 \0 \0a\0quit\0al.\n\0");
	Play("etFFFqFEFetFFFqFEF",
		"Play \0 \0 \0a \0fid\0dle.\n\0Hearts \0 \0 \0are \0brit\0tle.\n\0");
	Play("!etGEEeFDqC!F!F!GED",
		"Who \0 \0 \0can \0 \0mend \0the \0broke\0n \0heart\0ed?\n\0");
	Play("!etGEEeFDqC!F!F!GED",
		"Who \0 \0 \0can \0 \0mend \0the \0broke\0n \0heart\0ed?\n\0");
	Play("etBCG2sG!GGG3EEE!EetC!!A!!CqD!etE!EE!sFCF!C2etG3F!B",
		" \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0\"I \0can \0mend,\"\n\0 \0 \0 \0 \0says \0 \0the \0 \0Lord.\n\0 \0 \0");
	Play("BCG2sG!GGG3EEE!EetC!!A!!CqD!etE!EE!sFCF!C2etG3F!B",
		" \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0\"I \0can \0mend,\"\n\0 \0 \0 \0 \0says \0 \0the \0 \0Lord.\n\0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
