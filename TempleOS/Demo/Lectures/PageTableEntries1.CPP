//What's funner than page tables for programmers to play with!!

//This changes a page table entry for VGA graphics memory.
//TempleOS keeps a cached copy of VGA memory in
//[C:/TempleOS/Adam/Gr/GrScreen.CPP.Z,2] gr_screen_image.  See [C:/TempleOS/Adam/Gr/GrScreen.CPP.Z,719] GrUpdateVGAGraphics.

//TempleOS used 4096 size pages for memory below addr 2 Meg
//and 2 Meg pages above two Meg.  See [C:/TempleOS/Kernel/Memory.CPP.Z,185] MemPageSize() and [C:/TempleOS/Kernel/Memory.CPP.Z,1] INIT_MEMORY.


U0 PageTableDemo1()
{
  I64 *pte=MemPageTable(VGA_GRAPHIC_MEM_BASE),p0,p1;
  "Page table entries for VGA screen memory\n";
  DocD(pte);
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

//<CTRL-ALT-F> will flush screen VGA cache.
  VGAFlush;
  PressAKey;

  pte[0]=p0;
  pte[1]=p1;
  InvlPg(VGA_GRAPHIC_MEM_BASE);
  InvlPg(VGA_GRAPHIC_MEM_BASE+0x1000);

//<CTRL-ALT-F> will flush screen VGA cache.
  VGAFlush;
}

PageTableDemo1;

//See [C:/TempleOS/Kernel/Memory.CPP.Z,321] MemBlks2MegAlloc(), [C:/TempleOS/Kernel/Memory.CPP.Z,480] MemBlks2MegFree() for page aligned allocations.
//See [C:/TempleOS/Kernel/Adam1a.HPP.Z,2741] CBlkPool, [C:/TempleOS/Kernel/Adam1a.HPP.Z,2757] CHeapCtrl, [C:/TempleOS/Kernel/Adam1b.HPP.Z,120] sys_code_bp, [C:/TempleOS/Kernel/Adam1b.HPP.Z,121] sys_data_bp.
//See [Memory,1] Memory
