#include <stdio.h>
#include "object.h"
#include "cons.h"
#include "symtab.h"
#include "common.h"

object_t *addition (object_t * lst)
{
  if (lst == NIL)
    return c_int (0);
  return c_int (OINT (CAR (lst)) + OINT (addition (CDR (lst))));
}

void lisp_init ()
{
  /* install cfuncs */
  SET (c_sym ("+"), c_cfunc (&addition));
}
