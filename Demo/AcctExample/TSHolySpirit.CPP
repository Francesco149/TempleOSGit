#define TS_HS_BAD_BITS		4
#define GODLE_HS_BAD_BITS	4
#define GODLE_HS_GOOD_BITS	24

Bool godle_done;
I64 godle_ch;
CDC *main_dc;
U0 GodleDraw(CTask *task,CDC *dc)
{
  GrBlot(dc,0,0,main_dc);
  if (Blink) {
    dc->color=LTRED;
    if (godle_done)
      GrPrint(dc,(task->win_pixel_width-FONT_WIDTH*4)>>1,(task->win_pixel_height-FONT_HEIGHT)>>1,
	"Done");
  }
}

U0 GodleSmooth(I64 num)
{
  CDC *dc=DCExtract(main_dc,0,0,main_dc->width-1,main_dc->height-1);
  I64 i,x,y,x1,y1,c,histogram[16],best,best_cnt,c_old=main_dc->color;
  for (y=0;y<main_dc->height;y++)
    for (x=0;x<main_dc->width;x++) {
      MemSet(histogram,0,sizeof(histogram));
      for (y1=y-num;y1<=y+num;y1++)
	for (x1=x-num;x1<=x+num;x1++) {
	  c=GrPeek(dc,x1,y1);
	  if (0<=c<=15)
	    histogram[c]++;
	}
      best=BLACK;
      best_cnt=-1;
      for (i=0;i<16;i++)
	if (histogram[i]>best_cnt) {
	  best=i;
	  best_cnt=histogram[i];
	}
      main_dc->color=best;
      GrPlot(main_dc,x,y);
    }
  main_dc->color=c_old;
  DCDel(dc);
}

CFifoU8 *godle_timer_pick_fifo=FifoU8New(32);
U0 GodleTimerPickFlush()
{
  FifoU8Flush(godle_timer_pick_fifo);
}

I64 GodleTimerPickedBits(I64 num_bits)
{
  U8 b;
  I64 i,k,result=0;
  while (num_bits) {
    if (FifoU8Rem(godle_timer_pick_fifo,&b)) {
      result=result<<1+b;
      num_bits--;
    } else {
      godle_ch=GetChar(,FALSE);
      if (godle_ch==CH_ESC||godle_ch==CH_SHIFT_ESC)
	throw;
      else if (godle_ch=='\n') {
	DCFill(main_dc,WHITE);
	FifoU8Flush(godle_timer_pick_fifo);
      } else if ('0'<=godle_ch<='9')
	GodleSmooth(godle_ch-'0');
      else {
	k=KbdMouseEvtTime>>GODLE_HS_BAD_BITS;
	for (i=0;i<GODLE_HS_GOOD_BITS;i++) {
	  FifoU8Ins(godle_timer_pick_fifo,k&1);
	  k>>=1;
	}
      }
    }
  }
  return result;
}

U0 BadCodeJump()
{ //[C:/Apps/HolySpirit/HSNotes.TXT.Z,1]
  CDirEntry *tempde1=FilesFind("/*",FUF_JUST_FILES|FUF_RECURSE|FUF_JUST_SRC),
	*tempde=tempde1;
  I64 cnt=0,num;
  CDoc *doc;
  U8 *st;
  while (tempde) {
    doc=DocRead(tempde->full_name);
    tempde->user_data=doc->root.y;
    cnt+=doc->root.y;
    DocDel(doc);
    tempde=tempde->next;
  }
  num=KbdMouseEvtTime>>TS_HS_BAD_BITS%cnt;
  tempde=tempde1;
  while (tempde) {
    num-=tempde->user_data;
    if (num<0) {
      st=MPrint("FL:%s,%d",tempde->full_name,-num);
      break;
    }
    tempde=tempde->next;
  }
  DirLstDel(tempde1);
  Ed(st);
  Free(st);
}

U0 DCImgWrite(CDC *dc)
{
  U8 *st=FileNameNumNext("::" TS_BLOG_DIR "/Img*","::" TS_BLOG_DIR "/Img%03d.BMP"),*st2;
  st2=MPrint("http://www.templeos.org/Wb%s",st+2);
  BMPFileWrite(st,dc);
  Free(st);
  "$HC,\"<img src=\\\"%s\\\" width=\\\"%d\\\" height=\\\"%d\\\" alt=\\\"\\\">\"$\n",st2,dc->width,dc->height;
  Free(st2);
}

