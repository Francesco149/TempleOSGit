#help_index "Char/Define"

U0 LoadDocDefines()
{
//These are used in documents such as [::/LT/Doc/TrivialSolutions.TXZ] ::/LT/Doc/TrivialSolutions.TXZ
//The [C:/LT/OSMain/Adam1a.HPZ,1359] Ltf framework will search the sym table for define string
//constants if you specify that.
  BinFileHeaderStruct *bfh=sys_boot_base-sizeof(BinFileHeaderStruct);

  DefinePrintF(
    "DOC_OSMAIN","%08X",bfh);
  DefinePrintF(
    "DOC_OSMAIN2","%04X %04X",bfh(U64)/0x10000,bfh(U64)&0xFFFF);
  bfh(U64)+=bfh->file_size-1;
  DefinePrintF(
    "DOC_OSMAIN_END","%08X",bfh);
  DefinePrintF(
    "DOC_OSMAIN_END2","%04X %04X",bfh(U64)/0x10000,bfh(U64)&0xFFFF);

  DefinePrintF(
    "DOC_MP_VECT","%08X",MP_VECT_ADDR);
  DefinePrintF(
    "DOC_MP_VECT_END","%08X",MP_VECT_ADDR+SYS_MP_INIT_END-SYS_MP_INIT_START-1);


  DefinePrintF(
    "DOC_LOSETHOS_AGE","%h56D",GetCurTimeLTDate-A2LTDate("8/1/2003"));

  //This is cut and replaced when I generate a distribution.
  //See [::/LT/Demo/AcctExample/TSDistribute.APZ,DOC_LOSETHOS_LOC] DOC_LOSETHOS_LOC.
  -] LineCnt
    DefinePrintF("DOC_LOSETHOS_LOC","130,933");
    
  DefinePrintF(
    "DOC_PAGE_TABLE_BASE","%08X",PAGE_TABLE_BASE);
  DefinePrintF(
    "DOC_PAGE_TABLE_BASE2","%04X %04X",PAGE_TABLE_BASE/0x10000,PAGE_TABLE_BASE&0xFFFF);
  DefinePrintF(
    "DOC_PAGE_TABLE_END","%08X",PAGE_TABLE_BASE+PAGE_TABLE_SIZE-1);

  DefinePrintF(
    "DOC_SYS_HEAP_BASE","%08X",sys_heap_base);
  DefinePrintF(
    "DOC_SYS_HEAP_BASE2","%04X %04X",sys_heap_base/0x10000,sys_heap_base&0xFFFF);

  DefinePrintF(
    "DOC_MAPPED_MEM_SPACE_GIG","%d Gig",MAPPED_MEM_SPACE/0x40000000);
  DefinePrintF(
    "DOC_MAPPED_MEM_SPACE_END","%010X",MAPPED_MEM_SPACE-1);
  DefinePrintF(
    "DOC_MAPPED_MEM_SPACE_END2","%04X %04X %04X",(MAPPED_MEM_SPACE-1)/0x100000000,(MAPPED_MEM_SPACE-1)/0x10000&0xFFFF,(MAPPED_MEM_SPACE-1)&0xFFFF);
}

LoadDocDefines;
#help_index ""
