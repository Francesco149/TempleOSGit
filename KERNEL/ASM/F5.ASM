        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_STRINGS_CPP
FIL_STRINGS_CPP:

        DB '// Function codes for string EDIT routine:',13,10
        DB '#define ed_discard_parity     1',13,10,'#defin'
        DB 'e ed_remove_spaces      2',13,10,'#define ed_r'
        DB 'emove_ctrl_chars  4',13,10,'#define ed_remove_'
        DB 'leading     8',13,10,'#define ed_single_space '
        DB '      16',13,10,'#define ed_to_upper          '
        DB ' 32',13,10,'#define ed_remove_trailing    64',13
        DB 10,'#define ed_remove_invalid_filename_chars 128'
        DB 13,10,'//Flags for match_list_entry',13,10,'#'
        DB 'define mle_ignore_case       1',13,10,13,10,'c'
        DB 'har *point_at_list_entry(int entry_number, char '
        DB '*lst)',13,10,'{',13,10,'  while (*lst && ent'
        DB 'ry_number) {',13,10,'    while (*lst)  /* Adva'
        DB 'nce to end of current entry. */',13,10,'      '
        DB 'lst++;',13,10,'    lst++;    /* Skip trailing '
        DB 'zero */',13,10,'    entry_number--;',13,10,' '
        DB '   if (*lst==',39,'@',39,') { /* Check for ',39
        DB '@',39,' alias list entry*/',13,10,'      entr'
        DB 'y_number++;',13,10,'      lst++;',13,10,'   '
        DB ' }',13,10,'  }',13,10,'  return lst;',13,10
        DB '}',13,10,13,10,'char toupper(char ch)',13,10
        DB '{',13,10,'  if (ch>=',39,'a',39,' && ch<=',39
        DB 'z',39,') ',13,10,'    return ch-',39,'a',39
        DB '+',39,'A',39,';',13,10,'  else',13,10,'   '
        DB ' return ch;',13,10,'}',13,10,13,10,'int ma'
        DB 'tch_list_entry(char *src, char *lst,int flags,in'
        DB 't *ambiguity_overkill_ptr)',13,10,'/* RETURNS:'
        DB ' -2 if Ambiguous, -1 if not found, else entry_nu'
        DB 'mber */',13,10,'{',13,10,'  int l,entry_num='
        DB '0,result=-1,unutilized_src;',13,10,'  char *s;'
        DB 13,10,'  bool exact_match=false;',13,10,13,10
        DB '  l=strlen(src);',13,10,'  if (ambiguity_overk'
        DB 'ill_ptr)',13,10,'    *ambiguity_overkill_ptr=l'
        DB ';',13,10,'  while (*lst) {',13,10,'    if (*'
        DB 'lst==',39,'@',39,') { /* Check for ',39,'@',39
        DB ' alias list entry*/',13,10,'      entry_num--;'
        DB 13,10,'      lst++;',13,10,'    }',13,10,13
        DB 10,'    s=src;',13,10,'    if (flags & mle_ign'
        DB 'ore_case) ',13,10,'      while (*s && (toupper'
        DB '(*s) == toupper(*lst))) {',13,10,'        s++;'
        DB 13,10,'        lst++;',13,10,'      }',13,10
        DB '    else',13,10,'      while (*s && (*s == *ls'
        DB 't)) {',13,10,'        s++;',13,10,'        l'
        DB 'st++;',13,10,'      }',13,10,'    unutilized'
        DB '_src=l-(s-src);',13,10,'    if (!(*s)) {    /*'
        DB ' Did we reach end of source? */',13,10,'      '
        DB 'if (!(*lst)) {  /* Did we reach end of list entr'
        DB 'y? */',13,10,'        if (!ambiguity_overkill_'
        DB 'ptr)',13,10,'          return entry_num;  /* F'
        DB 'ound Exact match */',13,10,'        result=ent'
        DB 'ry_num;',13,10,'        exact_match=true;',13
        DB 10,'      } else {',13,10,'        if (result!'
        DB '=-1) {',13,10,'          if (ambiguity_overkil'
        DB 'l_ptr)',13,10,'            *ambiguity_overkill'
        DB '_ptr=0;',13,10,'          if (!exact_match)',13
        DB 10,'            result=-2; /* AMBIGUOUS RESPONSE'
        DB ' unless later exact match*/',13,10,'        } '
        DB 'else ',13,10,'          result=entry_num;',13
        DB 10,'      }',13,10,'    } else {',13,10,'   '
        DB '   if (ambiguity_overkill_ptr && *ambiguity_over'
        DB 'kill_ptr>(unutilized_src-1)) ',13,10,'        '
        DB '*ambiguity_overkill_ptr=unutilized_src-1;',13,10
        DB '    }',13,10,13,10,'    while (*lst)  /* Adv'
        DB 'ance to end of current entry. */',13,10,'     '
        DB ' lst++;',13,10,'    lst++;    /* Skip trailing'
        DB ' zero */',13,10,'    entry_num++;',13,10,'  '
        DB '}',13,10,'  if (ambiguity_overkill_ptr && resu'
        DB 'lt==-1) ',13,10,'    *ambiguity_overkill_ptr=0'
        DB ';',13,10,'  return result;',13,10,'}',13,10
        DB 13,10,'int occurrences(char *src, char ch)',13
        DB 10,'{',13,10,'  int i=0;',13,10,'  while (*s'
        DB 'rc)',13,10,'    if (*src++ == ch) i++;',13,10
        DB '  return i;',13,10,'}',13,10,13,10,'char *'
        DB 'edit_string(char *u, int mask)',13,10,'{',13,10
        DB '  char *src,*dst;',13,10,'  char ch;',13,10,13
        DB 10,'  src=u;',13,10,'  if (mask & ed_remove_le'
        DB 'ading)',13,10,'    while ((*src==32) || (*src='
        DB '=9))',13,10,'      src++;',13,10,13,10,'  '
        DB 'dst=u;',13,10,'  while (ch=*src++) {',13,10,' '
        DB '   if ((ch==32) || (ch==9)) {',13,10,'      if'
        DB ' (!(mask & ed_remove_spaces)) {',13,10,'      '
        DB '  if (mask & ed_single_space) {',13,10,'      '
        DB '    (*dst++) = 32;',13,10,'          while ((c'
        DB 'h= *src++) && ((ch==32) || (ch==9)) );',13,10,' '
        DB '           src--;',13,10,'        } else',13,10
        DB '          (*dst++) = ch;',13,10,'      }',13,10
        DB '    } else {',13,10,'      if ((!(mask & ed_re'
        DB 'move_ctrl_chars)) || (ch>=32))',13,10,'       '
        DB ' (*dst++)=ch;',13,10,'    }',13,10,'  }',13
        DB 10,'  *dst=0;',13,10,13,10,'  if (mask & ed_'
        DB 'remove_invalid_filename_chars) {',13,10,'    s'
        DB 'rc=u;',13,10,'    dst=u;',13,10,'    while ('
        DB 'ch=*src++) {',13,10,'      if ((ch>=32 && ch<='
        DB '126) && !occurrences("/\\:\"*?<>|",ch)) ',13,10
        DB '        (*dst++) = ch;',13,10,'    }',13,10,' '
        DB '   *dst=0;',13,10,'  }',13,10,13,10,'  if '
        DB '(mask & ed_remove_trailing)',13,10,'    while '
        DB '( (dst != u) && (!(*dst) || (*dst==32) || (*dst='
        DB '=9)))',13,10,'      (*dst--)=0;',13,10,'  if'
        DB ' (mask & ed_discard_parity)',13,10,'    for (d'
        DB 'st=u;*dst;dst++) {',13,10,'      ch=*dst;',13
        DB 10,'      *dst=ch & 127;',13,10,'    }',13,10
        DB '  if (mask & ed_to_upper)',13,10,'    for (dst'
        DB '=u;*dst;dst++) {',13,10,'      ch=*dst;',13,10
        DB '      if ((ch>=0x61) && (ch<=0x7A))',13,10,'  '
        DB '      *dst = ch-0x20;',13,10,'    }',13,10,' '
        DB ' return u;',13,10,'}',13,10,13,10,13,10,'c'
        DB 'har *remove_first_segment(char *src, char marker'
        DB ', char *dst)',13,10,'{',13,10,'  char *temp,'
        DB '*temp1,ch;',13,10,13,10,'  temp = src;',13,10
        DB '  temp1= dst;',13,10,'  while ( (ch = *src++) '
        DB '&& (ch != marker) )',13,10,'    *dst++=ch;',13
        DB 10,13,10,'  *dst = 0;',13,10,'  if (ch)',13
        DB 10,'    strcpy(temp,src);',13,10,'  else',13,10
        DB '    *temp=0;',13,10,'  return temp1;',13,10,'}'
        DB 13,10,13,10,'char *remove_last_segment(char *'
        DB 'src, char marker, char *dst)',13,10,'{',13,10
        DB '  char *temp;',13,10,13,10,'  temp=src+strle'
        DB 'n(src);',13,10,'  while (temp>src && *temp!=ma'
        DB 'rker)',13,10,'    temp--;',13,10,'  if (*tem'
        DB 'p==marker) {',13,10,'    strcpy(dst,temp+1);',13
        DB 10,'    *temp=0;',13,10,'  } else {',13,10,' '
        DB '   strcpy(dst,src);',13,10,'    *src=0;',13,10
        DB '  }',13,10,'  return dst;',13,10,'}',13,10
        DB 13,10,'bool wild_card_match(char *wild_st,char '
        DB '*src_st)',13,10,'{',13,10,'  BYTE ch1,ch2;',13
        DB 10,'  while (true) {',13,10,'    if ( !(ch1 = '
        DB '*src_st++)) {',13,10,'      if (*wild_st)',13
        DB 10,'        return false;',13,10,'      else',13
        DB 10,'        return true;',13,10,'    } else {',13
        DB 10,'      if ( !(ch2 = *wild_st++))',13,10,'  '
        DB '      return false;',13,10,'      else {',13,10
        DB '        if (ch2 == ',39,'*',39,') {',13,10,' '
        DB '         if (!(ch2 = *wild_st++))    ',13,10,' '
        DB '           return true;',13,10,'          whil'
        DB 'e (ch2 != ch1)',13,10,'            if (!(ch1 ='
        DB ' *src_st++))    ',13,10,'              return '
        DB 'false;',13,10,'        } else',13,10,'      '
        DB '    if (ch2!=',39,'?',39,') ',13,10,'       '
        DB '     if (ch1!=ch2)    ',13,10,'              r'
        DB 'eturn false;',13,10,'      } ',13,10,'    }',13
        DB 10,'  }',13,10,'}',13,10,' '
        DB 0
        ENDS
        END
