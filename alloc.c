#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct metadata_block {
  size_t size;
  _Bool is_free;
  struct metadata_block* next;
  struct metadata_block* prev;
} metadata_block_t;

typedef struct free_metadata_block {
  void* addr;
  struct free_metadata_block* next;
} free_block_t;

metadata_block_t* metadata_head;
free_block_t* free_head;
_Bool metadata_head_init_flag = false;
_Bool free_head_init_flag = false;

#define WORD 8
#define WORD_ALIGN (size - (size % WORD) + WORD);

void init_metadata_head(size_t size) {
  metadata_head_init_flag = true;
  metadata_head = (metadata_block_t*)sbrk(sizeof(metadata_block_t));
  metadata_head->size = size;
  metadata_head->is_free = false;
  metadata_head->next = NULL;
  metadata_head->prev = NULL;
}

void insert_metadata(size_t size) {
  if (!metadata_head_init_flag) {
    init_metadata_head(size);
    return;
  }

  metadata_block_t* current = metadata_head;

  while (current->next != NULL) {
    //printf("Metadata insert size: %ld\n", current->size);
    current = current->next;
  }

  current->next = (metadata_block_t*)sbrk(sizeof(metadata_block_t));
  current->next->size = size;
  current->next->is_free = false;
  current->next->next = NULL;
  current->next->prev = current;
}

void remove_block(free_block_t* free_block, metadata_block_t* metadata_block) {
  metadata_block->prev->next = metadata_block->next;
  metadata_block->next->prev = metadata_block->prev;
}

// defragment_free(void* addr){
//   metadata_block_t* metadata_next = (metadata_block_t*)(addr + sizeof(free_block_t)) - 1;
//   metadata_block_t* metadata_prev = (metadata_block_t*)metadata_next->prev;
//   metadata_prev->size += sizeof(free_block_t);
// }

void* search_free_blocks(size_t size) {
  if (free_head == NULL) { return NULL; }

  printf("<!>NEW SEARCH<!>\n");
  printf(">\n");
  printf(">\n");

  printf("Size searched for: %ld\n", size);

  int count = 0;

  free_block_t* current = free_head;

  while (current->next != NULL) {
    metadata_block_t* current_metadata = current->addr - sizeof(metadata_block_t);
    //printf("Size to fit: %ld\n", size);
    //printf("Current metadata size: %ld\n", current_metadata->size);
    if (current_metadata->size >= size) {
      remove_block(current, current_metadata);
      //defragment_free(current->addr);
      printf("--> Size match!\n");
      return current_metadata;
    }
    current = current->next;
  }
  printf("--> Free search end\n");
  return NULL;
}

void* mymalloc(size_t size) {
  printf("Malloced: %ld\n", size);

  if (size == 0) {
    return NULL;
  } else if ((size % WORD) != 0) {
    size = WORD_ALIGN;
  }

  void* free_block = search_free_blocks(size);
  if (free_block == NULL) {
    insert_metadata(size);
    return sbrk(size);
  }

  return free_block;
}

void* mycalloc(size_t nmemb, size_t size) {
  insert_metadata(size);
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

void init_free_head(void* addr) {
  free_head_init_flag = true;
  free_head = (free_block_t*)sbrk(sizeof(free_block_t));
  free_head->addr = addr;
  free_head->next = NULL;
}

void myfree(void* ptr) {
  if (!free_head_init_flag) {
    init_free_head(ptr);
    return;
   }

   //printf("<!>Myfree called\n");

  free_block_t* current = free_head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (free_block_t*)sbrk(sizeof(free_block_t));
  current->next->addr = ptr;
  current->next->next = NULL;

  metadata_block_t* metadata = ptr - sizeof(metadata_block_t);
  metadata->is_free = true;
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
