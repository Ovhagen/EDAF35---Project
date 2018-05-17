#include <unistd.h>
#include "malloc.h"
#include "list.h"

#define SBRK_FAILED ((void *)-1)
#define ALLOC_LIMIT 2048
#define malloc(size) my_malloc(size);

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
  printf("Linked list malloc called with size: %d\n", size);
  printf("Program starts here %d\n", sbrk(0));

  block_t new_block;
  /*Check if list is initialized, if not initialize it with new block.*/
  if(first == NULL){
    new_block.size = size;
    new_block.used = 1;
    size_t alloc_size = min(BLOCK_INFO_SIZE + size, MIN_ALLOC_SIZE);
    printf("%d\n", alloc_size);
    void* data_addr = heap_alloc(alloc_size);
    printf("%d\n", data_addr);
    if(data_addr == NULL || data_addr == SBRK_FAILED){
      return NULL; //Returns NULL for caller to handle
    }
    new_block.data = data_addr;
    list_append(&new_block, first);
    return new_block.data;
  }

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
  int bytes = nitems*size;

  void* ptr = sbrk(bytes);

  if(ptr < 0){
    return NULL;
  } else {
    return ptr;
  }
}

void* realloc(void* ptr, size_t size){}
void free(void* ptr){}
