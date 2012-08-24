#define MAX_CONNECTS	16

class MyMass:MassStruct
{
  F64 radius;
  U8 nid,type,num_connects,cur_connect;
  U32 audio_widget_capabilities;
  U8 connect_list[MAX_CONNECTS];
  U8 gain_list[MAX_CONNECTS];
  U32 pin_capabilities,pin_sense,
     pcm_size_rates,
     in_amp_cap,out_amp_cap;
  U8 pin_widget_ctl;
  BoolI8 disabled;
};

class MySpring:SpringStruct
{
  I64 color;
};

Ode *ode=NULL;

#define BORDER	10


U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  BoolI8 old_suspend,old_preempt;
  I16 *buf;
  I64 i,cxx,cyy,
    cy=task->win_pixel_height>>1;
  F64 dx,dy,d;
  MyMass   *tempm;
  MySpring *temps;

  old_preempt=Preempt(OFF);
  old_suspend=LBts(&task->task_flags,TASKf_SUSPENDED);
  temps=ode->next_spring;
  while (temps!=&ode->next_spring) {
    dc->color=temps->color;
    GrLine(dc,temps->end1->x,temps->end1->y,
		temps->end2->x,temps->end2->y);
    cxx=(temps->end1->x+temps->end2->x)/2;
    cyy=(temps->end1->y+temps->end2->y)/2;
    dx=temps->end1->x-temps->end2->x;
    dy=temps->end1->y-temps->end2->y;
    d=Sqrt(dx*dx+dy*dy);
    dx/=d;
    dy/=d;
    GrLine(dc,cxx,cyy,cxx+3.0*dy-3.0*dx,cyy-3.0*dx-3.0*dy);
    GrLine(dc,cxx,cyy,cxx-3.0*dy-3.0*dx,cyy+3.0*dx-3.0*dy);
    temps=temps->next;
  }

  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    if (tempm->disabled)
      dc->color=BLUE;
    else
      dc->color=LTBLUE;
    GrCircle(dc,tempm->x,tempm->y,tempm->radius);
    GrPrintF(dc,tempm->x,tempm->y-FONT_HEIGHT/2,"%02X %3tZ",
	tempm->nid,tempm->type,
	"ST_AUDIO_WIDGET_TYPES");
    tempm=tempm->next;
  }
  LBEqu(&task->task_flags,TASKf_SUSPENDED,old_suspend);
  Preempt(old_preempt);
  dc->color=BLUE;
  buf=_hda->istr0_buf[0];
  for (i=0;i<SND_BUF_LEN;i++)
    GrPlot(dc,i*task->win_pixel_width/SND_BUF_LEN,
      cy-buf[i]*cy/MAX_I16);
}


U0 MyDerivative(Ode *ode,F64,Order2D3 *,Order2D3 *)
{
//The forces due to springs and drag are
//automatically handled by the
//ode code.  We can add new forces
//here.
  TaskStruct *task=ode->win_task;
  I64 h=task->win_pixel_width,v=task->win_pixel_height;
  F64 d,dd;
  D3 p;
  MyMass *tempm1,*tempm2;

  tempm1=ode->next_mass;
  while (tempm1!=&ode->next_mass) {
    tempm2=tempm1->next;
    while (tempm2!=&ode->next_mass) {
      D3Sub(&tempm2->state->x,&tempm1->state->x,&p);
      dd=D3SqrNorm(&p);
      if (dd<=Sqr(tempm1->radius+tempm2->radius)) {
	d=Sqrt(dd)+0.0001;
	dd=10.0*Sqr(Sqr(Sqr(tempm1->radius+tempm2->radius)-dd));
	D3MulEqu(&p,dd/d);
	D3AddEqu(&tempm2->DstateDt->DxDt,&p);
	D3SubEqu(&tempm1->DstateDt->DxDt,&p);
      } else {
	D3MulEqu(&p,100000.0/dd);
	D3AddEqu(&tempm2->DstateDt->DxDt,&p);
	D3SubEqu(&tempm1->DstateDt->DxDt,&p);
      }
      tempm2=tempm2->next;
    }
    if (tempm1->state->x<BORDER)
      tempm1->DstateDt->DxDt+=1000*Sqr(tempm1->state->x-BORDER);
    if (tempm1->state->y<BORDER)
      tempm1->DstateDt->DyDt+=1000*Sqr(tempm1->state->y-BORDER);
    if (tempm1->state->x>h-BORDER-FONT_WIDTH*6)
      tempm1->DstateDt->DxDt-=1000*Sqr(tempm1->state->x-(h-BORDER-FONT_WIDTH*6));
    if (tempm1->state->y>v-BORDER)
      tempm1->DstateDt->DyDt-=1000*Sqr(tempm1->state->y-(v-BORDER));
    tempm1=tempm1->next;
  }
}

