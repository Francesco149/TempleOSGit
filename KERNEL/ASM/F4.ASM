        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_FAT32_CPP
FIL_FAT32_CPP:

        DB '#define BLOCK_SIZE 512',13,10,'DWORD drive_hea'
        DB 'ds=16;',13,10,'DWORD drive_sectors=0x3F;',13,10
        DB 'DWORD MAX_SECTOR=drive_heads*drive_sectors*0x100'
        DB '00;',13,10,'DWORD MAX_READS_AT_ONCE=0x80;',13
        DB 10,'DWORD MAX_WRITES_AT_ONCE=1;',13,10,13,10
        DB '//The following definitions are for low-level di'
        DB 'sk access',13,10,'//If you are interested in m'
        DB 'aking CD or Floppy access they',13,10,'//might'
        DB ' be handy.  Also,to access more than 32GB they m'
        DB 'ay 0be necessary',13,10,'/* ATA/ATAPI Commands'
        DB ' pre T13 Spec */',13,10,'#define WIN_NOP',9,9
        DB 9,9,'0x00',13,10,'#define CFA_REQ_EXT_ERROR'
        DB '_CODE',9,9,'0x03 /* CFA Request Extended Err'
        DB 'or Code */',13,10,'#define WIN_SRST',9,9,9
        DB '0x08 /* ATAPI soft reset command */',13,10,'#d'
        DB 'efine WIN_DEVICE_RESET',9,9,'0x08',13,10,'#'
        DB 'define WIN_RESTORE',9,9,9,'0x10',13,10,'#'
        DB 'define WIN_READ',9,9,9,'0x20 /* 28-Bit */',13
        DB 10,'#define WIN_READ_EXT',9,9,9,'0x24 /* 4'
        DB '8-Bit */',13,10,'#define WIN_READDMA_EXT',9,9
        DB 9,'0x25 /* 48-Bit */',13,10,'#define WIN_READ'
        DB 'DMA_QUEUED_EXT',9,9,'0x26 /* 48-Bit */',13,10
        DB '#define WIN_READ_NATIVE_MAX_EXT',9,9,'0x27 /'
        DB '* 48-Bit */',13,10,'#define WIN_MULTREAD_EXT',9
        DB 9,'0x29 /* 48-Bit */',13,10,'#define WIN_WRIT'
        DB 'E',9,9,9,'0x30 /* 28-Bit */',13,10,'#def'
        DB 'ine WIN_WRITE_EXT',9,9,9,'0x34 /* 48-Bit *'
        DB '/',13,10,'#define WIN_WRITEDMA_EXT',9,9,'0'
        DB 'x35 /* 48-Bit */',13,10,'#define WIN_WRITEDMA_'
        DB 'QUEUED_EXT',9,9,'0x36 /* 48-Bit */',13,10,'#'
        DB 'define WIN_SET_MAX_EXT',9,9,9,'0x37 /* 48-'
        DB 'Bit */',13,10,'#define CFA_WRITE_SECT_WO_ERASE'
        DB 9,9,'0x38 /* CFA Write Sectors without erase '
        DB '*/',13,10,'#define WIN_MULTWRITE_EXT',9,9,'0'
        DB 'x39 /* 48-Bit */',13,10,'#define WIN_WRITE_VER'
        DB 'IFY',9,9,'0x3C /* 28-Bit */',13,10,'#defin'
        DB 'e WIN_VERIFY',9,9,9,'0x40 /* 28-Bit - Read'
        DB ' Verify Sectors */',13,10,'#define WIN_VERIFY_'
        DB 'EXT',9,9,9,'0x42 /* 48-Bit */',13,10,'#d'
        DB 'efine WIN_FORMAT',9,9,9,'0x50',13,10,'#d'
        DB 'efine WIN_INIT',9,9,9,'0x60',13,10,'#def'
        DB 'ine WIN_SEEK',9,9,9,'0x70',13,10,'#defin'
        DB 'e CFA_TRANSLATE_SECTOR',9,9,'0x87 /* CFA Tra'
        DB 'nslate Sector */',13,10,'#define WIN_DIAGNOSE',9
        DB 9,9,'0x90',13,10,'#define WIN_SPECIFY',9,9
        DB 9,'0x91 /* set drive geometry translation */',13
        DB 10,'#define WIN_DOWNLOAD_MICROCODE',9,9,'0x9'
        DB '2',13,10,'#define WIN_STANDBYNOW2',9,9,9
        DB '0x94',13,10,'#define WIN_SETIDLE2',9,9,9
        DB '0x97',13,10,'#define WIN_CHECKPOWERMODE2',9,9
        DB '0x98',13,10,'#define WIN_SLEEPNOW2',9,9,9
        DB '0x99',13,10,'#define WIN_PACKETCMD',9,9,9
        DB '0xA0 /* Send a packet command. */',13,10,'#def'
        DB 'ine WIN_PIDENTIFY',9,9,9,'0xA1 /* identify'
        DB ' ATAPI device',9,'*/',13,10,'#define WIN_QUE'
        DB 'UED_SERVICE',9,9,'0xA2',13,10,'#define WIN'
        DB '_SMART',9,9,9,'0xB0 /* self-monitoring and'
        DB ' reporting */',13,10,'#define CFA_ERASE_SECTOR'
        DB 'S       ',9,'0xC0',13,10,'#define WIN_MULTRE'
        DB 'AD',9,9,9,'0xC4 /* read sectors using mult'
        DB 'iple mode*/',13,10,'#define WIN_MULTWRITE',9
        DB 9,9,'0xC5 /* write sectors using multiple mod'
        DB 'e */',13,10,'#define WIN_SETMULT',9,9,9,'0'
        DB 'xC6 /* enable/disable multiple mode */',13,10,'#'
        DB 'define WIN_READDMA_QUEUED',9,9,'0xC7 /* read'
        DB ' sectors using Queued DMA transfers */',13,10,'#'
        DB 'define WIN_READDMA',9,9,9,'0xC8 /* read se'
        DB 'ctors using DMA transfers */',13,10,'#define W'
        DB 'IN_WRITEDMA',9,9,9,'0xCA /* write sectors '
        DB 'using DMA transfers */',13,10,'#define WIN_WRI'
        DB 'TEDMA_QUEUED',9,9,'0xCC /* write sectors usi'
        DB 'ng Queued DMA transfers */',13,10,'#define CFA'
        DB '_WRITE_MULTI_WO_ERASE',9,'0xCD /* CFA Write mu'
        DB 'ltiple without erase */',13,10,'#define WIN_GE'
        DB 'TMEDIASTATUS',9,9,'0xDA',9,13,10,'#defin'
        DB 'e WIN_DOORLOCK',9,9,9,'0xDE /* lock door o'
        DB 'n removable drives */',13,10,'#define WIN_DOOR'
        DB 'UNLOCK',9,9,9,'0xDF /* unlock door on remo'
        DB 'vable drives */',13,10,'#define WIN_STANDBYNOW'
        DB '1',9,9,9,'0xE0',13,10,'#define WIN_IDLEI'
        DB 'MMEDIATE',9,9,'0xE1 /* force drive to become'
        DB ' "ready" */',13,10,'#define WIN_STANDBY       '
        DB '      ',9,'0xE2 /* Set device in Standby Mode '
        DB '*/',13,10,'#define WIN_SETIDLE1',9,9,9,'0'
        DB 'xE3',13,10,'#define WIN_READ_BUFFER',9,9,9
        DB '0xE4 /* force read only 1 sector */',13,10,'#d'
        DB 'efine WIN_CHECKPOWERMODE1',9,9,'0xE5',13,10
        DB '#define WIN_SLEEPNOW1',9,9,9,'0xE6',13,10
        DB '#define WIN_FLUSH_CACHE',9,9,9,'0xE7',13,10
        DB '#define WIN_WRITE_BUFFER',9,9,'0xE8 /* force'
        DB ' write only 1 sector */',13,10,'#define WIN_FL'
        DB 'USH_CACHE_EXT',9,9,'0xEA /* 48-Bit */',13,10
        DB '#define WIN_IDENTIFY',9,9,9,'0xEC /* ask d'
        DB 'rive to identify itself',9,'*/',13,10,'#defi'
        DB 'ne WIN_MEDIAEJECT',9,9,9,'0xED',13,10,'#'
        DB 'define WIN_IDENTIFY_DMA',9,9,'0xEE /* same a'
        DB 's WIN_IDENTIFY, but DMA */',13,10,'#define WIN'
        DB '_SETFEATURES',9,9,9,'0xEF /* set special d'
        DB 'rive features */',13,10,'#define EXABYTE_ENABL'
        DB 'E_NEST',9,9,'0xF0',13,10,'#define WIN_SECU'
        DB 'RITY_SET_PASS',9,9,'0xF1',13,10,'#define W'
        DB 'IN_SECURITY_UNLOCK',9,9,'0xF2',13,10,'#def'
        DB 'ine WIN_SECURITY_ERASE_PREPARE',9,'0xF3',13,10
        DB '#define WIN_SECURITY_ERASE_UNIT',9,9,'0xF4',13
        DB 10,'#define WIN_SECURITY_FREEZE_LOCK',9,'0xF5',13
        DB 10,'#define WIN_SECURITY_DISABLE',9,9,'0xF6',13
        DB 10,'#define WIN_READ_NATIVE_MAX',9,9,'0xF8 /'
        DB '* return the native maximum address */',13,10,'#'
        DB 'define WIN_SET_MAX',9,9,9,'0xF9',13,10,'#'
        DB 'define DISABLE_SEAGATE',9,9,9,'0xFB',13,10
        DB '#define WIN_MULTREAD',9,9,9,'0xC4 /* read '
        DB 'sectors using multiple mode*/',13,10,13,10,'/'
        DB '* Bits of HD_STATUS */',13,10,'#define ERR_STA'
        DB 'T',9,9,'0x01',13,10,'#define INDEX_STAT',9
        DB 9,'0x02',13,10,'#define ECC_STAT',9,9,'0x'
        DB '04',9,'/* Corrected error */',13,10,'#define'
        DB ' DRQ_STAT',9,9,'0x08',13,10,'#define SEEK_'
        DB 'STAT',9,9,'0x10',13,10,'#define WRERR_STAT'
        DB 9,9,'0x20',13,10,'#define READY_STAT',9,9
        DB '0x40',13,10,'#define BUSY_STAT',9,9,'0x80',13
        DB 10,13,10,'#define BAD_R_STAT',9,9,'(BUSY_S'
        DB 'TAT   | ERR_STAT)',13,10,'#define BAD_W_STAT',9
        DB 9,'(BAD_R_STAT  | WRERR_STAT)',13,10,'#define'
        DB ' BAD_STAT',9,9,'(BAD_R_STAT  | DRQ_STAT)',13
        DB 10,'#define DRIVE_READY',9,9,'(READY_STAT  |'
        DB ' SEEK_STAT)',13,10,'#define DATA_READY',9,9
        DB '(DRQ_STAT)',13,10,13,10,'#define IDE_DATA_RE'
        DB 'G    0x1f0',13,10,'#define IDE_FEATURE_REG 0x1'
        DB 'f1',13,10,'#define IDE_NSECTOR_REG 0x1f2',13,10
        DB '#define IDE_SECTOR_REG  0x1f3',13,10,'#define '
        DB 'IDE_LCYL_REG    0x1f4',13,10,'#define IDE_HCYL'
        DB '_REG    0x1f5',13,10,'#define IDE_SELECT_REG  '
        DB '0x1f6',13,10,'#define IDE_STATUS      0x1f7',13
        DB 10,'#define IDE_COMMAND     0x1f7',13,10,'#def'
        DB 'ine IDE_CONTROL_REG 0x3f6',13,10,13,10,13,10
        DB 'void irq0E_handler()',13,10,'{',13,10,'  bts'
        DB '(SYS_FLAGS,SYSF_IDE1_DONE);',13,10,'  outp(0xa'
        DB '0,0x20);',13,10,'  outp(0x20,0x20);',13,10,'}'
        DB 13,10,13,10,'void ide_select_block(DWORD unit'
        DB ',DWORD block,DWORD count)',13,10,'{',13,10,' '
        DB ' DWORD sect,head,cyl,track;',13,10,'  outp(IDE'
        DB '_CONTROL_REG,0x8);',13,10,'  track=block / dri'
        DB 've_sectors;',13,10,'  sect =block % drive_sect'
        DB 'ors+1;',13,10,'  outp(IDE_SECTOR_REG,sect);',13
        DB 10,'  head =track % drive_heads;',13,10,'  cyl'
        DB '  =track / drive_heads;',13,10,'  outp(IDE_FEA'
        DB 'TURE_REG,0);',13,10,'  outp(IDE_NSECTOR_REG,co'
        DB 'unt);',13,10,'  outp(IDE_LCYL_REG,cyl);',13,10
        DB '  outp(IDE_HCYL_REG,cyl>>8);',13,10,'  outp(ID'
        DB 'E_SELECT_REG,0xa0|(unit<<4)|head);',13,10,'}',13
        DB 10,13,10,'void ide_wait_for_drive()',13,10,'{'
        DB 13,10,'  while (bts(SYS_FLAGS,SYSF_IDE1_BUSY))',13
        DB 10,'    swap_in_next;',13,10,13,10,'  btr(SY'
        DB 'S_FLAGS,SYSF_IDE1_DONE);',13,10,'}',13,10,13
        DB 10,13,10,'void ide_read_blocks2(BYTE *buf; DWO'
        DB 'RD count)',13,10,'{',13,10,'  DWORD i,words_'
        DB 'avail,sectors_avail;',13,10,'  while (!btr(SYS'
        DB '_FLAGS,SYSF_IDE1_DONE))',13,10,'    swap_in_ne'
        DB 'xt;',13,10,'  while (count) {',13,10,'    wh'
        DB 'ile (inp(IDE_STATUS) & DATA_READY!=DATA_READY)',13
        DB 10,'      swap_in_next;',13,10,'    while (!(s'
        DB 'ectors_avail=count-inp(IDE_NSECTOR_REG)))',13,10
        DB '      swap_in_next;',13,10,'    words_avail=se'
        DB 'ctors_avail*BLOCK_SIZE/2;',13,10,'    rep_insw'
        DB '(buf,words_avail,IDE_DATA_REG);',13,10,'    bu'
        DB 'f=buf+words_avail*2;',13,10,'    count=count-s'
        DB 'ectors_avail;',13,10,'  }',13,10,13,10,'  '
        DB 'btr(SYS_FLAGS,SYSF_IDE1_BUSY);',13,10,'}',13,10
        DB 13,10,'void ide_read_blocks(BYTE *buf; DWORD bl'
        DB 'ock; DWORD count)',13,10,'{',13,10,'  DWORD '
        DB 'i,words_avail,sectors_avail;',13,10,'  ide_wai'
        DB 't_for_drive();',13,10,'  ide_select_block(0,bl'
        DB 'ock,count);',13,10,'  outp(IDE_COMMAND,WIN_MUL'
        DB 'TREAD);',13,10,'  ide_read_blocks2(buf,count);'
        DB 13,10,'}',13,10,13,10,'void rblocks(BYTE *b'
        DB 'uf, DWORD block, int count)',13,10,'{',13,10
        DB '  DWORD l;',13,10,'  while (count>0) {',13,10
        DB '    l=count;',13,10,'    if (l>MAX_READS_AT_ON'
        DB 'CE)',13,10,'      l=MAX_READS_AT_ONCE;',13,10
        DB '    ide_read_blocks(buf,block,l);',13,10,'    '
        DB 'buf=buf+l*BLOCK_SIZE;',13,10,'    block=block+'
        DB 'l;',13,10,'    count=count-l;',13,10,'  }',13
        DB 10,'}',13,10,13,10,13,10,'class mbr_partit'
        DB 'ion_entry',13,10,'{',13,10,'  BYTE active;  '
        DB '//0x80=active  0x00=inactive',13,10,'  BYTE st'
        DB 'art_head;',13,10,'  WORD start_cyl;',13,10,' '
        DB ' BYTE type;',13,10,'  BYTE end_head;',13,10,' '
        DB ' WORD end_cyl;',13,10,'  DWORD offset;  //Sect'
        DB 'ors between MBR and first sector',13,10,'  DWO'
        DB 'RD size;    //Sectors in partition',13,10,'};',13
        DB 10,13,10,'class master_boot_record',13,10,'{'
        DB 13,10,'  BYTE boot_code[446];',13,10,'  mbr_p'
        DB 'artition_entry p[4];',13,10,'  WORD signature;'
        DB '  //55AA',13,10,'};',13,10,13,10,'class FA'
        DB 'T32_boot_record',13,10,'{',13,10,'  BYTE jum'
        DB 'p_and_nop[3];',13,10,'  BYTE OEM_name[8];',13
        DB 10,'  WORD bytes_per_sector;',13,10,'  BYTE se'
        DB 'ctors_per_cluster;',13,10,'  WORD reserved_sec'
        DB 'tors;',13,10,'  BYTE copies_of_FAT;',13,10,' '
        DB ' WORD max_root_dir_entries;      //Not used',13
        DB 10,'  WORD old_sectors_in_partition;  //Not used'
        DB 13,10,'  BYTE media_descriptor;          //F8 f'
        DB 'or hard disk',13,10,'  WORD old_sectors_per_FA'
        DB 'T;       //Not used',13,10,'  WORD sectors_per'
        DB '_track;',13,10,'  WORD num_heads;',13,10,'  '
        DB 'DWORD hidden_sectors;',13,10,'  DWORD sectors;'
        DB 13,10,'  DWORD sectors_per_FAT;',13,10,'  WOR'
        DB 'D flags;',13,10,'  WORD version;',13,10,'  D'
        DB 'WORD root_cluster;',13,10,'  WORD file_system_'
        DB 'info_sector;',13,10,'  WORD backup_boot_sector'
        DB ';',13,10,'  BYTE reserved[12];',13,10,'  BYT'
        DB 'E logical_drive_num;',13,10,'  BYTE unused;',13
        DB 10,'  BYTE extended_signature; //0x29',13,10,' '
        DB ' DWORD serial_number;',13,10,'  char volume_na'
        DB 'me[11];',13,10,'  BYTE FAT_name[8];',13,10,' '
        DB ' BYTE code[420];',13,10,'  WORD signature; //0'
        DB 'xAA55',13,10,'};',13,10,13,10,'class FAT32'
        DB '_file_info_sector',13,10,'{',13,10,'  DWORD '
        DB 'signature1; //52 52 61 41',13,10,'  BYTE  unkn'
        DB 'own[480];',13,10,'  DWORD signature2; //72 72 '
        DB '41 61',13,10,'  DWORD free_clusters; // -1 if '
        DB 'unknown',13,10,'  DWORD most_recently_allocate'
        DB 'd;',13,10,'  BYTE  reserved[12];',13,10,'  W'
        DB 'ORD  unknown2;',13,10,'  WORD  signature3; //5'
        DB '5 AA',13,10,'}',13,10,13,10,13,10,'#defi'
        DB 'ne mpef_locked 0',13,10,'class my_partition_en'
        DB 'try',13,10,'{',13,10,'  DWORD offset;',13,10
        DB '  DWORD size;',13,10,'  DWORD type;',13,10,' '
        DB ' DWORD file_system_info_sector;',13,10,'  DWOR'
        DB 'D FAT1;',13,10,'  DWORD FAT2;',13,10,'  DWOR'
        DB 'D data;',13,10,'  DWORD flags;',13,10,'  DWO'
        DB 'RD owning_task;',13,10,'  DWORD cur_FAT32_blk_'
        DB 'num;',13,10,'  DWORD *cur_FAT32_blk;',13,10,' '
        DB ' FAT32_boot_record *cur_FAT32_br;',13,10,'  FA'
        DB 'T32_file_info_sector *cur_FAT32_fis;',13,10,13
        DB 10,'};',13,10,13,10,'DWORD num_partitions=0;'
        DB 13,10,'my_partition_entry local_partitions[32];'
        DB 13,10,13,10,'void read_partition_info() //Thi'
        DB 's is incomplete',13,10,'{',13,10,'  DWORD i,'
        DB 'j,offset=0;',13,10,'  bool primary=true;',13,10
        DB '  master_boot_record mbr;',13,10,'  my_partiti'
        DB 'on_entry *p;',13,10,'  FAT32_boot_record br;',13
        DB 10,13,10,'  num_partitions=0;',13,10,'  whil'
        DB 'e (true) {',13,10,'    rblocks(&mbr,offset,1);'
        DB 13,10,'    if (mbr.signature!=0xAA55)',13,10,' '
        DB '     return;',13,10,'    j=4;',13,10,'    fo'
        DB 'r (i=0;i<4;i++) {',13,10,'      if (mbr.p[i].a'
        DB 'ctive==0x80 || (!primary && i==0)) {',13,10,' '
        DB '       p=&local_partitions[num_partitions];',13
        DB 10,'        p->offset=mbr.p[i].offset+offset;',13
        DB 10,'        p->size  =mbr.p[i].size;',13,10,' '
        DB '       p->type  =mbr.p[i].type;',13,10,'      '
        DB '  p->flags = 0;',13,10,'        p->owning_task'
        DB '=0;',13,10,'        p->cur_FAT32_br=NULL;',13
        DB 10,'        p->cur_FAT32_fis=NULL;',13,10,'   '
        DB '     p->cur_FAT32_blk_num=0xFFFFFFFF;',13,10,' '
        DB '       ? "Part:",num_partitions,',13,10,'     '
        DB '     " Offset:",p->offset,',13,10,'          "'
        DB ' Size:",p->size,',13,10,'          " Type:",p-'
        DB '>type,"\r\n";',13,10,'        if (p->offset>=M'
        DB 'AX_SECTOR)',13,10,'          return;',13,10,' '
        DB '       if (p->type==0x0B) {',13,10,'          '
        DB 'rblocks(&br,p->offset,1);',13,10,'          p-'
        DB '>file_system_info_sector=p->offset+br.file_syste'
        DB 'm_info_sector;',13,10,'          p->FAT1=p->of'
        DB 'fset+br.reserved_sectors;',13,10,'          p-'
        DB '>FAT2=p->FAT1+br.sectors_per_FAT;',13,10,'    '
        DB '      p->data=p->FAT2+br.sectors_per_FAT-2*br.se'
        DB 'ctors_per_cluster; //Starts at cluster 2',13,10
        DB '          p->cur_FAT32_br=malloc(sizeof(FAT32_bo'
        DB 'ot_record));',13,10,'          rblocks(p->cur_'
        DB 'FAT32_br,p->offset,1);',13,10,'          p->cu'
        DB 'r_FAT32_fis=malloc(sizeof(FAT32_file_info_sector'
        DB '));',13,10,'          rblocks(p->cur_FAT32_fis'
        DB ',p->file_system_info_sector,1);',13,10,'      '
        DB '    p->cur_FAT32_blk=malloc(BLOCK_SIZE);',13,10
        DB '        }',13,10,'        num_partitions++;',13
        DB 10,'        j=i+1;',13,10,'      }',13,10,' '
        DB '   }',13,10,'    primary=false;',13,10,'    '
        DB 'if (j>=4)',13,10,'      return;',13,10,'    '
        DB 'if (mbr.p[j].offset==0)',13,10,'      return;',13
        DB 10,'    offset=mbr.p[j].offset+offset;',13,10,' '
        DB '   if (offset>=MAX_SECTOR)',13,10,'      retur'
        DB 'n;',13,10,'  }',13,10,'}',13,10,13,10,'v'
        DB 'oid select_default_drive()',13,10,'{',13,10,' '
        DB ' int i;',13,10,'  my_partition_entry *p;',13,10
        DB '  for (i=0;i<num_partitions;i++) {',13,10,'   '
        DB ' p=&local_partitions[i];',13,10,'    if (p->ty'
        DB 'pe==0x0B) {',13,10,'      fs->current_partitio'
        DB 'n=i;',13,10,'      fs->current_directory=new_s'
        DB 'tring("/");',13,10,'      fs->current_dir_clus'
        DB 'ter=p->cur_FAT32_br->root_cluster;',13,10,'   '
        DB '   return;',13,10,'    }',13,10,'  }',13,10
        DB '  ? "FATAL ERROR: No FAT32 partition found\r\n";'
        DB 13,10,'}',13,10,13,10,'void lock_drive(int '
        DB 'partition)',13,10,'{',13,10,'  my_partition_'
        DB 'entry *p=&local_partitions[partition];',13,10,' '
        DB ' if (p->owning_task!=fs->absolute_address && bts'
        DB '(&p->flags,mpef_locked))',13,10,'    swap_in_n'
        DB 'ext;',13,10,'  p->owning_task=fs->absolute_add'
        DB 'ress;',13,10,'}',13,10,13,10,'void unlock_'
        DB 'drive(int partition)',13,10,'{',13,10,'  my_'
        DB 'partition_entry *p=&local_partitions[partition];'
        DB 13,10,'  p->owning_task=0;',13,10,'  btr(&p->'
        DB 'flags,mpef_locked);',13,10,'}',13,10,13,10
        DB 'DWORD next_FAT32_cluster_num(DWORD c)',13,10,'{'
        DB 13,10,'  my_partition_entry *p=&local_partition'
        DB 's[fs->current_partition];',13,10,'  DWORD FAT_'
        DB 'blk_num;',13,10,'  FAT_blk_num=c/(BLOCK_SIZE/4'
        DB ');',13,10,'  if (p->cur_FAT32_blk_num!=FAT_blk'
        DB '_num) {',13,10,'    rblocks(p->cur_FAT32_blk,p'
        DB '->FAT1+FAT_blk_num,1);',13,10,'    p->cur_FAT3'
        DB '2_blk_num=FAT_blk_num;',13,10,'  }',13,10,' '
        DB ' return p->cur_FAT32_blk[c-FAT_blk_num*(BLOCK_SI'
        DB 'ZE/4)];',13,10,'}',13,10,13,10,13,10,'vo'
        DB 'id rcluster(BYTE *buf,DWORD c)',13,10,'{',13,10
        DB '  my_partition_entry *p=&local_partitions[fs->cu'
        DB 'rrent_partition];',13,10,'  rblocks(buf,p->dat'
        DB 'a+c*p->cur_FAT32_br->sectors_per_cluster,',13,10
        DB '          p->cur_FAT32_br->sectors_per_cluster);'
        DB 13,10,'}',13,10,13,10,'class FAT32_dir_entr'
        DB 'y',13,10,'{',13,10,'  char name[11];',13,10
        DB '  BYTE attr;',13,10,'  BYTE NTres;',13,10,' '
        DB ' BYTE CrtTimeTenth;',13,10,'  WORD CrtTime;',13
        DB 10,'  WORD CrtDate;',13,10,'  WORD LstAccDate;'
        DB 13,10,'  WORD cluster_hi;',13,10,'  WORD WrtT'
        DB 'ime;',13,10,'  WORD WRtDate;',13,10,'  WORD '
        DB 'cluster_lo;',13,10,'  DWORD size;',13,10,'}',13
        DB 10,13,10,'#define FAT32_attr_read_only 1',13,10
        DB '#define FAT32_attr_hidden    2',13,10,'#define'
        DB ' FAT32_attr_system    4',13,10,'#define FAT32_'
        DB 'attr_volume_id 8',13,10,'#define FAT32_attr_di'
        DB 'rectory 0x10',13,10,'#define FAT32_attr_archiv'
        DB 'e   0x20',13,10,'#define FAT32_attr_long_name '
        DB '0x0F',13,10,13,10,13,10,'void to_FAT32_nam'
        DB 'e(char *src,char *dst)',13,10,'{',13,10,'  D'
        DB 'WORD i;',13,10,'  char *ptr;',13,10,'  if (!'
        DB 'strcmp(src,"..")) {',13,10,'    strcpy(dst,src'
        DB ');',13,10,'    i=2;',13,10,'  } else if (!st'
        DB 'rcmp(src,".")) {',13,10,'    strcpy(dst,src);',13
        DB 10,'    i=1;',13,10,'  } else {',13,10,'    '
        DB 'ptr=src;',13,10,'    i=0;',13,10,'    while '
        DB '(i<8 && *ptr && *ptr!=',39,'.',39,') {',13,10
        DB '      dst[i++]=*ptr++;',13,10,'    }',13,10,' '
        DB '   while (i<8) ',13,10,'      dst[i++]=0x20;',13
        DB 10,'    if (*ptr==',39,'.',39,') ptr++;',13,10
        DB '    while (i<11 && *ptr) {',13,10,'      dst[i'
        DB '++]=*ptr++;',13,10,'    }',13,10,'  }',13,10
        DB '  while (i<11) ',13,10,'    dst[i++]=0x20;',13
        DB 10,'}',13,10,13,10,'void from_FAT32_name(cha'
        DB 'r *src,char *dst)',13,10,'{',13,10,'  int i,'
        DB 'j,k;',13,10,'  for (j=7;j>=0 && src[j]==0x20;j'
        DB '--);',13,10,'  k=0;',13,10,'  for(i=0;i<=j;i'
        DB '++)',13,10,'    dst[k++]=src[i];',13,10,'  i'
        DB 'f (*src!=',39,'.',39,')',13,10,'    dst[k++]'
        DB '=',39,'.',39,';',13,10,'  for (j=10;j>=8 && '
        DB 'src[j]==0x20;j--);',13,10,'  for(i=8;i<=j;i++)'
        DB 13,10,'    dst[k++]=src[i];',13,10,'  dst[k++'
        DB ']=0;',13,10,'}',13,10,13,10,'bool find_FAT'
        DB '32_file(char *name,FAT32_dir_entry *result,bool '
        DB 'dir_only,bool file_only,bool unlock=true)',13,10
        DB '{',13,10,'  my_partition_entry *p=&local_parti'
        DB 'tions[fs->current_partition];',13,10,'  FAT32_'
        DB 'dir_entry *buf;',13,10,'  DWORD cur_dir_cluste'
        DB 'r,cur_dir_entry,entries_per_cluster;',13,10,' '
        DB ' bool cont=true,cont2=false;',13,10,'  char *p'
        DB 'tr,*ptr2,dname[11];',13,10,'  DWORD i;',13,10
        DB '  BYTE ch;',13,10,13,10,'  lock_drive(fs->cu'
        DB 'rrent_partition);',13,10,'  if (p->type!=0x0B)'
        DB ' ',13,10,'    ? "Not a FAT32 Partition\r\n";',13
        DB 10,'  else {',13,10,'    to_FAT32_name(name,dn'
        DB 'ame);',13,10,'    buf=malloc(BLOCK_SIZE*p->cur'
        DB '_FAT32_br->sectors_per_cluster);',13,10,'    e'
        DB 'ntries_per_cluster=BLOCK_SIZE*p->cur_FAT32_br->s'
        DB 'ectors_per_cluster',13,10,'        /sizeof(FAT'
        DB '32_dir_entry);',13,10,'    cur_dir_cluster=fs-'
        DB '>current_dir_cluster;',13,10,'    rcluster(buf'
        DB ',cur_dir_cluster);',13,10,'    cur_dir_entry=0'
        DB ';',13,10,'    while (cont) {',13,10,'      i'
        DB 'f (cur_dir_entry==entries_per_cluster) {',13,10
        DB '        cur_dir_cluster=next_FAT32_cluster_num(c'
        DB 'ur_dir_cluster);',13,10,'        if (cur_dir_c'
        DB 'luster==0xFFFFFFF)',13,10,'          cont=fals'
        DB 'e;',13,10,'        else {',13,10,'          '
        DB 'rcluster(buf,cur_dir_cluster);',13,10,'       '
        DB '   cur_dir_entry=0;',13,10,'        }',13,10
        DB '      }',13,10,'      if (cont) {',13,10,'  '
        DB '      ch=buf[cur_dir_entry].name[0];',13,10,' '
        DB '       if (!ch) {',13,10,'          cont=false'
        DB ';',13,10,'        } else if (ch!=0xE5) {',13,10
        DB '          if (dir_only && !(buf[cur_dir_entry].a'
        DB 'ttr & FAT32_attr_directory))',13,10,'         '
        DB '   cont2=false;',13,10,'          else if (fil'
        DB 'e_only && (buf[cur_dir_entry].attr & FAT32_attr_'
        DB 'directory))',13,10,'            cont2=false;',13
        DB 10,'          else',13,10,'            cont2=t'
        DB 'rue;',13,10,'          ptr=dname;',13,10,'  '
        DB '        ptr2=buf[cur_dir_entry].name;',13,10,' '
        DB '         for (i=0;i<11 && cont2;i++)',13,10,' '
        DB '           if (*ptr++!=*ptr2++)',13,10,'      '
        DB '        cont2=false;',13,10,'          if (con'
        DB 't2) {',13,10,'            memcpy(result,&buf[c'
        DB 'ur_dir_entry],sizeof(FAT32_dir_entry));',13,10
        DB '            if (unlock)',13,10,'              '
        DB 'unlock_drive(fs->current_partition);',13,10,' '
        DB '           return true;',13,10,'          }',13
        DB 10,'        }',13,10,'      }',13,10,'      '
        DB 'cur_dir_entry++;',13,10,'    }',13,10,'    f'
        DB 'ree(buf);',13,10,'  }  ',13,10,'  if (unlock'
        DB ')',13,10,'    unlock_drive(fs->current_partiti'
        DB 'on);',13,10,'  return false;',13,10,'}',13,10
        DB 13,10,13,10,'BYTE *read_FAT32_file(char *file'
        DB 'name,DWORD *size=NULL,bool unlock=true)',13,10
        DB '{',13,10,'  BYTE *buf=NULL,*ptr;',13,10,'  F'
        DB 'AT32_dir_entry de;',13,10,'  my_partition_entr'
        DB 'y *p=&local_partitions[fs->current_partition];',13
        DB 10,'  DWORD c,cluster_size=BLOCK_SIZE*p->cur_FAT'
        DB '32_br->sectors_per_cluster,num_clusters;',13,10
        DB 13,10,'  if (size)',13,10,'    *size=0;',13,10
        DB '  lock_drive(fs->current_partition);',13,10,' '
        DB ' if (!find_FAT32_file(filename,&de,false,true,fa'
        DB 'lse))',13,10,'    ? "ERROR: File not found\r\n'
        DB '";',13,10,'  else {',13,10,'    num_clusters'
        DB '=(de.size+1+cluster_size-1)/cluster_size;',13,10
        DB '    buf=malloc(num_clusters*cluster_size);',13,10
        DB '    c=de.cluster_lo+(de.cluster_hi<<16);',13,10
        DB '    ptr=buf;',13,10,'    while (c!=0x0FFFFFFF)'
        DB ' {',13,10,'      rcluster(ptr,c);',13,10,'  '
        DB '    ptr=ptr+cluster_size;',13,10,'      c=next'
        DB '_FAT32_cluster_num(c);',13,10,'    }',13,10,' '
        DB '   buf[de.size]=0; //Terminate',13,10,'    if '
        DB '(size)',13,10,'      *size=de.size;',13,10,' '
        DB ' }',13,10,'  if (unlock)',13,10,'    unlock_'
        DB 'drive(fs->current_partition);',13,10,'  return'
        DB ' buf;',13,10,'}',13,10,13,10,'void cd_FAT3'
        DB '2(char *name)',13,10,'{',13,10,'  FAT32_dir_'
        DB 'entry de;',13,10,'  if (find_FAT32_file(name,&'
        DB 'de,true,false)) {',13,10,'    fs->current_dir_'
        DB 'cluster=de.cluster_lo+(de.cluster_hi << 16);',13
        DB 10,'  } else',13,10,'    ? "ERROR: File not fo'
        DB 'und\r\n";',13,10,'}',13,10,'//This is for lo'
        DB 'w level disk access',13,10,'//Call wblocks() i'
        DB 'nstead',13,10,'void ide_write_blocks(BYTE *buf'
        DB '; DWORD block; DWORD count)',13,10,'{',13,10
        DB '  DWORD i,words_avail,sectors_avail;',13,10,13
        DB 10,'  ide_wait_for_drive();',13,10,'  ide_sele'
        DB 'ct_block(0,block,count);',13,10,'  outp(IDE_CO'
        DB 'MMAND,WIN_MULTWRITE);',13,10,'  while (count) '
        DB '{',13,10,'    while (inp(IDE_STATUS) & DRIVE_R'
        DB 'EADY!=DRIVE_READY)',13,10,'      swap_in_next;'
        DB 13,10,'    while (inp(IDE_STATUS) & DATA_READY!'
        DB '=DATA_READY)',13,10,'      swap_in_next;',13,10
        DB '    sectors_avail=1;',13,10,'    words_avail=s'
        DB 'ectors_avail*BLOCK_SIZE/2;',13,10,'    rep_out'
        DB 'sw(buf,words_avail,IDE_DATA_REG);',13,10,'    '
        DB 'buf=buf+words_avail*2;',13,10,'    count=count'
        DB '-sectors_avail;',13,10,'  }',13,10,'  while '
        DB '(!btr(SYS_FLAGS,SYSF_IDE1_DONE))',13,10,'    s'
        DB 'wap_in_next;',13,10,'  btr(SYS_FLAGS,SYSF_IDE1'
        DB '_BUSY);',13,10,'}',13,10,13,10,13,10,'//'
        DB 'This writes blocks directly to disk.  This is da'
        DB 'ngerous',13,10,'//but if you can use it for da'
        DB 'tabase design, once you',13,10,'//allocate som'
        DB 'e blocks in the FAT.',13,10,'//Blocks are typi'
        DB 'cally 512 bytes',13,10,'//',13,10,'//It need'
        DB 's to be fixed so it does more than one block at '
        DB 13,10,'//a time when it calls ide_write_blocks.'
        DB 13,10,'void wblocks(BYTE *buf, DWORD block, DWO'
        DB 'RD count)',13,10,'{',13,10,'  BYTE *verify_b'
        DB 'uf;',13,10,'  DWORD l,i,verify_errors=0;',13,10
        DB '  verify_buf=malloc(MAX_WRITES_AT_ONCE*BLOCK_SIZ'
        DB 'E);',13,10,'  while (count>0) {',13,10,'    '
        DB 'l=count;',13,10,'    if (l>MAX_WRITES_AT_ONCE)'
        DB 13,10,'      l=MAX_WRITES_AT_ONCE;',13,10,'  '
        DB '  ide_write_blocks(buf,block,l);',13,10,'    r'
        DB 'blocks(verify_buf,block,l);',13,10,'    for (i'
        DB '=0;i<l*BLOCK_SIZE;i++)',13,10,'      if (verif'
        DB 'y_buf[i]!=buf[i])',13,10,'        verify_error'
        DB 's++;',13,10,'    buf=buf+l*BLOCK_SIZE;',13,10
        DB '    block=block+l;',13,10,'    count=count-l;',13
        DB 10,'  }',13,10,'  if (verify_errors)',13,10,' '
        DB '   ? systext(st_verify_error),verify_errors,"\r\'
        DB 'n";',13,10,'  free(verify_buf);',13,10,'}',13
        DB 10,13,10,13,10,'//This writes a cluster (typ'
        DB 'ically 8 blocks) and numbered',13,10,'//from t'
        DB 'he start of the data area of the partition.',13
        DB 10,'void wcluster(BYTE *buf,DWORD c)',13,10,'{'
        DB 13,10,'  my_partition_entry *p=&local_partition'
        DB 's[fs->current_partition];',13,10,'  wblocks(bu'
        DB 'f,p->data+c*p->cur_FAT32_br->sectors_per_cluster'
        DB ',',13,10,'          p->cur_FAT32_br->sectors_p'
        DB 'er_cluster);',13,10,'}',13,10,13,10,'//Thi'
        DB 's writes several clusters',13,10,'void wcluste'
        DB 'rs(BYTE *buf,DWORD c,DWORD num)',13,10,'{',13
        DB 10,'  my_partition_entry *p=&local_partitions[fs'
        DB '->current_partition];',13,10,'  while (num--) '
        DB '{',13,10,'    wcluster(buf,c);',13,10,'    b'
        DB 'uf=buf+BLOCK_SIZE*p->cur_FAT32_br->sectors_per_c'
        DB 'luster;',13,10,'    c=next_FAT32_cluster_num(c'
        DB ');',13,10,'  }',13,10,'}',13,10,13,10,'/'
        DB '/This frees a chain of clusters.  It needs to be'
        DB 13,10,'//made more efficient by doing one write'
        DB ' for',13,10,'//all modifications to the same F'
        DB 'AT block',13,10,'void free_FAT32_clusters(DWOR'
        DB 'D c,bool unlock)',13,10,'{',13,10,'  my_part'
        DB 'ition_entry *p=&local_partitions[fs->current_par'
        DB 'tition];',13,10,'  DWORD FAT_blk_num,next=0;',13
        DB 10,'  bool cont=true;',13,10,'  lock_drive(fs-'
        DB '>current_partition);',13,10,'  while (next!=0x'
        DB '0FFFFFFF && next!=0) {',13,10,'    FAT_blk_num'
        DB '=c/(BLOCK_SIZE/4);',13,10,'    if (p->cur_FAT3'
        DB '2_blk_num!=FAT_blk_num) {',13,10,'      rblock'
        DB 's(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);',13,10
        DB '      p->cur_FAT32_blk_num=FAT_blk_num;',13,10
        DB '    }',13,10,'    next=p->cur_FAT32_blk[c-FAT_'
        DB 'blk_num*(BLOCK_SIZE/4)];',13,10,'    p->cur_FA'
        DB 'T32_blk[c-FAT_blk_num*(BLOCK_SIZE/4)]=0;',13,10
        DB '    wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num'
        DB ',1);',13,10,'    wblocks(p->cur_FAT32_blk,p->F'
        DB 'AT2+FAT_blk_num,1);',13,10,'    c=next;',13,10
        DB '  }',13,10,'  if (unlock)',13,10,'    unlock'
        DB '_drive(fs->current_partition);',13,10,'}',13,10
        DB 13,10,'//This is a low level routine you genera'
        DB 'lly don',39,'t need',13,10,'//to call yoursel'
        DB 'f.  save_FAT32_file calls this automatically',13
        DB 10,'DWORD alloc_FAT32_clusters(DWORD c,DWORD num'
        DB ',bool unlock=true)  //c=0 means first cluster in'
        DB ' chain',13,10,'{',13,10,'  my_partition_entr'
        DB 'y *p=&local_partitions[fs->current_partition];',13
        DB 10,'  DWORD FAT_blk_num,first=0;',13,10,'  boo'
        DB 'l cont;',13,10,'  lock_drive(fs->current_parti'
        DB 'tion);',13,10,'  while (num--) {',13,10,'   '
        DB ' cont=true;',13,10,'    while (cont) {',13,10
        DB '      p->cur_FAT32_fis->most_recently_allocated+'
        DB '+;',13,10,'      p->cur_FAT32_fis->free_cluste'
        DB 'rs=0xFFFFFFFF;',13,10,'      FAT_blk_num=p->cu'
        DB 'r_FAT32_fis->most_recently_allocated/(BLOCK_SIZE'
        DB '/4);',13,10,'      if (p->cur_FAT32_blk_num!=F'
        DB 'AT_blk_num) {',13,10,'        rblocks(p->cur_F'
        DB 'AT32_blk,p->FAT1+FAT_blk_num,1);',13,10,'     '
        DB '   p->cur_FAT32_blk_num=FAT_blk_num;',13,10,' '
        DB '     }',13,10,'      if (!p->cur_FAT32_blk[p->'
        DB 'cur_FAT32_fis->most_recently_allocated-',13,10
        DB '        FAT_blk_num*(BLOCK_SIZE/4)]) {',13,10,' '
        DB '       p->cur_FAT32_blk[p->cur_FAT32_fis->most_r'
        DB 'ecently_allocated-',13,10,'          FAT_blk_n'
        DB 'um*(BLOCK_SIZE/4)]=0x0FFFFFFF;',13,10,'       '
        DB ' wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1)'
        DB ';',13,10,'        wblocks(p->cur_FAT32_blk,p->'
        DB 'FAT2+FAT_blk_num,1);',13,10,'        wblocks(p'
        DB '->cur_FAT32_fis,p->file_system_info_sector,1);',13
        DB 10,'        cont=false;',13,10,'      }',13,10
        DB '    }',13,10,'    if (!first)',13,10,'      '
        DB 'first=p->cur_FAT32_fis->most_recently_allocated;'
        DB 13,10,'    if (c) {',13,10,'      FAT_blk_num'
        DB '=c/(BLOCK_SIZE/4);',13,10,'      if (p->cur_FA'
        DB 'T32_blk_num!=FAT_blk_num) {',13,10,'        rb'
        DB 'locks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);',13
        DB 10,'        p->cur_FAT32_blk_num=FAT_blk_num;',13
        DB 10,'      }',13,10,'      p->cur_FAT32_blk[c-F'
        DB 'AT_blk_num*(BLOCK_SIZE/4)]=p->cur_FAT32_fis->mos'
        DB 't_recently_allocated;',13,10,'      wblocks(p-'
        DB '>cur_FAT32_blk,p->FAT1+FAT_blk_num,1);',13,10,' '
        DB '     wblocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_nu'
        DB 'm,1);',13,10,'    }',13,10,'    c=p->cur_FAT'
        DB '32_fis->most_recently_allocated;',13,10,'  }',13
        DB 10,'  if (unlock)',13,10,'    unlock_drive(fs-'
        DB '>current_partition);',13,10,'  return first;',13
        DB 10,'}',13,10,13,10,'//This searches for a co'
        DB 'ntiguous space of blocks',13,10,'//You typical'
        DB 'ly don',39,'t need to call this yourself, unles'
        DB 's',13,10,'//you are setting-up an area for a d'
        DB 'atabase.',13,10,'DWORD alloc_contiguous_FAT32_'
        DB 'clusters(DWORD num, bool unlock=true)  //c=0 mea'
        DB 'ns first cluster in chain',13,10,'{',13,10,' '
        DB ' my_partition_entry *p=&local_partitions[fs->cur'
        DB 'rent_partition];',13,10,'  DWORD FAT_blk_num,f'
        DB 'irst=1,i;',13,10,'  bool cont;',13,10,'  if '
        DB '(!num) return;',13,10,'  lock_drive(fs->curren'
        DB 't_partition);',13,10,'  while (true) {',13,10
        DB '    first++;',13,10,'    i=0;',13,10,'    co'
        DB 'nt=true;',13,10,'    while (cont && i<num) {',13
        DB 10,'      FAT_blk_num=(first+i)/(BLOCK_SIZE/4);',13
        DB 10,'      if (p->cur_FAT32_blk_num!=FAT_blk_num)'
        DB ' {',13,10,'        rblocks(p->cur_FAT32_blk,p-'
        DB '>FAT1+FAT_blk_num,1);',13,10,'        p->cur_F'
        DB 'AT32_blk_num=FAT_blk_num;',13,10,'      }',13
        DB 10,'      if (p->cur_FAT32_blk[(first+i)-FAT_blk'
        DB '_num*(BLOCK_SIZE/4)])',13,10,'        cont=fal'
        DB 'se;',13,10,'      else',13,10,'        i++;',13
        DB 10,'    }',13,10,'    if (cont) {',13,10,'  '
        DB '    for (i=0;i<num;i++) {',13,10,'        FAT_'
        DB 'blk_num=(first+i)/(BLOCK_SIZE/4);',13,10,'    '
        DB '    if (p->cur_FAT32_blk_num!=FAT_blk_num) {',13
        DB 10,'          rblocks(p->cur_FAT32_blk,p->FAT1+F'
        DB 'AT_blk_num,1);',13,10,'          p->cur_FAT32_'
        DB 'blk_num=FAT_blk_num;',13,10,'        }',13,10
        DB '        if (i+1==num)',13,10,'          p->cur'
        DB '_FAT32_blk[(first+i)-FAT_blk_num*(BLOCK_SIZE/4)]'
        DB '=0x0FFFFFFF;',13,10,'        else',13,10,'  '
        DB '        p->cur_FAT32_blk[(first+i)-FAT_blk_num*('
        DB 'BLOCK_SIZE/4)]=first+i+1;',13,10,'        if ('
        DB '(first+i+1)/(BLOCK_SIZE/4)!=p->cur_FAT32_blk_num'
        DB ') {',13,10,'          wblocks(p->cur_FAT32_blk'
        DB ',p->FAT1+FAT_blk_num,1);',13,10,'          wbl'
        DB 'ocks(p->cur_FAT32_blk,p->FAT2+FAT_blk_num,1);',13
        DB 10,'        }',13,10,'      }',13,10,'      '
        DB 'wblocks(p->cur_FAT32_blk,p->FAT1+FAT_blk_num,1);'
        DB 13,10,'      wblocks(p->cur_FAT32_blk,p->FAT2+F'
        DB 'AT_blk_num,1);',13,10,'      return first;',13
        DB 10,'    }',13,10,'  }',13,10,'  if (unlock)',13
        DB 10,'    unlock_drive(fs->current_partition);',13
        DB 10,'}',13,10,13,10,'//You typically don',39
        DB 't need to call this yourself.',13,10,'//save_F'
        DB 'AT32_file calls it automatically.',13,10,'bool'
        DB ' new_FAT32_dir_entry(FAT32_dir_entry *de)',13,10
        DB '{',13,10,'  my_partition_entry *p=&local_parti'
        DB 'tions[fs->current_partition];',13,10,'  FAT32_'
        DB 'dir_entry *buf;',13,10,'  DWORD cur_dir_cluste'
        DB 'r,cur_dir_entry,entries_per_cluster,entries_per_'
        DB 'block;',13,10,'  bool cont=true,cont2,written='
        DB 'false;',13,10,'  char *ptr,*ptr2,dname[11];',13
        DB 10,'  DWORD i;',13,10,'  BYTE ch;',13,10,13
        DB 10,'  lock_drive(fs->current_partition);',13,10
        DB '  if (p->type!=0x0B) ',13,10,'    ? systext(st'
        DB '_not_FAT32);',13,10,'  else {',13,10,'    bu'
        DB 'f=malloc(BLOCK_SIZE*p->cur_FAT32_br->sectors_per'
        DB '_cluster);',13,10,'    entries_per_block=BLOCK'
        DB '_SIZE/sizeof(FAT32_dir_entry);',13,10,'    ent'
        DB 'ries_per_cluster=entries_per_block*p->cur_FAT32_'
        DB 'br->sectors_per_cluster;',13,10,'    cur_dir_c'
        DB 'luster=fs->current_dir_cluster;',13,10,'    rc'
        DB 'luster(buf,cur_dir_cluster);',13,10,'    cur_d'
        DB 'ir_entry=0;',13,10,'    while (cont) {',13,10
        DB '      if (cur_dir_entry==entries_per_cluster) {',13
        DB 10,'        cur_dir_cluster=next_FAT32_cluster_n'
        DB 'um(cur_dir_cluster);',13,10,'        rcluster('
        DB 'buf,cur_dir_cluster);',13,10,'        cur_dir_'
        DB 'entry=0;',13,10,'      }',13,10,'      ch=bu'
        DB 'f[cur_dir_entry].name[0];',13,10,'      if (!c'
        DB 'h) {',13,10,'        if (!written) {',13,10,' '
        DB '         memcpy(&buf[cur_dir_entry],de,sizeof(FA'
        DB 'T32_dir_entry));',13,10,'          wblocks(&bu'
        DB 'f[cur_dir_entry & (0-entries_per_block)],',13,10
        DB '             p->data+cur_dir_cluster*p->cur_FAT3'
        DB '2_br->sectors_per_cluster',13,10,'            '
        DB ' +cur_dir_entry/entries_per_block,1);',13,10,' '
        DB '         cur_dir_entry++;',13,10,'          if'
        DB ' (cur_dir_entry==entries_per_cluster) {',13,10
        DB '            cur_dir_cluster=alloc_FAT32_clusters'
        DB '(cur_dir_cluster,1);',13,10,'            cur_d'
        DB 'ir_entry=0;',13,10,'          }',13,10,'    '
        DB '      buf[cur_dir_entry].name[0]=0;',13,10,'  '
        DB '        wblocks(&buf[cur_dir_entry & (0-entries_'
        DB 'per_block)],',13,10,'             p->data+cur_'
        DB 'dir_cluster*p->cur_FAT32_br->sectors_per_cluster'
        DB 13,10,'             +cur_dir_entry/entries_per_'
        DB 'block,1);',13,10,'        }',13,10,'        '
        DB 'cont=false;',13,10,'      } else if (ch==0xE5)'
        DB ' {',13,10,'        if (!written) {',13,10,' '
        DB '         memcpy(&buf[cur_dir_entry],de,sizeof(FA'
        DB 'T32_dir_entry));',13,10,'          wblocks(&bu'
        DB 'f[cur_dir_entry & (0-entries_per_block)],',13,10
        DB '            p->data+cur_dir_cluster*p->cur_FAT32'
        DB '_br->sectors_per_cluster',13,10,'            +'
        DB 'cur_dir_entry/entries_per_block,1);',13,10,'  '
        DB '        written=true;',13,10,'        }',13,10
        DB '      } else { ',13,10,'        cont2=true;',13
        DB 10,'        ptr=de->name;',13,10,'        ptr2'
        DB '=buf[cur_dir_entry].name;',13,10,'        for '
        DB '(i=0;i<11 && cont2;i++)',13,10,'          if ('
        DB '*ptr++!=*ptr2++)',13,10,'            cont2=fal'
        DB 'se;',13,10,'        if (cont2) { //Delete ond '
        DB 'entry with same name',13,10,'          buf[cur'
        DB '_dir_entry].name[0]=0xE5;',13,10,'          wb'
        DB 'locks(&buf[cur_dir_entry & (0-entries_per_block)'
        DB '],',13,10,'             p->data+cur_dir_cluste'
        DB 'r*p->cur_FAT32_br->sectors_per_cluster',13,10,' '
        DB '            +cur_dir_entry/entries_per_block,1);'
        DB 13,10,'          free_FAT32_clusters(buf[cur_di'
        DB 'r_entry].cluster_lo+',13,10,'            (buf['
        DB 'cur_dir_entry].cluster_hi<<16),false);',13,10,' '
        DB '       }',13,10,'      }',13,10,'      cur_d'
        DB 'ir_entry++;',13,10,'    }',13,10,'    free(b'
        DB 'uf);',13,10,'  }  ',13,10,'  unlock_drive(fs'
        DB '->current_partition);',13,10,'  return false;',13
        DB 10,'}',13,10,13,10,'//This is a low level ro'
        DB 'utine.',13,10,'//del() calls this',13,10,'bo'
        DB 'ol del_FAT32_files(char *wild)',13,10,'{',13,10
        DB '  my_partition_entry *p=&local_partitions[fs->cu'
        DB 'rrent_partition];',13,10,'  FAT32_dir_entry *b'
        DB 'uf;',13,10,'  DWORD cur_dir_cluster,cur_dir_en'
        DB 'try,entries_per_cluster,entries_per_block;',13,10
        DB '  bool cont=true;',13,10,'  char cbuf[16];',13
        DB 10,'  BYTE ch;',13,10,13,10,'  lock_drive(fs'
        DB '->current_partition);',13,10,'  if (p->type!=0'
        DB 'x0B) ',13,10,'    ? systext(st_not_FAT32);',13
        DB 10,'  else {',13,10,'    buf=malloc(BLOCK_SIZE'
        DB '*p->cur_FAT32_br->sectors_per_cluster);',13,10
        DB '    entries_per_block=BLOCK_SIZE/sizeof(FAT32_di'
        DB 'r_entry);',13,10,'    entries_per_cluster=entr'
        DB 'ies_per_block*p->cur_FAT32_br->sectors_per_clust'
        DB 'er;',13,10,'    cur_dir_cluster=fs->current_di'
        DB 'r_cluster;',13,10,'    rcluster(buf,cur_dir_cl'
        DB 'uster);',13,10,'    cur_dir_entry=0;',13,10,' '
        DB '   while (cont) {',13,10,'      if (cur_dir_en'
        DB 'try==entries_per_cluster) {',13,10,'        cu'
        DB 'r_dir_cluster=next_FAT32_cluster_num(cur_dir_clu'
        DB 'ster);',13,10,'        rcluster(buf,cur_dir_cl'
        DB 'uster);',13,10,'        cur_dir_entry=0;',13,10
        DB '      }',13,10,'      ch=buf[cur_dir_entry].na'
        DB 'me[0];',13,10,'      if (!ch)',13,10,'      '
        DB '  cont=false;',13,10,'      else if (ch!=0xE5)'
        DB ' {',13,10,'        from_FAT32_name(buf[cur_dir'
        DB '_entry].name,cbuf);',13,10,'        if (wild_c'
        DB 'ard_match(wild,cbuf)) {',13,10,'          ? sy'
        DB 'stext(st_delete),cbuf,"\r\n";',13,10,'        '
        DB '  buf[cur_dir_entry].name[0]=0xE5;',13,10,'   '
        DB '       wblocks(&buf[cur_dir_entry & (0-entries_p'
        DB 'er_block)],',13,10,'             p->data+cur_d'
        DB 'ir_cluster*p->cur_FAT32_br->sectors_per_cluster',13
        DB 10,'             +cur_dir_entry/entries_per_bloc'
        DB 'k,1);',13,10,'          free_FAT32_clusters(bu'
        DB 'f[cur_dir_entry].cluster_lo+',13,10,'         '
        DB '   (buf[cur_dir_entry].cluster_hi<<16),false);',13
        DB 10,'        }',13,10,'      }',13,10,'      '
        DB 'cur_dir_entry++;',13,10,'    }',13,10,'    f'
        DB 'ree(buf);',13,10,'  }  ',13,10,'  unlock_dri'
        DB 've(fs->current_partition);',13,10,'  return fa'
        DB 'lse;',13,10,'}',13,10,13,10,'//Save text f'
        DB 'ile calls this',13,10,'void save_FAT32_file(ch'
        DB 'ar *name,BYTE *buf,DWORD size,bool contiguous=tr'
        DB 'ue)',13,10,'{',13,10,'  FAT32_dir_entry de;',13
        DB 10,'  my_partition_entry *p=&local_partitions[fs'
        DB '->current_partition];',13,10,'  DWORD c,cluste'
        DB 'r_count,',13,10,'        cluster_size=BLOCK_SI'
        DB 'ZE*p->cur_FAT32_br->sectors_per_cluster;',13,10
        DB '  to_FAT32_name(name,de.name);',13,10,'  de.si'
        DB 'ze=size;',13,10,'  cluster_count=(size+cluster'
        DB '_size-1)/cluster_size;',13,10,'  if (contiguou'
        DB 's)',13,10,'    c=alloc_contiguous_FAT32_cluste'
        DB 'rs(cluster_count);',13,10,'  else',13,10,'  '
        DB '  c=alloc_FAT32_clusters(0,cluster_count);',13,10
        DB '  de.cluster_lo=c & 0xFFFF;',13,10,'  de.clust'
        DB 'er_hi=c >> 16;',13,10,'  de.attr=0;',13,10,' '
        DB ' de.CrtDate=0;',13,10,'  de.CrtTime=0;',13,10
        DB '  de.CrtTimeTenth=0;',13,10,'  de.LstAccDate=0'
        DB ';',13,10,'  de.NTres=0;',13,10,'  de.WRtDate'
        DB '=0;',13,10,'  de.WrtTime=0;',13,10,'  new_FA'
        DB 'T32_dir_entry(&de);',13,10,'  wclusters(buf,c,'
        DB 'cluster_count);',13,10,'}',13,10,13,10,'vo'
        DB 'id dir_FAT32(char *wild)',13,10,'{',13,10,' '
        DB ' my_partition_entry *p=&local_partitions[fs->cur'
        DB 'rent_partition];',13,10,'  FAT32_dir_entry *bu'
        DB 'f;',13,10,'  DWORD cur_dir_cluster,cur_dir_ent'
        DB 'ry,entries_per_cluster;',13,10,'  bool cont=tr'
        DB 'ue;',13,10,'  char *ptr,cbuf[16];',13,10,'  '
        DB 'DWORD i;',13,10,'  BYTE ch;',13,10,13,10,' '
        DB ' lock_drive(fs->current_partition);',13,10,'  '
        DB 'if (p->type!=0x0B) ',13,10,'    ? systext(st_n'
        DB 'ot_FAT32);',13,10,'  else {',13,10,'    ? "D'
        DB 'irectory of ",fs->current_directory,"\r\n";',13
        DB 10,'    spaces(16); ? "  SIZE   CLUSTER\r\n";',13
        DB 10,'    buf=malloc(BLOCK_SIZE*p->cur_FAT32_br->s'
        DB 'ectors_per_cluster);',13,10,'    entries_per_c'
        DB 'luster=BLOCK_SIZE*p->cur_FAT32_br->sectors_per_c'
        DB 'luster',13,10,'        /sizeof(FAT32_dir_entry'
        DB ');',13,10,'    cur_dir_cluster=fs->current_dir'
        DB '_cluster;',13,10,'    rcluster(buf,cur_dir_clu'
        DB 'ster);',13,10,'    cur_dir_entry=0;',13,10,' '
        DB '   while (cont) {',13,10,'      if (cur_dir_en'
        DB 'try==entries_per_cluster) {',13,10,'        cu'
        DB 'r_dir_cluster=next_FAT32_cluster_num(cur_dir_clu'
        DB 'ster);',13,10,'        if (cur_dir_cluster==0x'
        DB 'FFFFFFF)',13,10,'          cont=false;',13,10
        DB '        else {',13,10,'          rcluster(buf,'
        DB 'cur_dir_cluster);',13,10,'          cur_dir_en'
        DB 'try=0;',13,10,'        }',13,10,'      }',13
        DB 10,'      if (cont) {',13,10,'        ch=buf[c'
        DB 'ur_dir_entry].name[0];',13,10,'        if (!ch'
        DB ') {',13,10,'          cont=false;',13,10,'  '
        DB '      } else if (ch!=0xE5) {',13,10,'         '
        DB ' from_FAT32_name(buf[cur_dir_entry].name,cbuf);',13
        DB 10,'          if (wild_card_match(wild,cbuf)) {',13
        DB 10,'            i=buf[cur_dir_entry].cluster_lo+'
        DB '(buf[cur_dir_entry].cluster_hi<<16);',13,10,' '
        DB '           ? cbuf;',13,10,'            spaces('
        DB '16-strlen(cbuf));',13,10,'            ? buf[cu'
        DB 'r_dir_entry].size," ",i,"\r\n";',13,10,'      '
        DB '    }',13,10,'        }',13,10,'      }',13
        DB 10,'      cur_dir_entry++;',13,10,'    }',13,10
        DB '    free(buf);',13,10,'  }  ',13,10,'  unloc'
        DB 'k_drive(fs->current_partition);',13,10,'}',13
        DB 10,' ',13,10,'BYTE *read_file(char *filename,D'
        DB 'WORD *size=NULL)',13,10,'{',13,10,'  return '
        DB 'read_FAT32_file(filename,size);',13,10,'}',13
        DB 10,13,10,'defext(ext_wblocks,&wblocks);',13,10
        DB 'defext(ext_rblocks,&rblocks);',13,10,'defext(e'
        DB 'xt_irq0E,&irq0E_handler);',13,10,'defext(ext_r'
        DB 'ead_file,&read_file);',13,10,'defext(ext_selec'
        DB 't_default_drive,&select_default_drive);',13,10
        DB ' ',13,10,' '
        DB 0
        ENDS
        END
