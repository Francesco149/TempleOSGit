//This is the startup file for new local users
use_console_ltf("/HOPPY/KERNEL/CPP/USER.MEN");

void display_faulting_task()
{
  tss_struct *tss=*FAULTING_TSS;
  sys_hash_entry *temph;
  member_list_node *ml;
  class_node *cn;
  int *i_ptr,i;
  DWORD offset;
  class_node *cl=find_local_hash("tss_struct",HTT_CLASS);

  ? "Faulting TSS:",tss," IRQ#",*FAULTING_IRQ_NUM,"\r\n";
  ml=cl->member_list;
  while (ml) {
    if (ml->format_str) {
      i_ptr=tss+ml->offset;
      cn=ml->member_class;
      if (cn->type==HTT_INTERNAL_TYPE && ml->count==0) {
	if (ml->format_data)
	  printf(ml->format_str,*i_ptr,ml->format_data);
	else
	  printf(ml->format_str,*i_ptr);
        if (temph=locate_program_section(*i_ptr,&offset)) 
          printf(" %s,%X ",temph->string,offset);
      } else {
	if (ml->format_data)
	  printf(ml->format_str,i_ptr,ml->format_data);
	else
	  printf(ml->format_str,i_ptr);
        if (temph=locate_program_section(i_ptr,&offset)) 
          printf(" %s,%X ",temph->string,offset);
      }
      ? "\r\n";
    }
    ml=ml->next;
  }
  ? "\r\n";

  i_ptr=tss->esp;
  for (i=0;i<32;i++) {
    printf("[ESP+%04X]: %08X",i*4,i_ptr[i]);
    if (temph=locate_program_section(i_ptr[i],&offset)) 
      printf(" %s,%X ",temph->string,offset);
    ? "\r\n";
  }
  ? "\r\n";

  dasm(i_ptr[0]-24,24);
  
}

display_faulting_task;
;
 
