//7 has graphics


/* <1> <1> (image) */








U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  try {
  while (!ScanKey) {
	PutSprite(__BIN_1);
	NewLine(6);
	"$FG,GREEN$To God...\n$FG$";
	Play("3etD2GRsR3GRGqFetEDCCRCRCB2qGsG3A2G3A",
		"O\0kay, \0 \0 \0what \0 \0is \0it \0You \0 \0 \0want \0 \0us \0 \0to \0 \0do?\n\0 \0 \0 \0 \0");
	Play("etD2GRsR3GRGqFetEDCCRCRCB2qGsG3A2G3A",
		" \0 \0 \0 \0Let \0 \0me \0guess, \0 \0 \0 \0we \0 \0are \0 \0to \0 \0choose.\n\0 \0 \0 \0 \0");
	Play("eBFqDCeBBRGetERFqGE",
		" \0 \0 \0 \0 \0 \0 \0I'm \0so \0 \0con\0fused.\n\0 \0");
	Play("eBFqDCeBBRGetERFqGE",
		" \0 \0 \0 \0 \0 \0 \0I \0must \0 \0have \0snoozed.\n\0 \0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
