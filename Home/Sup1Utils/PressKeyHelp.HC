#help_index "Help System"

public U0 PressKeyHelp(CTask *task=NULL)
{//Jump to key hndlrs src for key presses.
  I64 arg1=0,arg2=0;
  CTask *old_hash=Fs->hash_table;
  if (!task) task=Fs;

    /*
This feature is a burden in code.  It would be nice to lst key
cmds for all applications, but it makes the code too ugly.
*/
  task=Fs;

  Fs->hash_table=task->hash_table;
  "Press keys to show hndlr code.\n"
	"<SHIFT-ESC> to exit.\n"
	"Do not press <CTRL-ALT-keys>.\n";
  while (arg1!=CH_ESC && arg1!=CH_SHIFT_ESC) {
    GetMsg(&arg1,&arg2,1<<MSG_KEY_DOWN);
    *keydev.desc=0;
    keydev.hndlr=NULL;
    if (task==Fs)
      PutKey(arg1,arg2|SCF_KEY_DESC);
    else if (TaskValidate(task))
      PostMsg(task,MSG_KEY_DOWN,arg1,arg2|SCF_KEY_DESC);
    Refresh(0,TRUE);
    Sleep(1);
    if (*keydev.desc)
      E(keydev.hndlr,512);
  }
  Fs->hash_table=old_hash;
}

PressKeyHelp;
