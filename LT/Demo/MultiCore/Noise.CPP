BoolI8 noise_done;
I64 done_ack;
I64 cnt;

U0 DrawNoise(TaskStruct *task)
{
  I64 i,
    h=task->win_pixel_width,
    v=task->win_pixel_height;
  GrDC *dc=GrDCAlias(gr_dc2,task);
  for (i=0;i<cnt;i++) {
    dc->color=RandU16&15;
    GrPlot(dc,RandU16%h,RandU16%v);
  }
  dc->color=WHITE;
  GrRect(dc,0,Gs->num*FONT_HEIGHT,12*FONT_WIDTH,FONT_HEIGHT);
  dc->color=BLACK;
  GrPrintF(dc,0,Gs->num*FONT_HEIGHT,"Cnt:%08X",cnt);
  GrDCDel(dc);
}

U0 MPTask(TaskStruct *task)
{
  while (!noise_done) {
    DrawNoise(task);
    WinSync;
  }
  LBts(&done_ack,Gs->num);
}

U0 NoiseDemo()
{
  I64 i;
  F64 t0;
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  WordStat(OFF);
  cnt=0x1000;

  noise_done=FALSE;
  for (i=1;i<mp_cnt;i++)
    MPSpawn(&MPTask,Fs,"Job",1<<i);

  while (!ScanChar) {
    t0=tNP;
    DrawNoise(Fs);
    if (tNP-t0>1.0/80.0)
      cnt-=0x100;
    else
      cnt+=0x100;
    WinSync;
  }

  done_ack=1;
  noise_done=TRUE;
  while (done_ack!=1<<mp_cnt-1)
    Yield;

  GrDCClear;
  SettingsPop;
}

NoiseDemo;
