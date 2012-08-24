#define __BIN_1_TYPE 0x00000002
#define __BIN_1_SIZE 0x000000E0
#define __BIN_1 "\x02\x00\x05\xF5\xFF\xFF\xFF\x03\x00\x00\x00\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x05\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\xFB\xFF\xFF\xFF\x05\x00\x00\x00\x00\xFB\xFF\xFF\xFF\x01\x00\x00\x00\xFB\xFF\xFF\xFF\x05\x01\x00\x00\x00\xFB\xFF\xFF\xFF\x01\x00\x00\x00\x00\x00\x00\x00\x05\x01\x00\x00\x00\x00\x00\x00\x00\x0C\x00\x00\x00\x04\x00\x00\x00\x05\x0C\x00\x00\x00\x04\x00\x00\x00\x06\x00\x00\x00\x05\x00\x00\x00\x05\x06\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\x05\x02\x00\x00\x00\x05\x00\x00\x00\x03\x00\x00\x00\x0C\x00\x00\x00\x05\x03\x00\x00\x00\x0B\x00\x00\x00\x02\x00\x00\x00\x08\x00\x00\x00\x05\x02\x00\x00\x00\x08\x00\x00\x00\xFE\xFF\xFF\xFF\x0A\x00\x00\x00\x05\xFD\xFF\xFF\xFF\x0E\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x00\x00\x05\x01\x00\x00\x00\x07\x00\x00\x00\xFE\xFF\xFF\xFF\x04\x00\x00\x00\x05\xFE\xFF\xFF\xFF\x04\x00\x00\x00\xF5\xFF\xFF\xFF\x04\x00\x00\x00\x00"
#define __BIN_2_TYPE 0x00000002
#define __BIN_2_SIZE 0x000000E0
#define __BIN_2 "\x02\x00\x05\xF8\xFF\xFF\xFF\x08\x00\x00\x00\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x05\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\xFB\xFF\xFF\xFF\x05\x00\x00\x00\x00\xFB\xFF\xFF\xFF\x01\x00\x00\x00\xFB\xFF\xFF\xFF\x05\x01\x00\x00\x00\xFB\xFF\xFF\xFF\x01\x00\x00\x00\x00\x00\x00\x00\x05\x01\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\x07\x00\x00\x00\x05\x0A\x00\x00\x00\x06\x00\x00\x00\x06\x00\x00\x00\x05\x00\x00\x00\x05\x06\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\x05\x02\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x0D\x00\x00\x00\x05\x03\x00\x00\x00\x0C\x00\x00\x00\x02\x00\x00\x00\x08\x00\x00\x00\x05\x02\x00\x00\x00\x08\x00\x00\x00\xFE\xFF\xFF\xFF\x0A\x00\x00\x00\x05\xFE\xFF\xFF\xFF\x0E\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x00\x00\x05\x01\x00\x00\x00\x07\x00\x00\x00\xFE\xFF\xFF\xFF\x04\x00\x00\x00\x05\xFE\xFF\xFF\xFF\x04\x00\x00\x00\xF8\xFF\xFF\xFF\x08\x00\x00\x00\x00"

U8 *poem=

"To a Waterfowl\0"
"by William Cullen Bryant\0"
" \0"
"Whither, 'midst falling dew,\0"
"While glow the heavens with the last steps of day,\0"
"Far, through their rosy depths, dost thou pursue\0"
"Thy solitary way?\0"
" \0"
"Vainly the fowler's eye\0"
"Might mark thy distant flight to do thee wrong,\0"
"As, darkly painted on the crimson sky,\0"
"Thy figure floats along.\0"
" \0"
"Seek'st thou the plashy brink\0"
"Of weedy lake, or marge of river wide,\0"
"Or where the rocking billows rise and sink\0"
"On the chafed ocean side?\0"
" \0"
"There is a Power whose care\0"
"Teaches thy way along that pathless coast,--\0"
"The desert and illimitable air,--\0"
"Lone wandering, but not lost.\0"
" \0"
"All day thy wings have fann'd\0"
"At that far height, the cold thin atmosphere:\0"
"Yet stoop not, weary, to the welcome land,\0"
"Though the dark night is near.\0"
" \0"
"And soon that toil shall end,\0"
"Soon shalt thou find a summer home, and rest,\0"
"And scream among thy fellows; reed shall bend\0"
"Soon o'er thy sheltered nest.\0"
" \0"
"Thou'rt gone, the abyss of heaven\0"
"Hath swallowed up thy form; yet, on my heart\0"
"Deeply hath sunk the lesson thou hast given,\0"
"And shall not soon depart.\0"
" \0"
"He, who, from zone to zone,\0"
"Guides through the boundless sky thy certain flight,\0"
"In the long way that I must tread alone,\0"
"Will lead my steps aright.\0";

