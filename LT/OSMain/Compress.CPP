//See [::/LT/Doc/Acknowledgements.TXZ] ::/LT/Doc/Acknowledgements.TXZ (4).
//See [C:/LT/OSMain/Adam1a.HPZ,3632] ArcCompressStruct, [C:/LT/OSMain/Adam1a.HPZ,3600] ArcTableEntry, and [C:/LT/OSMain/Adam1a.HPZ,3606] ArcCs

asm {
USE64
/****
U0 ArcGetTableEntry(ArcCs *c)
{
  U64 i;
  ArcTableEntry *temp,*temp1;

  if (c->entry_used) {
    i=c->free_index;

    c->entry_used=FALSE;
    c->cur_entry=c->next_entry;
    c->cur_bits_in_use=c->next_bits_in_use;
    if (c->next_bits_in_use<ARC_MAX_BITS) {
      c->next_entry = &c->compress[i++];
      if (i==c->free_limit) {
	c->next_bits_in_use++;
	c->free_limit=1<<c->next_bits_in_use;
      }
    } else {
      do if (++i==c->free_limit)
	   i=c->min_table_entry;
      while (c->hash[i]);
      temp=&c->compress[i];
      c->next_entry=temp;
      temp1=&c->hash[temp->basecode];
      while (temp1) {
	if (temp1->next==temp) {
	  temp1->next=temp->next;
	  break;
	} else
	  temp1=temp1->next;
      }
    }
    c->free_index=i;
  }
}
****/
_ARC_GET_TABLE_ENTRY::
	ENTER	0
	PUSH	RSI
	PUSH	RDI
	MOV	RSI,U64 SF_ARG1[RBP]
	BTR	U64 ArcCs.entry_used[RSI],0
	JNC	I32 @@30
	MOV	RDX,U64 ArcCs.free_index[RSI]
	XOR	RAX,RAX
	MOV	RAX,U64 ArcCs.next_entry[RSI]
	MOV	U64 ArcCs.cur_entry[RSI],RAX
	MOV	RCX,U64 ArcCs.next_bits_in_use[RSI]
	MOV	U64 ArcCs.cur_bits_in_use[RSI],RCX
	CMP	RCX,ARC_MAX_BITS
	JAE	@@05
	MOV	RAX,RDX
	SHL	RAX,4
	LEA	RAX,U64 ArcCs.compress[RSI+RAX]
	MOV	U64 ArcCs.next_entry[RSI],RAX
	INC	RDX
	CMP	U64 ArcCs.free_limit[RSI],RDX
	JNE	@@25
	INC	RCX
	MOV	U64 ArcCs.next_bits_in_use[RSI],RCX
	MOV	RAX,1
	SHL	RAX,CL
	MOV	U64 ArcCs.free_limit[RSI],RAX
	JMP	@@25
@@05:	INC	RDX
	CMP	U64 ArcCs.free_limit[RSI],RDX
	JNE	@@10
	MOV	RDX,U64 ArcCs.min_table_entry[RSI]
@@10:	MOV	RAX,U64 ArcCs.hash[RSI+RDX*8]
	OR	RAX,RAX
	JNZ	@@05
	MOV	RDI,RDX
	SHL	RDI,4
	LEA	RDI,U64 ArcCs.compress[RSI+RDI]
	MOV	U32 ArcCs.next_entry[RSI],EDI
	MOVZX	RBX,U16 ArcTableEntry.basecode[RDI]
	LEA	RCX,U64 ArcCs.hash[RSI+RBX*8]
@@15:	OR	RCX,RCX
	JZ	@@25
	MOV	RAX,U64 ArcTableEntry.next[RCX]
	CMP	RDI,RAX
	JNE	@@20
	MOV	RAX,U64 ArcTableEntry.next[RDI]
	MOV	U64 ArcTableEntry.next[RCX],RAX
	JMP	@@25
@@20:	MOV	RCX,RAX
	JMP	@@15
@@25:	MOV	U64 ArcCs.free_index[RSI],RDX
@@30:	POP	RDI
	POP	RSI
	LEAVE
	RET1	8
}

