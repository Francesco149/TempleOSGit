/*  <1> (image) */



























































U8 *poem=
"To a Waterfowl
by William Cullen Bryant

Whither, 'midst falling dew,
While glow the heavens with the last steps of day,
Far, through their rosy depths, dost thou pursue
Thy solitary way?

Vainly the fowler's eye
Might mark thy distant flight to do thee wrong,
As, darkly painted on the crimson sky,
Thy figure floats along.

Seek'st thou the plashy brink
Of weedy lake, or marge of river wide,
Or where the rocking billows rise and sink
On the chafed ocean side?

There is a Power whose care
Teaches thy way along that pathless coast,--
The desert and illimitable air,--
Lone wandering, but not lost.

All day thy wings have fann'd
At that far height, the cold thin atmosphere:
Yet stoop not, weary, to the welcome land,
Though the dark night is near.

And soon that toil shall end,
Soon shalt thou find a summer home, and rest,
And scream among thy fellows; reed shall bend
Soon o'er thy sheltered nest.

Thou'rt gone, the abyss of heaven
Hath swallowed up thy form; yet, on my heart
Deeply hath sunk the lesson thou hast given,
And shall not soon depart.

He, who, from zone to zone,
Guides through the boundless sky thy certain flight,
In the long way that I must tread alone,
Will lead my steps aright.
";

#define POEM_COLS	53
#define POEM_TIME	65.0

F64 t0;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64	w=task->pix_width,h=task->pix_height;
  F64	t=1.04-(0.35+0.65*Caw(tS-t0,20))`3,s=1.0-0.5*t,
	x=w*t,y=h*(0.35-.1*t);
  dc->color=WHITE;
  GrPrint(dc,w>>1-(POEM_COLS*FONT_WIDTH)>>1,
	Caw(tS-t0,POEM_TIME)*(h-3*FONT_HEIGHT),poem);
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
