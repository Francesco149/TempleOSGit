U0 PortNop()
{
  InU8(0x21); //This is an innoculous (reads IRQ Mask)
	      //access which should take
	      //fixed time because it's an ISA-bus standard.
	      //It takes 1.0uS-2.0uS
}

U16 EndianU16(U16 d)
{
  result.u8[1]=d.u8[0];
  result.u8[0]=d.u8[1];
}

U32 EndianU32(U32 d)
{
  result.u8[3]=d.u8[0];
  result.u8[2]=d.u8[1];
  result.u8[1]=d.u8[2];
  result.u8[0]=d.u8[3];
}

U64 EndianU64(U64 d)
{
  result.u8[7]=d.u8[0];
  result.u8[6]=d.u8[1];
  result.u8[5]=d.u8[2];
  result.u8[4]=d.u8[3];
  result.u8[3]=d.u8[4];
  result.u8[2]=d.u8[5];
  result.u8[1]=d.u8[6];
  result.u8[0]=d.u8[7];
}

I64 BCnt(U64 d)
{
  I64 result=0,i;
  for (i=0;i<8;i++)
    result+=set_bits_table[d.u8[i]];
}

U0 IntSimpleTimer(CTask *)
{
  I64 i;
  if (mp_cnt>1)
    while (LBts(&sys_semas[SYS_SEMA_SYS_TIMER],0))
      Pause;
  lock sys_jiffies++;
  sys_timer+=SYS_TIMER0_PERIOD+1;
  LBtr(&sys_semas[SYS_SEMA_SYS_TIMER],0);
  for (i=1;i<mp_cnt;i++)
    MPInt(I_TIMER,i);
}

U64 SysTimerRead()
{
  I64 i;
  PushFD;
  Cli;
  if (mp_cnt>1)
    while (LBts(&sys_semas[SYS_SEMA_SYS_TIMER],0))
      Pause;
  OutU8(0x43,0); //Latch Timer0
  if ((i=InU8(0x40)+InU8(0x40)<<8)==SYS_TIMER0_PERIOD) {
    if (InU8(0x20) & 1)
      i=-1;
  }
  result=sys_timer+SYS_TIMER0_PERIOD-i;
  LBtr(&sys_semas[SYS_SEMA_SYS_TIMER],0);
  PopFD;
}

U64 TaskTotalTime()
{
  return GetTimeStamp-Fs->time_slice_start+Fs->total_time;
}

U64 TimeStampFreqCal()
{
  static U64 time_stamp_start=0,timer_start=0,HPET_start=0;
  U64 i;
  if (time_stamp_start) {
    PushFD;
    Cli;
    if (HPET_start) {
      time_stamp_freq=HPET_freq*(GetTimeStamp-time_stamp_start);
      i=HPET-HPET_start;
    } else {
      time_stamp_freq=SYS_TIMER_FREQ*(GetTimeStamp-time_stamp_start);
      i=SysTimerRead-timer_start;
    }
    if (!i)
      Dbg("Timer Cal Error");
    time_stamp_freq/=i;
    time_stamp_kHz_freq=time_stamp_freq/1000;
    PopFD;
    time_stamp_freq_calibrated=TRUE;
  }
  PushFD;
  Cli;
  if (HPET_freq) {
    timer_start=0;
    HPET_start=HPET;
  } else {
    HPET_start=0;
    timer_start=SysTimerRead;
  }
  time_stamp_start=GetTimeStamp;
  PopFD;
  return time_stamp_freq;
}

F64 tNP()
//Time since boot in seconds as a float
{
  static U64 last_time_stamp=0;
  static F64 last_result=0;
  U64 time_stamp;
  if (HPET_freq)
    return ToF64(HPET)/HPET_freq;
  else { //Monotonically incresing,calibrated.
    PushFD;
    Cli;
    if (mp_cnt>1)
      while (LBts(&sys_semas[SYS_SEMA_TNP],0))
	Pause;
    time_stamp=GetTimeStamp;
    last_result+=ToF64(time_stamp-last_time_stamp)/time_stamp_freq;
    last_time_stamp=time_stamp;
    LBtr(&sys_semas[SYS_SEMA_TNP],0);
    PopFD;
    return last_result;
  }
}

F64 tP(CTask *task=NULL)
{ //Time in seconds, stopped during pauses
  F64 result;
  BoolI8 old_preempt=Preempt(OFF);
  if (!task) task=Fs;
  if (TaskValidate(task)) {
    if (Bt(&task->task_flags,TASKf_PAUSED))
      result=task->pause_start-task->pause_total;
    else
      result=tNP-task->pause_total;
    Preempt(old_preempt);
    return result;
  }
  Preempt(old_preempt);
  return tNP;
}

F64 tJ()
{
  return sys_jiffies/ToF64(JIFFY_FREQ);
}

F64 tS()
{
  return SysTimerRead/ToF64(SYS_TIMER_FREQ);
}

BoolI64 Blink(F64 Hz=2.5,F64 t=0)
{
  if (!Hz) return 0;
  if (t)
    return ToI64(t*2*Hz)&1;
  else
    return ToI64(sys_jiffies*2*Hz/JIFFY_FREQ)&1;
}

U0 BusyWait(U64 uS) //Loosely timed
{
  U64 i;
  for (i=0;i<uS;i++)
    PortNop;
}

U0 SleepUntil(U64 wake_jiffy)
//This routine is not for power-saving
//it is to make a program pause without
//hogging the CCPU.
{
  BoolI8 old_idle=LBts(&Fs->task_flags,TASKf_IDLE);
  Fs->wake_jiffy=wake_jiffy;
  Yield;
  LBEqu(&Fs->task_flags,TASKf_IDLE,old_idle);
}

