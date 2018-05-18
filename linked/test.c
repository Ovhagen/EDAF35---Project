#include <stdio.h>
#include "alloc.h"



int main(){
  int size = 1*sizeof(int);
  int p1 = malloc(100);
  free(p1);
  p1 = malloc(36);
  // int* p2 = malloc(size);
  // int* p3 = malloc(size);
  //
  // free(p3);
  // free(p2);
  // free(p1);
  //
  // p1 = malloc(size);
  // p2 = malloc(20);
  // p3 = malloc(50);
  //
  // free(p3);
  // free(p2); //Merge should happen to times here
  //
  // p3 = malloc(5);
  // p2 = malloc(200);
}
