#ifndef LINKED_H
#define LINKED_H

#include <stddef.h>

typedef struct block_t block_t;

/*A block contains some info about the specific block.
 *Also it refers to adjacent blocks keeping data.
 *The data is stored from the stored starting address in the block.
 */
typedef struct block_t{
  short used; /*2 bytes*/
  block_t* tail; /*8 bytes*/
  size_t size;   /*8 bytes*/
  void* data; /*8 bytes*/
};

#define BLOCK_INFO_SIZE offsetof(block_t, data)
#define BLOCK_T_SIZE sizeof(block_t)

size_t align_size(size_t size);

block_t* search_free_block(block_t* first, size_t size);
void list_append(block_t* new_block, block_t* first);
short free_block(void* addr, block_t* first);
block_t* fragment_block(block_t* block, size_t data_size);
void merge_adjacent(block_t* first);
void printList(block_t* first);


#endif
