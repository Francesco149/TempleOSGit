//9 has graphics

	 /* <1> <1> (image) */







#define X	330
#define Y	35

F64 t0=tMBeat;

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i;
  F64 x,y,dt=tMBeat-t0;
  SpritePlot3(dc,X,Y,0,__BIN_1);
  if (tMBeat-t0<24) {
    if (Rand<0.7) {
      dc->color=YELLOW;
      GrPlot(dc,X-12,Y+1);
      GrPlot(dc,X-12,Y+2);
      GrPlot(dc,X-11,Y+4);
      GrPlot(dc,X+10,Y+4);
      GrPlot(dc,X+4,Y+4);
      GrPlot(dc,X+5,Y+4);
      GrPlot(dc,X+6,Y+4);
      GrPlot(dc,X+7,Y+5);
      GrPlot(dc,X+8,Y+5);
    }
    for (i=0;i<512;i++) {
      x=1.0-Rand`3.0;
      y=Rand;
      x=5.0*Sin(pi*Sqrt(y))*x;
      y=25.0*y;
      if (Rand*(x*x+2.0)*Sqrt(y)<4.0) {
	if (Rand<0.5)
	  dc->color=BLUE;
	else
	  dc->color=LTBLUE;
      } else
	dc->color=YELLOW;
      if (RandI64<0)
	x=-x;
      x+=150.0*Sin(pi*dt)/(35-y)`1.5;
      GrPlot(dc,X+x,Y-y);
    }
  }
}

U0 Song()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=BLACK<<4+LTGRAY;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.0;
  try {
    "$BG+H,BLACK$$FD+H,LTGRAY$";
    while (TRUE) {
      t0=tMBeat;
      "$FG,WHITE$God said this was a dirge.\n$FG$";
      Play("FqDqDeACqEqEqDqDq",
      "Gone \0to \0sleep \0for \0the \0fi\0nal \0time.\n\0");
      Play("FqDqDeACqEqEqDqDq",
      "Gone \0to \0sleep \0for \0the \0fi\0nal \0time.\n\0");
      Play("DqGqFeGFqFqEqEqBq",
      " \0Ash \0to... \0and \0 \0dust \0to \0dust.\n\0");
      Play("DqGqFeGFqFqEqEqBq",
      " \0Ash \0to... \0and \0 \0dust \0to \0dust.\n\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
