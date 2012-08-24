//Generate "Stadium.LGZ" from "Stadium.TXZ"


U0 StadiumGen()
{
  U0 (*fp_old_update)(GrDC *dc);
  GrDC *dc;

  WinMax;
  WinBorder;
  LtfClear;
  Type("::/LT/Demo/GameStarters/Stadium/Stadium.TXZ");
  "$CM,0,%d$",TEXT_ROWS-2;
  LtfCursor(OFF);
  fp_old_update=fp_final_screen_update; //hide mouse cursor
  fp_final_screen_update=NULL;

  WinMgrSync(2);
  dc=GrDCExtract(gr_screen_image,0,FONT_HEIGHT,GR_WIDTH,GR_HEIGHT);

  LGRWrite("::/LT/Demo/GameStarters/Stadium/Stadium",dc);
  GrDCDel(dc);
 
  LtfCursor(ON);
  fp_final_screen_update=fp_old_update;
  LtfClear;
}

StadiumGen;

