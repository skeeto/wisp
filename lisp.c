#include <stdio.h>
#include <string.h>
#include "object.h"
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"

/* Maths */
object_t *addition (object_t * lst)
{
  REQM (lst, 1, c_sym ("+"));
  double accum = 0;
  int intmode = 1;
  object_t *p = lst;
  while (p != NIL)
    {
      if (CAR (p)->type == FLOAT)
	{
	  intmode = 0;
	  accum += OFLOAT (CAR (p));
	}
      else if (CAR (p)->type == INT)
	accum += OINT (CAR (p));
      else
	THROW (wrong_type, UPREF (CAR (p)));
      p = CDR (p);
    }
  if (intmode)
    return c_int ((int) accum);
  return c_float ((double) accum);
}

object_t *multiply (object_t * lst)
{
  REQM (lst, 1, c_sym ("*"));
  double accum = 1;
  int intmode = 1;
  object_t *p = lst;
  while (p != NIL)
    {
      if (CAR (p)->type == FLOAT)
	{
	  intmode = 0;
	  accum *= OFLOAT (CAR (p));
	}
      else if (CAR (p)->type == INT)
	accum *= OINT (CAR (p));
      else
	THROW (wrong_type, UPREF (CAR (p)));
      p = CDR (p);
    }
  if (intmode)
    return c_int ((int) accum);
  return c_float ((double) accum);
}

object_t *subtract (object_t * lst)
{
  REQM (lst, 1, c_sym ("-"));
  object_t *p = lst;
  double accum = 0;
  int intmode = 1;
  if (CAR (p)->type == INT)
    accum = OINT (CAR (p));
  else if (CAR (p)->type == FLOAT)
    {
      intmode = 0;
      accum = OFLOAT (CAR (p));
    }

  p = CDR (p);
  while (p != NIL)
    {
      if (CAR (p)->type == FLOAT)
	{
	  intmode = 0;
	  accum -= OFLOAT (CAR (p));
	}
      else if (CAR (p)->type == INT)
	accum -= OINT (CAR (p));
      else
	THROW (wrong_type, UPREF (CAR (p)));
      p = CDR (p);
    }
  if (intmode)
    return c_int ((int) accum);
  return c_float ((double) accum);
}

object_t *divide (object_t * lst)
{
  REQM (lst, 1, c_sym ("/"));
  object_t *p = lst;
  double accum = 0;
  int intmode = 1;
  if (CAR (p)->type == INT)
    accum = OINT (CAR (p));
  else if (CAR (p)->type == FLOAT)
    {
      intmode = 0;
      accum = OFLOAT (CAR (p));
    }

  p = CDR (p);
  while (p != NIL)
    {
      if (CAR (p)->type == FLOAT)
	{
	  intmode = 0;
	  accum /= OFLOAT (CAR (p));
	}
      else if (CAR (p)->type == INT)
	accum /= OINT (CAR (p));
      else
	THROW (wrong_type, UPREF (CAR (p)));
      p = CDR (p);
    }
  if (intmode)
    return c_int ((int) accum);
  return c_float ((double) accum);
}

object_t *less_than (object_t * lst)
{
  REQ (lst, 2, c_sym ("<"));
  object_t *ao = CAR (lst);
  object_t *bo = CAR (CDR (lst));
  double a = 0, b = 0;
  if (ao->type == INT)
    a = OINT (ao);
  else if (ao->type == FLOAT)
    a = OFLOAT (ao);
  if (bo->type == INT)
    b = OINT (bo);
  else if (bo->type == FLOAT)
    b = OFLOAT (bo);
  else
    THROW (wrong_type, UPREF (bo));
  if (a < b)
    return T;
  return NIL;
}

object_t *less_than_or_eq (object_t * lst)
{
  REQ (lst, 2, c_sym ("<="));
  object_t *ao = CAR (lst);
  object_t *bo = CAR (CDR (lst));
  double a = 0, b = 0;
  if (ao->type == INT)
    a = OINT (ao);
  else if (ao->type == FLOAT)
    a = OFLOAT (ao);
  if (bo->type == INT)
    b = OINT (bo);
  else if (bo->type == FLOAT)
    b = OFLOAT (bo);
  if (a <= b)
    return T;
  return NIL;
}

