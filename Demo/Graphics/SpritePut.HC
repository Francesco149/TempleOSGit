//Use <CTRL-r> to add or edit sprites

/*  <1> (image) */










/*  <2> (image) */









//Used by [C:/Demo/Graphics/SpritePutExt.HC.Z,1].

U0 SpritePut()
{
  I64 i;
  for (i=0;i<3;i++) {
    "US Flag:";
    Sprite(__BIN_1);
    "%h12c",'\n';
  }
  "US Map:";
  Sprite(__BIN_2);
  "%h10c",'\n';
}

SpritePut; //Start program when #included
