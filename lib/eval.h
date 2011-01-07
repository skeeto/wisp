/* eval.h - list evaluation/execution functions */
#ifndef EVAL_H
#define EVAL_H

#include "cons.h"
#include "object.h"
#include "common.h"
#include "str.h"

/* Initializes everything. */
void wisp_init ();

/* Must be called before calling any other functions. */
void eval_init ();

object_t *eval (object_t * o);
object_t *top_eval (object_t * o);
object_t *eval_body (object_t * body);
object_t *assign_args (object_t * vars, object_t * vals);
void unassign_args (object_t * vars);
object_t *apply (object_t * f, object_t * rawargs);

extern unsigned int stack_depth, max_stack_depth;
extern int interactive_mode;

/* Frequently used symbols */
extern object_t *lambda, *macro, *rest, *optional, *quote;
extern object_t *doc_string;

#define FUNCP(o) \
  ((o->type == CONS && CAR(o)->type == SYMBOL \
    && ((CAR(o) == lambda) || (CAR(o) == macro))) \
   || (o->type == CFUNC) || (o->type == SPECIAL))

/* Error handling */
extern object_t *err_symbol, *err_thrown, *err_attach;
extern object_t *void_function, *wrong_number_of_arguments, *wrong_type,
  *improper_list, *improper_list_ending, *err_interrupt;

#define THROW(to, ao) {err_thrown = to; err_attach = ao; return err_symbol;}
#define CHECK(o) if ((o) == err_symbol) return err_symbol;

#define REQ(lst, n, so) if (req_length (lst, so, n) == err_symbol) \
                          return err_symbol;
#define REQM(lst, n, so) if (reqm_length (lst, so, n) == err_symbol) \
                         return err_symbol;
#define REQX(lst, n, so) if (reqx_length (lst, so, n) == err_symbol) \
                         return err_symbol;
#define REQPROP(lst) if (properlistp(lst) == NIL) \
                         THROW (improper_list, lst);
#define DOC(str) if (lst == doc_string) return c_strs (xstrdup (str));

#endif /* EVAL_H */
