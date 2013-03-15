//7 has graphics



/* <1> <1> (image) */








U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	"$FG,GREEN$Jesus talking...$FG$\n";
	Play("W12etG!!3BC!!qGeDBqC2G3sG!AGAeEFsG!GGG",
		"It \0is \0 \0finished.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	NewLine(1);
	PutSprite(__BIN_1);
	NewLine(7);
	Play("2etG!!3BC!!qGeDBqC2G3sG!AGAeEFsG!GGG",
		"It \0is \0 \0finished.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("!etGCFqDetB!EB!2G3D!B!eE!AsEG!!EG!!2qGG",
		"That \0 \0was \0rough!\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("!3etGCFqDetB!EB!2G3D!B!eE!AsEG!!EG!!2qGG",
		"But \0 \0I'm \0tough.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
