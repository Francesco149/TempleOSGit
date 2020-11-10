//SDF visualization I wrote to document my anime eyes shader

U0 SdfDemo()
{
  U8 buf[64];
  I64 cx=GR_WIDTH/2,cy=GR_HEIGHT/2,rad=100;
  gr.dc->color=LTGRAY;
  while (!ms.lb) {
    DCFill;
    F64 dx=cx-ms.pos.x,dy=cy-ms.pos.y;
    F64 d=Sqrt(dx*dx+dy*dy),sdf=d-rad;
    GrCircle(gr.dc,cx,cy,rad);
    GrLine(gr.dc,ms.pos.x,ms.pos.y,cx-dx/d*rad,cy-dy/d*rad);
    StrPrint(buf,"SDF: %.02f",sdf);
    GrPrint(gr.dc,ms.pos.x-40,ms.pos.y-20,buf);
    Refresh;
  }
  DCFill;
}

SdfDemo;
