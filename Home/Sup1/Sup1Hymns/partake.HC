//5 has graphics
/* <1> <1> (image) */






U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      Play("5eDGqGeEGE4BqAA5eE4B5etCF4B",
	    "Thank \0 \0You, \0God \0 \0for \0a\0bun\0 \0dance.\n\0"
	    " \0 \0 \0 \0");
      Sprite(__BIN_1);
      "%h8c",'\n';
      Play("5eDGqGeEGE4BqAA5eE4B5etCF4B",
	    "Thank \0 \0You, \0God \0 \0for \0a\0bun\0 \0dance.\n\0"
	    " \0 \0 \0 \0");
      Sprite(__BIN_1);
      "%h7c",'\n';
      Play("4BBB5qF4eA5GqD4eB5DDFDCqE",
	    " \0 \0 \0Well, \0I \0guess, \0 \0"
	    "that \0now \0it's \0time \0to \0eat.\n\0 \0");
      Play("4etBBB5qF4eA5GqD4eB5DDFDCqE",
	    " \0 \0 \0Well, \0I \0guess, \0 \0"
	    "that \0now \0it's \0time \0to \0eat.\n\0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
