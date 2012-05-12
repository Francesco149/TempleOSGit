class MPCtrl
{
  TaskStruct *win_task;
  I64 i;
  BoolI8 app_done,app_done_ack;
  BoolI8 refresh_start,refresh_done;
} mp;


U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  I64 i;
  F64 t0=tNP,t1;

  mp.refresh_done=FALSE;
  mp.refresh_start=TRUE;
  mp.i++;

  for (i=0;i<32;i++) {
    t1=t0+i*2*pi/64;
    dc->color=(mp.i+i)/10&15;
    GrEllipse(dc,GR_WIDTH/2,GR_HEIGHT/2,
      GR_WIDTH/4*(Sin(t1)+1.0),GR_HEIGHT/4*(Sin(t1)+1.0),t1*2.0);
  }
  while (!mp.refresh_done)
    Yield;
}

U0 MPDrawIt()
{
  I64 i;
  F64 t0=tNP,t1;
  GrDC *dc=GrDCAlias;
  dc->win_task=mp.win_task;

  while (!mp.refresh_start && !mp.app_done)
    Yield;
  if (mp.app_done)
    goto mp_done;
  mp.refresh_start=FALSE;
  GrDCFill;
  for (i=32;i<64;i++) {
    t1=t0+i*2*pi/64;
    dc->color=(mp.i+i)/10&15;
    GrEllipse(dc,GR_WIDTH/2,GR_HEIGHT/2,
      GR_WIDTH/4*(Sin(t1)+1.0),GR_HEIGHT/4*(Sin(t1)+1.0),t1*2.0);
  }
mp_done:
  GrDCDel(dc);
  mp.refresh_done=TRUE;
}

U0 MPTask(U64 dummy=0)
{
  nounusedwarn dummy;
  while (!mp.app_done)
    MPDrawIt;
  mp.app_done_ack=TRUE;
  GrDCFill;
}


U0 Main()
{
  if (mp_cnt<2) throw(EXCEPT_MULTICORE);

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  LtfClear;
  Fs->draw_it=&DrawIt;
  WinBorder;

  mp.i=0;
  mp.app_done=FALSE;
  mp.win_task=Fs;
  MPSpawn(&MPTask);
  GetChar;
  mp.app_done=TRUE;
  while (!mp.app_done_ack)
    Yield;
  SettingsPop;
}

Main;
