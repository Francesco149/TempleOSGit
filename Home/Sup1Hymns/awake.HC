//5 has graphics
#define R1	13
#define R2	21

U0 DrawIt(CTask *,CDC *dc)
{
  static F64 theta=0;
  I64 x1=200,y1=20,x2=x1,y2=y1+R2;
  F64 tt=Beat%16.0,phi;
  theta+=1/(tt*tt+1);
  dc->color=BLACK;
  dc->thick=4;
  GrCircle3(dc,x2,y2,0,R2+10);
  for (phi=theta;phi<theta+2*pi;phi+=pi/3) {
    dc->color=BLACK;
    dc->thick=2;
    GrCircle3(dc,x2+R2*Cos(phi),y2-R2*Sin(phi),0,8);
    dc->color=WHITE;
    dc->thick=2;
    GrCircle3(dc,x2+(R2+15)*Cos(phi+pi/6),y2-(R2+15)*Sin(phi+pi/6),0,3);
  }
  dc->color=BROWN;
  GrFloodFill(dc,x2+R2*Cos(phi),y2-R2*Sin(phi));
  dc->color=DKGRAY;
  GrFloodFill(dc,x2,y2);
  dc->thick=6;
  dc->color=RED;
  GrCircle3(dc,x1,y1,0,R1-6);
  dc->color=DKGRAY;
  GrCircle3(dc,x1,y1,0,R1-2);
  dc->color=LTGRAY;
  GrRect(dc,x1-2,y1-R2-1,4,10);
  GrCircle3(dc,x1,y1,0,R1);
  dc->color=RED;
  GrFloodFill(dc,x1,y1,TRUE);
  dc->color=BLACK;
  GrFloodFill(dc,x1,y1);
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    task=PopUpViewPrint("[C:/Misc/Bible.TXT.Z,79970] Mark,14:37\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      "$GREEN$Jesus talking...$FG$\n";
      Play("4eB5C4qBsG5D4G5DetCGFsC4B5C4B5etCG4BeB5G4sB5C4B5C",
	    "Stay \0a\0wake.\n\0 \0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4eB5C4qBsG5D4G5DetCGFsC4B5C4B5etCG4BeB5G4sB5C4B5C",
	    "Stay \0a\0wake.\n\0 \0 \0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5qDsD4B5D4BetAG5G4qBGsA5D4A5DqDetGFD",
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "I \0 \0am \0a \0dead \0 \0man.\n\0");
      Play("5qDsD4B5D4BetAG5G4qBGsA5D4A5DqDetGFD",
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0"
	    "I \0 \0am \0a \0dead \0 \0man.\n\0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
