U0 Main()
{
  while (!ScanChar) {
    "%d ",ToI64((Now-A2Date("1/1/1970"))/CDATE_FREQ+6*3600-330);
    "%d\n",FloorI64((Now-A2Date("1/1/1970"))/CDATE_FREQ+6*3600-330,60);
    Sleep(100);
  }
}

Main;
