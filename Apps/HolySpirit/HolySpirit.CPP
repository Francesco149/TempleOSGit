#help_index "Holy Spirit"

U8 **hs_words=NULL;
I64 hs_num_words=0;
CFifoU8 *timer_pick_fifo=FifoU8New(32);

#define HS_BAD_BITS	4
#define HS_GOOD_BITS	24

U8 *TimeStampCB(CDoc *,CDocEntry *,CTask *mem_task)
{
  U8 *st=MAlloc(64,mem_task);
  SPrint(st,"%X",GetTSC>>HS_BAD_BITS);
  return st;
}

U8 *KbdMouseTimeCB(CDoc *,CDocEntry *,CTask *mem_task)
{
  U8 *st=MAlloc(64,mem_task);
  SPrint(st,"%X",KbdMouseEvtTime>>HS_BAD_BITS);
  return st;
}

I64 PopUpTimerOk(U8 *header=NULL,U8 *footer=NULL)
{
  I64 i;
  CDocEntry *doc_e;
  CDoc *doc=DocNew;
  if (header) DocPrint(doc,"%s",header);
  doc_e=DocPrint(doc,"\nTimer:$TX+TC,\" \"$");
  doc_e->tag_cb=&TimeStampCB;
  doc_e=DocPrint(doc,"\nLatch:$TX+TC,\" \"$");
  doc_e->tag_cb=&KbdMouseTimeCB;
  DocPrint(doc,"\n$CM+CX,0,4$$BT,\"OKAY\",1$\n");
  if (footer) DocPrint(doc,"%s",footer);
  i=PopUpMenu(doc);
  DocDel(doc);
  return i;
}

public I64 TimerPick(U8 *msg=NULL)
{ //HS_GOOD_BITS
  U8 *st;
  if (msg) {
    st=MAlloc(StrLen(msg)+256);
    StrCpy(st,msg);
  } else
    st=CAlloc(256);
  StrCat(st,"\n\nPress $GREEN$OKAY$FG$ to generate \n"
	"a random num from a timer.\n");
  PopUpTimerOk(st,"\n\n$WW,1$The [C:/Apps/HolySpirit/HSNotes.TXT,1] Holy Spirit can puppet you.\n\n");
  Free(st);
  return KbdMouseEvtTime>>HS_BAD_BITS;
}

public U0 TimerPickFlush()
{
  FifoU8Flush(timer_pick_fifo);
}

public U0 TimerPickIns(I64 num_bits,I64 n)
{
  I64 i;
  for (i=0;i<num_bits;i++) {
    FifoU8Ins(timer_pick_fifo,n&1);
    n>>=1;
  }
}

public I64 TimerPickedBits(I64 num_bits,U8 *msg=NULL)
{
  U8 b;
  I64 result=0;
  while (num_bits) {
    if (FifoU8Rem(timer_pick_fifo,&b)) {
      result=result<<1+b;
      num_bits--;
    } else
      TimerPickIns(HS_GOOD_BITS,TimerPick(msg));
  }
  return result;
}

public I64 HSInit(U8 *files_find_mask="/Apps/HolySpirit/Vocab.TXT*",U8 *fu_flags=NULL)
{
  I64 i,ch,fuf_flags=0;
  U8 *buf,*ptr,*ptr2;
  CDirEntry *tempde,*tempde1;
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),"+r+f+F+T");
  ScanFlags(&fuf_flags,Define("ST_FILE_UTIL_FLAGS"),fu_flags);
  if (fuf_flags&~FUG_FILES_FIND)
    throw('FUF');
  tempde=tempde1=FilesFind(files_find_mask,fuf_flags);
  i=0;
  while (tempde) {
    if (buf=ptr=TextFileRead(tempde->full_name)) {
      while (*ptr) {
	while (*ptr && !Bt(chars_bitmap_word,*ptr))
	  ptr++;
	if (*ptr) {
	  ptr2=ptr;
	  while (*ptr && Bt(chars_bitmap_word,*ptr))
	    ptr++;
	  i++;
	}
      }
      Free(buf);
    }
    tempde=tempde->next;
  }

  Free(hs_words);
  hs_num_words=i;
  hs_words=MAlloc(i*sizeof(U8 *));

  tempde=tempde1;
  i=0;
  while (tempde) {
    if (buf=ptr=TextFileRead(tempde->full_name)) {
      while (*ptr) {
	while (*ptr && !Bt(chars_bitmap_word,*ptr))
	  ptr++;
	if (*ptr) {
	  ptr2=ptr;
	  while (*ptr && Bt(chars_bitmap_word,*ptr))
	    ptr++;
	  ch=*ptr;
	  *ptr=0;
	  hs_words[i++]=StrNew(ptr2);
	  *ptr=ch;
	}
      }
      Free(buf);
    }
    tempde=tempde->next;
  }
  DirLstDel(tempde1);
  return hs_num_words;
} HSInit;

public U0 GodWord()
{// [C:/Apps/HolySpirit/HSNotes.TXT,1] Holy Spirit Instructions
  if (hs_num_words)
    "%s ",hs_words[TimerPickedBits(17)%hs_num_words];
}

public U0 GodQuote(I64 num_lines=20)
{ // [C:/Apps/HolySpirit/HSNotes.TXT,1] Holy Spirit Instructions
  I64 start=TimerPickedBits(21)%(ST_BIBLE_LINES-(num_lines-1))+1;
  DocBibleLines(,start,num_lines);
}
