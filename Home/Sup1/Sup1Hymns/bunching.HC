//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.260;
  music.stacatto_factor= 0.604;
  try {
    while (!ScanKey) {
	Play("4etAAA5CCC4A5EC4qA5sDD4A5DeEDDC4qB");
	Play("4etAAA5CCC4A5EC4qA5sDD4A5DeEDDC4qB");
	Play("4etG5FEeEsGGFFeG4B5CC4B5sECEC4eG5CqE");
	Play("4etG5FEeEsGGFFeG4B5CC4B5sECEC4eG5GqE");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
