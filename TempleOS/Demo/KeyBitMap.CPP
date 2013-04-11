//I recently added bitmaps based on scan codes
//and haven't made use of them, yet.


/* <1> <1> (image) */




I64 x=0;

U0 DrawIt(CTask *,CDC *dc)
{
  SpritePlot3(dc,x%100,100,0,__BIN_1);
}


U0 KeyDownBitMapDemo()
{
  //This conversion routine is slow.
  I64 i_sc=A2ScanCode('i');

  "Press and hold $FG,GREEN$'i'$FG$.\n";
  while (!Bt(key_down_bitmap,i_sc))
    Yield;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;

//[C:/TempleOS/Kernel/KGlbls.CPP.Z,124] key_down_bitmap is indexed with the
//merged scan code.  (left shift and
//right shift combined for example.)
//[C:/TempleOS/Kernel/KGlbls.CPP.Z,126] key_down_bitmap2 is indexed with the
//distinct scan code.  See [Char,1] Char and [C:/TempleOS/Kernel/Adam1a.HPP.Z,3276] Scan Codes.

  while (Bt(key_down_bitmap,i_sc)) {
    x++;
    Sleep(10);
  }
  SettingsPop;
  FlushMsgs;
}

KeyDownBitMapDemo;
