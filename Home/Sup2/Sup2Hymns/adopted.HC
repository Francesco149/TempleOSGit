//2 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "reek servility Brest thermal "
    "Agni's marabous aborigine's apportions "
    "clues Nova constraints inoculates "
    "Wilda's dismayed shoehorn's Mekong "
    "summarize mores's Ebola ferrying "
      
  );
  
U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    task=PopUpViewPrint(
	"[C:/Misc/Bible.TXT.Z,5068] Exodus,2:5-10\n");
    TaskWait(task);
    WinHorz(Fs->win_left,TEXT_COLS-10,task);
    WinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    while (!ScanKey) {
      Play("4qB5eCCqDeDDqECD4B",
	    "Park \0the \0 \0boat\n\0 \0 \0 \0 \0 \0 \0");
      Play("5eC4sAGeABGAFAGGqGhG",
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0Drive \0it \0 \0home\n\0");
      Play("qB5eCEqDeDsCDqECD4B",
	    "Rams \0and \0 \0goat\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("5eC4sAGeABGAFAGsGFeGGhG",
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0To \0 \0 \0the \0 \0bone\n\0");
      Play("5eGGAAGAqFGDsEeDsDqD",
	    " \0 \0 \0 \0 \0 \0 \0Voice \0my \0soul\n\0 \0 \0 \0");
      Play("M3/4");
      Play("GeFFGAM4/4");
      Play("qtFDChD#M4/4","Cho\0ic\0est \0coal\n\0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
