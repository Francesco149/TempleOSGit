asm {
/*
See [C:/Doc/Acknowledgements.TXT.Z,1].
See [C:/Doc/Mem.TXT.Z,1]
*/

USE32
SYS_INIT_MEM::
//Set page tables to identity map everything.
	MOV	EDI,MEM_PAGE_TABLE_BASE
	XOR	EAX,EAX
	MOV	ECX,0x1000*(MEM_NUM_PML1+MEM_NUM_PML2+MEM_NUM_PML3+MEM_NUM_PML4)/4
	REP_STOSD

//PML1
	MOV	EAX,0x107
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE
	MOV	ECX,0x200*MEM_NUM_PML1
@@05:	MOV	U32 [EDI],EAX
	ADD	EDI,4
	MOV	U32 [EDI],EDX
	ADD	EDI,4
	ADD	EAX,0x1000
	ADC	EDX,0
	LOOP	@@05

//video ram=write through
//0xA0000-0xBFFFF
	MOV	EAX,0xF
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0xA0*8
	MOV	ECX,0xC0-0xA0
@@10:	OR	U32 [EDI],EAX
	ADD	EDI,4
	OR	U32 [EDI],EDX
	ADD	EDI,4
	LOOP	@@10

//PML2
	MOV	EAX,MEM_PAGE_TABLE_BASE+7
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0x1000*MEM_NUM_PML1
	MOV	ECX,MEM_NUM_PML1
@@15:	MOV	U32 [EDI],EAX
	ADD	EDI,4
	MOV	U32 [EDI],EDX
	ADD	EDI,4
	ADD	EAX,0x1000
	ADC	EDX,0
	LOOP	@@15

	MOV	EAX,MEM_NUM_PML1*0x200000+0x87 //bit 7 is page size (1==2Meg)
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0x1000*MEM_NUM_PML1+8*MEM_NUM_PML1
	MOV	ECX,MEM_NUM_PML2*0x200-MEM_NUM_PML1
@@20:	MOV	U32 [EDI],EAX
	ADD	EDI,4
	MOV	U32 [EDI],EDX
	ADD	EDI,4
	ADD	EAX,0x200000
	ADC	EDX,0
	LOOP	@@20

//PCI Devs
//0xE0000000-0xFFFFFFFF
	MOV	EAX,0x17	//Cache disabled
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0x1000*MEM_NUM_PML1+8*0x700
	MOV	ECX,0x800-0x700
@@25:	OR	U32 [EDI],EAX
	ADD	EDI,4
	OR	U32 [EDI],EDX
	ADD	EDI,4
	LOOP	@@25

//PML3
	MOV	EAX,MEM_PAGE_TABLE_BASE+MEM_NUM_PML1*0x1000+7
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0x1000*(MEM_NUM_PML1+MEM_NUM_PML2)
	MOV	ECX,MEM_NUM_PML2
@@30:	MOV	U32 [EDI],EAX
	ADD	EDI,4
	MOV	U32 [EDI],EDX
	ADD	EDI,4
	ADD	EAX,0x1000
	ADC	EDX,0
	LOOP	@@30

//PML4
	MOV	EAX,MEM_PAGE_TABLE_BASE+(MEM_NUM_PML1+MEM_NUM_PML2)*0x1000+7
	XOR	EDX,EDX
	MOV	EDI,MEM_PAGE_TABLE_BASE+0x1000*(MEM_NUM_PML1+MEM_NUM_PML2+MEM_NUM_PML3)
	MOV	ECX,1
@@35:	MOV	U32 [EDI],EAX
	ADD	EDI,4
	MOV	U32 [EDI],EDX
	ADD	EDI,4
	ADD	EAX,0x1000
	ADC	EDX,0
	LOOP	@@35

	MOV	U32 [SYS_CODE_BP],SYS_BP_START
	MOV	U32 [SYS_CODE_BP+4],0

	MOV	U32 [SYS_DATA_BP],0
	MOV	U32 [SYS_DATA_BP+4],0

	MOV	U32 [SYS_BP_LOCKED_FLAGS],0
	MOV	U32 [SYS_BP_LOCKED_FLAGS+4],0
	MOV	U32 [SYS_BP_SIZE],0
	MOV	U32 [SYS_BP_SIZE+4],0
	MOV	U32 [SYS_BP_ALLOCATED_U8S],0
	MOV	U32 [SYS_BP_ALLOCATED_U8S+4],0

	MOV	EDI,U32 SYS_BP_FREE_PAGE_HASH
	MOV	ECX,FREE_PAGE_HASH_SIZE*2
	XOR	EAX,EAX
	REP_STOSD
	MOV	EDI,U32 SYS_BP_FREE_PAGE_HASH2
	MOV	ECX,(64-PAGE_BITS)*2
	REP_STOSD

	MOV	U32 [SYS_BP_MEM_FREE_LST],0
	MOV	U32 [SYS_BP_MEM_FREE_LST+4],0
	MOV	U32 [SYS_BP_MEM_FREE_2MEG_LST],0
	MOV	U32 [SYS_BP_MEM_FREE_2MEG_LST+4],0

	XOR	EAX,EAX
	MOV	AX,U16 [SYS_MEM_E801] //1 Kb blks between 1M and 16M
	SHL	EAX,10
	ADD	EAX,0x100000
	MOV	EBX,U32 [SYS_HEAP_BASE] //0x183000
	SUB	EAX,EBX

/*ExpandHeap
TempleOS initializes the heap to the val
of the BIOS E801 lowest 16Meg val.  It
later adds the rest to the heap.  There
are a few allocations which occur while
it is set to this small val.

At some point, it might be nice to
initialize to the full val but it is a
pain without MAlloc up-and-running.
*/

#define CONSERVATIVE_16MEG_SIZE	0xF00000
#define ESTIMATED_MISC_ALLOCS	0x100000

#assert CONSERVATIVE_16MEG_SIZE-0x183000>ADAM_STK+ESTIMATED_MISC_ALLOCS

//EBX=BASE EAX=SIZE
	TEST	U8 [SYS_MEM_INIT_FLAG],1
	JZ	@@40
	PUSH	EAX
	MOV	EDI,EBX
	MOV	ECX,EAX
	MOV	AL,U8 [SYS_MEM_INIT_VAL]
	REP_STOSB
	POP	EAX

@@40:	SHR	EAX,PAGE_BITS
	MOV	EDI,U32 [SYS_BP_MEM_FREE_LST]
	MOV	U32 CMemBlk.next[EBX],EDI
	MOV	U32 CMemBlk.next+4[EBX],0
	MOV	U32 [SYS_BP_MEM_FREE_LST],EBX
	MOV	U32 [SYS_BP_MEM_FREE_LST+4],0
	MOV	U32 CMemBlk.mb_signature[EBX],MBS_UNUSED_SIGNATURE_VAL
	MOV	U32 CMemBlk.pages[EBX],EAX
	ADD	U32 [SYS_BP_SIZE],EAX
	RET
}