object_t *greater_than (object_t * lst)
{
  REQ (lst, 2, c_sym (">"));
  object_t *ao = CAR (lst);
  object_t *bo = CAR (CDR (lst));
  double a = 0, b = 0;
  if (ao->type == INT)
    a = OINT (ao);
  else if (ao->type == FLOAT)
    a = OFLOAT (ao);
  if (bo->type == INT)
    b = OINT (bo);
  else if (bo->type == FLOAT)
    b = OFLOAT (bo);
  if (a > b)
    return T;
  return NIL;
}

object_t *greater_than_or_eq (object_t * lst)
{
  REQ (lst, 2, c_sym (">="));
  object_t *ao = CAR (lst);
  object_t *bo = CAR (CDR (lst));
  double a = 0, b = 0;
  if (ao->type == INT)
    a = OINT (ao);
  else if (ao->type == FLOAT)
    a = OFLOAT (ao);
  if (bo->type == INT)
    b = OINT (bo);
  else if (bo->type == FLOAT)
    b = OFLOAT (bo);
  if (a >= b)
    return T;
  return NIL;
}

object_t *numeq (object_t * lst)
{
  REQ (lst, 2, c_sym ("="));
  object_t *ao = CAR (lst);
  object_t *bo = CAR (CDR (lst));
  double a = 0, b = 0;
  if (ao->type == INT)
    a = OINT (ao);
  else if (ao->type == FLOAT)
    a = OFLOAT (ao);
  else
    THROW (wrong_type, UPREF (ao));
  if (bo->type == INT)
    b = OINT (bo);
  else if (bo->type == FLOAT)
    b = OFLOAT (bo);
  else
    THROW (wrong_type, UPREF (bo));
  if (a == b)
    return T;
  return NIL;
}

/* Various basic stuff */

object_t *lisp_cons (object_t * lst)
{
  REQ (lst, 2, c_sym ("cons"));
  return c_cons (UPREF (CAR (lst)), UPREF (CAR (CDR (lst))));
}

object_t *quote (object_t * lst)
{
  REQ (lst, 1, c_sym ("quote"));
  return UPREF (CAR (lst));
}

object_t *lambda_f (object_t * lst)
{
  if (!is_func_form (lst))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  return c_cons (lambda, UPREF (lst));
}

object_t *defun (object_t * lst)
{
  if (!is_func_form (CDR (lst)))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  object_t *f = c_cons (lambda, UPREF (CDR (lst)));
  SET (CAR (lst), f);
  return f;
}

object_t *defmacro (object_t * lst)
{
  if (!is_func_form (CDR (lst)))
    THROW (c_sym ("bad-function-form"), UPREF (lst));
  object_t *f = c_cons (macro, UPREF (CDR (lst)));
  SET (CAR (lst), f);
  return f;
}

object_t *lisp_cdr (object_t * lst)
{
  REQ (lst, 1, c_sym ("cdr"));
  return UPREF (CDR (CAR (lst)));
}

object_t *lisp_car (object_t * lst)
{
  REQ (lst, 1, c_sym ("car"));
  return UPREF (CAR (CAR (lst)));
}

object_t *lisp_list (object_t * lst)
{
  return UPREF (lst);
}

object_t *lisp_if (object_t * lst)
{
  REQM (lst, 2, wrong_number_of_arguments);
  object_t *r = eval (CAR (lst));
  CHECK (r);
  if (r != NIL)
    {
      obj_destroy (r);
      return eval (CAR (CDR (lst)));
    }
  return eval_body (CDR (CDR (lst)));
}

object_t *progn (object_t * lst)
{
  return eval_body (lst);
}

