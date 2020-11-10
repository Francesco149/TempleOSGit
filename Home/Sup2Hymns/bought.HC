//2 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "coals curest amazement water "
    "crucifixion uncultivated justina wishes "
    "egypt glorious loathing roll "
    "romanianus burst observed flew "
    "trembling station befalls bodily "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 1.985;
  music.stacatto_factor= 0.648;
  try {
    while (!ScanKey) {
      Play("5sDEDE4B5C4B5C4qBsB5C4B5C4eB5sGF4eB5sGFC4Bq.A",
	    "I \0 \0 \0 \0bought \0 \0 \0 \0cot,\n\0not \0"
	    " \0 \0 \0alot,\n\0 \0 \0warms \0 \0 \0my \0 \0soul.\n\0");
      Play("5sDEDE4B5C4B5C4qBsB5C4B5C4eB5sGF4eB5sGFC4Bq.A",
	    "Monk \0 \0 \0 \0hides \0 \0 \0 \0smile,\n\0little \0"
	    " \0 \0 \0child,\n\0 \0 \0like \0 \0 \0Van \0 \0Gogh.\n\0");
      Play("5sDGDGCGCG4qG5eFECF4qAeB5CDC",
	    "I \0 \0 \0 \0must \0 \0 \0 \0watch,\n\0"
	    " \0 \0 \0 \0 \0stay \0a\0wake \0at \0");
      Play("qEeDCqEeDCqCD4hG",
	    "night,\n\0 \0 \0 \0 \0 \0o\0ver\0see.\n\0");
      Play("5sDGDGCGCG4qG5eFECF4qAeB5CDC",
	    "I \0 \0 \0 \0love \0 \0 \0 \0God.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("qEeDCqEeDCqCD4hG",
	    " \0 \0 \0 \0Shepherd \0 \0o\0ver \0me.\n\0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