U0 BlogScreenShot()
{
  Bool old_cursor=DocCursor;
  CDC *dc1,*dc2;
  U8 *old_fp_draw_input_ptr=fp_draw_input_ptr;
  fp_draw_input_ptr=NULL;
  WinMgrSync(2);
  dc1=DCCaptureScreen;
  dc2=DCExtract(dc1,0,FONT_HEIGHT,GR_WIDTH-1,GR_HEIGHT-1);
  DCImgWrite(dc2);
  fp_draw_input_ptr=old_fp_draw_input_ptr;
  DCDel(dc1);
  DCDel(dc2);
  DocCursor(old_cursor);
}

U0 BlogGodle()
{
  I64 i,j,w,h,x,y,ch;
  godle_done=FALSE;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  WinBorder;
  WordStat;

  WinMgrSync;
  godle_ch=0;
  main_dc=DCNew(Fs->win_pixel_width,Fs->win_pixel_height);
  DCFill(main_dc,WHITE);
  w=main_dc->width;
  h=main_dc->height;

  Fs->draw_it=&GodleDraw;
  GodleTimerPickFlush;
  try {
    for (i=0;i<3;i++) {
      main_dc->color=RED;
      for (j=0;j<24;j++)
	nobound_switch (GodleTimerPickedBits(3)) {
	  case 0:
	    GrEllipse(main_dc,
	      (w-1)*GodleTimerPickedBits(5)/15.5-w/2,(h-1)*GodleTimerPickedBits(5)/15.5-h/2,
	      (w-1)*GodleTimerPickedBits(5)/15.5,(h-1)*GodleTimerPickedBits(5)/15.5);
	    break;
	  case 1:
	    GrCircle(main_dc,
	      (w-1)*GodleTimerPickedBits(5)/15.5-w/2,(h-1)*GodleTimerPickedBits(5)/15.5-h/2,
	      (w-1)*GodleTimerPickedBits(5)/15.5);
	    break;
	  case 2:
	    GrLineRect4(main_dc,
	      (w-1)*GodleTimerPickedBits(5)/15.5-w/2,(h-1)*GodleTimerPickedBits(5)/15.5-h/2,
	      (w-1)*GodleTimerPickedBits(5)/15.5,(h-1)*GodleTimerPickedBits(5)/15.5);
	    break;
	  case 3...7:
	    GrLine(main_dc,
	      (w-1)*GodleTimerPickedBits(4)/15,(h-1)*GodleTimerPickedBits(4)/15,
	      (w-1)*GodleTimerPickedBits(4)/15,(h-1)*GodleTimerPickedBits(4)/15);
	    break;
	}
      for (j=4;j;) {
	x=(w-1)*GodleTimerPickedBits(5)/31+w/64;
	y=(h-1)*GodleTimerPickedBits(5)/31+h/64;
	switch(GrPeek(main_dc,x,y)) {
	  case BLACK:
	    main_dc->color=WHITE;
	    GrFloodFill(main_dc,x,y);
	    j--;
	    break;
	  case WHITE:
	    main_dc->color=BLACK;
	    GrFloodFill(main_dc,x,y);
	    j--;
	    break;
	}
      }
      GodleSmooth(3);
    }
    godle_done=TRUE;
    do ch=GetChar(,FALSE);
    while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
  } catch {
    Fs->catch_except=TRUE;
    ch=CH_SHIFT_ESC;
  }
  DCFill;
  SettingsPop;
  if (ch==CH_ESC) DCImgWrite(main_dc);
  DCDel(main_dc);
}

U8 *WebBookMarks(U8 *filename="HOME/Archive/Bookmarks.html")
{
  U8 *st;
  CDocEntry *doc_e;
  CDoc *doc=DocNew;
  CLex *lx=LexNew(TextFileRead(filename),0,filename);
  while (Lex(lx))
    if (lx->token==TK_STR && *lx->ident(U32 *)=='http') {
      DocPrint(doc,"$MU-UL,\"%Q\",0x%X$\n",lx->ident,lx->ident);
      lx->ident=NULL;
    }
  LexDel(lx);

  st=StrNew(PopUpMenu(doc));

  doc_e=doc->root.next;
  while (doc_e!=doc) {
    if (doc_e->type_u8==DOCT_MENU_VAL)
      Free(doc_e->left_exp);
    doc_e=doc_e->next;
  }
  DocDel(doc);

  return st;
}

class CWebBookMarkForm
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text     :%s\"$\n";
  U8	special[136];
  I64	timestamp;
  I64	cert;
  I64	min;
  I64	sec;
};

