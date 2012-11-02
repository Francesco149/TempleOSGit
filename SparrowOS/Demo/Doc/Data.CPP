/*
  $DA	  is the data widget.
     -T  flag is for var
	 width fields with a terminator
	 character.
     +RD refreshes the data.
     +UD updates the val when you edit it.
     +U32 means the val is 4 bytes.
     -U64 U64 is the default so we might
	 want to turn-it-off.  Strings
	 use U64.
    ,32  sets the tag string width.
	 See [::/SparrowOS/Adam/Doc/DocPlain.CPP.Z,1] Data Tag Width.
     -P  Means it is a string var, basically.
*/



I32 i;
U8 buf[8];

U0 UpdateGlblTask(U64)
{
  while (TRUE) {
    i++;
    Sleep(1);
  }
}


U0 DataDemo()
{
  CDocEntry *doc_e;

//This is the command line document.
  CDoc *doc=DocCur;

//We do this to prevent access to
//doc_e->data before it is set.
  BoolI8 unlock=DocLock(doc);

//You might set the [C:/SparrowOS/OSMain/Adam1a.HPP.Z,1314] DOCF_FORM flag.
//  doc->flags|=DOCF_FORM
//if you wish.

  Spawn(&UpdateGlblTask,NULL,"Update Glbl",Fs);

  "Enter editor overstrike mode\n"
  "and you can modify the val.\n"
  "However, changes happen immediately,\n"
  "so it's tricky.\n\n";
  //Use <CTRL-L> for the $DA,$ format.
  doc_e=DocPutS(doc,"$DA-T+RD+UD-U64+U32,32,\"%12d\"$\n");
  doc_e->data=&i;

  StrCpy(buf,"Terry");
  doc_e=DocPutS(doc,"$DA-P+RD+UD,7,\"Str:%s\"$\n");
  doc_e->data=buf;

  if (unlock)
    DocUnlock(doc);
}

DataDemo;
