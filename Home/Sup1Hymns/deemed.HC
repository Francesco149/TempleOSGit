//5 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "cantilever's Huck's permanence's xerography "
    "discernible borrow foretells looseness "
    "civilizations Muawiya's sacrilege's xerography "
    "billy sedge's indication Hydra's "
    "discoing Susan august phlegm "
      
  );
  
/* <1> <1> (image) */









/* <2> <2> (image) */








F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=Beat-t0,tt=Saw(dt,37);
  U8 *tmps;
  tmps=SpriteInterpolate(tt,__BIN_1,__BIN_2);
  Sprite3(dc,200,5,0,tmps);
  Free(tmps);
}

U0 Song()
{
  CTask *task;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.714;
  try {
    task=PopUpViewPrint(
	"[C:/Misc/Bible.TXT.Z,98684] Revelation,5:1-9\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      t0=Beat;
	Play("5qFsGe.AeDGqDF6sC5e.GeFsEFqE",
		"Who \0can \0open?\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("M3/4");
	Play("FsEe.DeRM4/4",
		"Mind \0is \0jammed.\n\0 \0");
	Play("qFsGe.AetDGAqDF6sC5e.GeFsEFqE",
		"Got \0a \0Tolkien?\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("M3/4");
	Play("EsFe.GeRM4/4",
		"Jar \0is \0canned.\n\0 \0");
	Play("etCDC4eB5CqEeC4A5CD4sAGAG5qDG",
		"$RED$I \0 \0o\0pen.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("etCDC4eB5CqEeC4A5EDetGFGq.C",
		"Just \0 \0spok\0en.$FG$\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
