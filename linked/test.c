#include <stdio.h>
#include <assert.h>
#include "alloc.h"

void malloc_test(){
  int* p1 = malloc(100);
  assert(p1 != NULL);
  *p1 = 20000;
  assert(*p1 == 20000);
  int* p2 = malloc(100);
  assert(p2 == (void*)p1 + 104 + 24);
  int* p3 = malloc(100);
  assert(p3 == (void*)p1 + 104*2 + 24*2);

  void* old_start_addr = p1;

  free(p3);
  free(p2);
  free(p1);

  int size = 1*sizeof(int);
  p1 = malloc(size); //Fragments block on same start address
  assert(p1 == old_start_addr);
  p2 = malloc(20); //fragments block
  printf("%zu", (void*)p1 );
  assert(p2 == (void*)p1 + size + 4 + 24);  //plus 4 for size alignment
  p3 = malloc(50);
  assert(p3 == (void*)p1 + size + 4 + 24 + 24*2); //24 for size alignment

  free(p3);
  free(p2); //Merge should happen here

  p3 = malloc(5);
  assert(p3 == old_start_addr + 8 + 24); //p1 is still allocated
  p2 = malloc(200);
  int* p4 = malloc(48); // perfect in last empty block
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

// #define _BSD_SOURCE
//
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include "alloc.h"
//
// void* malloc_and_fill(size_t size, char padding) {
//     char* ptr = malloc(size);
//     size_t i;
//
//     if (ptr == NULL) {
//         fprintf(stderr, "Malloc failed\n");
//         exit(EXIT_FAILURE);
//     }
//
//     for (i = 0; i < size; i++) {
//       printf("%d", ptr[i]);
//         ptr[i] = padding;
//     }
//
//     printf("Malloc, size %zu\n", size);
//     printf(" ptr:\t%p\n", ptr);
//
//     return ptr;
// }
//
// void free_ptr(void* ptr) {
//     free(ptr);
//
//     #if DEBUG
//     printf("after freeing ptr (%p)\n", ptr);
//     print_memory();
//     print_avail();
//     #endif
// }
//
// int main(void) {
//
//     char *p, *q, *r, *rr, *s, *t, *u;
//
//     p =  malloc_and_fill(9927, 0x12);
//     q =  malloc_and_fill(100000, 0x34);
//     // r =  malloc_and_fill(100000, 0x56);
//     // rr = malloc_and_fill( 30000, 0x78);
//     //
//     // free_ptr(p);
//     // free_ptr(rr);
//     // free_ptr(r);
//     // free_ptr(q);
//     //
//     // s = malloc_and_fill(8, 0x78);
//     // t = malloc_and_fill(30, 0x9a);
//     // u = malloc_and_fill(4, 0xbc);
//     //
//     // free_ptr(s);
//     // free_ptr(t);
//     // free_ptr(u);
//
//     return 0;
// }
