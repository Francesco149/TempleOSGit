//0 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "fence windy mount hang "
    "reminded balneum enquire mightily "
    "having committed soothed pronouncing "
    "ebbing collected presented print "
    "offices acting grounded consuming "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("M5/4");
	Play("5hC4sA5CeDhCM4/4",
		"Can't \0be \0said \0too \0lou\0");
	Play("4h.AqRM5/4",
		"d.\n\0 \0");
	Play("5hC4sA5CeDhCM4/4",
		"Nerds \0will \0milk \0the \0cow\0");
	Play("4h.AqR",
		"ed?\n\0 \0");
	Play("G5C4BG",
		"The \0me\0chan\0i\0");
	Play("M5/4");
	Play("5ED4eG5EqC(4GM4/4",
		"cal \0 \0at\0 \0ti\0tude\n\0");
	Play("G5C4BGM5/4",
		" \0Is \0mor\0bid\0");
	Play("5ED4eG5EqC4G",
		"ly \0 \0fucked-\0 \0up, \0dude.\n\0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
