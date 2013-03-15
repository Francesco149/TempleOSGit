//Use <CTRL-R> to add or edit sprites


/*  <1> (image) */













/*  <2> (image) */













U0 SpritePut()
{
  I64 i;
  for (i=0;i<3;i++) {
    "US Flag:";
    PutSprite(__BIN_1);
    NewLine(12);
  }
  "US Map:";
  PutSprite(__BIN_2);
  NewLine(10);
}

//Start program when #included
SpritePut;
