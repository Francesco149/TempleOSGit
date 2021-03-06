//5 has graphics
/* <1> <1> (image) */




U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,w=task->pix_width,h=task->pix_height;
  dc->color=BROWN;
  for (i=60*Saw(Beat,20);i<w;i+=20)
    GrLine(dc,i,h,w,h-i/4);
  dc->color=BLACK;
  GrLine(dc,0,h-1,w,h-1);
  GrLine(dc,w-1,0,w-1,h);
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->text_attr=LTCYAN<<4+BLACK;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    task=PopUpViewPrint(
	  "[C:/Misc/Bible.TXT.Z,76908]\n"
	  "[C:/Misc/Bible.TXT.Z,81941]\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      "$CL$$BG,LTCYAN$";
      Play("5qEsGEGEeF4GB5CqCeDE4qB5sERER",
	    "$FG$Lord, \0I \0 \0am \0 \0feel\0ing \0"
	    "just \0a \0bit \0un\0in\0spired.\n\0With \0 \0 \0 \0");
      Play("5qEsGEGEeF4GB5CqCeDE4qB5sERER",
	    "net \0or \0 \0with \0 \0reel\0ing \0 \0"
	    "I \0keep \0get\0ting \0mired.\n\0 \0 \0 \0 \0");
      Play("5qD4etRA5FqDGetFC4GsB5C4B5CqDF",
	    "$RED$Throw \0 \0out \0your \0net.\n\0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0");
      Sprite(__BIN_1);
      Play("5D4etRA5FqDGetFC4GsB5C4B5CqDF",
	    "Make \0 \0you \0a \0bet.\n\0 \0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0");
      '\n\n\n\n\n';
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
