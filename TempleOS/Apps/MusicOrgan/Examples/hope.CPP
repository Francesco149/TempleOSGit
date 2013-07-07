//9 has graphics





	      /* <1> <1> (image) */




F64 t0=tM;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,x,y;
  x=30+50*((tM-t0)%3);
  y=40;
  SpritePlot3(dc,x,y,0,__BIN_1);
  dc->color=YELLOW;
  for (i=0;i<5;i++)
    GrLine(dc,x,y,x+RandI16%100,y+RandI16%50+50);
  dc->color=WHITE;
  for (i=0;i<5;i++)
    GrLine(dc,x,y,x+RandI16%100,y+RandI16%50+50);
}

U0 Song()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=LTCYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    "$BG+H,LTCYAN$$FG+H,BLACK$";
    while (!ScanKey) {
      t0=tM;
      DocClear;
      Play("3qBeB2GqG3DBeA2GqR3etGED",
	  "Sees \0the \0 \0end \0from \0the \0start.\n\0 \0 \0 \0 \0 \0");
      Play("qBeB2GqG3DBeA2GqR3etGED",
	  "Got \0Heav\0 \0en \0down \0to \0art.\n\0 \0 \0 \0 \0When \0");
      Play("eB2G3qGRsFEFECBCB2qGG3etDRD",
	  "filled \0with \0doubt.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0It'\0 \0ll \0");
      Play("eB2G3qGRsFEFECBCB2qGG3etDRD",
	  "all \0work \0out.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
