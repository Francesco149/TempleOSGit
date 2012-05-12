GrDC *dc[2]; //dbl buf
I64 cur_dc,;
BoolI8 restart;

U0 DrawIt(TaskStruct *task,GrDC *b)
{
  nounusedwarn task;
  dc[cur_dc]->flags|=DCF_NO_TRANSPARENTS;
  GrDCBlot(b,0,0,dc[cur_dc]);
}

U0 AnimateTask(U64 dummy=0)
{
  nounusedwarn dummy;
  I64 x,y,x1,y1,cnt,next_dc;
  while (TRUE) {
    next_dc=cur_dc^1;
start_over:
    restart=FALSE;
    GrDCClear(dc[next_dc]);
    for (y=1;y<Fs->parent_task->win_pixel_height-1;y++) {
      for (x=1;x<Fs->parent_task->win_pixel_width-1;x++) {
	cnt=0;
	for (y1=y-1;y1<=y+1;y1++)
	  for (x1=x-1;x1<=x+1;x1++)
	    if (GrPeek(dc[cur_dc],x1,y1)==GREEN)
	      cnt++;
	if (restart) goto start_over;
	if (GrPeek(dc[cur_dc],x,y)==GREEN) {
	  if (cnt==3) {
	    dc[next_dc]->color=GREEN;
	    GrPlot(dc[next_dc],x,y);
	  }
	} else {
//	  if (cnt==3) { //Use this for the classic rules
	  if (cnt==2) {
	    dc[next_dc]->color=GREEN;
	    GrPlot(dc[next_dc],x,y);
	  }
	}
      }
      Yield;
    }
    Sleep(50);
    cur_dc=next_dc;
  }
}

public U0 Life()
{
  I64 msg_code,cnt,x1,y1,x2,y2,p1,p2;

  dc[0]=GrDCNew(DCT_COLOR8,GR_WIDTH,GR_HEIGHT);
  dc[1]=GrDCNew(DCT_COLOR8,GR_WIDTH,GR_HEIGHT);
  cur_dc=0;
  restart=FALSE;

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  GrDCFill;
  "$CL$";

  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);
  WordStat;
  Fs->draw_it=&DrawIt;
  Fs->win_inhibit=WIF_ALL-WIF_BORDER;

  do {
    msg_code=GetMsg(&p1,&p2,
      1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN);
    switch (msg_code) {
      case MSG_IP_L_DOWN:
	x1=p1; y1=p2;
	x2=p1; y2=p2;
	cnt=0;
	while (msg_code!=MSG_IP_L_UP) {
	  restart=TRUE;
	  dc[cur_dc]->color=ROP_EQU+GREEN;
	  GrSpeedLine(dc[cur_dc],x1,y1,x2,y2,ip_speed);
	  restart=TRUE;
	  msg_code=GetMsg(&p1,&p2,
	    1<<MSG_IP_L_UP+1<<MSG_IP_MOVE);
	  x1=x2; y1=y2;
	  x2=p1; y2=p2;
	}
	GrLine3(dc[cur_dc],x1,y1,0,x2,y2,0);
	break;
      case MSG_KEY_DOWN:
	break;
    }
  } while (msg_code!=MSG_KEY_DOWN || !p1);
  SettingsPop;
  GrDCFill;
  GrDCDel(dc[0]);
  GrDCDel(dc[1]);
}

Life;
