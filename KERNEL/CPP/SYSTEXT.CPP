#define st_user_startup           2
#define st_undefined_systext      8 
#define st_invalid_systext_number 10
#define st_file_not_found         12
#define st_press_a_key            13
#define st_verify_error           14
#define st_not_FAT32              15
#define st_delete                 16
#define st_invalid_drive          17
#define st_current_partition      18
#define st_table_size 4096
void load_sys_strings()
{
  int i;

  i=0x002;  // #0-0xFF reserved for general OS text
  loadstr(i++,"Welcome\r\n");
  loadstr(i++,">");
  loadstr(i++,"Interrupt #");
  loadstr(i++,"Trap\r\n");
  loadstr(i++,"Invalid task address");
  loadstr(i++,"ERROR: Undefined extern\r\n");
  loadstr(i++,"ERROR: Undefined systext\r\n");
  loadstr(i++,"Invalid extern number\r\n");
  loadstr(i++,"Invalid systext number\r\n");
  loadstr(i++,"ERROR: Command not found.\r\n");
  loadstr(i++,"ERROR: File not found.\r\n");
  loadstr(i++,"PRESS A KEY\r\n");
  loadstr(i++,"Verify Error:");
  loadstr(i++,"ERROR: Not FAT32 Partition\r\n");
  loadstr(i++,"Delete ");
  loadstr(i++,"ERROR: Invalid Drive\r\n");
  loadstr(i++,"Current Partition: ");

  i=0x100;  // #0-0x1FF reserved for system parser/compiler
  loadstr(i++,"ERROR: ");
  loadstr(i++,"missing ';' at");
  loadstr(i++,"missing expression at ");
  loadstr(i++,"missing ')' at ");
  loadstr(i++,"missing ']' at ");
  loadstr(i++,"undefined symbol at ");
  loadstr(i++,"invalid class at ");
  loadstr(i++,"invalid member at ");
  loadstr(i++,"expecting type at ");
  loadstr(i++,"expecting identifier at ");
  loadstr(i++,"expecting ')' at ");
  loadstr(i++,"expecting '{' at ");
  loadstr(i++,"missing '}' at ");
  loadstr(i++,"invalid lvalue at ");
  loadstr(i++,"expecting '=' at ");
  loadstr(i++,"not pointer type at ");
  loadstr(i++,"expecting ',' at ");
  loadstr(i++,"expecting system symbol at ");
  loadstr(i++,"expecting string at ");

  i=0x200;  //Start of next block of system text

  i=0x400;  //Start of user messages
}


char *systext(DWORD num)
{
  char *result;
  if (!num || num>=4096) {
    ? fs->string_table[st_invalid_systext_number];
    return NULL;
  }
  result=fs->string_table[num];
  if (!result) 
    ? fs->string_table[st_undefined_systext];
  return result;
}

 