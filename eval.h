/* eval.h - list evaluation/execution functions */
#ifndef EVAL_H
#define EVAL_H

#include "cons.h"
#include "object.h"

/* Must be called before calling any other functions. */
void eval_init ();

object_t *eval (object_t * o);
object_t *eval_body (object_t * body);
void assign_args (object_t * vars, object_t * vals);
void unassign_args (object_t * vars);

extern object_t *lambda;	/* the lambda symbol */
extern object_t *macro;		/* the macro symbol */

#define FUNCP(o) \
  ((o->type == CONS && CAR(o)->type == SYMBOL \
    && (sym_eq(CAR(o), lambda) || sym_eq(CAR(o), macro))) \
   || (o->type == CFUNC) || (o->type == SPECIAL))

/* Error handling */
extern object_t *err_symbol, *err_thrown, *err_attach;

#define THROW(to, ao) err_thrown = to; err_attach = ao; return err_symbol
#define CHECK(o) if (o == err_symbol) return err_symbol;

#endif /* EVAL_H */
