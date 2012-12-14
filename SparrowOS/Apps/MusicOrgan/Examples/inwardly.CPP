//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W12q.GeG3qB2GG3A2GeFF",
		"I'm \0a \0mon\0ster,\n\0 \0 \0 \0 \0 \0");
	Play("q.GeG3qB2GG3A2GeFF",
		"not \0a \0hip\0stir.\n\0 \0 \0 \0 \0 \0");
	Play("3CD!qGB!AF!C!FeDB",
		"Gol\0ly-\0gee!  \0 \0 \0Noth\0ing's \0free, \0is \0it?\n\0");
	Play("CD!qGB!AF!C!FeDB",
		"In\0ward\0ly, \0 \0 \0not \0just \0me, \0is \0it?\n\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
