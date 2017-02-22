//5 has graphics




	      /* <1> <1> (image) */


F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,x,y;
  x=30+200*(Saw(Beat-t0,8.0));
  y=40;
  Sprite3(dc,x,y,0,__BIN_1);
  dc->color=YELLOW;
  for (i=0;i<5;i++)
    GrLine(dc,x,y,x+RandI16%100,y+RandI16%50+50);
  dc->color=WHITE;
  for (i=0;i<5;i++)
    GrLine(dc,x,y,x+RandI16%100,y+RandI16%50+50);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=LTCYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=2.5;
  try {
    "$BG+H,LTCYAN$$FG+H,BLACK$";
    while (!ScanKey) {
      t0=Beat;
      DocClear;
      Play("4qBeBGqG5D4BeAGqR5etGED",
	    "Sees \0the \0 \0end \0from \0the \0start.\n\0 \0 \0 \0 \0 \0");
      Play("4qBeBGqG5D4BeAGqR5etGED",
	    "Got \0Heav\0 \0en \0down \0to \0art.\n\0 \0 \0 \0 \0When \0");
      Play("4eBG5qGRsFEFEC4B5C4BqGG5etDRD",
	    "filled \0with \0doubt.\n\0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0It'\0 \0ll \0");
      Play("4eBG5qGRsFEFEC4B5C4BqGG5etDRD",
	    "all \0work \0out.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