U0 SysBadFree(I64 *ptr)
{
  Dbg("Bad Free:",ptr);
}

U0 SysBadMAlloc(I64 *ptr)
{
  Dbg("Bad MAlloc:",ptr);
}

I64 MemPageSize(U8 *a)
{
  if (a<MEM_NUM_PML1*0x1000*0x200)
    return 0x1000;
  else
    return 0x200000;
}

I64 *MemPageTable(U8 *a)
{
  if (a<MEM_NUM_PML1*0x1000*0x200)
    return MEM_PAGE_TABLE_BASE+ a>>12 *8;
  else
    return MEM_PAGE_TABLE_BASE+MEM_NUM_PML1*0x1000+
	   a>>21*8;
}

Bool MemPagePresentMark(U8 *a,Bool val)
{
  I64 *pte=MemPageTable(a);
  Bool result=*pte&1;
  if (val)
    *pte=*pte|1;  //Mark present
  else
    *pte=*pte&~1; //Mark not present
  InvlPg(a);
  return result;
}


U8 *MemBlksIndependentAlloc(I64 *_pages512,CBlkPool *bp=NULL)
//This will allocate a 512 byte pages from
//and not link them to any task.  (Linking to a task
//means it will be freed when the task
//dies.)
//It might give you more than you asked for
//so a ptr to a page count is passed.
//
//Returns NULL if out of memory
{
  CMemBlk *result=NULL,*m;
  I64 i,num=*_pages512;
  if (!bp) bp=sys_code_bp;
  PushFD;
  Cli;
  while (LBts(&bp->locked_flags,BPlf_LOCKED))
    Pause;
  if (num<FREE_PAGE_HASH_SIZE) {
    if (result=bp->free_page_hash[num]) {
      bp->free_page_hash[num]=result->next;
      goto at_done;
    }
    i=Bsr(FREE_PAGE_HASH_SIZE)+1;
  } else {
//We'll now round-up to a power of two.
//There is some overhead on allocations and
//we wouldn't want to round to the next
//power of two if a power of two was requested.
//So we use a little more than a power of two.
    num-=2;
    i=Bsr(num)+1;
    num=1<<i+2;
    if (result=bp->free_page_hash2[i]) {
      bp->free_page_hash2[i]=result->next;
      goto at_done;
    }
  }
  m=&bp->mem_free_lst;
  while (TRUE) {
    if (!(result=m->next)) {
      //We're probably out of luck, but lets search for a
      //freed larger size block... and, screw-it, return the whole thing.
      do {
	if (result=bp->free_page_hash2[++i]) {
	  num=1<<i+2;
	  bp->free_page_hash2[i]=result->next;
	  goto at_done;
	}
      } while (i<64-PAGE_BITS-1);
      num=0;
      result=NULL; //Out of memory
      goto at_done;
    }
    if (result->pages<num)
      m=result;
    else {
      if (result->pages==num) {
	m->next=result->next;
	goto at_done;
      } else {
	result->pages-=num;
	result(U8 *)+=result->pages<<PAGE_BITS;
	result->pages=num;
	goto at_done;
      }
    }
  }
at_done:
  *_pages512=num;
  bp->allocated_u8s+=num<<PAGE_BITS;
  LBtr(&bp->locked_flags,BPlf_LOCKED);
  PopFD;
  return result;
}

