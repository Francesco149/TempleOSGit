
U8 face[8]={

0b00111100,
0b01000010,
0b10100101,
0b10000001,
0b11000011,
0b10111101,
0b01000010,
0b00111100,

};


text.font[255]=face[0](U64);

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

//See [C:/Kernel/StrA.CPP.Z,325] chars_bmp_alpha and [C:/Kernel/StrA.CPP.Z,330] chars_bmp_alpha_numeric.
