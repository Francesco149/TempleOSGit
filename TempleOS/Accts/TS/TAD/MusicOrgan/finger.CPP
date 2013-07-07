//8 has graphics





/* <1> <1> (image) */



/* <2> <2> (image) */


Bool zap_on,finger_on;

U0 DrawIt(CTask *,CDC *dc)
{
  if (finger_on)
    SpritePlot3(dc,20*8,3*8,0,__BIN_1);
  if (zap_on)
    SpritePlot3(dc,20*8,7*8,0,__BIN_2);
}


U0 ZapTask(I64)
{
  finger_on=TRUE;
  Sleep(100);
  zap_on=TRUE;
  Noise(100,3000,4000);
  Sleep(100);
  zap_on=FALSE;
  Sleep(100);
  zap_on=TRUE;
  Noise(100,3000,4000);
  Sleep(100);
  zap_on=FALSE;
  Sleep(500);
  finger_on=FALSE;
}

U0 Song()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;
  music_tempo=2.5;
  finger_on=zap_on=FALSE;
  try {
  while (!ScanKey) {
	Play("3qEsCBCBqARReGRqA2sG3G2G3G",
		"Fin\0ger \0 \0of \0 \0God\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("qEsCBCBqARReGRqA2sG3G2G3G",
		"Light\0en \0 \0the \0 \0rod\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Play("qFetEA2GqGetG3B2G3sFDFDqA2eG3GqD",
		"All \0of \0this \0strife\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
	Spawn(&ZapTask);
	Play("FetEA2GqGetG3B2G3sFDFDqA2eG3GqD",
		"Jump \0start \0my \0life\n\0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0 \0");
  }
  } catch
    CatchAll;
  DocClear;
  SettingsPop;
}

Song;