MyMass *PlaceMass(I64 nid)
{
  MyMass *tempm=CAlloc(sizeof(MyMass));
  tempm->mass=1.0;
  tempm->drag_profile_factor=100.0;
  tempm->radius=5;
  tempm->nid=nid;
  tempm->gain_list[0]=0x7F;
  MemSet(tempm->gain_list+1,0x80,(MAX_CONNECTS-1)*sizeof(U8));
  QueIns(tempm,ode->last_mass);
  return tempm;
}

MyMass *FindMassByNID(I64 nid)
{
  MyMass   *tempm;
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    if (tempm->nid==nid)
      return tempm;
    tempm=tempm->next;
  }
  return NULL;
}

MyMass *FindMassByXY(I64 x,I64 y)
{
  I64 dd,best_dd=MAX_I64,result=NULL;
  MyMass   *tempm;
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    dd=SqrI64(tempm->x-x)+SqrI64(tempm->y-y);
    if (dd<best_dd) {
      result=tempm;
      best_dd=dd;
    }
    tempm=tempm->next;
  }
  return result;
}

I64 FindConnectIndex(MyMass *tempm,I64 nid)
{
  I64 i;
  for (i=0;i<tempm->num_connects;i++)
    if (tempm->connect_list[i]==nid)
      return i;
  return -1;
}


MySpring *PlaceSpring(MyMass *tempm1,MyMass *tempm2)
{
  MySpring *temps=CAlloc(sizeof(MySpring));
  temps->end1=tempm1;
  temps->end2=tempm2;
  temps->constant=10;
  temps->rest_len=0;
  temps->color=LTGRAY;
  QueIns(temps,ode->last_spring);
  return temps;
}

U0 RedoSprings()
{
  I64 i,k;
  MyMass   *tempm,*tempm1;
  MySpring *temps;

  QueDel(&ode->next_spring,TRUE);
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    for (i=0;i<tempm->num_connects;i++) {
      if ((k=tempm->connect_list[i]) &&
	  (tempm1=FindMassByNID(k)) ) {
	temps=PlaceSpring(tempm,tempm1);
	switch (tempm->type) {
	  case AWT_MIXER:
	    if (!(tempm->gain_list[i]&0x80)) { //If not mute
	      temps->color=GREEN;
	      temps->constant=100;
	    }
	    break;
	  case AWT_INPUT:
	  case AWT_SELECTOR:
	  case AWT_PIN_COMPLEX:
	  case AWT_VENDOR:
	    if (i==tempm->cur_connect) {
	      temps->color=RED;
	      temps->constant=100;
	    }
	    break;
	}
      }
    }
    tempm=tempm->next;
  }
}

U0 Init()
{
  ode=OdeNew(0,1e-4,ODEF_HAS_MASSES);
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.00001;
  ode->acceleration_limit=5e3;

  QueIns(ode,Fs->last_ode);
}

U0 CleanUp()
{
  QueRem(ode);
  QueDel(&ode->next_mass,TRUE);
  QueDel(&ode->next_spring,TRUE);
  OdeDel(ode);
}

