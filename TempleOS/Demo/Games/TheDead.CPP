//These are the coordinates of the player.
I64 x=0,y=GR_HEIGHT/2;


//This is a FIFO (first-in first-out) data structure
//which keeps track of bullets.  When a new bullet is
//fired, the shots_in val is increased by one.
//When a bullet expires, the shots_out val is increased
//by one.  The in and out vals loop-around, back to
//the start of the arrays.
#define MAX_SHOTS 32
I64 shots_in=0,shots_out=0,sx[MAX_SHOTS],sy[MAX_SHOTS];



//This is a fifo which keeps track of the bad guys.
#define MAX_DEAD 32
I64 dead_in=0,dead_out=0,gx[MAX_DEAD],gy[MAX_DEAD];
Bool g_dead[MAX_DEAD];


//This is a graphic for the player's character.
//Graphics are created and edited by pressing
//<CTRL-R>.  When created, they are assigned
//a num.  Press <CTRL-T> right now, to see
//the hidden definition for the sprite.  The
//text in quotes, you can set to whatever you want,
//but the ending num can't be changed.	It
//is set when the graphic is created and is used
//in future references to the sprite.



/* <1> <1> (image) */






 
/* <2> <2> (image) */
 
 
 
 
 

/* <3> <3> (image) */








//This routine updates the window.  It is called
//by the window mgr system task.  The task refers to
//the task which owns the window.
U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,j;
  //<CTRL-T> now to see the hidden part
  //where the sprite num is encoded. IB stands
  //for "insert ptr to binary object".
  SpritePlot3(dc,x,y,0,__BIN_1);

  i=shots_out;
  while (i!=shots_in) {
    j=i++ &(MAX_SHOTS-1);
    GrLine(dc,sx[j],sy[j],sx[j]-2,sy[j]);
  }
  i=dead_out;
  while (i!=dead_in) {
    j=i++ &(MAX_SHOTS-1);
    if (!g_dead[j]) {
      if (gx[j]%10>5)
	SpritePlot3(dc,gx[j],gy[j],0,__BIN_2);
      else
	SpritePlot3(dc,gx[j],gy[j],0,__BIN_3);
    }
  }
  i=0;
  PlotWinStr(task,"If you aspire to making games,",BLUE<<4+YELLOW,0,i++);
  PlotWinStr(task,"you must learn to make-up rules.",BLUE<<4+YELLOW,0,i++);
}

U0 SongTask(I64)
{ //Randomly generate (by God :-)
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("qDqDsDCDCetBCBqCqCqCqCqDqDsDCDCetBCBqCqCqCqCqCq"
	 "CsCCCCetCCBeBBeBBqBqBqCqCsCCCCetCCBeBBeBBqBqB");
  }
}

U0 TheDead()
{
  I64 i,j,i1,j1,k=0,sc=0,ch=0,msg_code,p1,p2;
  Bool gun_on=FALSE;

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Fire(,CH_SPACE);"
  "  Up(,,SC_CURSOR_UP);"
  "  Down(,,SC_CURSOR_DOWN);"
      "}"
      );
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  WordStat;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  WinBorder;

  try {
    do {
      if (msg_code=ScanMsg(&p1,&p2,1<<MSG_KEY_DOWN | 1<<MSG_KEY_UP)) {
      //With KEYBOARD msgs, the two msg args
      //are the scan code and ascii val for the key.
	sc=p2; ch=p1;

	if (msg_code==MSG_KEY_DOWN) {
	//The least significant byte of the scan code
	//identifies the key.
	  if (sc.u8[0]==SC_CURSOR_DOWN) {
	    y+=10;
	    if (y>GR_HEIGHT-30)
	      y=GR_HEIGHT-30;
	  } else if (sc.u8[0]==SC_CURSOR_UP) {
	    y-=10;
	    if (y<0)
	      y=0;
	  } else if (sc.u8[0]==SC_CURSOR_RIGHT ||
	      ch==CH_SPACE)
	    gun_on=TRUE;
	} else if (msg_code==MSG_KEY_UP) {
	  if (sc.u8[0]==SC_CURSOR_RIGHT ||
	      ch==CH_SPACE)
	    gun_on=FALSE;
	}
      }

      i=shots_out;
      while (i!=shots_in) {
	j=i++ &(MAX_SHOTS-1);
	sx[j]+=5;
	if (sx[j]>GR_WIDTH)
	  shots_out++;
	else {
	  i1=dead_out;
	  while (i1!=dead_in) {
	    j1=i1++ &(MAX_DEAD-1);
	    if (gy[j1]<=sy[j]<=gy[j1]+38 &&
		gx[j1]<=sx[j]<=gx[j1]+40)
	      g_dead[j1]=TRUE;
	  }
	}
      }

      if (gun_on) {
	j=shots_in&(MAX_SHOTS-1);
	sx[j]=x+32;
	sy[j]=y+14;
	shots_in++;
      }

      //This runs one out of four passes through this loop.
      if (!(k % 4)) {
	i=dead_out;
	while (i!=dead_in) {
	  j=i++ &(MAX_DEAD-1);
	  gx[j]-=1;
	  if (gx[j]<25)
	    dead_out++;
	}
      }

      //This runs one out of 150 passes through this loop.
      if (!(k%150)) {
	j=dead_in&(MAX_DEAD-1);
	gx[j]=GR_WIDTH-30;
	gy[j]=RandU32%(GR_HEIGHT-50)+25;
	g_dead[j]=FALSE;
	dead_in++;
      }
      k++;

      Sleep(10); //Swap this task-out for 10 miliseconds.
    } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
}

TheDead;	// run program when #included
