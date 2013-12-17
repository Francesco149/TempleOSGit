//This is the fastest you can update the whole screen.
//See [C:/Demo/Lectures/MiniGrLib.CPP,1] if this interests you.

F64 VGAPattern(I64 p)
{//This returns the time to update in seconds.
  F64 start=tT;
  I64 i,plane,*ptr;
  for (plane=1;plane<0x10;plane<<=1) {
    OutU8(VGA_SC_INDEX,VGA_MAP_MASK);
    OutU8(VGA_SC_DATA,plane);
    ptr=0xA0000;
    for (i=0;i<640*480/64;i++)
      *ptr++=p;
  }
  return tT-start;
}
 
 
U0 GrDemo()
{
  I64 i;
  F64 t;
  Bool old_preempt=Preempt(OFF);
  CPURep;
  for (i=0;i<16;i++) {
    t=VGAPattern(0x5555555555555555);
    "$GREEN$Rate:$RED$%5.1f FPS$FG$"
    "  $GREEN$Load@60Hz:$RED$%5.1f%%$FG$\n",1/t,100*t*60.0;
    t=VGAPattern(0x3333333333333333);
    "$GREEN$Rate:$RED$%5.1f FPS$FG$"
    "  $GREEN$Load@60Hz:$RED$%5.1f%%$FG$\n",1/t,100*t*60.0;
  }
  Preempt(old_preempt);
//<CTRL-ALT-F> will flush screen VGA cache.
  VGAFlush;
}

GrDemo;

/*  Program Output (Not VMware)

8 Cores 2.660GHz
Rate:369.7 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:370.3 FPS	Load@60Hz: 16.2%
Rate:361.2 FPS	Load@60Hz: 16.6%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:370.2 FPS	Load@60Hz: 16.2%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:360.6 FPS	Load@60Hz: 16.6%
Rate:370.4 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:353.1 FPS	Load@60Hz: 17.0%
Rate:370.3 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:357.0 FPS	Load@60Hz: 16.8%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:370.4 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:353.9 FPS	Load@60Hz: 17.0%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:370.2 FPS	Load@60Hz: 16.2%
Rate:370.4 FPS	Load@60Hz: 16.2%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:369.7 FPS	Load@60Hz: 16.2%
Rate:369.9 FPS	Load@60Hz: 16.2%
Rate:369.8 FPS	Load@60Hz: 16.2%

*/
