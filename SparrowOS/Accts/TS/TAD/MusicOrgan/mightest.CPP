//8 has graphics

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 x1,x2,y1,y2,x3,y3,radius,pedals,theta,t=tMBeat,
    cx=335,cy=task->win_pixel_height>>1;
  Bool first=TRUE;

  radius=cy*(1.0+Sin(2*pi*t));
  pedals=8;

  dc->color=BLACK;
  for (theta=0;theta<2*pi;theta+=2*pi/100) {
    P2R(&x2,&y2,radius*Abs(Sin(pedals/2*theta))+5,theta+t);
    if (!first)
      GrLine(dc,x1+cx,cy-y1,x2+cx,cy-y2);
    else {
      first=FALSE;
      x3=x2; y3=y2;
    }
    x1=x2; y1=y2;
  }
  GrLine(dc,x1+cx,cy-y1,x3+cx,cy-y3);

  dc->color=(2*t)%16;
  GrFloodFill(dc,cx,cy);
}

U0 Song()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  music_tempo=2.5;
  try {
    while (TRUE) {
      Play("qBq2G3sGAGAetCBBqAq2G3sBBBBeFE",
      "I \0am \0feeling \0 \0 \0 \0 \0 \0brave.\n\0");
      Play("qBq2G3sGAGAetCBBqAq2G3sBBBBeFE",
      "I \0am \0feeling \0 \0 \0 \0 \0 \0brave.\n\0");
      Play("qCsDEDEq2G3qDqBqAqGq2G3",
      "Test \0me \0 \0 \0 \0Lord. \0See \0if \0I'm \0wor\0thy.\n\0");
      Play("qCsDEDEq2G3qDqBqAqGq2G3",
      "Test \0me \0 \0 \0 \0Lord. \0See \0if \0I'm \0wor\0thy.\n\0");
    }
  } catch
    CatchAll;
  SettingsPop;
}

Song;
