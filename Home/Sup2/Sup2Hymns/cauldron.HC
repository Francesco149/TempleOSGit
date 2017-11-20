//2 has words

+] Reward1
  CallExtStr("JukeReward",""
  +] Reward2
      
    "soul warranty consuming confined "
    "careless comprehended dreams african "
    "wandering disagreements to begotten "
    "provincial mountain regarding checked "
    "falsified corporeal familiar knottiness "
      
  );
  
U0 Song()
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      Play("5qFeDF4e.AsG5qEF4sA5GeG4qB5e.CsD",
	    "Sim\0mer\0ing \0world \0 \0brought \0"
	    "to \0a \0 \0 \0bo\0il.\n\0 \0");
      Play("qFeDF4e.AsG5qEF4sA5GeG4qB5e.CsD",
	    "Re\0lent\0ing \0heat \0 \0salt\0ing \0"
	    "the \0 \0 \0so\0il.\n\0 \0");
      Play("e.EsDqEe.CsDqEFDEC",
	    "Sod\0den \0flesh \0 \0 \0and \0still \0"
	    "vaunt \0Yup\0pies.\n\0");
      Play("eEDqEe.CsDqEFDEC",
	    "Glass \0ex\0press \0 \0 \0your \0world's \0"
	    "for \0gup\0pies.\n\0");
    }
  } catch
    PutExcept;
  Snd;
}

Song;
