
#include "stdio.h"
#include "string.h"


void main(int _argc,char *_argv[])
{
  FILE *in_file,*out_file;
  int i,j,l,col;
  int last_code;
  unsigned char buf[512];
  char *ptr,*ptr2;
  if ((_argc!=3) || !(in_file=fopen(_argv[1],"rb")) || !(out_file=fopen(_argv[2],"w")))
	 printf("Could not open files\n");
  else {
	 ptr=_argv[1];
	 ptr2=ptr;
	 while (*ptr) {
		if (*ptr=='\\')
		  ptr2=ptr+1;
		ptr++;
	 }
	 strcpy(buf,"FIL_");
	 strcat(buf,ptr2);
	 ptr=buf;
	 while (*ptr) {
		if (*ptr=='.')
		  *ptr='_';
		ptr++;
	 }
	 printf("Cvt %s to %s\n",_argv[1],_argv[2]);

	 fprintf(out_file,
		"        INCLUDE OSINC.INC\n"
		";;***************************SEGMENT**************************\n"
		"SYSFILES SEGMENT DWORD USE32 PUBLIC\n"
		"        ASSUME CS:SYSFILES,DS:SYSFILES\n"
		"        ALIGN   4\n"
		"        PUBLIC  %s\n"
		"%s:\n",buf,buf);

	 col=80;
	 last_code=0;
	 while (l=fread(buf,1,sizeof(buf),in_file)) {
		for (i=0;i<l;i++) {
		  if (col==80) fprintf(out_file,"\n        DB ");
		  if (buf[i]>31 && buf[i]<127 && buf[i]!='\'') {
			 if (last_code==2) {
				fprintf(out_file,",");
				col--;
			 }
			 if (last_code!=1) {
				fprintf(out_file,"\'");
				col--;
			 }
			 fprintf(out_file,"%c",buf[i]);
			 col--;
			 last_code=1;
		  } else {
			 if (last_code==1) {
				fprintf(out_file,"\'");
				col--;
			 }
			 if (last_code!=0) {
				fprintf(out_file,",");
				col--;
			 }
			 fprintf(out_file,"%d",buf[i]);
			 col-=3;
			 last_code=2;
		  }
		  if (col<32) {
			 if (last_code==1) {
				fprintf(out_file,"\'");
				col--;
			 }
			 col=80;
			 last_code=0;
		  }
		}
	 }
	 if (last_code==1) {
		fprintf(out_file,"\'");
		col--;
	 }
	 fprintf(out_file,"\n        DB 0\n");
	 fprintf(out_file,
		"        ENDS\n"
		"        END\n");
	 fclose(in_file);
	 fclose(out_file);
  }
}
