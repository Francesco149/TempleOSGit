U8Fifo *U8FifoNew(U64 size,TaskStruct *mem_task=NULL)
{
  U8Fifo *f;
  if (!mem_task) mem_task=Fs;
  f=MAlloc(sizeof(U8Fifo),mem_task);
  f->buf=MAlloc(size,mem_task);
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

U0 U8FifoDel(U8Fifo *f)
{
  Free(f->buf);
  Free(f);
}

BoolI64 U8FifoIns(U8Fifo *f,U8 ch)
{
  U64 new_in_ptr;
  PushFD;
  Cli;
  new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    f->buf[f->in_ptr]=ch;
    f->in_ptr=new_in_ptr;
    PopFD;
    return TRUE;
  }
}

BoolI64 U8FifoRem(U8Fifo *f,U8 *ch)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *ch=f->buf[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    PopFD;
    return TRUE;
  }
}

BoolI64 U8FifoPeek(U8Fifo *f,U8 *ch)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *ch=f->buf[f->out_ptr];
    PopFD;
    return TRUE;
  }
}

U0 U8FifoFlush(U8Fifo *f)
{
  PushFD;
  Cli;
  f->out_ptr=f->in_ptr;
  PopFD;
}

U64 U8FifoCnt(U8Fifo *f)
{
  U64 result;
  PushFD;
  Cli;
  if (f->out_ptr>f->in_ptr)
    result=f->mask+1-(f->out_ptr-f->in_ptr);
  else
    result=f->in_ptr-f->out_ptr;
  PopFD;
  return result;
}

U64Fifo *U64FifoNew(U64 size,TaskStruct *mem_task=NULL)
{
  U64Fifo *f;
  if (!mem_task) mem_task=Fs;
  f=MAlloc(sizeof(U64Fifo),mem_task);
  f->buf=MAlloc(size*sizeof(U64),mem_task);
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

U0 U64FifoDel(U64Fifo *f)
{
  Free(f->buf);
  Free(f);
}

BoolI64 U64FifoIns(U64Fifo *f,U64 dd)
{
  U64 new_in_ptr;
  PushFD;
  Cli;
  new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    f->buf[f->in_ptr]=dd;
    f->in_ptr=new_in_ptr;
    PopFD;
    return TRUE;
  }
}

BoolI64 U64FifoRem(U64Fifo *f,U64 *dd)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *dd=f->buf[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    PopFD;
    return TRUE;
  }
}

BoolI64 U64FifoPeek(U64Fifo *f,U64 *dd)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *dd=f->buf[f->out_ptr];
    PopFD;
    return TRUE;
  }
}

U0 U64FifoFlush(U64Fifo *f)
{
  PushFD;
  Cli;
  f->out_ptr=f->in_ptr;
  PopFD;
}

U64 U64FifoCnt(U64Fifo *f)
{
  U64 result;
  PushFD;
  Cli;
  if (f->out_ptr>f->in_ptr)
    result=f->mask+1-(f->out_ptr-f->in_ptr);
  else
    result=f->in_ptr-f->out_ptr;
  PopFD;
  return result;
}
