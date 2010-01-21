#include <stdio.h>
#include "cons.h"
#include "symtab.h"
#include "eval.h"

void eval (object_t * o)
{
  object_t *lambda = c_sym ("lambda");

  if (o->type != CONS)
    {
      obj_print (o);
      return;
    }
  object_t *func = car(o);
  if (func->type == SYMBOL)
    func = get_value (func);
  if (func->type == CONS)
    {
      if ((car(func))->type != SYMBOL)
	{
	  printf ("error: not a function: ");
	  obj_print (func);
	  printf ("\n");
	  return;
	}
      if (!sym_eq (lambda, car(func)))
	{
	  printf ("error: not a function: ");
	  obj_print (func);
	  printf ("\n");
	  return;
	}
      else
	{
	  /* lisp code */
	  /* assign stuff here */
	  /* recurse */
	}
    }
  if (func->type == CFUNC)
    {
      object_t *(*f) (object_t * o) = func->val;
      f (cdr (o));
    }
}
