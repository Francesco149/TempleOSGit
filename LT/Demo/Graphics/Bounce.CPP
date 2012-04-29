//This uses [::/LT/Demo/Lectures/FixedPoint.CPZ] fixed-point-arithmetic.

I64 x[16],y[16],dx[16],dy[16];

U0 Init()
{
  I64 i;
  F64 theta;
  MemSet(x,0,sizeof(x));
  MemSet(y,0,sizeof(y));
  for (i=0;i<16;i++) {
    theta=Rand*2*pi;
    dx[i]=MAX_I32*Cos(theta);
    dy[i]=MAX_I32*Sin(theta);
  }
}
 
U0 Bounce()
{
  GrDC *dc=GrDCAlias(gr_dc2,Fs);
  I64 i,msg_code,p1,update;
  Init;
  try { //Catch <CTRL-ALT-C>
    while (TRUE) {
      update=win_updates;
      do for (i=0;i<16;i++) {
	  dc->color=i;
	  GrPlot(dc,x[i].i32[1],y[i].i32[1]);
	  x[i]+=dx[i];
	  y[i]+=dy[i];
	  if (!(0<=x[i]<Fs->win_pixel_width<<32)) {
	    x[i]-=dx[i];
	    dx[i]=-dx[i];
	  }
	  if (!(0<=y[i]<Fs->win_pixel_height<<32)) {
	    y[i]-=dy[i];
	    dy[i]=-dy[i];
	  }
	}
      while (update==win_updates);
      while (msg_code=ScanMsg(&p1,NULL,1<<MSG_KEY_DOWN|1<<MSG_RESIZE|1<<MSG_MOVE))
	if (msg_code==MSG_KEY_DOWN) {
	  if (p1==CH_SHIFT_ESC || p1==CH_ESC)
	    goto bc_done;
	  else
	    Init;
	} else
	  GrDCClear(dc);
    }
bc_done:
  } catch
    CatchAll;
  GrDCClear(dc);
  GrDCDel(dc);
}
 
Bounce;
