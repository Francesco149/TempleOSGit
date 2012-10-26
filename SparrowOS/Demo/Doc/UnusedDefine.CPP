/*
This scans the sym table and checks
each [C:/SparrowOS/OSMain/Adam1a.HPP.Z,585] HTT_DEFINE_STR entry to see if
it only occurs once in files.

It's a brute force solution, but
gets the job done... slowly.

[C:/SparrowOS/Adam/Grep.CPP.Z,165] Grep() returns a count of matches.

[C:/SparrowOS/Adam/Grep.CPP.Z,190] FileOcc() is [C:/SparrowOS/Adam/Grep.CPP.Z,165] Grep() with
output [C:/SparrowOS/OSMain/Misc1a.CPP.Z,259] Silent().

*/

U0 UnusedDefineScan()
{
  CDoc *old_doc=DocCur,*doc;
  I64 cnt=0;
  CHashTable *tempht;
  CHash *temph;
  CDocEntry *doc_e,*doc_e2;
  BoolI8 old_silent=IsSilent;

  //Technically, we might want to
  //turn-off preemption in case the
  //table was modified.

  try {
    tempht=Fs->hash_table;
    while (tempht) {
      temph=tempht->next_added;
      while (temph!=tempht) {
	if (temph->type&HTT_DEFINE_STR)
	  cnt++;
	temph=temph->next_added;
      }
      tempht=tempht->next;
    }

    progress1=0;
    progress1_max=cnt;
    StrCpy(progress1_desc,"Define Scan");

    tempht=Fs->hash_table;
    while (tempht) {
      temph=tempht->next_added;
      while (temph!=tempht) {
	if (temph->type&HTT_DEFINE_STR) {
	  progress1++;
	  if (FileOcc(temph->str,"/SparrowOS/"TEXT_FILE_MASK,"+l-i")==1) {
	    doc=DocNew;
	    DocQueRem(old_doc);
	    DocQueIns(doc,Fs->last_doc);
	    Grep(temph->str,"/SparrowOS/"TEXT_FILE_MASK,"+l-i");
	    DocQueRem(doc);
	    DocQueIns(old_doc,Fs->last_doc);
	    doc_e=doc->root.next;
	    while (doc_e!=doc) {
	      if (doc_e->type_u8==DOCT_LINK) {
		NewLine;
		"" temph->str;
		doc_e2=DocEntryCopy(doc,doc_e);
		DocIns(old_doc,doc_e2);
		NewLine;
	      }
	      doc_e=doc_e->next;
	    }
	    DocDel(doc);
	  }
	}
	temph=temph->next_added;
      }
      tempht=tempht->next;
    }
  } catch
    CatchAll;

  Silent(old_silent);
  NewLine;
  ProgressBarsRst;
}

UnusedDefineScan;
