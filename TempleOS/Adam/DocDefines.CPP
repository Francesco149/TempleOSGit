#help_index "Char/Define"

U0 LoadDocDefines()
{
//These are used in documents such as [C:/TempleOS/Doc/TrivialSolutions.TXT.Z,1]
//The [C:/TempleOS/Kernel/Adam1a.HPP.Z,1509] CDoc framework will search the sym table for define string
//consts if you specify that.
  CBinFile *bfh=sys_boot_base-sizeof(CBinFile);

  DefinePrint(
    "DD_KERNEL","%08X",bfh);
  DefinePrint(
    "DD_KERNEL2","%09,X",bfh);
  bfh(I64)+=bfh->file_size-1;
  DefinePrint(
    "DD_KERNEL_END","%08X",bfh);
  DefinePrint(
    "DD_KERNEL_END2","%09,X",bfh);
  bfh(I64)++;
  DefinePrint(
    "DD_COMPILER","%08X",bfh);
  DefinePrint(
    "DD_COMPILER2","%09,X",bfh);
  bfh(I64)+=bfh->file_size-1;
  DefinePrint(
    "DD_COMPILER_END","%08X",bfh);
  DefinePrint(
    "DD_COMPILER_END2","%09,X",bfh);

  //[C:/TempleOS/Adam/Boot/BootCD.CPP.Z,1] DD_BOOT_HIGH_LOC

  DefinePrint(
    "DD_MP_VECT","%08X",MP_VECT_ADDR);
  DefinePrint(
    "DD_MP_VECT_END","%08X",MP_VECT_ADDR+SYS_MP_INIT_END-SYS_MP_INIT_START-1);

  DefinePrint(
    "DD_OS_NAME_VERSION","TempleOS V%0.2f",os_version);
  DefinePrint(
    "DD_TEMPLEOS_AGE","%h56D",GetDateTime-A2Date("8/1/2003"));

  //This is cut and replaced when I generate a distro.
  //See [C:/TempleOS/Demo/AcctExample/TSDistro.CPP.Z,1] DD_TEMPLEOS_LOC.
  -] LineCnt
    DefinePrint("DD_TEMPLEOS_LOC","124,033");
    
  DefinePrint(
    "DD_PAGE_TABLE_BASE","%08X",PAGE_TABLE_BASE);
  DefinePrint(
    "DD_PAGE_TABLE_BASE2","%09,X",PAGE_TABLE_BASE);
  DefinePrint(
    "DD_PAGE_TABLE_END","%08X",PAGE_TABLE_BASE+PAGE_TABLE_SIZE-1);

  DefinePrint(
    "DD_SYS_HEAP_BASE","%08X",sys_heap_base);
  DefinePrint(
    "DD_SYS_HEAP_BASE2","%09,X",sys_heap_base);

  DefinePrint(
    "DD_SYS_HEAP_LIMIT","%08X",sys_heap_limit);
  DefinePrint(
    "DD_SYS_HEAP_LIMIT2","%09,X",sys_heap_limit);

  DefinePrint(
    "DD_MAPPED_MEM_SPACE_GIG","%d Gig",MAPPED_MEM_SPACE/0x40000000);
  DefinePrint(
    "DD_MAPPED_MEM_SPACE_END","%010X",MAPPED_MEM_SPACE-1);
  DefinePrint(
    "DD_MAPPED_MEM_SPACE_END2","%014,X",MAPPED_MEM_SPACE-1);
}

LoadDocDefines;
#help_index ""
