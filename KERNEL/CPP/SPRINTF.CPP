

char *out_char(char *dst,char ch)
{
  if (dst==NULL)
    putchar(ch);
  else
    *(dst++)=ch;
  return dst;
}

void sprintf_join(char *dst,char *src,int argc,int *argv)
{
  char buf[36],ch,*ptr;
  bool pad_zero=false,left_justify=false;
  int  cur_param=0,i,j,k,len;
  DWORD d;
  while (ch = *(src++)) {
    if (ch=='%') {
      if (*src=='-') {
	left_justify=true;
	src++;
      } else
	left_justify=false;
      if (*src=='0') {
	pad_zero=true;
	src++;
      } else
	pad_zero=false;
      len=0;
      while (*src>='0' && *src<='9')
	len=len*10+ *(src++)-'0';
      if (*src=='s') {
	src++;
	ptr=argv[cur_param++];
	i=strlen(ptr);
	if (left_justify) {
	  while (*ptr)
	    dst=out_char(dst,*(ptr++));
	  for (j=0;j<len-i;j++)
	    dst=out_char(dst,32);
	} else {
	  for (j=0;j<len-i;j++)
	    dst=out_char(dst,32);
	  while (*ptr)
	    dst=out_char(dst,*(ptr++));
	}
      } else if (*src=='e') {
	src++;
	ptr=point_at_list_entry(argv[cur_param],argv[cur_param+1]);
	cur_param=cur_param+2;
	i=strlen(ptr);
	for (j=0;j<len-i;j++)
	  dst=out_char(dst,32);

	while (*ptr)
	  dst=out_char(dst,*(ptr++));
      } else {
	d=argv[cur_param++];
        i=d;
	k=0;
	if (*src=='d') {
	  src++;
	  if (i<0) {
	    buf[k++]='-';
	    i=-i;
	  }
	  while (k==0|| i) {
	    buf[k++]=(i % 10)+'0';
	    i=i/10;
	  }
	} else if (*src=='u') {
	  src++;
	  while (k==0|| d) {
	    buf[k++]=(d % 10)+'0';
	    d=d/10;
	  }
	} else if (*src=='X') {
	  src++;
	  while (k==0 || d) {
	    buf[k]=(d & 15)+'0';
	    if (buf[k]>'9') buf[k]=buf[k]+'A'-0x3A;
	    k++;
	    d=d>>4;
	  }
	} else if (*src=='x') {
	  src++;
	  while (k==0 || d) {
	    buf[k]=(d & 15)+'0';
	    if (buf[k]>'9') buf[k]=buf[k]+'a'-0x3A;
	    k++;
	    d=d>>4;
	  }
	} else if (*src=='b') {
	  src++;
	  while (k==0 || d) {
	    buf[k++]=(d&1)+'0';
	    d=d>>1;
	  }
	} else if (*(src++)=='%') {
	  buf[k++]='%';
	  len=1;
	  cur_param--;
	}
	if (pad_zero)
	  for (i=0;i<len-k;i++)
	    dst=out_char(dst,'0');
	else
	  for (i=0;i<len-k;i++)
	    dst=out_char(dst,32);
	for (i=k-1;i>=0;i--)
	  dst=out_char(dst,buf[i]);
      }
    } else
      dst=out_char(dst,ch);
  }
  if (dst) out_char(dst,0);
}



void sprintf(char *dst,char *src,...)
{
  sprintf_join(dst,src,argc,argv);
}

void printf(char *src,...)
{
  sprintf_join(NULL,src,argc,argv);
}


void getf(char *src,char *fmt,...)
{
  char ch,*buf,*ptr;
  bool left_justify=false;
  int  cur_param=0,i,j,k,len,*i_ptr;
  while (ch = *(fmt++)) {
    if (ch=='%') {
      if (*fmt=='%') {
	src++;
	fmt++;
      } else {
	if (*fmt=='-') {
	  left_justify=true;
	  fmt++;
	} else
	  left_justify=false;
	len=0;
	while (*fmt>='0' && *fmt<='9')
	  len=len*10+ (*(fmt++)-'0');

	ch=*(fmt++);
	if (ch && !len) {
	  ptr=src;
	  while (*ptr && *ptr!=*fmt)
	    ptr++;
	  len=ptr-src;
	}
	buf=malloc(len+1);
	for (i=0;i<len;i++)
	  buf[i]=*(src++);
	buf[i]=0;
	if (ch=='s') {
	  ptr=argv[cur_param++];
	  strcpy(ptr,buf);
	} else if (ch=='e') {
	  i_ptr=argv[cur_param++];
	  *i_ptr=match_list_entry(buf,argv[cur_param++]);
	} else {
	  i_ptr=argv[cur_param++];
	  *i_ptr=0;
	  if (toupper(ch)=='D') {
	    for (k=0;k<len;k++)
	      if (buf[k]!=32)
		*i_ptr=*i_ptr*10+(buf[k]-'0');
	  } else if (toupper(ch)=='X') {
	    for (k=0;k<len;k++)
	      if (buf[k]!=32) {
		j=toupper(buf[k])-'0';
		if (j>9)  j=j-'A'+('9'+1);
		*i_ptr=*i_ptr*16+j;
	      }
	  } else if (toupper(ch)=='B') {
	    for (k=0;k<len;k++)
	      if (buf[k]!=32)
		*i_ptr=*i_ptr*2+(buf[k]-'0');
	  }
	}
	free(buf);
      }
    } else
      src++;
  }
}

