#include <stdio.h>
#include "cons.h"
#include "object.h"
#include "symtab.h"
#include "eval.h"

object_t *lambda, *macro;
object_t *err_symbol, *err_thrown, *err_attach;

void eval_init ()
{
  lambda = c_sym ("lambda");
  macro = c_sym ("macro");
  err_symbol = c_sym ("_error");
  err_thrown = err_attach = NIL;
}

object_t *eval_list (object_t * lst)
{
  if (lst == NIL)
    return NIL;
  return c_cons (eval (CAR (lst)), eval_list (CDR (lst)));
}

object_t *eval_body (object_t * body)
{
  object_t *r = NIL;
  while (body != NIL)
    {
      obj_destroy (r);
      r = eval (CAR (body));
      body = CDR (body);
    }
  return r;
}

void assign_args (object_t * vars, object_t * vals)
{
  if (vars == NIL)
    return;
  if (vals != NIL)
    sympush (CAR (vars), CAR (vals));
  else
    sympush (CAR (vars), NIL);
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
    return UPREF (o);
  else if (o->type == SYMBOL)
    return UPREF (GET (o));

  /* Find the function. */
  object_t *f = eval (CAR (o));
  if (!FUNCP (f))
    {
      printf ("error: not a function: ");
      obj_print (CAR (o), 1);
      obj_destroy (f);
      return NIL;
    }

  if (f->type == CFUNC || (f->type == CONS && sym_eq (CAR (f), lambda)))
    {
      /* c function or list function (eval args) */
      object_t *args = eval_list (CDR (o));
      if (f->type == CFUNC)
	/* c function */
	{
	  object_t *(*cf) (object_t * o) = f->val;
	  object_t *r = cf (args);
	  obj_destroy (args);
	  return r;
	}
      else
	{
	  /* list form */
	  object_t *vars = CAR (CDR (f));
	  assign_args (vars, args);
	  object_t *r = eval_body (CDR (CDR (f)));
	  unassign_args (vars);
	  obj_destroy (args);
	  return r;
	}
    }
  else
    {
      /* special form or macro */
      object_t *args = CDR (o);
      if (f->type == SPECIAL)
	{
	  /* special form */
	  object_t *(*cf) (object_t * o) = f->val;
	  object_t *r = cf (args);
	  return r;
	}
      else if (f->type == CONS)
	{
	  /* list form macro */
	  object_t *vars = CAR (CDR (f));
	  assign_args (vars, args);
	  object_t *body = eval_body (CDR (CDR (f)));
	  object_t *r = eval (body);
	  unassign_args (vars);
	  obj_destroy (body);
	  return r;
	}
    }
  return NIL;
}
