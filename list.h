#ifndef sh_list
#define sh_list

#define SBRK_FAILED ((void *)-1)

typedef struct block block_t;

/*A block contains some info about the specific block.
 *Also it refers to adjacent blocks keeping data.
 *The data is stored from the stored starting address in the block.
 */
typedef struct block{
  int blockID;
  size_t size;
  short used;
  block_t* head;
  block_t* tail;
  void *data
};

block_t* new_list(size_t size, void* addr);
void* search_free_block(size_t size);
void append(block_t** block1, block_t nblock2);
void delete_node(block_t* block);


#endif