object_t *let (object_t * lst)
{
  /* verify structure */
  if (!LISTP (CAR (lst)))
    THROW (c_sym ("bad-let-form"), UPREF (lst));
  object_t *vlist = CAR (lst);
  while (vlist != NIL)
    {
      object_t *p = CAR (vlist);
      if (!LISTP (p))
	THROW (c_sym ("bad-let-form"), UPREF (lst));
      if (!SYMBOLP (CAR (p)))
	THROW (c_sym ("bad-let-form"), UPREF (lst));
      vlist = CDR (vlist);
    }

  object_t *p;
  p = vlist = CAR (lst);
  int cnt = 0;
  while (p != NIL)
    {
      object_t *pair = CAR (p);
      object_t *e = eval (CAR (CDR (pair)));
      if (e == err_symbol)
	{
	  /* Undo scoping */
	  p = vlist;
	  while (cnt)
	    {
	      sympop (CAR (CAR (p)));
	      p = CDR (p);
	      cnt--;
	    }
	  return err_symbol;
	}
      sympush (CAR (pair), e);
      obj_destroy (e);
      p = CDR (p);
      cnt++;
    }
  object_t *r = eval_body (CDR (lst));
  p = vlist;
  while (p != NIL)
    {
      object_t *pair = CAR (p);
      sympop (CAR (pair));
      p = CDR (p);
    }
  return r;
}

object_t *lisp_while (object_t * lst)
{
  REQM (lst, 1, c_sym ("while"));
  object_t *r = NIL, *cond = CAR (lst), *body = CDR (lst);
  object_t *condr;
  while ((condr = eval (cond)) != NIL)
    {
      obj_destroy (r);
      obj_destroy (condr);
      r = eval_body (body);
    }
  return r;
}

/* Equality */

