F64 theta,phi,omega,s;
CDC *dc0,*dc1;









		 /* <1> <1> (image) */













U0 GrBlackLighting(CDC *dc,CD3I32 *,CD3I32 *,CD3I32 *,CColorROPU32)
{
  dc->color=0;
}

U0 DrawShadow(CTask *task)
{
  I64 r[4][4],*r2;

  GrIdentEqu(dc1->r);
  GrRotZEqu(dc1->r,theta);
  GrRotXEqu(dc1->r,phi);
  GrRotZEqu(dc1->r,omega);
  GrScaleMatEqu(dc1->r,s);
  GrSetTranslation(dc1->r,200,200,5000);
  dc1->flags|=DCF_TRANSFORMATION;
  GrSetRotMat(dc1,dc1->r);

  MemSet(r,0,sizeof(r));

  r[0][0]=gr_scale;
  r[1][1]=gr_scale/2; //Pick a plane for the shadow, to suit your taste.
  r[1][2]=gr_scale/2; //It doesn't have to have a norm of 1.0.
  r[2][2]=gr_scale;
  r[3][3]=gr_scale;

  GrSetTranslation(r,0,-2200,1000);
  r2=GrMulMat(r,dc1->r,task);
  Free(dc1->r);
  GrSetRotMat(dc1,r2);

  dc1->lighting=&GrBlackLighting;
  SpritePlot3(dc1,0,0,0,__BIN_1);
}

U0 DrawIt(CTask *task,CDC *)
{
  DCDepthBufRst(dc0);
  GrIdentEqu(dc0->r);
  GrRotZEqu(dc0->r,theta);
  GrRotXEqu(dc0->r,phi);
  GrRotZEqu(dc0->r,omega);
  GrScaleMatEqu(dc0->r,s);
  GrSetTranslation(dc0->r,200,200,5000);
  dc0->flags|=DCF_TRANSFORMATION;
  GrSetRotMat(dc0,dc0->r);
  if (mp_cnt>1)
    JobQue(&DrawShadow,task,1);
  else
    DrawShadow(task);
  SpritePlot3(dc0,0,0,0,__BIN_1);
}

U0 Shadow()
{
  dc0=DCAlias(gr_dc2,Fs);
  dc1=DCAlias(gr_dc2,Fs);
  DCAllocDepthBuf(dc0);
  dc1->depth_buf=dc0->depth_buf;

  theta=0;
  phi=0;
  omega=0;
  s=1.5;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Fs->draw_it=&DrawIt;
  Preempt(OFF);

  while (!ScanChar) {
    Sleep(20);
    theta+=2*pi/70;
    phi+=2*pi/90;
    omega+=2*pi/110;
  }
  SettingsPop;
  DCDel(dc0);
  dc1->depth_buf=NULL;
  DCDel(dc1);
}

Shadow;
