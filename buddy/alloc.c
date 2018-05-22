#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "alloc.h"

#define TREE_DEBUG 1
#define MEM_DEBUG 0
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

void getTree(buddy_t* node, size_t node_size, char* tree[DEPTH][MIN_DATA_SIZE][PRINT_LENGTH], short row, short col){
 // printf("row %d col %d\n", row, col);

  if(checkStatus(node->status, FILLED) == 1 && checkStatus(node->status, SPLIT) == 0){
   // printf("Printing used node\n");
    char buffer[35];
    snprintf(buffer, sizeof(buffer), "|Status: %d Addr: %zu |", node->status, &node->data);
    // printf("buff: %s\n", buffer);
    strcpy(tree[row][col], buffer);
    // printf("node copy: %s\n", tree[row][col]);
  }else if(checkStatus(node->status, SPLIT) == 1){
    char buffer[35];
    snprintf(buffer, sizeof(buffer), "|Status: %d Addr: %zu |", node->status, &node->data);
    // printf("buff: %s\n", buffer);
    strcpy(tree[row][col], buffer);
    // printf("node copy: %s\n", tree[row][col]);
      // printf("Printing right\n");
      getTree((void*)node + node_size/2, node_size/2, tree, row+1, (col*2 + 1));

      // printf("Printing left\n");
      getTree(&node->data, node_size/2, tree, row+1, (col*2));
  }
}

void printTree(){
  printf("Minimum data size in tree is: %d\n", MIN_DATA_SIZE);
  char* tree[DEPTH][MIN_DATA_SIZE][PRINT_LENGTH];
  getTree(root, MEMORY_SIZE, tree, 0, 0);
  for (int i = 0; i < DEPTH; i++) {
     for (int j = 0; j < MIN_DATA_SIZE; j++ ) {
       if(strstr(tree[i][j], "|") != NULL){
         printf("tree[%d][%d] = %s\n", i,j, tree[i][j] );
       }
     }
  }
}

buddy_t* add_buddy(buddy_t* node_addr, size_t node_size, size_t data_size){
  printf("====================================================\n");
  printf("add_buddy: %zu %d %d\n", node_addr, node_size, data_size);
  if(checkStatus(node_addr->status, (SPLIT|FILLED)) == 0 && data_size <= node_size && data_size > (node_size/2)){
      printf("Found free block at: %zu\n", node_addr);
      node_addr->status = FILLED;
      printf("New node data byte at: %zu\n", node_addr->data);
      node_addr->data = &node_addr->status + STATUS_SIZE;
      printf("New node data at: %zu\n", node_addr->data);
      return node_addr;
  }else if(checkStatus(node_addr->status, FILLED) == 0){
    printf("Going right\n");
    /*Split the node memory in half*/
    if(checkStatus(node_addr->status, SPLIT) == 0){
      printf("BLOCK not split, splitting.\n");
      node_addr->status = (SPLIT);
      // printf("status is: %d\n", node_addr->status);
      node_addr->data = &node_addr->status + STATUS_SIZE;
      // printf("data is: %zu\n", node_addr->data);
    }
    /*Next node_size*/
    node_size = node_size / 2;
    // printf("node addr: %zu\n", node_addr);
    /*Advance the current pointer*/
    // printf("New node addr: %zu\n", (void*)node_addr + node_size);
    printf("Right - Current node data: %zu\n", node_addr->data);
    buddy_t* res = add_buddy((void*)node_addr + node_size, node_size, data_size);
    if(res == NULL){
      printf("Left - Current node data: %zu\n", node_addr->data);
      printf("Node size: %d", node_size);
      printf("Going left\n");
      /*Go left instead (closest to the node start address)*/
      // printf("left node begins at: %zu\n", node_addr->data);
      res = add_buddy(&node_addr->data, node_size, data_size);
      // printf("left node status at: %zu\n", &res->status);
      // printf("left  node data at: %zu\n", &res->data);
    }

    buddy_t* right_node = (void*)node_addr + node_size;
    buddy_t* left_node = &node_addr->data;
    if(checkStatus(right_node->status, FILLED) == 1 && checkStatus(left_node->status, FILLED) == 1){
      /*The two buddies are fuilled and thus is also the parent*/
      node_addr->status = (node_addr->status | FILLED);
    }
    return res;
  }
  printf("No unused node found.\n");
  return NULL;
}



short merge_buddies(buddy_t* current_node, size_t size){
  printf("====================================================\n");
  printf("merge_buddies start: %zu %d\n", current_node, size);
  buddy_t* right_node = (void*)current_node + size;
  buddy_t* left_node = &current_node->data;
  printf("left_node: %zu\n", right_node);
  printf("right_node: %zu\n", left_node);

  if(checkStatus(current_node->status, (FILLED|SPLIT)) == 0){
    printf("Free node found.\n");
    if(current_node != root){
      /*Only clear data field if not root*/
      current_node->data = NULL;
    }
    return 1;
  }

  int res1 = 0;
  int res2 = 0;
  printf("%zu\n", right_node->status);
  if(right_node->status != 0 && checkStatus(right_node->status, SPLIT) == 1){
    printf("Going right\n");
    /*The two buddies are fuilled and thus is also the parent*/
    res1 = merge_buddies(right_node, size/2);
  }

  if(left_node->status != 0 && checkStatus(left_node->status, SPLIT) == 1){
    printf("Going left\n");
    res2 = merge_buddies(&left_node, size/2);
  }

  if(res1 == 1 && res2 == 1){
    current_node->status = 0;
    return merge_buddies(current_node, size);
  }else{
    return 0;
  }
}

void* my_malloc(size_t size){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Buddy malloc called with size: %d\n", size);
  #endif

  #if MEM_DEBUG
  printf("Program break point at %d\n", sbrk(0));
  #endif

  /*Initialize memory if first round*/
  if(root == NULL){
    #if MEM_DEBUG
    printf("Initializing memory of %d bytes for the first time.\n", MEMORY_SIZE);
    #endif
    root = heap_alloc(MEMORY_SIZE);
    if(root == SBRK_FAILED){
      return NULL;
    }

    /*Set the ending address*/
    end_address = sbrk(0);

    root->data = &root->status + STATUS_SIZE;
  }

  size_t real_size = align_size(size) + BUDDY_T_SIZE;

  #if MEM_DEBUG
  printf("Header size: %d\n", BUDDY_T_SIZE);
  printf("Allocation request size: %d\n", real_size - BUDDY_T_SIZE);
  #endif

  buddy_t* new_block = add_buddy(root, MEMORY_SIZE ,real_size);
  printf("New_block at: %zu\n", new_block);
  printf("Returning data pointer at: %zu\n", &new_block->data);
  if(new_block == NULL){
    printf("No allocation space left.\n");
    return NULL;
  }

  #if TREE_DEBUG
  printTree();
  #endif

  return new_block->data;

}

void* calloc(size_t nitems, size_t size){

}
void* realloc(void* ptr, size_t size){

}
void free(void* ptr){
  #if RUNTIME_DEBUG
  printf("=======================================================================\n");
  printf("Free called on address: %zu\n", ptr);
  #endif
  buddy_t* target_node = (void*)ptr - 1;
  if(checkStatus(target_node->status, FILLED) == 1){
    printf("Found occupied node at: %zu\n", target_node);
    target_node->status = 0;
    target_node->data = NULL;

    /*Recursively merge parent nodes which are free*/
    merge_buddies(root, MEMORY_SIZE/2);

    #if TREE_DEBUG
    printTree();
    #endif
  }
}
