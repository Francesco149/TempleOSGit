//Practice your ASCII knowledge ;-)

U0 SafePutChar(U8 ch)
{
  if (CH_SHIFT_SPACE<=ch<0x7F) {
    if (ch=='$')
      "%02X $RED$$$$FG$ ",ch;
    else
      "%02X $RED$%c$FG$ ",ch,ch;
  } else
      "%02X $RED$ $FG$ ",ch;
}






	    /* <1> <1> (image) */

U0 ASCIIOrgan()
{
  I64 i,ch;

  for (i=0;i<32;i++) {
    SafePutChar(i);
    SafePutChar(i+32);
    SafePutChar(i+64);
    SafePutChar(i+96);
    '\n';
  }

  "\nPress keys to make snds\n"
  "and press $GREEN$<SHIFT-ESC>$FG$ to exit.\n";
  do {
    //We don't want echo or scan codes, so we use NULL and FALSE...
    ch=GetChar(,FALSE);
    Snd((ch-28)*25);
  } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  Snd(0);

//Note the %d in the string for sprite number.
  '\n\n\n\n\n\n\n';
  Sprite(__BIN_1,"$SP+PU,\"Click for better organ\",LM=\"#include \\\"::/Apps/Psalmody/Run\\\";\",BI=%d$");
  '\n\n\n\n\n';
}

ASCIIOrgan;
