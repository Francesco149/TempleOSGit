//8 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W13eBEBEBEBEBDBDBEBE",
		"All \0rise!\n\0All \0rise!\n\0All \0rise!\n\0All \0rise!\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("BEBEBEBEBDBDBEBE");
	Play("qAAEFeGFCAqFE",
		"On \0your \0knees \0You \0enter \0to \0His \0 \0cour\0t.\n\0");
	Play("AAEFeGFCAqFE",
		"On \0your \0knees \0You \0enter \0to \0His \0 \0cour\0t.\n\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
