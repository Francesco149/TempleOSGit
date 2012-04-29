SysHashTable *ltf_hash;

U0 LtfHighlightInit()
{
  ltf_hash=MAllocIdentical(asm_hash);
  ltf_hash->next=NULL;
}

LtfHighlightInit;

U32 *LtfHighlight(Ltf *l,BoolI8 full_refresh,LtfEntry *cl,LtfSettingsStruct **_s,
  U8 *src,I64 len,U64 _temp_u32_attr)
{ //Be aware of [::/LT/Adam/ToHtml.APZ] ::/LT/Adam/ToHtml.APZ.
  U32 *result=MAlloc((len+1)*sizeof(U32)),*dst=result;
  U8 *ptr,ch,ch1,last_ch;
  LtfSettingsStruct *s=*_s;
  U64 temp_u32_attr,mask_temp_u32_attr=_temp_u32_attr&0xFFFFF000,
     comment_depth,brace_depth=s->brace_depth,paren_depth=s->paren_depth;
  SysHashEntry *temph;
  nobound_switch (s->state) {
    case LTFSS_NORMAL:
hl_normal:
      while (len) {
	while (len && !Bt(alpha_numeric_bitmap,*src)) {
	  temp_u32_attr=_temp_u32_attr;
	  ch1=*src++;
	  switch (ch1) {
	    case '/':
	      if (len>=2) {
		if (*src=='/') {
		  temp_u32_attr=(s->comment_attr&15)<<8|mask_temp_u32_attr;
		  *dst++=ch1+temp_u32_attr;
		  *dst++=*src++ +temp_u32_attr;
		  len-=2;
		  goto hl_cpp_comment;
		} else if (*src=='*') {
		  temp_u32_attr=(s->comment_attr&15)<<8|mask_temp_u32_attr;
		  *dst++=ch1+temp_u32_attr;
		  *dst++=*src++ +temp_u32_attr;
		  len-=2;
		  comment_depth=1;
		  goto hl_comment;
		}
	      }
	      break;
	    case '\'':
	      temp_u32_attr=(s->quote_attr&15)<<8|mask_temp_u32_attr;
	      *dst++=ch1+temp_u32_attr;
	      len--;
	      goto hl_single_quote;
	    case '\"':
	      temp_u32_attr=(s->quote_attr&15)<<8|mask_temp_u32_attr;
	      *dst++=ch1+temp_u32_attr;
	      len--;
	      goto hl_dbl_quote;
	    case '(':
	      if (paren_depth++&1)
		temp_u32_attr=(s->alt_text_attr&15)<<8|mask_temp_u32_attr;
	      break;
	    case ')':
	      if (--paren_depth&1)
		temp_u32_attr=(s->alt_text_attr&15)<<8|mask_temp_u32_attr;
	      break;
	    case '{':
	      if (brace_depth++&1)
		temp_u32_attr=(s->alt_text_attr&15)<<8|mask_temp_u32_attr;
	      break;
	    case '}':
	      if (--brace_depth&1)
		temp_u32_attr=(s->alt_text_attr&15)<<8|mask_temp_u32_attr;
	      break;
	  }
	  *dst++=ch1+temp_u32_attr;
	  if (!--len) goto hl_normal_done;
	}
	ptr=src;
	while (len && Bt(alpha_numeric_bitmap,*src)) {
	  src++;
	  len--;
	}
	ch=*src;
	*src=0;
	if (temph=HashFind(ptr,ltf_hash,HTT_KEYWORD|HTT_ASM_KEYWORD|HTT_OPCODE|HTT_R8|HTT_R16|HTT_R32|HTT_R64|HTT_SEG)) {
	  if (temph->type&(HTT_KEYWORD|HTT_ASM_KEYWORD|HTT_OPCODE))
	    temp_u32_attr=(s->keyword_attr&15) <<8|mask_temp_u32_attr;
	  else
	    temp_u32_attr=(s->keyword2_attr&15)<<8|mask_temp_u32_attr;
	} else
	  temp_u32_attr=_temp_u32_attr;
	while (ch1=*ptr++)
	  *dst++=ch1+temp_u32_attr;
	*src=ch;
      }
hl_normal_done:
      if (s->state!=LTFSS_NORMAL||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_NORMAL;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_SINGLE_QUOTE:
      temp_u32_attr=(s->quote_attr&15)<<8|mask_temp_u32_attr;
hl_single_quote:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch!='\\' && ch1=='\'')
	  goto hl_normal;
	if (last_ch=='\\' && ch1=='\\')
	  last_ch=0;
	else
	  last_ch=ch1;
      }
      if (s->state!=LTFSS_SINGLE_QUOTE||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_SINGLE_QUOTE;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_DBL_QUOTE:
      temp_u32_attr=(s->quote_attr&15)<<8|mask_temp_u32_attr;
hl_dbl_quote:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch!='\\' && ch1=='\"')
	  goto hl_normal;
	if (last_ch=='\\' && ch1=='\\')
	  last_ch=0;
	else
	  last_ch=ch1;
      }
      if (s->state!=LTFSS_DBL_QUOTE||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_DBL_QUOTE;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_COMMENT:
      temp_u32_attr=(s->comment_attr&15)<<8|mask_temp_u32_attr;
      comment_depth=s->comment_depth;
hl_comment:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch=='*' && ch1=='/') {
	  if (!--comment_depth)
	    goto hl_normal;
	} else if (last_ch=='/' && ch1=='*')
	  comment_depth++;
	last_ch=ch1;
      }
      if (s->state!=LTFSS_COMMENT||s->comment_depth!=comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_COMMENT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=comment_depth;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_CPP_COMMENT:
      temp_u32_attr=(s->comment_attr&15)<<8|mask_temp_u32_attr;
hl_cpp_comment:
      while (len--)
	*dst++=*src++ +temp_u32_attr;
      if (s->state!=LTFSS_CPP_COMMENT||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_CPP_COMMENT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_NORMAL+LTFSSF_BPT:
      temp_u32_attr=_temp_u32_attr & 0xFFFFF000 | RED<<8;
hl_bpt_normal:
      while (len) {
	while (len && !Bt(alpha_numeric_bitmap,*src)) {
	  ch1=*src++;
	  switch (ch1) {
	    case '/':
	      if (len>=2) {
		if (*src=='/') {
		  *dst++=ch1+temp_u32_attr;
		  *dst++=*src++ +temp_u32_attr;
		  len-=2;
		  goto hl_bpt_cpp_comment;
		} else if (*src=='*') {
		  *dst++=ch1+temp_u32_attr;
		  *dst++=*src++ +temp_u32_attr;
		  len-=2;
		  comment_depth=1;
		  goto hl_bpt_comment;
		}
	      }
	      break;
	    case '\'':
	      *dst++=ch1+temp_u32_attr;
	      len--;
	      goto hl_bpt_single_quote;
	    case '\"':
	      *dst++=ch1+temp_u32_attr;
	      len--;
	      goto hl_bpt_dbl_quote;
	    case '(':
	      paren_depth++;
	      break;
	    case ')':
	      paren_depth--;
	      break;
	    case '{':
	      brace_depth++;
	      break;
	    case '}':
	      brace_depth--;
	      break;
	  }
	  *dst++=ch1+temp_u32_attr;
	  if (!--len) goto hl_bpt_normal_done;
	}
	ptr=src;
	while (len && Bt(alpha_numeric_bitmap,*src)) {
	  src++;
	  len--;
	}
	ch=*src;
	*src=0;
	while (ch1=*ptr++)
	  *dst++=ch1+temp_u32_attr;
	*src=ch;
      }
hl_bpt_normal_done:
      if (s->state!=LTFSS_NORMAL+LTFSSF_BPT||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_NORMAL+LTFSSF_BPT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_SINGLE_QUOTE+LTFSSF_BPT:
      temp_u32_attr=_temp_u32_attr & 0xFFFFF000 | RED<<8;
hl_bpt_single_quote:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch!='\\' && ch1=='\'')
	  goto hl_bpt_normal;
	if (last_ch=='\\' && ch1=='\\')
	  last_ch=0;
	else
	  last_ch=ch1;
      }
      if (s->state!=LTFSS_SINGLE_QUOTE+LTFSSF_BPT||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_SINGLE_QUOTE+LTFSSF_BPT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_DBL_QUOTE+LTFSSF_BPT:
      temp_u32_attr=_temp_u32_attr & 0xFFFFF000 | RED<<8;
hl_bpt_dbl_quote:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch!='\\' && ch1=='\"')
	  goto hl_bpt_normal;
	if (last_ch=='\\' && ch1=='\\')
	  last_ch=0;
	else
	  last_ch=ch1;
      }
      if (s->state!=LTFSS_DBL_QUOTE+LTFSSF_BPT||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_DBL_QUOTE+LTFSSF_BPT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_COMMENT+LTFSSF_BPT:
      temp_u32_attr=_temp_u32_attr & 0xFFFFF000 | RED<<8;
      comment_depth=s->comment_depth;
hl_bpt_comment:
      last_ch=0;
      while (len--) {
	ch1=*src++;
	*dst++=ch1+temp_u32_attr;
	if (last_ch=='*' && ch1=='/') {
	  if (!--comment_depth)
	    goto hl_bpt_normal;
	} else if (last_ch=='/' && ch1=='*')
	  comment_depth++;
	last_ch=ch1;
      }
      if (s->state!=LTFSS_COMMENT+LTFSSF_BPT||s->comment_depth!=comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_COMMENT+LTFSSF_BPT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=comment_depth;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
    case LTFSS_CPP_COMMENT+LTFSSF_BPT:
      temp_u32_attr=_temp_u32_attr & 0xFFFFF000 | RED<<8;
hl_bpt_cpp_comment:
      while (len--)
	*dst++=*src++ +temp_u32_attr;
      if (s->state!=LTFSS_CPP_COMMENT+LTFSSF_BPT||s->comment_depth||
	  s->paren_depth!=paren_depth||s->brace_depth!=brace_depth) {
	if (full_refresh) {
	  s=LtfSettingsIdentical(l,s);
	  s->state=LTFSS_CPP_COMMENT+LTFSSF_BPT;
	  s->paren_depth=paren_depth;
	  s->brace_depth=brace_depth;
	  s->comment_depth=0;
	  InsQue(s,l->settings_root.last);
	} else
	  s=LtfSettingsNext(cl->next);
      }
      break;
  }
  *dst=0;
  *_s=s;
  return result;
}