LTextern _ARC_GET_TABLE_ENTRY
	U0 ArcGetTableEntry(ArcCs *c);


U64 ArcXSum(U32 *src,U64 size)
{
  U64 result=0;
  size>>=2;
  while (size--)
    result^=*src++;
  return result;
}

I64 ArcDetermineCompressionType(U8 *src,U64 size)
{
  while (size--)
    if (*src++&0x80)
      return CT_8_BIT;
  return CT_7_BIT;
}

U0 ArcCompressBuf(ArcCs *c)
//Use [C:/LT/OSMain/Compress.CPZ,269] CompressBuf() unless you are doing
//more than one buffer.
{
  ArcTableEntry *temp,*temp1;
  U64 basecode;
  U8 *src_ptr,*src_limit,ch;

  src_ptr=c->src_buf+c->src_pos;
  src_limit=c->src_buf+c->src_size;

  if (c->saved_basecode==MAX_U32)
    basecode=*src_ptr++;
  else
    basecode=c->saved_basecode;

  while (src_ptr<src_limit &&
    c->dst_pos+c->cur_bits_in_use<=c->dst_size) {
    ArcGetTableEntry(c);

    arc_start_loop1:
      if (src_ptr>=src_limit) goto arc_done_compression;
      ch=*src_ptr++;
      if (temp=c->hash[basecode])
	do {
	  if (temp->ch==ch) {
	    basecode=temp-&c->compress[0];
	    goto arc_start_loop1;
	  }
	} while (temp=temp->next);

    BFieldOrU32(c->dst_buf,c->dst_pos,basecode);
    c->dst_pos+=c->cur_bits_in_use;

    c->entry_used=TRUE;
    temp=c->cur_entry;
    temp->basecode=basecode;
    temp->ch=ch;
    temp1=&c->hash[basecode];
    temp->next=temp1->next;
    temp1->next=temp;

    basecode=ch;
  }
arc_done_compression:
  c->saved_basecode=basecode;
  c->src_pos=src_ptr-c->src_buf;
}

U0 ArcFinishCompression(ArcCs *c)
{
  if (c->dst_pos+c->cur_bits_in_use<=c->dst_size) {
    BFieldOrU32(c->dst_buf,c->dst_pos,c->saved_basecode);
    c->dst_pos+=c->cur_bits_in_use;
  }
}

U0 ArcExpandBuf(ArcCs *c)
//Use [C:/LT/OSMain/Compress.CPZ,302] ExpandBuf() unless you know what
//you're doing.
{
  U8 *dst_ptr,*dst_limit;
  U64 basecode,lastcode,code;
  ArcTableEntry *temp,*temp1;

  dst_ptr=c->dst_buf+c->dst_pos;
  dst_limit=c->dst_buf+c->dst_size;

  while (dst_ptr<dst_limit &&
	 c->stk_ptr!=c->stk_base)
    *dst_ptr++ = * -- c->stk_ptr;

  if (c->stk_ptr==c->stk_base && dst_ptr<dst_limit) {
    if (c->saved_basecode==MAX_U32) {
      lastcode=BFieldExtU32(c->src_buf,c->src_pos,
	 c->next_bits_in_use);
      c->src_pos+=c->next_bits_in_use;
      *dst_ptr++=lastcode;
      ArcGetTableEntry(c);
      c->last_ch=lastcode;
    } else
      lastcode=c->saved_basecode;
    while (dst_ptr<dst_limit &&
      c->src_pos+c->next_bits_in_use<=c->src_size) {
      basecode=BFieldExtU32(c->src_buf,c->src_pos,
	 c->next_bits_in_use);
      c->src_pos+=c->next_bits_in_use;
      if (c->cur_entry==&c->compress[basecode]) {
	*c->stk_ptr++=c->last_ch;
	code=lastcode;
      } else
	code=basecode;
      while (code>=c->min_table_entry) {
	*c->stk_ptr++=c->compress[code].ch;
	code=c->compress[code].basecode;
      }
      *c->stk_ptr++=code;
      c->last_ch=code;

      c->entry_used=TRUE;
      temp=c->cur_entry;
      temp->basecode=lastcode;
      temp->ch=c->last_ch;
      temp1=&c->hash[lastcode];
      temp->next=temp1->next;
      temp1->next=temp;

      ArcGetTableEntry(c);
      while (dst_ptr<dst_limit && c->stk_ptr!=c->stk_base)
	*dst_ptr++ = * -- c->stk_ptr;
      lastcode=basecode;
    }
    c->saved_basecode=lastcode;
  }
  c->dst_pos=dst_ptr-c->dst_buf;
}

