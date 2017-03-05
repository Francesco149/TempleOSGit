U0 BlkDevRep()
{//Block Device Report.
  I64 i;
  for (i=0;i<BLKDEVS_NUM;i++)
    if (blkdev.blkdevs[i].bd_signature==BD_SIGNATURE_VAL)
      //[C:/Adam/ADbg.HC.Z,161] ClassRep() dumps any structure, automatically.
      ClassRep(&blkdev.blkdevs[i]);
      //The lastclass arg tells ClassRep the datatype.
      //lastclass is a language feature of [C:/Doc/HolyC.DD.Z,1] HolyC.
      //See [C:/Demo/LastClass.HC.Z,1].
}

BlkDevRep;
