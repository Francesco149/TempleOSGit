#define DC_NUM	4

CDC *cafe_dc[DC_NUM];
F64 glbl_scale;

U0 DrawIt(CTask *,CDC *dc)
{
  dc->flags|=DCF_TRANSFORMATION;
  Mat4x4Scale(dc->r,glbl_scale);
  GrBlot3(dc,0,0,0,cafe_dc[winmgr.updates%DC_NUM]);
}

U0 Init(U8 *filename,F64 scale)
{
  I64 i;
  glbl_scale=scale;
  for (i=0;i<DC_NUM;i++)
    cafe_dc[i]=BMPRead(filename,TRUE);
}

U0 CleanUp()
{
  I64 i;
  for (i=0;i<DC_NUM;i++)
    DCDel(cafe_dc[i]);
}

U0 BMP24View(U8 *filename,F64 scale=1.0)
{
  Cd(__DIR__);
  if (!FileFind(filename))
    throw('NotFound');
  Init(filename,scale);
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar;
  SettingsPop;
  CleanUp;
}

BMP24View("Cafe.BMP.Z",0.785);
BMP24View("Elephants.BMP.Z",2.135);
