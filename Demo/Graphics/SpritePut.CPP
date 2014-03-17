//Use <CTRL-R> to add or edit sprites


/*  <1> (image) */










//Used by [C:/Apps/MusicOrgan/Examples/battlehymnsolo.CPP.Z,1].


/*  <2> (image) */









//Used by [C:/Demo/Graphics/SpritePutExt.CPP.Z,1].



U0 SpritePut()
{
  I64 i;
  for (i=0;i<3;i++) {
    "US Flag:";
    Sprite(__BIN_1);
    NewLine(12);
  }
  "US Map:";
  Sprite(__BIN_2);
  NewLine(10);
}

//Start program when #included
SpritePut;
