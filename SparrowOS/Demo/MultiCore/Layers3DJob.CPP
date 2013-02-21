



/* <1> <1> (image) */




class MPCtrl {
  Bool core0_done,core1_done;
} mp;


I64 *r1,*r2;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 *old_r;

  if (mp.core0_done) return;
  old_r=dc->r;
  dc->color=BLACK;
  GrPutS(dc,0,0,"This is not art.");
  GrPutS(dc,0,FONT_HEIGHT,"It's just a test pattern.");

  GrSetRotMat(dc,r1);
  dc->x=200;
  dc->y=200;
  dc->flags|=DCF_TRANSFORMATION;

  dc->pen_width=2;
  SpritePlot3(dc,0,0,0,__BIN_1);

  dc->color=BLUE;
  GrRect3(dc,50,75,0,100,50);
  GrRect3(dc,75,50,0,50,100);
  dc->color=GREEN;
  GrEllipse3(dc,100,100,0,50,25);
  GrEllipse3(dc,100,100,0,50,25,pi/2);
  dc->color=LTGREEN;
  GrEllipse3(dc,100,100,0,50,25,pi/6);
  GrEllipse3(dc,100,100,0,50,25,pi/3);

  GrSetRotMat(dc,old_r);
  mp.core0_done=TRUE;
}


U0 MPDrawIt(CTask *main_task)
{
  CDC *dc;
  I64 *old_r;

  if (mp.core1_done) return;

  dc=DCAlias;
  old_r=dc->r;
  dc->win_task=main_task;
  GrSetRotMat(dc,r2);
  dc->x=200;
  dc->y=200;
  dc->flags|=DCF_TRANSFORMATION;

  DCFill(dc);

  dc->pen_width=2;
  SpritePlot3(dc,0,0,0,__BIN_1);

  dc->color=BLUE;
  GrRect3(dc,50,75,0,100,50);
  GrRect3(dc,75,50,0,50,100);

  dc->color=GREEN;
  GrEllipse3(dc,100,100,0,50,25);
  GrEllipse3(dc,100,100,0,50,25,pi/2);

  dc->color=LTGREEN;
  GrEllipse3(dc,100,100,0,50,25,pi/6);
  GrEllipse3(dc,100,100,0,50,25,pi/3);

  GrSetRotMat(dc,old_r);
  DCDel(dc);
  mp.core1_done=TRUE;
}

U0 Layers3DJob()
{
  F64 theta2=0,phi2=0,theta3=0,phi3=0;
  mp.core0_done=TRUE;
  mp.core1_done=TRUE;
  r1=GrIdent;
  r2=GrIdent;

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  DocClear;
  Fs->draw_it=&DrawIt;
  Preempt(OFF); //Don't access r1,r2 while being changed

  while (!ScanChar) {
    GrIdentEqu(r1);
    GrRotXEqu(r1,phi2);
    GrRotZEqu(r1,theta2);

    GrIdentEqu(r2);
    GrRotXEqu(r2,phi3);
    GrRotZEqu(r2,theta3);

    mp.core0_done=FALSE;
    mp.core1_done=FALSE;

    //Pass the task of the window as data item to job.
    MPJob(&MPDrawIt,Fs,1);

    WinMgrSync;
    while (!mp.core0_done || !mp.core1_done)
      Yield;

    theta2+=2*pi/100;
    phi2	+=2*pi/130;
    theta3-=2*pi/100;
    phi3	-=2*pi/130;
  }
  DCFill;

  SettingsPop;
  Free(r1);
  Free(r2);
}

Layers3DJob;
