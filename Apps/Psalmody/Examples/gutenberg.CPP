//8 has graphics




/* <1> <1> (image) */

/* <2> <2> (image) */

F64 t0=Beat;

U0 DrawIt(CTask *,CDC *dc)
{
  F64 theta,dt=Beat-t0;
  if (dt>12) dt=12;
  theta=0.2*FullTri(dt,4);
  SpriteZ3B(dc,140+30*Sin(dt),40,0,__BIN_1,theta);
  Sprite3(dc,170,15,0,__BIN_2);
}

U0 Song()
{
  CTask *task;
  Fs->task_end_cb=&SndTaskEndCB;
  Fs->draw_it=&DrawIt;
  MusicSettingsRst;
  music.tempo= 2.500;
  music.stacatto_factor= 0.900;
  try {
    task=PopUpViewPrint("[C:/Misc/Bible.TXT.Z,98684] Revelation,22:18\n");
    TaskWait(task);
    SetWinHorz(Fs->win_left,Fs->win_right,task);
    SetWinVert(Fs->win_bottom+3,Fs->win_bottom+15,task);
    "$WW+H,1$";
    while (!ScanKey) {
      t0=Beat;
      Play("3qBRE2sG3C2G3CeCFqEeGAsR2GRG",
	    "The \0 \0quill\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("3qBRE2sG3C2G3CeCFqEeGAsR2GRG",
	    "is \0 \0still\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("3qERetACGqF2G3etD2G3FBF2G3DRF",
	    " \0 \0God's \0 \0 \0word \0for \0all \0 \0his \0 \0hap\0 \0py \0 \0peo\0");
      Play("qERetACGqF2G3etD2G3FBF2G3DRF",
	    "ple.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      "$GREEN$Gutenberg invented the printing press and made Bibles.  Until then, "
	    "I wonder how many man-hours for one word in the Bible!\n$FG$";
    }
  } catch
    PutExcept;
  Snd(0);
}

Song;
