#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BYTE 8

void* mymalloc(size_t size){
  if (size == 0){
    return NULL;
  } else if ((size % BYTE) != 0){
    size = (size - (size % BYTE) + BYTE);
  }

  return sbrk(size);
}

void* mycalloc(size_t nmemb, size_t size){
  if (nmemb == 0) {
    return NULL;
  }

  size = size * nmemb;
  void* return_pointer = sbrk(size);
  memset(return_pointer, 0, size);

  return return_pointer;
}

void myfree(void* ptr){
  if (ptr == NULL) {
    printf("Yeet");
  }
}

void* myrealloc(void *ptr, size_t size){
  if (ptr == NULL) {
    return NULL;
  } else if (size){
    return NULL;
  }
  return NULL;
}


/*
 * Enable the code below to enable system allocator support for your allocator.
 * Doing so will make debugging much harder (e.g., using printf may result in
 * infinite loops).
 */
#if 0
void *malloc(size_t size) { return mymalloc(size); }
void *calloc(size_t nmemb, size_t size) { return mycalloc(nmemb, size); }
void *realloc(void *ptr, size_t size) { return myrealloc(ptr, size); }
void free(void *ptr) { myfree(ptr); }
#endif
