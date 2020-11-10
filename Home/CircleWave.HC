F64 Wave(F64 x) {
  I64 s=ToI64(Sign(x-.5));
  F64 t=((x-.5)*2-(s-1)/2)*2-1;
  return -s*Sqrt(1-t*t);
}

I64 WavePt(I64 x) {
  return 320+Wave((x%120)/120.0)*30;
}

U0 Frame() {
  I64 i,p=WavePt(0);
  for (i=1;i<300;++i) {
    I64 np=WavePt(i);
    GrLine(gr.dc,i-1,p,i,np);
    p=np;
  }
}

DCFill;
gr.dc->color=LTRED;
Frame;
Refresh;
PressAKey;
DCFill;