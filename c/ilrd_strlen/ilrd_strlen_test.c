#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ilrd_strlen.h"




int main(){

const char word[]="tatyanna";
const char another_word[]="Tatyanna";
int size,cmp_size;
/*size = (sizeof(word)-1);
printf("word length is %d\n",size);
*/
size= StrLen(word);
printf("word is: %s and its length is %d\n",word,size);
printf("another word is: %s length is %d\n",another_word,StrLen(another_word));

cmp_size = StrCmp(word,another_word);
printf("StCmp func,(if equals 0 its good).\fwords compared: %d\n",cmp_size);
return 0;
}
