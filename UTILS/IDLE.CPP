//This displays the number of cycles through tasks per "JIFFIE" (55mS) 
void idle_check()
{
  DWORD count=0,jiffy=*JIFFIES;
  btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
  while (!bt(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY)) {
    count=0;
    while (jiffy==*JIFFIES) {
      count++;
      swap_in_next;
    }
    jiffy=*JIFFIES;
    ?count,"\r\n";
  }
}

spawn(&idle_check,"IDLE CHECK");;