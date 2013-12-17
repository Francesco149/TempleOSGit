public U0 DocProfile(U8 *filename,I64 flags)
{
  I64 i,*cmd_stats=CAlloc(sizeof(I64)*DOCT_NUM_TYPES),
       *flags_stats=CAlloc(sizeof(I64)*64),
       *type_flags_stats=CAlloc(sizeof(I64)*64);
//Note, word wrap is determined by
//doc->win_task when a [C:/Kernel/Adam1a.HPP.Z,1398] CDoc is recalculated
//use [C:/Adam/Doc/DocRecalc2b.CPP.Z,248] DocRecalc().
  CDoc *doc=DocRead(filename,flags);

//doc->root which is equal to doc is the
//header of the CQue and represents the end-of-file marker.
  CDocEntry *doc_e=doc->root.next;
  while (doc_e!=doc) {
    cmd_stats[doc_e->type_u8]++;
    for (i=0;i<64;i++)
      if (Bt(&doc_e->flags1,i))
	flags_stats[i]++;
    for (i=16;i<32;i++)
      if (Bt(&doc_e->type,i))
	type_flags_stats[i]++;
    doc_e=doc_e->next;
  }
  DocDel(doc);

  "$PURPLE$-------%s-------\n",filename;
  "$GREEN$The lowest byte of the 32-bit 'doc_e->type', '$PURPLE$doc_e->type.u8[0]$GREEN$', is cmd and accessed with the union '$PURPLE$doc_e->type_u8$GREEN$'.  "
  "See $LK,\"CDocEntry\",\"MN:CDocEntry\"$, "
  "$LK,\"Doc Type Defines\",\"MN:DOCT_TEXT\"$ and $LK,\"Doc Type Codes\",\"FF:::/Adam/Doc/Doc.CPP,ST_DOC_CMDS\"$.\n$FG$";
  for (i=0;i<DOCT_NUM_TYPES;i++)
    if (cmd_stats[i])
      "%4Z:%d\n",i,"ST_DOC_CMDS",cmd_stats[i];

  "\n$GREEN$'$PURPLE$doc_e->type.u8[1]$GREEN$' is the screen color attr of the entry.  "
  "'$PURPLE$doc_e->type.u16[1]$GREEN$' is some flags for blinking and stuff.  "
  "See $LK,\"Doc Type Flag Defines\",\"MN:DOCET_BLINK\"$.\n$FG$";

  for (i=16;i<32;i++)
    if (type_flags_stats[i])
      "%4d:%d\n",i,type_flags_stats[i];

  "$GREEN$\n'$PURPLE$doc_e->flags1$GREEN$' is 64-bit.  "
  "See $LK,\"Doc Flag Defines\",\"MN:DOCEf1_AUX_STR\"$ and $LK,\"Doc Flag Codes\",\"FF:::/Adam/Doc/Doc.CPP,ST_DOC_FLAGS\"$.\n$FG$";
  for (i=0;i<64;i++)
    if (flags_stats[i])
      "%4Z:%d\n",i,"ST_DOC_FLAGS",flags_stats[i];

  Free(cmd_stats);
  Free(flags_stats);
  PressAKey;
  NewLine;
}


//See [C:/Adam/Training.CPP.Z,203] TipOfDay.

//Note: Not all [C:/Kernel/Adam1a.HPP.Z,1223] CDocEntry's are full-sized nodes.
//Some are [C:/Kernel/Mem1b.CPP.Z,376] MAlloced with a smaller size to save
//mem.	They all have at least the size [C:/Kernel/Adam1a.HPP.Z,1205] CDocEntryBase.

//Note: [C:/Kernel/Adam1a.HPP.Z,1223] CDocEntry's should be allocated from the
//heap of the owning task, doc->mem_task.

//The flag arrays [C:/Adam/Doc/DocNew.CPP.Z,2] doc_type_flags_tag, [C:/Adam/Doc/DocNew.CPP.Z,4] doc_type_flags_form, etc
//are useful.

DocProfile("::/Doc/ChangeLog.TXT",0);
DocProfile("::/Doc/ChangeLog.TXT",DOCF_PLAIN_TEXT);
