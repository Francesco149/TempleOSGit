//5 no nothing
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("6etEEE5qB6etDDqtD5qB6etCCqtC5qBhB");
      Play("6etEEE5qB6etDDqtD5qB6etCCqtC5qBhB");
      Play("5etAB6CqD5etB6CCqDetDC5BqAhA");
      Play("5etAB6CqD5etB6CCqDetDC5BqAhA");
      Play("5qA6etC5A6EsGCGC5etB6C5GA6D5AqA6GetD5A6D");
      Play("5qA6etC5A6EsGCGC5etB6C5GA6D5AqA6GetD5A6D");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
