
void qsort(void *base,DWORD num, DWORD width, DWORD *compare) 
{ //This uses a shell sort
  int my_compare(void *elem1,void *elem2 );
  int span=num/2,i;
  BYTE *b1,*b2,*temp=malloc(width);
  bool cont;

  my_compare=compare;
  while (span) {
    cont=true;
    while (cont) {
      cont=false;
      for (i=0;i+span<num;i++) {
        b1=base+width*i;
        b2=base+width*(i+span);
        if (my_compare(b1,b2)>0) {
          memcpy(temp,b1,width);
	  memcpy(b1,b2,width);
	  memcpy(b2,temp,width);
	  cont=true;
	}
      }
    }
    span=span/2;
  }
  free(temp);
}

int compare_hash_entries(sys_hash_entry **h1,sys_hash_entry **h2)
{
  return strcmp((*h1)->string,(*h2)->string);
}

void dump_hash_table(sys_hash_table *h)
{
  sys_hash_entry *temph,**list;
  DWORD count=0;
  
  int i,j;
  for (i=0;i<=h->mask;i++) {
    temph=&h->body+4*i;
    while (*temph) {
      count++;
      temph=temph->next;
    }
  }

  j=0;
  list=malloc(count*sizeof(DWORD));
  for (i=0;i<=h->mask;i++) {
    temph=&h->body+4*i;
    while (*temph) {
      list[j++]=temph;
      temph=temph->next;
    }
  }

  qsort(list,count,sizeof(DWORD),&compare_hash_entries);

  for (i=0;i<count;i++) {
    ? list[i]->type," ",list[i]->string,"\r\n";
  }

  free(list);
}
