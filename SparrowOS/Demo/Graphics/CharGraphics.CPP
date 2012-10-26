U8 *old_font=gr_font;

// See [::/SparrowOS/OSMain/Font8x8.CPP.Z,1] ::/SparrowOS/OSMain/Font8x8.CPP.Z.
// See [::/SparrowOS/Demo/ScreenCodes.CPP.Z,1] ::/SparrowOS/Demo/ScreenCodes.CPP.Z.
// See [::/SparrowOS/Demo/ExtendedChars.CPP.Z,1] ::/SparrowOS/Demo/ExtendedChars.CPP.Z.
// See [::/SparrowOS/Demo/Graphics/FontEd.CPP.Z,1] ::/SparrowOS/Demo/Graphics/FontEd.CPP.Z.

U8 waves[8*4]={
0x00,0x11,0xAA,0x44,0x00,0x11,0xAA,0x44,
0x00,0x22,0x55,0x88,0x00,0x22,0x55,0x88,
0x00,0x44,0xAA,0x11,0x00,0x44,0xAA,0x11,
0x00,0x88,0x55,0x22,0x00,0x88,0x55,0x22};

U0 AnimateEndCB()
{
  gr_font=old_font;
  Exit;
}

U0 AnimateTask(U64)
{
  U64 i;
  U8 *font=MAlloc(256*8);
  Fs->task_end_cb=&AnimateEndCB;
  MemCpy(font,gr_font,256*8);
  gr_font=font;
  while (TRUE) {
    MemCpy(font+CH_SPACE*FONT_HEIGHT,waves+8*(i++&0x3),8);
    Sleep(100);
  }
}


Spawn(&AnimateTask,NULL,"Animate",Fs);
TaskRep;
