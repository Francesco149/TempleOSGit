#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x000000F6
#define __BIN_1 "\x02\x01\x08\x02\x00\x00\x00\x05\x0F\x00\x00\x00\x05\x00\x00\x00\x13\x00\x00\x00\xFD\xFF\xFF\xFF\x05\x13\x00\x00\x00\xFD\xFF\xFF\xFF\x2E\x00\x00\x00\x04\x00\x00\x00\x05\x2E\x00\x00\x00\x04\x00\x00\x00\x50\x00\x00\x00\xFD\xFF\xFF\xFF\x05\x50\x00\x00\x00\xFD\xFF\xFF\xFF\x19\x00\x00\x00\xFC\xFF\xFF\xFF\x05\x4F\x00\x00\x00\xFC\xFF\xFF\xFF\x72\x00\x00\x00\x09\x00\x00\x00\x05\x4E\x00\x00\x00\xFC\xFF\xFF\xFF\x74\x00\x00\x00\x01\x00\x00\x00\x05\x51\x00\x00\x00\xFB\xFF\xFF\xFF\x6F\x00\x00\x00\xFB\xFF\xFF\xFF\x05\x70\x00\x00\x00\xFB\xFF\xFF\xFF\x78\x00\x00\x00\x08\x00\x00\x00\x05\x78\x00\x00\x00\x08\x00\x00\x00\x82\x00\x00\x00\xF9\xFF\xFF\xFF\x05\x71\x00\x00\x00\xFC\xFF\xFF\xFF\x7A\x00\x00\x00\xF9\xFF\xFF\xFF\x05\x7A\x00\x00\x00\xF9\xFF\xFF\xFF\x7D\x00\x00\x00\xEE\xFF\xFF\xFF\x05\x7D\x00\x00\x00\xEE\xFF\xFF\xFF\x6F\x00\x00\x00\xEE\xFF\xFF\xFF\x05\x6F\x00\x00\x00\xEE\xFF\xFF\xFF\x6B\x00\x00\x00\xF9\xFF\xFF\xFF\x05\x76\x00\x00\x00\xF2\xFF\xFF\xFF\x76\x00\x00\x00\xF2\xFF\xFF\xFF\x00"
#define __BIN_2_TYPE 0x00000002
#define __BIN_2_SIZE 0x0000007F
#define __BIN_2 "\x02\x02\x08\x02\x00\x00\x00\x05\x12\x00\x00\x00\x03\x00\x00\x00\x2D\x00\x00\x00\xCF\xFF\xFF\xFF\x05\x24\x00\x00\x00\xE0\xFF\xFF\xFF\x3C\x00\x00\x00\xD8\xFF\xFF\xFF\x05\x26\x00\x00\x00\xD9\xFF\xFF\xFF\x24\x00\x00\x00\xCF\xFF\xFF\xFF\x05\x1C\x00\x00\x00\xED\xFF\xFF\xFF\x1A\x00\x00\x00\xD7\xFF\xFF\xFF\x05\x1B\x00\x00\x00\xF3\xFF\xFF\xFF\x30\x00\x00\x00\xEA\xFF\xFF\xFF\x05\x23\x00\x00\x00\xED\xFF\xFF\xFF\x2C\x00\x00\x00\xE3\xFF\xFF\xFF\x05\x17\x00\x00\x00\xF6\xFF\xFF\xFF\x15\x00\x00\x00\xE4\xFF\xFF\xFF\x00"

I64 num_people=100;


U0 SongTask(U64 dummy=0)
{ //Song by the Holy Spirit
  nounusedwarn dummy;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3eGFqGetBEFeBEEFetBDCeCFqF");
    Play("eGFqGetBEFeBEEFetBDCeCFqF");
    Play("AetDAAqBDeBEqEetECDGBG");
    Play("qAetDAAqBDeBEqEetECDGBG");
  }
}




/* <1> <1> (image) */







/* <2> <2> (image) */




