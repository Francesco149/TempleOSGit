#help_index "Misc/Host"

public Bool HostChgDsk(U8 *iso_filename)
{//Help user change CD/DVD or ISO_file.
  Bool res=FALSE;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  try {
    AutoComplete;
    WinBorder;
    WinMax;
    DocClear;
    "$PURPLE$$TX+CX,\"Set to %s\"$$FG$\n\n\n\n",iso_filename;
    switch (ins_reg.host_vm) {
      case VM_VMWARE:
	if (IsDotC(iso_filename))
"
Click CDROM Icon in bottom-right border of VMware window.

Click \"Settings\".

Set ISO image to \"%s\".
When browsing, change the file mask from \"*.iso\" to \"All Files\".

",iso_filename;
	  else
"
Click CDROM Icon in bottom-right border of VMware window.

Click \"Settings\".

Set ISO image to \"%s\".

",iso_filename;
	break;
      case VM_QEMU:
"
Change to QEMU monitor using <CTRL-ALT-2>

Get the CDROM designation.

	>info block

Use the designation.

	>eject ide1-cd0

Use the designation.

	>change ide1-cd0 %s

Change back to QEMU using <CTRL-ALT-1>

",iso_filename;
	break;
      default:
"
Place the CDDVD %s in the drive.

",iso_filename;
    }
    if (PressAKey!=CH_SHIFT_ESC) {
      ">DskChg('T');\n\n";
      DskChg('T');
      res=TRUE;
    }
  } catch
    PutExcept;
  SettingsPop;
  return res;
}
