


/* <1> <1> (image) */




U0 MenuSprite()
{
  I64 i;
  DocCur->flags|=DOCF_FORM; //Cursor navigation will skip empty locations.
  do {
    DocClear;
    NewLine(1);

//Sprite with flags for left-expression and an exit.
//See [C:/TempleOS/Adam/Doc/DocPlain.CPP.Z,28] doc_dft_flags1.	Note the %d in the string for the sprite number.
    PutSprite(__BIN_1,"$SP+LE+X,\"50 Hz\",50,%d$");

    NewLine(3);
    "$MU,\"100 Hz\",100$\n";
    "$MU,\"200 Hz\",200$\n";
    "$MU,\"CANCEL\",DOCM_CANCEL$\n";
    i=DocMenuDo(DocCur);
    if (i>0) Snd(i);
  } while (i>=0);
  Snd(0);
  DocClear;
  NewLine(1);
  DocCur->flags&=~DOCF_FORM;
}

MenuSprite;