ArcCs *ArcCsNew(BoolI8 expand,BoolI8 text_only)
{
  ArcCs *c;
  c=CAlloc(sizeof(ArcCs));
  if (expand) {
    c->stk_base=MAlloc(ARC_MAX_TABLE_ENTRY+1);
    c->stk_ptr=c->stk_base;
  }
  if (text_only)
    c->min_bits=7;
  else
    c->min_bits=8;
  c->min_table_entry=1<<c->min_bits;
  c->free_index=c->min_table_entry;
  c->next_bits_in_use=c->min_bits+1;
  c->free_limit=1<<c->next_bits_in_use;
  c->saved_basecode=MAX_U32;
  c->entry_used=TRUE;
  ArcGetTableEntry(c);
  c->entry_used=TRUE;
  return c;
}

U0 ArcCsDel(ArcCs *c)
{
  Free(c->stk_base);
  Free(c);
}


ArcCompressStruct *CompressBuf(U8 *src,U64 size,
		U64 flags=0,TaskStruct *mem_task=NULL)
{
  U64 size_out;
  ArcCompressStruct *result;
  BoolI8 text_only=ArcDetermineCompressionType(src,size)==CT_7_BIT;
  ArcCs *c=ArcCsNew(FALSE,text_only);
  c->src_size=size;
  c->src_buf=src;
  c->dst_size=(size+sizeof(ArcCompressStruct))<<3;
  c->dst_buf=CAlloc(c->dst_size>>3);
  c->dst_pos=sizeof(ArcCompressStruct)<<3;
  ArcCompressBuf(c);
  ArcFinishCompression(c);
  if (c->src_pos==c->src_size) {
    size_out=(c->dst_pos+7)>>3;
    result=MAlloc(size_out,mem_task);
    MemCpy(result,c->dst_buf,size_out);
    result->compression_type=text_only ? CT_7_BIT:CT_8_BIT;
    result->compressed_size=size_out;
  } else {
    result=MAlloc(size+sizeof(ArcCompressStruct),mem_task);
    MemCpy(&result->body,src,size);
    result->compression_type=CT_NONE;
    result->compressed_size=size+sizeof(ArcCompressStruct);
  }
  result->expanded_size=size;
  result->flags=flags;
  Free(c->dst_buf);
  ArcCsDel(c);
  return result;
}

U8 *ExpandBuf(ArcCompressStruct *r,TaskStruct *mem_task=NULL)
{
  ArcCs *c;
  U8 *result;
  BoolI8 text_only;
  result=MAlloc(r->expanded_size+1,mem_task);
  result[r->expanded_size]=0; //terminate
  text_only= r->compression_type==CT_7_BIT;
  if (r->compression_type==CT_NONE) {
    MemCpy(result,&r->body,r->expanded_size);
    goto expand_end;
  }
  c=ArcCsNew(TRUE,text_only);
  c->src_size=r->compressed_size<<3;
  c->src_pos=sizeof(ArcCompressStruct)<<3;
  c->src_buf=r;
  c->dst_size=r->expanded_size;
  c->dst_buf=result;
  c->dst_pos=0;
  ArcExpandBuf(c);
  ArcCsDel(c);

expand_end:
  return result;
}

