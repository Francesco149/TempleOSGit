/*
  An [C:/SparrowOS/OSMain/Adam1a.HPP.Z,204] CDate is a signed 64-bit date/time num
  representing how many 49710ths of a second
  since Christ.  The reason it is this crazy
  num is so that 32bits is exactly one day.

  These nums can be subtracted to get time
  differences.
 
  The time of day to the nearest 49710ths of a second
  is stored in the lower 32-bits.  The upper
  32-bits hold the num of days since Christ.

  [C:/SparrowOS/OSMain/Str1a.CPP.Z,2620] PrintF() and the file datetimes use
  this format, otherwise you might want to
  use [C:/SparrowOS/OSMain/Adam1b.HPP.Z,198] GetTimeStamp() for a higher frequency
  timing, divide by [C:/SparrowOS/OSMain/OSGlbls.CPP.Z,46] time_stamp_freq,
  but this num is tied to your CPU's
  clock frequency which is inexact when
  converted to real-time.
*/


CDate ltdt=GetCurTimeCDate;

I64 i;
for (i=0;i<NUM_DATE_FMTS;i++)
  "%%h%dD: %h*D\n",i,i,ltdt;
NewLine;

for (i=0;i<NUM_TIME_FMTS;i++)
  "%%h%dT: %h*T\n",i,i,ltdt;

"$FG,PURPLE$CDate freq:%12.6f$FG$\n",0x100000000/24.0/60.0/60.0;

/*
  You can customize to your locality
  with [C:/SparrowOS/OSMain/OSGlbls.CPP.Z,132] date_fmt_map[] and [C:/SparrowOS/OSMain/OSGlbls.CPP.Z,133] time_fmt_map[].
  [C:/SparrowOS/OSMain/OSEnd.CPP.Z,1] DateTimeFmtMapsRst() will rst
  them.
*/