U0 MemBlksNonTaskFree(CMemBlk *m,I64 pages512,CBlkPool *bp=NULL)
{
  I64 i;
  if (m) {
    if (!bp) bp=sys_code_bp;
    PushFD;
    Cli;
    while (LBts(&bp->locked_flags,BPlf_LOCKED))
      Pause;
    m->mb_signature=MBS_UNUSED_SIGNATURE_VAL;
    m->pages=pages512;
    bp->allocated_u8s-=pages512<<PAGE_BITS;
    if (pages512<FREE_PAGE_HASH_SIZE) {
      m->next=bp->free_page_hash[pages512];
      bp->free_page_hash[pages512]=m;
    } else {
//We'll now round-up to a power of two.
//There is some overhead on allocations and
//we wouldn't want to round to the next
//power of two if a power of two was requested.
//So we use a little more than a power of two.
      pages512-=2;
      i=Bsr(pages512);
      m->next=bp->free_page_hash2[i];
      bp->free_page_hash2[i]=m;
    }
    LBtr(&bp->locked_flags,BPlf_LOCKED);
    PopFD;
  }
}

U8 *MemBlks2MegAlloc(I64 *_pages2Meg,CBlkPool *bp=NULL)
//This will allocate 2Meg pages and not
//link it to any task.	(Linking to a task
//means they will be freed when the task
//dies.)
//It might give you more than you asked for
//so a ptr to a page count is passed.
//
//Returns NULL if out of memory
{
  I64 i,j,*pte,num=*_pages2Meg;
  CMemBlk *result=NULL,*m,*m1;

  if (!bp) bp=sys_code_bp;
  PushFD;
  Cli;
  while (LBts(&bp->locked_flags,BPlf_LOCKED))
    Pause;
  num<<=21-PAGE_BITS;

  m=&bp->mem_free_2meg_lst;
  while (TRUE) {
    if (!(result=m->next))
      break;
    if (result->pages<num)
      m=result;
    else {
      if (result->pages==num) {
	m->next=result->next;
	goto am_done;
      } else {
	result->pages-=num;
	result(U8 *)+=result->pages<<PAGE_BITS;
	result->pages=num;
	goto am_done;
      }
    }
  }

  m=&bp->mem_free_lst;
  while (TRUE) {
    if (!(result=m->next)) {
      num=0;
      result=NULL; //Out of memory
      goto am_done;
    }
    if (result->pages<num)
      m=result;
    else {
      if (result->pages==num) {
	if (result(U8 *)&0x1FFFFF)
	  m=result;
	else {
	  m->next=result->next;
	  goto am_done;
	}
      } else {
	if (i=(result(U8 *)&0x1FFFFF)>>PAGE_BITS) {
	  j=1<<(21-PAGE_BITS)-i;
	  if (result->pages<num+j)
	    m=result;
	  else if (result->pages==num+j) {
	    result->pages-=num;
	    result(U8 *)+=result->pages<<PAGE_BITS;
	    result->pages=num;
	    goto am_done;
	  } else {
	    m1=result;
	    result(U8 *)+=j<<PAGE_BITS;
	    result->pages=num;
	    m=result(U8 *)+num<<PAGE_BITS;
	    m->pages=m1->pages-num-j;
	    m1->pages=j;
	    m->next=m1->next;
	    m1->next=m;
	    m->mb_signature=MBS_UNUSED_SIGNATURE_VAL;
	    goto am_done;
	  }
	} else {
	  m=m->next=result(U8 *)+num<<PAGE_BITS;
	  m->next=result->next;
	  m->pages=result->pages-num;
	  m->mb_signature=MBS_UNUSED_SIGNATURE_VAL;
	  result->pages=num;
	  goto am_done;
	}
      }
    }
  }
am_done:
  i=num<<PAGE_BITS;
  bp->allocated_u8s+=i;
  num>>=21-PAGE_BITS;
  *_pages2Meg=num;
  m=result;
  m1=m(U8 *)+i;
  while (m<m1) {
    pte=MemPageTable(m);
    *pte &= ~0x18;
    InvlPg(m);
    m(U8 *)+=MemPageSize(m);
  }
  LBtr(&bp->locked_flags,BPlf_LOCKED);
  PopFD;
  return result;
}

