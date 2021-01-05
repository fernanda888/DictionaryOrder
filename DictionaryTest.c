//-----------------------------------------------------------------------------
// Maria Fernanda Becerra
// mfbecerr
// CSE101
// pa6
// DictionaryTest.c
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char* argv[]){
  int i, n=14;
   Dictionary Not = newDictionary(0);
   Dictionary Unique = newDictionary(1);
   Dictionary AlsoNot=newDictionary(0);
   Dictionary AlsoUnique=newDictionary(1);
   char* input[] = { "hello","this","is","my","test","file","for","the","dictionary","adt","i","hope","its","good" };
   VAL_TYPE x;
   VAL_TYPE y;
   for(i=0; i<n; i++){
      insert(Not, input[i], NULL);
   }
   for(i=n-1; i>=0; i--){
      insert(Unique, input[i], NULL);
   }
   printf("Dictionary Not:\n");
   printDictionary(stdout, Not, "in");
   printf("\n");
   printf("Dictionary Unique:\n");
   printDictionary(stdout, Unique,"in");
   printf("\n");
   //up to here means insert and printDictionary work

   delete(Not, "hello");
   delete(Unique,"is");
   printf("Dictionary Not:\n");
   printDictionary(stdout, Not,"pre");
   printf("\n");
   printf("Dictionary Unique:\n");
   printDictionary(stdout, Unique,"pre");
   printf("\n");
   //up to here delete works.

   for(x=beginForward(Not); currentKey(Not)!=KEY_UNDEF; x=next(Not)){
      insert(AlsoNot,currentKey(Not),currentVal(Not));
   }
   printf("Dictionary Not:\n");
   printDictionary(stdout, Not,"pre");
   printf("\n");
   printf("Dictionary AlsoNot:\n");
   printDictionary(stdout, AlsoNot,"pre");
   printf("\n");
   //should be the same
   //up to here means beginForward, 
   //current val,
   //current key and 
   //next work.

   for(y=beginReverse(Unique); currentKey(Unique)!=KEY_UNDEF; y=prev(Unique)){
      insert(AlsoUnique,currentKey(Unique),currentVal(Unique));
   }
   printf("Dictionary Unique:\n");
   printDictionary(stdout, Unique,"in");
   printf("\n");
   printf("Dictionary AlsoUnique:\n");
   printDictionary(stdout, AlsoUnique,"in");
   printf("\n");
   //up to here beginReverse and prev also work!!

   makeEmpty(Not);
   printf("Dictionary Not:\n");
   printDictionary(stdout, Not,"in");
   printf("\n");
   //make empty works

   printf("Size of AlsoNot: %d \n",size(AlsoNot));
   //size works!!

   printf("Not unique: %d \n",getUnique(Not));
   printf("AlsoNot unique: %d \n",getUnique(AlsoNot));
   printf("Unique unique: %d \n",getUnique(Unique));
   printf("AlsoUnique unique: %d \n",getUnique(Unique));
   //get unique works!!

   x=lookup(Unique,"is");
   //printf(VAL_FORMAT"\n",x);
   //look up works

   for(int i=0;i<3;i++){
     insert(Not,"rep",NULL);
   }
   printf("Dictionary Not:\n");
   printDictionary(stdout, Not,"in");
   printf("\n");
   //can insert duplicate keys!
   freeDictionary(&Not);
   freeDictionary(&Unique);
   freeDictionary(&AlsoNot);
   freeDictionary(&AlsoUnique);

  return(0);
}
