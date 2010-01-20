/* cons.h - cons cell and list functions */
#ifndef CONS_H
#define CONS_H

#include "mem.h"
#include "object.h"

typedef struct cons
{
  object_t *left;
  object_t *right;
} cons_t;

/* Must be called before any other functions. */
void cons_init ();

/* Memory management */
cons_t *cons_create ();
void cons_destroy (cons_t * c);

/* Construct a list */
object_t *cons (object_t * o, object_t * c);

void setleft (object_t * o, object_t * left);

void cons_print (object_t * o);

#endif /* CONS_H */
