U0 ACMisspelledFind(CDoc *doc)
{
  Bool unlock=DocLock(doc);
  CDocEntry *doc_e=doc->cur_entry;
  I64 col=doc->cur_col,col2,ch;
  do {
    if (doc_e!=doc && doc_e->de_flags&DOCEF_TAG) {
      while (col<doc_e->max_col) {
	while (col<doc_e->max_col && !Bt(char_bmp_word,doc_e->tag[col]))
	  col++;
	if (col<doc_e->max_col) {
	  col2=col;
	  while (col2<doc_e->max_col && Bt(char_bmp_word,doc_e->tag[col2]))
	    col2++;
	  ch=doc_e->tag[col2];
	  doc_e->tag[col2]=0;
	  if (StrICmp(&doc_e->tag[col],ACDWordPtAt(&doc_e->tag[col])+1)) {
	    doc_e->tag[col2]=ch;
	    doc->cur_entry=doc_e;
	    doc->cur_col=col;
	    goto fm_done;
	  }
	  doc_e->tag[col2]=ch;
	  col=col2;
	}
      }
    }
    doc_e=doc_e->next;
    col=doc_e->min_col;
  } while (doc_e!=doc->cur_entry);
fm_done:
  if (unlock)
    DocUnlock(doc);
}
