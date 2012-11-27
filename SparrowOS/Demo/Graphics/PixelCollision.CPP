


/* <1> <1> (image) */


/*
This CSprite was converted to a bitmap
in the <CTRL-R> menu.  If you don't convert
to a bitmap, you will probably
register collisions as the pieces
are drawn on top of each other.  You
can work with a nonzero val if
you choose or convert to a bitmap.
*/


U0 DrawShapes(CDC *dc,I64 x,I64 y,I64 rop)
{
  if (ip_lb)
    dc->bkcolor=RED;
  else
    dc->bkcolor=BLACK;
  dc->color=rop+RED;
  GrRect      (dc,x-10,y-30-10,20,20);
  GrRect      (dc,x-10-30,y-10,20,20);
  CSpritePlot3(dc,x,y,0,__BIN_1); //This just shows it works with CSprite
  dc->color=rop+BLACK;
  GrRect      (dc,x-10,y+30-10,20,20);
  GrRect      (dc,x-10+30,y-10,20,20);
}


U0 PixelCollision()
{
  CDC *dc=DCAlias;

  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WordStat;
  WinMax;
  DocClear;
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

//This waits for the Window Mgr task
//to update the screen.
    WinMgrSync;

//This will not flicker as long as it occurs just
//after a acreen update [C:/SparrowOS/Adam/Window.CPP.Z,608] WinMgrSync.
    DCFill(dc);

  }
  SettingsPop;

  DCDel(dc);
}

/*  If you are dealing with many objects,
create a bitmap just for checking collisions
using [C:/SparrowOS/Adam/Gr/GrDC.CPP.Z,42] DCNew().  You probably want to check
only when they are near each other or it will
take too long.	See [::/SparrowOS/Demo/GameStarters/Rocks.CPP.Z,1] ::/SparrowOS/Demo/GameStarters/Rocks.CPP.Z.
*/

PixelCollision;
