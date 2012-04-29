class NoiseFrame
{
  I64 duration;
  I64 min_freq,max_freq,waveform;
};

U0 NoiseEndTaskCB()
{
  Free(NamedFramePtr("NoiseFrame"));
  music_mute--;
  SndTaskEndCB;
}

U0 NoiseTask(NoiseFrame *ns)
{
  I64 i,f,timeout=GetTimeStamp+ns->duration*time_stamp_kHz_freq;
  NamedFramePtrAdd("NoiseFrame",ns);
  Fs->task_end_cb=&NoiseEndTaskCB;
  i=MaxI64(ns->max_freq-ns->min_freq,1);
  if (i<=0)
    i=1;
  while (GetTimeStamp<timeout) {
    if (f=RandU16%i+ns->min_freq) {
      Snd(f,ns->waveform);
      Sleep(MaxI64(3000/f,1));
    }
  }
}

public U0 Noise(I64 mS,I64 min_freq,I64 max_freq,I64 waveform=WF_NOISE)
{
  NoiseFrame *ns=MAlloc(sizeof(NoiseFrame));
  ns->duration=mS;
  ns->min_freq=min_freq;
  ns->max_freq=max_freq;
  ns->waveform=waveform;
  music_mute++;
  Spawn(&NoiseTask,ns,"Noise",Fs);
}
