/* cons.h - cons cell and list functions */
#ifndef CONS_H
#define CONS_H

#include "mem.h"
#include "object.h"

typedef struct cons
{
  object_t *car;
  object_t *cdr;
} cons_t;

/* Must be called before any other functions. */
void cons_init ();

/* Memory management */
cons_t *cons_create ();
void cons_destroy (cons_t * c);

/* list operators */
object_t *cons (object_t * o, object_t * c);
object_t *cdr (object_t * o);
object_t *car (object_t * o);
object_t *setcar (object_t * o, object_t * car);


void cons_print (object_t * o);

#endif /* CONS_H */
