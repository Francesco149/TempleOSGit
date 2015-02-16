
/* <1> <1> (image) */




I64 x=0;

U0 DrawIt(CTask *,CDC *dc)
{
  Sprite3(dc,x%100,100,0,__BIN_1);
}

U0 KeyDownBitMapDemo()
{
  //This conversion routine is slow.
  I64 i_sc=Char2ScanCode('i');

  "Press and hold $GREEN$'i'$FG$.\n";
  while (!Bt(kbd.down_bitmap,i_sc))
    Yield;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;

//[C:/Kernel/KernelA.HPP.Z,2890] kbd.down_bitmap is indexed with the
//merged scan code.  (left shift and
//right shift combined for example.)
//[C:/Kernel/KernelA.HPP.Z,2890] kbd.down_bitmap2 is indexed with the
//distinct scan code.  See [Char,1] Char and [C:/Kernel/KernelA.HPP.Z,3470] Scan Codes.

  while (Bt(kbd.down_bitmap,i_sc)) {
    x++;
    Sleep(10);
  }
  SettingsPop;
  FlushMsgs;
}

KeyDownBitMapDemo;
