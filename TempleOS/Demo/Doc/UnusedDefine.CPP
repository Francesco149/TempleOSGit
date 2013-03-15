/*
This scans the sym table and checks
each [C:/TempleOS/Kernel/Adam1a.HPP.Z,703] HTT_DEFINE_STR entry to see if
it only occurs once in files.

It's a brute force solution, but
gets the job done... slowly.

[C:/TempleOS/Adam/Grep.CPP.Z,163] Grep() returns a count of matches.

[C:/TempleOS/Adam/Grep.CPP.Z,193] FileOcc() is [C:/TempleOS/Adam/Grep.CPP.Z,163] Grep() with
output [C:/TempleOS/Kernel/Misc1a.CPP.Z,253] Silent().

*/

U0 UnusedDefineScan()
{
  CDoc *old_doc=DocCur,*doc;
  I64 cnt=0;
  CHashTable *tempht;
  CHash *temph;
  CDocEntry *doc_e,*doc_e2;
  Bool old_silent=IsSilent;

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
	  if (FileOcc(temph->str,"/TempleOS/"FILENAME_MASK_TXT,"+l-i")==1) {
	    doc=DocNew;
	    DocQueRem(old_doc);
	    DocQueIns(doc,Fs->last_doc);
	    Grep(temph->str,"/TempleOS/"FILENAME_MASK_TXT,"+l-i");
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
