#define T_MAN	0
#define T_BALL	1

U0 Models()
{
  I64 i;
  BoolI8 old_form=LBts(&(LtfCur)->flags,LTFf_FORM);
  LtfClear;
  "Make a GrSprite model.  Then, cut-and-Paste it.\n\n";
  "$LM,4$";
  "\n\n$BT,\"Man\",T_MAN$\n\n";
  "\n\n$BT,\"Ball\",T_BALL$\n\n";
  "$LM,0$";
  i=LtfMenuDo(LtfCur);
  LBEqu(&(LtfCur)->flags,LTFf_FORM,old_form);
  LtfBottom;
  switch (i) {
    case T_MAN:
      ManGen;
      break;
    case T_BALL:
      BallGen;
      break;
  }
}
