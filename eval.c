#include <stdio.h>
#include "cons.h"
#include "object.h"
#include "symtab.h"
#include "eval.h"

object_t *lambda;

void eval_init ()
{
  lambda = c_sym ("lambda");
}

object_t *eval_list (object_t * lst)
{
  if (lst == NIL)
    return NIL;
  return c_cons (eval (CAR (lst)), eval_list (CDR (lst)));
}

object_t *eval (object_t * o)
{
  if (o->type != CONS && o->type != SYMBOL)
    return o;
  else if (o->type == SYMBOL)
    return GET (o);

  /* Find the function. */
  object_t *f = eval (CAR (o));
  if (!IS_FUNC (f))
    {
      printf ("error: not a function: ");
      obj_print (f);
      printf ("\n");
    }


  if (f->type == CFUNC || f->type == CONS)
    {
      object_t *args = eval_list (CDR (o));
      if (f->type == CFUNC)
	{
	  object_t *(*cf) (object_t * o) = f->val;
	  return cf (args);
	}
      else
	{
	  /* list form */
	}
    }
  else
    {
      /* special form or macro */
    }
  return NIL;
}
