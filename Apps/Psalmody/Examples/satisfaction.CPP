//7 has graphics
//Melody by Terry A. Davis




/* <1> <1> (image) */





/* <2> <2> (image) */



/* <3> <3> (image) */


F64 t0=Beat;

#define PHASE1	9.5
#define PHASE2	10.0
#define PHASE3	10.5
#define PHASE4	16.0
#define PHASE5	16.5

#define MAN_X	250
#define MAN_Y	30

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0;
  CSprite *temps;
  if (dt<PHASE1)
    Sprite3(dc,MAN_X,MAN_Y,0,__BIN_1);
  else if (dt<PHASE2) {
    temps=SpriteInterpolate(__BIN_1,__BIN_2,(dt-PHASE1)/(PHASE2-PHASE1));
    Sprite3(dc,MAN_X,MAN_Y,0,temps);
    Free(temps);
  } else if (dt<PHASE3) {
    temps=SpriteInterpolate(__BIN_2,__BIN_3,(dt-PHASE2)/(PHASE3-PHASE2));
    Sprite3(dc,MAN_X,MAN_Y,0,temps);
    Free(temps);
  } else if (dt<PHASE4)
    Sprite3(dc,MAN_X,MAN_Y,0,__BIN_3);
  else if (dt<PHASE5) {
    temps=SpriteInterpolate(__BIN_3,__BIN_1,(dt-PHASE4)/(PHASE5-PHASE4));
    Sprite3(dc,MAN_X-100*(dt-PHASE4),MAN_Y-5*(dt-PHASE4),0,temps);
    Free(temps);
  } else
    Sprite3(dc,MAN_X-100*(PHASE5-PHASE4),MAN_Y-5*(PHASE5-PHASE4),0,__BIN_1);
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  MusicSettingsRst;
  music.tempo= 2.350;
  music.stacatto_factor= 0.900;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,56267] Ecclesiastes,2:24\n"
	  "[C:/Misc/Bible.TXT.Z,56267] Ecclesiastes,3:13\n"
	  );
    TaskWait(task);
    WinHorz(Fs->win_left,Fs->win_right,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    Fs->draw_it=&DrawIt;
    Fs->task_end_cb=&SndTaskEndCB;
    while (!ScanKey) {
      t0=Beat;
      Play("M5/43qEAeDDCCqBEAeDD",
	    "Up \0a\0bove \0He \0gen\0tly \0smiles\n\0as \0we \0meet \0and \0");
      Play("CCqBFBeEEDDqCF",
	    "con\0quer \0trials.\n\0$RED$You \0can \0do \0it \0lit\0tle \0one.\n\0With\0");
      Play("BeEEDDqC",
	    "out \0work \0there \0is \0no \0fun.\n\0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
