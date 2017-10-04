U0 MsgLoop()
{
  I64 arg1,arg2,msg_code;
  "Use [C:/Kernel/KernelA.HH.Z,3175] msg_code defines in your programs instead of hardcoded nums.\n"
	"<ESC> to Exit\n";

  //We don't want the mouse bttns to reposition the cursor,
  //so we inhibit the window mgr from processing them.

  Fs->win_inhibit=WIF_SELF_MS_L|WIF_SELF_MS_R;

  do {
    msg_code=GetMsg(&arg1,&arg2,~(1<<MSG_NULL));
    "CMD:%02X ARG1:%08X ARG2:%016X\n",msg_code,arg1,arg2;
  } while (msg_code!=MSG_KEY_DOWN || arg1!=CH_ESC && arg1!=CH_SHIFT_ESC);
  GetMsg(,,1<<MSG_KEY_UP);

  Fs->win_inhibit=WIG_USER_TASK_DFT;
}

MsgLoop;

// Press <CTRL-SHIFT-L> and "Insert [C:/Kernel/KernelA.HH.Z,3426] ASCII/[C:/Doc/CharOverview.DD.Z,1] ScanCode".
