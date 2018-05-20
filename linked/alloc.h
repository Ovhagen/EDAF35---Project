#ifndef ALLOC_H
#define ALLOC_H
#include <unistd.h>


/*Override alloc functions in C*/
#define malloc(size) my_malloc(size)
#define calloc(nitems, size) calloc(nitems, size)
#define realloc(ptr, size) realloc(ptr, size)
#define free(ptr) free(ptr)


void* heap_alloc(size_t size);

void* my_malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

#endif
