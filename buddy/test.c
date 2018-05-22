#include <stdio.h>
#include <assert.h>
#include "alloc.h"

void malloc_test(){
  int* p1 = malloc(100);
  assert(p1 != NULL);
  *p1 = 20000; //Write some data
  assert(*p1 == 20000);
  int* p2 = malloc(100);
  printf("%zu %zu\n", (void*)p1, (void*)p2);
  assert(p1 == (void*)p2 + 128  - 8); //-8 for pointer size
  int* p3 = malloc(100);
  int* p4 = malloc(100);
  int* p5 = malloc(100);
  int* p6 = malloc(100);
  int* p7 = malloc(100);
  int* p8 = malloc(100); //After this allocation the whole tree of 1024 bytes is filled.
  int* p9 = malloc(100);
  assert(p9 == NULL); //Make sure we can't overallocate.

  free(p1);

}

void calloc_test(){

  int i, n;
  int *a;

  printf("Number of elements to be entered:");
  scanf("%d",&n);

  a = (int*)calloc(n, sizeof(int));
  printf("Enter %d numbers:\n",n);
  for( i=0 ; i < n ; i++ ) {
     scanf("%d",&a[i]);
  }

  printf("The numbers entered have the addresses (in integer steps) -> ");
  for( i=0 ; i < n ; i++ ) {
     printf("%d ",&a[i]);
  }
  printf("\n");
  free(a);

}

void realloc_test(){
  char *str;

  /* Initial memory allocation */
  str = (char *) malloc(4);
  strcpy(str, "hej");
  printf("String = %s,  Address = %u\n", str, str);

  /* Reallocating memory to bigger space*/
  str = (char *) realloc(str, 64);
  strcat(str, ".com");
  printf("String = %s,  Address = %u\n", str, str);

  /* Reallocating memory to smaller space*/
  str = (char *) realloc(str, 5); //Splitting block for realloc

  /* Reallocating memory to smaller space*/
  str = (char *) realloc(str, 8); //just filling the current block

  free(str);
}

int main(){
  malloc_test(); //Test for 1024 bytes memory
  //calloc_test();
  //realloc_test();

  return 0;
}
