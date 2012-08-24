#help_index "File/Program Routines/Utils"

U64 LTDirTreeFlattenSize(LTDirEntry *tempm)
{
  U64 result=0;
  while (tempm) {
    result+=LT_DIR_ENTRY_SIZE+1;
    if (tempm->attr & LT_ATTR_DIR)
      result+=LTDirTreeFlattenSize(tempm->sub);
    tempm=tempm->next;
  }
  return result+1;
}

U64 LTDirTreeFlattenFill(LTDirEntry *tempm,U8 *dst)
{
  U64 result=0,i;
  while (tempm) {
    *dst++=1;
    result++;
    i=LT_DIR_ENTRY_SIZE;
    MemCpy(dst,&tempm->start,i);
    dst+=i;
    result+=i;
    if (tempm->attr & LT_ATTR_DIR) {
      i=LTDirTreeFlattenFill(tempm->sub,dst);
      dst+=i;
      result+=i;
    }
    tempm=tempm->next;
  }
  *dst=0;
  return result+1;
}

public U8 *LTDirTreeFlatten(LTDirEntry *tempm,U64 *size=NULL)
{
  U64 i=LTDirTreeFlattenSize(tempm);
  U8 *buf=MAlloc(i);
  LTDirTreeFlattenFill(tempm,buf);
  if (size) *size=i;
  return buf;
}

U8 *LTDirTreeDoUnflatten(U8 *src,LTDirEntry **tempm)
{
  U64 i;
  LTDirEntry *tempm1;
  if (*src++) {
    tempm1=CAlloc(sizeof(LTDirEntry));
    *tempm=tempm1;
    i=LT_DIR_ENTRY_SIZE;
    MemCpy(&tempm1->start,src,i);
    src+=i;
    if (tempm1->attr & LT_ATTR_DIR)
      src=LTDirTreeDoUnflatten(src,&tempm1->sub);
    src=LTDirTreeDoUnflatten(src,&tempm1->next);
  } else
    *tempm=NULL;
  return src;
}

public LTDirEntry *LTDirTreeUnflatten(U8 *src)
{
  LTDirEntry *tempm=NULL;
  LTDirTreeDoUnflatten(src,&tempm);
  return tempm;
}

#help_index "Install;File/Install Related"
#help_file "::/LT/Doc/Install"

#define ROUND_PRT_TO	(63*255)
#define PRT_HEADER	63

class PlannedPrt
{
  PlannedPrt *next,*last;
  I64 size;
  BoolI8 pri;
};

//This is available to assist in writing scripts.
I64 prt_dsk_dsk_size;

