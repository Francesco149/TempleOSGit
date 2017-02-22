

U0 FillTriF64(F64 x1,F64 y1,F64 x2,F64 y2,F64 x3,F64 y3)
{
  F64 x,y,x_left,x_right,inverse_slope_left,inverse_slope_right;

  x_left =x1;
  x_right=x1;
  inverse_slope_left =(x3-x1)/(y3-y1);
  inverse_slope_right=(x2-x1)/(y2-y1);
  for (y=y1;y<=y2;y++) {
    for (x=x_left;x<=x_right;x++)
      GrPlot(,x,y);
    x_left +=inverse_slope_left;
    x_right+=inverse_slope_right;
  }

  inverse_slope_right=(x3-x2)/(y3-y2);
  for (;y<=y3;y++) {
    for (x=x_left;x<=x_right;x++)
      GrPlot(,x,y);
    x_left +=inverse_slope_left;
    x_right+=inverse_slope_right;
  }
}

U0 FillTriI64(I64 x1,I64 y1,I64 x2,I64 y2,I64 x3,I64 y3)
{ //Fixed radix point
  I64 x,y,x_left,x_right,inverse_slope_left,inverse_slope_right;

  x_left =x1<<32;
  x_right=x1<<32;
  inverse_slope_left =(x3-x1)<<32/(y3-y1);
  inverse_slope_right=(x2-x1)<<32/(y2-y1);
  for (y=y1;y<=y2;y++) {
    for (x=x_left>>32;x<=x_right>>32;x++)
      GrPlot(,x,y);
    x_left +=inverse_slope_left;
    x_right+=inverse_slope_right;
  }

  inverse_slope_right=(x3-x2)<<32/(y3-y2);
  for (;y<=y3;y++) {
    for (x=x_left>>32;x<=x_right>>32;x++)
      GrPlot(,x,y);
    x_left +=inverse_slope_left;
    x_right+=inverse_slope_right;
  }
}

U0 Main()
{
  F64 t0,tf;
  I64 i,x1,y1,x2,y2,x3,y3;

  SettingsPush;
  WinMax;
  WinBorder;
  DocClear;

  while (!ms.lb)
    Yield;
  x1=ms.pos.x;
  y1=ms.pos.y;
  while (ms.lb)
    Yield;
  GrCircle(,x1,y1,10);

  while (!ms.lb)
    Yield;
  x2=ms.pos.x;
  y2=ms.pos.y;
  while (ms.lb)
    Yield;
  GrCircle(,x2,y2,10);


  while (!ms.lb)
    Yield;
  x3=ms.pos.x;
  y3=ms.pos.y;
  while (ms.lb)
    Yield;
  GrCircle(,x3,y3,10);

  t0=tS;
  for (i=0;i<10000;i++)
    FillTriF64(x1,y1,x2,y2,x3,y3);
  tf=tS;
  "Time:%12.6f\n",tf-t0;

  t0=tS;
  for (i=0;i<10000;i++)
    FillTriI64(x1,y1,x2,y2,x3,y3);
  tf=tS;
  "Time:%12.6f\n",tf-t0;

  t0=tS;
  for (i=0;i<10000;i++)
    FillTriF64(x1,y1,x2,y2,x3,y3);
  tf=tS;
  "Time:%12.6f\n",tf-t0;

  t0=tS;
  for (i=0;i<10000;i++)
    FillTriI64(x1,y1,x2,y2,x3,y3);
  tf=tS;
  "Time:%12.6f\n",tf-t0;

  PressAKey;
  DCFill;

  SettingsPop;
}

Main;