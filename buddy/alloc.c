#include <unistd.h>
#include <stdio.h>
#include "alloc.h"

#define LIST_DEBUG 1
#define MEM_DEBUG 1
#define RUNTIME_DEBUG 1

#define SBRK_FAILED ((void *)-1)

static buddy_t* root = NULL;

void* heap_alloc(size_t size){

  void* startaddress = sbrk(size);
  if(startaddress == SBRK_FAILED){
    //printf("Error: Failed to increment break point. Closing.");
    return SBRK_FAILED; //Returns NULL for caller to handle
  }

  return startaddress;
}

size_t align_size(size_t size){
  unsigned size_offset;

  size_offset = size % sizeof(size_t);

  return size_offset != 0 ? (size + sizeof(size_t) - size_offset): size;
}

void* my_malloc(size_t size){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Linked list malloc called with size: %d\n", size);
  #endif

  #if MEM_DEBUG
  printf("Program break point at %d\n", sbrk(0));
  #endif

  /*Initialize memory if first round*/
  if(root == NULL){
    #if MEM_DEBUG
    printf("Initializing memory of %d bytes for the first time.\n", MEMORY_SIZE);
    #endif
    root = heap_alloc(MEMORY_SIZE + BUDDY_T_SIZE);
    if(root == SBRK_FAILED){
      return NULL;
    }
    root->status = ((char)0 & FREE);
    printf("Root status: %c\n", FREE);
    root->data = &root->status + STATUS_SIZE;
    printf("Allocated space starts at: %zu\n", root->data);
  }

  size_t real_size = align_size(size) + BUDDY_T_SIZE;

  #if MEM_DEBUG
  printf("Header size: %d\n", BUDDY_T_SIZE);
  printf("Allocation request size: %d\n", real_size - BUDDY_T_SIZE);
  #endif

  return NULL;

}

void* calloc(size_t nitems, size_t size){

}
void* realloc(void* ptr, size_t size){

}
void free(void* ptr){

}
