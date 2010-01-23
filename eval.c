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

object_t *eval_body (object_t *body)
{
  object_t *r = NIL;
  while (body != NIL)
    {
      r = eval (CAR (body));
      body = CDR (body);
    }
  return r;
}

void assign_args (object_t * vars, object_t * vals)
{
  if (vars == NIL)
    return;
  sympush (CAR (vars), CAR (vals));
  assign_args (CDR (vars), CDR (vals));
}

void unassign_args (object_t * vars)
{
  if (vars == NIL)
    return;
  sympop (CAR (vars));
  unassign_args (CDR (vars));
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
      /* c function or list function (eval args) */
      object_t *args = eval_list (CDR (o));
      if (f->type == CFUNC)
	/* c function */
	{
	  object_t *(*cf) (object_t * o) = f->val;
	  return cf (args);
	}
      else
	{
	  /* list form */
	  object_t *vars = CAR (CDR (f));
	  assign_args (vars, args);
	  object_t *r = eval_body (CDR (CDR (f)));
	  unassign_args (vars);
	  return r;
	}
    }
  else
    {
      /* special form or macro */
      object_t *args = CDR (o);
      if (f->type == CMACRO)
	{
	  /* c macro */
	  object_t *(*cf) (object_t * o) = f->val;
	  return cf (args);
	}
      else if (f->type == SPECIAL)
	{
	  /* special form */
	  object_t *(*cf) (object_t * o) = f->val;
	  return cf (args);
	}
    }
  return NIL;
}
