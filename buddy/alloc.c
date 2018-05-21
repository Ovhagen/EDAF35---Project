#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "alloc.h"

#define LIST_DEBUG 1
#define MEM_DEBUG 1
#define RUNTIME_DEBUG 1

#define SBRK_FAILED ((void *)-1)


static buddy_t* root = NULL;
static unsigned end_address = 0;

void* heap_alloc(size_t size){

  void* startaddress = sbrk(size);
  if(startaddress == SBRK_FAILED){
    //printf("Error: Failed to increment break point. Closing.");
    return SBRK_FAILED; //Returns NULL for caller to handle
  }

  return startaddress;
}

size_t align_size(size_t size){
  unsigned size_offset;

  size_offset = size % VOID_SIZE;

  return size_offset != 0 ? (size + VOID_SIZE - size_offset): size;
}

short checkStatus(char c, char status_code){
  return (c & status_code) != 0 ? 1 : 0;
}

void getTree(buddy_t* node, size_t node_size, char* tree[DEPTH][2][PRINT_LENGTH], short row, short col){
 printf("row %d col %d\n", row, col);
  if(checkStatus(node->status, USED) == 1){
   printf("Printing used node\n");
    char buffer[35];
    snprintf(buffer, sizeof(buffer), "|Status: %d Addr: %zu |", node->status, node->data);
    printf("buff: %s\n", buffer);
    strcpy(tree[row][col], buffer);
    printf("node copy: %s\n", tree[row][col]);
  }else if(checkStatus(node->status, SPLIT) == 1){
    char buffer[35];
    snprintf(buffer, sizeof(buffer), "|Status: %d Addr: %zu |", node->status, node->data);
    printf("buff: %s\n", buffer);
    strcpy(tree[row][col], buffer);
    printf("node copy: %s\n", tree[row][col]);
    if(checkStatus(node->status, RIGHT) == 1){
      printf("Printing right\n");
      getTree((void*)node + node_size/2, node_size/2, tree, row+1, 1);
    }

    if(checkStatus(node->status, LEFT) == 1){
      printf("Printing left\n");
      getTree(node->data, node_size/2, tree, row+1, 0);
    }
  }
}

buddy_t* add_buddy(buddy_t* node_addr, size_t node_size, size_t data_size){
  printf("finished\n");
  printf("====================================================\n");
  printf("add_buddy: %zu %d %d\n", node_addr, node_size, data_size);
  if((node_addr->status & (USED|SPLIT)) == 0 && data_size <= node_size && data_size > (node_size/2)){
      printf("Found free block at: %zu\n", node_addr);
      node_addr->status = USED;
      node_addr->data = &node_addr->status + STATUS_SIZE;
      return node_addr;
  }else if((node_addr->status & USED) == 0){
    printf("Going right\n");
    /*Split the node memory in half*/
    if(checkStatus(node_addr->status, SPLIT) == 0){
      printf("BLOCK not split, splitting.\n");
      node_addr->status = (SPLIT|LEFT|RIGHT);
      printf("status is: %d\n", node_addr->status);
      node_addr->data = &node_addr->status + STATUS_SIZE;
      printf("data is: %d\n", node_addr->status);
    }
    /*Next node_size*/
    node_size = node_size / 2;
    printf("node addr: %zu\n", node_addr);
    /*Advance the current pointer*/
    printf("New node addr: %zu\n", (void*)node_addr + node_size);
    buddy_t* res = add_buddy((void*)node_addr + node_size, node_size, data_size);
    if(res == NULL){
      printf("Going left\n");
      /*Go left instead (closest to the node start address)*/
      res = add_buddy(node_addr->data, node_size, data_size);
    }
    return res;
  }
  printf("No node found\n");
  return NULL;
}

void* my_malloc(size_t size){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Linked list malloc called with size: %d\n", size);
  #endif

  #if MEM_DEBUG
  printf("Program break point at %d\n", sbrk(0));
  #endif

  /*Initialize memory if first round*/
  if(root == NULL){
    #if MEM_DEBUG
    printf("Initializing memory of %d bytes for the first time.\n", MEMORY_SIZE + BUDDY_T_SIZE);
    #endif
    root = heap_alloc(MEMORY_SIZE + STATUS_SIZE);
    if(root == SBRK_FAILED){
      return NULL;
    }

    /*Set the ending address*/
    end_address = sbrk(0);

    root->data = &root->status + STATUS_SIZE;
    printf("Allocated space starts at: %zu\n", root->data);
  }

  size_t real_size = align_size(size) + BUDDY_T_SIZE;

  #if MEM_DEBUG
  printf("Header size: %d\n", BUDDY_T_SIZE);
  printf("Allocation request size: %d\n", real_size - BUDDY_T_SIZE);
  #endif

  buddy_t* new_block = add_buddy(root, MEMORY_SIZE ,real_size);
  printf("New_block at: %zu\n", new_block->data);
  if(new_block == NULL){
    printf("No allocation space left.\n");
    return NULL;
  }

  char* tree[DEPTH][2][PRINT_LENGTH];
  getTree(root, MEMORY_SIZE, tree, 0, 0);
  for (int i = 0; i < DEPTH; i++) {
     for (int j = 0; j < 2; j++ ) {
        printf("tree[%d][%d] = %s\n", i,j, tree[i][j] );
     }
  }

  return new_block->data;

}

void* calloc(size_t nitems, size_t size){

}
void* realloc(void* ptr, size_t size){

}
void free(void* ptr){

}
