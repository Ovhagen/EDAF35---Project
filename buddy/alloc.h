#ifndef ALLOC_H
#define ALLOC_H

#include <unistd.h>

/*Override alloc functions in C*/
#define malloc(size) my_malloc(size)
#define calloc(nitems, size) calloc(nitems, size)
#define realloc(ptr, size) realloc(ptr, size)
#define free(ptr) free(ptr)

/*Status byte*/
/*
* first bit set if split
* second bit set if used
* thrid bit buddy left
* four bit buddy right
*/

typedef struct buddy_t buddy_t;

typedef struct buddy_t{
  unsigned char status;
  void* data;
};

enum STATUS
{
  SPLIT = 0x80,
  USED = 0x40,
  LEFT = 0x20,
  RIGHT = 0x10
} status_byte;


#define MEMORY_SIZE 1024
#define STATUS_SIZE sizeof(char)
#define VOID_SIZE sizeof(void*)
#define BUDDY_T_SIZE sizeof(buddy_t)
#define DEPTH (int)(log(MEMORY_SIZE)/log(2))
#define PRINT_LENGTH 60

void* heap_alloc(size_t size);
size_t align_size(size_t size);
short checkStatus(char c, char status_code);

void getTree(buddy_t* node, size_t node_size, char* tree[DEPTH][2][PRINT_LENGTH], short row, short col);
buddy_t* add_buddy(buddy_t* node_addr, size_t node_size, size_t data_size);

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

#endif
