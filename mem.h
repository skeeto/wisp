#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

typedef struct mmanager
{
  size_t osize;
  void **stack, **base;
  size_t size;
  void (*clearf) (void *o);
} mmanager_t;

/* Creates a new memory manager. */
mmanager_t *mm_create (size_t osize, void (*clear_func) (void *o));

/* Free a memory manager */
void mm_destroy (mmanager_t * mm);

/* Allocate and free a new object */
void *mm_alloc (mmanager_t * mm);
void mm_free (mmanager_t * mm, void *o);

#endif
