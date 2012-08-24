//7 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	Play("W1!!3qA!eFGsF!FFFqC2G!3etG!!DB!qC2eG3D",
		" \0Serious\0ly, \0God!\n\0 \0 \0 \0 \0 \0W\0T\0 \0F?\n\0 \0 \0");
	Play("!!qA!eFGsF!FFFqC2G!3etG!!DB!qC2eG3D",
		" \0Serious\0ly, \0God!\n\0 \0 \0 \0 \0 \0W\0T\0 \0F?\n\0 \0 \0");
	Play("qAACEeGB!qD!!eCEqB",
		"You \0stay \0hid\0den?\n\0(Shakes \0my \0head.)\n\0(Shakes \0my \0head.)\n\0");
	Play("AACEeGB!qD!!eCEqB",
		"You \0stay \0hid\0den?\n\0(Shakes \0my \0head.)\n\0(Shakes \0my \0head.)\n\0");
  }
  } catch
    CatchAll;
  Snd(0);
}

Song;
