//0 has words
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("3q.BeCqDBCeBCsG4A3FGeEE");
      Play("FAqGeFAsGFGFq.BeCqDB");
      Play("CeBCsG4A3FGeEGFAqGeFAsGFGF");
      Play("eG2G3sDFeGqBeAAEEqE2G3eCsBA",
	    "Lis\0ten \0to \0the \0 \0crowd... \0 \0 \0roar!\n\0"
	    " \0 \0 \0 \0 \0 \0");
      Play("eG2G3sDFeGqBeAAEEqE2G3eCsBA",
	    "Lis\0ten \0to \0the \0 \0crowd... \0 \0 \0roar!\n\0"
	    " \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
