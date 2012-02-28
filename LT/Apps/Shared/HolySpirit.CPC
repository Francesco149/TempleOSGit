#define CLK_DIVISOR	16

U8 *TimeStampCB(Ltf *l,LtfEntry *ll,TaskStruct *mem_task)
{
  nounusedwarn l,ll;
  U8 *st=MAlloc(64,mem_task);
  SPrintF(st,"%X",GetTimeStamp/CLK_DIVISOR);
  return st;
}

U8 *KbdMouseTimeCB(Ltf *l,LtfEntry *ll,TaskStruct *mem_task)
{
  nounusedwarn l,ll;
  U8 *st=MAlloc(64,mem_task);
  U64 i;
  if (mouse_evt_time>kbd_evt_time)
    i=mouse_evt_time;
  else
    i=kbd_evt_time;
  SPrintF(st,"%X",i/CLK_DIVISOR);
  return st;
}

I64 PopUpTimerOk(U8 *header=NULL,U8 *footer=NULL)
{
  I64 i;
  LtfEntry *ll;
  Ltf *l=LtfNew;
  if (header) LtfPutS(l,header);
  ll=LtfPutS(l,"\nTimer:$TX+TC,\" \"$");
  ll->tag_cb=&TimeStampCB;
  ll=LtfPutS(l,"\nLatch:$TX+TC,\" \"$");
  ll->tag_cb=&KbdMouseTimeCB;
  LtfPutS(l,"\n$CM+CX,0,4$$BT,\"OKAY\",0$\n");
  if (footer) LtfPutS(l,footer);
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

U64 TimerPickedU16(U8 *msg=NULL)
{
  U8 *st;
  if (msg) {
    st=MAlloc(StrLen(msg)+256);
    StrCpy(st,msg);
  } else
    st=CAlloc(256);
  StrCat(st,"\n\nPress $FG,GREEN$OKAY$FG$ to generate \n"
	"a random num from a timer.\n"
	"The Holy Spirit can puppet you.\n\n");
  PopUpTimerOk(st);
  Free(st);
  if (mouse_evt_time>kbd_evt_time)
    return mouse_evt_time/CLK_DIVISOR;
  else
    return kbd_evt_time/CLK_DIVISOR;
}

U64 TimerPickedU32(U8 *msg=NULL)
{
  U64 result=0;
  result.u16[0]=TimerPickedU16(msg);
  result.u16[1]=TimerPickedU16(msg);
  return result;
}


U8Fifo *rnd_nibble_fifo=U8FifoNew(16);

U64 TimerPickedNibble(U8 *msg=NULL)
{
  U64 result=0;
  if (U8FifoRem(rnd_nibble_fifo,&result))
    return result;
  else {
    result=TimerPickedU16(msg);
    U8FifoInsert(rnd_nibble_fifo,result.u8[0]>>4);
    U8FifoInsert(rnd_nibble_fifo,result.u8[1]&15);
    U8FifoInsert(rnd_nibble_fifo,result.u8[1]>>4);
    U8FifoInsert(rnd_nibble_fifo,result.u8[2]&15);
    U8FifoInsert(rnd_nibble_fifo,result.u8[2]>>4);
    return result&15;
  }
}