class CWebBibleForm
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text     :%s\"$\n";
  U8	special[136]	fmtstr "$DA-P,135,\"Bible Passage:%s\"$\n";
  I64	timestamp;
  I64	cert;
  I64	min;
  I64	sec;
};

class CYouTubeForm
{
  U8	tag[136];
  U8	special[136]	fmtstr "$DA-P,135,\"SerialNum:%s\"$\n";
  I64	timestamp;
  I64	cert;
  I64	min		fmtstr "$DA,32,   \"Minutes  :%d\"$\n";
  I64	sec		fmtstr "$DA,32,   \"Seconds  :%d\"$\n";
};

class CWikipediaForm
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text:%s\"$\n";
  U8	special[136]	fmtstr "$DA-P,135,\"Index   :%s\"$\n";
  I64	timestamp;
  I64	cert;
  I64	min;
  I64	sec;
};

class CRandDecForm
{
  U8	tag[136];
  U8	special[136]	fmtstr "$DA-P,135,\"Line (Dec):%s\"$\n";
  I64	timestamp;
  I64	cert;
  I64	min;
  I64	sec;
};

class CRandHexForm
{
  U8	tag[136];
  U8	special[136]	fmtstr "$DA-P,135,\"Line (Hex):%s\"$\n";
  I64	timestamp;
  I64	cert;
  I64	min;
  I64	sec;
};

class CRandCertForm
{
  U8	tag[136];
  U8	special[136]	fmtstr "$DA-P,135,\"Line (Dec) :%s\"$\n";
  I64	timestamp;
  I64	cert		fmtstr "$DA,32,   \"Certificate:%d\"$\n";
  I64	min;
  I64	sec;
};

class CRandHexBeaconForm
{
  U8	tag[136];
  U8	special[136]	fmtstr "$DA-P,135,\"Line (Hex):%s\"$\n";
  I64	timestamp	fmtstr "$DA,32,   \"TimeStamp :%d\"$\n";
  I64	cert;
  I64	min;
  I64	sec;
};

U8 *URLPercentSpaces(U8 *src)
{
  U8 buf[1024],*dst=buf;
  while (*src) {
    if (*src==CH_SPACE) {
      *dst++='%';
      *dst++='%';
      *dst++='2';
      *dst++='0';
      src++;
    } else
      *dst++=*src++;
  }
  *dst=0;
  return StrNew(buf);
}

U0 MyBibleLines(U8 *st,I64 num_lines,I64 radix)
{
  CDoc *doc_out,*doc_in;
  CDocEntry *doc_e;
  I64 i,start;
  if (*st && (doc_out=DocPut)) {
    i=A2I(st,radix);
    start=i%(ST_BIBLE_LINES-(num_lines-1))+1;
    if (radix==16)
      DocPrint(doc_out,"0x%s-->",st);
    DocPrint(doc_out,
	"(%d mod %d)+1-->Line:%d\n"
	"KingJamesBible: God says...\n$RED$",
	i,ST_BIBLE_LINES-(num_lines-1),start);
    doc_in=DocNew;
    DocBibleLines(doc_in,start,num_lines);
    while (TRUE) { //Del leading blanks
      doc_e=doc_in->root.next;
      if (doc_e==doc_in)
	break;
      else {
	if (doc_e->type_u8==DOCT_NEW_LINE)
	  DocEntryDel(doc_in,doc_e);
	else
	  break;
      }
    }
    while (TRUE) { //Del trailing blanks
      doc_e=doc_in->cur_entry->last;
      if (doc_e==doc_in)
	break;
      else {
	if (doc_e->type_u8==DOCT_NEW_LINE)
	  DocEntryDel(doc_in,doc_e);
	else
	  break;
      }
    }
    DocPrint(doc_in,"$FG$\n");
    DocInsDoc(doc_out,doc_in);
    DocDel(doc_in);
  }
}

#define NIST_TIME_OFFSET	(6*3600-335)

public U0 SyncNIST()
{
  while (!ScanChar) {
    "%d %d\n",
	ToI64	((Now-A2Date("1/1/1970"))/CDATE_FREQ+NIST_TIME_OFFSET),
	FloorI64((Now-A2Date("1/1/1970"))/CDATE_FREQ+NIST_TIME_OFFSET,60);
    Sleep(100);
  }
}

