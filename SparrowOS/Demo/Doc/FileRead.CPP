public U0 DocProfile(U8 *filename,U64 flags)
{
  U64 i,*cmd_stats=CAlloc(sizeof(U64)*DOCT_NUM_TYPES),
       *flags_stats=CAlloc(sizeof(U64)*64),
       *type_flags_stats=CAlloc(sizeof(U64)*64);
//Note, word wrap is determined by
//doc->win_task when a [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1403] CDoc is recalculated
//use [C:/SparrowOS/Adam/Doc/DocRecalc.CPP.Z,108] DocRecalc().
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

  "$FG,PURPLE$-------%s-------\n",filename;
  "$FG,GREEN$The lowest byte of the 32-bit 'doc_e->type', '$FG,PURPLE$doc_e->type.u8[0]$FG,GREEN$', is cmd and accessed with the union '$FG,PURPLE$doc_e->type_u8$FG,GREEN$'.  "
  "See $LK,\"CDocEntry\",\"MN:CDocEntry\"$, "
  "$LK,\"Doc Type Defines\",\"MN:DOCT_TEXT\"$ and $LK,\"Doc Type Codes\",\"FF:::/SparrowOS/Adam/Doc/Doc.CPP.Z,ST_DOC_CMDS\"$.\n$FG$";
  for (i=0;i<DOCT_NUM_TYPES;i++)
    if (cmd_stats[i])
      "%4Z:%d\n",i,"ST_DOC_CMDS",cmd_stats[i];

  "\n$FG,GREEN$'$FG,PURPLE$doc_e->type.u8[1]$FG,GREEN$' is the screen color attr of the entry.  "
  "'$FG,PURPLE$doc_e->type.u16[1]$FG,GREEN$' is some flags for blinking and stuff.  "
  "See $LK,\"Doc Type Flag Defines\",\"MN:DOCET_BLINK\"$.\n$FG$";

  for (i=16;i<32;i++)
    if (type_flags_stats[i])
      "%4d:%d\n",i,type_flags_stats[i];

  "$FG,GREEN$\n'$FG,PURPLE$doc_e->flags1$FG,GREEN$' is 64-bit.  "
  "See $LK,\"Doc Flag Defines\",\"MN:DOCEf1_AUX_STR\"$ and $LK,\"Doc Flag Codes\",\"FF:::/SparrowOS/Adam/Doc/Doc.CPP.Z,ST_DOC_FLAGS\"$.\n$FG$";
  for (i=0;i<64;i++)
    if (flags_stats[i])
      "%4Z:%d\n",i,"ST_DOC_FLAGS",flags_stats[i];

  Free(cmd_stats);
  Free(flags_stats);
  PressAKey;
  NewLine;
}


//See [C:/SparrowOS/Adam/Training.CPP.Z,203] TipOfDay.

//Note: Not all [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1227] CDocEntry's are full-sized nodes.
//Some are [C:/SparrowOS/OSMain/Memory.CPP.Z,952] MAlloced with a smaller size to save
//mem.	They all have at least the size [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1202] CDocEntryBase.

//Note: [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1227] CDocEntry's should be allocated from the
//heap of the owning task, doc->mem_task.

//The flag arrays [C:/SparrowOS/Adam/Doc/DocNew.CPP.Z,2] doc_tag_types, [C:/SparrowOS/Adam/Doc/DocNew.CPP.Z,4] doc_form_types, etc
//are useful.

DocProfile("::/SparrowOS/Doc/ChangeLog.TXT.Z",0);
DocProfile("::/SparrowOS/Doc/ChangeLog.TXT.Z",DOCF_PLAIN_TEXT);
