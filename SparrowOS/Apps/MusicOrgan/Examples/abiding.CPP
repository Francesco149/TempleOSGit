//9 has graphics

#define X	330
#define Y	15

#define NUM_BEAMS	256

F64 beams_r[NUM_BEAMS],beams_theta[NUM_BEAMS];

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i;
  F64 r,x1,y1,x2,y2;
  dc->color=YELLOW;
  GrCircle(dc,X,Y,10);
  GrFloodFill(dc,X,Y,10);
  dc->color=BROWN;
  GrCircle(dc,X,Y,10);
  dc->color=YELLOW;
  for (i=0;i<NUM_BEAMS;i++) {
    r=125.0*((0.25*tMBeat+beams_r[i])%1.0);
    P2R(&x1,&y1,r     ,beams_theta[i]);
    P2R(&x2,&y2,r+10.0,beams_theta[i]);
    GrLine(dc,x1+X,y1+Y,x2+X,y2+Y);
  }
}

U0 Song()
{
  CTask *task;
  I64 i;
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music_tempo= 2.480;
  music_stacatto_factor= 0.902;
  for (i=0;i<NUM_BEAMS;i++) {
    beams_r[i]=Rand;
    beams_theta[i]=2*pi*Rand;
  }
  try {
    task=PopUpViewStr(
    "[::/SparrowOS/Misc/Bible.TXT.Z,12005] Numbers,6:25\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,31:16\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,67:1\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,80:3\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,80:19\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,46190] Psalms,119:135\n"
    "[::/SparrowOS/Misc/Bible.TXT.Z,71804] Daniel,9:17\n"
    );
    TaskWait(task);
    task->win_right=Fs->win_right;
    task->win_left=Fs->win_left;
    task->win_top=Fs->win_bottom+3;
    task->win_bottom=task->win_top+12;
    TaskDerivedValsUpdate(task);
    "$BD+H,LTCYAN$$FD+H,BLACK$";
    while (!ScanKey) {
      "$FG,WHITE$To God...\n$FG$";
      Play("W13etCFE!!sDDDDD2G!3D2G!!3qC2eG3EqEetAA!F2qG",
      "I \0am \0a\0biding \0 \0 \0 \0in \0 \0Your \0 \0presence.\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("3etCFE!!sDDDDD2G!3D2G!!3qC2eG3EqEetAA!F2qG",
      "I \0am \0a\0biding \0 \0 \0 \0in \0 \0Your \0 \0presence.\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("!3DBsFGF!G2eG!!3FRDqEGeB!E",
      "What \0a \0joy!\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("!qDBsFGF!G2eG!!3FRDqEGeB!E",
      "I'm \0so \0coy!\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
    } catch
      CatchAll;
  SettingsPop;
}

Song;
