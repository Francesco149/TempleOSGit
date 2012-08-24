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
	 See [::/LT/Adam/Ltf/LtfPlain.APZ,DataTagWidth] Data Tag Width.
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
  LtfEntry *ll;

//This is the command line document.
  Ltf *l=LtfCur;

//We do this to prevent access to
//ll->data before it is set.
  BoolI8 unlock=LtfLock(l);

//You might set the [C:/LT/OSMain/Adam1a.HPZ,1288] LTFF_FORM flag.
//  l->flags|=LTFF_FORM
//if you wish.

  Spawn(&UpdateGlblTask,NULL,"Update Glbl",Fs);

  "Enter editor overstrike mode\n"
  "and you can modify the val.\n"
  "However, changes happen immediately,\n"
  "so it's tricky.\n\n";
  //Use <CTRL-L> for the $DA,$ format.
  ll=LtfPutS(l,"$DA-T+RD+UD-U64+U32,32,\"%12d\"$\n");
  ll->data=&i;

  StrCpy(buf,"Terry");
  ll=LtfPutS(l,"$DA-P+RD+UD,7,\"Str:%s\"$\n");
  ll->data=buf;

  if (unlock)
    LtfUnlock(l);
}

DataDemo;
