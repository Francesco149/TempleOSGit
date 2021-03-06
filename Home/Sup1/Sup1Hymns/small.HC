//5 has graphics


/* <1> <1> (image) */




/* <2> <2> (image) */





/* <3> <3> (image) */





/* <4> <4> (image) */





/* <5> <5> (image) */







/* <6> <6> (image) */


#define FRAMES 8

U8 *imgs[FRAMES]={
  __BIN_1,__BIN_2,__BIN_3,__BIN_4,
  __BIN_5,__BIN_4,__BIN_3,__BIN_1
};

F64 t0=Beat;

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *tmps;
  F64 tt=Beat,dt=Beat-t0;
  tmps=SpriteInterpolate(tt%1.0,imgs[tt%FRAMES],imgs[(tt+1)%FRAMES]);
  Sprite3(dc,180,40,0,tmps);
  Free(tmps);
  if (dt>24.0)
    Sprite3(dc,task->pix_width-12.0*(dt-24.0),40,0,__BIN_6);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      t0=Beat;
      DocClear;
      Play("qR5sDEDE4eGR5etGG4A5qEetDG4BsAGAGqR",
	    " \0What \0 \0seem \0 \0large, \0 \0for \0 \0 \0"
	    "God \0is \0quite \0small.\n\0 \0 \0 \0 \0 \0");
      Play("R5sDEDE4eGR5etGG4A5qEetDG4BsAGAGqR",
	    " \0Hea\0ven\0ly \0 \0host \0 \0waits \0 \0 \0 \0"
	    "at \0His \0call.\n\0 \0 \0 \0 \0 \0");
      Play("5EsFCFCeG4G5qDC4etB5CG4sG5C4G5CqG",
	    "So, \0do \0 \0what's \0 \0in \0your \0"
	    "pow\0er.\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5EsFCFCeG4G5qDC4etB5CG4sG5C4G5CqG",
	    " \0Then, \0 \0call \0 \0in \0"
	    "your \0ho\0ur.\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
