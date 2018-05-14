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
  if(startBlock == SBRK_FAILED){
    printf("Failed to increment break point. Closing.");
    return NULL; //Returns NULL for caller to handle
  }

  startBlock->blockID = 0;
  startBlock->used = 0;
  startBlock->head = NULL;
  startBlock->size = size;
  startBlock->data = addr;
  startBlock->tail = NULL;

  return startBlock;
}

void* search_free_block(size_t size){

}

void append(block_t** node1, block_t node2){

}

void delete_node(block_t* node){

}
