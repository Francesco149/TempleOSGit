//5 has graphics

#define SUN_X	240
#define SUN_Y	40

F64 t0=Beat;

/* <1> <1> (image) */
 
U0 DrawIt(CTask *,CDC *dc)
{
  F64 theta=0,dt=Beat-t0;
  I64 x=100.0*Cos(dt*2*pi/8.0),y=30.0*Sin(-dt*2*pi/8.0);

  dc->color=BLACK;
  GrCircle(dc,SUN_X,SUN_Y,10);
  dc->color=YELLOW;
  GrFloodFill(dc,SUN_X,SUN_Y,FALSE);

  if (dt>16)
    theta=(dt-16)*2*pi;
  Sprite3ZB(dc,x+SUN_X-32,y+SUN_Y,0,__BIN_1,theta);
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      t0=Beat;
      Play("4qBetA5RFqER4GsBABAetG5GFsC4G5C4G",
	    "A \0hap\0 \0py \0God.\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4qBetA5RFqER4GsBABAetG5GFsC4G5C4G",
	    "Takes \0nap\0 \0py \0nod.\n\0 \0 \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4eB5FsDR6DR5qG4etA5E4RqBetB5GD4eAB5sRERE",
	    " \0World \0comes \0 \0un\0 \0done.\n\0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
      Play("4eB5FsDRDRqG4etA5E4RqBetB5GD4eAB5sRERE",
	    " \0Too \0bad \0 \0only \0 \0one!\n\0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
