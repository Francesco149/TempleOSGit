public U0 DocProfile(U8 *filename,I64 flags)
{
  I64 i,*cmd_stats=CAlloc(sizeof(I64)*DOCT_TYPES_NUM),
	*flags_stats=CAlloc(sizeof(I64)*64),
	*type_flags_stats=CAlloc(sizeof(I64)*64);
//Note: word wrap is determined by
  //doc->win_task when a [C:/Kernel/KernelA.HH.Z,1367] CDoc is recalculated
  //use [C:/Adam/DolDoc/DocRecalc.HC.Z,256] DocRecalc().
  CDoc *doc=DocRead(filename,flags);

  //doc->head which is equ to doc is the
  //header of the CQue and represents the end-of-file marker.
  CDocEntry *doc_e=doc->head.next;
  while (doc_e!=doc) {
    cmd_stats[doc_e->type_u8]++;
    for (i=0;i<64;i++)
      if (Bt(&doc_e->de_flags,i))
	flags_stats[i]++;
    for (i=16;i<32;i++)
      if (Bt(&doc_e->type,i))
	type_flags_stats[i]++;
    doc_e=doc_e->next;
  }
  DocDel(doc);

  "$PURPLE$-------%s-------\n",filename;
  "$GREEN$The lowest byte of the 32-bit 'doc_e->type', "
	"'$PURPLE$doc_e->type.u8[0]$GREEN$', "
	"is cmd and accessed with the union "
	"'$PURPLE$doc_e->type_u8$GREEN$'.  "
	"See $LK,\"CDocEntry\",A=\"MN:CDocEntry\"$, "
	"$LK,\"Doc Type Defines\",A=\"MN:DOCT_TEXT\"$ and "
	"$LK,\"Doc Type Codes\","
	"A=\"FF:::/Adam/DolDoc/DocInit.HC,ST_DOC_CMDS\"$.\n"
	"$FG$";
  for (i=0;i<DOCT_TYPES_NUM;i++)
    if (cmd_stats[i])
      "%4Z:%d\n",i,"ST_DOC_CMDS",cmd_stats[i];

  "\n$GREEN$'$PURPLE$doc_e->type.u8[1]$GREEN$' is "
	"the scrn color attr of the "
	"entry.  '$PURPLE$doc_e->type.u16[1]"
	"$GREEN$' is some flags for blinking and "
	"stuff.  See $LK,\"Doc Type Flag "
	"Defines\",A=\"MN:DOCET_BLINK\"$.\n$FG$";

  for (i=16;i<32;i++)
    if (type_flags_stats[i])
      "%4d:%d\n",i,type_flags_stats[i];

  "$GREEN$\n'$PURPLE$doc_e->de_flags$GREEN$' is 64-bit.  "
	"See $LK,\"Doc Flag Defines\",A=\"MN:DOCEf_TAG\"$ and "
	"$LK,\"Doc Flag Codes\","
	"A=\"FF:::/Adam/DolDoc/DocInit.HC,ST_DOC_FLAGS\"$.\n"
	"$FG$";
  for (i=0;i<64;i++)
    if (flags_stats[i])
      "%4Z:%d\n",i,"ST_DOC_FLAGS",flags_stats[i];

  Free(cmd_stats);
  Free(flags_stats);
  PressAKey;
  '\n';
}

/*See [C:/Adam/Training.HC.Z,161] TipOfDay.

Note: Not all [C:/Kernel/KernelA.HH.Z,1191] CDocEntry's are full-sized nodes.
Some are [C:/Kernel/Mem/MAllocFree.HC.Z,387] MAlloced with a smaller size to save
mem.	They all have at least the size [C:/Kernel/KernelA.HH.Z,1169] CDocEntryBase.

Note: [C:/Kernel/KernelA.HH.Z,1191] CDocEntry's should be alloced from the
heap of the owning task, doc->mem_task.

The flag arrays [C:/Adam/DolDoc/MakeDoc.HC.Z,21] doldoc.type_flags_form, etc
are useful.
*/

DocProfile("::/Doc/Welcome.DD",0);
DocProfile("::/Doc/Welcome.DD",DOCF_PLAIN_TEXT);
