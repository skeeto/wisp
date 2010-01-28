#include <stdio.h>
#include "cons.h"
#include "object.h"
#include "symtab.h"
#include "eval.h"
#include "number.h"

object_t *lambda, *macro, *quote;
object_t *err_symbol, *err_thrown, *err_attach;
object_t *rest, *optional;
/* Commonly used thrown error symbols */
object_t *void_function, *wrong_number_of_arguments, *wrong_type,
  *improper_list, *improper_list_ending;

/* Stack counting */
unsigned int stack_depth = 0, max_stack_depth = 20000;

void eval_init ()
{
  /* regular evaluation symbols */
  lambda = c_sym ("lambda");
  macro = c_sym ("macro");
  quote = c_sym ("quote");
  rest = c_sym ("&rest");
  optional = c_sym ("&optional");

  /* error symbols */
  err_symbol = c_usym ("wisp-error");
  SET (err_symbol, err_symbol);
  err_thrown = err_attach = NIL;
  void_function = c_sym ("void-function");
  wrong_number_of_arguments = c_sym ("wrong-number-of-arguments");
  wrong_type = c_sym ("wrong-type-argument");
  improper_list = c_sym ("improper-list");
  improper_list_ending = c_sym ("improper-list-ending");
}

object_t *eval_list (object_t * lst)
{
  if (lst == NIL)
    return NIL;
  if (!CONSP (lst))
    THROW (improper_list_ending, UPREF (lst));
  object_t *car = eval (CAR (lst));
  CHECK (car);
  object_t *cdr = eval_list (CDR (lst));
  if (cdr == err_symbol)
    {
      obj_destroy (car);
      return err_symbol;
    }
  return c_cons (car, cdr);
}

object_t *eval_body (object_t * body)
{
  object_t *r = NIL;
  while (body != NIL)
    {
      obj_destroy (r);
      r = eval (CAR (body));
      CHECK (r);
      body = CDR (body);
    }
  return r;
}

object_t *assign_args (object_t * vars, object_t * vals)
{
  int optional_mode = 0;
  int cnt = 0;
  object_t *orig_vars = vars;
  while (vars != NIL)
    {
      object_t *var = CAR (vars);
      if (var == optional)
	{
	  /* Turn on optional mode and continue. */
	  optional_mode = 1;
	  vars = CDR (vars);
	  continue;
	}
      if (var == rest)
	{
	  /* Assign the rest of the list and finish. */
	  vars = CDR (vars);
	  sympush (CAR (vars), vals);
	  vals = NIL;
	  break;
	}
      else if (!optional_mode && vals == NIL)
	{
	  while (cnt > 0)
	    {
	      sympop (CAR (orig_vars));
	      orig_vars = CDR (orig_vars);
	      cnt--;
	    }
	  THROW (wrong_number_of_arguments, NIL);
	}
      else if (optional_mode && vals == NIL)
	{
	  sympush (var, NIL);
	}
      else
	{
	  object_t *val = CAR (vals);
	  sympush (var, val);
	  cnt++;
	}
      vars = CDR (vars);
      if (vals != NIL)
	vals = CDR (vals);
    }

  /* vals should be consumed by now */
  if (vals != NIL)
    {
      unassign_args (vars);
      THROW (wrong_number_of_arguments, NIL);
    }
  return T;
}

void unassign_args (object_t * vars)
{
  if (vars == NIL)
    return;
  object_t *var = CAR (vars);
  if (var != rest && var != optional)
    sympop (var);
  unassign_args (CDR (vars));
}

object_t *top_eval (object_t * o)
{
  stack_depth = 0;
  object_t *r = eval (o);
  if (r == err_symbol)
    {
      printf ("Wisp error: ");
      object_t *c = c_cons (err_thrown, err_attach);
      obj_print (c, 1);
      obj_destroy (c);
      return err_symbol;
    }
  return r;
}

object_t *eval (object_t * o)
{
  if (o->type != CONS && o->type != SYMBOL)
    return UPREF (o);
  else if (o->type == SYMBOL)
    return UPREF (GET (o));

  /* Find the function. */
  object_t *f = eval (CAR (o));
  CHECK (f);
  if (!FUNCP (f))
    {
      obj_destroy (f);
      THROW (void_function, UPREF (CAR (o)));
    }

  /* Check the stack */
  if (++stack_depth >= max_stack_depth)
    THROW (c_sym ("max-stack-depth"), c_int (stack_depth--));

  if (f->type == CFUNC || (f->type == CONS && (CAR (f) == lambda)))
    {
      /* c function or list function (eval args) */
      object_t *args = eval_list (CDR (o));
      if (args == err_symbol)
	{
	  stack_depth--;
	  return err_symbol;
	}
      else if (f->type == CFUNC)
	{
	  /* c function */
	  object_t *(*cf) (object_t * o) = f->val;
	  object_t *r = cf (args);
	  obj_destroy (args);
	  stack_depth--;
	  return r;
	}
      else
	{
	  /* list form */
	  object_t *vars = CAR (CDR (f));
	  object_t *assr = assign_args (vars, args);
	  if (assr == err_symbol)
	    {
	      obj_destroy (args);
	      err_attach = UPREF (CAR (o));
	      stack_depth--;
	      return err_symbol;
	    }
	  object_t *r = eval_body (CDR (CDR (f)));
	  unassign_args (vars);
	  obj_destroy (args);
	  stack_depth--;
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
	  stack_depth--;
	  return r;
	}
      else if (f->type == CONS)
	{
	  /* list form macro */
	  object_t *vars = CAR (CDR (f));
	  object_t *assr = assign_args (vars, args);
	  if (assr == err_symbol)
	    {
	      err_attach = UPREF (CAR (o));
	      obj_destroy (args);
	      stack_depth--;
	      return err_symbol;
	    }
	  object_t *body = eval_body (CDR (CDR (f)));
	  object_t *r = eval (body);
	  unassign_args (vars);
	  obj_destroy (body);
	  stack_depth--;
	  return r;
	}
    }
  stack_depth--;
  return NIL;
}