U8 *MemBlksUncachedAlloc(I64 *_pages2Meg,CBlkPool *bp=NULL)
//This will allocate 2Meg pages from
//and not link them to any task.  (Linking to a task
//means it will be freed when the task
//dies.)  It will be marked uncached.
//It might give you more than you asked for
//so a ptr to a page count is passed.
//
//Returns NULL if out of memory
{
  CMemBlk *result,*m,*m1;
  I64 num=*_pages2Meg,*pte;
  if (result=MemBlks2MegAlloc(_pages2Meg,bp)) {
    num=*_pages2Meg;
    m=result;
    m1=m(U8 *)+num<<21;
    while (m<m1) {
      pte=MemPageTable(m);
      *pte= *pte& ~0x18 |0x10;
      InvlPg(m);
      m(U8 *)+=MemPageSize(m);
    }
  }
  return result;
}

U8 *MemBlksWriteThroughAlloc(I64 *_pages2Meg,CBlkPool *bp=NULL)
//This will allocate 2Meg pages from
//and not link them to any task.  (Linking to a task
//means they will be freed when the task
//dies.)  It will be marked write-through.
//It might give you more than you asked for
//so a ptr to a page count is passed.
//
//Returns NULL if out of memory
{
  CMemBlk *result,*m,*m1;
  I64 num=*_pages2Meg,*pte;
  if (result=MemBlks2MegAlloc(_pages2Meg,bp)) {
    num=*_pages2Meg;
    m=result;
    m1=m(U8 *)+num<<21;
    while (m<m1) {
      pte=MemPageTable(m);
      *pte= *pte& ~0x18 |8;
      InvlPg(m);
      m(U8 *)+=MemPageSize(m);
    }
  }
  return result;
}

U0 MemBlks2MegFree(CMemBlk *m,I64 pages2Meg,CBlkPool *bp=NULL)
{
  I64 *pte;
  CMemBlk *m1,*m2;
  if (m) {
    if (!bp) bp=sys_code_bp;
    m2=m;
    m1=m(U8 *)+pages2Meg<<21;
    while (m2<m1) {
      pte=MemPageTable(m2);
      *pte=*pte & ~0x18;
      InvlPg(m2);
      m2(U8 *)+=MemPageSize(m2);
    }
    PushFD;
    Cli;
    while (LBts(&bp->locked_flags,BPlf_LOCKED))
      Pause;
    m->mb_signature=MBS_UNUSED_SIGNATURE_VAL;
    m->pages=pages2Meg<<(21-PAGE_BITS);
    bp->allocated_u8s-=pages2Meg<<21;
    m->next=bp->mem_free_2meg_lst;
    bp->mem_free_2meg_lst=m;
    LBtr(&bp->locked_flags,BPlf_LOCKED);
    PopFD;
  }
}

CMemBlk *MemBlksAlloc(I64 *_pages512,CHeapCtrl *hc)
{
//hc must be locked.  Interrupts should probably be off
//Currently, this is only called from [C:/Kernel/Mem1b.CPP.Z,376] MAlloc().
//
//Returns NULL if out of memory
  CMemBlk *result;
  I64 threshold,cnt,size;
  CMemUnusedAllocated *uum,**_uum,**_ptr;
  if (result=MemBlksIndependentAlloc(_pages512,hc->bp)) {
    QueIns(result,hc->last_mem_blk);
    result->mb_signature=MBS_USED_SIGNATURE_VAL;

    //Tidy-up free lst (Move into heap hash)
    //because if free lst gets long, delay causes crash
    threshold=HEAP_HASH_SIZE>>4;
#assert HEAP_HASH_SIZE>>4>=sizeof(U8 *)
    do {
      cnt=0;
      _uum=&hc->malloc_free_lst;
      while (uum=*_uum) {
#assert !offset(CMemUnusedAllocated.next)
	size=uum->size;
	if (size<threshold) {
	  *_uum=uum->next;
	  _ptr=(&hc->heap_hash)(U8 *)+size;
	  uum->next=*_ptr;
	  *_ptr=uum;
	} else {
	  cnt++;
	  _uum=uum;
	}
      }
      threshold<<=1;
    } while (cnt>8 && threshold<=HEAP_HASH_SIZE);
  }
  return result;
}

U0 MemBlksFree(CMemBlk *m,CHeapCtrl *hc)
{ //hc must be locked
  if (m) {
    PushFD;
    Cli;
    if (m->mb_signature!=MBS_USED_SIGNATURE_VAL)
      SysBadFree(m);
    else {
      QueRem(m);
      MemBlksNonTaskFree(m,m->pages,hc->bp);
    }
    PopFD;
  }
}
