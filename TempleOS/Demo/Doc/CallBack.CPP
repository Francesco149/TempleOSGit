U8 *tSCB(CDoc *,CDocEntry *,CTask *mem_task)
{
//This routine is called by the window
//mgr things would get corrupted
//if the window mgr used it's own
//heap, so we use the owning task's heap.
  U8 *st=MAlloc(64,mem_task);
  CDate spdt=tS*CDATE_FREQ;
  //Doesn't have to be fixed width!!
  SPrintF(st,"%d=%h11T",spdt,spdt);
  return st;
}

U8 *CurTimeCB(CDoc *,CDocEntry *,CTask *mem_task)
{
  U8 *st=MAlloc(64,mem_task);
  CDate spdt=GetCurTimeCDate;
  SPrintF(st,"%h0D %h0T",spdt,spdt);
  return st;
}

U0 DoIt()
{
/* The main window is kept at Fs->last_doc.
A second framework is kept at Fs->next_doc.
Normally, the next_doc is used for updating
the border, primarily.

See [C:/TempleOS/Adam/Doc/DocMain.CPP.Z,164] UseTermDoc().
See [C:/TempleOS/Adam/Doc/DocRecalc.CPP.Z,1800] DocUpdateTaskDocs().

Note: the next_doc is not to be confused
with the aux_str, a member of the [C:/TempleOS/Kernel/Adam1a.HPP.Z,1342] CDocEntry.

*/
  CDoc *aux=DocAux, //Fetches Fs->next_doc basically.
      *cur=DocCur; //Fetches Fs->last_doc basically.
  CDocEntry *doc_e;

// We don't want an update between
// creating the CDocEntry and defining the
// callback, so we turn-off preemption.

  Bool old_preempt=Preempt(OFF);

  DocBottom(aux);  //Ins at the bottom
  DocPutS(aux,"$FG,RED$$CM+BY+LX,5,-3$");

// The [C:/TempleOS/Adam/Doc/DocPlain.CPP.Z,1111] DocPutS() routine returns the addr of the last entry.
  doc_e=DocPutS(aux,"$TX+TC,\" \"$");
// The TC flag is "has tag callback".

// Flags are explained here:
//	[C:/TempleOS/Doc/CtrlL.TXT.Z,1]
//	[C:/TempleOS/Doc/Dollar.TXT.Z,1]
//	[C:/TempleOS/Kernel/Adam1a.HPP.Z,1213] Dollar Flags
//	[C:/TempleOS/Adam/Doc/Doc.CPP.Z,1] ST_DOC_FLAGS

  doc_e->tag_cb=&tSCB;
  DocPutS(aux,"$FG$");

//**WARNING:** If you use the last_doc you
//run the risk of the user pressing
//<CTRL-T> or using the clipboard, both
//of which will crash.	So, you might want
//to use the next_doc.

  DocPutS(cur,"$FG,LTRED$");
  doc_e=DocPutS(cur,"$TX+TC,\" \"$");
  doc_e->tag_cb=&CurTimeCB;
  DocPutS(cur,"$FG$");

  Preempt(old_preempt);

//Send carriage return, new line, so
//that the timer string is not part
//of the next cmd on the cmd line.
  NewLine;
}



U0 UndoIt()
{
//Clear-out entries without a +H hold flag.
  DocBottom(Fs->next_doc);
  DocPutS(Fs->next_doc,"$CL$");
}


DoIt;

// See [C:/TempleOS/Demo/Doc/CallBackAux2.CPP.Z,1] for another way.