U0 DrawGodTalking(TaskStruct *task,GrDC *dc)
{
  nounusedwarn task;
  I64 i;
  GrSpritePlot3(dc,0,70,0,__BIN_1);
  GrSpritePlot3(dc,200,70,0,__BIN_2);
  for (i=0;i<60;i++) {
    if (i&1)
      dc->color=RED;
    else
      dc->color=LTRED;
    GrLine(dc,215+RandU32%45,60-RandU32%60,
		215+RandU32%45,60-RandU32%60);
  }
}

U0 RandWord()
{
  I64 i;
  if (ws_num_words) {
    i=TimerPickedU32;
    "%s ",ws_sorted_words[i%ws_num_words]->str;
  }
}

U0 RandQuote()
{
  I64 i,size,l=300;
  U8 *bible_filename=Define("ST_BIBLE_FILE");
  U8 *buf=FileRead(bible_filename,&size);
  if (buf) {
    i=TimerPickedU32%(size-l);
    buf[i+l]=0;
    "" buf+i;
    Free(buf);
  }
}

U0 UpTheMountain()
{
  I64 i;
  Mountain;
  Horeb;

  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  Fs->draw_it=&DrawGodTalking;
  "$BG,YELLOW$$WW,1$$CM,0,30$God Says...$FG,RED$\n";
  if (RandU32&1) {
    for (i=0;i<16;i++)
      RandWord;
  } else
    RandQuote;
  "$FG$\n\nPress <SHIFT-ESC>.\n";
  View;
  SettingsPop(Fs,TSF_SAME_SONG);
}


U0 HoldCourt()
{
  I64 i;
  I64 accused,crime,victim;
  BoolI8 old_form=LBts(&(LtfCur)->flags,LTFf_FORM);

  FlushMsgs;
  LtfDblBufStart;

  NewLine;
  accused=RandU32%3;
  "" ListEntryPoint(accused,"A man \0A woman \0A child \0");

  crime=RandU32&3;
  "" ListEntryPoint(crime,"commits murder\0commits adultery\0commits blasphemy\0commits idolatry\0");

  if (crime<=1) {
    '' CH_SPACE;
    victim=RandU32%3;
    "" ListEntryPoint(victim,"to a man\0to a woman\0to a child\0to an animal\0");
  }
  if (!(RandU32%5))
    ", again!";
  else
    '.';
  NewLine(4);

  "$LM,4$"
  "\n\n$BT,\"Show Mercy\",1$\n\n"
  "\n\n$BT,\"Punish\",2$\n\n"
  "\n\n$BT,\"Really Punish\",3$\n\n"
  "$LM,0$";
  LtfDblBufEnd;
  i=LtfMenuDo(LtfCur);

  LBEqu(&(LtfCur)->flags,LTFf_FORM,old_form);
  LtfBottom;
}

#define T_BREAK_CAMP	1
#define T_TALK_WITH_GOD	2
#define T_VIEW_CLOUDS	3
#define T_HOLD_COURT	4
#define T_VIEW_MAP	5
#define T_WATER_ROCK	6
#define T_BATTLE	7
#define T_SACRIFICE	8
#define T_QUAIL		9
#define T_MOSES_COMICS	10
#define T_MUTE		11
#define T_HELP		12
#define T_QUIT		13


