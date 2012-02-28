//See [::/LT/Demo/Ltf/CallBack.CPZ] ::/LT/Demo/Ltf/CallBack.CPZ first.

U8 *BootTimeCB(Ltf *l,LtfEntry *ll,TaskStruct *mem_task)
{
  LTDate i;
  U8 *st=MAlloc(64,mem_task);
  nounusedwarn l,ll;
  i=BootTime;
  SPrintF(st,"%d=%h11T",i,i);
  return st;
}

U0 DoIt()
{
  LtfEntry *ll;
  Ltf *aux2=LtfNew;

  aux2->flags|=LTFF_AUX_LTF;

  LtfPutS(aux2,"$FG,RED$$CM+BY+LX,5,-3$");
  ll=LtfPutS(aux2,"$TX+TC,\" \"$");
  ll->tag_cb=&BootTimeCB;
  LtfPutS(aux2,"$FG$");

  LtfInsQueRev(aux2,LtfAux);

  NewLine;
}

U0 UndoIt()
{
  Ltf *aux2=LtfAux;
  LtfRemQue(aux2);
  LtfDel(aux2);
}

DoIt;
