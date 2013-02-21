/* This demonstrates "interpolation"
which is generating frames between
frames.  For interpolation to work,
your CSprite must not be bitmaps and
must be identical, except shifted
around.  Cut and paste and use
"shift points."
*/





/* <1> <1> (image) */









/* <2> <2> (image) */




F64 t0;

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *inter;

  if (tP(task)-t0<1.0) {
    inter=SpriteInterpolate(__BIN_1,__BIN_2,(tP(task)-t0)%1.0);
    SpritePlot3(dc,100,100,0,inter);
    Free(inter);
  } else
    SpritePlot3(dc,100,100,0,__BIN_1);
}

U0 Golf()
{
  SettingsPush; //See [C:/SparrowOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;

  DocClear;
  do t0=tP;
  while (PressAKey);
  SettingsPop;
}

Golf;
