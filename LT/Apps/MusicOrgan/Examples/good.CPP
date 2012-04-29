#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x00000033
#define __BIN_1 "\x08\x02\x00\x00\x00\x06\x05\x00\x00\x00\x44\x00\x00\x00\x03\x00\x00\x00\x4B\x00\x00\x00\x03\x00\x00\x00\x51\x00\x00\x00\x03\x00\x00\x00\x59\x00\x00\x00\x03\x00\x00\x00\x61\x00\x00\x00\x03\x00\x00\x00\x00"
#define __BIN_2_TYPE 0x00000002
#define __BIN_2_SIZE 0x00000033
#define __BIN_2 "\x08\x02\x00\x00\x00\x06\x05\x00\x00\x00\x45\x00\x00\x00\xFD\xFF\xFF\xFF\x4B\x00\x00\x00\x03\x00\x00\x00\x54\x00\x00\x00\x03\x00\x00\x00\x5C\x00\x00\x00\x03\x00\x00\x00\x62\x00\x00\x00\xFD\xFF\xFF\xFF\x00"

//8 has graphics


/* <1> <1> (image) */


/* <2> <2> (image) */

F64 t0=tM;

U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  nounusedwarn task;
  F64 dt=(tM-t0)*music_tempo,tt;
  U8 *inter;
  if (dt<16.0)
    tt=0;
  else if (dt<18.0)
    tt=((dt-16)/2)%1;
  else
    tt=1.0;
  inter=GrSpriteInterpolate(__BIN_1,__BIN_2,tt);
  GrSpritePlot3(dc,180,40,0,inter);
  Free(inter);
}

U0 Song()
{
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=3.0;
  try {
    while (!ScanKey) {
      t0=tM;
      LtfClear;
      Play("3qFeRRqAeRAqGDetBACeCB",
	  "God \0 \0 \0is \0 \0a \0good \0God.  \0(Thank \0 \0 \0God!!)\n\0 \0");
      Play("qFeRRqAeRAqGDetBACeCB",
	  "God \0 \0 \0is \0 \0a \0good \0God.  \0(Thank \0 \0 \0God!!)\n\0 \0");
      Play("qGsCECEeC2GqR3FeEDqEF",
	  " \0 \0 \0 \0 \0 \0 \0 \0He \0is \0a \0good \0God.\n\0");
      Play("GsCECEeC2GqR3FeEDqEF",
	  " \0 \0 \0 \0 \0 \0 \0 \0He \0is \0a \0good \0God.\n\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
