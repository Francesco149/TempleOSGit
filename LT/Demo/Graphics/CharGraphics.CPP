U8 *old_font=gr_font;

// See [::/LT/OSMain/Font8x8.CPZ] ::/LT/OSMain/Font8x8.CPZ.
// See [::/LT/Demo/ScreenCodes.CPZ] ::/LT/Demo/ScreenCodes.CPZ.
// See [::/LT/Demo/ExtendedChars.CPZ] ::/LT/Demo/ExtendedChars.CPZ.
// See [::/LT/Demo/Graphics/FontEd.CPZ] ::/LT/Demo/Graphics/FontEd.CPZ.

U8 waves[8*4]={
0x00,0x11,0xAA,0x44,0x00,0x11,0xAA,0x44,
0x00,0x22,0x55,0x88,0x00,0x22,0x55,0x88,
0x00,0x44,0xAA,0x11,0x00,0x44,0xAA,0x11,
0x00,0x88,0x55,0x22,0x00,0x88,0x55,0x22};

U0 AnimateEnd()
{
  gr_font=old_font;
  Exit;
}

U0 AnimateTask(U64)
{
  U64 i;
  U8 *font=MAlloc(256*8);
  Fs->task_end_cb=&AnimateEnd;
  MemCpy(font,gr_font,256*8);
  gr_font=font;
  while (TRUE) {
    MemCpy(font+CH_SPACE*FONT_HEIGHT,waves+8*(i++&0x3),8);
    Sleep(100);
  }
}


Spawn(&AnimateTask,NULL,"Animate",Fs);
TaskRep;