public U0 PrtDsk(U8 drv_let=0)
//Partition Disk
//drive=0 means add new dev
{
  LTBlkDev *bdev;
  PlannedPrt root,*temppp;
  MasterBootStruct mbr;
  BoolI8 pri=TRUE;
  I64 ext_base,num_prt,pri_cnt=0,
     i,start_offset,offset,remaining;
"This command does not play well\n"
"with other operating systems.\n"
"You really should use another\n"
"operating system's partitioner.\n"
"If you use this, it may, in fact,\n"
"make your hard drive impossible\n"
"to repartition with other operating\n"
"until you set block zero to zero\n"
"with $LK,\"MasterBootZero\",\"MN:MasterBootZero\"$()\n\n\n"
"Continue";
  if (!YorN) return;

  if (!drv_let) {
    Mount(TRUE);
    return;
  }

  bdev=Drv2BlkDev(drv_let);
  if (!bdev || bdev->type!=LTBDT_ATA)
    return;

  prt_dsk_dsk_size=bdev->max_blk+1;

  QueInit(&root);

  num_prt=bdev->drv_let_base;
  remaining=FloorU64(bdev->max_blk+1,ROUND_PRT_TO);
  while (FloorU64(remaining,ROUND_PRT_TO)>=ROUND_PRT_TO) {
    temppp=MAlloc(sizeof(PlannedPrt));
    do {
      "$FG,RED$Partition %C$FG$\n",num_prt;
      temppp->pri=FALSE;
      if (pri) {
	"Primary Partition";
	if (YorN) {
	  pri_cnt++;
	  temppp->pri=TRUE;
	  if (pri_cnt==3)
	    pri=FALSE;
	} else
	  pri=FALSE;
      }
      "Blocks Remaining:%d (0x%X)\n",
	remaining-PRT_HEADER,remaining-PRT_HEADER;
      temppp->size=CeilU64(PmtI64("Size in Blocks  :",
	remaining-PRT_HEADER)+PRT_HEADER,ROUND_PRT_TO);
    } while (!(ROUND_PRT_TO<=temppp->size<=FloorU64(remaining,ROUND_PRT_TO)));
    QueIns(temppp,root.last);
    remaining-=temppp->size;
    num_prt++;
  }

  "\n\n!!! Repartition Drive !!!\n\n";
  temppp=root.next;
  num_prt=bdev->drv_let_base;
  while (temppp!=&root) {
    "Drive %C:%08X %s\n",num_prt,temppp->size,
      temppp->pri ? "Primary":"Logical";
    temppp=temppp->next;
    num_prt++;
  }
  if (!AreYouSure)
    goto pd_done;

  remaining=FloorU64(bdev->max_blk+1,ROUND_PRT_TO)-ROUND_PRT_TO;
  temppp=root.next;
  MemSet(&mbr,0,BLK_SIZE);
  mbr.signature=0xAA55;
  offset=0;
  for (i=0;i<pri_cnt;i++) {
    mbr.p[i].active=0x80;
    mbr.p[i].start_head=0;
    mbr.p[i].start_cyl=0x101;
    mbr.p[i].type=1; //Will get set to MBR_PT_LT
    mbr.p[i].end_head=0xFE;
    mbr.p[i].end_cyl=0xFFFF;
    mbr.p[i].offset=PRT_HEADER+offset;
    mbr.p[i].size=temppp->size-PRT_HEADER;
    offset+=temppp->size;
    remaining-=temppp->size;
    temppp=temppp->next;
  }
  if (!i) i++;
  if (temppp!=&root) {
    mbr.p[i].active=0x80;
    mbr.p[i].start_head=0;
    mbr.p[i].start_cyl=0x101;
    mbr.p[i].type=0xF;
    mbr.p[i].end_head=0xFE;
    mbr.p[i].end_cyl=0xFFFF;
    mbr.p[i].offset=offset;
    mbr.p[i].size=remaining;
    ext_base=offset;
  }
  ATAWriteBlks(bdev,&mbr,0,1);

  while (temppp!=&root) {
    start_offset=offset;
    MemSet(&mbr,0,BLK_SIZE);
    mbr.signature=0xAA55;

    mbr.p[0].active=0x80;
    mbr.p[0].start_head=1;
    mbr.p[0].start_cyl=0x101;
    mbr.p[0].type=1; //Will get set to MBR_PT_LT
    mbr.p[0].end_head=0xFE;
    mbr.p[0].end_cyl=0xFFFF;
    mbr.p[0].offset=PRT_HEADER;
    mbr.p[0].size=temppp->size-PRT_HEADER;
    offset+=temppp->size;
    temppp=temppp->next;
    if (temppp!=&root) {
      mbr.p[1].active=0x80;
      mbr.p[1].start_head=0;
      mbr.p[1].start_cyl=0x101;
      mbr.p[1].type=5;
      mbr.p[1].end_head=0xFE;
      mbr.p[1].end_cyl=0xFFFF;
      mbr.p[1].offset=offset-ext_base;
      mbr.p[1].size=temppp->size;
    }
    ATAWriteBlks(bdev,&mbr,start_offset,1);
  }

  bdev->flags&=~(LTBDF_INITIALIZED | LTBDF_INIT_IN_PROGRESS);
  BlkDevAdd(bdev,FALSE);
  for (i=bdev->drv_let_base;i<num_prt;i++)
    Fmt(i,TRUE,FALSE);

pd_done:
  while (root.next!=&root) {
    temppp=root.next;
    QueRem(temppp);
    Free(temppp);
  }
}


#help_index "Info;File/Cmd Line (Typically);Cmd Line (Typically)"

