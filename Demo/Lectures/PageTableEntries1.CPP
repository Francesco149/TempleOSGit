/*What's funner than page tables for programmers to play with!!

This changes a page table entry for VGA graphics memory.
TempleOS keeps a cached copy of VGA memory in
[C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.screen_image.  See [C:/Adam/Gr/GrScreen.CPP.Z,380] GrUpdateVGAGraphics.

TempleOS used 4096 size pages for memory below addr 2 Meg
and 2 Meg pages above two Meg.  See [C:/Kernel/Mem1a.CPP.Z,163] MemPageSize() and [C:/Kernel/Mem1a.CPP.Z,1] INIT_MEMORY.
*/

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

//<CTRL-ALT-f> will flush screen VGA cache.
  VGAFlush;
  PressAKey;

  pte[0]=p0;
  pte[1]=p1;
  InvlPg(VGA_GRAPHIC_MEM_BASE);
  InvlPg(VGA_GRAPHIC_MEM_BASE+0x1000);

//<CTRL-ALT-f> will flush screen VGA cache.
  VGAFlush;
}

PageTableDemo1;

//See [C:/Kernel/Mem1a.CPP.Z,295] MemBlks2MegAlloc(), [C:/Kernel/Mem1a.CPP.Z,448] MemBlks2MegFree() for page aligned allocations.
//See [C:/Kernel/Adam1a.HPP.Z,2784] CBlkPool, [C:/Kernel/Adam1a.HPP.Z,2797] CHeapCtrl, [C:/Kernel/Adam1b.HPP.Z,153] sys_code_bp and [C:/Kernel/Adam1b.HPP.Z,154] sys_data_bp.
//See [C:/Doc/MemOverview.TXT.Z,1] Memory Overview and [Memory,1] Memory Routines.
