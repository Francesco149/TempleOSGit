//This is a drawing program

U0 Doodle()
{
  I64 msg_code,i,x1,y1,x2,y2,p1,p2,color=BLACK;
  GrDC *dc=GrDCAlias(gr_dc2,Fs);
 
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  Fs->win_inhibit=WIF_ALL-WIF_BORDER;

  WinMax;
  WordStat(OFF);

  GrDCClear;
  "$CL$"; //[C:/LT/Adam/Ltf/LtfRecalc.APZ,1684] LtfClear() is better
  do {
    msg_code=GetMsg(&p1,&p2,
      1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN+
      1<<MSG_IP_R_UP);
    switch (msg_code) {
      case MSG_IP_R_UP:
	i=PopUpColor;
	if (i>=0) color=i;
	break;
      case MSG_IP_L_DOWN:
	x1=p1; y1=p2;
	x2=p1; y2=p2;
	dc->color=ROP_XOR+color;
	dc->pen_width=1;
	while (msg_code!=MSG_IP_L_UP) {
	  GrLine3(dc,x1,y1,0,x2,y2,0);
	  msg_code=GetMsg(&p1,&p2,
	    1<<MSG_IP_L_UP+1<<MSG_IP_MOVE);
	  GrLine3(dc,x1,y1,0,x2,y2,0);
	  x2=p1; y2=p2;
	}
	GrLine3(dc,x1,y1,0,x2,y2,0);
	break;
      case MSG_KEY_DOWN:
	break;
    }
  } while (msg_code!=MSG_KEY_DOWN || !p1);
  GrDCClear;
  GrDCDel(dc);
  SettingsPop;
}

Doodle;  //Execute when #included
