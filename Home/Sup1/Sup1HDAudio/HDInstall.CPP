Cd(__DIR__);;

Adam("Cd(\"::%s\");\n",Fs->cur_dir);
AdamFile("HDAudio.CPP.Z");

Bool HDAudioScan()
{
  I64 i=-1,j;
  while (TRUE) {
    j=PCIClassFind(0x040300,++i);
    if (j<0)
      return FALSE;

    if (HDAudioInit(j.u8[2],j.u8[1],j.u8[0]))
      return TRUE;
  }
}

HDAudioScan;
