//These are the coordinates of the player.
I64 x=0,y=GR_HEIGHT/2;

/*This is a FIFO (first-in first-out) data structure
which keeps track of bullets.  When a new bullet is
fired, the bullets_in val is increased by one.
When a bullet expires, the bullets_out val is increased
by one.  The in and out vals loop-around, back to
the start of the arrays.
*/
#define MAX_BULLETS 32
I64 bullets_in=0,bullets_out=0,bx[MAX_BULLETS],by[MAX_BULLETS];

//This is a fifo which keeps track of the bad guys.
#define MAX_DEAD 32
I64 dead_in=0,dead_out=0,gx[MAX_DEAD],gy[MAX_DEAD];
Bool g_dead[MAX_DEAD];

/*This is a sprite created and edited by pressing
<CTRL-r>.  When created, they are assigned
a num.	Press <CTRL-t>, to see the hidden [C:/Doc/DolDocOverview.TXT.Z,1] DolDoc
place holder for the sprite.  The text in quotes
can be set to whatever you want or nothing,
but the ending num can't be changed.  It
is controled by the editor.
*/
/* <1> <1> (image) */



 
/* <2> <2> (image) */
 
 


/* <3> <3> (image) */




//Called by the window mgr bystem task 30fps.
//The task arg is the task owning the window.
U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,j;
  //<CTRL-t> now to see the [C:/Doc/DolDocOverview.TXT.Z,1] DolDoc place holder
  //where the sprite num is encoded. $IB...$ stands
  //for "insert ptr to binary object".
  Sprite3(dc,x,y,0,__BIN_1);

  i=bullets_out;
  while (i!=bullets_in) {
    j=i++ &(MAX_BULLETS-1);
    GrLine(dc,bx[j],by[j],bx[j]-2,by[j]);
  }
  i=dead_out;
  while (i!=dead_in) {
    j=i++ &(MAX_BULLETS-1);
    if (!g_dead[j]) {
      if (gx[j]%10>4)
	Sprite3(dc,gx[j],gy[j],0,__BIN_2);
      else
	Sprite3(dc,gx[j],gy[j],0,__BIN_3);
    }
  }
  dc->color=BLACK;
  GrPrint(dc,0,0,"If you aspire to making games,");
  GrPrint(dc,0,FONT_HEIGHT,"you must learn to make-up rules.");
}

U0 SongTask(I64)
{//Randomly generate (by God :-)
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  while (TRUE) {
    Play("qDqDsDCDCetBCBqCqCqCqCqDqDsDCDCetBCBqCqCqCqCqCq"
	 "CsCCCCetCCBeBBeBBqBqBqCqCsCCCCetCCBeBBeBBqBqB");
  }
}

U0 TheDead()
{
  I64 i,j,i1,j1,k=0,sc=0,ch=0,msg_code,a1,a2;
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
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);
  AutoComplete;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  WinBorder;

  try {
    do {
      if (msg_code=ScanMsg(&a1,&a2,1<<MSG_KEY_DOWN | 1<<MSG_KEY_UP)) {
      //With [C:/Doc/CharOverview.TXT.Z,1] keyboard msgs, the two msg args
      //are the scan code and ascii val for the key.
	sc=a2; ch=a1;

	if (msg_code==MSG_KEY_DOWN) {
	//The low byte of the scan code identifies the key.
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

      i=bullets_out;
      while (i!=bullets_in) {
	j=i++ &(MAX_BULLETS-1);
	bx[j]+=5;
	if (bx[j]>GR_WIDTH)
	  bullets_out++;
	else {
	  i1=dead_out;
	  while (i1!=dead_in) {
	    j1=i1++ &(MAX_DEAD-1);
	    if (gy[j1]<=by[j]<=gy[j1]+38 &&
		gx[j1]<=bx[j]<=gx[j1]+40)
	      g_dead[j1]=TRUE;
	  }
	}
      }

      if (gun_on) {
	j=bullets_in&(MAX_BULLETS-1);
	bx[j]=x+32;
	by[j]=y+14;
	bullets_in++;
      }

      //Runs one out of four passes through this loop.
      if (!(k % 4)) {
	i=dead_out;
	while (i!=dead_in) {
	  j=i++ &(MAX_DEAD-1);
	  gx[j]-=1;
	  if (gx[j]<25)
	    dead_out++;
	}
      }

      //Runs one out of 150 passes through this loop.
      if (!(k%150)) {
	j=dead_in&(MAX_DEAD-1);
	gx[j]=GR_WIDTH-30;
	gy[j]=RandU32%(GR_HEIGHT-50)+25;
	g_dead[j]=FALSE;
	dead_in++;
      }
      k++;

      Sleep(10); //Swap this task out for 10 miliseconds.
    } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  } catch
    PutExcept;
  SettingsPop;
  MenuPop;
}

TheDead; //Run program when #included
