/*  <1> (image) */



























































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

F64 t0;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64	w=task->pix_width,h=task->pix_height;
  F64	t=1.04-(0.35+0.65*Caw(tS-t0,20))`3,s=1.0-0.5*t,
	x=w*t,y=h*(0.35-.1*t);
  dc->color=WHITE;
  GrPrint(dc,w>>1-(POEM_COLS*FONT_WIDTH)>>1,
	    (Saw(tS-t0,POEM_TIME)-.8)*POEM_LINES*FONT_HEIGHT, poem);
  dc->color=BLACK;
  GrLine(dc,x,y,x-8*s,y+6*s*FullTri(tS,0.4));
  GrLine(dc,x,y,x+8*s,y+6*s*FullTri(tS,0.4));
}

U0 SongTask(I64)
{//Song by Terry A. Davis
//This specifies a callback routine
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
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  WinMax;
  WinBorder;
  AutoComplete;
  DocClear;
  t0=tS;
  Fs->draw_it=&DrawIt;
  Sprite(__BIN_1);
  "%h*c",TEXT_ROWS-2,'\n';
  GetChar;
  DocClear;
  SettingsPop;
}

WaterFowl;
