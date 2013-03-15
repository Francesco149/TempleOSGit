
/* <1> <1> (image) */






U0 SpriteText(U8 *elems)
{
  CSprite *tempg=elems-offset(CSprite.start);
  U8 *st;
  while (tempg->type) {
    //Study this routine [C:/TempleOS/Adam/Gr/CSpriteNew.CPP.Z,170] Sprite2Text().
    st=Sprite2Text(tempg);
    "%s\n",st;
    Free(st);
    tempg(U8 *)+=SpriteElemSize(tempg);
  }
}

SpriteText(__BIN_1);
