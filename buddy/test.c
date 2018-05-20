#include <stdio.h>
#include <assert.h>
#include "alloc.h"

void malloc_test(){
  int* p1 = malloc(100);
  assert(p1 != NULL);
  // *p1 = 20000;
  // assert(*p1 == 20000);
  // int* p2 = malloc(100);
  // assert(p2 == (void*)p1 + 104 + 24);
  // int* p3 = malloc(100);
  // assert(p3 == (void*)p1 + 104*2 + 24*2);
  //
  // void* old_start_addr = p1;
  //
  // free(p3);
  // free(p2);
  // free(p1);
  //
  // int size = 1*sizeof(int);
  // p1 = malloc(size); //Fragments block on same start address
  // assert(p1 == old_start_addr);
  // p2 = malloc(20); //fragments block
  // printf("%zu", (void*)p1 );
  // assert(p2 == (void*)p1 + size + 4 + 24);  //plus 4 for size alignment
  // p3 = malloc(50);
  // assert(p3 == (void*)p1 + size + 4 + 24 + 24*2); //24 for size alignment
  //
  // free(p3);
  // free(p2); //Merge should happen here
  //
  // p3 = malloc(5);
  // assert(p3 == old_start_addr + 8 + 24); //p1 is still allocated
  // p2 = malloc(200);
  // int* p4 = malloc(48); // perfect in last empty block
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
  malloc_test();
  //calloc_test();
  //realloc_test();

  return 0;
}
