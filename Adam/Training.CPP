#help_index "Help System"

public U0 PressKeyHelp(CTask *task=NULL)
{//Jump to key handlers src for key presses.
  I64 a1=0,a2=0;
  Bool old_preempt=Preempt;
  CTask *old_hash=Fs->hash_table;
  if (!task) task=Fs;

    /*
This feature is a burden in code.  It would be nice to lst key
cmds for all applications, but it makes the code too ugly.
*/
  task=Fs;

  Fs->hash_table=task->hash_table;
  "Press keys to show handler code.\n"
	"<SHIFT-ESC> to exit.\n"
	"Do not press <CTRL-ALT-keys>.\n";
  while (a1!=CH_ESC && a1!=CH_SHIFT_ESC) {
    GetMsg(&a1,&a2,1<<MSG_KEY_DOWN);
    *keydev.desc=0;
    keydev.handler=NULL;
    if (task==Fs)
      PutKey(a1,a2|SCF_KEY_DESC);
    else if (TaskValidate(task))
      PostMsg(task,MSG_KEY_DOWN,a1,a2|SCF_KEY_DESC);
    WinMgrSync(0,TRUE);
    Yield;
    if (*keydev.desc)
      E(keydev.handler,512);
  }
  Fs->hash_table=old_hash;
  Preempt(old_preempt);
}

U8 *KeyMapKeyMStrPrint(I64 sc,U0 (*fp_handler)(I64 sc),
	U8 *desc,CTask *task=NULL)
{
  I64 i=9;
  U8 *st,*st2,*result,*ptr;
  CHashTable *old_hash=Fs->hash_table;
  st=ScanCode2KeyName(sc);
  if (sc&SCF_CTRL)	i+=5;
  if (sc&SCF_ALT)	i+=4;
  if (sc&(SCF_SHIFT|SCF_NO_SHIFT)) i+=6;
  if (TaskValidate(task))
    Fs->hash_table=task->hash_table;
  st2=SrcEdLink(fp_handler,256);
  Fs->hash_table=old_hash;
  result=MStrPrint("%-*s $LK+PU,\"%Q\",A=\"%s\"$\n",i,st,desc,st2);
  Free(st);
  Free(st2);

  ptr=result;
  while (*ptr) {
    if (*ptr==CH_SPACE)
      *ptr=CH_SHIFT_SPACE;
    ptr++;
  }

  return result;
}

U0 KeyMapKeyPrint(I64 sc,U0 (*fp_handler)(I64 sc),U8 *desc,CTask *task=NULL)
{
  U8 *st=KeyMapKeyMStrPrint(sc,fp_handler,desc,task);
  "%s",st;
  Free(st);
}

U0 KeyMapCtrlAltFamily(Bool no_shift,Bool shift)
{
  I64 i,no_shift_f;
  if (no_shift && shift)
    no_shift_f=SCF_NO_SHIFT;
  else
    no_shift_f=0;
  if (no_shift) KeyMapKeyPrint(SC_DELETE+SCF_CTRL+SCF_ALT+no_shift_f,
	  &Reboot,"Sys   /Reboot");
  if (shift) KeyMapKeyPrint(SC_DELETE+SCF_CTRL+SCF_ALT+SCF_SHIFT,
	  &Reboot,"Sys   /Reboot");
  if (no_shift) KeyMapKeyPrint(SC_ESC+SCF_CTRL+SCF_ALT+no_shift_f,
	  &User,"Sys   /Spawn New User Task");
  if (shift) KeyMapKeyPrint(SC_ESC+SCF_CTRL+SCF_ALT+SCF_SHIFT,
	  &User,"Sys   /Spawn New User Task");
  if (no_shift) KeyMapKeyPrint(SC_TAB+SCF_CTRL+SCF_ALT+no_shift_f,
	  &WinToTop,"Sys   /Switch Focus to Next Task");
  if (shift) KeyMapKeyPrint(SC_TAB+SCF_CTRL+SCF_ALT+SCF_SHIFT,
	  &WinToTop,"Sys   /Switch Focus to Next Task");

  for (i=0;i<26;i++)
    if (keydev.fp_ctrl_alt_let_cbs[i]) {
      if (no_shift && keydev.ctrl_alt_let_no_shift_descs[i])
	KeyMapKeyPrint(Char2ScanCode(i+'a')+SCF_CTRL+SCF_ALT+no_shift_f,
	      keydev.fp_ctrl_alt_let_cbs[i],
	      keydev.ctrl_alt_let_no_shift_descs[i]);
      if (shift && keydev.ctrl_alt_let_shift_descs[i])
	KeyMapKeyPrint(Char2ScanCode(i+'a')+SCF_CTRL+SCF_ALT+SCF_SHIFT,
	      keydev.fp_ctrl_alt_let_cbs[i],
	      keydev.ctrl_alt_let_shift_descs[i]);
    }
}

