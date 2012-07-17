//What's funner than page tables for programmers to play with!!

//This changes a page table entry for VGA graphics memory.
//LoseThos keeps a cached copy of VGA memory in
//[C:/LT/Adam/Gr/GrScreen.APZ,2] gr_screen_image.  See [C:/LT/Adam/Gr/GrScreen.APZ,691] GrUpdateVGAGraphics.

//LoseThos used 4096 size pages for memory below addr 2 Meg
//and 2 Meg pages above two Meg.  See [C:/LT/OSMain/Memory.CPZ,183] MemPageSize() and [::/LT/OSMain/Memory.CPZ,SYS_INIT_MEM] INIT_MEMORY.


U0 PageTableDemo1()
{
  U64 *pte=MemPointAtPageTableEntry(VGA_GRAPHIC_MEM_BASE),p0,p1;
  "Page table entries for VGA screen memory\n";
  LtfD(pte);
  PressAKey;

  p0=pte[0];
  p1=pte[1];
  pte[0]=p1;
  pte[1]=p0;
  InvlPg(VGA_GRAPHIC_MEM_BASE); //Invalidate translation look aside buffer (TLB).
  InvlPg(VGA_GRAPHIC_MEM_BASE+0x1000);

//Use this if you wish to change
//the page table register.

//  SetRAX(0x100000+0x1000*(NUM_PML1+NUM_PML2+NUM_PML3));
//  asm {MOV_CR3_EAX}

  LBts(&sys_semas[SYS_SEMA_FLUSH_VGA_IMAGE],0);
  PressAKey;

  pte[0]=p0;
  pte[1]=p1;
  InvlPg(VGA_GRAPHIC_MEM_BASE);
  InvlPg(VGA_GRAPHIC_MEM_BASE+0x1000);

  LBts(&sys_semas[SYS_SEMA_FLUSH_VGA_IMAGE],0);
}

PageTableDemo1;

//See [C:/LT/OSMain/Memory.CPZ,325] Alloc2MegMemBlks(), [C:/LT/OSMain/Memory.CPZ,482] Free2MegMemBlks() for page aligned allocations.
//See [C:/LT/OSMain/Adam1a.HPZ,2863] BlkPool, [C:/LT/OSMain/Adam1a.HPZ,2878] HeapCtrl, [C:/LT/OSMain/Adam1b.HPZ,120] sys_code_bp, [C:/LT/OSMain/Adam1b.HPZ,121] sys_data_bp.
//See [Memory] Memory