U0 LTFSDrvView(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  U64 lohi,c1,i,x,y,l,s;
  U8 *bitmap;
  GrDC *dc=GrDCAlias;

  l=(GR_HEIGHT-3*FONT_HEIGHT)*(GR_WIDTH-FONT_WIDTH<<1),
  s=(p->size+p->spc-1)/p->spc;
 

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  LtfCursor;
  LtfClear;
  GrDCFill;
  try {
    i=((s+7)>>3+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    i=0;
    for (y=0;y<GR_HEIGHT-3*FONT_HEIGHT;y++) {
      if (ScanKey)
	break;
      for (x=0;x<GR_WIDTH-FONT_WIDTH<<1;x++) {
	lohi=i*s;
	c1=lohi/l;
	if (Bt(bitmap,c1))
	  dc->color=ROP_XOR+BLUE^COLOR_TRANSPARENT;
	else
	  dc->color=ROP_XOR+WHITE^COLOR_TRANSPARENT;
	GrPlot(dc,x,y);
	i++;
      }
    }
    Free(bitmap);
  } catch
    PrtUnlock(p);
  GetChar;
 
  SettingsPop;
  GrDCFill;
  GrDCDel(dc);
}

U0 FAT32DrvView(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let);
  U64 lohi,c1,i,x,y,l,s;
  U32 *bitmap;
  GrDC *dc=GrDCAlias;
  l=(GR_HEIGHT-3*FONT_HEIGHT)*(GR_WIDTH-FONT_WIDTH<<1),
  s=(p->size+p->spc-1)/p->spc;
 

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  WinMax;
  LtfCursor;
  LtfClear;
  GrDCFill;
  try {
    i=(s*4+BLK_SIZE-1)>>BLK_SIZE_BITS;
    bitmap=MAlloc(i<<BLK_SIZE_BITS);
    RBlks(p,bitmap,p->map1,i);
    i=0;
    for (y=0;y<GR_HEIGHT-3*FONT_HEIGHT;y++) {
      if (ScanKey)
	break;
      for (x=0;x<GR_WIDTH-FONT_WIDTH<<1;x++) {
	lohi=i*s;
	c1=lohi/l;
	if (bitmap[c1])
	  dc->color=ROP_XOR+BLUE^COLOR_TRANSPARENT;
	else
	  dc->color=ROP_XOR+WHITE^COLOR_TRANSPARENT;
	GrPlot(dc,x,y);
	i++;
      }
    }
    Free(bitmap);
  } catch
    PrtUnlock(p);
  GetChar;
 
  SettingsPop;
  GrDCFill;
  GrDCDel(dc);
}

public U0 DrvView(U8 drv_let=0)
{
  LTPrt *p=Drv2Prt(drv_let),*p_old=Fs->cur_prt;
  Drv(drv_let);
  switch (p->type) {
    case PT_LT:
      LTFSDrvView(drv_let);
      break;
    case PT_FAT32:
      FAT32DrvView(drv_let);
      break;
    default:
      PutDefineErr("ST_UNSUPPORTED_FILE_SYSTEM");
  }
  Drv(Prt2Drv(p_old));
}

public U0 DskView(U8 drv_let=0)
{
  I64 i,j,
    h=Fs->win_pixel_width,
    v=Fs->win_pixel_height,
    radius;
  LTPrt *p;
  LTBlkDev *bdev=Drv2BlkDevFwded(drv_let);
  GrDC *dc=GrDCAlias;
  F64 sect_start,sect_end;

  SettingsPush; //See [C:/LT/Adam/TaskSettings.APZ,3] SettingsPush
  LtfCursor;
  LtfClear;
  GrDCFill;
  if (h<v)
    radius=0.4*h;
  else
    radius=0.4*v;
  dc->color=ROP_EQU+BLACK;
  GrCircle(dc,h>>1,v>>1,radius);

  j=1;
  for (i=0;i<NUM_LTPRTS;i++) {
    p=&local_prts[i];
    if (bdev==p->bdev && p->type) {
      sect_start=-(p->offset*2*pi/(bdev->max_blk+1));
      sect_end	=-((p->offset+p->size)*2*pi/(bdev->max_blk+1));
      dc->color=ROP_EQU+BLACK;
      GrLine(dc,h>>1,v>>1,
	h>>1+radius*Cos(sect_start),
	v>>1+radius*Sin(sect_start));
      GrLine(dc,h>>1,v>>1,
	h>>1+radius*Cos(sect_end),
	v>>1+radius*Sin(sect_end));

      dc->color=ROP_EQU+j;
      GrPrintF(dc,0,v-FONT_HEIGHT*j,"%C %Z",Prt2Drv(p),
	p->type.u16[0],"ST_PRT_TYPES");
      GrFloodFill(dc,
	h>>1+(radius-4)*Cos((sect_start+sect_end)/2),
	v>>1+(radius-4)*Sin((sect_start+sect_end)/2),FALSE);
      j++;
    }
  }

  GetChar(NULL,FALSE);
  SettingsPop;
  GrDCFill;
  GrDCDel(dc);
}