U0 Sleep(U64 mS)
//This routine is not for power-saving
//it is to make a program pause without
//hogging the CCPU.
{
  if (!mS)
    Yield;
  else
    SleepUntil(sys_jiffies+mS*JIFFY_FREQ/1000);
}

U0 Snd(F64 freq,U64 waveform=WF_SQUARE,F64 amp=1.0)
// The internal PC speaker only does square waves.
// waveform and amp are for HDAudio.
// Use [C:/SparrowOS/Adam/Snd/SndFile.CPP.Z,264] SndFilePlay() or [::/SparrowOS/Demo/Snd/PolyPhonic.CPP.Z,1] ::/SparrowOS/Demo/Snd/PolyPhonic.CPP.Z as a template for
// doing polyphonic or fancy HD Audio snd.
{
  U64 p;
  if (!Bt(&sys_semas[SYS_SEMA_MUTE],0) && snd_vol &&
      !LBts(&sys_semas[SYS_SEMA_SND],0)) {
    if (!IsInDbg && snd_dev!=SD_PC_SPEAKER) {
      snd_freq=freq;
      if (fp_snd)
	(*fp_snd)(freq,waveform,amp);
    } else {
      if (freq!=snd_freq) {
	snd_freq=freq;
	if (freq) {
	  p=SYS_TIMER_FREQ/freq;
	  OutU8(0x43,0xB6);
	  OutU8(0x42,p);
	  OutU8(0x42,p.u8[1]);
	  OutU8(0x61,3|InU8(0x61));
	} else
	  OutU8(0x61,InU8(0x61)&~3);
      }
    }
    if (!IsInDbg && snd_record && fp_snd_record)
      (*fp_snd_record)(freq,waveform,amp);
    LBtr(&sys_semas[SYS_SEMA_SND],0);
  }
}

U0 Beep(U64 freq=500,BoolI8 busy=FALSE)
{
  Snd(freq);
  if (busy) BusyWait(500000);
    else Sleep(500);
  Snd(0);
  if (busy) BusyWait(200000);
    else Sleep(200);
}

BoolI64 Mute(BoolI8 val)
{
  if (val) {
    PushFD;
    Cli;
    Snd(0);
    result=LBts(&sys_semas[SYS_SEMA_MUTE],0);
    PopFD;
  } else
    result=LBtr(&sys_semas[SYS_SEMA_MUTE],0);
}

BoolI64 IsMute() {
  return Bt(&sys_semas[SYS_SEMA_MUTE],0);
}

BoolI64 Silent(BoolI8 val) {
//This turns-off console output, not sound.
  return LBEqu(&Fs->display_flags,DISPLAYf_SILENT,val);
}

BoolI64 IsSilent() {
  return Bt(&Fs->display_flags,DISPLAYf_SILENT);
}

BoolI64 IOSnd(BoolI8 val) {
  result=!LBEqu(&sys_semas[SYS_SEMA_NO_IO_SND],0,!val);
  Snd(0);
}

BoolI64 IsPreempt() {
  return Bt(&Fs->task_flags,TASKf_PREEMPT);
}

BoolI64 SysDbg(BoolI8 val) {
//Sets or rsts a bit which you can
//use while debugging.
  return LBEqu(&sys_semas[SYS_SEMA_DEBUG],0,val);
}

BoolI64 IsSysDbg() {
  return Bt(&sys_semas[SYS_SEMA_DEBUG],0);
}

BoolI64 SysDeterministic(BoolI8 val) {
//Sets or rsts a bit which tries to keep the system deterministic, for debugging.
  return LBEqu(&sys_semas[SYS_SEMA_DETERMINISTIC],0,val);
}

BoolI64 IsSysDeterministic() {
  return Bt(&sys_semas[SYS_SEMA_DETERMINISTIC],0);
}

BoolI64 Raw(BoolI8 val) {
//Switches to direct screen output
//mode for when you don't have
//a working window.
  if (!val)
    VGAFlush;
  return !LBEqu(&Fs->task_flags,TASKf_NOT_RAW,!val);
}

BoolI64 IsRaw() {
  return !Bt(&Fs->task_flags,TASKf_NOT_RAW);
}

BoolI64 SingleUser(BoolI8 val) {
  return LBEqu(&sys_semas[SYS_SEMA_SINGLE_USER],0,val);
}

BoolI64 IsSingleUser() {
  return Bt(&sys_semas[SYS_SEMA_SINGLE_USER],0);
}

BoolI64 InDbg(BoolI8 val) {
  return LBEqu(&sys_semas[SYS_SEMA_IN_DEBUGGER],0,val);
}

BoolI64 IsInDbg() {
  return Bt(&sys_semas[SYS_SEMA_IN_DEBUGGER],0);
}

U0 ProgressBarsRst()
{
  MemSet(sys_progresses,0,sizeof(sys_progresses));
}

U0 CtrlAltDel(U64)
{
  LBts(sys_ctrl_alt_flags,SYSf_CTRL_ALT_DEL);
}

U0 CtrlAltC(U64)
{
  LBts(sys_ctrl_alt_flags,SYSf_CTRL_ALT_C);
}

U0 CtrlAltE(U64)
{
  SpawnUser;
}

U0 CtrlAltF(U64)
{
  VGAFlush;
}

U0 CtrlAltM(U64)
{
  Mute(!IsMute);
}

U0 CtrlAltP(U64)
{
  if (sys_focus_task)
    TaskPause(sys_focus_task,!Bt(&sys_focus_task->task_flags,TASKf_PAUSED));
}

U0 CtrlAltT(U64)
{
  LBts(sys_ctrl_alt_flags,SYSf_CTRL_ALT_TAB);
}

U0 CtrlAltX(U64)
{
  LBts(sys_ctrl_alt_flags,SYSf_CTRL_ALT_X);
}
