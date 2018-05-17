#include <unistd.h>
#include <stdio.h>
#include "alloc.h"
#include "list.h"


#define LIST_DEBUG 1
#define MEM_DEBUG 1

#define SBRK_FAILED ((void *)-1)
#define ALLOC_LIMIT 2048

static short total_alloc = 0;
static block_t* first = NULL;


size_t min(size_t a, size_t b){
  return a < b ? a : b;
}


void* heap_alloc(size_t size){
  if(ALLOC_LIMIT - (total_alloc + size) <= 0){
    printf("Error: Memory limit reached. Allocation omitted.");
    return NULL;
  }

  void* startaddress = sbrk(size);
  if(startaddress == SBRK_FAILED){
    printf("Error: Failed to increment break point. Closing.");
    return SBRK_FAILED; //Returns NULL for caller to handle
  }

  total_alloc += size;
  return startaddress;
}


void* my_malloc(size_t size){

  #if LIST_DEBUG
  printf("=======================================================================\n");
  printf("Linked list malloc called with size: %d\n", size);
  #endif

  #if MEM_DEBUG
  printf("Program starts here %d\n", sbrk(0));
  #endif

  block_t* new_block = heap_alloc(BLOCK_INFO_SIZE);

  #if MEM_DEBUG
  printf("Header size: %d\n", BLOCK_INFO_SIZE);
  printf("Header start address: %d\n", new_block);
  #endif

  /*Check if list is initialized, if not initialize it with new block.*/
  new_block->size = size;
  new_block->used = 1;
  if(first == NULL){
    new_block->tail = NULL;
    new_block->head = NULL;
    size_t alloc_size = min(size, MIN_ALLOC_SIZE);
    void* data_addr = heap_alloc(alloc_size);

    #if MEM_DEBUG
    printf("Allocate size: %d\n", alloc_size);
    printf("Data start address: %d\n", data_addr);
    #endif

    if(data_addr == NULL || data_addr == SBRK_FAILED){
      return NULL; //Returns NULL for caller to handle
    }
    new_block->data = data_addr;
    first = new_block;

    printf("hello %d\n", first->data);

    #if LIST_DEBUG
    printList(first);
    #endif

    return new_block->data;
  }

  size_t alloc_size = min(size, MIN_ALLOC_SIZE);
  void* data_addr = heap_alloc(alloc_size);
  new_block->data = data_addr;

  list_append(&new_block, first);

  #if LIST_DEBUG
  printList(first);
  printf("\n=======================================================================\n");
  #endif

  return new_block->data;
  //}

  /*Search for free space for new allocation*/
  // block_t* free_block = NULL;
  //
  // if(free_block != NULL){
  //   /*See the block*/
  //   if(free_block->size >= MIN_ALLOC_SIZE){
  //
  //   }
  // }else{
  //   /*We didn't find a new block and need to allocate a new one*/
  // }

}

void* calloc(size_t nitems, size_t size){
  // int bytes = nitems*size;
  //
  // void* ptr = sbrk(bytes);
  //
  // if(ptr < 0){
  //   return NULL;
  // } else {
  //   return ptr;
  // }
}

void* realloc(void* ptr, size_t size){}
void free(void* ptr){

  list_delete(ptr, first);
}
