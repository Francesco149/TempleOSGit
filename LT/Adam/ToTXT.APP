#help_index "Utils;Cmd Line (Typically)"

U0 TXTPutS(Ltf *l,U8 *st,I64 *_col)
{
  U8 *ch;
  while (ch=*st++) {
    switch (ch) {
      case CH_TAB:
	do {
	  LtfPutKey(l,CH_SPACE,0);
	  *_col=*_col+1;
	} while (*_col&7);
	break;
      case CH_SHIFT_SPACE:
	LtfPutS(l," ");
	*_col=*_col+1;
	break;
      case 'pi':
	LtfPutS(l,"pi");
	*_col=*_col+1;
	break;
      case 'theta':
	LtfPutS(l,"theta");
	*_col=*_col+1;
	break;
      case 'phi':
	LtfPutS(l,"phi");
	*_col=*_col+1;
	break;
      case 'omega':
	LtfPutS(l,"omega");
	*_col=*_col+1;
	break;
      case 'inf':
	LtfPutS(l,"inf");
	*_col=*_col+1;
	break;
      default:
	LtfPutKey(l,ch,0);
	*_col=*_col+1;
    }
  }
}

public Ltf *Ltf2TXT(Ltf *l_in)
{
  LtfEntry *cl,*cl2;
  I64 y,col;
  Ltf *l_out=LtfNew;
  BoolI8 unlock_l=LtfLock(l_in),no_backward;

  LtfRecalc(l_in);

  l_out->flags|=LTFF_PLAIN_TEXT|LTFF_NO_CURSOR;

  cl=l_in->root.next;
  col=cl->x;
  y=cl->y;
  while (cl!=l_in) {
    if (!(cl->flags&(LTFLF_SKIP|LTFLF_HARD_SKIP))) {
      while (y<cl->y) {
	TXTPutS(l_out,"\n",&col);
	y++;
	col=0;
      }

      no_backward=TRUE;
      cl2=cl->next;
      while (cl2!=l_in && cl2->y==cl->y) {
	if (cl2->x<cl->x) {
	  no_backward=FALSE;
	  break;
	}
	cl2=cl2->next;
      }
      if (no_backward)
	while (col<cl->x)
	  TXTPutS(l_out," ",&col);

      switch (cl->type_u8) {
	case LTFT_TEXT:
	  TXTPutS(l_out,cl->tag,&col);
	  break;
	case LTFT_TAB:
	  TXTPutS(l_out,"\t",&col);
	  break;
	case LTFT_HTML_IMG:
	  break;
	case LTFT_HTML_VIDEO:
	  break;
	case LTFT_HTML_CODE:
	  break;
	case LTFT_PICTURE:
	  break;
	default:
	  if (Bt(ltf_tag_types,cl->type_u8))
	    TXTPutS(l_out,cl->tag,&col);
      }
    }
    cl=cl->next;
  }
  while (y<cl->y) {
    TXTPutS(l_out,"\n",&col);
    y++;
    col=0;
  }
  l_out->cur_entry=&l_out->root;
  LtfRecalc(l_out);

  if (unlock_l)
    LtfUnlock(l_in);
  return l_out;
}

public U0 ToTXT(U8 *_in_name,U8 *_out_name=NULL,I64 width=70)
{
  U8 *in_name,*out_name;
  Ltf *l_in,*l_out;

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  SetWinHorz(0,width-1); //This sets doc width for word wrap

  in_name=DftExt(_in_name,"TXZ"),
  if (_out_name)
    out_name=DftExt(_out_name,"TXT");
  else
    out_name=ChgExt(_in_name,"TXT");

  l_in=LtfRead(in_name);
  l_out=Ltf2TXT(l_in);
  StrCpy(&l_out->filename.name,out_name);

  SettingsPop;

  LtfWrite(l_out);
  LtfDel(l_in);
  LtfDel(l_out);
  Free(in_name);
  Free(out_name);
}

#help_index ""
