//5 has graphics





	/* <1> <1> (image) */





#define SIDE	20
#define BLADES	(SIDE*SIDE)
F64 blades_theta[BLADES];

F64 t0=Beat;
U0 DrawIt(CTask *,CDC *dc)
{
  I64 i,j,k;
  F64 x,y,x2,y2,dt=Beat-t0;
  DCDepthBufAlloc(dc);
  dc->x=300;
  dc->y=30;
  dc->z=GR_Z_ALL;
  dc->flags|=DCF_TRANSFORMATION;
  if (dt<16)
    Mat4x4RotZ(dc->r,(dt+5)*pi/4);
  else
    Mat4x4RotZ(dc->r,5*pi/4);
  Mat4x4RotX(dc->r,-5*pi/4);
  Mat4x4Scale(dc->r,0.75);
  Sprite3(dc,0,0,0,__BIN_1);
  if (dt>=16) {
    dc->color=GREEN;
    dt=0.5*(dt-16);
    k=0;
    for (i=0,y=-50;i<SIDE;i++,y+=100.0/SIDE)
      for (j=0,x=-50;j<SIDE;j++,x+=100.0/SIDE) {
	P2R(&x2,&y2,dt,blades_theta[k++]);
	GrLine3(dc,x,y,-40,x+x2,y+y2,-40+dt*2);
      }
  }
}

U0 Song()
{
  I64 i;
  for (i=0;i<BLADES;i++)
    blades_theta[i]=2*pi*Rand;

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("4etAAAeGFAGqFetAAAeGFAGqF",
	    "Je\0 \0sus \0 \0made \0fur\0ni\0ture.\n\0"
	    "Je\0 \0sus \0 \0made \0fur\0ni\0ture.\n\0");
      Play("4etGGGeABGAqGetGGGeABGAqG",
	    "God \0 \0 \0made \0 \0all \0Na\0ture.\n\0"
	    "God \0 \0 \0made \0 \0all \0Na\0ture.\n\0");
      Play("4G5etGDC4qG5eCDetD4A5CC4AGsBABAeG5F");
      Play("4qG5etGDC4qG5eCDetD4A5CC4AGsBABAeG5F");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
