I64 MyLeftCB1(CDoc *,CDocEntry *)
{
  I64 i;
  for (i=500;i<1000;i+=25) {
    Snd(Freq2Ona(i));
    Sleep(10);
  }
  Snd;
  return 0;
}

U0 ClickCallBack()
{
  CDocEntry *doc_e;
  DocLock(DocPut);
//The [C:/Adam/DolDoc/DocPutS.HC.Z,148] DocPrint() routine returns the addr of the last entry.
  doc_e=DocPrint(DocPut,"\n$MA+LC,\"Click Me\"$ ");
//The LC flag is "has left callback".

  //Flags are explained here:
  //[C:/Doc/DolDocOverview.DD.Z,1]	[C:/Doc/Widget.DD.Z,1]
  //[C:/Kernel/KernelA.HH.Z,1057] Dollar Flags		[C:/Adam/DolDoc/DocInit.HC.Z,1] ST_DOC_FLAGS
  "(Not preserved if you press <CTRL-t>.)\n\n";
  doc_e->left_cb=&MyLeftCB1;
  DocUnlock(DocPut);
}

ClickCallBack;

I64 MyLeftCB2(CDoc *,CDocEntry *)
{
  return cnts.jiffies;
}

U0 PopUpJiffiesCallBack(I64 i)
{
  CDoc *doc=DocNew;
  CDocEntry *doc_e=DocPrint(doc,
	"Click %d more times\n\n\n$BT+LC,\"Jiffies\"$\n\n\n",i);
  doc_e->left_cb=&MyLeftCB2;
  "Current Jiffies:%d\n",PopUpMenu(doc);
  DocDel(doc);
}

PopUpJiffiesCallBack(3);
PopUpJiffiesCallBack(2);
PopUpJiffiesCallBack(1);
