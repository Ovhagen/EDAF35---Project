#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

block_t* new_list(size_t size, void* addr){
  block_t* startBlock;
  printf("First block size: %d\n", sizeof(startBlock));
  startBlock = sbrk(sizeof(startBlock));
  printf("Initialized start block %d\n", startBlock);
  printf("program start %d\n", sbrk(0));


  return startBlock;
}

block_t* search_free_block(block_t* first, size_t data_size){
  block_t* p = first;

  while(p != NULL){
    if(p->size < MIN_ALLOC_SIZE){
      return p;
    }
    p = p->tail;
  }
  return NULL;
}

void list_append(block_t* new_block, block_t* block){
  block->tail = new_block;
  new_block->head = block;
  new_block->tail = NULL;
}

void list_split_append(block_t* new_block, block_t* block){

}

void list_delete(void* addr, block_t* first){
  block_t* p = first;
  /*Check against the address and free memory*/
  while(p->tail != NULL){
    p = p->tail;
  }
}

void printList(block_t* ptr){
  int i = 0;
  printf("%d\n", ptr->data);
  while(ptr != NULL){
    printf("|Block: %d Size: %d Addr: %d|  ->  ", i, ptr->size, ptr->data);
    ptr = ptr->tail;
    i += 1;
  }
  printf("\n\n");
}
