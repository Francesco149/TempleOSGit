U8QueVect *U8QueVectNew(I64 min_index=0)
{
  U8QueVect *result=MAlloc(sizeof(U8QueVect));
  result->next=result->last=result;
  result->total_cnt=result->node_cnt=0;
  result->min_index=min_index;
  return result;
}

U0 U8QueVectPut(U8QueVect *v,I64 index,U8 ch)
{
  U8QueVect *tempv;
  index-=v->min_index;
  if (index<0) return;
  if (index<v->total_cnt) {
    tempv=v;
    do {
      index-=tempv->node_cnt;
      if (index<0) {
	tempv->body[index+tempv->node_cnt]=ch;
	return;
      }
      tempv=tempv->next;
    } while (tempv!=v);
  } else
    index-=v->total_cnt;

  while (TRUE) {
    tempv=v->last;
    if (tempv->node_cnt>=U8_QUE_VECT_CNT) {
      tempv=MAlloc(sizeof(U8QueVect));
      tempv->node_cnt=0;
      InsQue(tempv,v->last);
    }
    if (index--) {
      tempv->body[tempv->node_cnt++]=0;
      v->total_cnt++;
    } else {
      tempv->body[tempv->node_cnt++]=ch;
      v->total_cnt++;
      break;
    }
  }
}

U0 U8QueVectDel(U8QueVect *v)
{
  U8QueVect *tempv,*tempv1;
  if (!v) return;
  tempv=v;
  do {
    tempv1=tempv->next;
    Free(tempv);
    tempv=tempv1;
  } while (tempv!=v);
}

U64 U8QueVectGet(U8QueVect *v,I64 index)
{
  U8QueVect *tempv;
  index-=v->min_index;
  if (!(0<=index<v->total_cnt)) return 0;
  tempv=v;
  do {
    index-=tempv->node_cnt;
    if (index<0)
      return tempv->body[index+tempv->node_cnt];
    tempv=tempv->next;
  } while (tempv!=v);
  return 0;
}
