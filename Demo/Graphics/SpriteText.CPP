
		/*  <1> (image) */






























U0 SpriteText(U8 *elems)
{
  CSprite *tempg=elems-offset(CSprite.start);
  U8 *st;
  while (tempg->type) {
    //Study this routine [C:/Adam/Gr/SpriteNew.CPP.Z,116] SpriteElem2OneLineSummary().
    st=SpriteElem2OneLineSummary(tempg);
    "%s\n",st;
    Free(st);
    tempg(U8 *)+=SpriteElemSize(tempg);
  }
}

"$PURPLE$**** LINE SUMMARIES ****$FG$\n";
SpriteText(__BIN_1);

"\n\n$PURPLE$**** ENTIRE SPRITE CODE ****$FG$\n";
Sprite2Code(,__BIN_1);
