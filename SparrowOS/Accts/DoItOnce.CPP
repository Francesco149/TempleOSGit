//This file is executed by the
//first terminal window upon start-up.
//See [HOME/Adam3fSys.CPP.Z,1] DoItOnce.

//It goes in your HOME directory and
//you can customize it.

//Delete this to get rid of
//default start-up msg.
Type("::/SparrowOS/Doc/Customize.TXT.Z");

"\nTake Tour";
if (YorN) {
  Cd("/SparrowOS/Misc/Tour");
  AutoFile("Tour");
}
