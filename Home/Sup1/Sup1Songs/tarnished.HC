//5 has graphics

F64 t0=Beat;

#define	LOWER	15
#define UPPER	18
#define CENTERX	230
#define CENTERY	38

U0 DrawIt(CTask *,CDC *dc)
{
  F64 dt=ToI64(Beat-t0),theta;
  if (dt<32)
    theta=2*pi*(5-dt)/32;
  else
    theta=2*pi*(5-32)/32;
  dc->pen_width=3;
  dc->color=BLUE;
  GrLine3(dc,CENTERX,CENTERY,0,CENTERX-LOWER*Sin(theta),CENTERY-LOWER*Cos(theta),0);
  GrLine3(dc,CENTERX-LOWER*Sin(theta),CENTERY-LOWER*Cos(theta),0,
	CENTERX-LOWER*Sin(theta)+UPPER*Sin(theta),CENTERY-LOWER*Cos(theta)-UPPER*Cos(theta),0);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("4eG5EsCD4eBA5C4GAsAAeBABAqGeB",
	    " \0Can \0I \0 \0bum \0a \0smoke?\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4G5EsCD4eBA5C4GAsAAeBABAqGeB",
	    " \0Pol\0ish\0 \0ing \0 \0stroke.\n\0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("5GACD4AB5qG4eB5FGFGE4qB",
	    "Chrom\0i\0um \0 \0 \0 \0dome.\n\0 \0 \0 \0 \0 \0 \0 \0");
      Play("5eGACD4AB5qG4eB5FGFGE4B5C",
	    "Sol\0id \0 \0 \0 \0 \0bone.\n\0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4B5C4B");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
