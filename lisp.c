#include <stdio.h>
#include <string.h>
#include "object.h"
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"

object_t *addition (object_t * lst)
{
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
	accum += OINT ( CAR(p));
      p = CDR (p);
    }
  if (intmode)
    return c_int ((int) accum);
  return c_float ((double) accum);
}

object_t *quote (object_t * lst)
{
  return CAR (lst);
}

object_t *lambda_f (object_t * lst)
{
  return c_cons (lambda, lst);
}

object_t *defun (object_t * lst)
{
  object_t *f = c_cons (lambda, CDR (lst));
  SET (CAR (lst), f);
  return f;
}

object_t *lisp_cdr (object_t * lst)
{
  return CDR (CAR (lst));
}

object_t *lisp_car (object_t * lst)
{
  return CAR (CAR (lst));
}

object_t *lisp_list (object_t * lst)
{
  return lst;
}

object_t *lisp_if (object_t * lst)
{
  object_t *r = eval (CAR (lst));
  if (r != NIL)
    return eval (CAR (CDR (lst)));
  return eval_body (CDR (CDR (lst)));
}

object_t *progn (object_t * lst)
{
  return eval_body (lst);
}

object_t *eq (object_t * lst)
{
  object_t *a = CAR (lst);
  object_t *b = CAR (CDR (lst));
  if (a == b)
    return T;
  return NIL;
}

object_t *eql (object_t * lst)
{
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
    case CMACRO:
    case SPECIAL:
      if (a->val == b->val)
	return T;
      break;
    }
  return NIL;
}

object_t *nullp (object_t * lst)
{
  if (CAR (lst) == NIL)
    return T;
  return NIL;
}

object_t *funcp (object_t *lst)
{
  if (IS_FUNC (CAR (lst)))
    return T;
  return NIL;
}

object_t *listp (object_t *lst)
{
  if (CAR (lst)->type == CONS || CAR (lst) == NIL)
    return T;
  return NIL;
}

void lisp_init ()
{
  /* install cfuncs */
  SET (c_sym ("+"), c_cfunc (&addition));
  SET (c_sym ("quote"), c_cmacro (&quote));
  SET (c_sym ("lambda"), c_cmacro (&lambda_f));
  SET (c_sym ("defun"), c_cmacro (&defun));
  SET (c_sym ("car"), c_cfunc (&lisp_car));
  SET (c_sym ("cdr"), c_cfunc (&lisp_cdr));
  SET (c_sym ("list"), c_cfunc (&lisp_list));
  SET (c_sym ("if"), c_special (&lisp_if));
  SET (c_sym ("eq"), c_cfunc (&eq));
  SET (c_sym ("eql"), c_cfunc (&eql));
  SET (c_sym ("nullp"), c_cfunc (&nullp));
  SET (c_sym ("not"), c_cfunc (&nullp));
  SET (c_sym ("funcp"), c_cfunc (&funcp));
  SET (c_sym ("listp"), c_cfunc (&listp));
  SET (c_sym ("progn"), c_special (&progn));
}