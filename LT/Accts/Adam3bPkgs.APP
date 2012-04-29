//Change anything you want in this
//acct file or others.

Cd("/LT/Adam/Boot");;
#include "InsBoot"
Cd("/LT/Adam");;
#include "Diff"
#include "Grep"
#include "LineCnt"
#include "Merge"
#include "S2T"
#include "LinkChk"
#include "HeapLog"
#include "MemRep"
#include "ToHtml"
#include "ToTXT"
#include "BinRep"
#include "BMPFile"
Cd("/LT/Apps");;

MapFileLoad("::/LT/OSMain/OSMain.MPZ");
MapFileLoad("::/LT/Compiler/Compiler.MPZ");

Cd("HOME");;
