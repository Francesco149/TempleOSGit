U0 DrawIt(CTask *task,CDC *dc)
{
  SND_OUT_CONTAINER *buf=hda.ostr0_buf[0];
  I64 i,x1,y1,x2,y2,
	cy=task->pix_height>>1;

  dc->color=BLUE;
  x2=0;y2=cy;
  for (i=0;i<SND_BUF_LEN;i+=2) {
    x1=i*task->pix_width/SND_BUF_LEN;
    y1=cy-buf[i]*cy/snd_vol/I32_MAX;
    GrLine(dc,x2,y2,x1,y1);
    x2=x1;y2=y1;
  }

  dc->color=GREEN;
  x2=0;y2=cy;
  for (i=1;i<SND_BUF_LEN;i+=2) {
    x1=i*task->pix_width/SND_BUF_LEN;
    y1=cy-buf[i]*cy/snd_vol/I32_MAX;
    GrLine(dc,x2,y2,x1,y1);
    x2=x1; y2=y1;
  }
}

U0 SndScope()
{
  if (snd_dev!=SD_HD_AUDIO) {
    "Only works for HD Audio.\n";
    return;
  }
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  WinHorz(1,18);
  WinVert(15,TEXT_ROWS-2);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar;
  SettingsPop;
}

#if __CMD_LINE__
SndScope;
#endif
