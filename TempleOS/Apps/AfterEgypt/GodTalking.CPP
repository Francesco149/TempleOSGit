



/* <1> <1> (image) */







/* <2> <2> (image) */




U0 DrawGodTalking(CTask *,CDC *dc)
{
  I64 i;
  F64 m,a,x1,y1,x2,y2;
  SpritePlot3(dc,0,70,0,__BIN_1);
  SpritePlot3(dc,200,70,0,__BIN_2);
  dc->color=win_updates&15;
  for (i=0;i<256;i++) {
    m=Sqr(Rand); a=2*pi*Rand;
    P2R(&x1,&y1,m,a);
    m=Sqr(Rand); a=2*pi*Rand;
    P2R(&x2,&y2,m,a);
    GrLine(dc,235+30*x1,40+30*y1,235+30*x2,40+30*y2);
  }
}

U0 RandWord()
{// [C:/TempleOS/Apps/HolySpirit/HSNotes.TXT.Z,1] Holy Spirit Instructions
 // [C:/TempleOS/Apps/HolySpirit/HolySpirit.CPP.Z,1] HolySpirit.CPP.Z
  if (ws_num_words)
    "%s ",ws_sorted_words[TimerPickedBits(32)%ws_num_words]->str;
}

U0 RandQuote()
{ // [C:/TempleOS/Apps/HolySpirit/HSNotes.TXT.Z,1] Holy Spirit Instructions
  I64 i,size,l=300;
  U8 *bible_filename=Define("ST_BIBLE_FILE");
  U8 *buf=FileRead(bible_filename,&size);
  if (buf) {
    i=TimerPickedBits(32)%(size-l);
//[C:/TempleOS/Apps/HolySpirit/HolySpirit.CPP.Z,1] HolySpirit.CPP.Z
    buf[i+l]=0;
    "%s",buf+i;
    Free(buf);
  }
}

U0 UpTheMountain()
{
  I64 i;
  Mountain;
  Horeb;

  SettingsPush(Fs,TSF_SAME_SONG); //See
[C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=YELLOW<<4+BLUE;
  Fs->draw_it=&DrawGodTalking;
  "$BG,YELLOW$$WW,1$$CM,0,30$God Says...$FG,RED$\n";
  TimerPickFlush;
  if (TimerPickedBits(1))
    for (i=0;i<16;i++)
      RandWord;
  else
    RandQuote;
  "$FG$\n\nPress <SHIFT-ESC>.\n";
  View;
  DocClear;
  SettingsPop(Fs,TSF_SAME_SONG);
}
