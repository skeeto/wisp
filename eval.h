#ifndef EVAL_H
#define EVAL_H

#include "cons.h"
#include "object.h"

/* Must be called before calling any other functions. */
void eval_init ();

object_t *eval (object_t * o);
object_t *eval_body (object_t *body);

extern object_t *lambda;	/* the lambda symbol */

#define IS_FUNC(o) \
  ((o->type == CONS && CAR(o)->type == SYMBOL && sym_eq(CAR(o), lambda)) \
   || (o->type == CFUNC) || (o->type == CMACRO) || (o->type == SPECIAL))

#endif /* EVAL_H */
