#include <stdio.h>
#include "object.h"
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"

object_t *addition (object_t * lst)
{
  if (lst == NIL)
    return c_int (0);
  return c_int (OINT (CAR (lst)) + OINT (addition (CDR (lst))));
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

void lisp_init ()
{
  /* install cfuncs */
  SET (c_sym ("+"), c_cfunc (&addition));
  SET (c_sym ("quote"), c_cmacro (&quote));
  SET (c_sym ("lambda"), c_cmacro (&lambda_f));
  SET (c_sym ("defun"), c_cmacro (&defun));
}
