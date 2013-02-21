Bool HDAudioScan()
{
  I64 result,i=0;
  while (TRUE) {
    result=PCIClassFind(0x040300,i++);
    "EFLAGS:%X\n",c32_eflags;
    "Result:%X\n",result;
    if (result<0)
      return FALSE;
//    if (HDAudioInit(result.u8[2],result.u8[1],result.u8[0]))
//	return TRUE;
  }
}

HDAudioScan;
