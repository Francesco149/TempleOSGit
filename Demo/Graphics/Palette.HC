U0 PaletteDemo()
{
  I64 i,j;
  CBGR48 bgr;
  for (i=0;i<COLORS_NUM;i++)
    "$BLACK$$BG,%d$__%-8Z__$FG$$BG$\n",i,i,"ST_COLORS";

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
//Alternatively, use [C:/Adam/Gr/GrPalette.HC.Z,83] GrPaletteSet(gr_palette_gray);
  for (i=0;i<COLORS_NUM;i++) {
    j=0xFFFF*i/(COLORS_NUM-1);
    bgr.b=j;
    bgr.g=j;
    bgr.r=j;
    GrPaletteColorSet(i,bgr);
  }
  PressAKey;

  //You can do it this way, too.
  GrPaletteColorSet(WHITE,0xFFFFFFFF0000);
  PressAKey;

  //[C:/Adam/TaskSettings.HC.Z,89] SettingsPop() restores original palette
  SettingsPop;
}

PaletteDemo;
