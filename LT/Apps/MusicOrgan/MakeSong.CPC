class MakeSongSettings
{
  BoolI8 simple_rhythms		fmtstr "$CB,\"Simple Rhythms\"$\n";
  BoolI8 rests			fmtstr "$CB,\"Rests\"$\n";
  BoolI8 change_waveforms	fmtstr "$CB,\"Change Waveforms\"$\n";
  BoolI8 six_eight		fmtstr "$CB,\"Six Eight\"$\n";
};

U0 InsertNote(MakeSongSettings *mss,U8 *buf,U64 k,U64 *j)
{  //k is a random note nibble
  U64 a=TimerPickedNibble; //accent
  if (!a) {
    buf[*j]='!';
    *j+=1;
  }
  if (a<4) {
    buf[*j]='!';
    *j+=1;
  }
  if (!k && mss->rests) {
    buf[*j]='R';
    *j+=1;
  } else {
    k/=2;
    if (!k) {
      buf[*j]=music_octave-1+'0';
      *j+=1;
      buf[*j]='G';
      *j+=1;
      buf[*j]=music_octave+'0';
      *j+=1;
    } else {
      buf[*j]=k-1+'A';
      *j+=1;
    }
  }
}

#define DUR_QUARTER	0
#define DUR_EIGHTH	1
#define DUR_TRIPLET	2
#define DUR_SIXTEENTH	3

U8 simple_songs [5]={DUR_EIGHTH,DUR_QUARTER,DUR_QUARTER,DUR_QUARTER,DUR_QUARTER};
U8 complex_songs[6]={DUR_EIGHTH,DUR_TRIPLET,DUR_SIXTEENTH,DUR_QUARTER,DUR_QUARTER,DUR_QUARTER};

U8 *MakeSong()
{
  MakeSongSettings mss;
  U8 *buf=CAlloc(256);
  I64 i,j=0,k,n,k2,duration,last_duration=-1,
     wf[2],w=0,len;

  mss.simple_rhythms=FALSE;
  mss.rests=FALSE;
  mss.change_waveforms=FALSE;

  PopUpForm(&mss,"MakeSongSettings");

  progress4=0;
  progress4_max=8;

  n=TimerPickedNibble/2;
  if (WF_NULL<n<WF_NOISE)
    wf[0]=n;
  else
    wf[0]=WF_SQUARE;

  n=TimerPickedNibble/2;
  if (WF_NULL<n<WF_NOISE)
    wf[1]=n;
  else
    wf[1]=wf[0];

  if (mss.six_eight) {
    len=6;
    buf[j++]='M';
    buf[j++]='6';
    buf[j++]='/';
    buf[j++]='8';
  } else
    len=8;
  if (mss.change_waveforms) {
    buf[j++]='W';
    buf[j++]='0'+wf[w++&1];
  }
  for (i=0;i<len;i++) {
    if (mss.change_waveforms) {
      if (TimerPickedNibble<6) {
	buf[j++]='W';
	buf[j++]='0'+wf[w++&1];
      }
    }

    n=TimerPickedNibble;
    k=TimerPickedNibble;
    if (mss.simple_rhythms)
      duration=simple_songs[n%5];
    else
      duration=complex_songs[n%6];
    switch (duration) {
      case DUR_EIGHTH:
	if (last_duration!=DUR_EIGHTH)
	  buf[j++]='e';
	InsertNote(&mss,buf,k,&j);
	k=TimerPickedNibble;
	InsertNote(&mss,buf,k,&j);
	break;
      case DUR_TRIPLET:
	if (last_duration!=DUR_TRIPLET) {
	  buf[j++]='e';
	  buf[j++]='t';
	}
	InsertNote(&mss,buf,k,&j);
	k=TimerPickedNibble;
	InsertNote(&mss,buf,k,&j);
	k=TimerPickedNibble;
	InsertNote(&mss,buf,k,&j);
	break;
      case DUR_SIXTEENTH:
	if (last_duration!=DUR_SIXTEENTH)
	  buf[j++]='s';
	InsertNote(&mss,buf,k,&j);
	k2=k;
	k=TimerPickedNibble;
	InsertNote(&mss,buf,k,&j);
	InsertNote(&mss,buf,k2,&j);
	InsertNote(&mss,buf,k,&j);
	break;
      default:
	if (last_duration!=DUR_QUARTER)
	  buf[j++]='q';
	InsertNote(&mss,buf,k,&j);
    }
    last_duration=duration;
    progress4++;
  }
  buf[j++]=0;
  progress4=progress4_max=0;
  return buf;
}
