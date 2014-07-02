U8 *old_font=text.font;

// See [C:/Kernel/Font8x8.CPP.Z,1], [C:/Demo/ScreenCodes.CPP.Z,1],
//[C:/Demo/ExtendedChars.CPP.Z,1], and [C:/Demo/Graphics/FontEd.CPP.Z,1].

U8 waves[8*4]={
0x00,0x11,0xAA,0x44,0x00,0x11,0xAA,0x44,
0x00,0x22,0x55,0x88,0x00,0x22,0x55,0x88,
0x00,0x44,0xAA,0x11,0x00,0x44,0xAA,0x11,
0x00,0x88,0x55,0x22,0x00,0x88,0x55,0x22};

U0 AnimateEndCB()
{
  text.font=old_font;
  Exit;
}

U0 AnimateTask(I64)
{
  I64 i;
  U8 *font=MAlloc(256*8);
  Fs->task_end_cb=&AnimateEndCB;
  MemCpy(font,text.font,256*8);
  text.font=font;
  while (TRUE) {
    MemCpy(font+CH_SPACE*FONT_HEIGHT,waves+8*(i++&0x3),8);
    Sleep(100);
  }
}

Spawn(&AnimateTask,NULL,"Animate",,Fs);
TaskRep;
