//2 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "disputed art fuller melodies "
    "solicited banished busied pictures "
    "consume horror secrets patricius "
    "breast devotion colours slept "
    "whenever before natures reposes "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.604;
  try {
    while (!ScanKey) {
	Play("4eG5DFG4BGsG5C4G5C4qB5CDhC",
		"You \0see \0the \0 \0bones?\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("4eG5DFG4BGsG5C4G5C4qB5CDhC",
		"You \0see \0the \0 \0cones?\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("FqEeDF4BA5qG4eAB5DC",
		"Rush? \0What's \0the \0 \0 \0 \0rush?\n\0 \0 \0 \0 \0");
	Play("hFqEeDF4BA5qG4eAB5DC",
		"What? \0Time \0to \0 \0 \0 \0flush.\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
