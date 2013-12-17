//This file is executed by the
//first terminal window upon start-up.
//See [C:/TempleOS/Accts/TAD/Adam3fSys.CPP,1] DoItOnce and [C:/Doc/GuideLines.TXT,1] Acct Files.

//It goes in your HOME directory and
//you can customize it.

//Delete this to get rid of
//default start-up msg.
Type("::/Doc/Customize.TXT");

"\nTake Tour";
if (YorN) {
  Cd("::/Misc/Tour");
  AutoFile("Tour");
}
