//8 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13qBeBDEDCq.BqBeBDED",
		"$FG,RED$Look, \0there \0is \0real\0ly \0a \0God.\n\0Look, \0there \0is \0real\0ly \0");
	Play("Cq.BqAeBCDCBq.AqB",
		"a \0God.\n\0Look, \0there \0is \0real\0ly \0a \0God.\n\0Look, \0");
	Play("eBDEDCq.BqCeEEqC!D",
		"there \0is \0real\0ly \0a \0God.$FG$\n\0Well, \0par\0don \0 \0me!\n\0");
	Play("!eB!AECFEqDCeEEqC!D",
		" \0 \0Now, \0 \0I \0can \0see!\n\0 \0Par\0don \0 \0me!\n\0");
	Play("!eB!AECFEqD",
		" \0 \0Now, \0 \0I \0can \0see.\n\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
