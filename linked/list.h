#ifndef LINKED_H
#define LINKED_H

#include <stddef.h>

typedef struct block_t block_t;

/*A block contains some info about the specific block.
 *Also it refers to adjacent blocks keeping data.
 *The data is stored from the stored starting address in the block.
 */
typedef struct block_t{
  size_t size;
  short used;
  block_t* head;
  block_t* tail;
  void *data;
};

#define BLOCK_INFO_SIZE offsetof(block_t, data)
#define MIN_ALLOC_SIZE 16

block_t* search_free_block(block_t* first, size_t size);
void list_append(block_t* new_block, block_t* first);
short free_block(void* addr, block_t* first);
block_t* fragment_block(block_t* block, size_t data_size);
void merge_adjacent(block_t* first);
void printList(block_t* first);


#endif
