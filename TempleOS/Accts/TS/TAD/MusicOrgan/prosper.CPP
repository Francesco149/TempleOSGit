//9 has graphics

//When using flood-fill in a graphic
//element, you probably want to
//convert it to a bitmap using the
//feature in the <CTRL-R> menu.

   /* <1> <1> (image) */


   /* <2> <2> (image) */



F64 t0=tMBeat;

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 dt=tMBeat-t0;
  dc->flags|=DCF_TRANSFORMATION;
  GrRotZEqu(dc->r,0.25*Sin(2*pi*tM));
  GrScaleMatEqu(dc->r,1.5+0.5*Sin(tM));
  GrSetRotMat(dc,dc->r);
  if (Blink(0.5))
    SpritePlot3b(dc,12.0*dt%task->win_pixel_width,20,0,__BIN_1);
  else
    SpritePlot3b(dc,12.0*dt%task->win_pixel_width,20,0,__BIN_2);
}

U0 Song()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+YELLOW;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=3.5;
  t0=tMBeat;
  try {
    while (!ScanKey) {
      Play("3qG2G3DBsDCDCqRCG",
	  "$CL$$BG,9$\n\n$BG,2$$CM-LE,3$$FG,14$Baa, \0the \0grass \0is \0green.\n\0 \0 \0 \0 \0 \0 \0");
      Play("G2G3DBsDCDCqRCG",
	  "This \0must \0be \0a \0dream.\n\0 \0 \0 \0 \0 \0 \0");
      Play("EeGFqBBDA2eGGqR",
	  "Thanks, \0my \0 \0shep\0herd.  \0You \0are \0good.\n\0 \0 \0");
      Play("3EeGFqBBDA2eGGqR",
	  "Thanks, \0my \0 \0shep\0herd.  \0You \0are \0good.\n\0 \0 \0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
