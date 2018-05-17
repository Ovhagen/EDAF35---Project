#include <stdio.h>
#include "alloc.h"



int main(){
  int size = 1*sizeof(int);
  int* p1 = malloc(size);
  int* p2 = malloc(size);
  int* p3 = malloc(size);
  if(p1 && p2 && p3){
    printf("Houston we have a pointer.\n");
    /*for(int n=0; n<4; ++n) // populate the array
      p1[n] = n*n;
    for(int n=0; n<4; ++n) // print it back out
      printf("p1[%d] == %d\n", n, p1[n]);*/
  }else{
    printf("Returned with an error in main.\n");
  }
}
