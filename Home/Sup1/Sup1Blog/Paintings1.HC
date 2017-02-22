U0 Paintings1()
{
  I64 i,j,l;
  U8 buf[STR_LEN];
  "Ed(\"~/Paintings1.DD.Z\");\n";
  Msg(MSG_KEY_DOWN,0,SC_CURSOR_UP|SCF_CTRL);
  for (i=1;i<=1000;i++) {
    StrPrint(buf,";A=");
    "%c%s%c",CH_CTRLF,buf,CH_ESC;

    "\n#";

    "%c",CH_CTRLF;
    Msg(MSG_KEY_DOWN,0,SC_CURSOR_RIGHT|SCF_CTRL);

    l=StrLen(buf);
    for (j=0;j<l;j++)
      "%c",CH_BACKSPACE;
    "%c",CH_ESC;

    StrPrint(buf,"\">");
    "%c%s%c",CH_CTRLF,buf,CH_ESC;

    "\n";

    "%c",CH_CTRLF;
    Msg(MSG_KEY_DOWN,0,SC_CURSOR_RIGHT|SCF_CTRL);

    l=StrLen(buf);
    for (j=0;j<l;j++)
      "%c",CH_BACKSPACE;
    "%c",CH_ESC;
  }
  "%c",CH_ESC;
}

U0 Main(U8 *src_html="D:/Home/Paintings.html")
{
  "Process http://en.most-famous-paintings.com/MostFamousPaintings.nsf/"
  "ListOfTop1000MostPopularPainting\n\n";
  if (Copy(src_html,"~/Paintings.DD.Z")) {
    ToDolDoc("~/Paintings.DD.Z");
    Copy("~/Paintings.DD.Z","~/Paintings1.DD.Z");
    '\n';
    InStr("Paintings1;");
  } else {
    "First, save the webpage to D:/Home/Paintings.html\n";
  }
}

Main;
