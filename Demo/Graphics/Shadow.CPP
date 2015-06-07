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

  Mat4x4IdentEqu(dc1->r);
  Mat4x4RotZ(dc1->r,theta);
  Mat4x4RotX(dc1->r,phi);
  Mat4x4RotZ(dc1->r,omega);
  Mat4x4Scale(dc1->r,s);
  Mat4x4TranslationEqu(dc1->r,200,200,5000);
  dc1->flags|=DCF_TRANSFORMATION;
  DCMat4x4Set(dc1,dc1->r);

  MemSet(r,0,sizeof(r));

  r[0][0]=GR_SCALE;
  r[1][1]=GR_SCALE/2; //Pick a plane for the shadow, to suit your taste.
  r[1][2]=GR_SCALE/2; //It doesn't have to have a norm of 1.0.
  r[2][2]=GR_SCALE;
  r[3][3]=GR_SCALE;

  Mat4x4TranslationEqu(r,0,-2200,1000);
  r2=Mat4x4MulMat4x4New(r,dc1->r,task);
  Free(dc1->r);
  DCMat4x4Set(dc1,r2);

  dc1->lighting=&GrBlackLighting;
  Sprite3(dc1,0,0,0,__BIN_1);
}

U0 DrawIt(CTask *task,CDC *)
{
  DCDepthBufRst(dc0);
  Mat4x4IdentEqu(dc0->r);
  Mat4x4RotZ(dc0->r,theta);
  Mat4x4RotX(dc0->r,phi);
  Mat4x4RotZ(dc0->r,omega);
  Mat4x4Scale(dc0->r,s);
  Mat4x4TranslationEqu(dc0->r,200,200,5000);
  dc0->flags|=DCF_TRANSFORMATION;
  DCMat4x4Set(dc0,dc0->r);
  if (mp_cnt>1)
    JobQue(&DrawShadow,task,1);
  else
    DrawShadow(task);
  Sprite3(dc0,0,0,0,__BIN_1);
}

U0 Shadow()
{//I bull-shitted the shadow image.  Any is plausible.
  dc0=DCAlias(gr.dc2,Fs);
  dc1=DCAlias(gr.dc2,Fs);
  DCDepthBufAlloc(dc0);
  dc1->depth_buf=dc0->depth_buf;

  theta=0;
  phi=0;
  omega=0;
  s=1.5;

  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Fs->draw_it=&DrawIt;
  Preempt;

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
