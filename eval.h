/* eval.h - list evaluation/execution functions */
#ifndef EVAL_H
#define EVAL_H

#include "cons.h"
#include "object.h"

/* Must be called before calling any other functions. */
void eval_init ();

object_t *eval (object_t * o);
object_t *top_eval (object_t * o);
object_t *eval_body (object_t * body);
object_t *assign_args (object_t * vars, object_t * vals);
void unassign_args (object_t * vars);

extern unsigned int max_stack_depth;

/* Frequently used symbols */
extern object_t *lambda, *macro, *rest, *optional, *quote;

#define FUNCP(o) \
  ((o->type == CONS && CAR(o)->type == SYMBOL \
    && ((CAR(o) == lambda) || (CAR(o) == macro))) \
   || (o->type == CFUNC) || (o->type == SPECIAL))

/* Error handling */
extern object_t *err_symbol, *err_thrown, *err_attach;
extern object_t *void_function, *wrong_number_of_arguments, *wrong_type;

#define THROW(to, ao) {err_thrown = to; err_attach = ao; return err_symbol;}
#define CHECK(o) if ((o) == err_symbol) return err_symbol;

#define REQ(lst, n, so) if (length (lst) != n) \
                         THROW(wrong_number_of_arguments, so);
#define REQM(lst, n, so) if (length (lst) < n) \
                         THROW(wrong_number_of_arguments, so);

#endif /* EVAL_H */
