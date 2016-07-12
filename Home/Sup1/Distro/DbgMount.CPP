#help_index "Install;File/Cmd Line (Typically);Cmd Line (Typically);"
public I64 Mount(Bool repartition=FALSE)
{
  I64 cnt,res=0,num_hints,drv_let,type,unit;
  U8 blks_buf[STR_LEN],addr_buf[STR_LEN],base0_buf[STR_LEN],base1_buf[STR_LEN],
	*filename=NULL,*filename2=NULL;
  CATARep *head=NULL,*tempha;
  Bool one_drv;
  CBlkDev *bd;
  do {
    cnt=0;
    DrvRep;
    "\n****** Mount Drives ******\n"
	  "$GREEN$A$FG$-$GREEN$B$FG$ are RAM drives.\n"
	  "$GREEN$C$FG$-$GREEN$L$FG$ are ATA hard drives.\n"
	  "$GREEN$M$FG$-$GREEN$P$FG$ are ISO file read drives.\n"
	  "$GREEN$Q$FG$-$GREEN$S$FG$ are ISO file write drives.\n"
	  "$GREEN$T$FG$-$GREEN$Z$FG$ are ATAPI CD/DVD drives.\n"
	  "\nDrive Letter ($PURPLE$<ENTER>$FG$ to exit):";
    drv_let=ToUpper(GetChar);
    '\n';
    if (type=Let2BlkDevType(drv_let)) {
      one_drv=FALSE;
      unit=0;
      switch (type) {
	case BDT_RAM:
	  "Addr of RAM disk ($PURPLE$<ENTER>$FG$ to MAlloc):";
	  GetS(addr_buf,STR_LEN);
	case BDT_ISO_FILE_WRITE:
	  "Blks of 512 bytes:";
	  GetS(blks_buf,STR_LEN);
	  break;
	case BDT_ISO_FILE_READ:
	  filename=GetStr("File Name:");
	  break;
	case BDT_ATA:
	case BDT_ATAPI:
	  num_hints=ATARep(,,&head);
	  do {
	    if (num_hints)
	      "Enter dev number or\nport with $PURPLE$0x$FG$ prefix.\n"
		    "I/O Port Base0:\n";
	    else
	      "Include $PURPLE$0x$FG$ prefix.\nI/O Port Base0:\n";
	    GetS(base0_buf,STR_LEN);
	  } while (!Str2I64(base0_buf));
	  if (1<=Str2I64(base0_buf)<=num_hints) {
	    tempha=ATARepFind(head,Str2I64(base0_buf));
	    StrPrint(base0_buf,"0x%X",tempha->base0);
	    StrPrint(base1_buf,"0x%X",tempha->base1);
	    unit=tempha->unit;
	  } else if (type!=BDT_ATAPI || *base0_buf) {
	    if (type==BDT_ATAPI)
	      StrCpy(base1_buf,"0");
	    else
	      do {
		"I/O Port Base1:\n";
		GetS(base1_buf,STR_LEN);
	      } while (!Str2I64(base1_buf));
	    do {
	      "\t$PURPLE$0$FG$=Master\n\t$PURPLE$1$FG$=Slave\nUnit:";
	      unit=GetChar-'0';
	    } while (!(0<=unit<=1));
	    '\n';
	  }
	  LinkedLstDel(head);
	  break;
      }
      bd=BlkDevNextFreeSlot(drv_let,type);
      bd->unit=unit;
      switch (type) {
	case BDT_RAM:
	  if (!*addr_buf)
	    bd->RAM_dsk=0;
	  else
	    bd->RAM_dsk=Str2I64(addr_buf);
	case BDT_ISO_FILE_WRITE:
	  if (!*blks_buf)
	    bd->max_blk=-1;
	  else
	    bd->max_blk=Str2I64(blks_buf)-1;
	  break;
	case BDT_ISO_FILE_READ:
	  filename2=FileNameAbs(filename);
	  bd->file_dsk_name=AStrNew(filename2);
	  bd->drv_offset=19<<2+(DVD_BLK_SIZE*2+DVD_BOOT_LOADER_SIZE)/BLK_SIZE;
	  break;
	case BDT_ATA:
	case BDT_ATAPI:
	  bd->base0=Str2I64(base0_buf);
	  bd->base1=Str2I64(base1_buf);
	  if (type==BDT_ATA && repartition) {
	    "\nReformat WHOLE drive!";
	    one_drv=YorN;
	  }
	  break;
      }
      if ((cnt=BlkDevAdd(bd,one_drv,TRUE)) && one_drv)
	ExePrint("PrtDsk('%C');\n",drv_let);
    }
    res+=cnt;
  } while (cnt);
  Free(filename);
  Free(filename2);
  return res;
}