U0 HDCfgConnectList(MyMass *tempm,I64 cad,I64 nid)
{
  I64 i,j,connection_list_len;
  j=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_CONNECT_LIST_LEN);
  connection_list_len=j&127;
  if (j&128) { //Long form?
    for (i=0;i<connection_list_len;i+=2) {
      j=HDWriteCORBSync(cad,nid,VERB_GET_CONNECT_LIST+i);
      tempm->connect_list[tempm->num_connects++]=j.u16[0];
      if (i+1<connection_list_len)
	tempm->connect_list[tempm->num_connects++]=j.u16[1];
    }
  } else {
    for (i=0;i<connection_list_len;i+=4) {
      j=HDWriteCORBSync(cad,nid,VERB_GET_CONNECT_LIST+i);
      tempm->connect_list[tempm->num_connects++]=j.u8[0];
      if (i+1<connection_list_len)
	tempm->connect_list[tempm->num_connects++]=j.u8[1];
      if (i+2<connection_list_len)
	tempm->connect_list[tempm->num_connects++]=j.u8[2];
      if (i+3<connection_list_len)
	tempm->connect_list[tempm->num_connects++]=j.u8[3];
    }
  }
}

U0 HDCfgTraverse(I64 cad,I64 nid)
{
  I64 i,j;
  MyMass *tempm=PlaceMass(nid);
  j=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_SUBNODE_CNT);
  if (j.u16[0]) {
    tempm->type=AWT_NODE;
    for (i=j.u16[1];i<j.u16[1]+j.u16[0];i++)
      HDCfgTraverse(cad,i);
  } else {
    tempm->audio_widget_capabilities=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_AUDIO_WIDGET_CAP);
    tempm->pcm_size_rates=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_SAMPLE_SIZE_RATE_CAP);
    tempm->in_amp_cap=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_INPUT_AMP_CAP);
    tempm->out_amp_cap=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_OUTPUT_AMP_CAP);
    tempm->type=tempm->audio_widget_capabilities>>20&15;
    switch (tempm->type) {
      case AWT_PIN_COMPLEX:
	tempm->pin_widget_ctl=HDWriteCORBSync(cad,nid,VERB_PIN_WIDGET_CTL_GET);
	tempm->pin_capabilities=HDWriteCORBSync(cad,nid,VERB_GET_PARAM+P_PIN_CAP);
	if (Bt(&tempm->pin_capabilities,0) ||
	    Bt(&tempm->pin_capabilities,2))
	  tempm->pin_sense=HDWriteCORBSync(cad,nid,VERB_PIN_SENSE_GET);
	HDWriteCORBSync(cad,nid,VERB_CONNECT_SELECT_SET+0x00);
	break;
      case AWT_INPUT:
	tempm->disabled=TRUE;
      case AWT_SELECTOR:
      case AWT_VENDOR:
	HDWriteCORBSync(cad,nid,VERB_CONNECT_SELECT_SET+0x00);
	break;
    }
    if (Bt(&tempm->audio_widget_capabilities,8))
      HDCfgConnectList(tempm,cad,nid);
  }
}

U0 HDCfgRandomizeXY()
{
  I64 h=Fs->win_pixel_width,v=Fs->win_pixel_height;
  MyMass   *tempm;
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    tempm->x=RandU32%(h-2*BORDER-FONT_WIDTH*6)+BORDER;
    tempm->y=RandU32%(v-2*BORDER)+BORDER;
    tempm=tempm->next;
  }
}

