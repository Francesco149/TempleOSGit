//7 has graphics



      /* <1> <1> (image) */



      /* <2> <2> (image) */


F64 t0=Beat;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 w=task->pix_width,h=task->pix_height;
  F64 t=(Beat-t0-16.5)/2,
      x=w*Saw(t,2.0),
      y=h*Sqr((x-w>>1)/w>>1),
      dx=w/2,
      dy=h*2*((x-w>>1)/w>>1),
      theta=Arg(dx,dy);
  if (0<t<8) {
    SpriteZ3B(dc,x,y,0,__BIN_3,0.3*Sin(30*t)+theta);
    SpriteZ3B(dc,x,y+20*FullSaw(30*t,0.5),0,__BIN_4,0.3*Sin(t)+theta);
  }
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.0;
  music.stacatto_factor= 0.902;
  try {
  while (!ScanKey) {
	t0=Beat;
	Play("M9/8");
	Play("3qAeCBAq.CsAAqAeCBA2q.G3sBC");
	Play("qAeDCAhBeFsEEeDq.CM4/4");
	Play("sAFAFeF2GsG3B2G3BqGeAsAAetEGDE2G3DeGsGG");
	Play("AFAFeF2GsG3B2G3BqGeAsAAetEGDE2G3DeGsGG");
  }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
