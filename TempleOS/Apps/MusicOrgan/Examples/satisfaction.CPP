//7 has graphics
//Melody by Terry A. Davis






/* <1> <1> (image) */







/* <2> <2> (image) */









/* <3> <3> (image) */






F64 t0=tMBeat;

#define PHASE1	9.5
#define PHASE2	10.0
#define PHASE3	10.5
#define PHASE4	16.0
#define PHASE5	16.5

#define MAN_X	250
#define MAN_Y	30

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=tMBeat-t0;
  U8 *inter;
	 if (dt<PHASE1)
    SpritePlot3(dc,MAN_X,MAN_Y,0,__BIN_1);
  else if (dt<PHASE2) {
    inter=SpriteInterpolate(__BIN_1,__BIN_2,(dt-PHASE1)/(PHASE2-PHASE1));
    SpritePlot3(dc,MAN_X,MAN_Y,0,inter);
    Free(inter);
  } else if (dt<PHASE3) {
    inter=SpriteInterpolate(__BIN_2,__BIN_3,(dt-PHASE2)/(PHASE3-PHASE2));
    SpritePlot3(dc,MAN_X,MAN_Y,0,inter);
    Free(inter);
  } else if (dt<PHASE4)
    SpritePlot3(dc,MAN_X,MAN_Y,0,__BIN_3);
  else if (dt<PHASE5) {
    inter=SpriteInterpolate(__BIN_3,__BIN_1,(dt-PHASE4)/(PHASE5-PHASE4));
    SpritePlot3(dc,MAN_X-100*(dt-PHASE4),MAN_Y-5*(dt-PHASE4),0,inter);
    Free(inter);
  } else
    SpritePlot3(dc,MAN_X-100*(PHASE5-PHASE4),MAN_Y-5*(PHASE5-PHASE4),0,__BIN_1);
}

U0 Song()
{
  CTask *task;
  MusicSettingsRst;
  music_tempo= 2.350;
  music_stacatto_factor= 0.900;
  try {
    task=PopUpViewStr(
    "[C:/TempleOS/Misc/Bible.TXT.Z,56267] Ecclesiastes,2:24\n"
	"[C:/TempleOS/Misc/Bible.TXT.Z,56267] Ecclesiastes,3:13\n"
	);
    TaskWait(task);
    SetWinHorz(Fs->win_left,Fs->win_right,task);
    SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
    Fs->draw_it=&DrawIt;
    Fs->task_end_cb=&SndTaskEndCB;
    while (!ScanKey) {
      t0=tMBeat;
      Play("M5/4W13qEAeDDCCqBEAeDD",
	  "Up \0a\0bove \0He \0gen\0tly \0smiles\n\0as \0we \0meet \0and \0");
      Play("CCqBFBeEEDDqCF",
	  "con\0quer \0trials.\n\0$FG,RED$You \0can \0do \0it \0lit\0tle \0one.\n\0With\0");
      Play("BeEEDDqC",
	  "out \0work \0there \0is \0no \0fun.\n\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
