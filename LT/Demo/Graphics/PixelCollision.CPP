#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x000001AA
#define __BIN_1 "\x16\x1A\x00\x00\x00\x19\x00\x00\x00\xF3\xFF\xFF\xFF\xF4\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x03\x00\x80\xFF\x0F\x00\xE0\xFF\x1F\x00\xF0\xFF\x3F\x00\xF8\xFF\x7F\x00\xFC\xFF\xFF\x00\xFC\xFF\xFF\x00\xFE\xFF\xFF\x00\xFE\xFF\xFF\x01\xFE\xFF\xFF\x01\xFE\xFF\xFF\x01\xFF\xFF\xFF\x01\xFF\xFF\xFF\x03\xFE\xFF\xFF\x01\xFE\xFF\xFF\x01\xFE\xFF\xFF\x01\xFE\xFF\xFF\x01\xFC\xFF\xFF\x00\xFC\xFF\xFF\x00\xF8\xFF\x7F\x00\xF0\xFF\x3F\x00\xE0\xFF\x1F\x00\xC0\xFF\x0F\x00\x00\xFF\x03\x00\x00\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"




/* <1> <1> (image) */


/*
This GrSprite was converted to a bitmap
in the <CTRL-R> menu.  If you don't convert
to a bitmap, you will probably
register collisions as the pieces
are drawn on top of each other.  You
can work with a nonzero val if
you choose or convert to a bitmap.
*/


U0 DrawShapes(GrDC *dc,I64 x,I64 y,I64 rop)
{
  if (ip_lb)
    dc->bkcolor=rop+RED;
  else
    dc->bkcolor=rop+BLACK;
  dc->color=rop+RED;
  GrRect      (dc,x-10,y-30-10,20,20);
  GrRect      (dc,x-10-30,y-10,20,20);
  GrSpritePlot3(dc,x,y,0,__BIN_1); //This just shows it works with GrSprite
  dc->color=rop+BLACK;
  GrRect      (dc,x-10,y+30-10,20,20);
  GrRect      (dc,x-10+30,y-10,20,20);
}


U0 PixelCollision()
{
  GrDC *dc=GrDCAlias(gr_dc2,Fs);

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WordStat(OFF);
  WinMax;
  LtfClear;
  while (!ScanChar) {
    dc->color=RED;
    GrRect(dc,Fs->win_pixel_width-200,Fs->win_pixel_height-200,200,200);
    DrawShapes(dc,Fs->win_pixel_width>>1,Fs->win_pixel_height>>1,ROP_EQU);

    dc->collision_cnt=0;
    DrawShapes(dc,ipx-Fs->win_pixel_left-Fs->win_scroll_x,ipy-Fs->win_pixel_top-Fs->win_scroll_y,ROP_COLLISION);

    if (ip_lb) {
      dc->color=ROP_EQU+BLACK;
      GrPrintF(dc,0,0*FONT_HEIGHT,"Non-RED background pixels overwritten.");
      GrPrintF(dc,0,2*FONT_HEIGHT,"Collisions:%d",dc->collision_cnt);
      dc->color=ROP_EQU+RED;
      GrPutS(dc,0,3*FONT_HEIGHT,"BKCOLOR=RED");
    } else {
      dc->color=ROP_EQU+BLACK;
      GrPutS(dc,0,0*FONT_HEIGHT,"Non-BLACK background pixels overwritten.");
      GrPutS(dc,0,1*FONT_HEIGHT,"(Unchanged pixels of gr_dc2 are 'BLACK'.)");
      GrPrintF(dc,0,2*FONT_HEIGHT,"Collisions:%d",dc->collision_cnt);
      dc->color=ROP_EQU+BLACK;
      GrPutS(dc,0,3*FONT_HEIGHT,"BKCOLOR=BLACK");
    }
    if (Blink) {
      dc->color=ROP_EQU+BLACK;
      GrPutS(dc,0,5*FONT_HEIGHT,"Hold Left Mouse");
    }

    DrawShapes(dc,ipx-Fs->win_pixel_left-Fs->win_scroll_x,ipy-Fs->win_pixel_top-Fs->win_scroll_y,ROP_EQU);

//This waits for the Window Manager task
//to update the screen.
    WinSync;

//This will not flicker as long as it occurs just
//after a acreen update [C:/LT/Adam/Window.APZ,601] WinSync.
    GrDCClear(dc);

  }
  SettingsPop;

  GrDCDel(dc);
}

/*  If you are dealing with many objects,
create a bitmap just for checking collisions
using [C:/LT/Adam/Gr/GrDC.APZ,45] GrDCNew().  You probably want to check
only when they are near each other or it will
take too long.	See [::/LT/Demo/GameStarters/Rocks.CPZ] ::/LT/Demo/GameStarters/Rocks.CPZ.
*/

PixelCollision;
