
/* <1> <1> (image) */






U0 SpriteText(U8 *elems)
{
  CSprite *tempg=elems-offset(CSprite.start);
  U8 *st;
  while (tempg->type) {
    //Study this routine [C:/SparrowOS/Adam/Gr/CSpriteNew.CPP.Z,170] CSprite2Text().
    st=CSprite2Text(tempg);
    "%s\n",st;
    Free(st);
    tempg(U8 *)+=CSpriteElemSize(tempg);
  }
}

SpriteText(__BIN_1);
