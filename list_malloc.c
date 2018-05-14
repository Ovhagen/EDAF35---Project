#include <unistd.h>


#define SBRK_FAILED ((void *)-1)

void* list_malloc(size_t size){
  printf("Linked list malloc called with size: %d\n", size);
  void* prev_break = sbrk(0);
  /*Increment the program break point to allocate memory*/
  void* startaddress = sbrk(size);

  if(startaddress == SBRK_FAILED){
    printf("Failed to increment break point. Closing.");
    return NULL; //Returns NULL for caller to handle
  }

  printf("Previous start: %d\n", prev_break);
  printf("The program starts here: %d\n", startaddress);
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
