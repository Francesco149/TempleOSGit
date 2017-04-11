//5 has graphics






	/* <1> <1> (image) */












			/* <2> <2> (image) */
 

#define WAVE_PERIOD	(20*pi)
#define X	240
#define Y	24

F64 t0=Beat;

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 dt=Beat-t0;
  I64 w=task->pix_width,h=task->pix_height,cy=h>>1+Y;
  if (dt>=24.0) {
    dc->color=BLACK;
    GrRect(dc,0,cy+2,w,h>>1);
    Sprite3(dc,X,cy,0,__BIN_2);
  } else {
    Sprite3ZB(dc,X,cy,0,__BIN_2,0.1*Sin(0.25*pi*dt));
    Sprite3(dc,WAVE_PERIOD*(Saw(2*dt,WAVE_PERIOD)-1),cy,0,__BIN_1);
  }
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=DKGRAY<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint("[C:/Misc/Bible.TXT.Z,79970]\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      t0=Beat;
      DocClear;
      "$FD,WHITE$$BG,DKGRAY$\n";
      Play("5qEsGEGEqDsCCCC4qB5sDEDEqFG",
	    "Lord, \0there's \0 \0a \0 \0storm \0u\0 \0 \0 \0"
	    "pon \0the \0 \0 \0 \0sea.\n\0 \0");
      Play("5EsGEGEqDsCCCC4qB5sDEDEqFG",
	    "Lord, \0there's \0 \0a \0 \0storm \0u\0 \0 \0 \0"
	    "pon \0the \0 \0 \0 \0sea.\n\0 \0");
      Play("5GEC4GetBG5FGGED4BBeB5E",
	    "$RED$Re\0lax \0fel\0las.$FG$\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5qGEC4GetBG5FGGED4BBeB5E",
	    "(Sea \0be\0came \0glass.)\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
