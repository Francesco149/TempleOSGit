//5 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("M9/8");
      Play("5hDeFhDeFhDC");
      Play("5FeDhFeDhFE");
      Play("M4/4");
      Play("5qD4GAeB5FFE4qG5D4A",
	    "Hold \0 \0on!\n\0 \0It's \0gon\0na \0be \0fine.\n\0 \0");
      Play("5D4GAeB5FFE4qG5D4A",
	    "Hold \0 \0on!\n\0 \0It's \0gon\0na \0be \0fine.\n\0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
