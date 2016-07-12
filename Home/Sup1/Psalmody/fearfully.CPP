//5 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 1.710;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3eDsEDeEsFEqGeFEqCDhA",
	    "God \0 \0 \0puts \0His \0 \0hand \0"
	    " \0u\0pon \0my \0soul.\n\0");
      Play("eFsFEGF4eA3qGeEFhDD",
	    "Lord, \0 \0 \0res\0cue \0 \0me \0from \0this \0Shoel.\n\0 \0");
      Play("eDsEDeEsFEqGeFEqCDhA",
	    "God \0 \0 \0puts \0His \0 \0hand \0"
	    " \0u\0pon \0my \0soul.\n\0");
      Play("eFsFEGF4eA3qGeEFhDD",
	    "Does \0 \0 \0He \0 \0 \0know \0my \0 \0woe?\n\0 \0");
      Play("qEsBCeBsGAGADCDC2eGG3sGD2eG3FsAEBCeB",
	    "God \0 \0 \0 \0have \0 \0You... \0"
	    " \0 \0 \0 \0 \0 \0 \0for\0 \0 \0got\0ten \0 \0me?\n\0 \0 \0");
      Play("qEsBCeBsGAGADCDC2eGG3sGD2eG3FsAEBCeB",
	    "Satan \0 \0 \0 \0has \0 \0come \0"
	    " \0 \0 \0 \0 \0 \0 \0and \0 \0 \0fought\0en \0 \0me.\n\0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
