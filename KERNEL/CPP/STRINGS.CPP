// Function codes for string EDIT routine:
#define ed_discard_parity     1
#define ed_remove_spaces      2
#define ed_remove_ctrl_chars  4
#define ed_remove_leading     8
#define ed_single_space       16
#define ed_to_upper	      32
#define ed_remove_trailing    64
#define ed_remove_invalid_filename_chars 128
//Flags for match_list_entry
#define mle_ignore_case       1

char *point_at_list_entry(int entry_number, char *lst)
{
  while (*lst && entry_number) {
    while (*lst)  /* Advance to end of current entry. */
      lst++;
    lst++;    /* Skip trailing zero */
    entry_number--;
    if (*lst=='@') { /* Check for '@' alias list entry*/
      entry_number++;
      lst++;
    }
  }
  return lst;
}

int match_list_entry(char *src, char *lst,int flags=0,int *ambiguity_overkill_ptr=NULL)
/* RETURNS: -2 if Ambiguous, -1 if not found, else entry_number */
{
  int l,entry_num=0,result=-1,unutilized_src;
  char *s;
  bool exact_match=false;

  l=strlen(src);
  if (ambiguity_overkill_ptr)
    *ambiguity_overkill_ptr=l;
  while (*lst) {
    if (*lst=='@') { /* Check for '@' alias list entry*/
      entry_num--;
      lst++;
    }

    s=src;
    if (flags & mle_ignore_case)
      while (*s && (toupper(*s) == toupper(*lst))) {
	s++;
	lst++;
      }
    else
      while (*s && (*s == *lst)) {
	s++;
	lst++;
      }
    unutilized_src=l-(s-src);
    if (!(*s)) {    /* Did we reach end of source? */
      if (!(*lst)) {  /* Did we reach end of list entry? */
	if (!ambiguity_overkill_ptr)
	  return entry_num;  /* Found Exact match */
	result=entry_num;
	exact_match=true;
      } else {
	if (result!=-1) {
	  if (ambiguity_overkill_ptr)
	    *ambiguity_overkill_ptr=0;
	  if (!exact_match)
	    result=-2; /* AMBIGUOUS RESPONSE unless later exact match*/
	} else
	  result=entry_num;
      }
    } else {
      if (ambiguity_overkill_ptr && *ambiguity_overkill_ptr>(unutilized_src-1))
	*ambiguity_overkill_ptr=unutilized_src-1;
    }

    while (*lst)  /* Advance to end of current entry. */
      lst++;
    lst++;    /* Skip trailing zero */
    entry_num++;
  }
  if (ambiguity_overkill_ptr && result==-1)
    *ambiguity_overkill_ptr=0;
  return result;
}

int occurrences(char *src, char ch)
{
  int i=0;
  while (*src)
    if (*src++ == ch) i++;
  return i;
}

char *edit_string(char *u, int mask)
{
  char *src,*dst;
  char ch;

  src=u;
  if (mask & ed_remove_leading)
    while ((*src==32) || (*src==9))
      src++;

  dst=u;
  while (ch=*src++) {
    if ((ch==32) || (ch==9)) {
      if (!(mask & ed_remove_spaces)) {
	if (mask & ed_single_space) {
	  (*dst++) = 32;
	  while ((ch= *src++) && ((ch==32) || (ch==9)) );
	    src--;
	} else
	  (*dst++) = ch;
      }
    } else {
      if ((!(mask & ed_remove_ctrl_chars)) || (ch>=32))
	(*dst++)=ch;
    }
  }
  *dst=0;

  if (mask & ed_remove_invalid_filename_chars) {
    src=u;
    dst=u;
    while (ch=*src++) {
      if ((ch>=32 && ch<=126) && !occurrences("/\\:\"*?<>|",ch))
	(*dst++) = ch;
    }
    *dst=0;
  }

  if (mask & ed_remove_trailing)
    while ( (dst != u) && (!(*dst) || (*dst==32) || (*dst==9)))
      (*dst--)=0;
  if (mask & ed_discard_parity)
    for (dst=u;*dst;dst++) {
      ch=*dst;
      *dst=ch & 127;
    }
  if (mask & ed_to_upper)
    for (dst=u;*dst;dst++) {
      ch=*dst;
      if ((ch>=0x61) && (ch<=0x7A))
	*dst = ch-0x20;
    }
  return u;
}


char *remove_first_segment(char *src, char marker, char *dst)
{
  char *temp,*temp1,ch;

  temp = src;
  temp1= dst;
  while ( (ch = *src++) && (ch != marker) )
    *dst++=ch;

  *dst = 0;
  if (ch)
    strcpy(temp,src);
  else
    *temp=0;
  return temp1;
}

char *remove_last_segment(char *src, char marker, char *dst)
{
  char *temp;

  temp=src+strlen(src);
  while (temp>src && *temp!=marker)
    temp--;
  if (*temp==marker) {
    strcpy(dst,temp+1);
    *temp=0;
  } else {
    strcpy(dst,src);
    *src=0;
  }
  return dst;
}

bool wild_card_match(char *wild_st,char *src_st)
{
  BYTE ch1,ch2;
  while (true) {
    if ( !(ch1 = *src_st++)) {
      if (*wild_st)
	return false;
      else
	return true;
    } else {
      if ( !(ch2 = *wild_st++))
	return false;
      else {
	if (ch2 == '*') {
	  if (!(ch2 = *wild_st++))
	    return true;
	  while (ch2 != ch1)
	    if (!(ch1 = *src_st++))
	      return false;
	} else
	  if (ch2!='?')
	    if (ch1!=ch2)
	      return false;
      }
    }
  }
}

bool wild_filename_list(char *wild_st,char *src_st)
{
  char *buf1=new_string(wild_st),
       *buf2=malloc(strlen(wild_st)+1);
  while (true) {
    remove_first_segment(buf1,';',buf2);
    if (*buf2) {
      if (wild_card_match(buf2,src_st))  {
	free(buf1);
	free(buf2);
	return true;
      }
    } else {
      free(buf1);
      free(buf2);
      return false;
    }
  }
}