U0 KMComparePrepare(U8 *buf,I64 *src)
{
  I64 i,*dst=buf;
  U8 *ptr;
  if (src) {
    *dst++=*src++;
    *dst++=*src++;
    *dst++=*src++;
    *dst++=*src++;
    *dst(U8 *)=0;
    if (ptr=StrMatch("SHIFT",buf)) {
      for (i=0;i<5;i++)
	ptr[i]=CH_SHIFT_SPACE;
      if (ptr=StrMatch("$",buf))
	*ptr=255;
    }
  } else
    *buf=0;
}

I64 KMCompare(U8 *e1,U8 *e2)
{
  U8 buf1[STR_LEN],buf2[STR_LEN];
  KMComparePrepare(buf1,e1);
  KMComparePrepare(buf2,e2);
  return StrCmp(buf1,buf2);
}

U0 KeyMapFamily2(U8 **entries,CTask *task,I64 scf)
{
  I64 i,a1,a2;
  for (i=0;i<256;i++) {
    a2=scf|i|SCF_KEY_DESC;
    a1=ScanCode2Char(a2);
    *keydev.desc=0;
    keydev.handler=NULL;
    if (TaskValidate(task) && !Bt(&task->win_inhibit,WIf_SELF_KEY_DESC)) {
      if (task==Fs)
	PutKey(a1,a2);
      else
	PostMsg(task,MSG_KEY_DOWN,a1,a2);
      WinMgrSync(0,TRUE);
      Yield;
    }
    if (*keydev.desc && StrNCmp(keydev.desc,"Char  /",7))
      entries[i]=KeyMapKeyMStrPrint(a2,keydev.handler,keydev.desc,task);
  }
}

U0 KeyMapFamily(CTask *task,I64 scf,Bool no_shift,Bool shift)
{
  I64 i,cnt=0;
  U8 **entries=CAlloc(2*256*sizeof(U8 *)),**ptr=entries;
  if (no_shift) {
    if (shift)
      KeyMapFamily2(ptr,task,scf+SCF_NO_SHIFT);
    else
      KeyMapFamily2(ptr,task,scf);
    ptr+=256;
    cnt+=256;
  }
  if (shift) {
    KeyMapFamily2(ptr,task,scf+SCF_SHIFT);
    ptr+=256;
    cnt+=256;
  }
  QSortI64(entries,cnt,&KMCompare);
  for (i=0;i<cnt;i++)
    if (entries[i]) {
      "%s",entries[i];
      Free(entries[i]);
    }
  Free(entries);
}

public U0 KeyMap(CTask *task=NULL)
{//Report desc of all keys.
  I64 old_win_inhibit;
  if (!task) task=Fs;
  old_win_inhibit=task->win_inhibit;
  task->win_inhibit&=~WIF_SELF_KEY_DESC;
  DocMax;
  KeyMapFamily(task,0,TRUE,TRUE);
  KeyMapFamily(task,SCF_CTRL,TRUE,TRUE);
  KeyMapFamily(task,SCF_ALT,TRUE,TRUE);
  KeyMapCtrlAltFamily(TRUE,TRUE);
  "\n$MA-X+PU,\"Click for help on key pressed\","
	"LM=\"PressKeyHelp(0x%X);\"$\n",task;
  task->win_inhibit=old_win_inhibit;
}

#help_index "Help System/Training"
public U0 TipOfDay(U8 *tip_file="::/Doc/Tips.TXT")
{//Print random tip-of-day from ::/Doc/Tips.TXT.
  I64 i=RandU16;
  CDoc *doc=DocRead(tip_file),*doc2=DocNew;
  CDocEntry *doc_e=doc->root.next;
  "$WW,1$\n";
  while (TRUE) {
    if (doc_e->type_u8==DOCT_TEXT && *doc_e->tag=='*')
      if (!i--) break;
    doc_e=doc_e->next;
  }
  if (doc_e->type_u8==DOCT_TEXT && *doc_e->tag=='*') {
    while (doc_e!=doc) {
      if (doc_e->type_u8!=DOCT_ERROR)
	DocInsEntry(doc2,DocEntryCopy(doc2,doc_e));
      doc_e=doc_e->next;
      if (doc_e->type_u8==DOCT_TEXT && *doc_e->tag=='*')
	break;
    }
  }
  DocInsDoc(DocPut,doc2);
  DocDel(doc2);
  DocDel(doc);
}
