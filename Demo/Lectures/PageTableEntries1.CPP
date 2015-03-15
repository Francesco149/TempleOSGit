/*What's funner than page tables for programmers to play with!!

This changes a page table entry for VGA graphics memory.
TempleOS keeps a cached copy of VGA memory in
[C:/Adam/Gr/GrGlbls.CPP.Z,3] gr.screen_image.  See [C:/Adam/Gr/GrScreen.CPP.Z,377] GrUpdateVGAGraphics.

TempleOS used 4096 size pages for memory below addr 2 Meg
and 2 Meg pages above two Meg.  See [C:/Kernel/MemA.CPP.Z,164] MemPageSize() and [C:/Kernel/MemA.CPP.Z,1] INIT_MEMORY.
*/

U0 PageTableDemo1()
{
  I64 *pte=MemPageTable(VGAM_GRAPHICS),p0,p1;
  "Page table entries for VGA screen memory\n";
  DocD(pte);
  PressAKey;

  p0=pte[0];
  p1=pte[1];
  pte[0]=p1;
  pte[1]=p0;
  InvlPg(VGAM_GRAPHICS); //Invalidate translation look aside buffer (TLB).
  InvlPg(VGAM_GRAPHICS+0x1000);

//Use this if you wish to change
//the page table reg.

//  SetRAX(0x100000+0x1000*(MEM_NUM_PML1+MEM_NUM_PML2+MEM_NUM_PML3));
//  MOV_CR3_EAX

//<CTRL-ALT-f> will flush screen VGA cache.
  VGAFlush;
  PressAKey;

  pte[0]=p0;
  pte[1]=p1;
  InvlPg(VGAM_GRAPHICS);
  InvlPg(VGAM_GRAPHICS+0x1000);

//<CTRL-ALT-f> will flush screen VGA cache.
  VGAFlush;
}

PageTableDemo1;

//See [C:/Kernel/MemA.CPP.Z,296] Mem2MegAlloc(), [C:/Kernel/MemA.CPP.Z,449] Mem2MegFree() for page aligned allocations.
//See [C:/Kernel/KernelA.HPP.Z,2783] CBlkPool, [C:/Kernel/KernelA.HPP.Z,2796] CHeapCtrl, [C:/Kernel/KernelB.HPP.Z,150] sys_code_bp and [C:/Kernel/KernelB.HPP.Z,151] sys_data_bp.
//See [C:/Doc/MemOverview.TXT.Z,1] Memory Overview and [Memory,1] Memory Routines.
