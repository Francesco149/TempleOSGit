


/* <1> <1> (image) */




U0 MenuSprite()
{
  I64 i;
  DocPut->flags|=DOCF_FORM; //Cursor navigation will skip empty locations.
  while (TRUE) {
    DocClear;
    NewLine(1);

//Sprite with flags for left-expression and an exit.
//See [C:/Adam/Doc/Doc.CPP.Z,63] doc_dft_flags1.	Note the %d in the string for the sprite number.
    Sprite(__BIN_1,"$LTBLUE$$SP+X,\"50 Hz\",LE=50,BI=%d$");

    NewLine(3);
    "$MU,\"100 Hz\",LE=100$\n";
    "$MU,\"200 Hz\",LE=200$\n";
    "$MU,\"CANCEL\",LE=DOCM_CANCEL$\n";
    i=DocMenuDo(DocPut);
    if (i>0)
      Snd(i);
    else
      break;
  }
  Snd(0);
  DocClear;
  NewLine(1);
  DocPut->flags&=~DOCF_FORM;
}

MenuSprite;