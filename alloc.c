#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct metadata_block {
  size_t size;
  size_t bepis;
} metadata_block_t;

typedef struct free_metadata_block {
  size_t size;
  void* addr;
  struct free_metadata_block* next;
  struct free_metadata_block* prev;
} free_block_t;

free_block_t* head;
_Bool head_init_flag = false;

#define WORD 8
#define WORD_ALIGN (size - (size % WORD) + WORD);

void insert_metadata(size_t size) {
  // void** metadata_pointer = (void**)sbrk(WORD);
  // metadata_block_t* metadata = (metadata_block_t*)brk(sizeof(metadata_block_t));
  //metadata->size = size;
  //void* test_metadata = sbrk(WORD);
}

void remove_block(free_block_t* block) {
  block->prev->next = block->next;
  block->next->prev = block->prev;
}

void* search_free_blocks(size_t size) {
  free_block_t* current = head;

  // while (current->next != NULL) {
  //   if (current->size >= size) {
  //     remove_block(current);
  //     return current->addr;
  //   }
  //   current = current->next;
  // }
  return NULL;
}

void* mymalloc(size_t size) {
  //insert_metadata(size);

  if (size == 0) {
    return NULL;
  } else if ((size % WORD) != 0) {
    size = WORD_ALIGN;
  }

  void* free_block = search_free_blocks(size);
  if (free_block == NULL) {
    return sbrk(size);
  }

  return free_block;
}

void* mycalloc(size_t nmemb, size_t size) {
  //insert_metadata(size);
  void* return_pointer = NULL;

  if (nmemb == 0 || size == 0) {
    return NULL;
  } else if ((size % WORD) != 0) {
    size = WORD_ALIGN;
  }

  return_pointer = sbrk(size * nmemb);
  memset(return_pointer, 0, size);

  return return_pointer;
}

void init_head(){
 //head = (free_block_t*)sbrk(sizeof(free_block_t));
 // printf("<!> Init <!> Size of head: %ld\n", sizeof(head));
 head->next = NULL;
 head->size = -1;
}

void myfree(void* ptr) {
  if (!head_init_flag) { init_head(); }

  metadata_block_t* block = (metadata_block_t*)(ptr - WORD);
  size_t size = block->size;

  free_block_t* current = head;
  while (current->next != NULL) {
    current = current->next;
  }

  //current->next = (free_block_t*)sbrk(sizeof(free_block_t));
  //current->next->size = size;
  //current->next->addr = ptr;
  // current->next->next = NULL;
  // current->next->prev = current;
}

void* myrealloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return NULL;
  } else if (size) {
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