U0 HDCfgPopUpInfoTask(MyMass *tempm)
{
  I64 i;
  "$FG,RED$NID:$FG$0x%02X $FG,RED$Type:$FG$%Z\n",
	tempm->nid,tempm->type,"ST_AUDIO_WIDGET_TYPES";

  i=tempm->audio_widget_capabilities;
  "\n$FG,RED$Audio Widget Capabilities:$FG$\n";
  if (Bt(&i,0))
     "Stereo, ";
  if (Bt(&i,1))
     "In Amp, ";
  if (Bt(&i,2))
     "Out Amp, ";
  if (Bt(&i,3))
     "Amp Override, ";
  if (Bt(&i,4))
     "Fmt Override, ";
  if (Bt(&i,5))
     "Stripe, ";
  if (Bt(&i,6))
     "Proc Wgt, ";
  if (Bt(&i,7))
     "Unsolicited, ";
  if (Bt(&i,8))
     "Cnt List, ";
  if (Bt(&i,9))
     "Digital, ";
  if (Bt(&i,10))
     "Power Ctrl, ";
  if (Bt(&i,11))
     "Left/Right Swap, ";
  "Delay:%d\n",i.u16[1]&0xF;

  i=tempm->in_amp_cap;
  "\n$FG,RED$Input Amp Capabilities:$FG$\n"
  "Offset:0x%02X Steps:0x%02X StepSize:%5.2fdB Mutable:%d\n",
    i&127,i.u8[1]&127+1,0.25*(i.u16[1]&127),Bt(&i,31);

  i=tempm->out_amp_cap;
  "\n$FG,RED$Output Amp Capabilities:$FG$\n"
  "Offset:0x%02X Steps:0x%02X StepSize:%5.2fdB Mutable:%d\n",
    i&127,i.u8[1]&127+1,0.25*(i.u16[1]&127),Bt(&i,31);

  "\n$FG,RED$Size Rates Capabilities:$FG$\n";
  for (i=0;i<21;i++)
    if (Bt(&tempm->pcm_size_rates,i))
      "%z",i,
	" 8.0kHz\0 11.025kHz\0 16.0kHz\0 22.05kHz\0"
	" 32.0kHz\0 44.1kHz\0 48.0kHz\0 88.2kHz\0"
	" 96.0kHz\0 176.4kHz\0 192.0kHz\0 384kHz\0"
	" \0 \0 \0 \0"
	" 8Bit\0 16bit\0 20Bit\0 24Bit\0 32Bit\0";
  NewLine;

 
  "\n$FG,RED$Connection List:$FG$\n";
  for (i=0;i<tempm->num_connects;i++)
    "%02X,",tempm->connect_list[i];
  NewLine;

  "\n$FG,RED$Gain List:$FG$\n";
  for (i=0;i<tempm->num_connects;i++)
    "%02X,",tempm->gain_list[i];
  NewLine;

  switch (tempm->type) {
    case AWT_PIN_COMPLEX:
      "\n$FG,RED$Pin Capabilities:$FG$\n";
      i=tempm->pin_capabilities;
      if (Bt(&i,0))
	"Impedance Sense, ";
      if (Bt(&i,1))
	"Trigger Required, ";
      if (Bt(&i,2))
	"Presence Detect, ";
      if (Bt(&i,3))
	"Headphone Drive, ";
      if (Bt(&i,4))
	"Output, ";
      if (Bt(&i,5))
	"Input, ";
      if (Bt(&i,6))
	"Balanced, ";
      if (Bt(&i,16))
	"EAPD, ";
      "Vref:%02X\n\n",i.u8[1];
      if (Bt(&tempm->pin_capabilities,0) ||
	  Bt(&tempm->pin_capabilities,2))
	tempm->pin_sense=HDWriteCORBSync(_hda->cad,tempm->nid,VERB_PIN_SENSE_GET);
      if (Bt(&tempm->pin_capabilities,0))
	"Impedance Measurement:%08X\n",tempm->pin_sense&0x7FFFFFFF;
      if (Bt(&tempm->pin_capabilities,2))
	"Presence:%d\n",Bt(&tempm->pin_sense,31);
      "\n\nPin widget ctrl bits:\n"
      "  7: High phn enable (low impedance output amp)\n"
      "  6: Output Enable\n"
      "  5: Input Enable\n"
      "0-2: Vref 0=HiZ 1=50% 2=Gnd 4=80% 5=100%\n";
      tempm->pin_widget_ctl=PmtI64("Widget Ctrl (0x%02X):",tempm->pin_widget_ctl,0,0xFF);
      HDWriteCORBSync(_hda->cad,tempm->nid,VERB_PIN_WIDGET_CTL_SET+tempm->pin_widget_ctl);
      break;
    case AWT_INPUT:
      "Disable";
      if (tempm->disabled=YorN)
	HDWriteCORBSync(_hda->cad,tempm->nid,VERB_CHAN_STREAM_ID_SET+0x00);
      else
	HDWriteCORBSync(_hda->cad,tempm->nid,VERB_CHAN_STREAM_ID_SET+0x20);
      NewLine;
      break;
    case AWT_OUTPUT:
      "Disable";
      if (tempm->disabled=YorN)
	HDWriteCORBSync(_hda->cad,tempm->nid,VERB_CHAN_STREAM_ID_SET+0x00);
      else
	HDWriteCORBSync(_hda->cad,tempm->nid,VERB_CHAN_STREAM_ID_SET+0x10);
      NewLine;
      break;
  }
}

