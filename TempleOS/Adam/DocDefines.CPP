#help_index "Char/Define"

U0 LoadDocDefines()
{
//These are used in documents such as [C:/TempleOS/Doc/TrivialSolutions.TXT.Z,1]
//The [C:/TempleOS/Kernel/Adam1a.HPP.Z,1509] CDoc framework will search the sym table for define string
//constants if you specify that.
  CBinFile *bfh=sys_boot_base-sizeof(CBinFile);

  DefinePrintF(
    "DD_KERNEL","%08X",bfh);
  DefinePrintF(
    "DD_KERNEL2","%09,X",bfh);
  bfh(I64)+=bfh->file_size-1;
  DefinePrintF(
    "DD_KERNEL_END","%08X",bfh);
  DefinePrintF(
    "DD_KERNEL_END2","%09,X",bfh);

  DefinePrintF(
    "DD_MP_VECT","%08X",MP_VECT_ADDR);
  DefinePrintF(
    "DD_MP_VECT_END","%08X",MP_VECT_ADDR+SYS_MP_INIT_END-SYS_MP_INIT_START-1);


  DefinePrintF(
    "DD_TEMPLEOS_AGE","%h56D",GetCurTimeCDate-A2CDate("8/1/2003"));

  //This is cut and replaced when I generate a distro.
  //See [C:/TempleOS/Demo/AcctExample/TSDistro.CPP.Z,1] DD_TEMPLEOS_LOC.
  -] LineCnt
    DefinePrintF("DD_TEMPLEOS_LOC","142,985");
    
  DefinePrintF(
    "DD_PAGE_TABLE_BASE","%08X",PAGE_TABLE_BASE);
  DefinePrintF(
    "DD_PAGE_TABLE_BASE2","%09,X",PAGE_TABLE_BASE);
  DefinePrintF(
    "DD_PAGE_TABLE_END","%08X",PAGE_TABLE_BASE+PAGE_TABLE_SIZE-1);

  DefinePrintF(
    "DD_SYS_HEAP_BASE","%08X",sys_heap_base);
  DefinePrintF(
    "DD_SYS_HEAP_BASE2","%09,X",sys_heap_base);

  DefinePrintF(
    "DD_MAPPED_MEM_SPACE_GIG","%d Gig",MAPPED_MEM_SPACE/0x40000000);
  DefinePrintF(
    "DD_MAPPED_MEM_SPACE_END","%010X",MAPPED_MEM_SPACE-1);
  DefinePrintF(
    "DD_MAPPED_MEM_SPACE_END2","%014,X",MAPPED_MEM_SPACE-1);
}

LoadDocDefines;
#help_index ""
