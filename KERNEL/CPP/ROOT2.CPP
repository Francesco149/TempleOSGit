#include "SPRINTF.CPP"
#include "UTILS.CPP"
#include "FIFO.CPP"

//The following are menu number allocations
#define MU_FIND        1
#define MU_REPLACE     2
#define MU_SAVE        3
#define MU_SAVE_AS     4
#define MU_ABORT       5
#define MU_EXIT        6
#define MU_TOGGLE_NUMS 7
#define MU_CUT	       8
#define MU_COPY        9
#define MU_PASTE      10
#define MU_FIND_NEXT  11
#define MU_GO_TO_LINE 12

#define MU_FIRST_USER 1000

char getchar2(DWORD *scan_code)
{
  bool cont=true;
//  btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY);
  while (cont) {
    if (bt(&fs->task_flags,TSSF_FOCUS)) {
      if(btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY))
        cont=false;
    }
    if (cont) swap_in_next;
  }
  *scan_code=*SYS_CURRENT_SCAN_CODE;
  return translate_scan_code;
}

bool scanchar(DWORD *scan_code,char *ch)
{
  bool result=false;
  if (bt(&fs->task_flags,TSSF_FOCUS)) {
    if (btr(SYS_CURRENT_SCAN_CODE,SCF_NEW_KEY)) {
      *scan_code=*SYS_CURRENT_SCAN_CODE;
      *ch=translate_scan_code;
      result=true;
    }
  }
  return result;
}

#include "EDIT.CPP"
*SYS_CLIPBOARD_LTF=new_linked_text_file;
to_linked_text_file("\r\n",*SYS_CLIPBOARD_LTF);
#include "INPTR.CPP"
#include "FIFO.CPP"
tss_struct *update_screen_tss;
#include "MOUSE.CPP"
#include "JOY.CPP"
load_joystick_calibration;  //Run "cal_joystick" first
#include "WINDOW.CPP"
#include "LEX.CPP"
#include "QSORT.CPP"

use_console_ltf(NULL);
btr(SYS_FLAGS,SYSF_RAW_CRT_OUT);
btr(SYS_FLAGS,SYSF_NON_VGA);  //VGA must be enabled in OSMAIN
preempt(on);
root_tss->window_top=2;
root_tss->window_bottom=10;

update_screen_tss=spawn(&window_manager_tsk,"WINDOW MANAGER");

/********* Optional Packages ********/
echo(off);;
cd("/HOPPY/UTILS");
;
#include "ASM.CPP";
#include "DISASM.CPP";
#include "GREP.CPP";
//#include "S2T.CPP";
cd("/HOPPY/KERNEL/ASM");
;
#include "BOOTINS.CPP";
echo(on);;
/************************************/

btr(&fs->task_flags,TSSF_FOCUS);
void duel_users()
{
  int i;
  tss_struct *user1_tss,*user2_tss;
  user1_tss=spawn(USER_COMMAND_LINE,"USER1 CMD PROMPT");
  user2_tss=spawn(USER_COMMAND_LINE,"USER2 CMD PROMPT");
  sleep(500);
  user1_tss->window_top=2;
  user1_tss->window_right=98;
  user2_tss->window_top=2;
  user2_tss->window_left=100;
  btr(&user2_tss->task_flags,TSSF_FOCUS);
  window_to_top(user2_tss);
  window_to_top(user1_tss);
}

duel_users;

