public U8 *ACDWord(I64 i)
{//MAlloc dict word given rand offset.
  I64	ch;
  U8	*st=&acd.word_lst[i%acd.word_lst_size],
	*src,*dst;
  while (*st!=ACD_WORD_CHAR)
    st--;
  st=StrNew(st+1);
  src=dst=st;
  while (ch=*src++)
    if (ch==',')
      break;
    else if (Bt(chars_bmp_word,ch))
      *dst++=ch;
  *dst=0;
  return st;
}

U0 GodDict()
{
  U8 *st;
  I64 ch;
  while (TRUE) {
    ch=GetChar(,FALSE);
    if (ch==CH_ESC||ch==CH_SHIFT_ESC)
      break;
    st=ACDWord(GetTSC);
    "%s ",st;
    Free(st);
  }
  "\n";
}

GodDict;
