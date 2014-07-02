//8 has graphics
/* <1> <1> (image) */






U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  music.tempo=2.5;
  try {
  while (!ScanKey) {
    Play("3eDGqGeEGEBqAAeEBetCFB",
	"Thank \0 \0You, \0God \0 \0for \0a\0bun\0 \0dance.\n\0 \0 \0 \0 \0");
    Sprite(__BIN_1);
    "%h8c",'\n';
    Play("eDGqGeEGEBqAAeEBetCFB",
	"Thank \0 \0You, \0God \0 \0for \0a\0bun\0 \0dance.\n\0 \0 \0 \0 \0");
    Sprite(__BIN_1);
    "%h7c",'\n';
    Play("BBBqFeAGqDeBDDFDCqE",
	" \0 \0 \0Well, \0I \0guess, \0 \0that \0now \0it's \0time \0to \0eat.\n\0 \0");
    Play("etBBBqFeAGqDeBDDFDCqE",
	    " \0 \0 \0Well, \0I \0guess, \0 \0that \0now \0it's \0time \0to \0eat.\n\0 \0");
  }
  } catch
    PutExcept;
  Snd(0);
}

Song;
