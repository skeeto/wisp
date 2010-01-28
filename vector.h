#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include "object.h"

typedef struct vector
{
  object_t **v;
  size_t len;
} vector_t;

/* standard object functions */
void vector_init ();
vector_t *vector_create ();
void vector_destroy (vector_t * o);

/* General vector creation. */
object_t *c_vec (size_t len);

/* Convert list to vector */
object_t *list2vector (object_t * lst);

/* Access objects in a vector. */
void vset (object_t * vo, size_t i, object_t * val);
object_t *vget (object_t * vo, size_t i);

/* Print a vector */
void vec_print (object_t * vo);

#endif /* VECTOR_H */
