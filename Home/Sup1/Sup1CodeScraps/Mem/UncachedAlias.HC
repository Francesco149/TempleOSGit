#define SAMPLE_SIZE	100000

U0 TimeIns()
{
  I64	i,start,end,
	*cached_i64	=MAlloc(sizeof(I64),Fs->code_heap),
	*uncached_i64	=dev.uncached_alias+cached_i64;
  CPURep;

  //Measure cached time
  start=GetTSC;
  for (i=0;i<SAMPLE_SIZE;i++)
    *cached_i64=i;
  end=GetTSC;
  "  Cached Cycles: %10,d\n",end-start;

  //Measure uncached time
  start=GetTSC;
  for (i=0;i<SAMPLE_SIZE;i++)
    *uncached_i64=i;
  end=GetTSC;
  "Uncached Cycles: %10,d\n",end-start;

  "$GREEN$The difference is even greater because the loop "
	"overhead was not subtracted.$FG$\n\n";
  Free(cached_i64);
}

TimeIns;
TimeIns;
