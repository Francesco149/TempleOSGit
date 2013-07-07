/*
  An [C:/TempleOS/Kernel/Adam1a.HPP.Z,286] CDate is a signed 64-bit date/time num
  representing how many 49710ths of a second
  since Christ.  The reason it is this crazy
  num is so that 32bits is exactly one day.

  These nums can be subtracted to get time
  differences.
 
  The time of day to the nearest 49710ths of a second
  is stored in the lower 32-bits.  The upper
  32-bits hold the num of days since Christ.

  [C:/TempleOS/Kernel/Str1a.CPP.Z,2351] PrintF() and the file datetimes use
  this format, otherwise you might want to
  use [C:/TempleOS/Kernel/Adam1b.HPP.Z,199] GetTimeStamp() for a higher frequency
  timing, divide by [C:/TempleOS/Kernel/KGlbls.CPP.Z,57] time_stamp_freq,
  but this num is tied to your CPU's
  clock frequency which is inexact when
  converted to real-time.
*/


CDate cdt=GetCurTimeDate;

I64 i;
for (i=0;i<DF_NUM;i++)
  "%%h%dD: %h*D\n",i,i,cdt;
NewLine;

for (i=0;i<TF_NUM;i++)
  "%%h%dT: %h*T\n",i,i,cdt;

"$FG,PURPLE$CDate freq:%12.6f$FG$\n",0x100000000/24.0/60.0/60.0;

/*
  You can customize to your locality
  with [C:/TempleOS/Kernel/KGlbls.CPP.Z,136] date_fmt_map[] and [C:/TempleOS/Kernel/KGlbls.CPP.Z,137] time_fmt_map[].
  [C:/TempleOS/Kernel/Date.CPP.Z,199] DateTimeFmtMapsRst() will rst
  them.
*/

//Example of how to use with [C:/TempleOS/Kernel/Adam1a.HPP.Z,280] DF_NUM, [C:/TempleOS/Kernel/Adam1a.HPP.Z,221] TF_NUM #define's
"%h*D\n",DF_MM_DD_YYYY,cdt;
"%h*T\n",TF_HH24_MM,cdt;
