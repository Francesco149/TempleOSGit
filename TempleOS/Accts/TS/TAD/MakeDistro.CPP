/*
                             Roll Your Own Distro

It's unlikely you will get it running on bare hardware, so you'll probably
run it in a virtual machine such as VMware.  Download the free 64-bit
VMware Player for Linux or Windows and point the CDROM at the TempleOSCD.ISO.
(The VMware web page takes a whole minute to load.  They hide the free version.)

Purchase VMware WorkStation or download WinImage if you wish to transfer files
in and out.  The VMWare Workstation will stretch to full-screen and make drives
accessible from Windows.

*/

#define STD_DISTRO_CDROM_CFG	"b10x20000\nt3 \n\n\n\n\n"

U0 MakeMyISO(U8 *out_iso_filename)
{//Does everything with current drive.
  if (!IsDrvWritable) {
    "Drive must be writable.  Install on Hard drive, first.\n";
    return;
  }
  DelTree("/Distro");
  Auto(STD_DISTRO_CDROM_CFG);
  MakeOSInstallPrtBootLoader(,'T');
  CopyTree("/0000Boot","/Distro/0000Boot");
  CopyTree("/TempleOS","/Distro/TempleOS");
  //To save space, optionally delete dictionary.
  Del("/Distro/TempleOS/Adam/WordStat/WSDefs.DAT");
  Del("/Distro/TempleOS/Misc/Bible.TXT.Z");
  Del("/Distro/TempleOS/Misc/PCIDevices.TXT.Z");
  MkDir("/Distro/Temp");
  MkDir("/Distro/Temp/ScreenShots");
  CDFileCreate(out_iso_filename,"/Distro/*","",BOOTCD_BIN_Z);
  //CDImageWrite('T',out_iso_filename,MT_CD); //Uncomment to burn a CD-ROM or (MT_DVD)
  //DelTree("/Distro");
}

MakeMyISO("/MyDistro.ISO");

// Study my account examples [C:/TempleOS/Demo/AcctExample/TSAdam.CPP.Z,1] Cfg Strs [C:/TempleOS/Demo/AcctExample/TSDistro.CPP.Z,1] Update Funs.





/*

* "Linux" is probably a trademark owned by Linus Torvalds.
* "VMware Player" and "VMware WorkStation" are trademarks of VMware, Inc.
* "Windows" is a trademark of MicroSoft Corp.
* "WinImage" is a trademark of Gilles Vollant.

*/
