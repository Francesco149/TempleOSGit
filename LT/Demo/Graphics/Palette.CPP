U0 PaletteDemo()
{
  U64 i,j;
  BGREntry bgr;
  for (i=0;i<16;i++)
    "$FG,BLACK$$BG,%d$__%-8Z__$FG$$BG$\n",i,i,"ST_COLORS";

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
//Alternatively, use [C:/LT/Adam/Gr/GrScreen.APZ,226] SetVGAPalette4(vga_palette4_gray);
  for (i=0;i<16;i++) {
    j=0xFF*i/15;
    bgr.b=j;
    bgr.g=j;
    bgr.r=j;
    SetVGAPaletteColor(i,bgr);
  }
  PressAKey;

  //You can do it this way, too.
  SetVGAPaletteColor(WHITE,0xFFFF00);
  PressAKey;

//[C:/LT/Adam/TaskSettings.APZ,64] SettingsPop() restores original palette
  SettingsPop;
}

PaletteDemo;
