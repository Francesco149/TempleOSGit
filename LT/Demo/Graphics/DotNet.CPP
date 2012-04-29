//(The name is a joke.  Certain people
//like to eclipse words with other words
//to confuse people.  I'm retaliating.)

U0 DotNet()
{
  I64 i;
  GrDC *dc=GrDCAlias(gr_dc2,Fs);
  LtfClear;  //Clear text
  dc->color=RED;
  for (i=0;i<480;i+=20)
    GrLine(dc,i,0,0,480-i,3);
  GrDCDel(dc);
  PressAKey;
  GrDCClear;
}

DotNet;  //Run when #included