#help_index "Info;File/Cmd Line (Typically);Cmd Line (Typically);File/Install Related;Install"
public U0 BlkDevRep()
{
  I64 i;
  for (i=0;i<num_blkdevs;i++)
    ClassRep("LTBlkDev",&local_blkdevs[i]);
}

#help_index "Install;File/Install Related"
#define SC_BLK_CNT	2048
public U64 NonLTScan(I64 drv_let,U32 signature)
{
// See [::/LT/Doc/LTZ.TXZ] ::/LT/Doc/LTZ.TXZ
  U8 *buf=CAlloc(BLK_SIZE*SC_BLK_CNT),*b;
  U32 *d;
  U64 *qq;
  LTBlkDev *bdev=Drv2BlkDev(drv_let);
  LTPrt *p=Drv2Prt(drv_let);
  I64 blk=p->offset;
  U64 result=0,start=0,i,j,n,to_go,size,
  contiguous=0,new_drv_let;
  BoolI8 good,proceed=FALSE;
  try {
    while (!result) {
      '-';
      n=p->size+p->offset-blk;
      if (n>SC_BLK_CNT)
	n=SC_BLK_CNT;
      if (!n)
	break;
      RBlks(p,buf,blk,1);
      d=buf;
      qq=d;
      if (d[127]==signature) {
	b=d;
	good=TRUE;
	for (j=16;j<BLK_SIZE-4;j++)
	  if (b[j]!=j.u8[0]) {
	    good=FALSE;
	    break;
	  }
	if (good) {
	  result=blk;
	  break;
	}
      }
      blk+=n;
    }
    if (result) {
      blk=result-(SC_BLK_CNT-1);
      if (blk<0) blk=0;
      result=0;
      n=p->size+p->offset-blk;
      if (n>SC_BLK_CNT)
	n=SC_BLK_CNT;
      RBlks(p,buf,blk,n);
      for (i=0;i<n;i++) {
	d=buf+BLK_SIZE*i;
	qq=d;
	if (d[127]==signature && !qq[0]) {
	  b=d;
	  good=TRUE;
	  for (j=16;j<BLK_SIZE-4;j++)
	    if (b[j]!=j.u8[0]) {
	      good=FALSE;
	      break;
	    }
	  if (good) {
	    result=blk+i;
	    size=qq[1];
	    break;
	  }
	}
      }
    }
    if (result) {
      start=result;
      "\nStart  :0x%X Size:0x%X\n",result,size;
      to_go=size;
      blk=result;
      while (to_go && result) {
	'+';
	n=to_go;
	if (n>SC_BLK_CNT)
	  n=SC_BLK_CNT;
	RBlks(p,buf,blk,n);
	for (i=0;i<n;i++) {
	  d=buf+BLK_SIZE*i;
	  qq=d;
	  if (d[127]==signature && qq[0]==blk-result+i &&
	  qq[1]==size) {
	    b=d;
	    for (j=16;j<BLK_SIZE-4;j++)
	      if (b[j]!=j.u8[0]) {
		result=0;
		break;
	      }
	    if (result)
	      contiguous++;
	    else
	      break;
	  } else {
	    result=0;
	    break;
	  }
	}
	to_go-=n;
	blk+=n;
      }
    }
    "Start: 0x%X\n",result;
    if (result) {
      "\nSuccess\n";
      proceed=TRUE;
    } else {
      "\nFailed, but found 0x%X contiguous.\n",contiguous;
      if (contiguous) {
	"Proceed Anyway ";
	proceed=YorN;
      }
    }
  } catch
    Fs->catch_except=TRUE;
 
  Cd("HOME");
  Free(buf);
 
  if (proceed) {
    "New Drive Letter:";
    new_drv_let=ToUpper(GetChar);
    NewLine;
    bdev=&local_blkdevs[num_blkdevs++];
    bdev->drv_let_base=new_drv_let;
    bdev->blk_size=BLK_SIZE;
    bdev->min_blk=start;
    bdev->max_blk=start+contiguous-1;
    bdev->non_lt_drv=drv_let;
    bdev->non_lt_offset=start;
    bdev->type=LTBDT_NON_LT_FILE;
    bdev->flags=LTBDF_FMT;
    if (!BlkDevAdd(bdev,FALSE))
      PutDefine("ST_FAILED");
    Drv(new_drv_let);
    "File Blk: 0x%X\n",start;
    result=start;
  } else
    result=0;
  return result;
}
#help_index ""
