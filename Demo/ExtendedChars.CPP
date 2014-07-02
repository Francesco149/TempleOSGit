U0 Install()
{
  text.font[8*255+0]=0b00111100;
  text.font[8*255+1]=0b01000010;
  text.font[8*255+2]=0b10100101;
  text.font[8*255+3]=0b10000001;
  text.font[8*255+4]=0b11000011;
  text.font[8*255+5]=0b10111101;
  text.font[8*255+6]=0b01000010;
  text.font[8*255+7]=0b00111100;
}

Install;

"Face:\xFF\n";

//Chars 128-255 are treated as letters
//by the compiler and everything else.
//Enter them by holding ALT and
//pressing a 3 digit decimal number or
//press <CTRL-ALT-a>.

U0 SrcExample()
{
  I64 f\xFF\xFF=12345;
  "f\xFF\xFF=%d\n",f\xFF\xFF;
}

SrcExample;
//See [C:/Demo/ScreenCodes.CPP.Z,1], [C:/Demo/Graphics/CharGraphics.CPP.Z,1],
//and [C:/Demo/Graphics/FontEd.CPP.Z,1].

//See [C:/Kernel/Str1a.CPP.Z,320] chars_bitmap_alpha and [C:/Kernel/Str1a.CPP.Z,324] chars_bitmap_alpha_numeric.
