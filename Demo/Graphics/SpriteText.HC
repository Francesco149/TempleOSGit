
		/*  <1> (image) */






























U0 SpriteText(U8 *elems)
{
  CSprite *tmpg=elems-offset(CSprite.start);
  U8 *st;
  while (tmpg->type&SPG_TYPE_MASK) {
//Study this routine [C:/Adam/Gr/SpriteNew.HC.Z,126] SpriteElem2Summary().
    st=SpriteElem2Summary(tmpg);
    "%s\n",st;
    Free(st);
    tmpg(U8 *)+=SpriteElemSize(tmpg);
  }
}

"$PURPLE$**** LINE SUMMARIES ****$FG$\n";
SpriteText(__BIN_1);

"\n\n$PURPLE$**** ENTIRE SPRITE CODE ****$FG$\n";
Sprite2Code(,__BIN_1);
