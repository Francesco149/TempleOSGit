#include "SPRINTF.CPP"
#include "UTILS.CPP"
#include "EDIT.CPP"
*SYS_CLIPBOARD_LTF=new_linked_text_file;
to_linked_text_file("\r\n",*SYS_CLIPBOARD_LTF);
#include "INPTR.CPP"
#include "FIFO.CPP"
tss_struct *update_screen_tss;
#include "WINDOW.CPP"
#include "FORM.CPP"

use_console_ltf(NULL);
btr(SYS_FLAGS,SYSF_RAW_CRT_OUT);
preempt(on);
root_tss->window_top=2;
root_tss->window_bottom=10;

//spawn(TSK_FREEMEM,"FREE MEMORY");
update_screen_tss=spawn(&window_manager_tsk,"WINDOW MANAGER");
spawn(COMMAND_LINE,"USER CMD PROMPT");

#include "JOY.CPP"
spawn(&joystick_tsk,"JOYSTICK POLL");
load_joystick_calibration;
; 
 
