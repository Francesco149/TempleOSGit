U0 DotNet()
{
  I64 i;
  GrDC *dc=GrDCAlias;
  LtfClear;  //Clear text
  dc->color=RED;
  for (i=0;i<480;i+=20)
    GrLine(dc,i,0,0,480-i,3);
  GrDCDel(dc);
  PressAKey;
  GrDCFill;
}

DotNet;  //Run when #included