U0 FillInMyURL()
{
  U8 *tag,*st,*st2;
  CWebBibleForm *url=CAlloc(sizeof(CWebBibleForm));
  url->timestamp=
	FloorI64((Now-A2Date("1/1/1970"))/CDATE_FREQ+NIST_TIME_OFFSET,60);
  switch (PopUpPickLst("NISTBeacon\0WebBible\0YouTube\0Wikipedia\0RandomNumbers.info\0"
	"Random.org\0VirtualNotary\0Nist\0HotBits\0GenerateData\0WebBookMark\0")) {
    case 0:
      if (DocFormDo(url(CRandHexBeaconForm *))) {
	"$TX+HTML,\"National Institute of Standards Public Randomness Beacon\","
	"\"http://beacon.nist.gov/home\"$\n\n"
	"$TX+HTML,\"Guidelines for Talking with God\","
	"\"http://www.templeos.org/Wb/Apps/HolySpirit/HSNotes.html\"$\n\n";
	tag=MPrint("http://beacon.nist.gov/rest/record/%d",url->timestamp);
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	Free(tag);
	MyBibleLines(url->special,20,16);
      }
      break;
    case 1:
      if (DocFormDo(url(CWebBibleForm *))) {
	tag=url->special;
	st2=URLPercentSpaces(url->special);
	st=MPrint("http://www.biblegateway.com/passage/?search=%s&version=NIV",st2);
	Free(st2);
	if (*url->tag)
	  tag=url->tag;
	"$TX+HTML,\"%Q\",\"%Q\"$",tag,st;
	Free(st);
      }
      break;
    case 2:
      if (DocFormDo(url(CYouTubeForm *))) {
	if (url->min || url->sec)
	  st=MPrint("%s&hl=enUS&start=%d",url->special,url->min*60+url->sec);
	else
	  st=StrNew(url->special);
	"$HC,\"<object width=\\\"640\\\" height=\\\"520\\\"><param name=\\\"movie\\\" "
	"value=\\\"http://www.youtube.com/v/%s\\\"></param><param "
	"name=\\\"allowscriptaccess\\\" value=\\\"always\\\"></param><embed "
	"src=\\\"http://www.youtube.com/v/%s\\\" "
	"type=\\\"application/x-shockwave-flash\\\" allowscriptaccess=\\\"always\\\" "
	"width=\\\"640\\\" height=\\\"520\\\"></embed></object>\"$\n",st,st;
	Free(st);
      }
      break;
    case 3:
      if (DocFormDo(url(CWikipediaForm *))) {
	tag=url->special;
	st=MPrint("http://en.wikipedia.org/wiki/%s",url->special);
	if (*url->tag)
	  tag=url->tag;
	"$TX+HTML,\"%Q\",\"%Q\"$",tag,st;
	Free(st);
      }
      break;
    case 4:
      if (DocFormDo(url(CRandDecForm *))) {
	tag="http://www.randomnumbers.info";
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	MyBibleLines(url->special,20,10);
      }
      break;
    case 5:
      if (DocFormDo(url(CRandDecForm *))) {
	tag="http://www.random.org";
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	MyBibleLines(url->special,20,10);
      }
      break;
    case 6:
      if (DocFormDo(url(CRandCertForm *))) {
	tag="http://virtual-notary.org";
	"$TX+HTML,\"%Q\",\"%Q\"$\n",tag,tag;
	tag="Here is the certificate";
	st=MPrint("http://www.templeos.org/Certs/virtual-notary-cert-randomnum-%d.p12",url->cert);
	"\n$TX+HTML,\"%Q: %d\",\"%Q\"$.\n\n",tag,url->cert,st;
	Free(st);
	MyBibleLines(url->special,20,10);
      }
      break;
    case 7:
      if (DocFormDo(url(CRandHexForm *))) {
	tag="http://qrng.anu.edu.au/NIST.php";
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	MyBibleLines(url->special,20,16);
      }
      break;
    case 8:
      if (DocFormDo(url(CRandHexForm *))) {
	tag="https://www.fourmilab.ch/hotbits/secure_generate.html";
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	MyBibleLines(url->special,20,16);
      }
      break;
    case 9:
      if (DocFormDo(url(CRandDecForm *))) {
	tag="http://www.generatedata.com";
	"$TX+HTML,\"%Q\",\"%Q\"$\n\n",tag,tag;
	MyBibleLines(url->special,20,10);
      }
      break;
    case 10:
      if (DocFormDo(url(CWebBookMarkForm *))) {
	tag=st=WebBookMarks;
	if (*url->tag)
	  tag=url->tag;
	"$TX+HTML,\"%Q\",\"%Q\"$",tag,st;
	Free(st);
      }
      break;
  }
  Free(url);
}
