











	/* <1> <1> (image) */












	/* <2> <2> (image) */







	/* <3> <3> (image) */















/* <4> <4> (image) */ //Pointer to sprite in [C:/Demo/Graphics/Mountain.CPP.Z,1] Mountain.CPP file

U0 DrawGodTalking(CTask *,CDC *dc)
{
  I64 i;
  F64 m,a,x1,y1,x2,y2,t;
  Sprite3(dc,0,20*FONT_HEIGHT,0,__BIN_4);
  if (Blink(0.4))
    Sprite3(dc,44,99,0,__BIN_1);
  else
    Sprite3(dc,44,99,0,__BIN_2);
  Sprite3(dc,213,91,0,__BIN_3);
  dc->color=winmgr.updates&15;
  for (i=0;i<256;i++) {
    m=Sqr(Rand); a=2*pi*Rand;
    P2R(&x1,&y1,m,a);
    m=Sqr(Rand); a=2*pi*Rand;
    P2R(&x2,&y2,m,a);
    GrLine(dc,235+30*x1,56+30*y1,235+30*x2,40+30*y2);
  }
  if (tS%4.0<2.0)
    t=FullTri(tS,2.0);
  else
    t=0;
  dc->pen_width=3;
  dc->color=BROWN;
  for (i=0+10*Abs(t);i<120-10*Abs(t);i++)
    GrPlot3(dc,i+10,110+4*t*Sin(i/6.0),0);
}

U0 UpTheMountain()
{
  I64 i;
  Mountain;
  Horeb;

  SettingsPush(Fs,TSF_SAME_SONG); //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=YELLOW<<4+BLUE;
  Fs->draw_it=&DrawGodTalking;
  "$BG,LTCYAN$%h10c",'\n';
  "$BG,YELLOW$$WW,1$$CM,0,22$God Says...$RED$\n";
  if (!FileFind(BIBLE_FILENAME) || GodBits(1))
    for (i=0;i<16;i++) {
      FifoU8Flush(god.fifo);
      GodWord;
    }
  else {
    FifoU8Flush(god.fifo);
    GodPassage;
  }
  "$FG$\n\nPress <SHIFT-ESC>.\n";
  View;
  DocClear;
  SettingsPop(Fs,TSF_SAME_SONG);
}

