//5 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
	Play("5qCeEEqFDE4B5DC",
		"Shade \0my \0 \0eyes \0the \0noon \0ap\0proach\0es.\n\0");
	Play("5eAAAGAGFGEFEDhC");
	Play("4qB5GF4BG5eGDqGeFG",
		"Look \0a\0way.\n\0 \0 \0Oy!\n\0 \0 \0 \0 \0");
	Play("4qB5GF4BG5eGDqGeFG",
		"Hot \0to\0day!\n\0 \0 \0Oy!\n\0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
