/*
This is one of the oldest songs.  I picked
the random name "risen" and said to God
"Oh, you're ambitious," thinking it was
an epic name.  He laughed and gave an
epic song!
*/




				/* <1> <1> (image) */

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
  dc->pen_width=3;
  GrLine3(dc,0,0,-BODY,0,0,BODY);
}

#define NUM_BIRDS	7
#define RANGE		40

F64 bx[NUM_BIRDS],by[NUM_BIRDS];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i;
  F64 theta,dt=Beat-t0;
  if (t0) {
    DCDepthBufAlloc(dc);
    dc->x=task->pix_width>>1;
    dc->y=task->pix_height>>1;
    dc->z=GR_Z_ALL;
    dc->flags|=DCF_TRANSFORMATION;
    Mat4x4IdentEqu(dc->r);
    Mat4x4Scale(dc->r,.5+.04*dt);
    Sprite3(dc,0,0,0,__BIN_1);
    dc->y=task->pix_height+60;
    for (i=0;i<NUM_BIRDS;i++) {
      bx[i]=Clamp(bx[i]+0.35*SignI64(RandI16),-RANGE,RANGE);
      by[i]=Clamp(by[i]+0.35*SignI64(RandI16),-RANGE,RANGE);
      theta=pi/2*i/NUM_BIRDS+0.2*pi*dt+pi/2;
      Mat4x4IdentEqu(dc->r);
      Mat4x4RotX(dc->r,pi/2);
      Mat4x4RotZ(dc->r,theta);
      Mat4x4TranslationEqu(dc->r,230*Cos(theta)+bx[i],230*Sin(theta)+by[i],
	    -(120*i/NUM_BIRDS+16*dt));
      Mat4x4RotX(dc->r,0.45*pi+pi);
      Bird(dc,Sin(2*pi*dt+i*2*pi/NUM_BIRDS));
    }
  }
}

U0 Song()
{
  I64 i,old_update=gr.fp_final_screen_update;
  gr.fp_final_screen_update=NULL;
  t0=0;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
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
  for (i=0;i<NUM_BIRDS;i++) {
    bx[i]=RANGE*RandI16/RANGE;
    by[i]=RANGE*RandI16/RANGE;
  }
  try {
    "$BG+H,LTCYAN$";
    Sleep(250);
    for (i=0;i<1;i++) {
      t0=Beat;
      Play("3eDEqFFetEEFqDeCDDEetCGF");
      Play("eDEqFFetEEFqDeCDDEetCGF");
      Play("eDCqDEeAAetEEFEDGBDCqF");
      Play("eDCqDEeAAetEEFEDGBDCqF");
    }
    Sleep(250);
  } catch
    PutExcept;
  SettingsPop;
  gr.fp_final_screen_update=old_update;
}

Song;
