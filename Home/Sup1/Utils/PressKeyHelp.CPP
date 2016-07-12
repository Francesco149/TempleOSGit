#help_index "Help System"

public U0 PressKeyHelp(CTask *task=NULL)
{//Jump to key handlers src for key presses.
  I64 a1=0,a2=0;
  CTask *old_hash=Fs->hash_table;
  if (!task) task=Fs;

    /*
This feature is a burden in code.  It would be nice to lst key
cmds for all applications, but it makes the code too ugly.
*/
  task=Fs;

  Fs->hash_table=task->hash_table;
  "Press keys to show handler code.\n"
	"<SHIFT-ESC> to exit.\n"
	"Do not press <CTRL-ALT-keys>.\n";
  while (a1!=CH_ESC && a1!=CH_SHIFT_ESC) {
    GetMsg(&a1,&a2,1<<MSG_KEY_DOWN);
    *keydev.desc=0;
    keydev.handler=NULL;
    if (task==Fs)
      PutKey(a1,a2|SCF_KEY_DESC);
    else if (TaskValidate(task))
      PostMsg(task,MSG_KEY_DOWN,a1,a2|SCF_KEY_DESC);
    WinMgrSync(0,TRUE);
    Sleep(1);
    if (*keydev.desc)
      E(keydev.handler,512);
  }
  Fs->hash_table=old_hash;
}

PressKeyHelp;
