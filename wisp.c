#include <stdio.h>
#include "common.h"
#include "cons.h"
#include "symtab.h"
#include "object.h"
#include "eval.h"

/* Initilize all the systems. */
void init ()
{
  /* These *must* be called in this order. */
  object_init ();
  symtab_init ();
  cons_init ();
  eval_init ();
}

object_t *setq (object_t * lst)
{
  object_t *v = CAR (lst);
  if (v->type != SYMBOL)
    {
      printf ("error: setq: not a symbol!\n");
      return NIL;
    }
  SET (v, CAR (CDR (lst)));
  return NIL;
}

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  init ();

  obj_print (cons
	     (c_sym ("defun"),
	      cons (c_str ("hello"), c_cons (c_int (10), NIL))));
  printf ("\n");

  object_t *str = c_str ("hello, world");
  obj_print (str);
  printf ("\n");
  SET (c_sym ("str"), str);
  obj_print (GET (c_sym ("str")));
  printf ("\n");

  /* install setq */
  object_t *setqo = obj_create (CFUNC);
  setqo->val = &setq;
  SET (c_sym ("setq"), setqo);

  object_t *tail = obj_create (CONS);
  CAR (tail) = c_int (102);
  object_t *lst = cons (c_sym ("setq"), cons (c_sym ("n"), tail));
  obj_print (lst);
  printf ("\n");
  eval (lst);
  obj_print (GET (c_sym ("n")));
  printf ("\n");
}
