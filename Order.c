//-----------------------------------------------------------------------------
// Maria Fernanda Becerra
// mfbecerr
// CSE101
// pa6
// Order.c
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Dictionary.h"

#define MAX_LEN 160
int main(int argc, char * argv[]){
   FILE *in, *out;
   VAL_TYPE x;

   // check command line for correct number of arguments
   if( argc != 3 ){
      printf("Usage: Order <input file> <output file>\n");
      exit(1);
   }

   // open files for reading and writing 
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }
//printf("hello\n");
   char temp;
   int n=0;
   while((temp=fgetc(in))!=EOF){
     if(temp=='\n'){
       n++;
     }
   }
   rewind(in);
   char** A=calloc(n+1,sizeof(char*));
   for(int i=0;i<n;i++){
     A[i]=calloc(500,sizeof(char));
   }
   int i=0;
   while(fgets(A[i],255,in)!=NULL){
    i++; 
   }
//printf("before\n");
   Dictionary D=newDictionary(0);
   for(int i=0; i<n; i++){
      insert(D, A[i], NULL);
   }
//   delete(D,A[1]);
//printf("after\n");
   fprintf(out,"******************************************************\n");
   fprintf(out,"PRE-ORDER:\n");
   fprintf(out,"******************************************************\n");
   printDictionary(out,D,"pre");
   fprintf(out,"\n");
   fprintf(out,"******************************************************\n");
   fprintf(out,"IN-ORDER:\n");
   fprintf(out,"******************************************************\n");
   printDictionary(out,D,"in");
   fprintf(out,"\n");
   fprintf(out,"******************************************************\n");
   fprintf(out,"POST-ORDER:\n");
   fprintf(out,"******************************************************\n");
   printDictionary(out,D,"post");
   fprintf(out,"\n");
//   fprintf(out,"\n");
   fclose(in);
   fclose(out);
   for(int i=0;i<n;i++){
     free(A[i]);
   }
   free(A);
   freeDictionary(&D);
   return(0);
}
