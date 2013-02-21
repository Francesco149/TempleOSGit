//8 special
U8 *bible;
I64 size;

U0 TaskEndCB()
{
  Snd(0);
  Free(bible);
  Exit;
}

#define LEN 256

U0 PutVerse()
{
  U8 *ch;
  I64 *start;

  "$CL$";
  start=GetTimeStamp/16%size;
  if (start>size-LEN) start=size-LEN;
  ch=bible[start+LEN-1];
  bible[start+LEN-1]=0;
  "" bible+start;
  bible[start+LEN-1]=ch;
}

U0 Song()
{
  CTask *task;
  U8 *bible_filename=Define("ST_BIBLE_FILE");
  Fs->task_end_cb=&TaskEndCB;
  music_tempo=2.5;
  bible=FileRead(bible_filename,&size);
  "$WW+H,1$";
  try {
  task=PopUpViewStr(
    "$WW,1$This shows a random passage based on "
    "the time at the moment this song was run.  "
    "Talk to God as an offering, then run AfterEgypt
`Cd("::/SparrowOS/Apps/AfterEgypt");;#include "Run"\n`
 if you "
    "want God to talk some more.  The [::/SparrowOS/Apps/HolySpirit/HSNotes.TXT.Z,1] Holy Spirit can "
    "puppet you.\n");
  TaskWait(task);
  task->win_right=Fs->win_right;
  task->win_left=Fs->win_left;
  task->win_top=Fs->win_bottom+3;
  task->win_bottom=task->win_top+12;
  TaskDerivedValsUpdate(task);
  DocRecalc(DocCur(task)); //Reformat word-wrap with new window size.
  while (!ScanKey) {
	PutVerse;
	Play("3qAeABCBqCetABBqCetAAREE2G");
	Play("3qAeABCBqCetABBqCetAAREE2G");
	Play("3eDRRRqGReRRGGGGqE");
	Play("eDRRRqGReRRGGGGqE");
  }
  } catch
    CatchAll;
  Snd(0);
  Free(bible);
}

Song;
