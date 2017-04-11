//5 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "+'clat's Schelling Eve's coral "
    "Yalu's leap floodlit electrocutions "
    "accompanists Katrina's tromping Jewell "
    "gumdrop Gil might's bronco "
    "heterosexuals Sargent cogently cattily "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.260;
  music.stacatto_factor= 0.604;
  try {
    while (!ScanKey) {
      Play("4etAAA5CCC4A5EC4qA5sDD4A5DeEDDC4qB",
	    "Grapes\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "Mun\0ching\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("etAAA5CCC4A5EC4qA5sDD4A5DeEDDC4qB",
	    "Crapes\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "Bun\0ching\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("etG5FEeEsGGFFeG4B5CC4B5sECEC4eG5CqE",
	    "Two \0 \0 \0 \0hands\0 \0 \0 \0ful\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4etG5FEeEsGGFFeG4B5CC4B5sECEC4eG5GqE",
	    "Most \0 \0 \0 \0dole\0 \0 \0 \0ful\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
