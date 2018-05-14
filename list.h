#ifndef sh_list
#define sh_list

typedef struct node_t node_t

typedef struct node{
  int blockID;
  short used;
  node_t* head;
  node_t* tail;
  void *data
};

node_t* new_list(void*);
void append(node_t** node1, node_t node2);
void delete_node(node_t* node);


#endif
