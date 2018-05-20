#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

size_t align_size(size_t size){
  unsigned size_offset;

  size_offset = size % sizeof(size_t);

  return size_offset != 0 ? (size + sizeof(size_t) - size_offset): size;
}

block_t* search_free_block(block_t* block, size_t data_size){
  while(block != NULL){
    if(block->used == 0 && block->size >= data_size){
      return block;
    }
    block = block->tail;
  }
  return NULL;
}


void list_append(block_t* new_block, block_t* ptr){
  while(ptr->tail != NULL){
    ptr = ptr->tail;
  }

  ptr->tail = new_block;
  new_block->tail = NULL;
}


short free_block(void* addr, block_t* ptr){
  /*Check against the address
  * If match, mark as unused */
  while(ptr != NULL){
    if(&ptr->data == addr){
      ptr->used = 0;
      return 1;
    }
    ptr = ptr->tail;
  }
  return 0;
}


block_t* fragment_block(block_t* block, size_t data_size){
  /*Point the new block after allocated data*/
  block_t* new_block = (void*)&block->data + data_size;
  /*Initialize the new unused block and put it in the list*/
  new_block->used = 0;
  new_block->data = (void*)new_block + BLOCK_INFO_SIZE; //void* for address increment
  new_block->size = block->size - data_size - BLOCK_INFO_SIZE;
  new_block->tail = block->tail;

  block->tail = new_block;
  block->used = 1;
  block->size = data_size;
}


void merge_adjacent(block_t* ptr){
  block_t* to_remove = NULL;
  while(ptr != NULL){
      if(ptr->used == 0 && ptr->tail != NULL && ptr->tail->used == 0){
        to_remove = ptr->tail;
        //printf("Merging unallocated blocks\n");

        /*Extend the first of the two blocks and empty the second empty block.*/
        ptr->tail = to_remove->tail;
        ptr->size += BLOCK_INFO_SIZE + to_remove->size;

        to_remove->data = NULL;
        to_remove->tail = NULL;
        to_remove->size = 0;
      }else{
        ptr = ptr->tail;
      }
  }
}


void printList(block_t* ptr){
  int i = 0;
  printf("\n");
  while(ptr != NULL){
    printf("|Block: %d Size: %d Addr: %d Used: %d|  ->  ", i, ptr->size, &ptr->data, ptr->used);
    ptr = ptr->tail;
    i += 1;
  }
  printf("\n\n");
}
