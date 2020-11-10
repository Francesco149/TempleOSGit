//0 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "beguile inserted distracted duties "
    "narrowness possible history drunken "
    "cultivating jest sun liker "
    "pylades careless copyright few "
    "sakes unemployed notes is "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("5qE4eB5sDC4eBB5qDM5/4",
		"Fly\0ing \0 \0 \0 \0 \0 \0");
	Play("DeEEDEsFDqtRM4/4",
		" \0on \0 \0the \0 \0lev\0el.\n\0 \0");
	Play("qE4eB5sDC4eBB5qDM5/4",
		"Ha\0lo \0 \0 \0lost -- \0 \0 \0");
	Play("DeEEDEqDeRM4/4",
		" \0golf\0ing \0 \0dev\0el.\n\0 \0");
	Play("qFeDCsD4A5D4A5eDCqD4A5eC4G5etCD4B");
	Play("5qFeDCsD4A5D4A5eDCqD4A5eC4G5etCD4B");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
