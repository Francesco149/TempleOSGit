class byte_fifo
{
  BYTE *buffer;
  DWORD mask,in_ptr,out_ptr;
};

byte_fifo *new_byte_fifo(DWORD size)
{
  byte_fifo *f;
  f=malloc(sizeof(byte_fifo));
  f->buffer=malloc(size);
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

bool byte_fifo_insert(byte_fifo *f,BYTE ch)
{
  DWORD new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr)
    return false;
  else {
    f->buffer[f->in_ptr]=ch;
    f->in_ptr=new_in_ptr;
    return true;
  }
}

bool byte_fifo_remove(byte_fifo *f,BYTE *ch)
{
  if (f->in_ptr==f->out_ptr)
    return false;
  else {
    *ch=f->buffer[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    return true;
  }
}

bool byte_fifo_peek(byte_fifo *f,BYTE *ch)
{
  if (f->in_ptr==f->out_ptr)
    return false;
  else {
    *ch=f->buffer[f->out_ptr];
    return true;
  }
}

class dword_fifo
{
  DWORD *buffer;
  DWORD mask,in_ptr,out_ptr;
};

dword_fifo *new_dword_fifo(DWORD size)
{
  dword_fifo *f;
  f=malloc(sizeof(dword_fifo));
  f->buffer=malloc(size*sizeof(DWORD));
  f->mask=size-1;
  f->in_ptr=0;
  f->out_ptr=0;
  return f;
}

bool dword_fifo_insert(dword_fifo *f,DWORD d)
{
  DWORD new_in_ptr=(f->in_ptr+1)&f->mask;
  if (new_in_ptr==f->out_ptr)
    return false;
  else {
    f->buffer[f->in_ptr]=d;
    f->in_ptr=new_in_ptr;
    return true;
  }
}

bool dword_fifo_remove(dword_fifo *f,DWORD *d)
{
  if (f->in_ptr==f->out_ptr)
    return false;
  else {
    *d=f->buffer[f->out_ptr];
    f->out_ptr=(f->out_ptr+1)&f->mask;
    return true;
  }
}

bool dword_fifo_peek(dword_fifo *f,DWORD *d)
{
  if (f->in_ptr==f->out_ptr)
    return false;
  else {
    *d=f->buffer[f->out_ptr];
    return true;
  }
}

