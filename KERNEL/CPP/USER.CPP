//This is the startup file for new local users
use_console_ltf("/HOPPY/KERNEL/CPP/USER.MEN");

void my_grep(char *pattern)
{
  grep(pattern,"/HOPPY/*.ASM;*.CPP;*.TXT","ri");
}

? systext(st_user_startup);
type("/HOPPY/KERNEL/CPP/HELP.TXT");
cd("/HOPPY/KERNEL/ASM");
;



  
 
