U8 *poem=
"Will lead my steps aright.
In the long way that I must tread alone,
Guides through the boundless sky thy certain flight,
He, who, from zone to zone,

And shall not soon depart.
Deeply hath sunk the lesson thou hast given,
Hath swallowed up thy form; yet, on my heart
Thou'rt gone, the abyss of heaven

Soon o'er thy sheltered nest.
And scream among thy fellows; reed shall bend
Soon shalt thou find a summer home, and rest,
And soon that toil shall end,

Though the dark night is near.
Yet stoop not, weary, to the welcome land,
At that far height, the cold thin atmosphere:
All day thy wings have fann'd

Lone wandering, but not lost.
The desert and illimitable air,--
Teaches thy way along that pathless coast,--
There is a Power whose care

On the chafed ocean side?
Or where the rocking billows rise and sink
Of weedy lake, or marge of river wide,
Seek'st thou the plashy brink

Thy figure floats along.
As, darkly painted on the crimson sky,
Might mark thy distant flight to do thee wrong,
Vainly the fowler's eye

Thy solitary way?
Far, through their rosy depths, dost thou pursue
While glow the heavens with the last steps of day,
Whither, 'midst falling dew,
 
by William Cullen Bryant
To a Waterfowl;
";

#define POEM_LINES	42
#define POEM_COLS	53
#define POEM_TIME	80.0


/* <1> <1> (image) */




/* <2> <2> (image) */


#define NUM_GEESE	11
F64 t0,t0_s[NUM_GEESE];
I64 x[NUM_GEESE],y[NUM_GEESE];
I64 x0[NUM_GEESE],y0[NUM_GEESE];

U0 DrawIt(CTask *task,CDC *dc)
{
  U8 *temps;
  I64 i,w=task->win_pixel_width,h=task->win_pixel_height;

  dc->color=DKGRAY;
  GrPrint(dc,w>>1-(POEM_COLS*FONT_WIDTH)>>1,
	    (Saw(tT-t0,POEM_TIME)-.8)*POEM_LINES*FONT_HEIGHT, poem);

  for (i=0;i<NUM_GEESE;i++) {
    temps=SpriteInterpolate(__BIN_1,__BIN_2,1.2*Tri(tT-t0_s[i],0.7));
    SpritePlot3(dc,w>>1+x0[i]+x[i],h>>1+y0[i]+y[i],0,temps);
    Free(temps);
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

U0 AnimateTask(I64)
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

U0 SongTask(I64)
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
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=LTGRAY<<4+BLACK;
  WinMax;
  Init;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  WordStat;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  t0=tT;
  try //This catches <CTRL-ALT-C> exits
    GetChar;
  catch
    PutExcept;
  SettingsPop;
}

WaterFowl;
