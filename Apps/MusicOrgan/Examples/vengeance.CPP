//8 has graphics










     /* <1> <1> (image) */




F64 t0=0;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0-6.0;
  if (t0 && 0<=dt<1.0) {
    dc->flags|=DCF_TRANSFORMATION;
    GrRotZEqu(dc->r,5.0*(dt-0.5));
    GrRotXEqu(dc->r,1.5-2.0*dt);
    GrSetRotMat(dc,dc->r);
    SpritePlot3B(dc,200,55,0,__BIN_1);
  }
}

U0 Song()
{
  CTask *task;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    task=PopUpViewStr(
	"[C:/Misc/Bible.TXT.Z,90914] Romans,12:19\n"
	"[C:/Misc/Bible.TXT.Z,76908] Matthew,5:38\n"
	);
    TaskWait(task);
    SetWinHorz(Fs->win_left,Fs->win_right,task);
    SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("etFDAqDqAqAqDqCeEGeDC",
	  "Leave \0it \0to \0God.\n\0");
      Play("etFDAqDqAqAqDqCeEGeDC",
	  "Leave \0it \0to \0God.\n\0");
      t0=Beat;
      Play("qDqFqGqCqEqCetDGAeAA",
	  " \0 \0 \0 \0Stand \0back.\n\0");
      t0=Beat;
      Play("qDqFqGqCqEqCetDGAeAA",
	  " \0 \0 \0 \0Stand \0back.\n\0");
      t0=0;
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
