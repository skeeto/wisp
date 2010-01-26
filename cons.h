/* cons.h - cons cell and list functions */
#ifndef CONS_H
#define CONS_H

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
int length (object_t * lst);
int is_func_form (object_t * lst);
int is_var_list (object_t * lst);

#define CAR(o) ((cons_t *) ((o)->val))->car
#define CDR(o) ((cons_t *) ((o)->val))->cdr

#define LISTP(o) (o->type == CONS || o == NIL)
#define PAIRP(o) (o->type == CONS && !LISTP(CDR(o)))

#endif /* CONS_H */
