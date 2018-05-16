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

  while(p->tail != NULL){
    if(p->size < MIN_ALLOC_SIZE){
      return p;
    }
    p = p->tail;
  }
  return NULL;
}

void list_append(block_t* new_block, block_t* block){
  if(block == NULL){
    /*Adding first block*/
    printf("here2\n");
    new_block->head = NULL;
    new_block->tail = NULL;
    block = new_block;
    return 0;
  }

  block->tail = new_block;
  new_block->head = block;
  new_block->tail = NULL;
  printf("Adding block with start addr: %s", (char*)new_block->data);
}

void list_split_append(block_t* new_block, block_t* block){

}

void list_delete(block_t* block){
  /*Check against the address and free memory*/
}
