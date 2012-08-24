//Change anything you want in this
//acct file or others.

U0 FillInTime()
{
  LTDate ltdt;
  ltdt=GetCurTimeLTDate;
  "$IV,1$----%D %T----$IV,0$\n",ltdt,ltdt;
}

U0 RandQuote(I64 num)
{
  I64 i,size,l=512;
  U8 *bible_filename=Define("ST_BIBLE_FILE");
  U8 *buf=FileRead(bible_filename,&size);
  if (buf) {
    i=num%(size-l);
    buf[i+l]=0;
    "" buf+i;
    Free(buf);
  }
}

U0 BadCodeJump(I64 num)
{
  LTDirEntry *root=FilesFind("/LT/*",FUF_JUST_FILES|FUF_RECURSE|FUF_FLATTEN_TREE|FUF_JUST_SRC_FILES),
	     *tempm=root;
  I64 cnt=0;
  Ltf *l;
  U8 *st;
  while (tempm) {
    l=LtfRead(tempm->full_name);
    tempm->user_data=l->root.y;
    cnt+=l->root.y;
    LtfDel(l);
    tempm=tempm->next;
  }
  num=num%cnt;
  tempm=root;
  while (tempm) {
    num-=tempm->user_data;
    if (num<0) {
      st=MSPrintF("FL:%s,%d",tempm->full_name,-num);
      break;
    }
    tempm=tempm->next;
  }
  LTDirListDel(root);
  Ed(st);
  Free(st);
}

BoolI64 MyPutKey(U64 ch,U64 sc)
{  //ch=ASCII; sc=scancode

//See [Char] Char for definition of scan codes.
//See [::/LT/Doc/KeyBoard.TXZ] Keyboard Handling

//You can customize keys.  This routine
//is called before the main editor
//key handler [C:/LT/Adam/Ltf/LtfEd.APZ,1394] LtfPutKey().
//You can intercept any key.

//Return TRUE if you completely
//handled the key.

  U64 i,sc2=sc.u8[0];

  if (SC_F1<=sc2<=SC_F10 &&
      sc&SCF_ALT &&
      !(sc & SCF_CTRL)) {
    switch (sc2) {
      case SC_F1:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Dollar/LTPURPLE");
	  else
	    SetKeyDesc("Dollar/PURPLE");
	} else {
	  if (sc & SCF_SHIFT)
	    "$FG,LTPURPLE$";
	  else
	    "$FG,PURPLE$";
	}
	break;
      case SC_F2:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Dollar/LTRED");
	  else
	    SetKeyDesc("Dollar/RED");
	} else {
	  if (sc & SCF_SHIFT)
	    "$FG,LTRED$";
	  else
	    "$FG,RED$";
	}
	break;
      case SC_F3:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Dollar/LTGREEN");
	  else
	    SetKeyDesc("Dollar/GREEN");
	} else {
	  if (sc & SCF_SHIFT)
	    "$FG,LTGREEN$";
	  else
	    "$FG,GREEN$";
	}
	break;
      case SC_F4:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Dollar/LTBLUE");
	  else
	    SetKeyDesc("Dollar/Default Color");
	} else {
	  if (sc & SCF_SHIFT)
	    "$FG,LTBLUE$";
	  else
	    "$FG$";
	}
	break;
      case SC_F5:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("God Passage");
	  else
	    SetKeyDesc("God Word");
	} else {
	  if (sc & SCF_SHIFT)
	    RandQuote(GetTimeStamp/16);
	  else
	    "%s ",ws_sorted_words[GetTimeStamp/16%ws_num_words]->str;
	}
	break;
      case SC_F6:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Cmd/Init WordStat Text");
	  else
	    SetKeyDesc("Cmd/Init WordStat LT");
	} else {
	  if (sc & SCF_SHIFT)
	    PopUp("WsInit(\"::/LT/Misc/Bible.TXZ\");");
	  else
	    PopUp("WsInit(\"::/LT/\" TEXT_FILE_MASK \";~*Bible.TXZ\");");
	}
	break;
      case SC_F7:
	if (sc&SCF_KEY_DESC) {
	  if (sc & SCF_SHIFT)
	    SetKeyDesc("Dollar/Insert Hidden Widget");
	  else
	    SetKeyDesc("Misc/Insert Datetime");
	} else {
	  if (sc & SCF_SHIFT)
	    LtfInsSafe(LtfCur,
		"$HS+C$$FG$$HE$");
	  else
	    FillInTime();
	}
	break;
    }
    return TRUE;
  } else {
    if (!(sc & SCF_CTRL)) {
      if (sc & SCF_ALT) {
	switch (ch) {
	  case 'h':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Win/Tile Horizontally");
	    else
	      WinTileHorz;
	    return TRUE;
	  case 'm':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Win/Maximize");
	    else {
	      WinMax;
	      WinBorder;
	    }
	    return TRUE;
	  case 'v':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Win/Tile Vertically");
	    else
	      WinTileVert;
	    return TRUE;
	  case 'V':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Sys/Frame Grabber Toggle");
	    else
	      FrameGrabberToggle;
	    return TRUE;

	  case 'l':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Put Link to Cur Pos on Clipboard");
	    else {
	      ClipBoardDel;
	      LtfPrintF(sys_clipboard_ltf,"$LK-A,\"FL:%s,%d\"$",
		LtfCur->filename.name,LtfCur->cur_entry->y+1);
	    }
	    return TRUE;
	  case 'L':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Edit/Place Anchor, Put Link to Clipboard");
	    else {
	      i=RandU32;
	      ClipBoardDel;
	      LtfPrintF(sys_clipboard_ltf,"$LK,\"<TODO>\",\"FA:%s,ANC%d\"$",
		LtfCur->filename.name,i);
	      "$AN,\"<TODO>\",\"ANC%d\"$",i;
	    }
	    return TRUE;

	  case 'w':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Win/WordStat(ON);");
	    else
	      WordStat(ON);
	    return TRUE;
	  case 'W':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Win/WordStat;");
	    else
	      WordStat;
	    return TRUE;

//Ins your own ALT-key plug-ins

	  case 'f':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Cmd/Jump to Notes File");
	    else
	      Ed("HOME/Notes.TXZ");
	    return TRUE;

	  case 'b':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("Cmd/Jump to Bad Code");
	    else
	      BadCodeJump(GetTimeStamp/16);
	    return TRUE;

	  case 'p':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("JukeBox(TAD)");
	    else
	      JukeBox("::/LT/Accts/TS/TAD/MusicOrgan");
	    return TRUE;
	  case 'P':
	    if (sc&SCF_KEY_DESC)
	      SetKeyDesc("JukeBox(Examples)");
	    else
	      JukeBox("::/LT/Apps/MusicOrgan/Examples");
	    return TRUE;
	}
      }
    }
  }
  return FALSE;
}

BoolI64 MyPutS(U8 *st)
{
  nounusedwarn st;
  return FALSE;
}

KeyDevAdd(&MyPutKey,&MyPutS,0x20000000,TRUE);
