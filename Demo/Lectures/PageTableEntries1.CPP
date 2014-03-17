//What's funner than page tables for programmers to play with!!

//This changes a page table entry for VGA graphics memory.
//TempleOS keeps a cached copy of VGA memory in
//[C:/Adam/Gr/GrGlbls.CPP.Z,2] gr_screen_image.  See [C:/Adam/Gr/GrScreen.CPP.Z,390] GrUpdateVGAGraphics.

//TempleOS used 4096 size pages for memory below addr 2 Meg
//and 2 Meg pages above two Meg.  See [C:/Kernel/Mem1a.CPP.Z,166] MemPageSize() and [C:/Kernel/Mem1a.CPP.Z,1] INIT_MEMORY.


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
//the page table reg.

//  SetRAX(0x100000+0x1000*(MEM_NUM_PML1+MEM_NUM_PML2+MEM_NUM_PML3));
//  MOV_CR3_EAX

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

//See [C:/Kernel/Mem1a.CPP.Z,301] MemBlks2MegAlloc(), [C:/Kernel/Mem1a.CPP.Z,460] MemBlks2MegFree() for page aligned allocations.
//See [C:/Kernel/Adam1a.HPP.Z,2594] CBlkPool, [C:/Kernel/Adam1a.HPP.Z,2608] CHeapCtrl, [C:/Kernel/Adam1b.HPP.Z,120] sys_code_bp, [C:/Kernel/Adam1b.HPP.Z,121] sys_data_bp.
//See [Memory,1] Memory
