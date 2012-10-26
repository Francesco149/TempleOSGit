CFifoU8 *FifoU8New(U64 size,CTask *mem_task=NULL)
{
  CFifoU8 *f;
  if (!mem_task) mem_task=Fs;
  f=MAlloc(sizeof(CFifoU8),mem_task);
  f->buf=MAlloc(size,mem_task);
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

U0 FifoU8Del(CFifoU8 *f)
{
  Free(f->buf);
  Free(f);
}

BoolI64 FifoU8Ins(CFifoU8 *f,U8 b)
{
  U64 new_in_ptr;
  PushFD;
  Cli;
  new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    f->buf[f->in_ptr]=b;
    f->in_ptr=new_in_ptr;
    PopFD;
    return TRUE;
  }
}

BoolI64 FifoU8Rem(CFifoU8 *f,U8 *_b)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *_b=f->buf[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    PopFD;
    return TRUE;
  }
}

BoolI64 FifoU8Peek(CFifoU8 *f,U8 *_b)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *_b=f->buf[f->out_ptr];
    PopFD;
    return TRUE;
  }
}

U0 FifoU8Flush(CFifoU8 *f)
{
  PushFD;
  Cli;
  f->out_ptr=f->in_ptr;
  PopFD;
}

U64 FifoU8Cnt(CFifoU8 *f)
{
  PushFD;
  Cli;
  if (f->out_ptr>f->in_ptr)
    result=f->mask+1-(f->out_ptr-f->in_ptr);
  else
    result=f->in_ptr-f->out_ptr;
  PopFD;
}

CFifoU64 *FifoU64New(U64 size,CTask *mem_task=NULL)
{
  CFifoU64 *f;
  if (!mem_task) mem_task=Fs;
  f=MAlloc(sizeof(CFifoU64),mem_task);
  f->buf=MAlloc(size*sizeof(U64),mem_task);
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

U0 FifoU64Del(CFifoU64 *f)
{
  Free(f->buf);
  Free(f);
}

BoolI64 FifoU64Ins(CFifoU64 *f,U64 q)
{
  U64 new_in_ptr;
  PushFD;
  Cli;
  new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    f->buf[f->in_ptr]=q;
    f->in_ptr=new_in_ptr;
    PopFD;
    return TRUE;
  }
}

BoolI64 FifoU64Rem(CFifoU64 *f,U64 *_q)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *_q=f->buf[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    PopFD;
    return TRUE;
  }
}

BoolI64 FifoU64Peek(CFifoU64 *f,U64 *_q)
{
  PushFD;
  Cli;
  if (f->in_ptr==f->out_ptr) {
    PopFD;
    return FALSE;
  } else {
    *_q=f->buf[f->out_ptr];
    PopFD;
    return TRUE;
  }
}

U0 FifoU64Flush(CFifoU64 *f)
{
  PushFD;
  Cli;
  f->out_ptr=f->in_ptr;
  PopFD;
}

U64 FifoU64Cnt(CFifoU64 *f)
{
  PushFD;
  Cli;
  if (f->out_ptr>f->in_ptr)
    result=f->mask+1-(f->out_ptr-f->in_ptr);
  else
    result=f->in_ptr-f->out_ptr;
  PopFD;
}
