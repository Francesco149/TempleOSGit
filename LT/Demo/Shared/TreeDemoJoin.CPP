U0 TreeSub(Ltf *l,LTDirEntry *tempm)
{
  LTDirEntry *tempm1;
  while (tempm) {
    tempm1=tempm->next;
    if (tempm->attr & LT_ATTR_DIR) {
      LtfPutS(l,"$TR,\"\"$");
      LtfPrintF(l,"$MA+A,\"%s\",\"Cd(\\\"%s\\\");Dir;\n\"$\n",
      tempm->name,tempm->full_name);
      if (tempm->sub) {
	LtfPutS(l,"$ID,+2$");
	TreeSub(l,tempm->sub);
	LtfPutS(l,"$ID,-2$");
      }
    } else {
      LtfPrintF(l,"$LK,\"%s\",\"FI:%s\"$\n",
	tempm->name,tempm->full_name);
    }
    //Note there is also a routine
    //to delete an entire LTDirEntry tree.
    //See [C:/LT/OSMain/Dsk/DskDir.CPZ,34] LTDirListDel().
    LTDirEntryDel(tempm);
    tempm=tempm1;
  }
}
