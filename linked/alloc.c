#include <unistd.h>
#include <stdio.h>
#include "alloc.h"
#include "list.h"


#define LIST_DEBUG 1
#define MEM_DEBUG 1
#define RUNTIME_DEBUG 1

#define SBRK_FAILED ((void *)-1)
#define ALLOC_LIMIT 2048

static int total_alloc = 0;
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

  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Linked list malloc called with size: %d\n", size);
  #endif

  #if MEM_DEBUG
  printf("Program break point at %d\n", sbrk(0));
  printf("Memory allocated: %d\n", total_alloc);
  #endif

  /*Search for free space for new allocation*/
  block_t* free_block = search_free_block(first, size);
   if(free_block != NULL){
     printf("A free block was found.\n");
     /*Allocate on free space and split if possible*/
     if(free_block->size - size > BLOCK_INFO_SIZE){
       fragment_block(free_block, size);
       free_block->used = 1;
       free_block->size = size;

       #if LIST_DEBUG
       printList(first);
       #endif

       return free_block->data;
     }
     free_block = NULL;
     printf("Remaining data was smaller than header. Allocating new block.\n");
   }

   /*We didn't find a new block and need to allocate a new one*/
   block_t* new_block = heap_alloc(BLOCK_INFO_SIZE);

   if(new_block == NULL || new_block == SBRK_FAILED){
     return NULL; //Returns NULL for caller to handle
   }

   /*Allocate for perfect fit and set used*/
   new_block->size = size;
   new_block->used = 1;

   #if MEM_DEBUG
   printf("Header size: %d\n", BLOCK_INFO_SIZE);
   printf("Header start address: %d\n", new_block);
   #endif

   /*Allocate the data space and set address*/
   void* data_addr = heap_alloc(size);

   if(data_addr == NULL || data_addr == SBRK_FAILED){
     return NULL; //Returns NULL for caller to handle
   }

   new_block->data = data_addr;

   #if MEM_DEBUG
   printf("Allocation size: %d\n", size);
   printf("Data start address: %d\n", data_addr);
   printf("Program break point at %d\n", sbrk(0));
   #endif

   /*Check if list is initialized, if not initialize it with new block.*/
   if(first == NULL){
     new_block->tail = NULL;
     new_block->head = NULL;
     first = new_block;

     #if LIST_DEBUG
     printList(first);
     #endif

     return new_block->data;
   }

   list_append(new_block, first);

   #if LIST_DEBUG
   printList(first);
   #endif

   return new_block->data;


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
  #if MEM_DEBUG
  printf("=======================================================================\n");
  printf("Free was called.\n");
  printf("Data address to remove from: %d\n", ptr);
  #endif

  short res = free_block(ptr, first);
  if(res == 0){
    printf("Free failed. Exiting.\n");
    return 0;
  }

  /*If emptied block has an empty adjacent block, merge these.*/
  merge_adjacent(first);
  #if LIST_DEBUG
  printList(first);
  #endif
}
