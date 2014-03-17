












	/* <1> <1> (image) */
















	/* <2> <2> (image) */











	/* <3> <3> (image) */









	/* <4> <4> (image) */




	/* <5> <5> (image) */



#define FRAMES		4
#define DOWN_TIME	0.075
#define UP_TIME		0.2
#define SPREAD_RATE	5

CSprite *imgs[FRAMES]={__BIN_1,__BIN_2,__BIN_3,__BIN_4};

F64 t0_down,t0_up;
Bool down_stroke;


U0 DrawWaterRock(CTask *task,CDC *dc)
{
  I64 i,r,
      cx=task->pix_width>>1,
      cy=task->pix_height>>1;
  CSprite *temps;
  F64 tt;
  if (t0_down) {
    tt=(tS-t0_down);
    if (tt>DOWN_TIME) {
      r=SPREAD_RATE*(tt-DOWN_TIME);
      if (r>17) r=17;
      dc->color=BLUE;
      GrCircle(dc,cx-63,cy-20,r);
      if (r>=2)
	GrFloodFill(dc,cx-63,cy-20);
    }
  }
  if (down_stroke) {
    tt=(tS-t0_down)/DOWN_TIME;
    if (tt>0.9999) tt=0.9999;
  } else {
    tt=(tS-t0_up)/UP_TIME;
    if (tt>0.9999) tt=0.9999;
    tt=1.0-tt;
  }
  i=(FRAMES-1)*tt;
  temps=SpriteInterpolate(imgs[i],imgs[i+1],(FRAMES-1)*tt-i);
  Sprite3(dc,cx,cy,0,temps);
  Sprite3(dc,cx-64,cy-4,0,__BIN_5);
  Free(temps);
}

U0 WaterRock()
{
  I64 msg_code,ch,sc;

  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=YELLOW<<4+BLUE;
  try {
    DocClear;
    "$BG,YELLOW$";

    BibleVerse(,"Exodus,17:6",4);
    "\n$BK,1$<SPACE BAR>$BK,0$\n";

    t0_down=t0_up=0;
    down_stroke=FALSE;
    Fs->draw_it=&DrawWaterRock;
    do {
      msg_code=GetMsg(&ch,&sc,1<<MSG_KEY_DOWN|1<<MSG_KEY_UP);
      if (ch==CH_SPACE) {
	if (msg_code==MSG_KEY_DOWN) {
	  t0_down=tS;
	  down_stroke=TRUE;
	} else if (t0_down) { //Ignore extra key_up from start-up
	  t0_up=tS;
	  down_stroke=FALSE;
	}
      }
    } while (msg_code!=MSG_KEY_DOWN || ch!=CH_SHIFT_ESC && ch!=CH_ESC);
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    PutExcept;
  DocClear;
  SettingsPop(Fs,TSF_SAME_SONG);
}
