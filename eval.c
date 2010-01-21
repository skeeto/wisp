#include <stdio.h>
#include "cons.h"
#include "symtab.h"
#include "eval.h"

static object_t *lambda;

void eval_init()
{
  lambda = c_sym ("lambda");
}

void eval (object_t * o)
{
 
  if (o->type != CONS)
    {
      obj_print (o);
      return;
    }
  object_t *func = CAR(o);
  if (func->type == SYMBOL)
    func = GET (func);
  if (func->type == CONS)
    {
      if ((CAR(func))->type != SYMBOL)
	{
	  printf ("error: not a function: ");
	  obj_print (func);
	  printf ("\n");
	  return;
	}
      if (!sym_eq (lambda, CAR(func)))
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
      f (CDR(o));
    }
}