U0 HDCfgPopUpInfo(MyMass *tempm)
{
  U8 buf[136];
  SPrintF(buf,"HDCfgPopUpInfoTask(0x%X);\"\\n\\nPress SHIFT-ESC\\n\\n\\n\\n\";View;",tempm);
  Fs->win_inhibit|=WIF_ODE;
  Fs->draw_it=NULL;
  WinMgrSync;
  PopUp(buf,Fs);
  Fs->win_inhibit&=~WIF_ODE;
  Fs->draw_it=&DrawIt;
}

U0 HDCfgEdLink(MyMass *tempm_out,MyMass *tempm_in)
{
  I64 i,j;
  if ((i=FindConnectIndex(tempm_in,tempm_out->nid))>=0) {
    switch (tempm_in->type) {
      case AWT_MIXER:
	MemSet(tempm_in->gain_list,0x80,sizeof(U8)*MAX_CONNECTS);
	tempm_in->gain_list[i]=0x7F;
	for (j=0;j<tempm_in->num_connects;j++)
	  HDWriteCORBSync(_hda->cad,tempm_in->nid,
	    VERB_AMPLIFIER_GAIN_SET+0x7000+tempm_in->gain_list[j]+j<<8);
	break;
      case AWT_INPUT:
      case AWT_SELECTOR:
      case AWT_PIN_COMPLEX:
      case AWT_VENDOR:
	tempm_in->cur_connect=i;
	HDWriteCORBSync(_hda->cad,tempm_in->nid,VERB_CONNECT_SELECT_SET+i);
	break;
    }
    RedoSprings;
  }
}

U0 HDCfgSave()
{
  Ltf *l=LtfNew;
  I64 i;
  MyMass   *tempm;
  LtfPutS(l,"//This file was generated by $LK,\"::/LT/Adam/Snd/HDCfg.CPZ\",\"FI:::/LT/Adam/Snd/HDCfg.CPZ\"$\n\n"
	    "U0 MyHDCfg()\n{\n");
  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    LtfPrintF(l,"  //0x%02X %Z\n",tempm->nid,tempm->type,"ST_AUDIO_WIDGET_TYPES");
    if (tempm->num_connects) {
      LtfPutS(l,"  //Connection List:");
      for (i=0;i<tempm->num_connects;i++) {
	if (i==tempm->cur_connect)
	  LtfPutS(l,"*");
	LtfPrintF(l,"0x%02X ",tempm->connect_list[i]);
      }
      LtfPutS(l,"\n");
      if (tempm->type==AWT_MIXER) {
	LtfPutS(l,"  //Gain List:");
	for (i=0;i<tempm->num_connects;i++)
	  LtfPrintF(l,"0x%02X ",tempm->gain_list[i]);
	LtfPutS(l,"\n");
      }
    }
    switch (tempm->type) {
      case AWT_OUTPUT:
	if (tempm->disabled)
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_CHAN_STREAM_ID_SET+0x00);\n",
	    tempm->nid);
	else
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_CHAN_STREAM_ID_SET+0x10);\n",
	    tempm->nid);
	break;
      case AWT_MIXER:
	LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_AMPLIFIER_GAIN_SET+0xB07F);\n",
	    tempm->nid);
	for (i=0;i<tempm->num_connects;i++)
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_AMPLIFIER_GAIN_SET+0x%04X);\n",
	    tempm->nid,0x7000+tempm->gain_list[i]+i<<8);
	break;
      case AWT_INPUT:
	if (tempm->disabled)
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_CHAN_STREAM_ID_SET+0x00);\n",
	    tempm->nid);
	else
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_CHAN_STREAM_ID_SET+0x20);\n",
	    tempm->nid);
	goto here;
      case AWT_PIN_COMPLEX:
	LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_PIN_WIDGET_CTL_SET+0x%02X);\n",
	   tempm->nid,tempm->pin_widget_ctl);
      case AWT_SELECTOR:
      case AWT_VENDOR:
