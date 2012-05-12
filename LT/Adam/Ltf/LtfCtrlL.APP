#define LK_FILE 	0
#define LK_FILE_ANCHOR	1
#define LK_FILE_FIND	2
#define LK_FILE_LINE	3
#define LK_MAN_PAGE	4
#define LK_PLAIN	5
#define LK_PLAIN_FIND	6
#define LK_PLAIN_LINE	7
#define LK_BIBLE_FIND	8
#define LK_DEF		9
#define LK_HELP_INDEX	10
#define LK_ADDR	11
#define LK_PLACE_ANCHOR	12

#define DFT_ADDR_LINK_BIN_SIZE	64

I64 PopUpLinkType(BoolI8 include_anchor)
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$MU,\"To file\",LK_FILE$\n"
	"$MU,\"To anchor in file\",LK_FILE_ANCHOR$\n"
	"$MU,\"To str in file\",LK_FILE_FIND$\n"
	"$MU,\"To line in file\",LK_FILE_LINE$\n"
	"$MU,\"To man page\",LK_MAN_PAGE$\n"
	"$MU,\"To plain-text file\",LK_PLAIN$\n"
	"$MU,\"To str in plain-text file\",LK_PLAIN_FIND$\n"
	"$MU,\"To line in plain-text file\",LK_PLAIN_LINE$\n"
	"$MU,\"To Bible chapter line and verse\",LK_BIBLE_FIND$\n"
	"$MU,\"To Dictionary Definition\",LK_DEF$\n"
	"$MU,\"To Help Index\",LK_HELP_INDEX$\n"
	"$MU,\"To Addr\",LK_ADDR$\n");
  if (include_anchor)
    LtfPutS(l,"$MU,\"Place Anchor\",LK_PLACE_ANCHOR$\n");
  LtfPutS(l,"\n$MU,\"CANCEL\",LTFM_CANCEL$");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

class EdFileLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136]	fmtstr "$DA-P,135,\"File          :%s\"$\n";
  U8	aux[136];
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdFileAnchorLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136]	fmtstr "$DA-P,135,\"File          :%s\"$\n";
  U8	aux[136]	fmtstr "$DA-P,135,\"Anchor Label  :%s\"$\n";
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdFileFindLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136]	fmtstr "$DA-P,135,\"File          :%s\"$\n";
  U8	aux[136]	fmtstr "$DA-P,135,\"Str        :%s\"$\n";
  I64	num		fmtstr "$DA-T,135,\"Occurrence Num:%04d\"$\n";
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdFileLineLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136]	fmtstr "$DA-P,135,\"File          :%s\"$\n";
  U8	aux[136];
  I64	num		fmtstr "$DA-T,135,\"Line Num      :%04d\"$\n";
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdManPageLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136];
  U8	aux[136]	fmtstr "$DA-P,135,\"Label         :%s\"$\n";
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdAddrLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136];
  U8	aux[136]	fmtstr "$DA-P,135,\"Addr Exp   :%s\"$\n";
  I64	num		fmtstr "$DA-T,135,\"Bin Size      :%04d\"$\n";
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdPlaceAnchorStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136];
  U8	aux[136]	fmtstr "$DA-P,135,\"Anchor Label  :%s\"$\n";
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide	fmtstr "$CB,\"Hide\"$\n";
};

class EdBibleLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136];
  U8	aux[136]	fmtstr "$DA-P,135,\"Chapter Verse :%s\"$\n";
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdDefLinkStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136];
  U8	aux[136]	fmtstr "$DA-P,135,\"Word          :%s\"$\n";
  I64	num		fmtstr "$DA-T,135,\"Def Num       :%4d\"$\n";
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

class EdProjectStruct
{
  U8	tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8	file[136]	fmtstr "$DA-P,135,\"File          :%s\"$\n";
  U8	aux[136];
  I64	num;
  U8	html_link[136]	fmtstr "$DA-P,135,\"Html Link     :%s\"$\n";
  BoolI8	quote	fmtstr "$CB,\"Quote\"$\n";
  BoolI8	hide;
};

U8 *ctrl_L_header="<WINDOWS KEY> for help\n\n";

