//5 has graphics

/* <1> <1> (image) */






U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      "$GREEN$Jesus talking...$FG$\n";
      Play("4etGB5CqGeD4B5qC4G5sG4A5G4A5eEFsGGGG",
	    "It \0is \0 \0finished.\n\0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      '\n';
      Sprite(__BIN_1);
      "%h7c",'\n';
      Play("4etGB5CqGeD4B5qC4G5sG4A5G4A5eEFsGGGG",
	    "It \0is \0 \0finished.\n\0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5etGCFqD4etB5E4BG5D4B5eE4A5sEGEG4qGG",
	    "That \0 \0was \0rough!\n\0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5etGCFqD4etB5E4BG5D4B5eE4A5sEGEG4qGG",
	    "But \0 \0I'm \0tough.\n\0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