BoolI64 TakeTurn()
{
  I64 i;
  BoolI8 result,old_form=LBts(&(LtfCur)->flags,LTFf_FORM);
  num_people*=1.0+0.01*((RandU16%100)-30);
  if (num_people>MAX_PEOPLE)
    num_people=MAX_PEOPLE;
  Camp(num_people);

  FlushMsgs;
  LtfDblBufStart;
	"$LM,4$"
	"\n\n$BT,\"Break Camp\",T_BREAK_CAMP$\n\n"
	"\n\n$BT,\"Talk with God\",T_TALK_WITH_GOD$\n\n"
	"\n\n$BT,\"View Clouds\",T_VIEW_CLOUDS$\n\n"
	"\n\n$BT,\"Hold Court\",T_HOLD_COURT$\n\n"
	"\n\n$BT,\"View Map\",T_VIEW_MAP$\n\n"
	"\n\n$BT,\"Make Water\",T_WATER_ROCK$\n\n"
	"\n\n$BT,\"Battle\",T_BATTLE$\n\n"
	"\n\n$BT,\"Sacrifice\",T_SACRIFICE$\n\n"
	"\n\n$BT,\"Beg for Meat\",T_QUAIL$\n\n"
	"\n\n$BT,\"Moses Comics\",T_MOSES_COMICS$\n\n"
	"\n\n$BT,\"Toggle Mute\",T_MUTE$\n\n"
	"\n\n$BT,\"Help\",T_HELP$\n\n"
	"\n\n$BT,\"Quit\",T_QUIT$\n\n"
	"$LM,0$";
  LtfDblBufEnd;
  i=LtfMenuDo(LtfCur);

  LBEqu(&(LtfCur)->flags,LTFf_FORM,old_form);
  LtfBottom;

  if (i<0 || i==T_QUIT)
    result=FALSE;
  else {
    switch (i) {
      case T_BREAK_CAMP:
	break;
      case T_TALK_WITH_GOD:
	UpTheMountain;
	break;
      case T_VIEW_CLOUDS:
	CloudScene;
	break;
      case T_HOLD_COURT:
	HoldCourt;
	break;
      case T_VIEW_MAP:
	AEMap;
	break;
      case T_WATER_ROCK:
	WaterRock;
	break;
      case T_BATTLE:
	Battle;
	break;
      case T_SACRIFICE:
	Sacrifice;
	break;
      case T_QUAIL:
	Quail;
	break;
      case T_MOSES_COMICS:
	LBts(&Fs->song_task->task_flags,TASKf_SUSPENDED);
	Snd(0);
	ViewComics;
	LBtr(&Fs->song_task->task_flags,TASKf_SUSPENDED);
	break;
      case T_MUTE:
	Mute(!IsMute);
	break;
      case T_HELP:
	PopUp("Cd(Fs->parent_task->cur_dir);Ed(\"FL:AENotes.TXZ,1\");",Fs);
	PopUpOk("Add your own story-line...\nLike old school toys.\n");
	break;
    }
    result=TRUE;
  }
  WinSync;
  return result;
}

U0 TMsg(GrDC *dc,U8 *msg)
{
  F64 t0=tP;
  while (tP-t0<1.5) {
    if (Blink(5,tP))
      dc->color=BLACK;
    else
      dc->color=RED;
    GrRect(dc,0,GR_HEIGHT-FONT_HEIGHT*3,GR_WIDTH,FONT_HEIGHT*2);
    dc->color=BLACK;
    GrRect(dc,2,GR_HEIGHT-FONT_HEIGHT*3+2,GR_WIDTH-4,FONT_HEIGHT*2-4);
    dc->color=YELLOW;
    GrPutS(dc,(GR_WIDTH-StrLen(msg)*FONT_WIDTH)/2,GR_HEIGHT-5*FONT_HEIGHT/2,msg);
    WinSync;
    if (ScanChar)
      throw;
  }
}

U0 Trailer()
{
  GrDC *dc=GrDCAlias(gr_dc2,Fs);
  WinMax;
  WinBorder(OFF);
  Cd(Fs->parent_task->cur_dir);
  Type("AESplash.TXZ");
  Sleep(500);
  FlushMsgs;
  try {
    TMsg(dc,"Leaving all behind, they fled.");
    TMsg(dc,"Found themselves in a desert.");
    TMsg(dc,"God!  We're gonna die!");
    TMsg(dc,"\"Trust Me!\"");
  } catch
    Fs->catch_except=TRUE;
  GrDCClear;
  GrDCDel(dc);
}

U0 AfterEgypt()
{
  BoolI8 was_mute=IsMute;
  U8 *bible_filename=Define("ST_BIBLE_FILE");

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinBorder(OFF);
  WordStat(OFF);
  LtfClear;
  WinMax;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",Fs);
  PopUp("Trailer;",Fs);

  WsInit(bible_filename);
  WordStat(OFF);

  try //This is to catch <CTRL-ALT-C>.
    while (TakeTurn);
  catch
    CatchAll;
  SettingsPop;
  Mute(was_mute);
}