BoolI64 PmtLink(I64 type,U8 **_tag,U8 **_link,BoolI8 *_quote,U8 **_html_link)
{
  I64 i;
  BoolI8 result=FALSE;
  EdFileLinkStruct *el=CAlloc(sizeof(EdFileLinkStruct));
  if (type>=0) {
    el->num=1;
    switch (type) {
      case LK_FILE:
	if (LtfFormDo(el,"EdFileLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->file);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("FI:%s",el->file);
	}
	break;
      case LK_PLAIN:
	if (LtfFormDo(el,"EdFileLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->file);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("PI:%s",el->file);
	}
	break;
      case LK_FILE_ANCHOR:
	if (LtfFormDo(el,"EdFileAnchorLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->aux);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("FA:%s,%s",el->file,el->aux);
	}
	break;
      case LK_FILE_FIND:
	if (LtfFormDo(el,"EdFileFindLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (el->num==1) {
	    if (!*el->tag)
	      *_tag=StrNew(el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("FF:%s,%s",el->file,el->aux);
	  } else {
	    if (!*el->tag)
	      *_tag=StrNew(el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("FF:%s,%s:%d",el->file,el->aux,el->num);
	  }
	}
	break;
      case LK_PLAIN_FIND:
	if (LtfFormDo(el,"EdFileFindLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (el->num==1) {
	    if (!*el->tag)
	      *_tag=StrNew(el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("PF:%s,%s",el->file,el->aux);
	  } else {
	    if (!*el->tag)
	      *_tag=StrNew(el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("PF:%s,%s:%d",el->file,el->aux,el->num);
	  }
	}
	break;
      case LK_FILE_LINE:
	if (LtfFormDo(el,"EdFileLineLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->file);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("FL:%s,%d",el->file,el->num);
	}
	break;
      case LK_PLAIN_LINE:
	if (LtfFormDo(el,"EdFileLineLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->file);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("PL:%s,%d",el->file,el->num);
	}
	break;
      case LK_MAN_PAGE:
	if (LtfFormDo(el,"EdManPageLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->aux);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("MN:%s",el->aux);
	}
	break;
      case LK_PLACE_ANCHOR:
	if (LtfFormDo(el,"EdPlaceAnchorStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag) {
	    if (el->hide)
	      *_tag=StrNew("");
	    else
	      *_tag=StrNew(el->aux);
	  } else
	    *_tag=StrNew(el->tag);
	  *_link=StrNew(el->aux);
	}
	break;
      case LK_BIBLE_FIND: //TODO
	i=PopUpPickDefineSub("ST_BIBLE_BOOKS");
	if (i>=0) {
	  if (LtfFormDo(el,"EdBibleLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	    result=TRUE;
	    if (!*el->tag)
	      *_tag=MSPrintF("%Z,%s",i,"ST_BIBLE_BOOKS",el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("BF:%Z,%s",i,"ST_BIBLE_BOOKS",el->aux);
	  }
	}
	break;
      case LK_DEF:
	el->num=-1;
	if (LtfFormDo(el,"EdDefLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->aux);
	  else
	    *_tag=StrNew(el->tag);
	  if (el->num<0)
	    *_link=MSPrintF("DF:%s",el->aux);
	  else
	    *_link=MSPrintF("DF:%s,%d",el->aux,el->num);

	}
	break;
      case LK_HELP_INDEX:
	if (LtfFormDo(el,"EdManPageLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (!*el->tag)
	    *_tag=StrNew(el->aux);
	  else
	    *_tag=StrNew(el->tag);
	  *_link=MSPrintF("HI:%s",el->aux);
	}
	break;
      case LK_ADDR:
	el->num=DFT_ADDR_LINK_BIN_SIZE;
	if (LtfFormDo(el,"EdAddrLinkStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	  result=TRUE;
	  if (el->num==DFT_ADDR_LINK_BIN_SIZE) {
	    if (!*el->tag)
	      *_tag=StrNew(el->aux);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("AD:%s",el->aux);
	  } else {
	    if (!*el->tag)
	      *_tag=MSPrintF("%s,%d",el->aux,el->num);
	    else
	      *_tag=StrNew(el->tag);
	    *_link=MSPrintF("AD:%s,%d",el->aux,el->num);
	  }
	}
	break;
    }
  }
  if (*el->html_link)
    *_html_link=StrNew(el->html_link);
  else
    *_html_link=NULL;
  if (el->quote)
    *_quote=TRUE;
  else
    *_quote=FALSE;
  Free(el);
  return result;
}

U0 EdInsertLink()
{
  U8 *tag=NULL,*link=NULL,*st=NULL,*html_link=NULL;
  BoolI8 quote=FALSE;
  I64 type=PopUpLinkType(TRUE);
  if (type>=0) {
    if (PmtLink(type,&tag,&link,&quote,&html_link)) {
      switch (type) {
	case LK_PLACE_ANCHOR:
	  if (html_link)
	    st=MSPrintF("$AN+HTML,\"%Q\",\"%Q\",\"%Q\"$",tag,link,html_link);
	  else
	    st=MSPrintF("$AN,\"%Q\",\"%Q\"$",tag,link);
	  break;
	default:
	  if (html_link)
	    st=MSPrintF("$LK+HTML,\"%Q\",\"%Q\",\"%Q\"$",tag,link,html_link);
	  else
	    st=MSPrintF("$LK,\"%Q\",\"%Q\"$",tag,link);
      }
    }
  }
  if (st) {
    if (quote)
      "%Q",st;
    else
      "" st;
  }
  Free(tag);
  Free(link);
  Free(html_link);
  Free(st);
}

I64 PopUpColorType()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$MU,\"Foreground\",LTFT_FOREGROUND$\n"
	"$MU,\"Background\",LTFT_BACKGROUND$\n"
	"$MU,\"Default Foreground\",LTFT_DFT_FOREGROUND$\n"
	"$MU,\"Default Background\",LTFT_DFT_BACKGROUND$\n"
	"$MU,\"Alt Foreground\",LTFT_ALT_FOREGROUND$\n"
	"$MU,\"Link Foreground\",LTFT_LINK_FOREGROUND$\n"
	"$MU,\"Macro Foreground\",LTFT_MACRO_FOREGROUND$\n"
	"$MU,\"Anchor Foreground\",LTFT_ANCHOR_FOREGROUND$\n"
	"$MU,\"Hidden Foreground\",LTFT_HIDDEN_FOREGROUND$\n"
	"$MU,\"Tree Foreground\",LTFT_TREE_FOREGROUND$\n"
	"$MU,\"User Foreground\",LTFT_USER_FOREGROUND$\n"
	"$MU,\"Binary Foreground\",LTFT_BINARY_FOREGROUND$\n"
	"$MU,\"Keyword Foreground\",LTFT_KEYWORD_FOREGROUND$\n"
	"$MU,\"Keyword2 Foreground\",LTFT_KEYWORD2_FOREGROUND$\n"
	"$MU,\"Comment Foreground\",LTFT_COMMENT_FOREGROUND$\n"
	"$MU,\"Quote Foreground\",LTFT_QUOTE_FOREGROUND$\n"
	"\n$MU,\"CANCEL\",LTFM_CANCEL$");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

I64 PopUpPageSettingType()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$MU,\"Page Length\",LTFT_PAGE_LEN$\n"
	"$MU,\"Page Header\",LTFT_HEADER$\n"
	"$MU,\"Page Footer\",LTFT_FOOTER$\n"
	"$MU,\"Left Margin\",LTFT_LEFT_MARGIN$\n"
	"$MU,\"Right Margin\",LTFT_RIGHT_MARGIN$\n"
	"$MU,\"Lock To Page\",LTFT_LOCK_TO_PAGE$\n"
	"$MU,\"Lock To Center\",LTFT_LOCK_TO_CENTER$\n");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

U0 EdInsertColor()
{
  I64 type=PopUpColorType,col=LTF_DFT;
  if (type>=0) {
    col=PopUpColor(,FALSE);
    if (col==LTF_DFT)
      "$%Z$",type,"ST_LTF_CMDS";
    else if (col>=0)
      "$%Z,%d$",type,"ST_LTF_CMDS",col;
  }
}

class EdPageSettingStruct
{
  U8	val[136]	fmtstr "$DA-P,135,\"Setting Val:%s\"$\n";
};

class EdPageSettingStruct2
{
  U8	val[136]	fmtstr "$DA-P,135,\"Setting Val:%s\"$\n";
  BoolI8 winrel		fmtstr "$CB,\"Win Relative\"$\n";
};

class EdPageSettingStruct3
{
  BoolI8 lock		fmtstr "$CB,\"Lock To Page\"$\n";
};

class EdPageSettingStruct4
{
  BoolI8 lock		fmtstr "$CB,\"Lock To Center\"$\n";
};


U0 EdInsertPageSetting()
{
  I64 type=PopUpPageSettingType;
  EdPageSettingStruct *el=CAlloc(sizeof(EdPageSettingStruct));
  EdPageSettingStruct2 *el2=CAlloc(sizeof(EdPageSettingStruct2));
  EdPageSettingStruct3 *el3=CAlloc(sizeof(EdPageSettingStruct3));
  EdPageSettingStruct4 *el4=CAlloc(sizeof(EdPageSettingStruct4));
  if (type>=0) {
    if (type==LTFT_RIGHT_MARGIN||type==LTFT_PAGE_LEN) {
      if (LtfFormDo(el2,"EdPageSettingStruct2")) {
	if (el2->winrel) {
	  if (*el2->val)
	    "$%Z+WR,%s$",type,"ST_LTF_CMDS",el2->val;
	  else
	    "$%Z+WR,0$",type,"ST_LTF_CMDS";
	} else {
	  if (*el2->val)
	    "$%Z,%s$",type,"ST_LTF_CMDS",el2->val;
	  else
	    "$%Z$",type,"ST_LTF_CMDS";
	}
      }
    } else if (type==LTFT_LOCK_TO_PAGE) {
      el3->lock=TRUE;
      if (LtfFormDo(el3,"EdPageSettingStruct3")) {
	if (el3->lock)
	  "$%Z,1$",type,"ST_LTF_CMDS",el->val;
	else
	  "$%Z,0$",type,"ST_LTF_CMDS";
      }
    } else if (type==LTFT_LOCK_TO_CENTER) {
      el4->lock=TRUE;
      if (LtfFormDo(el4,"EdPageSettingStruct4")) {
	if (el4->lock)
	  "$%Z,1$",type,"ST_LTF_CMDS",el->val;
	else
	  "$%Z,0$",type,"ST_LTF_CMDS";
      }
    } else {
      if (LtfFormDo(el,"EdPageSettingStruct")) {
	if (*el->val)
	  "$%Z,%s$",type,"ST_LTF_CMDS",el->val;
	else
	  "$%Z$",type,"ST_LTF_CMDS";
      }
    }
  }
  Free(el4);
  Free(el3);
  Free(el2);
  Free(el);
}

I64 PopUpDollarType()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$MU,\"Link\",LTFT_LINK$\n"
	"$MU,\"Text\",LTFT_TEXT$\n"
	"$MU,\"Tree Branch\",LTFT_TREE$\n"
	"$MU,\"Color\",LTFT_FOREGROUND$\n"
	"$MU,\"Page Settings\",LTFT_PAGE_LEN$\n"
	"$MU,\"Cursor Movement\",LTFT_CURSOR_MOVEMENT$\n"
	"$MU,\"Macro\",LTFT_MACRO$\n"
	"$MU,\"Button\",LTFT_BUTTON$\n"
	"$MU,\"Check Box\",LTFT_CHECK_BOX$\n"
	"$MU,\"Menu Val\",LTFT_MENU_VAL$\n"
	"$MU,\"Data\",LTFT_DATA$\n"
	"$MU,\"Hex Edit\",LTFT_HEX_ED$\n"
	"$MU,\"Hidden Widget\",LTFT_HIDE_START$\n"
	"$MU,\"Syntax Highlighting\",LTFT_HIGHLIGHT$\n"
	"$MU,\"HTML\",LTFT_HTML_CODE$\n"
	"$MU,\"Song\",LTFT_SONG$\n"
	"\n$MU,\"CANCEL\",LTFM_CANCEL$");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

class EdDollarTextStruct
{
  U8	tag[136]	  fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  BoolI8	left_x	  fmtstr "$CB,\"Left X\"$\n";
  BoolI8	center_x  fmtstr "$CB,\"Center X\"$\n";
  BoolI8	right_x   fmtstr "$CB,\"Right X\"$\n";
  BoolI8	margin_rel fmtstr "$CB,\"Margin Rel X\"$\n";
  BoolI8	blink	  fmtstr "$CB,\"Blink\"$\n";
  BoolI8	invert	  fmtstr "$CB,\"Invert\"$\n";
  BoolI8	underline fmtstr "$CB,\"Underline\"$\n";
  BoolI8	tree	  fmtstr "$CB,\"Tree\"$\n";
  BoolI8	collapsed fmtstr "$CB,\"Collapsed\"$\n";
  BoolI8	alias	  fmtstr "$CB,\"Alias\"$\n";
  U8	scroll_x [136]	  fmtstr "$DA-P,135,\"Scroll X Length Expression:%s\"$\n";
  U8	shift_x  [136]	  fmtstr "$DA-P,135,\"X Offset Expression       :%s\"$\n";
  U8	shift_y  [136]	  fmtstr "$DA-P,135,\"Y Offset Expression       :%s\"$\n";
  U8	define_str[136]	  fmtstr "$DA-P,135,\"Define Str                :%s\"$\n";
  U8	html_link[136]	  fmtstr "$DA-P,135,\"Html Link                 :%s\"$\n";
};

U0 EdInsertDollarText()
{
  U8 *st,buf[136];
  EdDollarTextStruct *dt=CAlloc(sizeof(EdDollarTextStruct));
  if (LtfFormDo(dt,"EdDollarTextStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (dt->left_x  )  StrCat(buf,"+LX");
    if (dt->center_x)  StrCat(buf,"+CX");
    if (dt->right_x )  StrCat(buf,"+RX");
    if (dt->margin_rel) StrCat(buf,"+MRX");
    if (dt->blink)     StrCat(buf,"+BK");
    if (dt->invert)    StrCat(buf,"+IV");
    if (dt->underline) StrCat(buf,"+UL");
    if (dt->tree)      StrCat(buf,"+TR");
    if (dt->collapsed) StrCat(buf,"+C");
    if (dt->alias)     StrCat(buf,"+AL");
    if (*dt->scroll_x) StrCat(buf,"+SCX");
    if (*dt->shift_x)  StrCat(buf,"+SX");
    if (*dt->shift_y)  StrCat(buf,"+SY");
    if (*dt->define_str) StrCat(buf,"+D");
    if (*dt->html_link) StrCat(buf,"+HTML");
    st=MSPrintF("%q",dt->tag);
    "$TX%s,\"%Q\"",buf,st;
    Free(st);
    if (*dt->shift_x)
      ",%s",dt->shift_x;
    if (*dt->shift_y)
      ",%s",dt->shift_y;
    if (*dt->scroll_x)
      ",%s",dt->scroll_x;
    if (*dt->define_str) {
      st=MSPrintF("%q",dt->define_str);
      ",\"%Q\"",st;
      Free(st);
    }
    if (*dt->html_link) {
      st=MSPrintF("%q",dt->html_link);
      ",\"%Q\"",st;
      Free(st);
    }
    "$";
  }
  Free(dt);
}

class EdSongStruct
{
  U8 tag[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  U8 song[512]	fmtstr "$DA-P,511,\"Song          :%s\"$\n";
};

U0 EdInsertSong()
{
  EdSongStruct *dt=CAlloc(sizeof(EdSongStruct));
  if (LtfFormDo(dt,"EdSongStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
    "$SO,\"%Q\",\"%s\"$",dt->tag,dt->song;
  Free(dt);
}

class EdHighlightStruct
{
  BoolI8 highlight_on	fmtstr "$CB,\"Highlight On\"$\n";
};

U0 EdInsertHighlight()
{
  EdHighlightStruct *dt=CAlloc(sizeof(EdHighlightStruct));
  if (LtfFormDo(dt,"EdHighlightStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
    "$HL,%d$",dt->highlight_on;
  Free(dt);
}

#define HT_CODE		0
#define HT_IMAGE	1
#define HT_VIDEO	2

I64 PopUpHtmlType()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"$MU,\"Html Code\",HT_CODE$\n"
	"$MU,\"Html Image\",HT_IMAGE$\n"
	"$MU,\"Html Video\",HT_VIDEO$\n"
	"\n$MU,\"CANCEL\",LTFM_CANCEL$");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

class EdHtmlCodeStruct
{
  U8 tag[512]	fmtstr "$DA-P,511,\"Html Code:%s\"$\n";
};

class EdHtmlURLStruct
{
  U8 tag[512]	fmtstr "$DA-P,511,\"URL:%s\"$\n";
};

U0 EdInsertHtml()
{
  EdHtmlCodeStruct *dt=CAlloc(sizeof(EdHtmlCodeStruct));
  switch (PopUpHtmlType) {
    case HT_CODE:
      if (LtfFormDo(dt,"EdHtmlCodeStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
	"$HC,\"%Q\"$",dt->tag;
      break;
    case HT_IMAGE:
      if (LtfFormDo(dt,"EdHtmlURLStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
	"$HI,\"%Q\"$",dt->tag;
      break;
    case HT_VIDEO:
      if (LtfFormDo(dt,"EdHtmlURLStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
	"$HV,\"%Q\"$",dt->tag;
      break;
  }
  Free(dt);
}

class EdMacroMenuStruct
{
  U8 tag[136]		fmtstr "$DA-P,135,\"Tag Text                  :%s\"$\n";
  U8 left_macro[512]	fmtstr "$DA-P,511,\"Left Click Macro          :%s\"$\n";
  U8 left_exp[136]	fmtstr "$DA-P,135,\"Left Click Expression     :%s\"$\n";
  BoolI8 popup		fmtstr "$CB,\"PopUp\"$\n";
  BoolI8 left_is_auto	fmtstr "$CB,\"Left is AutoStr\"$\n";
  BoolI8 left_x		fmtstr "$CB,\"Left X \"$\n";
  BoolI8 center_x	fmtstr "$CB,\"Center X \"$\n";
  BoolI8 right_x	fmtstr "$CB,\"Right X \"$\n";
  BoolI8 margin_rel	fmtstr "$CB,\"Margin Rel X\"$\n";
  BoolI8 blink		fmtstr "$CB,\"Blink\"$\n";
  BoolI8 invert		fmtstr "$CB,\"Invert\"$\n";
  BoolI8 underline	fmtstr "$CB,\"Underline\"$\n";
  BoolI8 escape		fmtstr "$CB,\"Escape\"$\n";
  U8 scroll_x[136]	fmtstr "$DA-P,135,\"Scroll X Length Expression:%s\"$\n";
  U8 shift_x[136]	fmtstr "$DA-P,135,\"X Offset Expression       :%s\"$\n";
  U8 shift_y[136]	fmtstr "$DA-P,135,\"Y Offset Expression       :%s\"$\n";
};

U0 EdInsertMacroMenu(BoolI8 is_macro)
{
  U8 *st,buf[136];
  EdMacroMenuStruct *dt=CAlloc(sizeof(EdMacroMenuStruct));
  dt->underline=TRUE;
  dt->escape=TRUE;
  if (LtfFormDo(dt,"EdMacroMenuStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (is_macro && *dt->tag) StrCat(buf,"+A ");
    if (dt->popup)	StrCat(buf,"+PU-X");
    if (dt->left_x  )	StrCat(buf,"+LX");
    if (dt->center_x)	StrCat(buf,"+CX");
    if (dt->right_x )	StrCat(buf,"+RX");
    if (dt->margin_rel) StrCat(buf,"+MRX");
    if (dt->blink)	StrCat(buf,"+BK");
    if (dt->invert)	StrCat(buf,"+IV");
    if (!dt->underline) StrCat(buf,"-UL");
    if (!dt->escape)	StrCat(buf,"-X");
    if (*dt->scroll_x)	StrCat(buf,"+SCX");
    if (*dt->shift_x)	StrCat(buf,"+SX");
    if (*dt->shift_y)	StrCat(buf,"+SY");
    if (is_macro) {
      if (*dt->left_exp)	StrCat(buf,"+LE");
      if (!*dt->left_macro) StrCat(buf,"-LM");
    } else {
      if (!*dt->left_exp)	StrCat(buf,"-LE");
      if (*dt->left_macro) StrCat(buf,"+LM");
    }
    if (dt->left_is_auto) StrCat(buf,"+LA");
    if (*dt->tag || is_macro) {
      if (is_macro) {
	if (*dt->tag) {
	  st=MSPrintF("%q",dt->tag);
	  "$MA%s,\"%Q\"",buf,st;
	  Free(st);
	} else
	  "$MA%s,",buf;
      } else {
	st=MSPrintF("%q",dt->tag);
	"$MU%s,\"%Q\"",buf,st;
	Free(st);
      }
      if (*dt->left_exp)
	",%s",dt->left_exp;
      if (*dt->left_macro) {
	st=MSPrintF("%q",dt->left_macro);
	",\"%Q\"",st;
	Free(st);
      }
      if (*dt->shift_x)
	",%s",dt->shift_x;
      if (*dt->shift_y)
	",%s",dt->shift_y;
      if (*dt->scroll_x)
	",%s",dt->scroll_x;
      "$";
    }
  }
  Free(dt);
}

class EdButtonStruct
{
  U8	tag[136]	   fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  BoolI8	popup	   fmtstr "$CB,\"PopUp\"$\n";
  BoolI8	left_x	   fmtstr "$CB,\"Left X \"$\n";
  BoolI8	center_x   fmtstr "$CB,\"Center X \"$\n";
  BoolI8	right_x	   fmtstr "$CB,\"Right X \"$\n";
  BoolI8	margin_rel fmtstr "$CB,\"Margin Rel X\"$\n";
  BoolI8	escape	   fmtstr "$CB,\"Escape\"$\n";
  U8	left_macro  [136]	fmtstr "$DA-P,135,\"Left Click Macro:%s\"$\n";
  BoolI8	left_is_auto	fmtstr "$CB,\"Left is AutoStr\"$\n";
  U8	left_exp    [136]	fmtstr "$DA-P,135,\"Left Click Expression:%s\"$\n";
  BoolI8	quote		fmtstr "$CB,\"Quote\"$\n";
};

U0 EdInsertButton()
{
  U8 *st,buf[136];
  EdButtonStruct *dt=CAlloc(sizeof(EdButtonStruct));
  dt->escape=TRUE;
  if (LtfFormDo(dt,"EdButtonStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (dt->popup)    StrCat(buf,"+PU-X");
    if (dt->left_x  ) StrCat(buf,"+LX");
    if (dt->center_x) StrCat(buf,"+CX");
    if (dt->right_x ) StrCat(buf,"+RX");
    if (dt->margin_rel) StrCat(buf,"+MRX");
    if (!dt->escape)	StrCat(buf,"-X");
    if (!*dt->left_exp)	StrCat(buf,"-LE");
    if (*dt->left_macro) StrCat(buf,"+LM");
    if (dt->left_is_auto) StrCat(buf,"+LA");
    if (*dt->tag) {
      if (dt->quote) {
	st=MSPrintF("%q",dt->tag);
	"$$BT%s,\\\"%Q\\\"",buf,st;
	Free(st);
	if (*dt->left_exp)
	  ",%s",dt->left_exp;
	if (*dt->left_macro) {
	  st=MSPrintF("%q",dt->left_macro);
	  ",\\\"%Q\\\"",st;
	  Free(st);
	}
	"$$";
      } else {
	st=MSPrintF("%q",dt->tag);
	"$BT%s,\"%Q\"",buf,st;
	Free(st);
	if (*dt->left_exp)
	  ",%s",dt->left_exp;
	if (*dt->left_macro) {
	  st=MSPrintF("%q",dt->left_macro);
	  ",\"%Q\"",st;
	  Free(st);
	}
	"$";
      }
    }
  }
  Free(dt);
}


class EdCursorMovementStruct
{
  U8 left_exp[136]	 fmtstr "$DA-P,135,\"X Expression:%s\"$\n";
  U8 right_exp[136]	 fmtstr "$DA-P,135,\"Y Expression:%s\"$\n";
  BoolI8	left_x	 fmtstr "$CB,\"Left   X \"$\n";
  BoolI8	center_x fmtstr "$CB,\"Center X \"$\n";
  BoolI8	right_x  fmtstr "$CB,\"Right  X \"$\n";
  BoolI8	margin_rel fmtstr "$CB,\"Margin Rel X\"$\n";
  BoolI8	top_y	 fmtstr "$CB,\"Top    Y \"$\n";
  BoolI8	center_y fmtstr "$CB,\"Center Y \"$\n";
  BoolI8	bottom_y fmtstr "$CB,\"Bottom Y \"$\n";
  BoolI8	page_rel fmtstr "$CB,\"Page Rel Y\"$\n";
  BoolI8	quote	 fmtstr "$CB,\"Quote\"$\n";
};

U0 EdInsertCursorMovement()
{
  U8 buf[136];
  EdCursorMovementStruct *dt=CAlloc(sizeof(EdCursorMovementStruct));
  if (LtfFormDo(dt,"EdCursorMovementStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (dt->left_x  ) StrCat(buf,"+LX");
    if (dt->center_x) StrCat(buf,"+CX");
    if (dt->right_x ) StrCat(buf,"+RX");
    if (dt->margin_rel) StrCat(buf,"+MRX");
    if (dt->top_y   ) StrCat(buf,"+TY");
    if (dt->center_y) StrCat(buf,"+CY");
    if (dt->bottom_y) StrCat(buf,"+BY");
    if (dt->page_rel) StrCat(buf,"+PRY");
    if (!*dt->left_exp)  StrCat(buf,"-LE");
    if (!*dt->right_exp) StrCat(buf,"-RE");
    if (dt->quote)
      "$";
    "$CM%s",buf;
    if (*dt->left_exp)
      ",%s",dt->left_exp;
    if (*dt->right_exp)
      ",%s",dt->right_exp;
    "$";
    if (dt->quote)
      "$";
  }
  Free(dt);
}

class EdDataStruct
{
  U8	fmt_str [136]	fmtstr "$DA-P,135,\"Format Str:%s\"$\n";
  U64 len		fmtstr "$DA,256,\"Length:%d\"$\n";
  BoolI8	term	fmtstr "$CB,\"Terminator\"$\n";
  BoolI8	str	fmtstr "$CB,\"Str\"$\n";
  BoolI8	refresh	fmtstr "$CB,\"Refresh Data\"$\n";
  BoolI8	update	fmtstr "$CB,\"Update Data\"$\n";
  BoolI8	u8	fmtstr "$CB,\"U8\"$\n";
  BoolI8	u16	fmtstr "$CB,\"U16\"$\n";
  BoolI8	u32	fmtstr "$CB,\"U32\"$\n";
  BoolI8	u64	fmtstr "$CB,\"U64\"$\n";
};

U0 EdInsertData()
{
  U8 *st,buf[136];
  EdDataStruct *dt=CAlloc(sizeof(EdDataStruct));
  dt->term=TRUE;
  dt->len=256;
  dt->u64=TRUE;
  if (LtfFormDo(dt,"EdDataStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (dt->str)   StrCat(buf,"-P");
    if (!dt->term)    StrCat(buf,"-T");
    if (dt->refresh)  StrCat(buf,"+RD");
    if (dt->update)   StrCat(buf,"+UD");

    if (dt->u8)        StrCat(buf,"-U64+U8");
    else if (dt->u16)  StrCat(buf,"-U64+U16");
    else if (dt->u32)  StrCat(buf,"-U64+U32");

    st=MSPrintF("%q",dt->fmt_str);
    "$$DA%s,%d,\\\"%Q\\\"$$",buf,dt->len,st;
    Free(st);
  }
  Free(dt);
}

class EdCheckBoxStruct
{
  U8	 label[136]	fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  BoolI8 refresh	fmtstr "$CB,\"Refresh Data\"$\n";
};

U0 EdInsertCheckBox()
{
  U8 *st,buf[136];
  EdCheckBoxStruct *dt=CAlloc(sizeof(EdCheckBoxStruct));
  if (LtfFormDo(dt,"EdCheckBoxStruct",DOF_HELP_IS_FILE,CTRL_L_HELP)) {
    *buf=0;
    if (dt->refresh)  StrCat(buf,"+RD");
    st=MSPrintF("%q",dt->label);
    "$$CB%s,\\\"%Q\\\"$$",buf,st;
    Free(st);
  }
  Free(dt);
}

class EdHexEdStruct
{
  U64 cnt	fmtstr "$DA,256,\"Count:%d\"$\n";
  U64 cols	fmtstr "$DA,256,\"Columns:%d\"$\n";
  BoolI8 zero	fmtstr "$CB,\"Zero Based\"$\n";
  BoolI8 refresh fmtstr "$CB,\"Refresh Data\"$\n";
};

U0 EdInsertHexEd()
{
  U8 buf[136];
  EdHexEdStruct *dt=CAlloc(sizeof(EdHexEdStruct));
  dt->cnt=128;
  dt->cols=4;
  dt->zero=TRUE;
  if (LtfFormDo(dt,"EdHexEdStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (!dt->zero)  StrCat(buf,"-Z");
    if (dt->refresh)  StrCat(buf,"+RD");
    "$$HX%s,%d,%d$$",buf,dt->cnt,dt->cols;
  }
  Free(dt);
}


class EdBinaryStruct
{
  I64 num fmtstr "$DA,256,\"Num:%d\"$\n";
};

U0 EdInsertBinary(I64 type)
{
  EdBinaryStruct *dt=CAlloc(sizeof(EdBinaryStruct));
  dt->num=1;
  if (LtfFormDo(dt,"EdBinaryStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    if (LtfBinFind(LtfCur,dt->num))
      "$%Z,\"<%d>\",%d$",type,"ST_LTF_CMDS",dt->num,dt->num;
    else
      PopUpOk("Invalid Binary Num");
  }
  Free(dt);
}

class EdTreeStruct
{
  U8	label[136] fmtstr "$DA-P,135,\"Tag Text      :%s\"$\n";
  I64	indent	   fmtstr "$DA,135,\"Indention:%d\"$\n";
  BoolI8	collapsed  fmtstr "$CB,\"Collapsed\"$\n";
};

U0 EdInsertTree()
{
  U8 *st,*st2,buf[136];
  EdTreeStruct *dt=CAlloc(sizeof(EdTreeStruct));
  dt->collapsed=TRUE;
  dt->indent=2;
  if (LtfFormDo(dt,"EdTreeStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
    *buf=0;
    if (!dt->collapsed)  StrCat(buf,"-C");
    st=MSPrintF("%q",dt->label);
    st2=MSPrintF("$TR%s,\"%Q\"$\n$ID,%d$*\n$ID,%d$",
      buf,st,dt->indent,-dt->indent);
    LtfInsertSafe(LtfCur,st2);
    Free(st);
    Free(st2);
  }
  Free(dt);
}

U0 EdInsertHidden()
{
  LtfInsertSafe(LtfCur,"$HS$*$HE$");
}

U0 EdInsertDollarCmd()
{
  I64 type=PopUpDollarType;
  switch (type) {
    case LTFT_FOREGROUND:
      EdInsertColor;
      break;
    case LTFT_PAGE_LEN:
      EdInsertPageSetting;
      break;
    case LTFT_LINK:
      EdInsertLink;
      break;
    case LTFT_TEXT:
      EdInsertDollarText;
      break;
    case LTFT_TREE:
      EdInsertTree;
      break;
    case LTFT_MACRO:
      EdInsertMacroMenu(TRUE);
      break;
    case LTFT_MENU_VAL:
      EdInsertMacroMenu(FALSE);
      break;
    case LTFT_CURSOR_MOVEMENT:
      EdInsertCursorMovement;
      break;
    case LTFT_BUTTON:
      EdInsertButton;
      break;
    case LTFT_DATA:
      EdInsertData;
      break;
    case LTFT_CHECK_BOX:
      EdInsertCheckBox;
      break;
    case LTFT_HEX_ED:
      EdInsertHexEd;
      break;
    case LTFT_HIDE_START:
      EdInsertHidden;
      break;
    case LTFT_SONG:
      EdInsertSong;
      break;
    case LTFT_HIGHLIGHT:
      EdInsertHighlight;
      break;
    case LTFT_HTML_CODE:
      EdInsertHtml;
   }
}

#define PT_PICTURE		0
#define PT_PICTURE_MACRO	1
#define PT_PICTURE_MENU		2
#define PT_PICTURE_LINK		3
#define PT_GRSPRITES_PTR	4
#define PT_GRSPRITES_SIZE	5
#define PT_DUP_PICTURE		6

I64 PopUpPictureType()
{
  I64 i;
  Ltf *l=LtfNew;
  LtfPutS(l,"A picture is composed of elements\n"
	"such as lines and colors.  A collection\n"
	"is called $FG,GREEN$Sprite$FG$.\n\n"
	"You can create an icon by placing a picture\n"
	"with a payload of code to execute.\n"
	"This is a called a picture macro.\n"
	"Check \"Pop-Up\" for icons and \"Esc\" for\n"
	"cmd line macros.\n\n"
	"In a program, you first make a picture.\n"
	"Then, you insert a pointer to it.\n\n"
	"$MU,\"Insert just Picture (GrSprite)\",PT_PICTURE$\n"
	"$MU,\"Picture Macro (Icon)\",PT_PICTURE_MACRO$\n"
	"$MU,\"Picture Menu Item\",PT_PICTURE_MENU$\n"
	"$MU,\"Picture Link\",PT_PICTURE_LINK$\n"
	"$MU,\"Pointer to GrSprite Data\",PT_GRSPRITES_PTR$\n"
	"$MU,\"GrSprite Size (not really needed)\",PT_GRSPRITES_SIZE$\n"
	"$MU,\"Duplicate Picture\",PT_DUP_PICTURE$\n"
	"\n$MU,\"CANCEL\",LTFM_CANCEL$\n\n"
	"The potential exists for binary data\n"
	"besides $FG,GREEN$Sprites$FG$.  This is why\n"
	"the pointer to a Sprite is known as\n"
	"\"Insert Binary\".  There is one more\n"
	"option called \"Insert Binary Type\".\n"
	"The only other binary data type is source\n"
	"line info in map files.\n\n"
	"$MU,\"Insert Binary Data Type\",7$\n");
  i=PopUpMenu(l);
  LtfDel(l);
  return i;
}

class EdJustPictureStruct
{
  U8 tag[136]	fmtstr "$DA-P,135,\"Tag Text       :%s\"$\n";
};

class EdMacroPictureStruct
{
  U8 tag[136]		fmtstr "$DA-P,135,\"Tag Text       :%s\"$\n";
  U8 exp[512]		fmtstr "$DA-P,511,\"Macro          :%s\"$\n";
  BoolI8 escape		fmtstr "$CB,\"Escape\"$\n";
  BoolI8 popup		fmtstr "$CB,\"PopUp\"$\n";
  BoolI8 is_auto	fmtstr "$CB,\"AutoStr\"$\n";
};

class EdMenuPictureStruct
{
  U8 tag[136]		fmtstr "$DA-P,135,\"Tag Text       :%s\"$\n";
  U8 exp[512]		fmtstr "$DA-P,511,\"Expression     :%s\"$\n";
  BoolI8 escape		fmtstr "$CB,\"Escape\"$\n";
};

U8 *EdPictureLink(U8 **_html_link)
{
  U8 *result=NULL,*tag=NULL,*link=NULL;
  BoolI8 quote=FALSE;
  I64 type=PopUpLinkType(FALSE);
  *_html_link=NULL;
  if (type>=0) {
    if (PmtLink(type,&tag,&link,&quote,_html_link))
      result=MSPrintF("\"%Q\",\"%Q\"",tag,link);
  }
  Free(tag);
  Free(link);
  return result;
}

U8 *EdPicture()
{
  I64 type=PopUpPictureType;
  U8 *st,*st1=NULL,*st2=NULL,buf[1024],*html_link=NULL;
  EdMacroPictureStruct *dt=CAlloc(sizeof(EdMacroPictureStruct));
  *buf=0;
  switch (type) {
    case PT_PICTURE:
      SPrintF(dt->tag,"<%d>",LtfCur->cur_bin_num);
      if (LtfFormDo(dt,"EdJustPictureStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header))
	st1=MSPrintF("\"%Q\"",dt->tag);
      break;
    case PT_PICTURE_MACRO:
      dt->escape=TRUE;
      if (LtfFormDo(dt,"EdMacroPictureStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	StrCat(buf,"+LM+UL");
	if (dt->is_auto)StrCat(buf,"+LA");
	if (dt->popup)	StrCat(buf,"+PU");
	if (dt->escape)	StrCat(buf,"+X");
	st=MSPrintF("%q",dt->exp);
	st1=MSPrintF("\"%Q\",\"%Q\"",dt->tag,st);
	Free(st);
      }
      break;
    case PT_PICTURE_MENU:
      dt->escape=TRUE;
      if (LtfFormDo(dt,"EdMenuPictureStruct",DOF_HELP_IS_FILE,CTRL_L_HELP,ctrl_L_header)) {
	StrCat(buf,"+LE");
	if (dt->escape)	StrCat(buf,"+X");
	st1=MSPrintF("\"%Q\",\"%Q\"",dt->tag,dt->exp);
      }
      break;
    case PT_PICTURE_LINK:
      StrCat(buf,"+L+A");
      st1=EdPictureLink(&html_link);
      break;
    case PT_GRSPRITES_PTR:
      EdInsertBinary(LTFT_INSERT_BINARY);
      break;
    case PT_GRSPRITES_SIZE:
      EdInsertBinary(LTFT_INSERT_BINARY_SIZE);
      break;
    case PT_DUP_PICTURE:
      EdInsertBinary(LTFT_PICTURE);
      break;
  }
  if (st1) {
    if (html_link) {
      StrCat(buf,"+HTML");
      st2=MSPrintF("$PI-BI%s,%s,\"%Q\"$",buf,st1,html_link);
    } else
      st2=MSPrintF("$PI-BI%s,%s$",buf,st1);
    Free(st1);
  }
  Free(dt);
  Free(html_link);
  return st2;
}
