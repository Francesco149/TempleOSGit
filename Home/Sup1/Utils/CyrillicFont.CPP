// http://www.orientdisplay.com/images/cyrillic-Russian-fonts.gif

/* <1> <1> (image) */





































































































U0 CyrillicFont()
{
  CDoc *doc=DocNew("CyrillicFont.TXT");
  I64 x,y,i,j,k,l,ch,sum,b;
  CDC *dc=Sprite2DC(__BIN_1);

  SettingsPush;
  DocCursor;
  DocClear;
  WinMax;
  WinBorder;
  AutoComplete;

  for (i=0x0;i<=0x05;i++)
    for (j=0x00;j<=0x0F;j++) {
      ch=0xA0+i*16+j;
      x=i*30.5+5+j/8;
      y=j*50.1+4+i/4;
      DocPrint(doc,"0x");
      for (k=7;k>=0;k--) {
	b=0;
	for (l=0;l<=4;l++) {
	  sum=0;
	  if (GrPeek(dc,x+5*l  ,y+4.7*k  )!=WHITE) sum++;
	  if (GrPeek(dc,x+5*l+1,y+4.7*k  )!=WHITE) sum++;
	  if (GrPeek(dc,x+5*l  ,y+4.7*k+1)!=WHITE) sum++;
	  if (GrPeek(dc,x+5*l+1,y+4.7*k+1)!=WHITE) sum++;

	  if (sum) {
	    b|=1<<l;
	    dc->color=RED;
	    GrPlot(dc,x+5*l,y+4.7*k);
	  } else {
	    dc->color=GREEN;
	    GrPlot(dc,x+5*l,y+4.7*k);
	  }
	}
	DocPrint(doc,"%02X",b);
      }
      DocPrint(doc,",//%c\n",ch);
    }
  GrBlot(,0,0,dc);
  GetChar;
  GrBlot(,0,-400,dc);
  GetChar;
  DCFill;
  SettingsPop;
  DocWrite(doc);
}

CyrillicFont;
