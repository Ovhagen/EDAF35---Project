#ifndef sh_list
#define sh_list

#include <stddef.h>

typedef struct block block_t;

/*A block contains some info about the specific block.
 *Also it refers to adjacent blocks keeping data.
 *The data is stored from the stored starting address in the block.
 */
typedef struct block{
  size_t size;
  short used;
  block_t* head;
  block_t* tail;
  void *data
};

#define BLOCK_INFO_SIZE offsetof(block_t, data)
#define MAX_BLOCK_SIZE 64  //maximum data => 56
#define MIN_ALLOC_SIZE 16 //minimum data => 8

block_t* new_list(size_t size, void* addr);
block_t* search_free_block(block_t* first, size_t size);
void list_append(block_t* new_block, block_t* block);
void list_split_append(block_t* new_block, block_t* block);
void list_delete(block_t* block);


#endif
