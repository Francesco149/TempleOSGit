//9 has graphics


/* <1> <1> (image) */





/* <2> <2> (image) */






/* <3> <3> (image) */





/* <4> <4> (image) */






/* <5> <5> (image) */










/* <6> <6> (image) */




#define MAN_FRAMES 8

U8 *man_imgs[MAN_FRAMES]=
{
__BIN_1,__BIN_2,__BIN_3,__BIN_4,
__BIN_5,__BIN_4,__BIN_3,__BIN_1
};

F64 t0=tMBeat;

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *inter;
  F64 tt=4.0*tM,dt=tMBeat-t0;
  inter=CSpriteInterpolate(man_imgs[tt%MAN_FRAMES],man_imgs[(tt+1)%MAN_FRAMES],tt%1.0);
  CSpritePlot3(dc,180,40,0,inter);
  Free(inter);
  if (dt>24.0)
    CSpritePlot3(dc,task->win_pixel_width-12.0*(dt-24.0),40,0,__BIN_6);
}

U0 Song()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    while (!ScanKey) {
      t0=tMBeat;
      DocClear;
      Play("qR3sDEDE2eGR3etGGAqEetDGBsA2G3A2GqR",
	  " \0What \0 \0seem \0 \0large, \0 \0for \0 \0 \0God \0is \0quite \0small.\n\0 \0 \0 \0 \0 \0");
      Play("R3sDEDE2eGR3etGGAqEetDGBsA2G3A2GqR",
	  " \0Hea\0ven\0ly \0 \0host \0 \0waits \0 \0 \0 \0at \0His \0call.\n\0 \0 \0 \0 \0 \0");
      Play("3EsFCFCeG2G3qDCetBCG2sG3C2G3CqG",
	  "So, \0do \0 \0what's \0 \0in \0your \0pow\0er.\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("EsFCFCeG2G3qDCetBCG2sG3C2G3CqG",
	  " \0Then, \0 \0call \0 \0in \0your \0ho\0ur.\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
