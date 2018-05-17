#ifndef ALLOC_H
#define ALLOC_H

#include <unistd.h>

void* heap_alloc(size_t size);

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

#endif
