/*
This is one of the oldest songs.  I picked
the random name "risen" and said to God
"Oh, you're ambitious," thinking it was
an epic name.  He laughed and gave an
epic song!
*/




				/* <1> <1> (image) */









				/* <2> <2> (image) */









				/* <3> <3> (image) */









				/* <4> <4> (image) */



F64 t0=Beat;

#define WING	24
#define BODY	8

U0 Bird(CDC *dc,F64 wing_theta)
{
  CD3I32 p[3];
  dc->color=WHITE;

  p[0].x=0; p[0].y=0; p[0].z=-BODY;
  p[1].x=0; p[1].y=0; p[1].z=BODY;
  p[2].x=-WING*Cos(wing_theta); p[2].y=-WING*Sin(wing_theta); p[2].z=0;
  GrFillPoly3(dc,3,p);

  p[0].x=0; p[0].y=0; p[0].z=-BODY;
  p[1].x=0; p[1].y=0; p[1].z=BODY;
  p[2].x=WING*Cos(wing_theta); p[2].y=-WING*Sin(wing_theta); p[2].z=0;
  GrFillPoly3(dc,3,p);

  dc->color=ROPF_DITHER+WHITE<<16+LTGRAY;
  dc->thick=3;
  GrLine3(dc,0,0,-BODY,0,0,BODY);
}

#define SCALE		115
#define ARM		112
#define CHAIN		100
#define CUP		28
#define TILT		-.25
#define ARM_Y		-(0.93*CHAIN)
#define ARM_Z		-45

#define BIRDS_NUM	7
#define RANGE		40
F64 bx[BIRDS_NUM],by[BIRDS_NUM];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i;
  F64 theta,dt=Beat-t0,d_chain=Sin(pi/64*Sin(pi*dt/4))*CHAIN;
  if (t0) {
    DCDepthBufAlloc(dc);
    dc->flags|=DCF_TRANSFORMATION;

    Mat4x4IdentEqu(dc->r);
    dc->x=task->pix_width>>1;
    dc->y=task->pix_height>>1-SCALE;
    dc->z=3*GR_Z_ALL>>1;
    Mat4x4RotX(dc->r,TILT);
    Mat4x4RotZ(dc->r,pi/64*Sin(pi*dt/4));
    Sprite3(dc,0,0,0,__BIN_3);

    Mat4x4IdentEqu(dc->r);
    dc->x=task->pix_width>>1-ARM;
    dc->y=task->pix_height>>1-SCALE+CHAIN-d_chain;
    dc->z=3*GR_Z_ALL>>1;
    Mat4x4RotX(dc->r,TILT+pi/32*Sin(pi*dt/4));
    Mat4x4RotY(dc->r,pi/32*Sin(pi*dt/4));
    Sprite3(dc,0,0,0,__BIN_2);

    Mat4x4IdentEqu(dc->r);
    Mat4x4RotX(dc->r,TILT);
    dc->color=BLACK;
    GrLine3(dc,0,ARM_Y,ARM_Z,0,0,0.707*CUP);
    GrLine3(dc,0,ARM_Y,ARM_Z,CUP,0,-0.707*CUP);
    GrLine3(dc,0,ARM_Y,ARM_Z,-CUP,0,-0.707*CUP);

    Mat4x4IdentEqu(dc->r);
    dc->x=task->pix_width>>1+ARM;
    dc->y=task->pix_height>>1-SCALE+CHAIN+d_chain;
    dc->z=3*GR_Z_ALL>>1;
    Mat4x4RotX(dc->r,TILT-pi/32*Sin(pi*dt/4));
    Mat4x4RotY(dc->r,-pi/32*Sin(pi*dt/4));
    Sprite3(dc,0,0,0,__BIN_2);

    Mat4x4IdentEqu(dc->r);
    Mat4x4RotX(dc->r,TILT);
    dc->color=BLACK;
    GrLine3(dc,0,ARM_Y,ARM_Z,0,0,0.707*CUP);
    GrLine3(dc,0,ARM_Y,ARM_Z,CUP,0,-0.707*CUP);
    GrLine3(dc,0,ARM_Y,ARM_Z,-CUP,0,-0.707*CUP);

    Mat4x4IdentEqu(dc->r);
    dc->x=task->pix_width>>1;
    dc->y=task->pix_height>>1;
    dc->z=3*GR_Z_ALL>>1;
    Mat4x4RotZ(dc->r,0.6-pi/32*Sin(pi*dt/4));
    Mat4x4RotX(dc->r,TILT-pi/32*Sin(pi*dt/4));
    Mat4x4RotY(dc->r,-pi/32*Sin(pi*dt/4));
    Sprite3(dc,0,0,0,__BIN_4);

    if (dt>=12.0) {
      Mat4x4IdentEqu(dc->r);
      dc->x=task->pix_width>>1;
      dc->y=task->pix_height>>1;
      dc->z=GR_Z_ALL;
      Mat4x4Scale(dc->r,.5+.04*dt);
      Sprite3(dc,0,0,0,__BIN_1);
    }

    dc->y=task->pix_height+60;
    for (i=0;i<BIRDS_NUM;i++) {
      bx[i]=Clamp(bx[i]+0.35*SignI64(RandI16),-RANGE,RANGE);
      by[i]=Clamp(by[i]+0.35*SignI64(RandI16),-RANGE,RANGE);
      theta=pi/2*i/BIRDS_NUM+0.2*pi*dt+pi/2;
      Mat4x4IdentEqu(dc->r);
      Mat4x4RotX(dc->r,pi/2);
      Mat4x4RotZ(dc->r,theta);
      Mat4x4TranslationEqu(dc->r,230*Cos(theta)+bx[i],230*Sin(theta)+by[i],
	    -(120*i/BIRDS_NUM+16*dt));
      Mat4x4RotX(dc->r,0.45*pi+pi);
      Bird(dc,Sin(2*pi*dt+i*2*pi/BIRDS_NUM));
    }
  }
}

U0 TOSTheme()
{
  I64 i,old_update=gr.fp_final_scrn_update;
  gr.fp_final_scrn_update=NULL;
  t0=0;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  WinVert(0,TEXT_ROWS-1,Fs);
  DocCursor;
  DocClear;
  Fs->text_attr=LTCYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.85;
  music.stacatto_factor= 0.902;
  for (i=0;i<BIRDS_NUM;i++) {
    bx[i]=RANGE*RandI16/RANGE;
    by[i]=RANGE*RandI16/RANGE;
  }
  try {
    "$BG+H,LTCYAN$";
    Refresh(2);
    for (i=0;i<1;i++) {
      t0=Beat;
      Play("5eDEqFFetEEFqDeCDDEetCGF");
      Play("5eDEqFFetEEFqDeCDDEetCGF");
      Play("5eDCqDE4eAA5etEEFEDG4B5DCqF");
      Play("5eDCqDE4eAA5etEEFEDG4B5DCqF");
    }
    Refresh(2);
  } catch
    PutExcept;
  SettingsPop;
  gr.fp_final_scrn_update=old_update;
}

TOSTheme;
