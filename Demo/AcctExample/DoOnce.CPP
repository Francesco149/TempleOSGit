//Place this file in /Home and change
//anything you want.

//This file is executed by the
//first terminal window upon start-up.
//See [D:/Home/AdamSys.CPP,1] DoOnce and [C:/Doc/GuideLines.TXT.Z,1] Home Files.

//  Type("::/Doc/Customize.TXT");

U0 DoOnce()
{
  if (FileFind("D:/Temp/Logs/access.log")) {
    Auto("\n\n%C",CH_ESC);
    WebLogRep("D:/Temp/Logs/*.log","D:/Temp/LogRep.TXT");
    Del("D:/Temp/Logs/*.log");
  }
}

DoOnce;
