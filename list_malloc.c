#include <unistd.h>
#include "list.h"



void* list_malloc(size_t size){
  printf("Linked list malloc called with size: %d\n", size);
  printf("Program starts here %d\n", sbrk(0));

  /*Increment the program break point to allocate memory*/
  void* startaddress = sbrk(size); //startaddress = sbrk(0)

  if(startaddress == SBRK_FAILED){
    printf("Failed to increment break point. Closing.");
    return NULL; //Returns NULL for caller to handle
  }

  /*Call new_list() and get a node to start of list*/
  block_t* list = new_list(size, startaddress); //This should be some kind of global variable?

  if(list == NULL){
    return NULL;
  }

}






int main(){
  int size = 10*sizeof(int);
  int* p1 = list_malloc(size);

  if(p1){
    printf("Houston we have a pointer.\n");
    /*for(int n=0; n<4; ++n) // populate the array
      p1[n] = n*n;
    for(int n=0; n<4; ++n) // print it back out
      printf("p1[%d] == %d\n", n, p1[n]);*/
  }else{
    printf("Returned with an error in main.\n");
  }
}