#define POEM_LINES	42
#define POEM_COLS	53
#define POEM_TIME	80.0


/* <1> <1> (image) */




/* <2> <2> (image) */



#define NUM_GEESE	11
F64 t0,t0_s[NUM_GEESE];
I64 x[NUM_GEESE],y[NUM_GEESE];
I64 x0[NUM_GEESE],y0[NUM_GEESE];

U0 PutPoem(TaskStruct *task,GrDC *dc,I64 w,I64 h)
{
  F64 tt=Saw(tP(task)-t0,POEM_TIME);
  I64 i,j=tt*(POEM_LINES+4)*FONT_HEIGHT-h/2,
     x=w>>1-(POEM_COLS*FONT_WIDTH)>>1;
  U8 *st=poem;
  dc->color=DKGRAY;
  for (i=POEM_LINES-1;i>=0;i--) {
    GrPutS(dc,x,
      (FONT_HEIGHT*i+j)%((POEM_LINES+4)*FONT_HEIGHT),
      st);
    st+=StrLen(st)+1;
  }
}

U0 DrawIt(TaskStruct *task,GrDC *dc)
{
  U8 *inter;
  I64 i,w=task->win_pixel_width,h=task->win_pixel_height;
  F64 tt;
  task->text_attr=LTGRAY<<4+BLACK;
  PutPoem(task,dc,w,h);

  for (i=0;i<NUM_GEESE;i++) {
    tt=1.2*Tri(tP(task)-t0_s[i],0.7);
    inter=GrSpriteInterpolate(__BIN_1,__BIN_2,tt);
    GrSpritePlot3(dc,w>>1+x0[i]+x[i],h>>1+y0[i]+y[i],0,inter);
    Free(inter);
  }
}

U0 Init()
{
  I64 i,j;
  for (i=0;i<NUM_GEESE;i++) {
    j=i-NUM_GEESE>>1;
    t0_s[i]=ToF64(RandU16)/MAX_U16;
    x0[i]=j*20;
    y0[i]=AbsI64(j)*20;
    x[i]=5*RandI16/MAX_I16;
    y[i]=5*RandI16/MAX_I16;
  }
}

U0 AnimateTask(U64)
{
  I64 i;
  while (TRUE) {
    for (i=0;i<NUM_GEESE;i++) {
      x[i]=LimitI64(x[i]+SignI64(RandI16),-5,5);
      y[i]=LimitI64(y[i]+SignI64(RandI16),-5,5);
    }
    Sleep(100);
  }
}

U0 SongTask(U64)
{ //Song by Terry A. Davis
//This specifies a call-back routine
//to be called when the task dies.
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("3eEDCBCBCBAqReEDCBC");
    Play("BCBAqReFEDEDEDCqB");
    Play("ReFEDEDEDCBqR");
  }
}

U0 WaterFowl()
{
  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  Init;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",Fs);
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",Fs);
  WordStat;
  LtfCursor;
  LtfClear;
  Fs->draw_it=&DrawIt;

  t0=tP;
  try //This catches <CTRL-ALT-C> exits
    GetChar;
  catch
    CatchAll;
  SettingsPop;
}

WaterFowl;
