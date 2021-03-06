//Uses [C:/Demo/Lectures/FixedPoint.HC.Z,1] fixed-point-arithmetic.

I64 x[16],y[16],dx[16],dy[16];

U0 Init()
{
  I64 i;
  F64 theta;
  MemSet(x,0,sizeof(x));
  MemSet(y,0,sizeof(y));
  for (i=0;i<16;i++) {
    theta=Rand*2*pi;
    dx[i]=I32_MAX*Cos(theta);
    dy[i]=I32_MAX*Sin(theta);
  }
}
 
U0 Bounce()
{
  CDC *dc=DCAlias;
  I64 i,ch;
  Init;
  try {//Catch <CTRL-ALT-c>
    do {
      for (i=0;i<16;i++) {
	dc->color=i;
	GrPlot(dc,x[i].i32[1],y[i].i32[1]);
	x[i]+=dx[i];
	y[i]+=dy[i];
	if (!(0<=x[i]<Fs->pix_width<<32)) {
	  x[i]-=dx[i];
	  dx[i]=-dx[i];
	}
	if (!(0<=y[i]<Fs->pix_height<<32)) {
	  y[i]-=dy[i];
	  dy[i]=-dy[i];
	}
      }
      Yield;
    } while (!(ch=ScanChar) || (ch!=CH_SHIFT_ESC && ch!=CH_ESC));
  } catch
    PutExcept;
  DCFill(dc);
  DCDel(dc);
}
 
Bounce;