object_t *eq (object_t * lst)
{
  REQ (lst, 2, c_sym ("eq"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (a == b)
    return T;
  return NIL;
}

object_t *eql (object_t * lst)
{
  REQ (lst, 2, c_sym ("eql"));
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (a->type != b->type)
    return NIL;
  switch (a->type)
    {
    case INT:
      if (OINT (a) == OINT (b))
	return T;
      break;
    case FLOAT:
      if (OFLOAT (a) == OFLOAT (b))
	return T;
      break;
    case SYMBOL:
    case CONS:
      if (a == b)
	return T;
      break;
    case STRING:
      if (strcmp (OSTR (a), OSTR (b)) == 0)
	return T;
      break;
    case CFUNC:
    case SPECIAL:
      if (a->val == b->val)
	return T;
      break;
    }
  return NIL;
}

object_t *lisp_print (object_t * lst)
{
  REQ (lst, 1, c_sym ("print"));
  obj_print (CAR (lst), 1);
  return NIL;
}

/* Symbol table */

object_t *lisp_set (object_t * lst)
{
  REQ (lst, 2, c_sym ("set"));
  if (!SYMBOLP (CAR (lst)))
    THROW (wrong_type, c_cons (c_sym ("set"), CAR (lst)));

  SET (CAR (lst), CAR (CDR (lst)));
  return UPREF (CAR (CDR (lst)));
}

object_t *lisp_value (object_t * lst)
{
  REQ (lst, 1, c_sym ("value"));
  if (!SYMBOLP (CAR (lst)))
    THROW (wrong_type, c_cons (c_sym ("value"), CAR (lst)));

  return UPREF (GET (CAR (lst)));
}

/* Predicates */

object_t *nullp (object_t * lst)
{
  REQ (lst, 1, c_sym ("nullp"));
  if (CAR (lst) == NIL)
    return T;
  return NIL;
}

object_t *funcp (object_t * lst)
{
  REQ (lst, 1, c_sym ("funcp"));
  if (FUNCP (CAR (lst)))
    return T;
  return NIL;
}

object_t *listp (object_t * lst)
{
  REQ (lst, 1, c_sym ("listp"));
  if (CAR (lst)->type == CONS || CAR (lst) == NIL)
    return T;
  return NIL;
}

object_t *symbolp (object_t * lst)
{
  REQ (lst, 1, c_sym ("symbolp"));
  if (CAR (lst)->type == SYMBOL)
    return T;
  return NIL;
}

object_t *nump (object_t * lst)
{
  REQ (lst, 1, c_sym ("nump"));
  if (CAR (lst)->type == INT || CAR (lst)->type == FLOAT)
    return T;
  return NIL;
}

object_t *stringp (object_t * lst)
{
  REQ (lst, 1, c_sym ("stringp"));
  if (CAR (lst)->type == STRING)
    return T;
  return NIL;
}

object_t *intp (object_t * lst)
{
  REQ (lst, 1, c_sym ("intp"));
  if (CAR (lst)->type == INT)
    return T;
  return NIL;
}

object_t *floatp (object_t * lst)
{
  REQ (lst, 1, c_sym ("floatp"));
  if (CAR (lst)->type == FLOAT)
    return T;
  return NIL;
}

/* Error handling */

object_t *throw (object_t * lst)
{
  THROW (UPREF (CAR (lst)), UPREF (CAR (CDR (lst))));
}

object_t *catch (object_t * lst)
{
  object_t *csym = eval (CAR (lst));
  CHECK (csym);
  object_t *body = CDR (lst);
  object_t *r = eval_body (body);
  if (r == err_symbol)
    {
      if (csym == err_thrown)
	{
	  obj_destroy (csym);
	  obj_destroy (err_thrown);
	  return err_attach;
	}
      else
	return err_symbol;
    }
  return r;
}

/* Installs all of the above functions. */
void lisp_init ()
{
  /* Maths */
  SSET (c_sym ("+"), c_cfunc (&addition));
  SSET (c_sym ("*"), c_cfunc (&multiply));
  SSET (c_sym ("-"), c_cfunc (&subtract));
  SSET (c_sym ("/"), c_cfunc (&divide));
  SSET (c_sym ("<"), c_cfunc (&less_than));
  SSET (c_sym ("<="), c_cfunc (&less_than_or_eq));
  SSET (c_sym (">"), c_cfunc (&greater_than));
  SSET (c_sym (">="), c_cfunc (&greater_than_or_eq));
  SSET (c_sym ("="), c_cfunc (&numeq));

  /* Various */
  SSET (c_sym ("quote"), c_special (&quote));
  SSET (c_sym ("lambda"), c_special (&lambda_f));
  SSET (c_sym ("defun"), c_special (&defun));
  SSET (c_sym ("defmacro"), c_special (&defmacro));
  SSET (c_sym ("car"), c_cfunc (&lisp_car));
  SSET (c_sym ("cdr"), c_cfunc (&lisp_cdr));
  SSET (c_sym ("list"), c_cfunc (&lisp_list));
  SSET (c_sym ("if"), c_special (&lisp_if));
  SSET (c_sym ("not"), c_cfunc (&nullp));
  SSET (c_sym ("progn"), c_special (&progn));
  SSET (c_sym ("let"), c_special (&let));
  SSET (c_sym ("while"), c_special (&lisp_while));
  SSET (c_sym ("eval"), c_cfunc (&eval_body));
  SSET (c_sym ("print"), c_cfunc (&lisp_print));
  SSET (c_sym ("cons"), c_cfunc (&lisp_cons));

  /* Symbol table */
  SSET (c_sym ("set"), c_cfunc (&lisp_set));
  SSET (c_sym ("value"), c_cfunc (&lisp_value));

  /* Equality */
  SSET (c_sym ("eq"), c_cfunc (&eq));
  SSET (c_sym ("eql"), c_cfunc (&eql));

  /* Predicates */
  SSET (c_sym ("nullp"), c_cfunc (&nullp));
  SSET (c_sym ("funcp"), c_cfunc (&funcp));
  SSET (c_sym ("listp"), c_cfunc (&listp));
  SSET (c_sym ("symbolp"), c_cfunc (&symbolp));
  SSET (c_sym ("stringp"), c_cfunc (&stringp));
  SSET (c_sym ("nump"), c_cfunc (&nump));
  SSET (c_sym ("intp"), c_cfunc (&intp));
  SSET (c_sym ("floatp"), c_cfunc (&floatp));

  /* Error handling */
  SSET (c_sym ("throw"), c_cfunc (&throw));
  SSET (c_sym ("catch"), c_special (&catch));
}