here:
	if (tempm->num_connects>1)
	  LtfPrintF(l,"  HDWriteCORBSync(_hda->cad,0x%02X,VERB_CONNECT_SELECT_SET+0x%02X);\n",
	    tempm->nid,tempm->cur_connect);
	break;
    }
    LtfPutS(l,"\n");
    tempm=tempm->next;
  }
  LtfPutS(l,"  LBtr(&sys_semas[SYS_SEMA_SND],0);\n"
		"}\n\nif (snd_dev==SD_HD_AUDIO)\n"
		"  MyHDCfg;\n");
  StrCpy(l->filename.name,"HOME/HDAudioCfg.APZ");
  LtfWrite(l);
  LtfDel(l);
}

U0 HDCfg()
{
  I64 p1,p2;
  MyMass *tempm1=NULL,*tempm2=NULL;

  if (snd_dev!=SD_HD_AUDIO) {
    "HD Audio not detected\n";
    return;
  }
  HDAudioInit(0,27,0);

  MenuPush(
"File {"
"  Abort(,CH_SHIFT_ESC);"
"  Exit(,CH_ESC);"
"}"
"Edit {"
"  Randomize(,CH_NEW_LINE);"
"  Options(,CH_SPACE);"
"}"
  );
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  Preempt(OFF);
  LtfBottom;
  WordStat;
  WinMax;
  WinBorder;
  Fs->win_inhibit=WIF_IP_L|WIF_IP_R|WIF_DBL_CLICK|WIF_LTF;
  Init;

  HDCfgTraverse(_hda->cad,0);
  HDCfgRandomizeXY;
  RedoSprings;
  LtfCursor;
  LtfClear;
  Fs->draw_it=&DrawIt;
  PopUpOk(
"This is a tool to cfgure\n"
"HD Audio.  It creates $FG,RED$HOME/HDAudioCfg.APZ$FG$\n"
"which you should $FG,GREEN$#include$FG$ in your\n"
"$FG,RED$HOME/Adam3cSnd.APZ$FG$ file.\n\n\n"
"Left click to see info and cfgure a widget.\n"
"Right click drag to connect output to input.\n\n"
"You will need to set input and output amplifier\n"
"gains by hand -- edit $FG,RED$HOME/HDAudioCfg.APZ$FG$.\n\n"
"The $FG,BLUE$BLUE$FG$ line in the middle is microphone waveform.\n\n"
);

  HDRun(TRUE,TRUE); //run input output
  Snd(500,WF_SINE,0.50);

  try {
    while (TRUE) {
      switch (GetMsg(&p1,&p2,1<<MSG_IP_L_UP|1<<MSG_IP_R_DOWN|1<<MSG_IP_R_UP|1<<MSG_KEY_DOWN)) {
	case MSG_IP_L_UP:
	  if (tempm1=FindMassByXY(p1,p2))
	    HDCfgPopUpInfo(tempm1);
	  tempm1=NULL;
	  break;
	case MSG_IP_R_DOWN:
	  tempm1=FindMassByXY(p1,p2);
	  break;
	case MSG_IP_R_UP:
	  tempm2=FindMassByXY(p1,p2);
	  if (tempm1 && tempm2)
	    HDCfgEdLink(tempm1,tempm2);
	  break;
	case MSG_KEY_DOWN:
	  switch (p1) {
	    case CH_SPACE:
	      if (tempm1=FindMassByXY(ipx-Fs->win_pixel_left-Fs->win_scroll_x,ipy-Fs->win_pixel_top-Fs->win_scroll_y))
		HDCfgPopUpInfo(tempm1);
	      tempm1=NULL;
	      break;
	    case CH_NEW_LINE:
	      HDCfgRandomizeXY;
	      break;
	    case CH_ESC:
	      HDCfgSave;
	    case CH_SHIFT_ESC:
	      goto hd_done;
	  }
	  break;
      }
    }
hd_done: //Don't goto out of try
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    Fs->catch_except=TRUE;
  SettingsPop;
  HDStop(TRUE,FALSE); //stop input
  CleanUp;
  MenuPop;
  "$BK,1$Note: #include \"HOME/HDAudioCfg.APZ\" in your start-up scripts.$BK,0$\n";
}

HDCfg;

