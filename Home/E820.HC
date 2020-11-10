CMemE820 *r = &MEM_E820;
I64 i;

for(i=0;i<MEM_E820_ENTRIES_NUM && r[i].type;++i) {
  "%016x-%016x %02x\n",r[i].base,r[i].base+r[i].len,r[i].type;
}