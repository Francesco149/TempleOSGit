//Practice your ASCII knowledge ;-)

U0 SPrintFPutChar(U8 ch)
{
  if (ch>=CH_SHIFT_SPACE && ch!=CH_BACKSPACE) {
    if (ch=='$')
      "%02X $FG,RED$$$$FG$ ",ch;
    else
      "%02X $FG,RED$%c$FG$ ",ch,ch;
  } else
      "%02X $FG,RED$ $FG$ ",ch;
}







	    /* <1> <1> (image) */


U0 ASCIIOrgan()
{
  I64 i,ch;

  for (i=0;i<32;i++) {
    SPrintFPutChar(i);
    SPrintFPutChar(i+32);
    SPrintFPutChar(i+64);
    SPrintFPutChar(i+96);
    NewLine;
  }

  "\nPress keys to make snds\n"
  "and press $FG,GREEN$<SHIFT-ESC>$FG$ to exit.\n";
  do {
    //We don't want echo or scan codes, so we use NULL and FALSE...
    ch=GetChar(NULL,FALSE);
    Snd((ch-28)*25);
  } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  Snd(0);

//Note the %d in the string for sprite number.
  NewLine(7);
  PutSprite(__BIN_1,"$SP+LM+PU,\"Click for better organ\",\"#include \\\"::/SparrowOS/Apps/MusicOrgan/Run\\\";\",%d$");
  NewLine(5);

}

ASCIIOrgan;
