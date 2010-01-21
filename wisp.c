#include <stdio.h>
#include "cons.h"
#include "symtab.h"
#include "common.h"
#include "eval.h"

/* Initilize all the systems. */
void init ()
{
  cons_init ();
  object_init ();
  symtab_init ();
}

object_t *setq(object_t *lst)
{
  object_t *v = car(lst);
  if (v->type != SYMBOL)
    {
      printf("error: setq: not a symbol!\n");
      return NULL;
    }
  set_value(v, car(cdr(lst)));
  return NULL;
}

int main (int argc, char **argv)
{
  (void) argc;
  (void) argv;
  init ();

  printf("%p\n", c_sym("symbol"));
  printf("%p\n", c_sym("symbol"));

  object_t *c = obj_create (CONS);
  setcar (c, c_int (10));
  obj_print (cons (c_sym ("defun"), cons (c_str ("hello"), c)));
  printf ("\n");

  object_t *str = c_str("hello, world");
  obj_print(str);
  printf("\n");
  set_value(c_sym ("str"), str);
  obj_print(get_value(c_sym ("str")));
  printf("\n");

  /* install setq */
  object_t *setqo = obj_create(CFUNC);
  setqo->val = &setq;
  set_value(c_sym("setq"), setqo);
  
  object_t *tail = obj_create(CONS);
  setcar (tail, c_int(102));
  object_t *lst = cons(c_sym("setq"), cons(c_sym("n"), tail));
  obj_print(lst);
  printf("\n");
  eval( lst );
  obj_print(get_value(c_sym("n")));
  printf("\n");
}
