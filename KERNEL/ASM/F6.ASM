        INCLUDE OSINC.INC
;;***************************SEGMENT**************************
SYSFILES SEGMENT DWORD USE32 PUBLIC
        ASSUME CS:SYSFILES,DS:SYSFILES
        ALIGN   4
        PUBLIC  FIL_SYSTEXT_CPP
FIL_SYSTEXT_CPP:

        DB '#define st_user_startup           2',13,10,'#d'
        DB 'efine st_undefined_systext      8 ',13,10,'#de'
        DB 'fine st_invalid_systext_number 10',13,10,'#def'
        DB 'ine st_file_not_found         12',13,10,'#defi'
        DB 'ne st_press_a_key            13',13,10,'#defin'
        DB 'e st_verify_error           14',13,10,'#define'
        DB ' st_not_FAT32              15',13,10,'#define '
        DB 'st_delete                 16',13,10,'#define s'
        DB 't_invalid_drive          17',13,10,'#define st'
        DB '_current_partition      18',13,10,'#define st_'
        DB 'table_size 4096',13,10,'void load_sys_strings('
        DB ')',13,10,'{',13,10,'  int i;',13,10,13,10
        DB '  i=0x002;  // #0-0xFF reserved for general OS t'
        DB 'ext',13,10,'  loadstr(i++,"Welcome\r\n");',13
        DB 10,'  loadstr(i++,">");',13,10,'  loadstr(i++,'
        DB '"Interrupt #");',13,10,'  loadstr(i++,"Trap\r\'
        DB 'n");',13,10,'  loadstr(i++,"Invalid task addre'
        DB 'ss");',13,10,'  loadstr(i++,"ERROR: Undefined '
        DB 'extern\r\n");',13,10,'  loadstr(i++,"ERROR: Un'
        DB 'defined systext\r\n");',13,10,'  loadstr(i++,"'
        DB 'Invalid extern number\r\n");',13,10,'  loadstr'
        DB '(i++,"Invalid systext number\r\n");',13,10,'  '
        DB 'loadstr(i++,"ERROR: Command not found.\r\n");',13
        DB 10,'  loadstr(i++,"ERROR: File not found.\r\n");'
        DB 13,10,'  loadstr(i++,"PRESS A KEY\r\n");',13,10
        DB '  loadstr(i++,"Verify Error:");',13,10,'  load'
        DB 'str(i++,"ERROR: Not FAT32 Partition\r\n");',13,10
        DB '  loadstr(i++,"Delete ");',13,10,'  loadstr(i+'
        DB '+,"ERROR: Invalid Drive\r\n");',13,10,'  loads'
        DB 'tr(i++,"Current Partition: ");',13,10,13,10,' '
        DB ' i=0x100;  // #0-0x1FF reserved for system parse'
        DB 'r/compiler',13,10,'  loadstr(i++,"ERROR: ");',13
        DB 10,'  loadstr(i++,"missing ',39,';',39,' at");'
        DB 13,10,'  loadstr(i++,"missing expression at ");'
        DB 13,10,'  loadstr(i++,"missing ',39,')',39,' a'
        DB 't ");',13,10,'  loadstr(i++,"missing ',39,']',39
        DB ' at ");',13,10,'  loadstr(i++,"undefined symbo'
        DB 'l at ");',13,10,'  loadstr(i++,"invalid class '
        DB 'at ");',13,10,'  loadstr(i++,"invalid member a'
        DB 't ");',13,10,'  loadstr(i++,"expecting type at'
        DB ' ");',13,10,'  loadstr(i++,"expecting identifi'
        DB 'er at ");',13,10,'  loadstr(i++,"expecting ',39
        DB ')',39,' at ");',13,10,'  loadstr(i++,"expecti'
        DB 'ng ',39,'{',39,' at ");',13,10,'  loadstr(i+'
        DB '+,"missing ',39,'}',39,' at ");',13,10,'  lo'
        DB 'adstr(i++,"invalid lvalue at ");',13,10,'  loa'
        DB 'dstr(i++,"expecting ',39,'=',39,' at ");',13,10
        DB '  loadstr(i++,"not pointer type at ");',13,10,' '
        DB ' loadstr(i++,"expecting ',39,',',39,' at ");',13
        DB 10,'  loadstr(i++,"expecting system symbol at ")'
        DB ';',13,10,'  loadstr(i++,"expecting string at "'
        DB ');',13,10,13,10,'  i=0x200;  //Start of next'
        DB ' block of system text',13,10,13,10,'  i=0x40'
        DB '0;  //Start of user messages',13,10,'}',13,10
        DB 13,10,13,10,'char *systext(DWORD num)',13,10
        DB '{',13,10,'  char *result;',13,10,'  if (!num'
        DB ' || num>=4096) {',13,10,'    ? fs->string_tabl'
        DB 'e[st_invalid_systext_number];',13,10,'    retu'
        DB 'rn NULL;',13,10,'  }',13,10,'  result=fs->st'
        DB 'ring_table[num];',13,10,'  if (!result) ',13,10
        DB '    ? fs->string_table[st_undefined_systext];',13
        DB 10,'  return result;',13,10,'}',13,10,13,10
        DB ' '
        DB 0
        ENDS
        END
