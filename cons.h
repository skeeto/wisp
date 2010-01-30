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
object_t *req_length (object_t * lst, object_t * thr, int n);	/* exact */
object_t *reqm_length (object_t * lst, object_t * thr, int n);	/* min */
object_t *reqx_length (object_t * lst, object_t * thr, int n);	/* max */
int is_func_form (object_t * lst);
int is_var_list (object_t * lst);

#define CAR(o) ((cons_t *) ((o)->val))->car
#define CDR(o) ((cons_t *) ((o)->val))->cdr

#define LISTP(o) (o->type == CONS || o == NIL)
#define PAIRP(o) (o->type == CONS && !LISTP(CDR(o)))

/* Determine if list is proper. */
object_t *properlistp (object_t * t);

uint32_t cons_hash (object_t * o);

#endif /* CONS_H */
