//Place this file in /Home and change
//anything you want.

//This file is executed by the
//first terminal window upon start-up.
//See [D:/Home/AdamSys.CPP,1] DoOnce and [C:/Doc/GuideLines.TXT.Z,1] Home Files.

//Delete the rest from this file.

U0 DoOnce()
{
  if (sys_boot_src.u16[0]==BOOT_SRC_CDROM) {
    "\nIf you answer 'No' you can play with\n"
	  "the live CD without installing.\n\n"
	  "Install onto hard drive ";
    if (YorN) {
      DocBottom;
      ExeFile("::/Misc/OSInstall");
    }
  } else {
    "$PURPLE$$TX+CX,\"Tip of the Day\"$$FG$\n";
    TipOfDay;
  }
  Type("::/Doc/Customize.TXT");
  if (FileFind("::/Misc/Tour")) {
    "\nTake Tour";
    if (YorN) {
      Cd("::/Misc/Tour");
      AutoFile("Tour");
    }
  }
}

DoOnce;
