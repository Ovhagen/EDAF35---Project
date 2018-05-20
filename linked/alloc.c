#include <unistd.h>
#include <stdio.h>
#include "alloc.h"
#include "list.h"


#define LIST_DEBUG 0
#define MEM_DEBUG 0
#define RUNTIME_DEBUG 0

#define SBRK_FAILED ((void *)-1)

static int total_alloc = 0;
static block_t* first = NULL;


size_t min(size_t a, size_t b){
  return a < b ? a : b;
}


void* heap_alloc(size_t size){

  void* startaddress = sbrk(size);
  if(startaddress == SBRK_FAILED){
    //printf("Error: Failed to increment break point. Closing.");
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

  size_t real_size = align_size(size);
  /*Search for free space for new allocation*/
  block_t* free_block = search_free_block(first, size);
   if(free_block != NULL){
     //printf("A free block was found.\n");
     /*Allocate on free space and split if possible*/
     int diff = (int)(free_block->size - real_size - BLOCK_INFO_SIZE);
     if(diff == 0){
       /*Perfect fit. Allocate block*/
       free_block->used = 1;

       #if LIST_DEBUG
       printList(first);
       #endif

       return &free_block->data;
     }
     else if(diff > (int)BLOCK_INFO_SIZE){
       /*Fits in free block but fragmenting needs to be done.*/
       //printf("Fragmenting block.\n");
       fragment_block(free_block, real_size);
       #if LIST_DEBUG
       printList(first);
       #endif

       return &free_block->data;
     }
     free_block = NULL;
     //printf("Remaining data was smaller than header. Allocating new block.\n");
   }

   /*We didn't find a new block and need to allocate a new one*/
   block_t* new_block = heap_alloc(BLOCK_INFO_SIZE);

   if(new_block == NULL || new_block == SBRK_FAILED){
     return NULL; //Returns NULL for caller to handle
   }
   /*Allocate for perfect fit and set used*/
   new_block->size = real_size;
   new_block->used = 1;

   #if MEM_DEBUG
   printf("Header size: %d\n", BLOCK_INFO_SIZE);
   printf("Header start address: %d\n", new_block);
   #endif

   /*Allocate the data space and set address*/
   void* data_addr = heap_alloc(real_size);

   if(data_addr == NULL || data_addr == SBRK_FAILED){
     return NULL; //Returns NULL for caller to handle
   }

   new_block->data = data_addr;

   #if MEM_DEBUG
   printf("Allocation size: %d\n", real_size);
   printf("Data start address: %d\n", data_addr);
   printf("Program break point at %d\n", sbrk(0));
   #endif

   /*Check if list is initialized, if not initialize it with new block.*/
   if(first == NULL){
     new_block->tail = NULL;
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

   return &new_block->data;

}

void* calloc(size_t nitems, size_t size){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Calloc called with size and items: %d %d", size, nitems);
  #endif
  size_t real_size = nitems*size;
  void* ptr = malloc(real_size);
  if(ptr != NULL){
    /*Set the allocated memory to zero*/
    memset(ptr, 0, real_size);
  }else{
    /*Space could not be allocated.*/
    return NULL;
  }
}

void* realloc(void* ptr, size_t size){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("realloc called with ptr and size: %d %d\n", ptr, size);
  #endif

  /*If zero just deallocate the memory*/
  if(size == 0){
    free(ptr);
    return NULL;
  }

  block_t* old_block = first;
  /*Find the block*/
  while(old_block != NULL){
    if(&old_block->data == ptr){
      break;
    }
    old_block = old_block->tail;
  }

  if(old_block == NULL){
    /*No block found for the address*/
    return NULL;
  }
  //printf("Found ptr to realloc.\n");
  /*The block was found in memory*/
  size_t real_size = align_size(size);
  #if MEM_DEBUG
  printf("Allocation size: %d\n", real_size);
  #endif

  if(real_size <= old_block->size){
    /*Size is smaller than before*/
    size_t diff = old_block->size - real_size - BLOCK_INFO_SIZE;
    printf("%d", diff);
    if(diff > BLOCK_INFO_SIZE){
      /*Enough space left to inject a new block from the old data space*/
      /*Hold new data size for old block and create a new free block*/
      printf("Splitting old block to free memory.\n");
      fragment_block(old_block, diff);
    }
    printf("Size is smaller than before. Returning ptr.");

    #if LIST_DEBUG
    printList(first);
    #endif


    return ptr;
  }

  /*Larger sized block than before
   *Allocate new block with malloc and copy the old data*/
  void* new_block_data = malloc(size);
  memcpy(new_block_data, &old_block->data, old_block->size);

  /*Free the old block*/
  free(ptr);

  return new_block_data;
}

void free(void* ptr){
  #if MEM_DEBUG
  printf("=======================================================================\n");
  printf("Free was called.\n");
  printf("Data address to remove from: %d\n", ptr);
  #endif

  short res = free_block(ptr, first);
  if(res == 0){
    //printf("Free failed. Exiting.\n");
    return 0;
  }

  /*If emptied block has an empty adjacent block, merge these.*/
  merge_adjacent(first);
  #if LIST_DEBUG
  printList(first);
  #endif
}
